using System;
using System.Runtime.InteropServices;
using System.Text;

namespace PX
{
    [StructLayout(LayoutKind.Sequential, Size = 64)]
    internal unsafe struct PXSBPMessage
    {
        byte* Data;
        ulong DataSize;
    }

    internal delegate void PXSBPOnMessageReceivedFunction(ref PXSBPMessage pxSBPMessage);

   // internal delegate string CallBack();

    public class SBPClient : IDisposable
    {
        [StructLayout(LayoutKind.Sequential, Size = 1784)]
        private struct PXSBPClient
        {
            int dummy;
        }

        [DllImport("PXUltima.dll")] private static extern void PXSBPClientConstruct(ref PXSBPClient pxSBPClient);
        [DllImport("PXUltima.dll")] private static extern void PXSBPClientDestruct(ref PXSBPClient pxSBPClient);

        [DllImport("PXUltima.dll")] private static extern void PXSBPClientMessageReceivedCallBackAdd(ref PXSBPClient pxSBPClient, PXSBPOnMessageReceivedFunction pxSBPOnMessageReceivedFunction);


        [DllImport("PXUltima.dll")] private static extern ActionResult PXSBPClientConnectToServer(ref PXSBPClient pxSBPClient, ref PXText ip, ushort port);
        [DllImport("PXUltima.dll")] private static extern ActionResult PXSBPClientDisconnectFromServer(ref PXSBPClient pxSBPClient);

        [DllImport("PXUltima.dll")] private static extern unsafe void PXSBPClientSendMessage(ref PXSBPClient pxSBPClient, void* data, UIntPtr dataSize);
        [DllImport("PXUltima.dll")] private static extern void PXSBPClientSendFile(ref PXSBPClient pxSBPClient, ref PXText filePath);


        public delegate void SBPOnMessageReceivedFunction(string message);

        public event SBPOnMessageReceivedFunction OnMessageReceived;

        private PXSBPClient _pxSBPClient = new PXSBPClient();

        internal void PXSBPOnMessageReceivedFunction(ref PXSBPMessage pxSBPMessage)
        {
            OnMessageReceived?.Invoke("EVENT TRIGGERED");
        }

        public SBPClient()
        {
            PXSBPClientConstruct(ref _pxSBPClient);

            PXSBPClientMessageReceivedCallBackAdd(ref _pxSBPClient, PXSBPOnMessageReceivedFunction);
        }

        public void Dispose()
        {
            PXSBPClientDestruct(ref _pxSBPClient);
        }

        public unsafe ActionResult ConnectToServer(string ip, ushort port)
        {

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
                PXText pXText = new PXText();

                pXText.SizeAllocated = (ulong)ip.Length;
                pXText.SizeUsed = pXText.SizeAllocated;
                pXText.Format = 2;
                pXText.NumberOfCharacters = 0;
                pXText.TextW = adress;

                return PXSBPClientConnectToServer(ref _pxSBPClient, ref pXText, port);
            }
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

    [StructLayout(LayoutKind.Sequential, Size = 1352)]
    public class SBPServer : IDisposable
    {
        private struct PXSBPServer
        {
            int dummy;
        }

        [DllImport("PXUltima.dll")] private static extern void PXSBPServerConstruct(ref PXSBPServer pxSBPServer);
        [DllImport("PXUltima.dll")] private static extern void PXSBPServerDestruct(ref PXSBPServer pxSBPServer);
        [DllImport("PXUltima.dll")] private static extern ActionResult PXSBPServerStart(ref PXSBPServer server, ushort port);
        [DllImport("PXUltima.dll")] private static extern ActionResult PXSBPServerStop(ref PXSBPServer server);


        private PXSBPServer _pxSBPServer = new PXSBPServer();

        public SBPServer()
        {
            PXSBPServerConstruct(ref _pxSBPServer);
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
