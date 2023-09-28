using System.Runtime.InteropServices;

namespace PX
{
    [StructLayout(LayoutKind.Explicit, Size = 8)]
    internal struct PXVector2F
    {
        [FieldOffset(0)] public float X;
        [FieldOffset(4)] public float Y;
    }

    public class Vector2
    {
        internal PXVector2F _pxVector2F = new PXVector2F();

        [DllImport("PXUltima.dll", CallingConvention = CallingConvention.StdCall)] private static extern void PXVector2FSet(ref PXVector2F vector, ref PXVector2F vectorSet);
        [DllImport("PXUltima.dll", CallingConvention = CallingConvention.StdCall)] private static extern void PXVector2FSetXY(ref PXVector2F vector, float x, float y);

        [DllImport("PXUltima.dll", CallingConvention = CallingConvention.StdCall)] private static extern void PXVector2FAdd(ref PXVector2F vector, ref PXVector2F pxVectorData);
        [DllImport("PXUltima.dll", CallingConvention = CallingConvention.StdCall)] private static extern void PXVector2FAddXY(ref PXVector2F vector, float x, float y);

        [DllImport("PXUltima.dll", CallingConvention = CallingConvention.StdCall)] private static extern void PXVector2FMultiply(ref PXVector2F vector, ref PXVector2F pxVectorData);
        [DllImport("PXUltima.dll", CallingConvention = CallingConvention.StdCall)] private static extern void PXVector2FMultiplyXY(ref PXVector2F vector, float x, float y);

        [DllImport("PXUltima.dll", CallingConvention = CallingConvention.StdCall)] private static extern float PXVector2FDistanceTo(ref PXVector2F vectorA, ref PXVector2F vectorB);
        [DllImport("PXUltima.dll", CallingConvention = CallingConvention.StdCall)] private static extern float PXVector2FDotProduct(ref PXVector2F vectorA, ref PXVector2F vectorB);


        public static Vector2 Zero = new Vector2(0, 0);

        public float X { get => _pxVector2F.X; set => _pxVector2F.X = value; }
        public float Y { get => _pxVector2F.Y; set => _pxVector2F.Y = value; }

        public Vector2() : this(0,0)
        {

        }

        public Vector2(float x, float y)
        {
            X = x;
            Y = y;
        }

        public Vector2(Vector2 vector2)
        {
            X = vector2.X;
            Y = vector2.Y;
        }

        public static Vector2 operator -(Vector2 a, Vector2 b)
        {
            Vector2 vector2 = new Vector2(a);

            PXVector2FAdd(ref vector2._pxVector2F, ref b._pxVector2F);

            return vector2;
        }

        public static Vector2 operator +(Vector2 a, Vector2 b)
        {
            Vector2 vector2 = new Vector2(a);

            PXVector2FMultiply(ref vector2._pxVector2F, ref b._pxVector2F);

            return vector2;
        }

        public void CrossProduct()
        {
            
        }

        public float DotProduct(Vector2 Vector2)
        {
            return PXVector2FDotProduct(ref _pxVector2F, ref Vector2._pxVector2F);
        }

        public void Rotate()
        {
            
        }

        public void Interpolate()
        {

        }

        public void Normalize()
        {

        }

        public override string ToString()
        {
            return "x: " + X + " y: " + Y;
        }
    }
}