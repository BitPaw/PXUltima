using System;
using System.Runtime.InteropServices;

namespace PX
{
    [StructLayout(LayoutKind.Sequential, Size = 64)]
    internal struct PXBitmap
    {

    }   

    public class Bitmap : PX.Image, PX.IImageFormat, IDisposable
    {
        [DllImport("PXUltima.dll")] private static extern void PXBitmapConstruct(ref PXBitmap bmp);
        [DllImport("PXUltima.dll")] private static extern void PXBitmapDestruct(ref PXBitmap bmp);
        [DllImport("PXUltima.dll")] private static extern UIntPtr PXBitmapFilePredictSize(UIntPtr width, UIntPtr height, UIntPtr bitsPerPixel);
        [DllImport("PXUltima.dll")] private static extern PX.ActionResult PXBitmapParseToImage(ref PXImage image, ref PXFile dataStream);
        [DllImport("PXUltima.dll")] private static extern PX.ActionResult PXBitmapSerializeFromImage(ref PXImage image, ref PXFile dataStream);

        internal PXBitmap _pxBitmap = new PXBitmap();

        public Bitmap()
        {
            PXBitmapConstruct(ref _pxBitmap);
        }

        public void Dispose()
        {
            PXBitmapDestruct(ref _pxBitmap);
        }

        public ActionResult ParseToImage(string filePath)
        {
            return ActionResult.NotImplemented;
        }

        public int PredictSize(int width, int height, int bitsPerPixel)
        {
            return (int)PXBitmapFilePredictSize((UIntPtr)width, (UIntPtr)height, (UIntPtr)bitsPerPixel);
        }

        public ActionResult SerializeFromImage(string filePath)
        {
            return ActionResult.NotImplemented;
        }
    }
}
