#include "types.h"
#include <algorithm>
#include <array>
#include <cstring>
#include <optional>
#include <vector>

extern "C"
{
	#include "smacker.h"
}

#include "DEBUG.H"
#include "Cinematics.h"
#include "himage.h"
#include "local.h"
#include "soundman.h"
#include "vobject.h"
#include "vsurface.h"
#include <vfs/Core/vfs.h>
#include <vfs/Core/vfs_file_raii.h>

static constexpr UINT32 MAX_FLICS        = 4;	// How many flics can be open at once
static constexpr UINT32 NUM_AUDIO_TRACKS = 7;	// Audio tracks an SMK file can carry
static constexpr UINT32 WAV_HEADER_SIZE  = 44;	// Header we prepend to the decoded audio

struct SMKFLIC {
  // libsmacker decoder; null means the slot is free
  smk decoder = nullptr;
  bool playing = false;
  // Close the flic once it has played to the end
  bool autoClose = false;
  // Where on the screen the flic is blitted
  UINT32 left = 0, top = 0;
  // Tick count the flic started playing at
  UINT32 startTime = 0;
  // Frame the decoder is on
  UINT32 frame = 0;
  double millisecondsPerFrame = 0.0;
  // Last result of smk_first()/smk_next()
  char status = SMK_DONE;

  // WAV header followed by the decoded audio
  std::array<std::vector<UINT8>, NUM_AUDIO_TRACKS> audio;
  // SOUND_ERROR when the track isn't playing
  std::array<UINT32, NUM_AUDIO_TRACKS> soundId;
};

static std::array<SMKFLIC, MAX_FLICS> flics;

static auto SmkGetFreeFlic() -> SMKFLIC*;
static auto SmkReadFile(const CHAR8 *filename) -> std::optional<std::vector<vfs::Byte>>;
static auto SmkDecodeAudio(SMKFLIC &flic) -> void;
static auto SmkStartAudio(SMKFLIC &flic, const CHAR8 *filename) -> void;
static auto SmkWriteWavHeader(UINT8 *wav, UINT32 pcmSize, UINT8 channels, UINT8 bitDepth, UINT32 rate) -> void;
static auto SmkAdvanceToCurrentFrame(SMKFLIC &flic) -> void;
static auto SmkBlitFrame(SMKFLIC &flic) -> void;


BOOLEAN SmkPollFlics()
{
	bool anyPlaying = false;

	for (SMKFLIC &flic : flics)
	{
		if (!flic.playing)
			continue;

		SmkAdvanceToCurrentFrame(flic);

		if (flic.status == SMK_DONE || flic.status == SMK_ERROR)
		{
			if (flic.autoClose)
				SmkCloseFlic(&flic);
			else
				flic.playing = false;
		}
		else
		{
			anyPlaying = true;
			SmkBlitFrame(flic);
		}
	}

	return anyPlaying;
}

void SmkInitialize()
{
	for (SMKFLIC &flic : flics)
	{
		flic.decoder = nullptr;
		flic.playing = false;
		flic.autoClose = false;
		flic.soundId.fill(SOUND_ERROR);
	}
}

void SmkShutdown()
{
	// Close and deallocate any open flics
	for (SMKFLIC &flic : flics)
		SmkCloseFlic(&flic);
}

SMKFLIC *SmkPlayFlic(const CHAR8 *filename, UINT32 left, UINT32 top, BOOLEAN autoClose)
{
	SMKFLIC *flic = SmkGetFreeFlic();
	if (flic == nullptr)
	{
		ErrorMsg("SMK ERROR: Out of flic slots, cannot open another");
		return nullptr;
	}

	std::optional<std::vector<vfs::Byte>> fileData = SmkReadFile(filename);
	if (!fileData)
		return nullptr;

	// libsmacker copies the compressed frames it needs out of the buffer, so
	// fileData can go away again as soon as the file is open
	flic->decoder = smk_open_memory(reinterpret_cast<const unsigned char *>(fileData->data()), static_cast<unsigned long>(fileData->size()));
	if (flic->decoder == nullptr)
	{
		ErrorMsg("SMK ERROR: libsmacker won't open the SMK file");
		return nullptr;
	}

	// The slot is now taken (decoder != nullptr) and ready to go
	flic->left = left;
	flic->top = top;

	double microsecondsPerFrame = 0.0;
	if (smk_info_all(flic->decoder, nullptr, nullptr, &microsecondsPerFrame) < 0 || microsecondsPerFrame <= 0.0)
	{
		ErrorMsg("SMK ERROR: Can't read the playback speed of the SMK file");
		SmkCloseFlic(flic);
		return nullptr;
	}
	// The video plays too slowly at microsecond resolution, speed it up by
	// rounding down to milliseconds
	flic->millisecondsPerFrame = microsecondsPerFrame / 1000.0;

	SmkDecodeAudio(*flic);

	// Rewind, this time to decode the picture
	if (smk_enable_all(flic->decoder, SMK_VIDEO_TRACK) < 0)
	{
		ErrorMsg("SMK ERROR: Can't enable the video track of the SMK file");
		SmkCloseFlic(flic);
		return nullptr;
	}
	flic->status = smk_first(flic->decoder);
	flic->frame = 0;
	flic->startTime = GetTickCount();

	SmkStartAudio(*flic, filename);

	// We're now playing, flag the flic for the poller to update
	flic->playing = true;
	flic->autoClose = autoClose != FALSE;

	return flic;
}

