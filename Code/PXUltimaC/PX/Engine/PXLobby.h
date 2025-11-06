#pragma once

#ifndef PXLobbyIncluded
#define PXLobbyIncluded

#include <PX/Engine/PXResource.h>

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
    PXLobbyUserActiveStateOffline,            // friend is not currently logged on
    PXLobbyUserActiveStateOnline,            // friend is logged on
    PXLobbyUserActiveStateBusy,            // user is on, but busy
    PXLobbyUserActiveStateAway,            // auto-away feature
    PXLobbyUserActiveStateSnooze,            // auto-away for a long time
    PXLobbyUserActiveStateLookingToTrade,    // Online, trading
    PXLobbyUserActiveStateLookingToPlay,    // Online, wanting to play
    PXLobbyUserActiveStateInvisible        // Online, but appears offline to friends.  This status is never published to clients.
}
PXLobbyUserActiveState;

typedef enum PXLobbyUserFriendshipStatus_
{
    PXLobbyUserFriendshipStatusInvalid,
    PXLobbyUserFriendshipStatusStanger,
    PXLobbyUserFriendshipStatusBlocked,            // this doesn't get stored; the user has just done an Ignore on an friendship invite
    PXLobbyUserFriendshipStatusRecipient,
    PXLobbyUserFriendshipStatusFriend,
    PXLobbyUserFriendshipStatusRequestInitiator,
    PXLobbyUserFriendshipStatusIgnored,            // this is stored; the user has explicit blocked this other user from comments/chat/etc
    PXLobbyUserFriendshipStatusIgnoredFriend,
    PXLobbyUserFriendshipStatusSuggested_DEPRECATED,        // was used by the original implementation of the facebook linking feature, but now unused.
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
    PXI64U ProfileID; // Unique ID for the user.
    PXI32U ProfileAccountLevel; // Used in some programs

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

typedef PXBool(PXAPI* PXLobbyCreate)(void PXREF owner);
typedef PXBool(PXAPI* PXLobbyJoin)(void PXREF owner);
typedef PXBool(PXAPI* PXLobbyDestroy)(void PXREF owner);

typedef PXBool(PXAPI* PXLobbyDestroy)(void PXREF owner);

// For non dedicated servers, promote player to host the server.
// if you dont have permission, you start a vote.
typedef PXActionResult(PXAPI* PXLobbyUserPromoteToHost)(void PXREF owner);
typedef PXActionResult(PXAPI* PXLobbyUserRename)(void PXREF owner);
typedef PXActionResult(PXAPI* PXLobbyUserKick)(void PXREF owner);
typedef PXActionResult(PXAPI* PXLobbyUserBan)(void PXREF owner);

typedef PXActionResult(PXAPI* PXLobbyUserMe)(void PXREF owner);
typedef PXActionResult(PXAPI* PXLobbyUserListAll)(void PXREF owner);

typedef PXActionResult(PXAPI* PXLobbyUserProfileIconFetch)(void PXREF owner, PXTexture PXREF image);



typedef PXActionResult(PXAPI* PXLobbyUserFriendGroupListAmount)(void PXREF owner);
typedef PXActionResult(PXAPI* PXLobbyUserFriendGroupListSingle)(void PXREF owner);
typedef PXActionResult(PXAPI* PXLobbyUserFriendGroupListAll)(void PXREF owner);

typedef PXActionResult(PXAPI* PXLobbyUserFriendClanListAmount)(void PXREF owner);
typedef PXActionResult(PXAPI* PXLobbyUserFriendClanListSingle)(void PXREF owner);

typedef PXActionResult(PXAPI* PXLobbyUserFriendClanListAll)(void PXREF owner);

// If you payed with a guy, store that info localy
typedef PXActionResult(PXAPI* PXLobbyUserRemember)(void PXREF owner);


// If User is ingame speaking (Push to talk) set this to supress a current voice call.
typedef PXActionResult(PXAPI* PXLobbyUserVoiceInGameSet)(void PXREF owner);

// User can be NULL
typedef PXActionResult(PXAPI* PXLobbyExternalOverlayActivate)(void PXREF owner);

#if 1

// Fetch user inform,at
typedef PXActionResult(PXAPI* PXLobbyUserInformationRequest)(void PXREF owner, PXLobbyPlayer PXREF pxLobbyPlayer, const PXI32U fetchFlagList);


typedef PXActionResult(PXAPI* PXLobbyClanOfficerListRequest)(void PXREF owner, PXLobbyPlayer PXREF pxLobbyPlayer);


typedef PXActionResult(PXAPI* PXLobbyClanOwnerGet)(void PXREF owner, PXLobbyPlayer PXREF pxLobbyPlayer);

typedef PXActionResult(PXAPI* PXLobbyClanOfficerCountGet)(void PXREF owner, PXLobbyPlayer PXREF pxLobbyPlayer);

typedef PXActionResult(PXAPI* PXLobbyClanOfficerByIndexGet)(void PXREF owner, PXLobbyPlayer PXREF pxLobbyPlayer, int iOfficer);

typedef PXActionResult(PXAPI* PXLobbyUserRestrictionsGet)(void PXREF owner);


typedef PXActionResult(PXAPI* PXLobbyRichPresenceChange)(void PXREF owner, const char* pchKey, const char* pchValue);
typedef PXActionResult(PXAPI* PXLobbyClearRichPresence)(void PXREF owner);
typedef PXActionResult(PXAPI* PXLobbyFriendRichPresenceFetch)(void PXREF owner, PXLobbyPlayer PXREF pxLobbyPlayer, const char* pchKey, void PXREF outputBuffer, const unsigned int outputBufferSize);
typedef PXActionResult(PXAPI* PXLobbyFriendRichPresenceKeyCountFetch)(void PXREF owner, PXLobbyPlayer PXREF pxLobbyPlayer, void PXREF outputBuffer, const unsigned int outputBufferSize);
typedef PXActionResult(PXAPI* PXLobbyFriendRichPresenceKeyByIndex)(void PXREF owner, PXLobbyPlayer PXREF pxLobbyPlayer, int iKey);

typedef PXActionResult(PXAPI* PXLobbyRequestFriendRichPresence)(void PXREF owner, PXLobbyPlayer PXREF pxLobbyPlayer);

typedef PXActionResult(PXAPI* PXLobbyUserInviteToGame)(void PXREF owner, PXLobbyPlayer PXREF pxLobbyPlayer, const char* pchConnectString);


typedef PXActionResult(PXAPI* PXLobbyCoplayFriendCountGet)(void PXREF owner);
typedef PXActionResult(PXAPI* PXLobbyCoplayFriendGet)(void PXREF owner, int iCoplayFriend);
typedef PXActionResult(PXAPI* PXLobbyFriendCoplayTimeGet)(void PXREF owner, PXLobbyPlayer PXREF pxLobbyPlayer);
typedef PXActionResult(PXAPI* PXLobbyFriendCoplayGameGet)(void PXREF owner, PXLobbyPlayer PXREF pxLobbyPlayer);


typedef PXActionResult(PXAPI* PXLobbyClanChatRoomJoin)(void PXREF owner, PXLobbyPlayer PXREF pxLobbyPlayer);
typedef PXActionResult(PXAPI* PXLobbyClanChatRoomLeave)(void PXREF owner, PXLobbyPlayer PXREF pxLobbyPlayer);
typedef PXActionResult(PXAPI* PXLobbyClanChatMemberCountGet)(void PXREF owner, PXLobbyPlayer PXREF pxLobbyPlayer);
typedef PXActionResult(PXAPI* PXLobbyChatMemberByIndexGet)(void PXREF owner, PXLobbyPlayer PXREF pxLobbyPlayer, int iUser);
typedef PXActionResult(PXAPI* PXLobbyClanChatMessageSend)(void PXREF owner, PXLobbyPlayer PXREF pxLobbyPlayer, const char* pchText);
typedef PXActionResult(PXAPI* PXLobbyClanChatMessageGet)(void PXREF owner, PXLobbyPlayer PXREF pxLobbyPlayer, PXLobbyChatMessage PXREF pxLobbyChatMessage);
typedef PXActionResult(PXAPI* PXLobbyClanChatIsAdmin)(void PXREF owner, PXLobbyPlayer PXREF pxLobbyPlayer);

typedef PXActionResult(PXAPI* PXLobbyClanChatWindowInExternalOpen)(void PXREF owner, PXLobbyPlayer PXREF pxLobbyPlayer);
typedef PXActionResult(PXAPI* PXLobbyClanChatWindowInExternalClose)(void PXREF owner, PXLobbyPlayer PXREF pxLobbyPlayer);

typedef PXActionResult(PXAPI* PXLobbyFriendsMessagesListenForSet)(void PXREF owner, const PXBool enable);
typedef PXActionResult(PXAPI* PXLobbyFriendMessageReplyTo)(void PXREF owner, PXLobbyPlayer PXREF pxLobbyPlayer, const char* pchMsgToSend);
typedef PXActionResult(PXAPI* PXLobbyFriendMessageGet)(void PXREF owner, PXLobbyPlayer PXREF pxLobbyPlayer, PXLobbyChatMessage PXREF pxLobbyChatMessage);

typedef PXActionResult(PXAPI* PXLobbyUserFollowerAmount)(void PXREF owner, PXLobbyPlayer PXREF pxLobbyPlayer);
typedef PXActionResult(PXAPI* PXLobbyUserFollowingAmount)(void PXREF owner, PXLobbyPlayer PXREF pxLobbyPlayer);
typedef PXActionResult(PXAPI* PXLobbyEnumerateFollowingList)(void PXREF owner, unsigned int unStartIndex);

typedef PXActionResult(PXAPI* PXLobbyClanIsPublic)(void PXREF owner, PXLobbyPlayer PXREF pxLobbyPlayer);
typedef PXActionResult(PXAPI* PXLobbyClanIsOfficialGameGroup)(void PXREF owner, PXLobbyPlayer PXREF pxLobbyPlayer);

typedef PXActionResult(PXAPI* PXLobbyNumChatsWithUnreadPriorityMessagesGet)(void PXREF owner);

typedef PXActionResult(PXAPI* PXLobbyRegisterProtocolInOverlayBrowser)(void PXREF owner, const char* pchProtocol);

typedef PXActionResult(PXAPI* PXLobbyActivateGameOverlayInviteDialogConnectString)(void PXREF owner, const char* pchConnectString);

typedef PXActionResult(PXAPI* PXLobbyEquippedProfileItemsRequest)(void PXREF owner, PXLobbyPlayer PXREF pxLobbyPlayer);
typedef PXActionResult(PXAPI* PXLobbyEquippedProfileItemHas)(void PXREF owner, PXLobbyPlayer PXREF pxLobbyPlayer, PXLobbyPlayerItem PXREF pxLobbyPlayerItem);
typedef PXActionResult(PXAPI* PXLobbyProfileItemPropertyStringGet)(void PXREF owner, PXLobbyPlayer PXREF pxLobbyPlayer, PXLobbyPlayerItem PXREF pxLobbyPlayerItem);
typedef PXActionResult(PXAPI* PXLobbyProfileItemPropertyUintGet)(void PXREF owner, PXLobbyPlayer PXREF pxLobbyPlayer, PXLobbyPlayerItem PXREF pxLobbyPlayerItem);

#endif

#endif
