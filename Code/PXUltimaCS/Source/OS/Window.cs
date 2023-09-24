using System;
using System.Runtime.InteropServices;

namespace PX
{
    //   [MarshalAs(UnmanagedType.ByValArray)]
    //[StructLayout(LayoutKind.Sequential)]
    [StructLayout(LayoutKind.Sequential, Size = 2100)]
    internal unsafe struct PXWindow
    {
        public byte IsRunning;
        public void* ID;

        public int X;
        public int Y;
        public int Width;
        public int Height;
    };


    public class Window : IDisposable
    {
   
        [DllImport("PXUltima.dll")] private static extern void PXWindowConstruct(ref PXWindow window);
        [DllImport("PXUltima.dll")] private static extern void PXWindowDestruct(ref PXWindow window);

        [DllImport("PXUltima.dll")] private static extern float PXWindowScreenRatio(ref PXWindow window);

        // Create a window based on the OS implementation.
        // if a NULL pointer is used as a title, the window will be hidden.
        [DllImport("PXUltima.dll")] private static extern void PXWindowCreate(ref PXWindow window, int x, int y, int width, int height, ref PXText title, bool async);
        [DllImport("PXUltima.dll")] private static extern void PXWindowCreateHidden(ref PXWindow window, int width, int height, bool async);

        [DllImport("PXUltima.dll")] private static extern bool PXWindowTitleSet(ref PXWindow window, ref PXText title);
        [DllImport("PXUltima.dll")] private static extern unsafe UIntPtr PXWindowTitleGet(ref PXWindow window, PXText* title);

        [DllImport("PXUltima.dll")] private static extern void PXWindowIconCorner();
        [DllImport("PXUltima.dll")] private static extern void PXWindowIconTaskBar();

        //[DllImport("PXUltima.dll")] private static unsafe extern void PXWindowSize(PXWindow* pxWindow, uint* x, uint* y, uint* width, uint* height);
        //[DllImport("PXUltima.dll")] private static unsafe extern void PXWindowSizeChange(PXWindow* pxWindow, uint x, uint y, uint width, uint height);
        //[DllImport("PXUltima.dll")] private static unsafe extern void PXWindowPosition(PXWindow* pxWindow, uint* x, uint* y);
        //[DllImport("PXUltima.dll")] private static unsafe extern void PXWindowPositionChange(PXWindow* pxWindow, uint x, uint y);
        //[DllImport("PXUltima.dll")] private static unsafe extern void PXWindowPositonCenterScreen(PXWindow* pxWindow);
        //[DllImport("PXUltima.dll")] private static unsafe extern void PXWindowCursor(PXWindow* pxWindow);

        // [DllImport("PXUltima.dll")] private static unsafe extern void PXWindowCursorTexture();
        //[DllImport("PXUltima.dll")] private static unsafe extern void PXWindowCursorCaptureMode(PXWindow* pxWindow, PXWindowCursorMode cursorMode);

        [DllImport("PXUltima.dll")] private static extern byte PXWindowFrameBufferSwap(ref PXWindow pxWindow);

        //  [DllImport("PXUltima.dll")] private static unsafe extern bool PXWindowInteractable(PXWindow* pxWindow);

        // [DllImport("PXUltima.dll")] private static unsafe extern bool PXWindowCursorPositionInWindowGet(PXWindow* window, int* x, int* y);
        // [DllImport("PXUltima.dll")] private static unsafe extern bool PXWindowCursorPositionInDestopGet(PXWindow* window, int* x, int* y);


        // Checks if the current window is the one in focus.
        // [DllImport("PXUltima.dll")] private static unsafe extern bool PXWindowIsInFocus( PXWindow*  window);




        internal PXWindow _pxWindow = new PXWindow();

        public unsafe Window()
        {            
            PXWindowConstruct(ref _pxWindow);
        }

        public void Dispose()
        {
            PXWindowDestruct(ref _pxWindow);
        }

        public int Width { get => _pxWindow.Width; }
        public int Height { get => _pxWindow.Height; }
        public unsafe string Title 
        { 
            get
            {               
                string title = string.Empty;
                char* buffer = stackalloc char[256];
                PXText* pXText = stackalloc PXText[1];

                pXText->MakeFromBufferW((IntPtr)buffer, (IntPtr)256);

                ulong length = (ulong)PXWindowTitleGet(ref _pxWindow, pXText);

                if (length > 0)
                {
                    title = new string(buffer, 0, (int)length);
                }

                return title;
            }
            set
            {
                fixed (char* bufferaAdress = value.ToCharArray())
                {
                    PXText pXText = PXText.MakeFromStringW(bufferaAdress, value.Length);

                    PXWindowTitleSet(ref _pxWindow, ref pXText);
                }
            }
        }

        public unsafe void Create(string title)
        {
            fixed (char* charBuffer = title.ToCharArray())
            {
                PXText pXText = PXText.MakeFromStringW(charBuffer, title.Length);

                PXWindowCreate(ref _pxWindow, 0, 0, -1, -1, ref pXText, true);
            }
        }
        public unsafe void Create(int x, int y, int width, int height, string title)
        {            
            fixed (char* charBuffer = title.ToCharArray())
            {
                PXText pXText = PXText.MakeFromStringW(charBuffer, title.Length);

                PXWindowCreate(ref _pxWindow, x, y, width, height, ref pXText, true);
            }
        }

        public bool FrameBufferSwap()
        {
            return PXWindowFrameBufferSwap(ref _pxWindow) != 0x00;
        }       
    }
}