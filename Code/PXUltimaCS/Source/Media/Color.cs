namespace PX
{
    public enum PXColorFormat
    {
        PXColorFormatInvalid,

        PXColorFormatAlphaBinary,

        // Single channel
        PXColorFormatR8,
        PXColorFormatG8,
        PXColorFormatB8,
        PXColorFormatA8,

        PXColorFormatR16,
        PXColorFormatG16,
        PXColorFormatB16,
        PXColorFormatA16,

        PXColorFormatR32,
        PXColorFormatG32,
        PXColorFormatB32,
        PXColorFormatA32,

        PXColorFormatRF,
        PXColorFormatGF,
        PXColorFormatBF,
        PXColorFormatAF,

        PXColorFormatRD,
        PXColorFormatGD,
        PXColorFormatBD,
        PXColorFormatAD,



        // RGB
        PXColorFormatRGBI8,
        PXColorFormatRGBI16,
        PXColorFormatRGBI32,
        PXColorFormatRGBF,
        PXColorFormatRGBD,

        // BGR
        PXColorFormatBGRI8,
        PXColorFormatBGRI16,
        PXColorFormatBGRI32,
        PXColorFormatBGRF,
        PXColorFormatBGRD,

        // RGBA
        PXColorFormatRGBAI8,
        PXColorFormatRGBAI16,
        PXColorFormatRGBAI32,
        PXColorFormatRGBAF,
        PXColorFormatRGBAD,

        // BGRA
        PXColorFormatBGRAI8,
        PXColorFormatBGRAI16,
        PXColorFormatBGRAI32,
        PXColorFormatBGRAF,
        PXColorFormatBGRAD,




        PXColorFormatYCbCrI8,







        PXColorFormatARGBI8,
        PXColorFormatARGBF16,
        PXColorFormatARGBF32,

        PXColorFormatXRGBI8,

        PXColorFormatRGBI5,





        PXColorFormatLuminance,
        PXColorFormatLuminanceAlpha,
        PXColorFormatColorIndex,
        PXColorFormatStencilIndex,
        PXColorFormatDepthComponent
    }

    public struct PXColorRGBI8
    {
        byte Red;
        byte Green;
        byte Blue;
    }

    public struct PXColorRGBI16
    {
        ushort Red;
        ushort Green;
        ushort Blue;
    }

    public struct PXColorRGBI32
    {
        uint Red;
        uint Green;
        uint Blue;
    }
    public struct PXColorRGBF
    {
        float Red;
        float Green;
        float Blue;
    }

    public struct PXColorRGBAF
    {
        public float Red;
        public float Green;
        public float Blue;
        public float Alpha;
    }

    public struct PXColorRGBD
    {
        public double Red;
        public double Green;
        public double Blue;
    }

    public struct PXColorYCbCrI8
    {
        byte Y;
        byte Cb;
        byte Cr;
    }

    //---[ Public-Functions ]----------------------------------------------

    public class Color
    {
        public static void RGBToYCbCr(ref PXColorRGBI8 colorRGB, ref PXColorYCbCrI8 ColorYCbCr)
        {
        
        }
        public static void YCbCrToRGB(ref PXColorYCbCrI8 colorYCbCr, ref PXColorRGBI8 colorRGB)
        {
            
        }

        public static float RGBToY(float red, float green, float blue)
        {
            return 0;
        }
        public static float RGBToCb(float red, float green, float blue)
        {
            return 0;
        }
        public static float RGBToCr(float red, float green, float blue)
        {
            return 0;
        }
    }
}
