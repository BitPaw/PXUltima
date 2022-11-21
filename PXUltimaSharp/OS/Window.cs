using System.Runtime.InteropServices;

namespace PX
{
    //[StructLayout(LayoutKind.Explicit, Size = 1480)]
    public unsafe struct PXWindow
    {
        byte IsRunning;
        void* ID;

        uint X;
        uint Y;
        uint Width;
        uint Height;

        fixed byte Data[1840];
    }

    public class Window
    {
        private PXWindow _pxWindow;

        [DllImport("PXUltima.dll", EntryPoint = "PXWindowCreateW")]
        static extern unsafe void PXWindowCreateW(PXWindow* image, uint width, uint height, char* title, byte async);

        public void Create(uint width, uint height, string title)
        {
            char[] filePathA = title.ToCharArray();

            unsafe
            {
                fixed (PXWindow* pxWindowAdress = &_pxWindow)
                {
                    fixed (char* p = filePathA)
                    {
                        PXWindowCreateW(pxWindowAdress, width, height, p, 0x01);
                    }
                }
            }
        }
    }
}
