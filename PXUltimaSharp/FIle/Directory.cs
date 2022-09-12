using System.Net.Sockets;
using System.Runtime.InteropServices;

namespace PX
{
    public class Directory
    {
        [DllImport("PXUltima.dll", EntryPoint = "DirectoryCreateW")]
        static extern unsafe int DirectoryCreateW(char* filePath);

        public static int Create(string directoryName)
        {
            int result = -1;
            char[] filePathA = directoryName.ToCharArray();

            unsafe
            {
                fixed (char* filePathAdress = filePathA)
                {
                    result = DirectoryCreateW(filePathAdress);
                }
            }

            return result;
        }
    }
}
