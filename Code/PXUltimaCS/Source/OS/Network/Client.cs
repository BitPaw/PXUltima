using System;
using System.Runtime.InteropServices;

namespace PX
{
    public class Client : IDisposable
    {
        [StructLayout(LayoutKind.Sequential, Size = 500)]
        private unsafe struct PXClient
        {
            public void* SocketCreatingCallBack;
            public void* SocketCreatedCallBack;

            public void* SocketClosedCallBack;

            public void* SocketConnectedCallBack;
            public void* SocketDisconnectedCallBack;

            public void* SocketStateChangedCallBack;

            public void* SocketDataSendCallBack;
            public void* SocketDataReceiveCallBack;


            //public PXSocketEventList EventList;
            //public PXSocket Client;
            //public PXSocket Server;
            //public void* Owner;
        }

        [DllImport("PXUltima.dll", CallingConvention = CallingConvention.Cdecl)] private static extern void PXClientConstruct(ref PXClient pxClient);
        [DllImport("PXUltima.dll", CallingConvention = CallingConvention.Cdecl)] private static extern void PXClientDestruct(ref PXClient pxClient);
        [DllImport("PXUltima.dll", CallingConvention = CallingConvention.Cdecl)] private static extern PX.ActionResult PXClientSendData(ref PXClient pxClient, IntPtr data, IntPtr dataSize);
        [DllImport("PXUltima.dll", CallingConvention = CallingConvention.Cdecl)] private static extern PX.ActionResult PXClientConnectToSelf(ref PXClient client, ushort port);
        [DllImport("PXUltima.dll", CallingConvention = CallingConvention.Cdecl)] private static extern PX.ActionResult PXClientConnectToServer(ref PXClient client, ref PXText ip, ushort port);
        [DllImport("PXUltima.dll", CallingConvention = CallingConvention.Cdecl)] private static extern PX.ActionResult PXClientDisconnectFromServer(ref PXClient client);

       // [MarshalAs(UnmanagedType.ByValArray, SizeConst = 584)]
        private PXClient _pxClient;

        public event SocketCreatingEvent OnCreating;
        public event SocketCreatedEvent OnCreated;
        public event SocketClosedEvent OnClosed;
        public event SocketConnectedEvent OnConnected;
        public event SocketDisconnectedEvent OnDisconnected;
        public event SocketStateChangedEvent OnStateChanged;
        public event SocketDataSendEvent OnDataSend;
        public event SocketDataReceiveEvent OnDataReceive;

        public unsafe Client()
        {
            _pxClient = new PXClient();

            PXClientConstruct(ref _pxClient);

            //_pxClient.SocketCreatingCallBack = Marshal.GetFunctionPointerForDelegate<PXSocketCreatingEvent>(OnSocketCreatingCallBack).ToPointer();
            //_pxClient.SocketCreatedCallBack = Marshal.GetFunctionPointerForDelegate<PXSocketCreatedEvent>(OnSocketCreatedCallBack).ToPointer();
            //_pxClient.SocketClosedCallBack = Marshal.GetFunctionPointerForDelegate<PXSocketClosedEvent>(OnSocketClosedCallBack).ToPointer();
            //_pxClient.SocketConnectedCallBack = Marshal.GetFunctionPointerForDelegate<PXSocketConnectedEvent>(OnSocketConnectedCallBack).ToPointer();
            //_pxClient.SocketDisconnectedCallBack = Marshal.GetFunctionPointerForDelegate<PXSocketDisconnectedEvent>(OnSocketDisconnectedCallBack).ToPointer();
            //_pxClient.SocketStateChangedCallBack = Marshal.GetFunctionPointerForDelegate<PXSocketStateChangedEvent>(OnSocketStateChangedCallBack).ToPointer();
            //_pxClient.SocketDataSendCallBack = Marshal.GetFunctionPointerForDelegate<PXSocketDataSendEvent>(OnSocketDataSendCallBack).ToPointer();
            //_pxClient.SocketDataReceiveCallBack = Marshal.GetFunctionPointerForDelegate<PXSocketDataReceiveEvent>(OnSocketDataReceiveCallBack).ToPointer();
        }

        private void OnSocketDataReceiveCallBack(UIntPtr owner, ref PXSocketDataReceivedEventData pxSocketDataReceivedEventData)
        {
            OnDataReceive?.Invoke(new SocketDataReceivedEventData(ref pxSocketDataReceivedEventData));
        }

        private void OnSocketDataSendCallBack(UIntPtr owner, ref PXSocketDataSendEventData pxSocketDataSendEventData)
        {
            OnDataSend?.Invoke(new SocketDataSendEventData(ref pxSocketDataSendEventData));
        }

        private void OnSocketStateChangedCallBack(UIntPtr owner, ref PXSocket pxSocket, SocketState oldState, SocketState newState)
        {
            OnStateChanged?.Invoke(new Socket(pxSocket), oldState, newState);
        }

        private void OnSocketDisconnectedCallBack(UIntPtr owner, ref PXSocket serverSocket, ref PXSocket clientSocket)
        {
            OnDisconnected?.Invoke(new Socket(serverSocket), new Socket(clientSocket));
        }

        private void OnSocketConnectedCallBack(UIntPtr objectowner, ref PXSocket serverSocket, ref PXSocket clientSocket)
        {
            OnConnected?.Invoke(new Socket(serverSocket), new Socket(clientSocket));
        }

        private void OnSocketCreatedCallBack(UIntPtr owner, ref PXSocket pxSocket)
        {
            OnCreated?.Invoke(new Socket(pxSocket));
        }
        
        private void OnSocketCreatingCallBack(UIntPtr owner, ref PXSocket pxSocket, ref byte use)
        {
            Socket socket = new Socket(pxSocket);
            bool useBool = true;

            OnCreating?.Invoke(socket, ref useBool);

            use = Convert.ToByte(useBool);
        }

        private void OnSocketClosedCallBack(UIntPtr owner, ref PXSocket pxSocket)
        {
           OnClosed?.Invoke(new Socket(pxSocket));
        }

        public void Dispose()
        {
            PXClientDestruct(ref _pxClient);
        }

        public unsafe ActionResult SendData(string message)
        {
            fixed (char* messageAdress = message.ToCharArray())
            {
                return SendData((IntPtr)messageAdress, (IntPtr)(message.Length * sizeof(char)));
            }
        }

        public ActionResult SendData(IntPtr data, IntPtr dataSize)
        {
            return PXClientSendData(ref _pxClient, data, dataSize);
        }

        public ActionResult ConnectToServer(ushort port)
        {
            return PXClientConnectToSelf(ref _pxClient, port);
        }

        public unsafe ActionResult ConnectToServer(string ip, ushort port)
        {
            fixed (char* charBuffer = ip.ToCharArray())
            {
                PXText pXText = PXText.MakeFromStringW(charBuffer, ip.Length);

                return PXClientConnectToServer(ref _pxClient, ref pXText, port);
            }             
        }

        public ActionResult DisconnectFromServer()
        {
            return PXClientDisconnectFromServer(ref _pxClient);
        }
    }
}