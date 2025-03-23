#include "PXSocket.h"

#if PXSocketUSE

#include <PX/Media/PXText.h>
#include <PX/OS/Memory/PXMemory.h>
#include <PX/Math/PXMath.h>
#include <stdio.h>

#if OSUnix
#include <sys/select.h>
#include <poll.h>
#define OSSocketPoll poll
#elif OSWindows
#pragma comment(lib, "Ws2_32.lib")
#define OSSocketPoll WSAPoll
#endif



void PXAPI PXSocketConstruct(PXSocket* const pxSocket)
{
    PXMemoryClear(pxSocket, sizeof(PXSocket));
    PXThreadConstruct(&pxSocket->CommunicationThread);
    PXDictionaryConstruct(&pxSocket->SocketLookup, sizeof(PXSocketID), sizeof(PXSocket), PXDictionaryValueLocalityInternalEmbedded);

    pxSocket->ID = PXSocketUnused;
}

void PXAPI PXSocketDestruct(PXSocket* const pxSocket)
{
    printf("[C] Socket destruct %i\n", pxSocket->ID);

    PXSocketClose(pxSocket);
    PXThreadDestruct(&pxSocket->CommunicationThread);
    //PXDictionaryDestruct(&pxSocket->SocketLookup);

    PXSocketConstruct(pxSocket);
}

