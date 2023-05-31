using System;
using System.Runtime.InteropServices;

namespace PX
{
    public class Server : IDisposable
    {
        [StructLayout(LayoutKind.Sequential, Size = 160)]
        private struct PXServer
        {

        }

        [DllImport("PXUltima.dll")] private static extern void PXServerConstruct(ref PXServer server);
        [DllImport("PXUltima.dll")] private static extern void PXServerDestruct(ref PXServer server);

        [DllImport("PXUltima.dll")] private static extern PX.ActionResult PXServerStart(ref PXServer server, ushort port, ProtocolMode protocolMode);
        [DllImport("PXUltima.dll")] private static extern PX.ActionResult PXServerStop(ref PXServer server);
        [DllImport("PXUltima.dll")] private static extern PX.ActionResult PXServerKickClient(ref PXServer server, uint socketID);

        private PXServer _pxServer = new PXServer();

        public void Dispose()
        {
            PXServerDestruct(ref _pxServer);
        }

        public Server()
        {
            PXServerConstruct(ref _pxServer);
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