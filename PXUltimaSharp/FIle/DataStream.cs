using System.Runtime.InteropServices;

namespace PX
{
    public unsafe struct PXPXDataStream
    {
        byte IsRunning;
        void* ID;

        uint X;
        uint Y;
        uint Width;
        uint Height;

        fixed byte Data[1840];
    }

    public class PXDataStream
    {
        private PXPXDataStream _pxPXDataStream;


	}
}
