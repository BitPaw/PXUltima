#include "PXSBP.h"

#if PXSocketUSE

#include <Math/PXMath.h>
#include <OS/File/PXFile.h>
#include <OS/Memory/PXMemory.h>
#include <OS/User/PXUser.h>

void PXSBPMessageConstruct(PXSBPMessage* const pxSBPMessage)
{
    PXClear(PXSBPMessage, pxSBPMessage);

    PXTimeNow(&pxSBPMessage->FirstKnown);

    pxSBPMessage->ID = -1;
    pxSBPMessage->LastKnown = pxSBPMessage->FirstKnown;
}
void PXSBPMessageConstructFlat(PXSBPMessage* const pxSBPMessage, const void* const data, const PXSize dataSize, const void* const owner)
{
    PXSBPMessageConstruct(pxSBPMessage);

    pxSBPMessage->Owner = owner;
    pxSBPMessage->Data = data;
    pxSBPMessage->DataSizeCurrent = dataSize;
    pxSBPMessage->DataSizeExpected = dataSize;
    pxSBPMessage->StorageType = PXSBPMessageStorageTypeDirect;
}
PXBool PXSBPMessageChunkDataIsComplete(const PXSBPChunkCache* const pxSBPMessageChunk)
{
    return pxSBPMessageChunk->SizeExpected <= pxSBPMessageChunk->SizeCurrent;
}

PXBool PXSBPMessageChunkDataContainsMultible(const PXSBPChunkCache* const pxSBPMessageChunk)
{
    return pxSBPMessageChunk->SizeExpected < pxSBPMessageChunk->SizeCurrent;
}

void PXSBPReceiverConstruct(PXSBPReceiver* const pxSBPReceiver)
{
    PXMemoryClear(pxSBPReceiver, sizeof(PXSBPReceiver));

    pxSBPReceiver->State = PXSBPRecieverStateAwaitingHeaderBegin;
    pxSBPReceiver->EnableSBP = PXTrue;

    //PXDictionaryConstruct(&pxSBPClient->Receiver.MessageLookup, sizeof(PXMessageID), sizeof(PXSBPMessage), PXDictionaryValueLocalityInternalEmbedded);
}

void PXSBPServerConstruct(PXSBPServer* const pxSBPServer)
{
    PXServerConstruct(&pxSBPServer->Server);
    PXSBPReceiverConstruct(&pxSBPServer->Receiver);
    PXSBPEmitterConstruct(&pxSBPServer->Emitter);

    pxSBPServer->Server.EventList.SocketDataReceiveCallBack = PXSBPOnDataRawReceive;
    pxSBPServer->Server.Owner = &pxSBPServer->Receiver;
}

void PXSBPServerDestruct(PXSBPServer* const pxSBPServer)
{
    PXServerDestruct(&pxSBPServer->Server);

    // Reset default state
    PXSBPServerConstruct(pxSBPServer);
}

void PXSBPServerReceiverEventListSet(PXSBPServer* const pxSBPServe, PXSBPReceiverEventList* const pxSBPReceiverEventList)
{
    PXSBPReceiverEventListSet(&pxSBPServe->Receiver, pxSBPReceiverEventList);
}

PXActionResult PXSBPServerStart(PXSBPServer* const pxSBPServer, const PXInt16U port)
{
    return PXServerStart(&pxSBPServer->Server, port, PXProtocolModeTCP);
}

PXActionResult PXSBPServerStop(PXSBPServer* const pxSBPServer)
{
    return PXServerStop(&pxSBPServer->Server);
}

PXActionResult PXSBPServerSendToAll(PXSBPServer* const pxSBPServer, const void* const data, const PXSize dataSize)
{
    for (PXSize i = 0; i < pxSBPServer->Server.ServerSocketListSize; i++)
    {
        PXSocket* const serverSubSocket = &pxSBPServer->Server.ServerSocketList[i];

        for (PXSize i = 0; i < serverSubSocket->SocketLookup.EntryAmountCurrent; i++) // All sockets
        {
            PXDictionaryEntry pxDictionaryEntry;

            PXDictionaryIndex(&serverSubSocket->SocketLookup, i, &pxDictionaryEntry);

            const PXSocketID clientID = *(PXSocketID*)pxDictionaryEntry.Key;

            PXBufferConstruct(&serverSubSocket->BufferOutput, (void*)data, dataSize, PXBufferTypeStack);

            pxSBPServer->Emitter.MessageID = -1;
            pxSBPServer->Emitter.SocketSender = serverSubSocket;
            pxSBPServer->Emitter.SocketReceiverID = clientID;

            PXSBPEmitterDeploy(&pxSBPServer->Emitter, data, dataSize);
        }

        PXBufferDestruct(&serverSubSocket->BufferOutput);
    }

    return PXActionSuccessful;
}

