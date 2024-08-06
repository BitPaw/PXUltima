using System;
using System.Runtime.InteropServices;

namespace PX
{
    public enum IPAdressFamily
    {
        IPAdressFamilyInvalid,
        IPAdressFamilyUnspecified, // unspecified
        IPAdressFamilyUNIX, // local to host (pipes, portals)
        IPAdressFamilyINET, // internetwork: UDP, TCP, etc.
        IPAdressFamilyIMPLINK, // arpanet imp addresses
        IPAdressFamilyPUP, // pup protocols: e.g. BSP
        IPAdressFamilyCHAOS, // mit CHAOS protocols
        IPAdressFamilyNS, // XEROX NS protocols
        IPAdressFamilyIPX, // IPX protocols: IPX, SPX, etc.
        IPAdressFamilyISO, // ISO protocols
        IPAdressFamilyOSI, // OSI is ISO
        IPAdressFamilyECMA, // european computer manufacturers
        IPAdressFamilyDATAKIT, // datakit protocols
        IPAdressFamilyCCITT, // CCITT protocols, X.25 etc
        IPAdressFamilySNA, // IBM SNA
        IPAdressFamilyDECnet, // DECnet
        IPAdressFamilyDLI, // Direct data link interface
        IPAdressFamilyLAT, // LAT
        IPAdressFamilyHYLINK, // NSC Hyperchannel
        IPAdressFamilyAPPLETALK, // AppleTalk
        IPAdressFamilyNETBIOS, // NetBios-style addresses
        IPAdressFamilyVOICEVIEW, // VoiceView
        IPAdressFamilyFIREFOX, // Protocols from Firefox
        IPAdressFamilyUNKNOWN1, // Somebody is using this!
        IPAdressFamilyBAN, // Banyan
        IPAdressFamilyATM, // Native ATM Services
        IPAdressFamilyINET6, // Internetwork Version 6
        IPAdressFamilyCLUSTER, // Microsoft Wolfpack
        IPAdressFamilyIEEE12844, // IEEE 1284.4 WG AF
        IPAdressFamilyIRDA, // IrDA
        IPAdressFamilyNETDES, // Network Designers OSI & gateway
        IPAdressFamilyMAX, // depends
        IPAdressFamilyLINK,
        IPAdressFamilyHYPERV,
        IPAdressFamilyBTH, // Bluetooth RFCOMM/L2CAP protocols
        IPAdressFamilyTCNPROCESS,
        IPAdressFamilyTCNMESSAGE,
        IPAdressFamilyICLFXBM
    }


    public enum PXSocketType
    {
        PXSocketTypeInvalid,
        PXSocketTypeStream,// stream socket */
        PXSocketTypeDatagram, // datagram socket */
        PXSocketTypeRaw, // raw-protocol interface */
        PXSocketTypeRDM, // reliably-delivered message */
        PXSocketTypeSeqPacket // sequenced packet stream */
    }


    public enum ProtocolMode
    {
        ProtocolModeInvalid,
        ProtocolModeHOPOPTS,  // IPv6 Hop-by-Hop options
        ProtocolModeICMP,
        ProtocolModeIGMP,
        ProtocolModeGGP,
        ProtocolModeIPV4,
        ProtocolModeST,
        ProtocolModeTCP,
        ProtocolModeCBT,
        ProtocolModeEGP,
        ProtocolModeIGP,
        ProtocolModePUP,
        ProtocolModeUDP,
        ProtocolModeIDP,
        ProtocolModeRDP,
        ProtocolModeIPV6, // IPv6 header
        ProtocolModeROUTING, // IPv6 Routing header
        ProtocolModeFRAGMENT, // IPv6 fragmentation header
        ProtocolModeESP, // encapsulating security payload
        ProtocolModeAH, // authentication header
        ProtocolModeICMPV6, // ICMPv6
        ProtocolModeNONE, // IPv6 no next header
        ProtocolModeDSTOPTS, // IPv6 Destination options
        ProtocolModeND,
        ProtocolModeICLFXBM,
        ProtocolModePIM,
        ProtocolModePGM,
        ProtocolModeL2TP,
        ProtocolModeSCTP,
        ProtocolModeRAW,
        ProtocolModeMAX,
        //
        //  These are reserved for private use by Windows.
        //
        ProtocolModeWindowsRAW,
        ProtocolModeWindowsIPSEC,
        ProtocolModeWindowsIPSECOFFLOAD,
        ProtocolModeWindowsWNV,
        ProtocolModeWindowsMAX
    }

    public enum SocketState
    {
        NotInitialised,

        Initialised,

        IDLE, // Waiting for action, currently doing nothing
        EventPolling, // Polling for events on multible s
        Offline,

        Failed,

        // PXClient only
        Connecting,
        Connected,
        DataReceiving,
        DataSending,
        FileReceiving,
        FileSending,

        // PXServer only
    };

    internal unsafe struct PXSocketDataReceivedEventData
    {
        public void* Data;
        public void* DataSize;
        public PXSocket* SocketReceiving;
        public uint SocketSending;
    };

    internal unsafe struct PXSocketDataSendEventData
    {
        public void* Data;
        public void* DataSize;
        public PXSocket* SocketSending;
        public uint SocketReceiving;
    };

    public class SocketDataReceivedEventData
    {
        public uint SocketSending { get; private set; }
        public PX.Socket SocketReceiving { get; private set; }
        public UIntPtr DataAdress { get; private set; }
        public ulong DataSize { get; private set; }

