using System.Runtime.InteropServices;

namespace PX
{
    public unsafe struct CImage
    {
        public ulong Width;
        public ulong Height;

        public char Format;

        public ulong PixelDataSize;
        void* PixelData;
    }

    public class Image //: IDisposable
    {
        private CImage _cImage;

        public ulong Width { get => _cImage.Width; }
        public ulong Height { get => _cImage.Height; }

        //[DllImport("PXUltima.dll", EntryPoint = "TESTDLL", CharSet = CharSet.Auto, CallingConvention = CallingConvention.Cdecl)]
        //static extern int TESTDLL();

        [DllImport("PXUltima.dll", EntryPoint = "ImageLoadW")]
        static extern unsafe int ImageLoadW(CImage* image, char* filePath);

        public int Load(string filePath)
        {
            int result = -1;
            char[] filePathA = filePath.ToCharArray();

            unsafe
            {
                fixed(CImage* imageAdress = &_cImage)
                {
                    fixed (char* p = filePathA)
                    {
                        result = ImageLoadW(imageAdress, p);
                    }
                }
            }

            return result;
        }
    }
}