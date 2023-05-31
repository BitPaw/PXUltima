using System.Runtime.InteropServices;

namespace PX
{
    public class User
    {
        [DllImport("PXUltima.dll")] private static extern byte PXUserNameGet(ref PXText name);

        public static unsafe string Name
        {
            get
            {
                byte* buffer = stackalloc byte[64];
                PXText pXText = PXText.MakeFromBufferU(buffer, 64);

                PXUserNameGet(ref pXText);

                string buildDate = new string((sbyte*)pXText.TextA, 0, (int)pXText.SizeUsed);

                return buildDate;
            }
        }
    }
}