using System;
using System.Runtime.InteropServices;

namespace PX
{
    public class Window : IDisposable
    {
        //   [MarshalAs(UnmanagedType.ByValArray)]
        //[StructLayout(LayoutKind.Sequential)]
        [StructLayout(LayoutKind.Sequential, Size = 2100)]
        private unsafe struct PXWindow
        {
            public byte IsRunning;
            public void* ID;

            public int X;
            public int Y;
            public int Width;
            public int Height;
        };

        [DllImport("PXUltima.dll")] private static extern void PXWindowConstruct(ref PXWindow window);
        [DllImport("PXUltima.dll")] private static extern void PXWindowDestruct(ref PXWindow window);

        [DllImport("PXUltima.dll")] private static extern float PXWindowScreenRatio(ref PXWindow window);

        // Create a window based on the OS implementation.
        // if a NULL pointer is used as a title, the window will be hidden.
        [DllImport("PXUltima.dll")] private static unsafe extern void PXWindowCreateA(ref PXWindow window, int width, int height, char* title, bool async);
        [DllImport("PXUltima.dll")] private static unsafe extern void PXWindowCreateW(ref PXWindow window, int width, int height, char* title, bool async);
        [DllImport("PXUltima.dll")] private static unsafe extern void PXWindowCreateU(ref PXWindow window, int width, int height, char* title, bool async);
        [DllImport("PXUltima.dll")] private static unsafe extern void PXWindowCreate(ref PXWindow window, int width, int height, bool async);
        [DllImport("PXUltima.dll")] private static unsafe extern void PXWindowCreateHidden(ref PXWindow window, int width, int height, bool async);

        [DllImport("PXUltima.dll")] private static unsafe extern bool PXWindowTitleSetA(ref PXWindow window, char* title, UInt64 titleSize);
        [DllImport("PXUltima.dll")] private static unsafe extern UInt64 PXWindowTitleGetA(ref PXWindow window, char* title, UInt64 titleSize);
        [DllImport("PXUltima.dll")] private static unsafe extern UInt64 PXWindowTitleGetW(ref PXWindow window, char* title, UInt64 titleSize);

        [DllImport("PXUltima.dll")] private static unsafe extern void PXWindowIconCorner();
        [DllImport("PXUltima.dll")] private static unsafe extern void PXWindowIconTaskBar();

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



        private PXWindow _pxWindow = new PXWindow();

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
                char[] buffer = new char[256];

                fixed(char* bufferaAdress = buffer)
                {
                    UInt64 length = PXWindowTitleGetW(ref _pxWindow, bufferaAdress, 256);

                    if (length > 0)
                    {
                        title = new string(bufferaAdress, 0, (int)length);
                    }
                }

         

                return title;
            }
            set
            {
                fixed (char* bufferaAdress = value.ToCharArray())
                {
                    PXWindowTitleSetA(ref _pxWindow, bufferaAdress, (UInt64)value.Length);
                }
            }
        }

        public unsafe void Create(string title)
        {
            fixed (char* charBuffer = title)
            {
                PXWindowCreateW(ref _pxWindow, -1, -1, charBuffer, true);
            }
        }
        public unsafe void Create(int width, int height, string title)
        {
            fixed (char* charBuffer = title)
            {
                PXWindowCreateW(ref _pxWindow, width, height, charBuffer, true);
            }
        }

        public bool FrameBufferSwap()
        {
            return PXWindowFrameBufferSwap(ref _pxWindow) != 0x00;
        }       
    }
}