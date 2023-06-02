using System.Drawing;

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

    internal unsafe struct PXSocketDataMoveEventInfo
    {
        public PXSocket* SocketSending;
        public void* SocketReceiving;
        public void* Data;
        public void* DataSize;
    };

    public class SocketDataMoveEventInfo
    {
        public PX.Socket SocketSending;
        public uint SocketReceiving;
        public byte[] Data;
    };

    internal struct PXSocket
    {

    }

    internal struct PXSocketEventList
    {
        public PXSocketCreatingEvent SocketCreatingCallBack;
        public PXSocketCreatedEvent SocketCreatedCallBack;

        public PXSocketClosedEvent SocketClosedCallBack;

        public PXSocketConnectedEvent SocketConnectedCallBack;
        public PXSocketDisconnectedEvent SocketDisconnectedCallBack;

        public PXSocketStateChangedEvent SocketStateChangedCallBack;

        public PXSocketDataSendEvent SocketDataSendCallBack;
        public PXSocketDataReceiveEvent SocketDataReceiveCallBack;
    };

    internal unsafe delegate void PXSocketCreatingEvent(void* owner, ref PXSocket pxSocket, byte* use);
	internal unsafe delegate void PXSocketCreatedEvent(void* owner, ref PXSocket pxSocket);

    internal unsafe delegate void PXSocketClosedEvent(void* owner, ref PXSocket pxSocket);

    internal unsafe delegate void PXSocketConnectedEvent(void* owner, ref PXSocket serverSocket, ref PXSocket clientSocket);
	internal unsafe delegate void PXSocketDisconnectedEvent(void* owner, ref PXSocket serverSocket, ref PXSocket clientSocket);

	internal unsafe delegate void PXSocketStateChangedEvent(void* owner, ref PXSocket pxSocket);

	internal unsafe delegate void PXSocketDataSendEvent(void* owner, ref PXSocketDataMoveEventInfo pxSocketDataMoveEventInfo);
	internal unsafe delegate void PXSocketDataReceiveEvent(void* owner, ref PXSocketDataMoveEventInfo pxSocketDataMoveEventInfo);



    public delegate void SocketCreatingEvent(PX.Socket pxSocket, ref bool use);
    public delegate void SocketCreatedEvent(PX.Socket pxSocket);

    public delegate void SocketClosedEvent(PX.Socket pxSocket);

    public delegate void SocketConnectedEvent(PX.Socket serverSocket, PX.Socket clientSocket);
    public delegate void SocketDisconnectedEvent(PX.Socket serverSocket, PX.Socket clientSocket);

    public delegate void SocketStateChangedEvent(PX.Socket pxSocket);

    public delegate void SocketDataSendEvent(PX.SocketDataMoveEventInfo pxSocketDataMoveEventInfo);
    public delegate void SocketDataReceiveEvent(PX.SocketDataMoveEventInfo pxSocketDataMoveEventInfo);


    public class Socket
    {
        

    }
}
