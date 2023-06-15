using System;
using System.Runtime.InteropServices;

namespace PX
{
    public class SBPClient : IDisposable
    {
        [StructLayout(LayoutKind.Sequential, Size = 1400)]
        private struct PXSBPClient
        {

        }

        [DllImport("PXUltima.dll", CallingConvention = CallingConvention.Cdecl)] private static extern void PXSBPClientConstruct(ref PXSBPClient pxSBPClient);
        [DllImport("PXUltima.dll", CallingConvention = CallingConvention.Cdecl)] private static extern void PXSBPClientDestruct(ref PXSBPClient pxSBPClient);

        [DllImport("PXUltima.dll", CallingConvention = CallingConvention.Cdecl)] private static extern void PXSBPClientReceiverEventListSet(ref PXSBPClient pxSBPReceiver, ref PXSBPReceiverEventList pxSBPReceiverEventList);

        [DllImport("PXUltima.dll", CallingConvention = CallingConvention.Cdecl)] private static extern PX.ActionResult PXSBPClientConnectToServer(ref PXSBPClient pxSBPClient, ref PXText ip, ushort port);
        [DllImport("PXUltima.dll", CallingConvention = CallingConvention.Cdecl)] private static extern PX.ActionResult PXSBPClientDisconnectFromServer(ref PXSBPClient pxSBPClient);

        [DllImport("PXUltima.dll", CallingConvention = CallingConvention.Cdecl)] private static extern PX.ActionResult PXSBPClientSendMessage(ref PXSBPClient pxSBPClient, UIntPtr data, UIntPtr dataSize);
        [DllImport("PXUltima.dll", CallingConvention = CallingConvention.Cdecl)] private static extern PX.ActionResult PXSBPClientSendFile(ref PXSBPClient pxSBPClient, ref PXText filePath);

        private PXSBPClient _pxSBPClient = new PXSBPClient();

        public event SBPOnMessageUpdatedFunction OnMessageUpdated;
        public event SBPOnMessageReceivedFunction OnMessageReceived;

        public unsafe SBPClient()
        {
            PXSBPClientConstruct(ref _pxSBPClient);

            PXSBPReceiverEventList pXSBPReceiverEventList = new PXSBPReceiverEventList();

            pXSBPReceiverEventList.OnMessageUpdatedCallBack = Marshal.GetFunctionPointerForDelegate<PXSBPOnMessageUpdatedFunction>(OnMessageUpdatedCallBack).ToPointer();
            pXSBPReceiverEventList.OnMessageReceivedCallBack = Marshal.GetFunctionPointerForDelegate<PXSBPOnMessageReceivedFunction>(OnMessageReceivedCallBack).ToPointer();

            PXSBPClientReceiverEventListSet(ref _pxSBPClient, ref pXSBPReceiverEventList);
        }

        private void OnMessageReceivedCallBack(ref PXSBPMessage pxSBPMessage)
        {
            OnMessageReceived?.Invoke(new SBPMessage(ref pxSBPMessage));
        }

        private void OnMessageUpdatedCallBack(ref PXSBPMessage pxSBPMessage)
        {
            OnMessageUpdated?.Invoke(new SBPMessage(ref pxSBPMessage));
        }

        public void Dispose()
        {
            PXSBPClientDestruct(ref _pxSBPClient);
        }

        public unsafe ActionResult SendData(string message)
        {
            fixed (char* messageAdress = message.ToCharArray())
            {
                return SendData((UIntPtr)messageAdress, (UIntPtr)(message.Length * sizeof(char)));
            }
        }

        public ActionResult SendData(UIntPtr data, UIntPtr dataSize)
        {
            return PXSBPClientSendMessage(ref _pxSBPClient, data, dataSize);
        }

        public unsafe ActionResult SendFile(string filePath)
        {
            fixed (char* charBuffer = filePath.ToCharArray())
            {
                PXText pXText = PXText.MakeFromStringW(charBuffer, filePath.Length);

                return PXSBPClientSendFile(ref _pxSBPClient, ref pXText);
            }
        }

        public unsafe ActionResult ConnectToServer(ushort port)
        {
            return ConnectToServer("127.0.0.1", port);
        }

        public unsafe ActionResult ConnectToServer(string ip, ushort port)
        {
            fixed (char* charBuffer = ip.ToCharArray())
            {
                PXText pXText = PXText.MakeFromStringW(charBuffer, ip.Length);

                return PXSBPClientConnectToServer(ref _pxSBPClient, ref pXText, port);
            }
        }

        public ActionResult DisconnectFromServer()
        {
            return PXSBPClientDisconnectFromServer(ref _pxSBPClient);
        }
    }
}