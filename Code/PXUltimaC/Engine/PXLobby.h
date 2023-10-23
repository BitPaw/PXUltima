#ifndef PXLobbyInclude
#define PXLobbyInclude

#include <Media/PXResource.h>
#include <OS/Error/PXActionResult.h>

#ifdef __cplusplus
extern "C"
{
#endif

	typedef enum PXLobbyType_
	{
		PXLobbyTypeInvalid,
		PXLobbyTypeServerList,
		PXLobbyTypeGameList
	}
	PXLobbyType;

	typedef enum PXLobbyPlayerType_
	{
		PXLobbyPlayerTypeInvalid,
		PXLobbyPlayerTypePlayer,
		PXLobbyPlayerTypeBot
	}
	PXLobbyPlayerType;

	typedef enum PXLobbyUserActiveState_
	{
		PXLobbyUserActiveStateInvalid,
		PXLobbyUserActiveStateOffline,			// friend is not currently logged on
		PXLobbyUserActiveStateOnline,			// friend is logged on
		PXLobbyUserActiveStateBusy,			// user is on, but busy
		PXLobbyUserActiveStateAway,			// auto-away feature
		PXLobbyUserActiveStateSnooze,			// auto-away for a long time
		PXLobbyUserActiveStateLookingToTrade,	// Online, trading
		PXLobbyUserActiveStateLookingToPlay,	// Online, wanting to play
		PXLobbyUserActiveStateInvisible		// Online, but appears offline to friends.  This status is never published to clients.
	}
	PXLobbyUserActiveState;

	typedef enum PXLobbyUserFriendshipStatus_
	{
		PXLobbyUserFriendshipStatusInvalid,
		PXLobbyUserFriendshipStatusStanger,
		PXLobbyUserFriendshipStatusBlocked,			// this doesn't get stored; the user has just done an Ignore on an friendship invite
		PXLobbyUserFriendshipStatusRecipient,
		PXLobbyUserFriendshipStatusFriend,
		PXLobbyUserFriendshipStatusRequestInitiator,
		PXLobbyUserFriendshipStatusIgnored,			// this is stored; the user has explicit blocked this other user from comments/chat/etc
		PXLobbyUserFriendshipStatusIgnoredFriend,
		PXLobbyUserFriendshipStatusSuggested_DEPRECATED,		// was used by the original implementation of the facebook linking feature, but now unused.
	}
	PXLobbyUserFriendshipStatus;

	typedef struct PXLobbyPlayer_
	{
		PXInt64U ID; // Unique ID for the user.
	
		PXInt32U Level; // Used in Steam as the account Level

		char Name[64]; // Username, can be changed at will.
		PXSize NameSize; // Username size

		PXLobbyUserActiveState ActiveState;
		PXLobbyUserFriendshipStatus FriendshipStatus;

	}
	PXLobbyPlayer;

	typedef struct PXLobby_
	{
		PXLobbyType Type;

		// List of clients/players/server
	}
	PXLobby;

	typedef PXBool (PXAPI* PXLobbyCreate)(void* const owner);
	typedef PXBool (PXAPI* PXLobbyJoin)(void* const owner);
	typedef PXBool (PXAPI* PXLobbyDestroy)(void* const owner);

	typedef PXBool(PXAPI* PXLobbyDestroy)(void* const owner);

	// For non dedicated servers, promote player to host the server.
	// if you dont have permission, you start a vote.
	typedef PXActionResult(PXAPI* PXLobbyUserPromoteToHost)(void* const pxSteam);
	typedef PXActionResult(PXAPI* PXLobbyUserRename)(void* const pxSteam);
	typedef PXActionResult(PXAPI* PXLobbyUserKick)(void* const pxSteam);
	typedef PXActionResult(PXAPI* PXLobbyUserBan)(void* const pxSteam);

	typedef PXActionResult(PXAPI* PXLobbyUserMe)(void* const pxSteam);
	typedef PXActionResult(PXAPI* PXLobbyUserListAll)(void* const pxSteam);

	typedef PXActionResult(PXAPI* PXLobbyUserProfileIconFetch)(void* const pxSteam, PXImage* const image);



	typedef PXActionResult(PXAPI* PXLobbyUserFriendGroupListAmount)(void* const pxSteam);
	typedef PXActionResult(PXAPI* PXLobbyUserFriendGroupListSingle)(void* const pxSteam);
	typedef PXActionResult(PXAPI* PXLobbyUserFriendGroupListAll)(void* const pxSteam);

	typedef PXActionResult(PXAPI* PXLobbyUserFriendClanListAmount)(void* const pxSteam);
	typedef PXActionResult(PXAPI* PXLobbyUserFriendClanListSingle)(void* const pxSteam);
	typedef PXActionResult(PXAPI* PXLobbyUserFriendClanListAll)(void* const pxSteam);


#if 0

	// User is in a game pressing the talk button (will suppress the microphone for all voice comms from the Steam friends UI)
	PXSteamPublic void PXSteamSetInGameVoiceSpeaking(PXSteam* const pxSteam, const PXSteamID pxSteamID, bool bSpeaking);

	// activates the game overlay, with an optional dialog to open 
	// valid options include "Friends", "Community", "Players", "Settings", "OfficialGameGroup", "Stats", "Achievements",
	// "chatroomgroup/nnnn"
	PXSteamPublic void PXSteamActivateGameOverlay(PXSteam* const pxSteam, const char* pchDialog);

	// activates game overlay to a specific place
	// valid options are
	//		"steamid" - opens the overlay web browser to the specified user or groups profile
	//		"chat" - opens a chat window to the specified user, or joins the group chat 
	//		"jointrade" - opens a window to a Steam Trading session that was started with the ISteamEconomy/StartTrade Web API
	//		"stats" - opens the overlay web browser to the specified user's stats
	//		"achievements" - opens the overlay web browser to the specified user's achievements
	//		"friendadd" - opens the overlay in minimal mode prompting the user to add the target user as a friend
	//		"friendremove" - opens the overlay in minimal mode prompting the user to remove the target friend
	//		"friendrequestaccept" - opens the overlay in minimal mode prompting the user to accept an incoming friend invite
	//		"friendrequestignore" - opens the overlay in minimal mode prompting the user to ignore an incoming friend invite
	PXSteamPublic void PXSteamActivateGameOverlayToUser(PXSteam* const pxSteam, const char* pchDialog, const PXSteamID steamID);

	// activates game overlay web browser directly to the specified URL
	// full address with protocol type is required, e.g. http://www.steamgames.com/
	PXSteamPublic void ActivateGameOverlayToWebPage(PXSteam* const pxSteam, const char* pchURL, PXActivateGameOverlayToWebPageMode eMode);

	// activates game overlay to store page for app
	PXSteamPublic void PXSteamActivateGameOverlayToStore(PXSteam* const pxSteam, unsigned int nAppID, PXSteamOverlayToStoreFlag eFlag);

	// Mark a target user as 'played with'. This is a client-side only feature that requires that the calling user is in game 
	PXSteamPublic void PXSteamSetPlayedWith(PXSteam* const pxSteam, const PXSteamID pxSteamIDPlayedWith);

	// activates game overlay to open the invite dialog. Invitations will be sent for the provided lobby.
	PXSteamPublic void PXSteamActivateGameOverlayInviteDialog(PXSteam* const pxSteam, const PXSteamID steamIDLobby);








	// requests information about a user - persona name & avatar
	// if bRequireNameOnly is set, then the avatar of a user isn't downloaded 
	// - it's a lot slower to download avatars and churns the local cache, so if you don't need avatars, don't request them
	// if returns true, it means that data is being requested, and a PersonaStateChanged_t callback will be posted when it's retrieved
	// if returns false, it means that we already have all the details about that user, and functions can be called immediately
	PXSteamPublic PXSteamBool PXSteamRequestUserInformation(PXSteam* const pxSteam, const PXSteamID pxSteamID, const PXSteamBool bRequireNameOnly);

	// requests information about a clan officer list
	// when complete, data is returned in ClanOfficerListResponse_t call result
	// this makes available the calls below
	// you can only ask about clans that a user is a member of
	// note that this won't download avatars automatically; if you get an officer,
	// and no avatar image is available, call RequestUserInformation( steamID, false ) to download the avatar
	PXSteamPublic __int64 PXSteamRequestClanOfficerList(PXSteam* const pxSteam, const PXSteamID pxSteamID);

	// iteration of clan officers - can only be done when a RequestClanOfficerList() call has completed

	// returns the steamID of the clan owner
	PXSteamPublic PXSteamID PXSteamGetClanOwner(PXSteam* const pxSteam, const PXSteamID pxSteamID);
	// returns the number of officers in a clan (including the owner)
	PXSteamPublic int PXSteamGetClanOfficerCount(PXSteam* const pxSteam, const PXSteamID pxSteamID);
	// returns the steamID of a clan officer, by index, of range [0,GetClanOfficerCount)
	PXSteamPublic PXSteamID PXSteamGetClanOfficerByIndex(PXSteam* const pxSteam, const PXSteamID pxSteamID, int iOfficer);
	// if current user is chat restricted, he can't send or receive any text/voice chat messages.
	// the user can't see custom avatars. But the user can be online and send/recv game invites.
	// a chat restricted user can't add friends or join any groups.
	PXSteamPublic unsigned int PXSteamGetUserRestrictions(PXSteam* const pxSteam);

	// Rich Presence data is automatically shared between friends who are in the same game
	// Each user has a set of Key/Value pairs
	// Note the following limits: k_cchMaxRichPresenceKeys, k_cchMaxRichPresenceKeyLength, k_cchMaxRichPresenceValueLength
	// There are five magic keys:
	//		"status"  - a UTF-8 string that will show up in the 'view game info' dialog in the Steam friends list
	//		"connect" - a UTF-8 string that contains the command-line for how a friend can connect to a game
	//		"steam_display"				- Names a rich presence localization token that will be displayed in the viewing user's selected language
	//									  in the Steam client UI. For more info: https://partner.steamgames.com/doc/api/ISteamFriends#richpresencelocalization
	//		"steam_player_group"		- When set, indicates to the Steam client that the player is a member of a particular group. Players in the same group
	//									  may be organized together in various places in the Steam UI.
	//		"steam_player_group_size"	- When set, indicates the total number of players in the steam_player_group. The Steam client may use this number to
	//									  display additional information about a group when all of the members are not part of a user's friends list.
	// GetFriendRichPresence() returns an empty string "" if no value is set
	// SetRichPresence() to a NULL or an empty string deletes the key
	// You can iterate the current set of keys for a friend with GetFriendRichPresenceKeyCount()
	// and GetFriendRichPresenceKeyByIndex() (typically only used for debugging)
	PXSteamPublic PXSteamBool PXSteamRichPresenceChange(PXSteam* const pxSteam, const char* pchKey, const char* pchValue);
	PXSteamPublic void PXSteamClearRichPresence(PXSteam* const pxSteam);
	PXSteamPublic PXSteamBool PXSteamFriendRichPresenceFetch(PXSteam* const pxSteam, const PXSteamID steamIDFriend, const char* pchKey, void* const outputBuffer, const unsigned int outputBufferSize);
	PXSteamPublic int PXSteamFriendRichPresenceKeyCountFetch(PXSteam* const pxSteam, const PXSteamID steamIDFriend, void* const outputBuffer, const unsigned int outputBufferSize);
	PXSteamPublic PXSteamBool PXSteamFriendRichPresenceKeyByIndex(PXSteam* const pxSteam, const PXSteamID steamIDFriend, int iKey);
	// Requests rich presence for a specific user.
	PXSteamPublic void PXSteamRequestFriendRichPresence(PXSteam* const pxSteam, const PXSteamID steamIDFriend);

	// Rich invite support.
	// If the target accepts the invite, a GameRichPresenceJoinRequested_t callback is posted containing the connect string.
	// (Or you can configure your game so that it is passed on the command line instead.  This is a deprecated path; ask us if you really need this.)
	PXSteamPublic PXSteamBool PXSteamUserInviteToGame(PXSteam* const pxSteam, const PXSteamID steamIDFriend, const char* pchConnectString);

	// recently-played-with friends iteration
	// this iterates the entire list of users recently played with, across games
	// GetFriendCoplayTime() returns as a unix time
	PXSteamPublic int PXSteamGetCoplayFriendCount(PXSteam* const pxSteam);
	PXSteamPublic PXSteamID PXSteamGetCoplayFriend(PXSteam* const pxSteam, int iCoplayFriend);
	PXSteamPublic int PXSteamGetFriendCoplayTime(PXSteam* const pxSteam, const PXSteamID steamIDFriend);
	PXSteamPublic unsigned int PXSteamGetFriendCoplayGame(PXSteam* const pxSteam, const PXSteamID steamIDFriend);

	// chat interface for games
	// this allows in-game access to group (clan) chats from in the game
	// the behavior is somewhat sophisticated, because the user may or may not be already in the group chat from outside the game or in the overlay
	// use ActivateGameOverlayToUser( "chat", steamIDClan ) to open the in-game overlay version of the chat
	PXSteamPublic __int64 PXSteamJoinClanChatRoom(PXSteam* const pxSteam, const PXSteamID pxSteamID);
	PXSteamPublic PXSteamBool PXSteamLeaveClanChatRoom(PXSteam* const pxSteam, const PXSteamID pxSteamID);
	PXSteamPublic int PXSteamGetClanChatMemberCount(PXSteam* const pxSteam, const PXSteamID pxSteamID);
	PXSteamPublic PXSteamID PXSteamGetChatMemberByIndex(PXSteam* const pxSteam, const PXSteamID pxSteamID, int iUser);
	PXSteamPublic PXSteamBool PXSteamSendClanChatMessage(PXSteam* const pxSteam, const PXSteamID pxSteamIDChat, const char* pchText);
	PXSteamPublic int PXSteamGetClanChatMessage(PXSteam* const pxSteam, const PXSteamID pxSteamIDChat, int iMessage, void* prgchText, int cchTextMax, PXSteamChatEntryType* peChatEntryType, PXSteamID* psteamidChatter);
	PXSteamPublic PXSteamBool PXSteamIsClanChatAdmin(PXSteam* const pxSteam, const PXSteamID pxSteamIDChat, const PXSteamID pxSteamID);

	// interact with the Steam (game overlay / desktop)
	PXSteamPublic PXSteamBool PXSteamIsClanChatWindowOpenInSteam(PXSteam* const pxSteam, const PXSteamID pxSteamIDChat);
	PXSteamPublic PXSteamBool PXSteamOpenClanChatWindowInSteam(PXSteam* const pxSteam, const PXSteamID pxSteamIDChat);
	PXSteamPublic PXSteamBool PXSteamCloseClanChatWindowInSteam(PXSteam* const pxSteam, const PXSteamID pxSteamIDChat);

	// peer-to-peer chat interception
	// this is so you can show P2P chats inline in the game
	PXSteamPublic PXSteamBool PXSteamSetListenForFriendsMessages(PXSteam* const pxSteam, bool bInterceptEnabled);
	PXSteamPublic PXSteamBool PXSteamReplyToFriendMessage(PXSteam* const pxSteam, const PXSteamID steamIDFriend, const char* pchMsgToSend);
	PXSteamPublic int PXSteamGetFriendMessage(PXSteam* const pxSteam, const PXSteamID steamIDFriend, int iMessageID, void* pvData, int cubData, PXSteamChatEntryType* peChatEntryType);

	// following apis

	PXSteamPublic __int64 PXSteamGetFollowerCount(PXSteam* const pxSteam, const PXSteamID steamID);
	PXSteamPublic __int64 PXSteamIsFollowing(PXSteam* const pxSteam, const PXSteamID steamID);
	PXSteamPublic __int64 PXSteamEnumerateFollowingList(PXSteam* const pxSteam, unsigned int unStartIndex);

	PXSteamPublic PXSteamBool PXSteamIsClanPublic(PXSteam* const pxSteam, const PXSteamID pxSteamID);
	PXSteamPublic PXSteamBool PXSteamIsClanOfficialGameGroup(PXSteam* const pxSteam, const PXSteamID pxSteamID);

	/// Return the number of chats (friends or chat rooms) with unread messages.
	/// A "priority" message is one that would generate some sort of toast or
	/// notification, and depends on user settings.
	///
	/// You can register for UnreadChatMessagesChanged_t callbacks to know when this
	/// has potentially changed.
	PXSteamPublic int PXSteamGetNumChatsWithUnreadPriorityMessages(PXSteam* const pxSteam);

	// activates game overlay to open the remote play together invite dialog. Invitations will be sent for remote play together
	PXSteamPublic void PXSteamActivateGameOverlayRemotePlayTogetherInviteDialog(PXSteam* const pxSteam, const PXSteamID steamIDLobby);

	// Call this before calling ActivateGameOverlayToWebPage() to have the Steam Overlay Browser block navigations
	// to your specified protocol (scheme) uris and instead dispatch a OverlayBrowserProtocolNavigation_t callback to your game.
	// ActivateGameOverlayToWebPage() must have been called with k_EActivateGameOverlayToWebPageMode_Modal
	PXSteamPublic PXSteamBool PXSteamRegisterProtocolInOverlayBrowser(PXSteam* const pxSteam, const char* pchProtocol);

	// Activates the game overlay to open an invite dialog that will send the provided Rich Presence connect string to selected friends
	PXSteamPublic void PXSteamActivateGameOverlayInviteDialogConnectString(PXSteam* const pxSteam, const char* pchConnectString);

	// Steam Community items equipped by a user on their profile
	// You can register for EquippedProfileItemsChanged_t to know when a friend has changed their equipped profile items
	PXSteamPublic __int64 PXSteamRequestEquippedProfileItems(PXSteam* const pxSteam, const PXSteamID steamID);
	PXSteamPublic PXSteamBool PXSteamBHasEquippedProfileItem(PXSteam* const pxSteam, const PXSteamID steamID, const PXSteamCommunityProfileItemType itemType);
	PXSteamPublic PXSteamBool PXSteamGetProfileItemPropertyString(PXSteam* const pxSteam, const PXSteamID steamID, const PXSteamCommunityProfileItemType itemType, const PXSteamCommunityProfileItemProperty prop, void* const outputBuffer, const unsigned int outputBufferSize);
	PXSteamPublic unsigned int PXSteamGetProfileItemPropertyUint(PXSteam* const pxSteam, const PXSteamID steamID, const PXSteamCommunityProfileItemType itemType, const PXSteamCommunityProfileItemProperty prop);
#endif


#ifdef __cplusplus
}
#endif

#endif