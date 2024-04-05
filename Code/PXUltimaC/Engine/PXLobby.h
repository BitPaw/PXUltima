#ifndef PXLobbyInclude
#define PXLobbyInclude

#include <Media/PXResource.h>
#include <OS/Error/PXActionResult.h>

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

	typedef struct PXLobbyChatMessage_
	{
		int x;
	}
	PXLobbyChatMessage;

	typedef struct PXLobbyPlayerItem_
	{
		int x;
	}
	PXLobbyPlayerItem;

	typedef struct PXLobbyPlayer_
	{
		PXInt64U ProfileID; // Unique ID for the user.	
		PXInt32U ProfileAccountLevel; // Used in some programs

		char Name[64]; // Username, can be changed at will.
		PXSize NameSize; // Username size

		PXLobbyUserActiveState ActiveState;
		PXLobbyUserFriendshipStatus FriendshipStatus;

	}
	PXLobbyPlayer;

	typedef struct PXLobby_
	{
		void* Owner; // Is passed to every callback as a first argument.

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
	typedef PXActionResult(PXAPI* PXLobbyUserPromoteToHost)(void* const owner);
	typedef PXActionResult(PXAPI* PXLobbyUserRename)(void* const owner);
	typedef PXActionResult(PXAPI* PXLobbyUserKick)(void* const owner);
	typedef PXActionResult(PXAPI* PXLobbyUserBan)(void* const owner);

	typedef PXActionResult(PXAPI* PXLobbyUserMe)(void* const owner);
	typedef PXActionResult(PXAPI* PXLobbyUserListAll)(void* const owner);

	typedef PXActionResult(PXAPI* PXLobbyUserProfileIconFetch)(void* const owner, PXImage* const image);



	typedef PXActionResult(PXAPI* PXLobbyUserFriendGroupListAmount)(void* const owner);
	typedef PXActionResult(PXAPI* PXLobbyUserFriendGroupListSingle)(void* const owner);
	typedef PXActionResult(PXAPI* PXLobbyUserFriendGroupListAll)(void* const owner);

	typedef PXActionResult(PXAPI* PXLobbyUserFriendClanListAmount)(void* const owner);
	typedef PXActionResult(PXAPI* PXLobbyUserFriendClanListSingle)(void* const owner);
	
	typedef PXActionResult(PXAPI* PXLobbyUserFriendClanListAll)(void* const owner);

	// If you payed with a guy, store that info localy
	typedef PXActionResult(PXAPI* PXLobbyUserRemember)(void* const owner);


	// If User is ingame speaking (Push to talk) set this to supress a current voice call.
	typedef PXActionResult(PXAPI* PXLobbyUserVoiceInGameSet)(void* const owner);

	// User can be NULL
	typedef PXActionResult(PXAPI* PXLobbyExternalOverlayActivate)(void* const owner);

#if 1

	// Fetch user inform,at
	typedef PXActionResult(PXAPI* PXLobbyUserInformationRequest)(void* const owner, PXLobbyPlayer* const pxLobbyPlayer, const PXInt32U fetchFlagList);


	typedef PXActionResult(PXAPI* PXLobbyClanOfficerListRequest)(void* const owner, PXLobbyPlayer* const pxLobbyPlayer);


	typedef PXActionResult(PXAPI* PXLobbyClanOwnerGet)(void* const owner, PXLobbyPlayer* const pxLobbyPlayer);

	typedef PXActionResult(PXAPI* PXLobbyClanOfficerCountGet)(void* const owner, PXLobbyPlayer* const pxLobbyPlayer);
	
	typedef PXActionResult(PXAPI* PXLobbyClanOfficerByIndexGet)(void* const owner, PXLobbyPlayer* const pxLobbyPlayer, int iOfficer);

	typedef PXActionResult(PXAPI* PXLobbyUserRestrictionsGet)(void* const owner);


	typedef PXActionResult(PXAPI* PXLobbyRichPresenceChange)(void* const owner, const char* pchKey, const char* pchValue);
	typedef PXActionResult(PXAPI* PXLobbyClearRichPresence)(void* const owner);
	typedef PXActionResult(PXAPI* PXLobbyFriendRichPresenceFetch)(void* const owner, PXLobbyPlayer* const pxLobbyPlayer, const char* pchKey, void* const outputBuffer, const unsigned int outputBufferSize);
	typedef PXActionResult(PXAPI* PXLobbyFriendRichPresenceKeyCountFetch)(void* const owner, PXLobbyPlayer* const pxLobbyPlayer, void* const outputBuffer, const unsigned int outputBufferSize);
	typedef PXActionResult(PXAPI* PXLobbyFriendRichPresenceKeyByIndex)(void* const owner, PXLobbyPlayer* const pxLobbyPlayer, int iKey);
	
	typedef PXActionResult(PXAPI* PXLobbyRequestFriendRichPresence)(void* const owner, PXLobbyPlayer* const pxLobbyPlayer);

	typedef PXActionResult(PXAPI* PXLobbyUserInviteToGame)(void* const owner, PXLobbyPlayer* const pxLobbyPlayer, const char* pchConnectString);


	typedef PXActionResult(PXAPI* PXLobbyCoplayFriendCountGet)(void* const owner);
	typedef PXActionResult(PXAPI* PXLobbyCoplayFriendGet)(void* const owner, int iCoplayFriend);
	typedef PXActionResult(PXAPI* PXLobbyFriendCoplayTimeGet)(void* const owner, PXLobbyPlayer* const pxLobbyPlayer);
	typedef PXActionResult(PXAPI* PXLobbyFriendCoplayGameGet)(void* const owner, PXLobbyPlayer* const pxLobbyPlayer);


	typedef PXActionResult(PXAPI* PXLobbyClanChatRoomJoin)(void* const owner, PXLobbyPlayer* const pxLobbyPlayer);
	typedef PXActionResult(PXAPI* PXLobbyClanChatRoomLeave)(void* const owner, PXLobbyPlayer* const pxLobbyPlayer);
	typedef PXActionResult(PXAPI* PXLobbyClanChatMemberCountGet)(void* const owner, PXLobbyPlayer* const pxLobbyPlayer);
	typedef PXActionResult(PXAPI* PXLobbyChatMemberByIndexGet)(void* const owner, PXLobbyPlayer* const pxLobbyPlayer, int iUser);
	typedef PXActionResult(PXAPI* PXLobbyClanChatMessageSend)(void* const owner, PXLobbyPlayer* const pxLobbyPlayer, const char* pchText);
	typedef PXActionResult(PXAPI* PXLobbyClanChatMessageGet)(void* const owner, PXLobbyPlayer* const pxLobbyPlayer, PXLobbyChatMessage* const pxLobbyChatMessage);
	typedef PXActionResult(PXAPI* PXLobbyClanChatIsAdmin)(void* const owner, PXLobbyPlayer* const pxLobbyPlayer);

	typedef PXActionResult(PXAPI* PXLobbyClanChatWindowInExternalOpen)(void* const owner, PXLobbyPlayer* const pxLobbyPlayer);
	typedef PXActionResult(PXAPI* PXLobbyClanChatWindowInExternalClose)(void* const owner, PXLobbyPlayer* const pxLobbyPlayer);

	typedef PXActionResult(PXAPI* PXLobbyFriendsMessagesListenForSet)(void* const owner, const PXBool enable);
	typedef PXActionResult(PXAPI* PXLobbyFriendMessageReplyTo)(void* const owner, PXLobbyPlayer* const pxLobbyPlayer, const char* pchMsgToSend);
	typedef PXActionResult(PXAPI* PXLobbyFriendMessageGet)(void* const owner, PXLobbyPlayer* const pxLobbyPlayer, PXLobbyChatMessage* const pxLobbyChatMessage);

	typedef PXActionResult(PXAPI* PXLobbyUserFollowerAmount)(void* const owner, PXLobbyPlayer* const pxLobbyPlayer);
	typedef PXActionResult(PXAPI* PXLobbyUserFollowingAmount)(void* const owner, PXLobbyPlayer* const pxLobbyPlayer);
	typedef PXActionResult(PXAPI* PXLobbyEnumerateFollowingList)(void* const owner, unsigned int unStartIndex);

	typedef PXActionResult(PXAPI* PXLobbyClanIsPublic)(void* const owner, PXLobbyPlayer* const pxLobbyPlayer);
	typedef PXActionResult(PXAPI* PXLobbyClanIsOfficialGameGroup)(void* const owner, PXLobbyPlayer* const pxLobbyPlayer);

	typedef PXActionResult(PXAPI* PXLobbyNumChatsWithUnreadPriorityMessagesGet)(void* const owner);

	typedef PXActionResult(PXAPI* PXLobbyRegisterProtocolInOverlayBrowser)(void* const owner, const char* pchProtocol);

	typedef PXActionResult(PXAPI* PXLobbyActivateGameOverlayInviteDialogConnectString)(void* const owner, const char* pchConnectString);

	typedef PXActionResult(PXAPI* PXLobbyEquippedProfileItemsRequest)(void* const owner, PXLobbyPlayer* const pxLobbyPlayer);
	typedef PXActionResult(PXAPI* PXLobbyEquippedProfileItemHas)(void* const owner, PXLobbyPlayer* const pxLobbyPlayer, PXLobbyPlayerItem* const pxLobbyPlayerItem);
	typedef PXActionResult(PXAPI* PXLobbyProfileItemPropertyStringGet)(void* const owner, PXLobbyPlayer* const pxLobbyPlayer, PXLobbyPlayerItem* const pxLobbyPlayerItem);
	typedef PXActionResult(PXAPI* PXLobbyProfileItemPropertyUintGet)(void* const owner, PXLobbyPlayer* const pxLobbyPlayer, PXLobbyPlayerItem* const pxLobbyPlayerItem);
#endif

#endif