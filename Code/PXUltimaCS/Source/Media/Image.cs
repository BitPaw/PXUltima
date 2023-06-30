using System.Runtime.InteropServices;

namespace PX
{
    public interface IImageFormat
    {

    }


    [StructLayout(LayoutKind.Sequential, Size = 72)]
    internal unsafe struct PXImage
    {
       
    }

    public class Image
    {
        public void Load(string filePath)
        {

        }

        public void Save(string filePath)
        {

        }
    }
}