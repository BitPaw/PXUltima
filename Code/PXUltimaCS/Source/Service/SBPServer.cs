using System;
using System.Runtime.InteropServices;

namespace PX
{
    public class SBPServer : UnmanagedStructure, IDisposable
    {
        [StructLayout(LayoutKind.Sequential, Size = 1400)]
        private struct PXSBPServer
        {

        }

        [DllImport("PXUltima.dll", CallingConvention = CallingConvention.StdCall)] private static extern void PXSBPServerConstruct(ref PXSBPServer pxSBPServer);
        [DllImport("PXUltima.dll", CallingConvention = CallingConvention.StdCall)] private static extern void PXSBPServerDestruct(ref PXSBPServer pxSBPServer);

        [DllImport("PXUltima.dll", CallingConvention = CallingConvention.StdCall)] private static extern void PXSBPServerReceiverEventListSet(ref PXSBPServer pxSBPServe, ref PXSBPReceiverEventList pxSBPReceiverEventList);

        [DllImport("PXUltima.dll", CallingConvention = CallingConvention.StdCall)] private static extern PX.ActionResult PXSBPServerStart(ref PXSBPServer pxSBPServer, ushort port);
        [DllImport("PXUltima.dll", CallingConvention = CallingConvention.StdCall)] private static extern PX.ActionResult PXSBPServerStop(ref PXSBPServer pxSBPServer);


        [DllImport("PXUltima.dll", CallingConvention = CallingConvention.StdCall)] private static extern PX.ActionResult PXSBPServerSendToAll(ref PXSBPServer pxSBPServer, UIntPtr data, UIntPtr dataSize);

        private PXSBPServer _pxSBPServer = new PXSBPServer();

        public event SBPOnMessageUpdatedFunction OnMessageUpdated;
        public event SBPOnMessageReceivedFunction OnMessageReceived;

        public unsafe SBPServer()
        {
            PXSBPServerConstruct(ref _pxSBPServer);

            OwnerSet(this);

            PXSBPReceiverEventList pXSBPReceiverEventList = new PXSBPReceiverEventList();
            pXSBPReceiverEventList.Owner = Adress;
            pXSBPReceiverEventList.OnMessageUpdatedCallBack = Marshal.GetFunctionPointerForDelegate<PXSBPOnMessageUpdatedFunction>(OnMessageUpdatedCallBack).ToPointer();
            pXSBPReceiverEventList.OnMessageReceivedCallBack = Marshal.GetFunctionPointerForDelegate<PXSBPOnMessageReceivedFunction>(OnMessageReceivedCallBack).ToPointer();

            PXSBPServerReceiverEventListSet(ref _pxSBPServer, ref pXSBPReceiverEventList);
        }

        private static unsafe void OnMessageReceivedCallBack(ref PXSBPMessage pxSBPMessage)
        {
            SBPServer sbpServer = ObjectFromAdress<SBPServer>(pxSBPMessage.Owner);

            sbpServer.OnMessageReceived?.Invoke(new SBPMessage(ref pxSBPMessage));
        }

        private static unsafe void OnMessageUpdatedCallBack(ref PXSBPMessage pxSBPMessage)
        {
            SBPServer sbpServer = ObjectFromAdress<SBPServer>(pxSBPMessage.Owner);

            sbpServer.OnMessageUpdated?.Invoke(new SBPMessage(ref pxSBPMessage));
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

        public unsafe ActionResult SendToAll(string message)
        {
            fixed (char* charBuffer = message.ToCharArray())
            {
                UIntPtr dataSize = (UIntPtr)(message.Length * 2);

                return PXSBPServerSendToAll(ref _pxSBPServer, (UIntPtr)charBuffer, dataSize);
            }
        }
    }
}