void PXSBPClientConstruct(PXSBPClient* const pxSBPClient)
{
    PXClientConstruct(&pxSBPClient->Client);
    PXSBPReceiverConstruct(&pxSBPClient->Receiver);
    PXSBPEmitterConstruct(&pxSBPClient->Emitter);

    pxSBPClient->EnableSBP = PXTrue;

    pxSBPClient->Client.EventList.SocketDataReceiveCallBack = PXSBPOnDataRawReceive;
    pxSBPClient->Client.Owner = &pxSBPClient->Receiver;
}

void PXSBPClientDestruct(PXSBPClient* const pxSBPClient)
{
    PXClientDestruct(&pxSBPClient->Client);

    PXSBPClientConstruct(pxSBPClient);
}

void PXSBPClientReceiverEventListSet(PXSBPClient* const pxSBPClient, PXSBPReceiverEventList* const pxSBPReceiverEventList)
{
    pxSBPClient->Receiver.EventList = *pxSBPReceiverEventList;
}

PXActionResult PXSBPClientConnectToServer(PXSBPClient* const pxSBPClient, const PXText* const ip, const PXInt16U port)
{
    return PXClientConnectToServer(&pxSBPClient->Client, ip, port);
}

PXActionResult PXSBPClientDisconnectFromServer(PXSBPClient* const pxSBPClient)
{
    return PXClientDisconnectFromServer(&pxSBPClient->Client);
}

PXActionResult PXSBPClientSendMessage(PXSBPClient* const pxSBPClient, const void* const data, const PXSize dataSize)
{
    if (!pxSBPClient->EnableSBP)
    {
        return PXClientSendData(&pxSBPClient->Client, data, dataSize);
    }

    pxSBPClient->Emitter.SocketSender = &pxSBPClient->Client.SocketClient;
    pxSBPClient->Emitter.SocketReceiverID = pxSBPClient->Client.SocketClient.ID;

    const PXActionResult result = PXSBPEmitterDeploy(&pxSBPClient->Emitter, data, dataSize);

    return result;
}

PXActionResult PXSBPClientSendFile(PXSBPClient* const pxSBPClient, const PXText* const filePath)
{
    return PXActionRefusedNotImplemented;
}

PXBool PXSBPMessageChunkParse(PXSBPChunkCache* const pxSBPMessageChunk, const void* const data, const PXSize dataSize)
{
    if (dataSize < PXSBPMessageChunkHeaderSize)
    {
        PXMemoryClear(pxSBPMessageChunk, sizeof(PXSBPChunkCache));
        return PXFalse;
    }

    const PXBool hasValidSignature = ((PXAdress)data)[0] == (PXByte)'°' && ((PXAdress)data)[1] == (PXByte)'°';

    if (!hasValidSignature)
    {
        PXMemoryClear(pxSBPMessageChunk, sizeof(PXSBPChunkCache));
        return PXFalse;
    }

    pxSBPMessageChunk->ID = PXInt16MakeEndianBig(((PXAdress)data)[2], ((PXAdress)data)[3]);
    pxSBPMessageChunk->SizeExpected = PXInt16MakeEndianBig(((PXAdress)data)[4], ((PXAdress)data)[5]);
    pxSBPMessageChunk->Message = data;
    pxSBPMessageChunk->SizeCurrent = dataSize - PXSBPMessageChunkHeaderSize;

    return PXTrue;
}

void PXSBPReceiverStateChanged(PXSBPReceiver* const pxSBPReceiver, const PXSBPRecieverState pxSBPRecieverState)
{
    pxSBPReceiver->State = pxSBPRecieverState;
}

void PXSBPReceiverEventListSet(PXSBPReceiver* const pxSBPReceiver, PXSBPReceiverEventList* const pxSBPReceiverEventList)
{
    pxSBPReceiver->EventList = *pxSBPReceiverEventList;
}

PXInt16U PXSBPMessageSizeMissing(PXSBPChunkCache* const pxSBPMessage)
{
    return pxSBPMessage->SizeExpected - pxSBPMessage->SizeCurrent;
}

