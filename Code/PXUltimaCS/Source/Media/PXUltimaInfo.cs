using System.Runtime.InteropServices;

namespace PX
{
    public class PXUltimaInfo
    {
        [DllImport("PXUltima.dll")] private static extern void PXUltimaInfoBuildDate(ref PXText text);

        public static unsafe string BuildDate 
        {
            get
            {
                byte* buffer = stackalloc byte[64];

                PXText pXText = new PXText();
                pXText.SizeAllocated = 64;
                pXText.TextA = buffer;

                PXUltimaInfoBuildDate(ref pXText);

                string buildDate = new string((sbyte*)pXText.TextA, 0, (int)pXText.SizeUsed);

                return buildDate;
            } 
        }
    }
}
