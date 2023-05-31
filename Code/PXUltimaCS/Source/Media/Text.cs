using System;
using System.Collections.Generic;
using System.Runtime.InteropServices;
using System.Text;

namespace PX
{
    internal enum PXTextFormat
    {
        TextFormatInvalid,

        // 1 Byte per character, range from 0 to 255
        TextFormatASCII,

        // 2 Bytes per character, range from 0 to 65535
        TextFormatUNICODE,

        // Variable length of characters, 1 to 4 Bytes
        TextFormatUTF8,
        TextFormatUTF16
    }

    [StructLayout(LayoutKind.Explicit, Size = 60)]
    internal unsafe struct PXText
    {
        [FieldOffset(0)] public ulong SizeAllocated; // Size that the buffer ponited to has
        [FieldOffset(8)] public ulong SizeUsed;
        [FieldOffset(16)] public ulong NumberOfCharacters;

        [FieldOffset(24)] public byte* TextA;
        [FieldOffset(24)] public char* TextW;

        [FieldOffset(32)] public uint Format;

        //public static PXText CreateFromString(string text)

        public static PXText MakeFromBufferU(byte* buffer, int length)
        {
            PXText pxText = new PXText();

            pxText.NumberOfCharacters = 0;
            pxText.SizeAllocated = (ulong)length;
            pxText.SizeUsed = 0;
            pxText.TextA = buffer;
            pxText.Format = 3;

            return pxText;
        }

        public static PXText MakeFromStringW(char* text, int length)
        {
            PXText pxText = new PXText();

            pxText.NumberOfCharacters = (ulong)length;
            pxText.SizeAllocated = pxText.NumberOfCharacters * 2;
            pxText.SizeUsed = pxText.SizeAllocated;
            pxText.TextW = text;
            pxText.Format = 2;

            return pxText;
        }
    };
}