PXBool PXSBPMessageSizeComplete(PXSBPChunkCache* const pxSBPMessage)
{
    return pxSBPMessage->SizeCurrent == pxSBPMessage->SizeExpected;
}

PXSize PXSBPMessageChunkDataConsume(PXSBPChunkCache* const pxSBPMessage, const void* const data, const PXSize dataSize)
{
    const PXSize before = PXSBPMessageSizeMissing(pxSBPMessage);
    const PXAdress* adress = (PXAdress)pxSBPMessage->Message + pxSBPMessage->SizeCurrent;
    const PXSize copyedSize = PXMemoryCopy(data, dataSize, adress, before);

    pxSBPMessage->SizeCurrent += copyedSize;

    return copyedSize;
}

void PXSBPOnDataRawReceive(PXSBPReceiver* const pxSBPReceiver, const PXSocketDataReceivedEventData* const pxSocketDataReceivedEventData)
{
    if (!pxSBPReceiver->EnableSBP)
    {
        PXSBPMessage pxSBPMessage;

        PXSBPMessageConstructFlat(&pxSBPMessage, pxSocketDataReceivedEventData->Data, pxSocketDataReceivedEventData->DataSize, pxSBPReceiver->EventList.Owner);

        PXFunctionInvoke(pxSBPReceiver->EventList.OnMessageReceivedCallBack, &pxSBPMessage);
        return;
    }

    switch (pxSBPReceiver->State)
    {
        case PXSBPRecieverStateAwaitingHeaderBegin:
        {
            PXMemoryClear(&pxSBPReceiver->MessageChunkCurrent, sizeof(PXSBPChunkCache)); // Clear current message

            // Check if message is too short
            {
                const PXBool isMessageTooShort = pxSocketDataReceivedEventData->DataSize < PXSBPMessageChunkHeaderSize;

                if (isMessageTooShort) // The incomming message is not long enough, we jump into the next state
                {
                    pxSBPReceiver->HeaderCacheSize += PXMemoryCopy(pxSBPReceiver->HeaderCache, PXSBPMessageChunkHeaderSize, pxSocketDataReceivedEventData->Data, pxSocketDataReceivedEventData->DataSize); // Write into buffer

                    PXSBPReceiverStateChanged(pxSBPReceiver, PXSBPRecieverStateAwaitingHeaderEnd); // Wait for header end
                    break; // Finished, parsed part of the header
                }
            }

            // Try parse message
            {
                const PXBool success = PXSBPMessageChunkParse(&pxSBPReceiver->MessageChunkCurrent, pxSocketDataReceivedEventData->Data, pxSocketDataReceivedEventData->DataSize); // Parse full header

                if (!success)
                {
                    // Invalid header detected
                   // PXFunctionInvoke(pxSBPReceiver->OnMessageInvalidCallBack, receiveSocket, clientSocketID);
                    break;
                }
            }

            // if message is exactly just the header
            {
                const PXBool isHeaderOnly = pxSocketDataReceivedEventData->DataSize == PXSBPMessageChunkHeaderSize;

                if (isHeaderOnly)
                {
                    PXSBPReceiverStateChanged(pxSBPReceiver, PXSBPRecieverStateAwaitingDataStart);
                    break;
                }
            }

            // Is all the data available right now?
            {
                const PXBool isComplete = PXSBPMessageChunkDataIsComplete(&pxSBPReceiver->MessageChunkCurrent);

                // if current data segment has less then one package, we cahce it for later
                if (!isComplete)
                {
                    // Chunk is not fully recieved yet

                    PXSBPMessageChunkDataConsume(&pxSBPReceiver->MessageChunkCurrent, pxSocketDataReceivedEventData->Data, pxSocketDataReceivedEventData->DataSize);

                    PXSBPReceiverStateChanged(pxSBPReceiver, PXSBPRecieverStateAwaitingDataEnd);

                    PXFunctionInvoke(pxSBPReceiver->EventList.OnChunkSegmentUpdatedCallBack, &pxSBPReceiver->MessageChunkCurrent);
                    break;
                }
            }

            // Do we have multible message packed together?
            {
                const PXBool containsMultible = PXSBPMessageChunkDataContainsMultible(&pxSBPReceiver->MessageChunkCurrent);

                if (containsMultible) // Message contains multible blocks and needs to be copped
                {
                    const PXSize additionalDataSize = pxSBPReceiver->MessageChunkCurrent.SizeCurrent - pxSBPReceiver->MessageChunkCurrent.SizeExpected;
                    const PXAdress additionalDataAdress = (PXAdress)pxSBPReceiver->MessageChunkCurrent.Message + pxSBPReceiver->MessageChunkCurrent.SizeExpected;
                    PXSBPChunk pxSBPChunk;
                    pxSBPChunk.Data = pxSBPReceiver->MessageChunkCurrent.Message;
                    pxSBPChunk.ChannelID = pxSBPReceiver->MessageChunkCurrent.ID;
                    pxSBPChunk.DataSize = pxSBPReceiver->MessageChunkCurrent.SizeExpected; // truncate message size

                    PXSBPReceiverStateChanged(pxSBPReceiver, PXSBPRecieverStateProcessingData);
                    PXSBPOnDataChunkReceive(pxSBPReceiver, &pxSBPChunk);
                    PXSBPReceiverStateChanged(pxSBPReceiver, PXSBPRecieverStateAwaitingHeaderBegin);// We wait for new header

                    {
                        PXSocketDataReceivedEventData pxSocketDataMoveEventInfoSubSegment;
                        pxSocketDataMoveEventInfoSubSegment.SocketSending = pxSocketDataReceivedEventData->SocketSending;
                        pxSocketDataMoveEventInfoSubSegment.SocketReceiving = pxSocketDataReceivedEventData->SocketReceiving;
                        pxSocketDataMoveEventInfoSubSegment.Data = additionalDataAdress;
                        pxSocketDataMoveEventInfoSubSegment.DataSize = additionalDataSize;

                        PXSBPOnDataRawReceive(pxSBPReceiver, &pxSocketDataMoveEventInfoSubSegment); // Handle additional messages
                    }

                    break;
                }
            }

            // Message is only one block and can be consumed instandly
            PXSBPReceiverStateChanged(pxSBPReceiver, PXSBPRecieverStateProcessingData);

            {
                PXSBPChunk pxSBPChunk;
                pxSBPChunk.Data = (PXAdress)pxSBPReceiver->MessageChunkCurrent.Message + PXSBPMessageChunkHeaderSize;
                pxSBPChunk.DataSize = pxSBPReceiver->MessageChunkCurrent.SizeCurrent - PXSBPMessageChunkHeaderSize;
                pxSBPChunk.ChannelID = pxSBPReceiver->MessageChunkCurrent.ID;

                PXSBPOnDataChunkReceive(pxSBPReceiver, &pxSBPChunk);
            }

            PXSBPReceiverStateChanged(pxSBPReceiver, PXSBPRecieverStateAwaitingHeaderBegin);// We wait for new header

            break;
        }
        case PXSBPRecieverStateAwaitingHeaderEnd:
            // Parse header

            break;
        case PXSBPRecieverStateAwaitingDataStart:
        case PXSBPRecieverStateAwaitingDataEnd:
        {
            const PXSize consumedBytes = PXSBPMessageChunkDataConsume(&pxSBPReceiver->MessageChunkCurrent, pxSocketDataReceivedEventData->Data, pxSocketDataReceivedEventData->DataSize);
            const PXSize remainingData = pxSocketDataReceivedEventData->DataSize - consumedBytes;
            const PXBool hasAdditionalData = 0 < remainingData; // Parsing was ok, next block

            const PXBool isComplete = PXSBPMessageChunkDataIsComplete(&pxSBPReceiver->MessageChunkCurrent);

            if (!isComplete)
            {
                // Wait for additional extrended data
                PXFunctionInvoke(pxSBPReceiver->EventList.OnChunkSegmentUpdatedCallBack, &pxSBPReceiver->MessageChunkCurrent);
                break;
            }

            PXSBPChunk pxSBPChunk;
            pxSBPChunk.Data = pxSBPReceiver->MessageChunkCurrent.Message;
            pxSBPChunk.DataSize = pxSBPReceiver->MessageChunkCurrent.SizeCurrent;
            pxSBPChunk.ChannelID = pxSBPReceiver->MessageChunkCurrent.ID;

            if (!hasAdditionalData)          // We have additional data
            {
                const PXSize additionalDataSize = pxSBPReceiver->MessageChunkCurrent.SizeCurrent - pxSBPReceiver->MessageChunkCurrent.SizeExpected;
                const PXAdress additionalDataAdress = (PXAdress)pxSBPReceiver->MessageChunkCurrent.Message + pxSBPReceiver->MessageChunkCurrent.SizeExpected;

                pxSBPChunk.DataSize = pxSBPReceiver->MessageChunkCurrent.SizeExpected; // truncate message size

                PXSBPReceiverStateChanged(pxSBPReceiver, PXSBPRecieverStateProcessingData);
                PXSBPOnDataChunkReceive(pxSBPReceiver, &pxSBPChunk);
                PXSBPReceiverStateChanged(pxSBPReceiver, PXSBPRecieverStateAwaitingHeaderBegin);// We wait for new header

                {
                    PXSocketDataReceivedEventData pxSocketDataMoveEventInfoSubSegment;
                    pxSocketDataMoveEventInfoSubSegment.SocketSending = pxSocketDataReceivedEventData->SocketSending;
                    pxSocketDataMoveEventInfoSubSegment.SocketReceiving = pxSocketDataReceivedEventData->SocketReceiving;
                    pxSocketDataMoveEventInfoSubSegment.Data = additionalDataAdress;
                    pxSocketDataMoveEventInfoSubSegment.DataSize = additionalDataSize;

                    PXSBPOnDataRawReceive(pxSBPReceiver, &pxSocketDataMoveEventInfoSubSegment); // Handle additional messages
                }

                break;
            }

            // Chunk is now consumed consumable
            PXSBPReceiverStateChanged(pxSBPReceiver, PXSBPRecieverStateProcessingData);
            PXSBPOnDataChunkReceive(pxSBPReceiver, &pxSBPChunk);
            PXSBPReceiverStateChanged(pxSBPReceiver, PXSBPRecieverStateAwaitingHeaderBegin);// We wait for new header

            break;
        }
        default:
            // Illegal state
            break;

    }
}

