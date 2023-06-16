using System;
using System.Runtime.InteropServices;

namespace PX
{
    public class PXUltimaInfo
    {
        [DllImport("PXUltima.dll", CallingConvention = CallingConvention.StdCall)] private static extern void PXUltimaInfoBuildDate(ref PXText text);

        public static unsafe string BuildDate 
        {
            get
            {
                const int bufferSize = 64;
                sbyte* bufferAdress = stackalloc sbyte[bufferSize];

#if false
                PXText* pXTextAdress = stackalloc PXText[1];
                PXText pXText = *pXTextAdress;
#else
                PXText pXText = new PXText();
#endif

                pXText.MakeFromBufferW((IntPtr)bufferAdress, (IntPtr)bufferSize);

                PXUltimaInfoBuildDate(ref pXText);

                return pXText.GenerateString();
            } 
        }
    }
}
