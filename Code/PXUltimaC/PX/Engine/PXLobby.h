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
typedef PXResult(PXAPI* PXLobbyUserPromoteToHost)(void PXREF owner);
typedef PXResult(PXAPI* PXLobbyUserRename)(void PXREF owner);
typedef PXResult(PXAPI* PXLobbyUserKick)(void PXREF owner);
typedef PXResult(PXAPI* PXLobbyUserBan)(void PXREF owner);

typedef PXResult(PXAPI* PXLobbyUserMe)(void PXREF owner);
typedef PXResult(PXAPI* PXLobbyUserListAll)(void PXREF owner);

typedef PXResult(PXAPI* PXLobbyUserProfileIconFetch)(void PXREF owner, PXTexture PXREF image);



typedef PXResult(PXAPI* PXLobbyUserFriendGroupListAmount)(void PXREF owner);
typedef PXResult(PXAPI* PXLobbyUserFriendGroupListSingle)(void PXREF owner);
typedef PXResult(PXAPI* PXLobbyUserFriendGroupListAll)(void PXREF owner);

typedef PXResult(PXAPI* PXLobbyUserFriendClanListAmount)(void PXREF owner);
typedef PXResult(PXAPI* PXLobbyUserFriendClanListSingle)(void PXREF owner);

typedef PXResult(PXAPI* PXLobbyUserFriendClanListAll)(void PXREF owner);

// If you payed with a guy, store that info localy
typedef PXResult(PXAPI* PXLobbyUserRemember)(void PXREF owner);


// If User is ingame speaking (Push to talk) set this to supress a current voice call.
typedef PXResult(PXAPI* PXLobbyUserVoiceInGameSet)(void PXREF owner);

// User can be NULL
typedef PXResult(PXAPI* PXLobbyExternalOverlayActivate)(void PXREF owner);

#if 1

// Fetch user inform,at
typedef PXResult(PXAPI* PXLobbyUserInformationRequest)(void PXREF owner, PXLobbyPlayer PXREF pxLobbyPlayer, const PXI32U fetchFlagList);


typedef PXResult(PXAPI* PXLobbyClanOfficerListRequest)(void PXREF owner, PXLobbyPlayer PXREF pxLobbyPlayer);


typedef PXResult(PXAPI* PXLobbyClanOwnerGet)(void PXREF owner, PXLobbyPlayer PXREF pxLobbyPlayer);

typedef PXResult(PXAPI* PXLobbyClanOfficerCountGet)(void PXREF owner, PXLobbyPlayer PXREF pxLobbyPlayer);

typedef PXResult(PXAPI* PXLobbyClanOfficerByIndexGet)(void PXREF owner, PXLobbyPlayer PXREF pxLobbyPlayer, int iOfficer);

typedef PXResult(PXAPI* PXLobbyUserRestrictionsGet)(void PXREF owner);


typedef PXResult(PXAPI* PXLobbyRichPresenceChange)(void PXREF owner, const char* pchKey, const char* pchValue);
typedef PXResult(PXAPI* PXLobbyClearRichPresence)(void PXREF owner);
typedef PXResult(PXAPI* PXLobbyFriendRichPresenceFetch)(void PXREF owner, PXLobbyPlayer PXREF pxLobbyPlayer, const char* pchKey, void PXREF outputBuffer, const unsigned int outputBufferSize);
typedef PXResult(PXAPI* PXLobbyFriendRichPresenceKeyCountFetch)(void PXREF owner, PXLobbyPlayer PXREF pxLobbyPlayer, void PXREF outputBuffer, const unsigned int outputBufferSize);
typedef PXResult(PXAPI* PXLobbyFriendRichPresenceKeyByIndex)(void PXREF owner, PXLobbyPlayer PXREF pxLobbyPlayer, int iKey);

typedef PXResult(PXAPI* PXLobbyRequestFriendRichPresence)(void PXREF owner, PXLobbyPlayer PXREF pxLobbyPlayer);

typedef PXResult(PXAPI* PXLobbyUserInviteToGame)(void PXREF owner, PXLobbyPlayer PXREF pxLobbyPlayer, const char* pchConnectString);


typedef PXResult(PXAPI* PXLobbyCoplayFriendCountGet)(void PXREF owner);
typedef PXResult(PXAPI* PXLobbyCoplayFriendGet)(void PXREF owner, int iCoplayFriend);
typedef PXResult(PXAPI* PXLobbyFriendCoplayTimeGet)(void PXREF owner, PXLobbyPlayer PXREF pxLobbyPlayer);
typedef PXResult(PXAPI* PXLobbyFriendCoplayGameGet)(void PXREF owner, PXLobbyPlayer PXREF pxLobbyPlayer);


