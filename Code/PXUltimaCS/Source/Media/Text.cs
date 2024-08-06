using System;
using System.Runtime.InteropServices;

namespace PX
{
    internal enum PXTextFormat
    {
        TextFormatInvalid,
        TextFormatASCII,    // 1 Byte per character, range from 0 to 255    
        TextFormatUNICODE,  // 2 Bytes per character, range from 0 to 65535    
        TextFormatUTF8,     // Variable length of characters, 1 to 4 Bytes
        TextFormatUTF16
    }

    [StructLayout(LayoutKind.Sequential)]
    internal unsafe struct PXText
    {
        public void* SizeAllocated; // Size that the buffer ponited to has
        public void* SizeUsed;
        public void* NumberOfCharacters;
        public void* TextAdress;
        public uint Format;

        //public static PXText CreateFromString(string text)

        public unsafe string GenerateString()
        {
            switch ((PXTextFormat)Format)
            {      
                case PXTextFormat.TextFormatASCII:
                case PXTextFormat.TextFormatUTF8:
                    return new string((sbyte*)TextAdress, 0, (int)SizeUsed);
                case PXTextFormat.TextFormatUNICODE:
                    return new string((char*)TextAdress, 0, (int)SizeUsed/2);

                default:
                    return "Error : InvalidFormat";
            }
        }

        public void MakeFromBufferW(IntPtr buffer, IntPtr length)
        {
            NumberOfCharacters = (void*)IntPtr.Zero;
            SizeAllocated = (void*)length;
            SizeUsed = (void*)IntPtr.Zero;
            TextAdress = (void*)buffer;
            Format = (int)PXTextFormat.TextFormatUNICODE;
        }
        public unsafe static PXText MakeFromBufferU(void* buffer, int length)
        {
            PXText pxText = new PXText();

            pxText.NumberOfCharacters = (void*)IntPtr.Zero;
            pxText.SizeAllocated = (void*)length;
            pxText.SizeUsed = (void*)IntPtr.Zero;
            pxText.TextAdress = (void*)buffer;
            pxText.Format = (int)PXTextFormat.TextFormatUTF8;

            return pxText;
        }

        public unsafe static PXText MakeFromStringW(void* text, int length)
        {
            PXText pxText = new PXText();

            pxText.NumberOfCharacters = (void*)length;
            pxText.SizeAllocated = (void*)(length * 2);
            pxText.SizeUsed = pxText.SizeAllocated;
            pxText.TextAdress = (void*)text;
            pxText.Format = (int)PXTextFormat.TextFormatUNICODE;

            return pxText;
        }
    };
}