void SmkCloseFlic(SMKFLIC *flic)
{
	// SmkPollFlics() closes flics that have played to the end by itself, so the
	// caller may well be handing us a slot that is already free
	if (flic->decoder == nullptr)
		return;

	for (UINT32 track = 0; track < NUM_AUDIO_TRACKS; ++track)
	{
		// The sound was streaming straight out of the audio buffer, so it has to
		// be stopped before that buffer is released
		if (flic->soundId[track] != SOUND_ERROR)
		{
			SoundStop(flic->soundId[track]);
			flic->soundId[track] = SOUND_ERROR;
		}
		flic->audio[track] = {};
	}

	smk_close(flic->decoder);
	flic->decoder = nullptr;
	flic->playing = false;
	flic->autoClose = false;
}

static SMKFLIC *SmkGetFreeFlic()
{
	for (SMKFLIC &flic : flics)
		if (flic.decoder == nullptr)
			return &flic;

	return nullptr;
}

//	Reads a video file out of the VFS in one go. libsmacker can only read from a
//	plain file or from memory, and the file may well be sitting inside an archive.
static std::optional<std::vector<vfs::Byte>> SmkReadFile(const CHAR8 *filename)
{
	vfs::Path videoName(filename);
	std::vector<vfs::Byte> data;

	try
	{
		if (!getVFS()->fileExists(videoName))
			return std::nullopt;

		vfs::COpenReadFile file(videoName);
		vfs::size_t size = file->getSize();
		if (size == 0)
			return std::nullopt;

		data.resize(size);
		file->read(data.data(), size);
	}
	catch (std::exception &ex)
	{
		SGP_RETHROW(_BS(L"Video file \"") << videoName << L"\" could not be read" << _BS::wget, ex);
	}

	return data;
}

//	Decodes the audio of the whole file into flic.audio, each track preceded by a
//	WAV header. Unlike SMACKW32.DLL, libsmacker hands out audio a frame at a time
//	and leaves playing it to us, so it all has to be collected up front.
static void SmkDecodeAudio(SMKFLIC &flic)
{
	unsigned char						trackMask = 0;
	std::array<unsigned char, NUM_AUDIO_TRACKS>	channels{};
	std::array<unsigned char, NUM_AUDIO_TRACKS>	bitDepth{};
	std::array<unsigned long, NUM_AUDIO_TRACKS>	rate{};

	if (smk_info_audio(flic.decoder, &trackMask, channels.data(), bitDepth.data(), rate.data()) < 0 || trackMask == 0)
		return;

	// Decoding the audio means running through the entire file. Leave the video
	// track disabled so that this pass doesn't decode the picture as well.
	if (smk_enable_all(flic.decoder, trackMask) < 0)
		return;

	// Leave room for the WAV header, which can only be written once the size of
	// the track is known
	for (UINT32 track = 0; track < NUM_AUDIO_TRACKS; ++track)
		if (trackMask & (1 << track))
			flic.audio[track].resize(WAV_HEADER_SIZE);

	for (char status = smk_first(flic.decoder); status != SMK_DONE; status = smk_next(flic.decoder))
	{
		if (status == SMK_ERROR)
		{
			ErrorMsg("SMK ERROR: libsmacker failed to decode the audio of the SMK file");
			break;
		}

		for (UINT32 track = 0; track < NUM_AUDIO_TRACKS; ++track)
		{
			if (!(trackMask & (1 << track)))
				continue;

			unsigned long size = smk_get_audio_size(flic.decoder, static_cast<unsigned char>(track));
			if (size == 0)
				continue;

			const UINT8 *pcm = smk_get_audio(flic.decoder, static_cast<unsigned char>(track));
			flic.audio[track].insert(flic.audio[track].end(), pcm, pcm + size);
		}
	}

	for (UINT32 track = 0; track < NUM_AUDIO_TRACKS; ++track)
	{
		std::vector<UINT8> &trackAudio = flic.audio[track];

		if (trackAudio.size() <= WAV_HEADER_SIZE)
		{
			// Track carries no audio after all
			trackAudio = {};
			continue;
		}

		SmkWriteWavHeader(trackAudio.data(), static_cast<UINT32>(trackAudio.size()) - WAV_HEADER_SIZE,
			channels[track], bitDepth[track], static_cast<UINT32>(rate[track]));
	}
}

