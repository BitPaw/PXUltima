using System;
using System.Runtime.InteropServices;

namespace PX
{
    internal unsafe struct PXSBPReceiverEventList
    {
        public void* Owner;

        public void* OnMessageUpdatedCallBack;
        public void* OnMessageReceivedCallBack;

        public void* OnChunkSegmentUpdatedCallBack;
        public void* OnChunkReceivedCallBack;

        public void* OnMessageInvalidCallBack;
    };


    [StructLayout(LayoutKind.Sequential, Size = 72)]
    internal unsafe struct PXSBPMessage
    {
        // Runtime info
        public fixed byte FirstKnown[10];
        public fixed byte LastKnown[10];

        public void* Owner;

        // Raw data
        public void* MessageData; // Payload data. Context of this data is not interpreted.
        public void* MessageSize;// Current size of the data chunk, cant be bigger than expected size.
        public void* MessageSizeCached;

        public int MessageBitSize;
        public int StorageType;

        // Delegation info
        public ushort ID; // ID that belongs to message itself. So that chunks can conbine them.

        public byte MessageSizeCachedInPercent;

        // Extended delegation
        public byte HasExtendedDelegation;
    }


    [UnmanagedFunctionPointer(CallingConvention.StdCall)] internal delegate void PXSBPOnMessageUpdatedFunction(ref PXSBPMessage pxSBPMessage); // If message has updated but not fully recieved
    [UnmanagedFunctionPointer(CallingConvention.StdCall)] internal delegate void PXSBPOnMessageReceivedFunction(ref PXSBPMessage pxSBPMessage); // if message is fully recieved

    public delegate void SBPOnMessageUpdatedFunction(SBPMessage sbpMessage); // If message has updated but not fully recieved
    public delegate void SBPOnMessageReceivedFunction(SBPMessage sbpMessage); // if message is fully recieved

    public class SBPMessage
    {
        // Runtime info
        public Time FirstKnown;
        public Time LastKnown;

        // Raw data
        public UIntPtr MessageData; // Payload data. Context of this data is not interpreted.
        public ulong MessageSize;// Current size of the data chunk, cant be bigger than expected size.
        public ulong MessageSizeCached;

        // Perma stored
        public byte[] MessageDataCopy;

        public int MessageBitSize;
        public int StorageType;

        // Delegation info
        public ushort ID; // ID that belongs to message itself. So that chunks can conbine them.

        public int MessageSizeCachedInPercent;

        // Extended delegation
        public bool HasExtendedDelegation;

        internal unsafe SBPMessage(ref PXSBPMessage pxSBPMessage)
        {
            // Runtime info
            FirstKnown = new Time(); // (PXTime*)pxSBPMessage.LastKnown
            LastKnown = new Time();

            // Raw data
            MessageData = (UIntPtr)pxSBPMessage.MessageData; // Payload data. Context of this data is not interpreted.
            MessageSize = (ulong)pxSBPMessage.MessageSize;// Current size of the data chunk, cant be bigger than expected size.
            MessageSizeCached = (ulong)pxSBPMessage.MessageSizeCached;

            MessageBitSize = pxSBPMessage.MessageBitSize;
            StorageType = pxSBPMessage.StorageType;

            // Delegation info
            ID = pxSBPMessage.ID; // ID that belongs to message itself. So that chunks can conbine them.

            MessageSizeCachedInPercent = pxSBPMessage.MessageSizeCachedInPercent;

            // Extended delegation
            HasExtendedDelegation = pxSBPMessage.HasExtendedDelegation != 0;
        }

        public override string ToString()
        {
            ulong delta = FirstKnown.MillisecondsDelta(LastKnown);

            return "Message (" + MessageSize + " Bytes in " + delta + " ms)";
        }
    }

}


#if false

namespace PX
{
    [StructLayout(LayoutKind.Sequential, Size = 64)]
    internal unsafe struct PXSBPMessage
    {
        byte* Data;
        ulong DataSize;
    }

    [StructLayout(LayoutKind.Sequential, Size = 128)]
    internal unsafe struct PXSBPChunk
    {
        int Dummy;
    }


    //internal delegate void PXSBPOnChunkSegmentUpdatedFunction (ref PXSBPChunkCache pxSBPChunkSegment);

    // Chunk
    internal delegate void PXSBPOnChunkReceivedFunction(ref PXSBPChunk pxSBPChunk);
    internal delegate void PXSBPOnChunkEmittedFunction(ref PXSBPChunk pxSBPChunk);

