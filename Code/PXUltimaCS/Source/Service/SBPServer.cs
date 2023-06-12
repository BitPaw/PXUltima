using System;
using System.Runtime.InteropServices;

namespace PX
{
    public class SBPServer : IDisposable
    {
        [StructLayout(LayoutKind.Sequential, Size = 1400)]
        private struct PXSBPServer
        {

        }

        [DllImport("PXUltima.dll", CallingConvention = CallingConvention.Cdecl)] private static extern void PXSBPServerConstruct(ref PXSBPServer pxSBPServer);
        [DllImport("PXUltima.dll", CallingConvention = CallingConvention.Cdecl)] private static extern void PXSBPServerDestruct(ref PXSBPServer pxSBPServer);

        [DllImport("PXUltima.dll", CallingConvention = CallingConvention.Cdecl)] private static extern void PXSBPServerReceiverEventListSet(ref PXSBPServer pxSBPServe, ref PXSBPReceiverEventList pxSBPReceiverEventList);

        [DllImport("PXUltima.dll", CallingConvention = CallingConvention.Cdecl)] private static extern PX.ActionResult PXSBPServerStart(ref PXSBPServer pxSBPServer, ushort port);
        [DllImport("PXUltima.dll", CallingConvention = CallingConvention.Cdecl)] private static extern PX.ActionResult PXSBPServerStop(ref PXSBPServer pxSBPServer);

        private PXSBPServer _pxSBPServer = new PXSBPServer();

        public event SBPOnMessageUpdatedFunction OnMessageUpdated;
        public event SBPOnMessageReceivedFunction OnMessageReceived;

        public unsafe SBPServer()
        {
            PXSBPServerConstruct(ref _pxSBPServer);

            PXSBPReceiverEventList pXSBPReceiverEventList = new PXSBPReceiverEventList();

            pXSBPReceiverEventList.OnMessageUpdatedCallBack = Marshal.GetFunctionPointerForDelegate<PXSBPOnMessageUpdatedFunction>(OnMessageUpdatedCallBack).ToPointer();
            pXSBPReceiverEventList.OnMessageReceivedCallBack = Marshal.GetFunctionPointerForDelegate<PXSBPOnMessageReceivedFunction>(OnMessageReceivedCallBack).ToPointer();

            PXSBPServerReceiverEventListSet(ref _pxSBPServer, ref pXSBPReceiverEventList);
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
