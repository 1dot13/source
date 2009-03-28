/// \file
/// \brief In a fully connected mesh, determines which system can act as a host. This generally is the system that has been connected to the mesh the longest.
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

#ifndef __FCM_HOST_H
#define __FCM_HOST_H

#include "RakNetTypes.h"
#include "Export.h"
#include "PluginInterface.h"
#include "PacketPriority.h"
#include "DS_List.h"
#include "DS_Map.h"

class RakPeerInterface;
struct Packet;

typedef unsigned char FCMHostGroupID;

/// \defgroup FCM_HOST_GROUP FullyConnectedMeshHost
/// \ingroup PLUGINS_GROUP

/// \brief In a fully connected mesh of peers, allows all peers to agree that another peer is hot.
///
/// In peer to peer games it is often still useful to have one system act as a host / server
/// For example, if the game has AI, only the host / server should control the AI
/// This plugin automatically determines the host, changing the host as the prior host disconnects, and allowing all peers to agree on who is the host
///
/// Algorithm:
/// 1. Every system has a synchronized list of RakNetGuid, representing remote systems in the mesh
/// 2. The system at the head of the list is the host.
/// 3. New systems to join the mesh are added to the end of the list.
/// 4. To avoid contention, only the current host modifies or broadcasts the list. Modifications are broadcast to other systems.
/// 5. If the connection to the host is lost, the next connected system in the list becomes the host.
/// 6. If two meshes join each other, the host with the lower RakNetGuid takes precedence, and the lower priority mesh is appended.
///
/// \pre A fully connected mesh is required among the participants - e.g. all participants must eventually connect to all other participants. Otherwise the algorithm will not return the same host among all systems.
/// \note ID_FCM_HOST_CHANGED is returned to RakPeerInterface::Receive() every time the host changes.
/// \ingroup FCM_HOST_GROUP
class RAK_DLL_EXPORT FCMHost : public PluginInterface
{
public:
	/// Constructor
	FCMHost();

	/// Destructor
	virtual ~FCMHost();

	/// Add a connected system to the list of systems to manage host tracking
	/// \param[in] guid The system to add
	/// \param[in] groupId Which group to assign this system to. Groups are used if you want to manage more than one list at a time. Use 0 if you don't care.
	void AddParticipant(RakNetGUID guid, FCMHostGroupID groupId=0);

	/// Remove a participant added with AddParticipant(), or automatically added with SetAutoAddNewConnections(), from all groups
	/// \param[in] guid The system to remove
	void RemoveParticipantFromAllGroups(RakNetGUID guid);

	/// Remove a participant added with AddParticipant(), or automatically added with SetAutoAddNewConnections()
	/// \param[in] guid The system to remove
	/// \param[in] groupId Which group to remove this system from. Groups are used if you want to manage more than one list at a time. Use 0 if you don't care.
	void RemoveParticipant(RakNetGUID guid, FCMHostGroupID groupId=0);

	/// If set to true, all subsequent new connections will be added to \a groupId.
	/// \param[in] autoAdd If true, removes the need to call AddParticipant() on ID_NEW_INCOMING_CONNECTION or ID_CONNECTION_REQUEST_ACCEPTED
	/// Which group to automatically add systems to. Unused parameter if \a autoAdd is false
	void SetAutoAddNewConnections(bool autoAdd, FCMHostGroupID groupId=0);

	/// Get the current host. This will usually be the oldest system added with AddParticipant() or automatically added with SetAutoAddNewConnections()
	/// \param[in] groupId Which group we are referring to.
	/// \return A RakNetGuid representing which system should be the host. We are NOT necessarily connected to this system, if the mesh is not fully connected (yet?)
	RakNetGUID GetHost(FCMHostGroupID groupId=0) const;

	/// Convenience function to return if the attached instance of RakPeerInterface is the host
	/// \param[in] groupId Which group we are referring to.
	/// \return true if this system is the host, false otherwise.
	bool AmIHost(FCMHostGroupID groupId=0) const;

	/// Returns if we are connected to other systems, and one of those systems (possibly ourselves) is a host
	/// If we are not connected to other systems, then our own system is always the host, and in a sense the host is undetermined
	/// This function could also be named HasValidHost, or IsHostDetermined
	/// \param[in] groupId Which group we are referring to.
	/// \return true If there is a networked host
	bool HasConnectedHost(FCMHostGroupID groupId=0) const;

	/// Convenience function to return if the attached instance of RakPeerInterface is the host
	/// Differs from AmIHost in that if this system is the only system in the mesh, returns false
	/// \param[in] groupId Which group we are referring to.
	/// \return true if this system is the host, false otherwise.
	bool AmIConnectedHost(FCMHostGroupID groupId=0) const;

	/// \internal For plugin handling
	virtual void OnAttach(RakPeerInterface *peer);
	/// \internal For plugin handling
	virtual PluginReceiveResult OnReceive(RakPeerInterface *peer, Packet *packet);
	/// \internal For plugin handling
	virtual void OnShutdown(RakPeerInterface *peer);
	/// \internal For plugin handling
	virtual void OnCloseConnection(RakPeerInterface *peer, SystemAddress systemAddress);
protected:

	void Clear(void);

	void OnAddParticipantRequest(RakPeerInterface *peer, Packet *packet);
	void OnNewConnection(RakPeerInterface *peer, Packet *packet);
	void OnHostListUpdate(RakPeerInterface *peer, Packet *packet);
	void WriteHostListGroup(FCMHostGroupID groupId, RakNet::BitStream *bs);
	void ReadHostListGroup(RakNet::BitStream *bs, DataStructures::List<RakNetGUID> *theList);
	void BroadcastToGroup(FCMHostGroupID groupId, RakNet::BitStream *bs);
	void BroadcastGroupList(FCMHostGroupID groupId);
	void NotifyUserOfHost(FCMHostGroupID groupId, RakNetGUID guid);
	void RemoveParticipantFromGroup(RakNetGUID guid, FCMHostGroupID groupId);

	/// List of hosts, in a map of groupIDs
	DataStructures::Map<FCMHostGroupID, DataStructures::List<RakNetGUID>* > hostListsGroup;

	// All systems added with AddParticipant(). If we become the host, and a system is in this list and not in hostListsGroup,
	// Then we add it.
	DataStructures::Map<FCMHostGroupID, DataStructures::List<RakNetGUID>* > participantList;

	RakPeerInterface *rakPeer;

	bool autoAddConnections;
	FCMHostGroupID autoAddConnectionsTargetGroup;
};

#endif
