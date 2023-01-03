/// \file
/// \brief This will write all incoming and outgoing network messages to the local console screen.  See derived functions for other outputs
///
/// This file is part of RakNet Copyright 2003 Kevin Jenkins.
///
/// Usage of RakNet is subject to the appropriate license agreement.
/// Creative Commons Licensees are subject to the
/// license found at
/// http://creativecommons.org/licenses/by-nc/2.5/
/// Single application licensees are subject to the license found at
/// http://www.jenkinssoftware.com/SingleApplicationLicense.html
/// Custom license users are subject to the terms therein.
/// GPL license users are subject to the GNU General Public
/// License as published by the Free
/// Software Foundation; either version 2 of the License, or (at your
/// option) any later version.

#ifndef __PACKET_LOGGER_H
#define __PACKET_LOGGER_H

class RakPeerInterface;
#include "RakNetTypes.h"
#include "PluginInterface.h"
#include "Export.h"

/// \defgroup PACKETLOGGER_GROUP PacketLogger
/// \ingroup PLUGINS_GROUP

/// \brief Writes incoming and outgoing messages to the screen.
/// This will write all incoming and outgoing messages to the console window, or to a file if you override it and give it this functionality.
/// \ingroup PACKETLOGGER_GROUP
class RAK_DLL_EXPORT PacketLogger : public PluginInterface
{
public:
	PacketLogger();
	virtual ~PacketLogger();

	virtual void OnAttach(RakPeerInterface *peer);

	virtual void Update(RakPeerInterface *peer);

	// Translate the supplied parameters into an output line - overloaded version that takes a MessageIdentifier
	// and translates it into a string (numeric or textual representation based on printId); this calls the
	// second version which takes a const char* argument for the messageIdentifier
	virtual void FormatLine(char* into, const char* dir, const char* type, unsigned int packet, unsigned int frame
		, unsigned char messageIdentifier, const BitSize_t bitLen, unsigned long long time, const SystemAddress& local, const SystemAddress& remote,
		unsigned int splitPacketId, unsigned int splitPacketIndex, unsigned int splitPacketCount, unsigned int orderingIndex);
	virtual void FormatLine(char* into, const char* dir, const char* type, unsigned int packet, unsigned int frame
		, const char* idToPrint, const BitSize_t bitLen, unsigned long long time, const SystemAddress& local, const SystemAddress& remote,
		unsigned int splitPacketId, unsigned int splitPacketIndex, unsigned int splitPacketCount, unsigned int orderingIndex);

	/// Events on low level sends and receives.  These functions may be called from different threads at the same time.
	virtual void OnDirectSocketSend(const char *data, const BitSize_t bitsUsed, SystemAddress remoteSystemAddress);
	virtual void OnDirectSocketReceive(const char *data, const BitSize_t bitsUsed, SystemAddress remoteSystemAddress);
	virtual void OnInternalPacket(InternalPacket *internalPacket, unsigned frameNumber, SystemAddress remoteSystemAddress, RakNetTime time, bool isSend);

	/// Logs out a header for all the data
	virtual void LogHeader(void);

	/// Override this to log strings to wherever.  Log should be threadsafe
	virtual void WriteLog(const char *str);

	// Set to true to print ID_* instead of numbers
	virtual void SetPrintID(bool print);
	// Print or hide acks (clears up the screen not to print them but is worse for debugging)
	virtual void SetPrintAcks(bool print);

	/// Prepend this string to output logs.
	virtual void SetPrefix(const char *_prefix);

	/// Append this string to output logs. (newline is useful here)
	virtual void SetSuffix(const char *_suffix);
	static const char* BaseIDTOString(unsigned char Id);
protected:
	const char* IDTOString(unsigned char Id);
	virtual void AddToLog(const char *str);
	// Users should override this
	virtual const char* UserIDTOString(unsigned char Id);

	RakPeerInterface *rakPeer;
	bool printId, printAcks;
	char prefix[256];
	char suffix[256];
};

#endif