    // Message
    internal delegate void PXSBPOnMessageUpdatedFunction(ref PXSBPMessage pxSBPMessage);
    internal delegate void PXSBPOnMessageReceivedFunction(ref PXSBPMessage pxSBPMessage);


    // internal delegate string CallBack();

    public class SBPClient : IDisposable
    {
        [StructLayout(LayoutKind.Sequential, Size = 1800)]
        private struct PXSBPClient
        {
            int Dummy;
        }

        [DllImport("PXUltima.dll")] private static extern void PXSBPClientConstruct(ref PXSBPClient pxSBPClient);
        [DllImport("PXUltima.dll")] private static extern void PXSBPClientDestruct(ref PXSBPClient pxSBPClient);

        [DllImport("PXUltima.dll")] private static extern void PXSBPClientMessageReceivedCallBackSet(ref PXSBPClient pxSBPClient, PXSBPOnMessageUpdatedFunction pxSBPOnMessageReceivedFunction);
        [DllImport("PXUltima.dll")] private static extern void PXSBPClientOnMessageUpdatedCallBackSet(ref PXSBPClient pxSBPClient, PXSBPOnMessageReceivedFunction pxSBPOnMessageReceivedFunction);
       // [DllImport("PXUltima.dll")] private static extern void PXSBPClientChunkSegmentUpdatedCallBackSet(ref PXSBPClient pxSBPClient, PXSBPOnChunkSegmentUpdatedFunction pxSBPOnMessageReceivedFunction);
        [DllImport("PXUltima.dll")] private static extern void PXSBPClientChunkReceivedCallBackSet(ref PXSBPClient pxSBPClient, PXSBPOnChunkReceivedFunction pxSBPOnMessageReceivedFunction);



        [DllImport("PXUltima.dll")] private static extern ActionResult PXSBPClientConnectToServer(ref PXSBPClient pxSBPClient, ref PXText ip, ushort port);
        [DllImport("PXUltima.dll")] private static extern ActionResult PXSBPClientDisconnectFromServer(ref PXSBPClient pxSBPClient);

        [DllImport("PXUltima.dll")] private static extern unsafe void PXSBPClientSendMessage(ref PXSBPClient pxSBPClient, void* data, UIntPtr dataSize);
        [DllImport("PXUltima.dll")] private static extern void PXSBPClientSendFile(ref PXSBPClient pxSBPClient, ref PXText filePath);


        [UnmanagedFunctionPointer(CallingConvention.StdCall)]
        public delegate void SBPOnMessageReceivedFunction(string message);

        public event SBPOnMessageReceivedFunction OnMessageReceived;
        public event SBPOnMessageReceivedFunction OnMessageUpdated;
        public event SBPOnMessageReceivedFunction OnChunkReceived;



        private PXSBPClient _pxSBPClient = new PXSBPClient();

        private void OnMessageReceivedFunction(ref PXSBPMessage pxSBPMessage)
        {
            OnMessageReceived?.Invoke("EVENT TRIGGERED");
        }
        private void OnClientChunkReceivedFunction(ref PXSBPChunk pxSBPChunk)
        {
            OnChunkReceived?.Invoke("EVENT TRIGGERED");
        }

        private void OnClientOnMessageUpdatedFunction(ref PXSBPMessage pxSBPMessage)
        {
            OnMessageUpdated?.Invoke("EVENT TRIGGERED");
        }

        public SBPClient()
        {
            PXSBPClientConstruct(ref _pxSBPClient);

            PXSBPClientMessageReceivedCallBackSet(ref _pxSBPClient, OnMessageReceivedFunction);
            PXSBPClientOnMessageUpdatedCallBackSet(ref _pxSBPClient, OnClientOnMessageUpdatedFunction);
            PXSBPClientChunkReceivedCallBackSet(ref _pxSBPClient, OnClientChunkReceivedFunction);
        }  

        public void Dispose()
        {
            PXSBPClientDestruct(ref _pxSBPClient);
        }

        public unsafe ActionResult ConnectToServer(string ip, ushort port)
        {
            ActionResult actionResult = ActionResult.Invalid;

#if false
            Encoding encoding =  Encoding.GetEncoding(ip);

            byte[] bytes = Encoding.Default.GetBytes(ip);

            fixed(byte* adress = &bytes[0])
            {
                PXText pXText = new PXText();             

                pXText.SizeAllocated = (ulong)bytes.Length;
                pXText.SizeUsed = pXText.SizeAllocated;
                pXText.Format = 2; 
                pXText.NumberOfCharacters = 0;
                pXText.TextA = adress;

                return PXSBPClientConnectToServer(ref _pxSBPClient, ref pXText, port);               
            }  
            
#else

            fixed (char* adress = ip.ToCharArray())
            {
                PXText pXText = PXText.MakeFromStringW(adress, ip.Length);

                actionResult = PXSBPClientConnectToServer(ref _pxSBPClient, ref pXText, port);
            }

            return actionResult;
#endif
        }