PXActionResult PXAPI PXSocketSetupAdress
(
    PXSocket* const pxSocketList,
    const PXSize socketListSizeMax,
    PXSize* PXSocketListSize,
    const PXSocketAdressSetupInfo* const pxSocketAdressSetupInfoList,
    const PXSize pxSocketAdressSetupInfoListSize
)
{
#if OSWindows
    {
        const PXActionResult wsaResult = WindowsSocketAgentStartup();

        PXActionReturnOnError(wsaResult);
    }
#endif


#if 1 // DEACTIAVTE when compile for windows XP

    *PXSocketListSize = 0;

    for (PXSize i = 0; i < pxSocketAdressSetupInfoListSize; ++i)
    {
        const PXSocketAdressSetupInfo* const pxSocketAdressSetupInfo = &pxSocketAdressSetupInfoList[i];

        PXText portText;
        PXTextConstructNamedBufferA(&portText, portTextBuffer, 30);
        char* portTextAdress = 0;


        portText.Format = pxSocketAdressSetupInfo->IP.Format;

        if (pxSocketAdressSetupInfo->Port != -1)
        {
            PXTextFromInt(&portText, pxSocketAdressSetupInfo->Port);

            portTextAdress = portText.TextA;
        }

        switch (pxSocketAdressSetupInfo->IP.Format)
        {
        case TextFormatASCII:
        case TextFormatUTF8:
        {
#if OSUnix
            struct addrinfo addressInfoHint;
            struct addrinfo addressInfoResult;

            PXMemoryClear(&addressInfoHint, sizeof(struct addrinfo));
            addressInfoHint.ai_flags = AI_PASSIVE;    // For wildcard IP address (AI_NUMERICHOST | AI_PASSIVE;)
            addressInfoHint.ai_family = PXIPAdressFamilyToID(pxSocketAdressSetupInfo->IPMode);
            addressInfoHint.ai_socktype = PXSocketTypeToID(pxSocketAdressSetupInfo->SocketType); // Datagram socket
            addressInfoHint.ai_protocol = ConvertFromProtocolMode(pxSocketAdressSetupInfo->ProtocolMode);

            const int adressInfoResult = getaddrinfo(pxSocketAdressSetupInfo->IP.TextA, portTextAdress, &addressInfoHint, &addressInfoResult);
            const PXBool validAdressInfo = adressInfoResult == 0;

#elif OSWindow
            ADDRINFOA addressInfoHintA;
            ADDRINFOA* addressInfoListA = 0;

            PXMemoryClear(&addressInfoHintA, sizeof(ADDRINFOA));
            addressInfoHintA.ai_flags = AI_PASSIVE;    // For wildcard IP address (AI_NUMERICHOST | AI_PASSIVE;)
            addressInfoHintA.ai_family = ConvertFromIPAdressFamily(pxSocketAdressSetupInfo->IPMode);
            addressInfoHintA.ai_socktype = ConvertFromSocketType(pxSocketAdressSetupInfo->SocketType); // Datagram socket
            addressInfoHintA.ai_protocol = ConvertFromProtocolMode(pxSocketAdressSetupInfo->ProtocolMode);


            const int adressInfoResult = GetAddrInfoA(pxSocketAdressSetupInfo->IP.TextA, portTextAdress, &addressInfoHintA, &addressInfoListA);
            const PXBool validAdressInfo = adressInfoResult == 0;

            if (!validAdressInfo)
            {
#if OSUnix
                return PXErrorCurrent();
#elif OSWindows
                return PXWindowsSocketAgentErrorFetch();
#endif
            }


            for (ADDRINFOA* adressInfoCurrent = addressInfoListA; adressInfoCurrent; adressInfoCurrent = adressInfoCurrent->ai_next)
            {
                PXSocket* const pxSocket = &pxSocketList[(*PXSocketListSize)++];
                struct sockaddr_in6* ipv6 = (struct sockaddr_in6*)adressInfoCurrent->ai_addr;
                const char* result = 0;

                PXMemoryClear(pxSocket->IP, IPv6LengthMax);

                result = inet_ntop(adressInfoCurrent->ai_family, &ipv6->sin6_addr, pxSocket->IP, IPv6LengthMax); // Windows Vista (+UWP), Ws2_32.dll, ws2tcpip.h

                switch (adressInfoCurrent->ai_family)
                {
                case AF_INET:
                {
                    pxSocket->Port = ntohs((((struct sockaddr_in*)adressInfoCurrent->ai_addr)->sin_port)); // Windows Vista (+UWP), Ws2_32.dll, ws2tcpip.h
                    break;
                }
                case AF_INET6:
                {
                    pxSocket->Port = ntohs((((struct sockaddr_in6*)adressInfoCurrent->ai_addr)->sin6_port)); // Windows Vista (+UWP), Ws2_32.dll, ws2tcpip.h
                    break;
                }
                default:
                {
                    pxSocket->Port = 0;
                    break;
                }
                }

                pxSocket->Protocol = pxSocketAdressSetupInfo->ProtocolMode;
                pxSocket->Family = pxSocketAdressSetupInfo->IPMode;
                pxSocket->Type = pxSocketAdressSetupInfo->SocketType;
                pxSocket->IPSize = adressInfoCurrent->ai_addrlen;

                PXMemoryCopy(adressInfoCurrent->ai_addr, adressInfoCurrent->ai_addrlen, pxSocket->IP, IPv6LengthMax);

                pxSocket->State = SocketInitialised;
            }

            FreeAddrInfoA(addressInfoListA);

#endif
            break;
        }
        case TextFormatUNICODE:
        {
#if OSUnix
#elif OSWindows
            ADDRINFOW addressInfoHintW;
            ADDRINFOW* addressInfoListW = 0;

            PXClear(ADDRINFOW, &addressInfoHintW);
            addressInfoHintW.ai_flags = AI_PASSIVE;    // For wildcard IP address (AI_NUMERICHOST | AI_PASSIVE;)
            addressInfoHintW.ai_family = PXIPAdressFamilyToID(pxSocketAdressSetupInfo->IPMode);
            addressInfoHintW.ai_socktype = PXSocketTypeToID(pxSocketAdressSetupInfo->SocketType); // Datagram socket
            addressInfoHintW.ai_protocol = ConvertFromProtocolMode(pxSocketAdressSetupInfo->ProtocolMode);


            const int adressInfoResultID =
#if WindowsAtleastVista
                GetAddrInfoW(pxSocketAdressSetupInfo->IP.TextW, (wchar_t*)portTextAdress, &addressInfoHintW, &addressInfoListW); // Windows Vista, Ws2_32.dll, ws2tcpip.h
#else
                0;
#endif

            const PXBool validAdressInfo = adressInfoResultID == 0;

            if (!validAdressInfo)
            {
#if OSUnix
                return PXErrorCurrent();
#elif OSWindows
                return PXWindowsSocketAgentErrorFetch();
#endif
            }

            for (ADDRINFOW* adressInfoCurrent = addressInfoListW; adressInfoCurrent; adressInfoCurrent = adressInfoCurrent->ai_next)
            {
                PXSocket* const pxSocket = &pxSocketList[(*PXSocketListSize)++];
                struct sockaddr_in6* ipv6 = (struct sockaddr_in6*)adressInfoCurrent->ai_addr;
                const char* result = 0;

                PXClearList(char, pxSocket->IP, IPv6LengthMax);

                // Convert IPv4 or IPv6 into string
#if WindowsAtleastVista
                result = inet_ntop  // Windows Vista (*UWP), Ws2_32.dll, ws2tcpip.h
                         (
                             adressInfoCurrent->ai_family,
                             &ipv6->sin6_addr,
                             pxSocket->IP,
                             IPv6LengthMax
                         );
#else
                // ???
#endif

                switch (adressInfoCurrent->ai_family)
                {
                case AF_INET:
                {
                    pxSocket->Port = ntohs((((struct sockaddr_in*)adressInfoCurrent->ai_addr)->sin_port));
                    break;
                }
                case AF_INET6:
                {
                    pxSocket->Port = ntohs((((struct sockaddr_in6*)adressInfoCurrent->ai_addr)->sin6_port));
                    break;
                }
                default:
                {
                    pxSocket->Port = 0;
                    break;
                }
                }

                pxSocket->Protocol = pxSocketAdressSetupInfo->ProtocolMode;
                pxSocket->Family = pxSocketAdressSetupInfo->IPMode;
                pxSocket->Type = pxSocketAdressSetupInfo->SocketType;
                pxSocket->IPSize = adressInfoCurrent->ai_addrlen;

                PXMemoryCopy(adressInfoCurrent->ai_addr, adressInfoCurrent->ai_addrlen, pxSocket->IP, IPv6LengthMax);

                pxSocket->State = SocketInitialised;
            }

#if WindowsAtleastVista
            FreeAddrInfoW(addressInfoListW); // Windows Vista (+UWP), Ws2_32.dll, ws2tcpip.h

#endif

#endif
            break;
        }
        }
    }
#endif

    return PXActionSuccessful;
}

PXBool PXAPI PXSocketIsCurrentlyUsed(PXSocket* const pxSocket)
{
    return pxSocket->ID != PXSocketUnused;
}

void PXAPI PXSocketStateChange(PXSocket* const pxSocket, const PXSocketState socketState)
{
    const PXSocketState oldState = pxSocket->State;

    pxSocket->State = socketState;

    PXFunctionInvoke(pxSocket->EventList.SocketStateChangedCallBack, pxSocket->Owner, pxSocket, oldState, socketState);
}

PXActionResult PXAPI PXSocketClientRemove(PXSocket* const serverSocket, const PXSocketID clientID)
{
    PXSocket clientSockket;

    // Extract client socket
    {
        const PXSocketID clientIDCopy = clientID;
        const PXBool success = PXDictionaryExtract(&serverSocket->SocketLookup, &clientID, &clientSockket);

        if (!success)
        {
            return PXActionFailedElementNotFound;
        }
    }

#if SocketDebug
    printf("[PXSocket] <%i> remove <%i>\n", (int)serverSocket->ID, (int)clientSockket.ID);
#endif

    PXSocketClose(&clientSockket);

    return PXActionSuccessful;
}


#endif
