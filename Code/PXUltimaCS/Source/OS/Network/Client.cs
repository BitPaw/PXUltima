using System;
using System.Runtime.InteropServices;

namespace PX
{
    public class Client : IDisposable
    {
        [StructLayout(LayoutKind.Sequential, Size = 600)]
        private struct PXClient
        {

        }

        [DllImport("PXUltima.dll")] private static extern PX.ActionResult PXClientConstruct(ref PXClient pxClient);
        [DllImport("PXUltima.dll")] private static extern PX.ActionResult PXClientDestruct(ref PXClient pxClient);

        [DllImport("PXUltima.dll")] private static extern unsafe PX.ActionResult PXClientSendData(ref PXClient pxClient, void* data, UIntPtr dataSize);

        [DllImport("PXUltima.dll")] private static extern PX.ActionResult PXClientConnectToServer(ref PXClient client, ref PXText ip, ushort port);
        [DllImport("PXUltima.dll")] private static extern PX.ActionResult PXClientDisconnectFromServer(ref PXClient client);

        private PXClient _pxClient = new PXClient();

        Client()
        {
            PXClientConstruct(ref _pxClient);
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
