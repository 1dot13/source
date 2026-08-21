/* No-op Bink exports, linked straight into AddressSanitizer builds.
 *
 * Retail binkw32.dll is based at 0x30000000, which is asan's 32-bit Windows
 * shadow (kWindowsShadowOffset32 = 3<<28). Loading it aborts asan before
 * main(). Rather than ship a stub DLL, the asan build defines __RADINEXE__ so
 * bink.h declares these as plain in-exe functions (see sgp/RAD.H) instead of
 * dllimports, and compiles this file into the exe. Every call returns 0, so
 * BinkOpen fails and the game skips video. The asan exe imports no binkw32.dll
 * at all, so gamedir/binkw32.dll is never consulted or replaced. */

/* WIN32 GUI apps have no console, so asan's stderr report is lost. Route it to
 * gamedir/asan.report.<pid> instead. ASAN_OPTIONS overrides this. */
__declspec(dllexport) const char *__asan_default_options(void)
{
	return "log_path=asan.report";
}

typedef void *P;

#define S0(fn)  P __stdcall fn(void) { return 0; }
#define S1(fn)  P __stdcall fn(P a) { return 0; }
#define S2(fn)  P __stdcall fn(P a, P b) { return 0; }
#define S3(fn)  P __stdcall fn(P a, P b, P c) { return 0; }
#define S4(fn)  P __stdcall fn(P a, P b, P c, P d) { return 0; }
#define S5(fn)  P __stdcall fn(P a, P b, P c, P d, P e) { return 0; }
#define S7(fn)  P __stdcall fn(P a, P b, P c, P d, P e, P f, P g) { return 0; }
#define S11(fn) P __stdcall fn(P a, P b, P c, P d, P e, P f, P g, P h, P i, P j, P k) { return 0; }
#define S12(fn) P __stdcall fn(P a, P b, P c, P d, P e, P f, P g, P h, P i, P j, P k, P l) { return 0; }
#define S13(fn) P __stdcall fn(P a, P b, P c, P d, P e, P f, P g, P h, P i, P j, P k, P l, P m) { return 0; }
#define S14(fn) P __stdcall fn(P a, P b, P c, P d, P e, P f, P g, P h, P i, P j, P k, P l, P m, P n14) { return 0; }
#define S15(fn) P __stdcall fn(P a, P b, P c, P d, P e, P f, P g, P h, P i, P j, P k, P l, P m, P n14, P o) { return 0; }

S3(BinkBufferBlit)
S3(BinkBufferCheckWinPos)
S2(BinkBufferClear)
S1(BinkBufferClose)
S1(BinkBufferGetDescription)
S0(BinkBufferGetError)
S1(BinkBufferLock)
S4(BinkBufferOpen)
S2(BinkBufferSetDirectDraw)
S2(BinkBufferSetHWND)
S3(BinkBufferSetOffset)
S3(BinkBufferSetResolution)
S3(BinkBufferSetScale)
S1(BinkBufferUnlock)
S5(BinkCheckCursor)
S1(BinkClose)
S1(BinkCloseTrack)
S7(BinkCopyToBuffer)
S11(BinkCopyToBufferRect)
S1(BinkDDSurfaceType)
S1(BinkDX8SurfaceType)
S1(BinkDoFrame)
S0(BinkGetError)
S3(BinkGetKeyFrame)
S3(BinkGetRealtime)
S2(BinkGetRects)
S2(BinkGetSummary)
S2(BinkGetTrackData)
S2(BinkGetTrackID)
S2(BinkGetTrackMaxSize)
S2(BinkGetTrackType)
S3(BinkGoto)
S2(BinkIsSoftwareCursor)
S0(BinkLogoAddress)
S1(BinkNextFrame)
S2(BinkOpen)
S1(BinkOpenDirectSound)
S1(BinkOpenMiles)
S2(BinkOpenTrack)
S1(BinkOpenWaveOut)
S2(BinkPause)
S1(BinkRestoreCursor)
S1(BinkService)
S1(BinkSetError)
S2(BinkSetFrameRate)
S1(BinkSetIO)
S1(BinkSetIOSize)
S5(BinkSetMixBinVolumes)
S4(BinkSetMixBins)
S3(BinkSetPan)
S1(BinkSetSimulate)
S2(BinkSetSoundOnOff)
S2(BinkSetSoundSystem)
S2(BinkSetSoundTrack)
S2(BinkSetVideoOnOff)
S3(BinkSetVolume)
S1(BinkWait)
S2(RADSetMemory)
S0(RADTimerRead)
S13(YUV_blit_16a1bpp)
S15(YUV_blit_16a1bpp_mask)
S13(YUV_blit_16a4bpp)
S15(YUV_blit_16a4bpp_mask)
S12(YUV_blit_16bpp)
S14(YUV_blit_16bpp_mask)
S12(YUV_blit_24bpp)
S14(YUV_blit_24bpp_mask)
S12(YUV_blit_24rbpp)
S14(YUV_blit_24rbpp_mask)
S13(YUV_blit_32abpp)
S15(YUV_blit_32abpp_mask)
S12(YUV_blit_32bpp)
S14(YUV_blit_32bpp_mask)
S13(YUV_blit_32rabpp)
S15(YUV_blit_32rabpp_mask)
S12(YUV_blit_32rbpp)
S14(YUV_blit_32rbpp_mask)
S12(YUV_blit_UYVY)
S14(YUV_blit_UYVY_mask)
S12(YUV_blit_YUY2)
S14(YUV_blit_YUY2_mask)
S13(YUV_blit_YV12)
S1(YUV_init)
S1(radfree)
S1(radmalloc)
