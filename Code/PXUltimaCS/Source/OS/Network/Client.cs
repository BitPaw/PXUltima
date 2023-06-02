using System;
using System.Runtime.InteropServices;

namespace PX
{
    public class Client : IDisposable
    {
        [StructLayout(LayoutKind.Sequential, Size = 600)]
        private struct PXClient
        {
            public PXSocketEventList EventList;
        }

        [DllImport("PXUltima.dll")] private static extern PX.ActionResult PXClientConstruct(ref PXClient pxClient);
        [DllImport("PXUltima.dll")] private static extern PX.ActionResult PXClientDestruct(ref PXClient pxClient);

        [DllImport("PXUltima.dll")] private static extern unsafe PX.ActionResult PXClientSendData(ref PXClient pxClient, void* data, UIntPtr dataSize);

        [DllImport("PXUltima.dll")] private static extern PX.ActionResult PXClientConnectToServer(ref PXClient client, ref PXText ip, ushort port);
        [DllImport("PXUltima.dll")] private static extern PX.ActionResult PXClientDisconnectFromServer(ref PXClient client);

        private PXClient _pxClient = new PXClient();


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
            PXClientConstruct(ref _pxClient);

            _pxClient.EventList.SocketCreatingCallBack = OnSocketCreatingCallBack;
            _pxClient.EventList.SocketCreatedCallBack = OnSocketCreatedCallBack;
            _pxClient.EventList.SocketClosedCallBack = OnSocketClosedCallBack;
            _pxClient.EventList.SocketConnectedCallBack = OnSocketConnectedCallBack;
            _pxClient.EventList.SocketDisconnectedCallBack = OnSocketDisconnectedCallBack;
            _pxClient.EventList.SocketStateChangedCallBack = OnSocketStateChangedCallBack;
            _pxClient.EventList.SocketDataSendCallBack = OnSocketDataSendCallBack;
            _pxClient.EventList.SocketDataReceiveCallBack = OnSocketDataReceiveCallBack;
        }

        private unsafe void OnSocketDataReceiveCallBack(void* owner, ref PXSocketDataMoveEventInfo pxSocketDataMoveEventInfo)
        {
            throw new NotImplementedException();
        }

        private unsafe void OnSocketDataSendCallBack(void* owner, ref PXSocketDataMoveEventInfo pxSocketDataMoveEventInfo)
        {
            throw new NotImplementedException();
        }

        private unsafe void OnSocketStateChangedCallBack(void* owner, ref PXSocket pxSocket)
        {
            throw new NotImplementedException();
        }

        private unsafe void OnSocketDisconnectedCallBack(void* owner, ref PXSocket serverSocket, ref PXSocket clientSocket)
        {
            throw new NotImplementedException();
        }

        private unsafe void OnSocketConnectedCallBack(void* owner, ref PXSocket serverSocket, ref PXSocket clientSocket)
        {
            throw new NotImplementedException();
        }

        private unsafe void OnSocketCreatedCallBack(void* owner, ref PXSocket pxSocket)
        {
            throw new NotImplementedException();
        }

        private unsafe void OnSocketCreatingCallBack(void* owner, ref PXSocket pxSocket, byte* use)
        {
            throw new NotImplementedException();
        }

        private unsafe void OnSocketClosedCallBack(void* owner, ref PXSocket pxSocket)
        {
            throw new NotImplementedException();
        }

        public void Dispose()
        {
            PXClientDestruct(ref _pxClient);
        }

        public unsafe ActionResult SendData(void* data, UIntPtr dataSize)
        {
            return PXClientSendData(ref _pxClient, data, dataSize);
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