        internal unsafe SocketDataReceivedEventData(ref PXSocketDataReceivedEventData pxSocketDataReceivedEventData)
        {
            SocketReceiving = new PX.Socket(pxSocketDataReceivedEventData.SocketReceiving);
            SocketSending= pxSocketDataReceivedEventData.SocketSending;
            DataAdress = (UIntPtr)pxSocketDataReceivedEventData.Data;
            DataSize = (ulong)pxSocketDataReceivedEventData.DataSize;
        }

        public override string ToString()
        {
            return "You <-- " + SocketSending + "(" + DataSize + " Bytes)";
        }
    }

    public class SocketDataSendEventData
    {
        public PX.Socket SocketSending { get; private set; }
        public uint SocketReceiving { get; private set; }
        public UIntPtr DataAdress { get; private set; }
        public ulong DataSize { get; private set; }

        internal unsafe SocketDataSendEventData(ref PXSocketDataSendEventData pxSocketDataMoveEventInfo)
        {
            SocketSending = new PX.Socket(pxSocketDataMoveEventInfo.SocketSending);
            SocketReceiving = pxSocketDataMoveEventInfo.SocketReceiving;
            DataAdress = (UIntPtr)pxSocketDataMoveEventInfo.Data;
            DataSize = (ulong)pxSocketDataMoveEventInfo.DataSize;
        }

        public override string ToString()
        {
            return "You --> " + SocketReceiving + "(" + DataSize + " Bytes)";
        }
    };


    [StructLayout(LayoutKind.Sequential, Size = 256)]
    internal struct PXSocket
    {

    }

    [StructLayout(LayoutKind.Sequential, Size = 64)]
    internal unsafe struct PXSocketEventList
    {
        /*
        [MarshalAs(UnmanagedType.FunctionPtr)] public PXSocketTEST PXSocketTESTCallBack;

        [MarshalAs(UnmanagedType.FunctionPtr)] public PXSocketCreatingEvent SocketCreatingCallBack;
        [MarshalAs(UnmanagedType.FunctionPtr)] public PXSocketCreatedEvent SocketCreatedCallBack;

        [MarshalAs(UnmanagedType.FunctionPtr)] public PXSocketClosedEvent SocketClosedCallBack;

        [MarshalAs(UnmanagedType.FunctionPtr)] public PXSocketConnectedEvent SocketConnectedCallBack;
        [MarshalAs(UnmanagedType.FunctionPtr)] public PXSocketDisconnectedEvent SocketDisconnectedCallBack;

        [MarshalAs(UnmanagedType.FunctionPtr)] public PXSocketStateChangedEvent SocketStateChangedCallBack;

        [MarshalAs(UnmanagedType.FunctionPtr)] public PXSocketDataSendEvent SocketDataSendCallBack;
        [MarshalAs(UnmanagedType.FunctionPtr)] public PXSocketDataReceiveEvent SocketDataReceiveCallBack;
        */

        public void* PXSocketTESTCallBack;

        public void* SocketCreatingCallBack;
        public void* SocketCreatedCallBack;

        public void* SocketClosedCallBack;

        public void* SocketConnectedCallBack;
        public void* SocketDisconnectedCallBack;

        public void* SocketStateChangedCallBack;

        public void* SocketDataSendCallBack;
        public void* SocketDataReceiveCallBack;

    };

    [UnmanagedFunctionPointer(CallingConvention.StdCall)] internal delegate void PXSocketCreatingEvent(UIntPtr owner, ref PXSocket pxSocket, ref byte use);
    [UnmanagedFunctionPointer(CallingConvention.StdCall)] internal delegate void PXSocketCreatedEvent(UIntPtr owner, ref PXSocket pxSocket);

    [UnmanagedFunctionPointer(CallingConvention.StdCall)] internal delegate void PXSocketClosedEvent(UIntPtr owner, ref PXSocket pxSocket);

    [UnmanagedFunctionPointer(CallingConvention.StdCall)] internal delegate void PXSocketConnectedEvent(UIntPtr owner, ref PXSocket serverSocket, ref PXSocket clientSocket);
    [UnmanagedFunctionPointer(CallingConvention.StdCall)] internal delegate void PXSocketDisconnectedEvent(UIntPtr owner, ref PXSocket serverSocket, ref PXSocket clientSocket);

    [UnmanagedFunctionPointer(CallingConvention.StdCall)] internal delegate void PXSocketStateChangedEvent(UIntPtr owner, ref PXSocket pxSocket, SocketState oldState, SocketState newState);

    [UnmanagedFunctionPointer(CallingConvention.StdCall)] internal delegate void PXSocketDataSendEvent(UIntPtr owner, ref PXSocketDataSendEventData pxSocketDataSendEventData);
    [UnmanagedFunctionPointer(CallingConvention.StdCall)] internal delegate void PXSocketDataReceiveEvent(UIntPtr owner, ref PXSocketDataReceivedEventData pxSocketDataReceivedEventData);



    public delegate void SocketCreatingEvent(PX.Socket pxSocket, ref bool use);
    public delegate void SocketCreatedEvent(PX.Socket pxSocket);

    public delegate void SocketClosedEvent(PX.Socket pxSocket);

    public delegate void SocketConnectedEvent(PX.Socket serverSocket, PX.Socket clientSocket);
    public delegate void SocketDisconnectedEvent(PX.Socket serverSocket, PX.Socket clientSocket);

    public delegate void SocketStateChangedEvent(PX.Socket pxSocket, SocketState oldState, SocketState newState);

    public delegate void SocketDataSendEvent(PX.SocketDataSendEventData socketDataSendEventData);
    public delegate void SocketDataReceiveEvent(PX.SocketDataReceivedEventData socketDataReceivedEventData);



    public class Socket
    {
        public Socket()
        {

        }
        internal Socket(PXSocket socket)
        {

        }
        internal unsafe Socket(PXSocket* socket)
        {

        }
    }
}