        public ActionResult DisconnectFromServer()
        {
            return PXSBPClientDisconnectFromServer(ref _pxSBPClient);
        }

        public void SendData(string data)
        {
            SendData(Encoding.Default.GetBytes(data));
        }

        public unsafe void SendData(byte[] data)
        {
            fixed(byte* dataAdress = &data[0])
            {
                PXSBPClientSendMessage(ref _pxSBPClient, dataAdress, (UIntPtr)data.Length);
            }
        }
    }

 
    public class SBPServer : IDisposable
    {
        [StructLayout(LayoutKind.Sequential, Size = 1400)]
        private struct PXSBPServer
        {
            public int Dummy;
        }

        [DllImport("PXUltima.dll")] private static extern void PXSBPServerConstruct(ref PXSBPServer pxSBPServer);
        [DllImport("PXUltima.dll")] private static extern void PXSBPServerDestruct(ref PXSBPServer pxSBPServer);
        [DllImport("PXUltima.dll")] private static extern ActionResult PXSBPServerStart(ref PXSBPServer server, ushort port);
        [DllImport("PXUltima.dll")] private static extern ActionResult PXSBPServerStop(ref PXSBPServer server);


        [DllImport("PXUltima.dll")] private static extern void PXSBPServerMessageReceivedCallBackSet(ref PXSBPServer pxSBPServer, PXSBPOnMessageUpdatedFunction pxSBPOnMessageReceivedFunction);
        [DllImport("PXUltima.dll")] private static extern void PXSBPServerOnMessageUpdatedCallBackSet(ref PXSBPServer pxSBPServer, PXSBPOnMessageReceivedFunction pxSBPOnMessageReceivedFunction);
        //[DllImport("PXUltima.dll")] private static extern void PXSBPServerChunkSegmentUpdatedCallBackSet(ref PXSBPServer pxSBPServer, PXSBPOnChunkSegmentUpdatedFunction pxSBPOnMessageReceivedFunction);
        [DllImport("PXUltima.dll")] private static extern void PXSBPServerChunkReceivedCallBackSet(ref PXSBPServer pxSBPServer, PXSBPOnChunkReceivedFunction pxSBPOnMessageReceivedFunction);


        [UnmanagedFunctionPointer(CallingConvention.StdCall)]
        public delegate void SBPOnMessageReceivedFunction(string message);
        public event SBPOnMessageReceivedFunction OnMessageReceived;
        public event SBPOnMessageReceivedFunction OnMessageUpdated;
        public event SBPOnMessageReceivedFunction OnChunkReceived;

        private PXSBPServer _pxSBPServer = new PXSBPServer();

        private void OnChunkReceivedCallBackSet(ref PXSBPChunk pxSBPChunk)
        {
            OnMessageReceived?.Invoke("EVENT YEET");
        }

        private void OnServerOnMessageUpdatedCallBackSet(ref PXSBPMessage pxSBPMessage)
        {
            OnMessageUpdated?.Invoke("EVENT YEET");
        }

        private void OnServerMessageReceivedCallBackSet(ref PXSBPMessage pxSBPMessage)
        {
            OnChunkReceived?.Invoke("EVENT YEET");
        }

        public SBPServer()
        {
            PXSBPServerConstruct(ref _pxSBPServer);
            PXSBPServerMessageReceivedCallBackSet(ref _pxSBPServer, OnServerMessageReceivedCallBackSet);
            PXSBPServerOnMessageUpdatedCallBackSet(ref _pxSBPServer, OnServerOnMessageUpdatedCallBackSet);
            PXSBPServerChunkReceivedCallBackSet(ref _pxSBPServer, OnChunkReceivedCallBackSet);
        }      

        public void Dispose()
        {
            PXSBPServerDestruct(ref _pxSBPServer);
        }

        public ActionResult Start(ushort port)
        {
            return PXSBPServerStart(ref _pxSBPServer, port);
        }

        public ActionResult Stop()
        {
            return PXSBPServerStop(ref _pxSBPServer);
        }
    }
}

#endif