using System;
using System.Collections.Generic;
using System.Runtime.InteropServices;
using System.Text;

namespace PX
{
    public class Window : IDisposable
    {
        byte[] _data = new byte[1820];

        [DllImport("PXUltima.dll")]
        private static unsafe extern uint PXWindowConstruct(void* thing);

        [DllImport("PXUltima.dll")]
        private static unsafe extern uint PXWindowCreateW(void* thing, uint width, uint height, void*  title, bool async);

        [DllImport("PXUltima.dll")]
        private static unsafe extern bool PXWindowFrameBufferSwap(void* thing);

        public unsafe Window()
        {
            fixed (byte* pointerToFirst = _data)
            {
                PXWindowConstruct(pointerToFirst);
            }
        }

        public uint Width { get; }
        public uint Height { get; }
        public uint Title { get; }

        public unsafe void Create(uint width, uint height, string title)
        {
            fixed (byte* pointerToFirst = _data)
            {
                fixed (char* textArray = (title.ToCharArray()))
                {
                    uint res = PXWindowCreateW(pointerToFirst, width, height, textArray, false);
                }
            }
        }

        public unsafe bool FrameBufferSwap()
        {
            fixed (byte* pointerToFirst = _data)
            {
                return PXWindowFrameBufferSwap(pointerToFirst);
            }
        }

        public void Dispose()
        {
            
        }
    }
}