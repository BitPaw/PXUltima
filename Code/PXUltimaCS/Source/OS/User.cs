using System;
using System.Runtime.InteropServices;

namespace PX
{
    public class User
    {
        [DllImport("PXUltima.dll", CallingConvention = CallingConvention.StdCall)] private static unsafe extern PX.ActionResult PXUserNameGet(PXText* name);

        public static unsafe string Name
        {
            get
            {
                const int bufferSize = 64;
                sbyte* bufferAdress = stackalloc sbyte[bufferSize];
                PXText* pXTextAdress = stackalloc PXText[1];
                string nameString = null;

                pXTextAdress->MakeFromBufferW((IntPtr)bufferAdress, (IntPtr)bufferSize);

                PXUserNameGet(pXTextAdress);

                nameString = pXTextAdress->GenerateString();

                return nameString;
            }
        }
    }
}