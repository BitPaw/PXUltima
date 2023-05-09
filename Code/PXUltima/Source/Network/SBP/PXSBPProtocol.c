#include "PXSBPProtocol.h"

#include <OS/File/PXFile.h>
#include <OS/Memory/PXMemory.h>
#include <Math/PXMath.h>
#include <OS/User/PXUser.h>
#include <Event/PXEvent.h>

void PXSBPMessageConstruct(PXSBPMessage* const pxSBPMessage)
{
    MemoryClear(pxSBPMessage, sizeof(PXSBPMessage));

    pxSBPMessage->ID = -1;

    PXTimeNow(&pxSBPMessage->FirstKnown);

    pxSBPMessage->LastKnown = pxSBPMessage->FirstKnown;
}
PXBool PXSBPMessageChunkDataIsComplete(const PXSBPChunkSegment* const pxSBPMessageChunk)
{
    return pxSBPMessageChunk->SizeExpected <= pxSBPMessageChunk->SizeCurrent;
}

PXBool PXSBPMessageChunkDataContainsMultible(const PXSBPChunkSegment* const pxSBPMessageChunk)
{
    return pxSBPMessageChunk->SizeExpected < pxSBPMessageChunk->SizeCurrent;
}

void PXSBPReceiverConstruct(PXSBPReceiver* const pxSBPReceiver)
{
    MemoryClear(pxSBPReceiver, sizeof(PXSBPReceiver));

    pxSBPReceiver->State = PXSBPRecieverStateAwaitingHeaderBegin;

    //PXDictionaryConstruct(&pxSBPClient->Receiver.MessageLookup, sizeof(PXMessageID), sizeof(PXSBPMessage), PXDictionaryValueLocalityInternalEmbedded);
}

void PXSBPServerConstruct(PXSBPServer* const pxSBPServer)
{
    PXServerConstruct(&pxSBPServer->Server);
    PXSBPReceiverConstruct(&pxSBPServer->Receiver);
    PXSBPEmitterConstruct(&pxSBPServer->Emitter);

    pxSBPServer->Server.EventListener.MessageReceiveCallback = PXSBPOnDataRawReceive;
    pxSBPServer->Server.Owner = &pxSBPServer->Receiver;
}

void PXSBPServerDestruct(PXSBPServer* const pxSBPServer)
{
    PXServerDestruct(&pxSBPServer->Server);
    //PXSBPReceiverConstruct(&pxSBPServer->Receiver);
    //PXSBPEmitterConstruct(&pxSBPServer->Emitter);
}

void PXSBPClientConstruct(PXSBPClient* const pxSBPClient)
{
    PXClientConstruct(&pxSBPClient->Client);
    PXSBPReceiverConstruct(&pxSBPClient->Receiver);
    PXSBPEmitterConstruct(&pxSBPClient->Emitter);

    pxSBPClient->Client.EventListener.MessageReceiveCallback = PXSBPOnDataRawReceive;
    pxSBPClient->Client.Owner = &pxSBPClient->Receiver;
}

void PXSBPClientDestruct(PXSBPClient* const pxSBPClient)
{
    PXClientDestruct(&pxSBPClient->Client);
}

void PXSBPClientSendMessage(PXSBPClient* const pxSBPClient, const void* const data, const PXSize dataSize)
{
    // Get free message ID channel.
    const PXInt16U messageID = 0xBEEF;

    // How many packages do we have?
    const PXSize numberOfPackages = (dataSize / pxSBPClient->Emitter.PackageSizeMaximal)+1;
    PXSize accumulator = dataSize;

    for (PXSize i = 0; i < numberOfPackages; ++i)
    {
        PXInt16U packageSizeCurrent = PXMathMinimumIU(accumulator, pxSBPClient->Emitter.PackageSizeMaximal);
        const PXAdress dataSourcePoint = (PXAdress)data + (dataSize - accumulator);

        accumulator -= packageSizeCurrent;


        if (i == 0)
        {
            packageSizeCurrent += 3u;
        }


        // write header
        PXByte header[PXSBPMessageChunkHeaderSize] = { '°','°' }; 

        PXInt16SplittEndianBig(messageID, header[2], header[3]); // Write ID 
        PXInt16SplittEndianBig(packageSizeCurrent, header[4], header[5]); // Write size

        PXSBPEmitterBeginAndReset(&pxSBPClient->Emitter);
        PXSBPEmitterAppend(&pxSBPClient->Emitter, header, PXSBPMessageChunkHeaderSize); // Write header

        if (i == 0)
        {
            PXByte beginHeader[3] = { 0b01000000, 0, 0 };

            PXInt16SplittEndianBig(dataSize, beginHeader[1], beginHeader[2]); // Write size

            PXSBPEmitterAppend(&pxSBPClient->Emitter, beginHeader, sizeof(beginHeader));
            PXSBPEmitterAppend(&pxSBPClient->Emitter, dataSourcePoint, packageSizeCurrent-3); // Write size
        }
        else
        {
            PXSBPEmitterAppend(&pxSBPClient->Emitter, dataSourcePoint, packageSizeCurrent); // Write size
        }

  
       // PXSBPEmitterEndAndDeploy(&pxSBPClient->Emitter);

        PXClientSendData(&pxSBPClient->Client, pxSBPClient->Emitter.Cache, pxSBPClient->Emitter.CacheSizeCurrent);
    }
}

