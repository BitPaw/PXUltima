using System;
using System.Collections.Generic;
using System.Runtime.InteropServices;


namespace PX
{
    public class SBPClient : UnmanagedStructure, IDisposable
    {
        [StructLayout(LayoutKind.Sequential, Size = 1400)]
        private struct PXSBPClient
        {

        }

        [DllImport("PXUltima.dll", CallingConvention = CallingConvention.StdCall)] private static extern void PXSBPClientConstruct(ref PXSBPClient pxSBPClient);
        [DllImport("PXUltima.dll", CallingConvention = CallingConvention.StdCall)] private static extern void PXSBPClientDestruct(ref PXSBPClient pxSBPClient);

        [DllImport("PXUltima.dll", CallingConvention = CallingConvention.StdCall)] private static extern void PXSBPClientReceiverEventListSet(ref PXSBPClient pxSBPReceiver, ref PXSBPReceiverEventList pxSBPReceiverEventList);

        [DllImport("PXUltima.dll", CallingConvention = CallingConvention.StdCall)] private static extern PX.ActionResult PXSBPClientConnectToServer(ref PXSBPClient pxSBPClient, ref PXText ip, ushort port);
        [DllImport("PXUltima.dll", CallingConvention = CallingConvention.StdCall)] private static extern PX.ActionResult PXSBPClientDisconnectFromServer(ref PXSBPClient pxSBPClient);

        [DllImport("PXUltima.dll", CallingConvention = CallingConvention.StdCall)] private static extern PX.ActionResult PXSBPClientSendMessage(ref PXSBPClient pxSBPClient, UIntPtr data, UIntPtr dataSize);
        [DllImport("PXUltima.dll", CallingConvention = CallingConvention.StdCall)] private static extern PX.ActionResult PXSBPClientSendFile(ref PXSBPClient pxSBPClient, ref PXText filePath);

        private PXSBPClient _pxSBPClient;

        public event SBPOnMessageUpdatedFunction OnMessageUpdated;
        public event SBPOnMessageReceivedFunction OnMessageReceived;

        public unsafe SBPClient()
        {
            _pxSBPClient = new PXSBPClient();

            PXSBPClientConstruct(ref _pxSBPClient);

            PXSBPReceiverEventList pXSBPReceiverEventList = new PXSBPReceiverEventList();

            OwnerSet(this);

            pXSBPReceiverEventList.Owner = Adress;
            pXSBPReceiverEventList.OnMessageUpdatedCallBack = DelegateToAdress<PXSBPOnMessageUpdatedFunction>(OnMessageUpdatedCallBack);
            pXSBPReceiverEventList.OnMessageReceivedCallBack = DelegateToAdress<PXSBPOnMessageReceivedFunction>(OnMessageReceivedCallBack);

            PXSBPClientReceiverEventListSet(ref _pxSBPClient, ref pXSBPReceiverEventList);
        }

        private static unsafe void OnMessageReceivedCallBack(ref PXSBPMessage pxSBPMessage)
        {
            SBPClient sBPClient = ObjectFromAdress<SBPClient>(pxSBPMessage.Owner);
            SBPMessage sbpMessageBuffer = new SBPMessage(ref pxSBPMessage);

            sBPClient.OnMessageReceived?.Invoke(sbpMessageBuffer);
        }

        private static unsafe void OnMessageUpdatedCallBack(ref PXSBPMessage pxSBPMessage)
        {
            SBPClient sBPClient = ObjectFromAdress<SBPClient>(pxSBPMessage.Owner);
            SBPMessage sbpMessageBuffer = new SBPMessage(ref pxSBPMessage);

            sBPClient.OnMessageUpdated?.Invoke(sbpMessageBuffer);
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

        public ActionResult SendObject<T>()
        {
            return ActionResult.NotImplemented;
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