static void SmkStartAudio(SMKFLIC &flic, const CHAR8 *filename)
{
	SOUNDPARMS params;
	memset(&params, 0xff, sizeof(params));	// 0xffffffff in every field means "use the default"
	params.uiLoop = 1;

	for (UINT32 track = 0; track < NUM_AUDIO_TRACKS; ++track)
	{
		if (flic.audio[track].empty())
			continue;

		flic.soundId[track] = SoundPlayFromBuffer(filename, flic.audio[track].data(),
			static_cast<UINT32>(flic.audio[track].size()), &params);
	}
}

//	Writes a canonical 44 byte WAV header for uncompressed PCM, so that the audio
//	we decoded can be handed to the sound module as if it came from a sound file.
static void SmkWriteWavHeader(UINT8 *wav, UINT32 pcmSize, UINT8 channels, UINT8 bitDepth, UINT32 rate)
{
	const UINT16 blockAlign = static_cast<UINT16>(channels * (bitDepth / 8));

	memcpy(wav +  0, "RIFF", 4);
	*reinterpret_cast<UINT32 *>(wav +  4) = 36 + pcmSize;			// size of everything after this field
	memcpy(wav +  8, "WAVEfmt ", 8);
	*reinterpret_cast<UINT32 *>(wav + 16) = 16;						// size of the fmt chunk
	*reinterpret_cast<UINT16 *>(wav + 20) = 1;						// format: uncompressed PCM
	*reinterpret_cast<UINT16 *>(wav + 22) = channels;
	*reinterpret_cast<UINT32 *>(wav + 24) = rate;
	*reinterpret_cast<UINT32 *>(wav + 28) = rate * blockAlign;		// byte rate
	*reinterpret_cast<UINT16 *>(wav + 32) = blockAlign;
	*reinterpret_cast<UINT16 *>(wav + 34) = bitDepth;
	memcpy(wav + 36, "data", 4);
	*reinterpret_cast<UINT32 *>(wav + 40) = pcmSize;
}

//	Decodes forward until the frame that should be on screen by now. SMACKW32.DLL
//	kept the playback clock itself, libsmacker leaves that to the caller.
static void SmkAdvanceToCurrentFrame(SMKFLIC &flic)
{
	UINT32 milliseconds = GetTickCount() - flic.startTime;
	UINT32 targetFrame = static_cast<UINT32>(milliseconds / flic.millisecondsPerFrame);

	while (flic.status != SMK_DONE && flic.status != SMK_ERROR && flic.frame != targetFrame)
	{
		flic.status = smk_next(flic.decoder);
		++flic.frame;
	}
}

static void SmkBlitFrame(SMKFLIC &flic)
{
	// TODO: the Y scale modes (SMK_FLAG_Y_INTERLACE, SMK_FLAG_Y_DOUBLE) are not
	// handled, none of the videos that ship with the game use them
	const UINT8 *source = smk_get_video(flic.decoder);
	if (source == nullptr)
		return;
	const UINT8 *sourcePalette = smk_get_palette(flic.decoder);
	if (sourcePalette == nullptr)
		return;
	unsigned long sourceWidth, sourceHeight;
	if (smk_info_video(flic.decoder, &sourceWidth, &sourceHeight, nullptr) < 0)
		return;

	UINT16 palette[256];
	for (UINT32 i = 0; i < 256; ++i)
		palette[i] = Get16BPPColor(FROMRGB(sourcePalette[i * 3], sourcePalette[i * 3 + 1], sourcePalette[i * 3 + 2]));

	UINT32 destPitch;
	UINT16 *dest = reinterpret_cast<UINT16 *>(LockVideoSurface(FRAME_BUFFER, &destPitch));
	if (dest == nullptr)
		return;
	destPitch /= sizeof(UINT16);	// pitch in pixels rather than bytes

	// Blit the part of the frame that lands on the screen
	// std::min is parenthesised to keep windows.h's min() macro off it
	unsigned long columns = flic.left >= static_cast<UINT32>(SCREEN_WIDTH)  ? 0 : (std::min)(sourceWidth,  static_cast<unsigned long>(SCREEN_WIDTH  - flic.left));
	unsigned long rows    = flic.top  >= static_cast<UINT32>(SCREEN_HEIGHT) ? 0 : (std::min)(sourceHeight, static_cast<unsigned long>(SCREEN_HEIGHT - flic.top));

	dest += flic.left + flic.top * destPitch;
	for (unsigned long y = 0; y < rows; ++y)
	{
		for (unsigned long x = 0; x < columns; ++x)
			dest[x] = palette[source[x]];

		dest += destPitch;
		source += sourceWidth;
	}

	UnLockVideoSurface(FRAME_BUFFER);
}