void PXSBPOnDataChunkReceive(PXSBPReceiver* const pxSBPReceiver, const PXSBPChunk* const pxSBPChunk)
{
    // Fetch steam if it exists
    {
        PXSBPMessage* pxSBPMessage = 0;

        const PXBool isFound = PXDictionaryFind(&pxSBPReceiver->MessageStreamLookup, &pxSBPChunk->ChannelID, &pxSBPMessage);

        if (isFound)
        {
            pxSBPMessage->DataSizeExpected += PXMemoryCopy
            (
                (PXAdress)pxSBPMessage->Data + pxSBPMessage->DataSizeExpected,
                pxSBPMessage->DataSizeCurrent - pxSBPMessage->DataSizeExpected,
                pxSBPChunk->Data,
                pxSBPChunk->DataSize
            );

            pxSBPMessage->MessageSizeCachedInPercent = pxSBPMessage->DataSizeExpected / pxSBPMessage->DataSizeCurrent;

            // is done?
            if (pxSBPMessage->DataSizeExpected == pxSBPMessage->DataSizeCurrent)
            {
                // We are done
                PXFunctionInvoke(pxSBPReceiver->EventList.OnChunkReceivedCallBack, pxSBPChunk);

                PXDictionaryRemove(&pxSBPReceiver->MessageStreamLookup, &pxSBPChunk->ChannelID);

                return;
            }

            PXFunctionInvoke(pxSBPReceiver->EventList.OnChunkSegmentUpdatedCallBack, pxSBPChunk);
        }
    }

    // Is first chunk, read header and register
    {
        PXSBPMessage pxSBPMessage;
        PXSBPMessageConstruct(&pxSBPMessage);

        PXFile pxFile;
        PXFileBufferExternal(&pxFile, pxSBPChunk->Data, pxSBPChunk->DataSize);

        pxSBPMessage.ID = pxSBPChunk->ChannelID;

        // Parse byte
        {
            PXInt8U flagValue = 0;

            PXFileReadI8U(&pxFile, &flagValue);

            PXSBPMessageBitSize pxSBPMessageBitSize = (PXSBPMessageBitSize)(((flagValue & 0b11000000) >> 6u) + 1);
            pxSBPMessage.HasExtendedDelegation = (flagValue & 0b00100000) >> 5u;

            switch (pxSBPMessageBitSize)
            {
                case PXSBPMessageBitSize8Bit:
                {
                    PXInt8U size;

                    PXFileReadI8U(&pxFile, &size);

                    pxSBPMessage.DataSizeCurrent = size;

                    break;
                }
                case PXSBPMessageBitSize16Bit:
                {
                    PXInt16U size;

                    PXFileReadI16UE(&pxFile, &size, PXEndianBig);

                    pxSBPMessage.DataSizeCurrent = size;

                    break;
                }
                case PXSBPMessageBitSize32Bit:
                {
                    PXInt32U size;

                    PXFileReadI32UE(&pxFile, &size, PXEndianBig);

                    pxSBPMessage.DataSizeCurrent = size;

                    break;
                }
                case PXSBPMessageBitSize64Bit:
                {
                    PXInt64U size;

                    PXFileReadI64UE(&pxFile, &size, PXEndianBig);

                    pxSBPMessage.DataSizeCurrent = size;

                    break;
                }
            }

            pxSBPMessage.DataSizeCurrent -= pxFile.DataCursor;
            pxSBPMessage.Data = (PXAdress)pxSBPChunk->Data + pxFile.DataCursor;
            pxSBPMessage.Owner = pxSBPReceiver->EventList.Owner;
        }

        // Is chunk singular and can be consumed instandly?
        const PXBool isConsumableRightNow = pxSBPMessage.DataSizeCurrent == pxSBPChunk->DataSize;

        if (isConsumableRightNow)
        {
            pxSBPMessage.StorageType = PXSBPMessageStorageTypeDirect;
            PXFunctionInvoke(pxSBPReceiver->EventList.OnMessageReceivedCallBack, &pxSBPMessage);
            return;
        }

        pxSBPMessage.StorageType = PXSBPMessageStorageTypeCacheCollected;

        PXDictionaryAdd(&pxSBPReceiver->MessageStreamLookup, &pxSBPChunk->ChannelID, &pxSBPMessage);

        PXFunctionInvoke(pxSBPReceiver->EventList.OnMessageUpdatedCallBack, &pxSBPMessage);
    }
}

