using System;
using System.Runtime.InteropServices;

namespace PX
{
    public class Server : IDisposable
    {
        [StructLayout(LayoutKind.Sequential, Size = 160)]
        private struct PXServer
        {
            public IntPtr SocketCreatingCallBack;
            public IntPtr SocketCreatedCallBack;

            public IntPtr SocketClosedCallBack;

            public IntPtr SocketConnectedCallBack;
            public IntPtr SocketDisconnectedCallBack;

            public IntPtr SocketStateChangedCallBack;

            public IntPtr SocketDataSendCallBack;
            public IntPtr SocketDataReceiveCallBack;
        }

        [DllImport("PXUltima.dll", CallingConvention = CallingConvention.StdCall)] private static extern void PXServerConstruct(ref PXServer server);
        [DllImport("PXUltima.dll", CallingConvention = CallingConvention.StdCall)] private static extern void PXServerDestruct(ref PXServer server);

        [DllImport("PXUltima.dll", CallingConvention = CallingConvention.StdCall)] private static extern PX.ActionResult PXServerStart(ref PXServer server, ushort port, ProtocolMode protocolMode);
        [DllImport("PXUltima.dll", CallingConvention = CallingConvention.StdCall)] private static extern PX.ActionResult PXServerStop(ref PXServer server);
        [DllImport("PXUltima.dll", CallingConvention = CallingConvention.StdCall)] private static extern PX.ActionResult PXServerKickClient(ref PXServer server, uint socketID);

        private PXServer _pxServer;

        public event SocketCreatingEvent OnCreating;
        public event SocketCreatedEvent OnCreated;
        public event SocketClosedEvent OnClosed;
        public event SocketConnectedEvent OnConnected;
        public event SocketDisconnectedEvent OnDisconnected;
        public event SocketStateChangedEvent OnStateChanged;
        public event SocketDataSendEvent OnDataSend;
        public event SocketDataReceiveEvent OnDataReceive;

        public void Dispose()
        {
            PXServerDestruct(ref _pxServer);
        }

        public unsafe Server()
        {
            _pxServer = new PXServer();

            PXServerConstruct(ref _pxServer);

            _pxServer.SocketCreatingCallBack = Marshal.GetFunctionPointerForDelegate<PXSocketCreatingEvent>(OnSocketCreatingCallBack);
            _pxServer.SocketCreatedCallBack = Marshal.GetFunctionPointerForDelegate<PXSocketCreatedEvent>(OnSocketCreatedCallBack);
            _pxServer.SocketClosedCallBack = Marshal.GetFunctionPointerForDelegate<PXSocketClosedEvent>(OnSocketClosedCallBack);
            _pxServer.SocketConnectedCallBack = Marshal.GetFunctionPointerForDelegate<PXSocketConnectedEvent>(OnSocketConnectedCallBack);
            _pxServer.SocketDisconnectedCallBack = Marshal.GetFunctionPointerForDelegate<PXSocketDisconnectedEvent>(OnSocketDisconnectedCallBack);
            _pxServer.SocketStateChangedCallBack = Marshal.GetFunctionPointerForDelegate<PXSocketStateChangedEvent>(OnSocketStateChangedCallBack);
            _pxServer.SocketDataSendCallBack = Marshal.GetFunctionPointerForDelegate<PXSocketDataSendEvent>(OnSocketDataSendCallBack);
            _pxServer.SocketDataReceiveCallBack = Marshal.GetFunctionPointerForDelegate<PXSocketDataReceiveEvent>(OnSocketDataReceiveCallBack);
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

        private void OnSocketConnectedCallBack(UIntPtr owner, ref PXSocket serverSocket, ref PXSocket clientSocket)
        {
            OnConnected?.Invoke(new Socket(serverSocket), new Socket(clientSocket));
        }

        private void OnSocketClosedCallBack(UIntPtr owner, ref PXSocket pxSocket)
        {
            OnClosed?.Invoke(new Socket(pxSocket));
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

        public ActionResult Start(ushort port, ProtocolMode protocolMode)
        {
            return PXServerStart(ref _pxServer, port, protocolMode);
        }

        public ActionResult Stop()
        {
            return PXServerStop(ref _pxServer);
        }

        public ActionResult KiclClient(uint socketID)
        {
            return PXServerKickClient(ref _pxServer, socketID);
        }
    }
}