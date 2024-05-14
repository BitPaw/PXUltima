using System.Numerics;
using System.Runtime.InteropServices;

namespace PX
{
    [StructLayout(LayoutKind.Explicit, Size = 12)]
    internal struct PXVector3F
    {
        [FieldOffset(0)] public float X;
        [FieldOffset(4)] public float Y;
        [FieldOffset(8)] public float Z;
    }

    public class Vector3
    {
        internal PXVector3F _pxVector3F = new PXVector3F();

        [DllImport("PXUltima.dll", CallingConvention = CallingConvention.StdCall)] private static extern void PXVector3FAdd( ref PXVector3F  vectorA, ref PXVector3F vectorB);
        [DllImport("PXUltima.dll", CallingConvention = CallingConvention.StdCall)] private static extern void PXVector3FSubstract(ref PXVector3F vectorA, ref PXVector3F vectorB);
        [DllImport("PXUltima.dll", CallingConvention = CallingConvention.StdCall)] private static extern void PXVector3FMultiply(ref PXVector3F vectorA, ref PXVector3F vectorB);
        [DllImport("PXUltima.dll", CallingConvention = CallingConvention.StdCall)] private static extern void PXVector3FMultiplyS(ref PXVector3F vector,  float scalar);
        [DllImport("PXUltima.dll", CallingConvention = CallingConvention.StdCall)] private static extern void PXVector3FDivide(ref PXVector3F vectorA, ref PXVector3F vectorB);
        [DllImport("PXUltima.dll", CallingConvention = CallingConvention.StdCall)] private static extern void PXVector3FDivideS(ref PXVector3F vectorA,  float scalar);
        [DllImport("PXUltima.dll", CallingConvention = CallingConvention.StdCall)] private static extern void PXVector3FNormalize(ref PXVector3F vector);
        [DllImport("PXUltima.dll", CallingConvention = CallingConvention.StdCall)] private static extern float PXVector3FLength(ref PXVector3F vector);
        [DllImport("PXUltima.dll", CallingConvention = CallingConvention.StdCall)] private static extern void PXVector3FCrossProduct(ref PXVector3F vectorA, ref PXVector3F vectorB, ref PXVector3F vectorResult);
        [DllImport("PXUltima.dll", CallingConvention = CallingConvention.StdCall)] private static extern float PXVector3FDotProduct(ref PXVector3F vectorA, ref PXVector3F vectorB);
        [DllImport("PXUltima.dll", CallingConvention = CallingConvention.StdCall)] private static extern void PXVector3FInterpolate(ref PXVector3F vectorA, ref PXVector3F vectorB,  float speed, ref PXVector3F vectorResult);

        public static Vector3 Zero = new Vector3(0, 0, 0);

        public float X { get => _pxVector3F.X; set => _pxVector3F.X = value; }
        public float Y { get => _pxVector3F.Y; set => _pxVector3F.Y = value; }
        public float Z { get => _pxVector3F.Z; set => _pxVector3F.Z = value; }

        public Vector3() : this(0, 0, 0)
        {
  
        }

        public Vector3(float x, float y, float z)
        {
            X = x;
            Y = y;
            Z = z;
        }
    
        public static Vector3 operator +(Vector3 a, Vector3 b)
        {
            Vector3 vector3 = new Vector3();

            PXVector3FAdd(ref vector3._pxVector3F, ref a._pxVector3F);

            return vector3;
        }
        public static Vector3 operator -(Vector3 a, Vector3 b)
        {
            Vector3 vector3 = new Vector3();

            PXVector3FSubstract(ref vector3._pxVector3F, ref a._pxVector3F);

            return vector3;
        }
        public static Vector3 operator *(Vector3 a, Vector3 b)
        {
            Vector3 vector3 = new Vector3();

            PXVector3FMultiply(ref vector3._pxVector3F, ref a._pxVector3F);

            return vector3;
        }
        public static Vector3 operator *(Vector3 a, float scalar)
        {
            Vector3 vector3 = new Vector3();

            PXVector3FMultiplyS(ref vector3._pxVector3F, scalar);

            return vector3;
        }

        public float Length
        {
            get
            {
                return PXVector3FLength(ref _pxVector3F);
            }
        }

        public void Normalize()
        {
            PXVector3FNormalize(ref _pxVector3F);
        }

        public Vector3 CrossProduct(Vector3 vector3)
        {
            Vector3 vectorResult = new Vector3();

            PXVector3FCrossProduct(ref _pxVector3F, ref vector3._pxVector3F, ref vectorResult._pxVector3F);

            return vectorResult;
        }

        public void DotProduct(Vector3 vector3)
        {
            PXVector3FDotProduct(ref _pxVector3F, ref vector3._pxVector3F);
        }

        public void Rotate(Vector3 vector3)
        {

        }

        public Vector3 Interpolate(Vector3 vectorTarget, float speed)
        {
            Vector3 vectorResult = new Vector3();

            PXVector3FInterpolate(ref _pxVector3F, ref vectorTarget._pxVector3F, speed, ref vectorResult._pxVector3F);

            return vectorResult;
        }

        public override string ToString()
        {
            return "x: " + X + " y: " + Y + " z: " + Z;
        }
    }
}