void PXSBPEmitterConstruct(PXSBPEmitter* const pxSBPEmitter)
{
    PXMemoryClear(pxSBPEmitter, sizeof(PXSBPEmitter));

    pxSBPEmitter->PackageSizeMaximal = PXSocketBufferSize;
}

PXActionResult PXSBPEmitterDeploy(PXSBPEmitter* const pxSBPEmitter, const void* const message, const PXSize messageSize)
{
    PXActionResult sendResult = PXActionInvalid;

    // How many packages do we have?
    const PXSize numberOfPackages = (messageSize / pxSBPEmitter->PackageSizeMaximal) + 1;
    PXSize accumulator = messageSize;

    for (PXSize i = 0; i < numberOfPackages; ++i)
    {
        PXInt16U packageSizeCurrent = PXMathMinimumIU(accumulator, pxSBPEmitter->PackageSizeMaximal);
        const PXAdress dataSourcePoint = (PXAdress)message + (messageSize - accumulator);

        accumulator -= packageSizeCurrent;

        if (i == 0)
        {
            packageSizeCurrent += 3u;
        }

        char cache[1024];

        PXFile pxFileBuffer;
        PXFileBufferExternal(&pxFileBuffer, cache, 1024);

        // write header
        PXFileWriteI8U(&pxFileBuffer, '°');
        PXFileWriteI8U(&pxFileBuffer, '°');

        PXFileWriteI16UE(&pxFileBuffer, pxSBPEmitter->MessageID, PXEndianBig);
        PXFileWriteI16UE(&pxFileBuffer, packageSizeCurrent, PXEndianBig);

        if (i == 0)
        {
            PXFileWriteI8U(&pxFileBuffer, 0b01000000);
            PXFileWriteI16UE(&pxFileBuffer, messageSize, PXEndianBig); // Write size

            PXFileWriteB(&pxFileBuffer, dataSourcePoint, packageSizeCurrent - 3);
        }
        else
        {
            PXFileWriteB(&pxFileBuffer, dataSourcePoint, packageSizeCurrent); // Write size
        }

        PXBufferConstruct(&pxSBPEmitter->SocketSender->BufferOutput, pxFileBuffer.Data, pxFileBuffer.DataCursor, PXBufferTypeStack);
        sendResult = PXSocketSend(pxSBPEmitter->SocketSender, pxSBPEmitter->SocketReceiverID);
        PXBufferDestruct(&pxSBPEmitter->SocketSender->BufferOutput);
    }

    return sendResult;
}
#endif