typedef PXResult(PXAPI* PXLobbyClanChatRoomJoin)(void PXREF owner, PXLobbyPlayer PXREF pxLobbyPlayer);
typedef PXResult(PXAPI* PXLobbyClanChatRoomLeave)(void PXREF owner, PXLobbyPlayer PXREF pxLobbyPlayer);
typedef PXResult(PXAPI* PXLobbyClanChatMemberCountGet)(void PXREF owner, PXLobbyPlayer PXREF pxLobbyPlayer);
typedef PXResult(PXAPI* PXLobbyChatMemberByIndexGet)(void PXREF owner, PXLobbyPlayer PXREF pxLobbyPlayer, int iUser);
typedef PXResult(PXAPI* PXLobbyClanChatMessageSend)(void PXREF owner, PXLobbyPlayer PXREF pxLobbyPlayer, const char* pchText);
typedef PXResult(PXAPI* PXLobbyClanChatMessageGet)(void PXREF owner, PXLobbyPlayer PXREF pxLobbyPlayer, PXLobbyChatMessage PXREF pxLobbyChatMessage);
typedef PXResult(PXAPI* PXLobbyClanChatIsAdmin)(void PXREF owner, PXLobbyPlayer PXREF pxLobbyPlayer);

typedef PXResult(PXAPI* PXLobbyClanChatWindowInExternalOpen)(void PXREF owner, PXLobbyPlayer PXREF pxLobbyPlayer);
typedef PXResult(PXAPI* PXLobbyClanChatWindowInExternalClose)(void PXREF owner, PXLobbyPlayer PXREF pxLobbyPlayer);

typedef PXResult(PXAPI* PXLobbyFriendsMessagesListenForSet)(void PXREF owner, const PXBool enable);
typedef PXResult(PXAPI* PXLobbyFriendMessageReplyTo)(void PXREF owner, PXLobbyPlayer PXREF pxLobbyPlayer, const char* pchMsgToSend);
typedef PXResult(PXAPI* PXLobbyFriendMessageGet)(void PXREF owner, PXLobbyPlayer PXREF pxLobbyPlayer, PXLobbyChatMessage PXREF pxLobbyChatMessage);

typedef PXResult(PXAPI* PXLobbyUserFollowerAmount)(void PXREF owner, PXLobbyPlayer PXREF pxLobbyPlayer);
typedef PXResult(PXAPI* PXLobbyUserFollowingAmount)(void PXREF owner, PXLobbyPlayer PXREF pxLobbyPlayer);
typedef PXResult(PXAPI* PXLobbyEnumerateFollowingList)(void PXREF owner, unsigned int unStartIndex);

typedef PXResult(PXAPI* PXLobbyClanIsPublic)(void PXREF owner, PXLobbyPlayer PXREF pxLobbyPlayer);
typedef PXResult(PXAPI* PXLobbyClanIsOfficialGameGroup)(void PXREF owner, PXLobbyPlayer PXREF pxLobbyPlayer);

typedef PXResult(PXAPI* PXLobbyNumChatsWithUnreadPriorityMessagesGet)(void PXREF owner);

typedef PXResult(PXAPI* PXLobbyRegisterProtocolInOverlayBrowser)(void PXREF owner, const char* pchProtocol);

typedef PXResult(PXAPI* PXLobbyActivateGameOverlayInviteDialogConnectString)(void PXREF owner, const char* pchConnectString);

typedef PXResult(PXAPI* PXLobbyEquippedProfileItemsRequest)(void PXREF owner, PXLobbyPlayer PXREF pxLobbyPlayer);
typedef PXResult(PXAPI* PXLobbyEquippedProfileItemHas)(void PXREF owner, PXLobbyPlayer PXREF pxLobbyPlayer, PXLobbyPlayerItem PXREF pxLobbyPlayerItem);
typedef PXResult(PXAPI* PXLobbyProfileItemPropertyStringGet)(void PXREF owner, PXLobbyPlayer PXREF pxLobbyPlayer, PXLobbyPlayerItem PXREF pxLobbyPlayerItem);
typedef PXResult(PXAPI* PXLobbyProfileItemPropertyUintGet)(void PXREF owner, PXLobbyPlayer PXREF pxLobbyPlayer, PXLobbyPlayerItem PXREF pxLobbyPlayerItem);

#endif

#endif