void PXSBPClientSendFile(PXSBPClient* const pxSBPClient, const PXText* const filePath)
{
   
}

PXBool PXSBPMessageChunkParse(PXSBPChunkSegment* const pxSBPMessageChunk, const void* const data, const PXSize dataSize)
{
    if (dataSize < PXSBPMessageChunkHeaderSize)
    {
        MemoryClear(pxSBPMessageChunk, sizeof(PXSBPChunkSegment));
        return PXFalse;
    }

    const PXBool hasValidSignature = ((PXAdress)data)[0] == (PXByte)'°' && ((PXAdress)data)[1] == (PXByte)'°';

    if (!hasValidSignature)
    {
        MemoryClear(pxSBPMessageChunk, sizeof(PXSBPChunkSegment));
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

PXInt16U PXSBPMessageSizeMissing(PXSBPChunkSegment* const pxSBPMessage)
{
    return pxSBPMessage->SizeExpected - pxSBPMessage->SizeCurrent;
}

PXBool PXSBPMessageSizeComplete(PXSBPChunkSegment* const pxSBPMessage)
{
    return pxSBPMessage->SizeCurrent == pxSBPMessage->SizeExpected;
}

PXSize PXSBPMessageChunkDataConsume(PXSBPChunkSegment* const pxSBPMessage, const void* const data, const PXSize dataSize)
{
    const PXSize before = PXSBPMessageSizeMissing(pxSBPMessage);
    const PXAdress* adress = (PXAdress)pxSBPMessage->Message + pxSBPMessage->SizeCurrent;
    const PXSize copyedSize = MemoryCopy(data, dataSize, adress, before);

    pxSBPMessage->SizeCurrent += copyedSize;

    return copyedSize;
}

void PXSBPOnDataRawReceive(PXSBPReceiver* const pxSBPReceiver, const PXSocketDataMoveEventInfo* const pxSocketDataMoveEventInfo)
{
    switch (pxSBPReceiver->State)
    {
        case PXSBPRecieverStateAwaitingHeaderBegin:
        {         
            MemoryClear(&pxSBPReceiver->MessageChunkCurrent, sizeof(PXSBPChunkSegment)); // Clear current message

            // Check if message is too short
            {
                const PXBool isMessageTooShort = pxSocketDataMoveEventInfo->DataSize < PXSBPMessageChunkHeaderSize;

                if (isMessageTooShort) // The incomming message is not long enough, we jump into the next state
                {
                    pxSBPReceiver->HeaderCacheSize += MemoryCopy(pxSBPReceiver->HeaderCache, PXSBPMessageChunkHeaderSize, pxSocketDataMoveEventInfo->Data, pxSocketDataMoveEventInfo->DataSize); // Write into buffer

                    PXSBPReceiverStateChanged(pxSBPReceiver, PXSBPRecieverStateAwaitingHeaderEnd); // Wait for header end
                    break; // Finished, parsed part of the header
                }
            }

            // Try parse message
            {
                const PXBool success = PXSBPMessageChunkParse(&pxSBPReceiver->MessageChunkCurrent, pxSocketDataMoveEventInfo->Data, pxSocketDataMoveEventInfo->DataSize); // Parse full header

                if (!success)
                {
                    // Invalid header detected
                   // InvokeEvent(pxSBPReceiver->OnMessageInvalidCallBack, receiveSocket, clientSocketID);
                    break;
                }
            }
        
            // if message is exactly just the header
            {
                const PXBool isHeaderOnly = pxSocketDataMoveEventInfo->DataSize == PXSBPMessageChunkHeaderSize;

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

                    PXSBPMessageChunkDataConsume(&pxSBPReceiver->MessageChunkCurrent, pxSocketDataMoveEventInfo->Data, pxSocketDataMoveEventInfo->DataSize);

                    PXSBPReceiverStateChanged(pxSBPReceiver, PXSBPRecieverStateAwaitingDataEnd);

                    InvokeEvent(pxSBPReceiver->OnMessageProgressUpdatedCallBack, 0);
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
                        PXSocketDataMoveEventInfo pxSocketDataMoveEventInfoSubSegment;
                        pxSocketDataMoveEventInfoSubSegment.SocketSending = pxSocketDataMoveEventInfo->SocketSending;
                        pxSocketDataMoveEventInfoSubSegment.SocketReceiving = pxSocketDataMoveEventInfo->SocketReceiving;
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
            const PXSize consumedBytes = PXSBPMessageChunkDataConsume(&pxSBPReceiver->MessageChunkCurrent, pxSocketDataMoveEventInfo->Data, pxSocketDataMoveEventInfo->DataSize);
            const PXSize remainingData = pxSocketDataMoveEventInfo->DataSize - consumedBytes;
            const PXBool hasAdditionalData = 0 < remainingData; // Parsing was ok, next block

            const PXBool isComplete = PXSBPMessageChunkDataIsComplete(&pxSBPReceiver->MessageChunkCurrent);

            if (!isComplete)
            {
                // Wait for additional extrended data
                InvokeEvent(pxSBPReceiver->OnMessageProgressUpdatedCallBack, 0);
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
                    PXSocketDataMoveEventInfo pxSocketDataMoveEventInfoSubSegment;
                    pxSocketDataMoveEventInfoSubSegment.SocketSending = pxSocketDataMoveEventInfo->SocketSending;
                    pxSocketDataMoveEventInfoSubSegment.SocketReceiving = pxSocketDataMoveEventInfo->SocketReceiving;
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
            pxSBPMessage->MessageSizeCached += MemoryCopy
            (
                (PXAdress)pxSBPMessage->MessageData + pxSBPMessage->MessageSizeCached,
                pxSBPMessage->MessageSize - pxSBPMessage->MessageSizeCached,
                pxSBPChunk->Data,
                pxSBPChunk->DataSize
            );

            pxSBPMessage->MessageSizeCachedInPercent = pxSBPMessage->MessageSizeCached / pxSBPMessage->MessageSize;

            // is done?
            if (pxSBPMessage->MessageSizeCached == pxSBPMessage->MessageSize)
            {
                // We are done
                InvokeEvent(pxSBPReceiver->OnMessageReceivedCallBack, pxSBPChunk);

                PXDictionaryRemove(&pxSBPReceiver->MessageStreamLookup, &pxSBPChunk->ChannelID);

                return;
            }

            InvokeEvent(pxSBPReceiver->OnMessageProgressUpdatedCallBack, pxSBPChunk);
        }
    }

    // Is first chunk, read header and register
    {
        PXSBPMessage pxSBPMessage;

        PXFile pxFile;
        PXFileBufferExternal(&pxFile, pxSBPChunk->Data, pxSBPChunk->DataSize);

        const PXByte flagValue = 0;

        PXFileReadI8U(&pxFile, &flagValue);

        const PXSBPMessageBitSize pxSBPMessageBitSize = (PXSBPMessageBitSize)((flagValue & 0b11000000) >> 6u) + 1;
        pxSBPMessage.HasExtendedDelegation = (flagValue & 0b00100000) >> 5u;

        switch (pxSBPMessageBitSize)
        {
            case PXSBPMessageBitSize8Bit:
            {
                PXInt8U size;

                PXFileReadI8U(&pxFile, &size);

                pxSBPMessage.MessageSize = size;

                break;
            }
            case PXSBPMessageBitSize16Bit:
            {
                PXInt16U size;

                PXFileReadI16UE(&pxFile, &size, PXEndianBig);

                pxSBPMessage.MessageSize = size;

                break;
            }
            case PXSBPMessageBitSize32Bit:
            {
                PXInt32U size;

                PXFileReadI32UE(&pxFile, &size, PXEndianBig);

                pxSBPMessage.MessageSize = size;

                break;
            }
            case PXSBPMessageBitSize64Bit:
            {
                PXInt64U size;

                PXFileReadI64UE(&pxFile, &size, PXEndianBig);

                pxSBPMessage.MessageSize = size;

                break;
            }
        }

        PXDictionaryAdd(&pxSBPReceiver->MessageStreamLookup, &pxSBPChunk->ChannelID, &pxSBPMessage);
    }    
}

void PXSBPEmitterConstruct(PXSBPEmitter* const pxSBPEmitter)
{
    MemoryClear(pxSBPEmitter, sizeof(PXSBPEmitter));

    pxSBPEmitter->PackageSizeMaximal = 1024u;
    pxSBPEmitter->CacheSizeMax = 1024;
}

void PXSBPEmitterBeginAndReset(PXSBPEmitter* const pxSBPEmitter)
{
#if 1
    MemorySet(pxSBPEmitter->Cache, '#', pxSBPEmitter->CacheSizeMax);
#endif // 1

    pxSBPEmitter->CacheSizeCurrent = 0;
}

void PXSBPEmitterAppend(PXSBPEmitter* const pxSBPEmitter, const void* data, const PXSize dataSize)
{
    pxSBPEmitter->CacheSizeCurrent += MemoryCopy
    (
        data,
        dataSize,
        pxSBPEmitter->Cache + pxSBPEmitter->CacheSizeCurrent,
        pxSBPEmitter->CacheSizeMax - pxSBPEmitter->CacheSizeCurrent    
    );
}

void PXSBPEmitterEndAndDeploy(PXSBPEmitter* const pxSBPEmitter)
{

}

void PXSBPEmitterSendChunk(PXSBPEmitter* const pxSBPEmitter, const void* data, const PXSize dataSize)
{

}