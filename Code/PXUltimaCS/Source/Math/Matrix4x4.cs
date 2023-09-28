using System.Runtime.InteropServices;

namespace PX
{
    [StructLayout(LayoutKind.Explicit, Size = 64)]
    internal unsafe struct PXMatrix4x4F
    {
        [FieldOffset(0)] public float XX;
        [FieldOffset(4)] public float XY;
        [FieldOffset(8)] public float XZ;
        [FieldOffset(12)] public float XW;
        [FieldOffset(16)] public float YX;
        [FieldOffset(20)] public float YY;
        [FieldOffset(24)] public float YZ;
        [FieldOffset(28)] public float YW;
        [FieldOffset(32)] public float ZX;
        [FieldOffset(36)] public float ZY;
        [FieldOffset(40)] public float ZZ;
        [FieldOffset(44)] public float ZW;
        [FieldOffset(48)] public float WX;
        [FieldOffset(52)] public float WY;
        [FieldOffset(56)] public float WZ;
        [FieldOffset(60)] public float WW;

        [FieldOffset(0)] fixed float Data[16];

        // [FieldOffset(0)] fixed float DataXY[4,4];	// You cant do two-dimensional arrays like this in C# ??
    }



    public class Matrix4x4
    {


        [DllImport("PXUltima.dll", CallingConvention = CallingConvention.StdCall)] private static extern void PXMatrix4x4FIdentity(ref PXMatrix4x4F matrix4x4F);
        [DllImport("PXUltima.dll", CallingConvention = CallingConvention.StdCall)] private static extern void PXMatrix4x4FResetAxisW(ref PXMatrix4x4F matrix4x4F);

        [DllImport("PXUltima.dll", CallingConvention = CallingConvention.StdCall)] private static extern void PXMatrix4x4FPositionGet(ref PXMatrix4x4F matrix, ref PXVector3F position);
        [DllImport("PXUltima.dll", CallingConvention = CallingConvention.StdCall)] private static extern void PXMatrix4x4FPositionSet(ref PXMatrix4x4F matrixInput, ref PXVector3F position);


        [DllImport("PXUltima.dll", CallingConvention = CallingConvention.StdCall)] private static extern void PXMatrix4x4FMultiply(ref PXMatrix4x4F matrixA, ref PXMatrix4x4F matrixB, ref PXMatrix4x4F matrixResult);
        [DllImport("PXUltima.dll", CallingConvention = CallingConvention.StdCall)] private static extern void PXMatrix4x4FRotate(ref PXMatrix4x4F matrixA, float x, float y, float z);

        [DllImport("PXUltima.dll", CallingConvention = CallingConvention.StdCall)] private static extern void PXMatrix4x4FCopy(ref PXMatrix4x4F matrixA, ref PXMatrix4x4F matrixResult);

        [DllImport("PXUltima.dll", CallingConvention = CallingConvention.StdCall)] private static extern void PXMatrix4x4FMove3F(ref PXMatrix4x4F matrixA, PXVector3F vector3F);
        [DllImport("PXUltima.dll", CallingConvention = CallingConvention.StdCall)] private static extern void PXMatrix4x4FMoveXY(ref PXMatrix4x4F matrixA, float x, float y);
        [DllImport("PXUltima.dll", CallingConvention = CallingConvention.StdCall)] private static extern void PXMatrix4x4FMoveToScaleXY(ref PXMatrix4x4F matrixA, float x, float y);
        [DllImport("PXUltima.dll", CallingConvention = CallingConvention.StdCall)] private static extern void PXMatrix4x4FMoveXYZ(ref PXMatrix4x4F matrixA, float x, float y, float z);

        [DllImport("PXUltima.dll", CallingConvention = CallingConvention.StdCall)] private static extern void PXMatrix4x4FScaleBy(ref PXMatrix4x4F matrixA, float scalar);

        [DllImport("PXUltima.dll", CallingConvention = CallingConvention.StdCall)] private static extern void PXMatrix4x4FScaleByMargin(ref PXMatrix4x4F pxMatrix4x4F, ref PXRectangleOffset pxMargin);
        [DllImport("PXUltima.dll", CallingConvention = CallingConvention.StdCall)] private static extern void PXMatrix4x4FScaleByXY(ref PXMatrix4x4F pxMatrix4x4F, float x, float y);

        [DllImport("PXUltima.dll", CallingConvention = CallingConvention.StdCall)] private static extern void PXMatrix4x4FScaleSet(ref PXMatrix4x4F pxMatrix4x4F, ref PXVector3F pxVector3F);
        [DllImport("PXUltima.dll", CallingConvention = CallingConvention.StdCall)] private static extern void PXMatrix4x4FScaleSetXY(ref PXMatrix4x4F pxMatrix4x4F, float x, float y);
        [DllImport("PXUltima.dll", CallingConvention = CallingConvention.StdCall)] private static extern void PXMatrix4x4FScaleSetXYZ(ref PXMatrix4x4F pxMatrix4x4F, float x, float y, float z);

        [DllImport("PXUltima.dll", CallingConvention = CallingConvention.StdCall)] private static extern void PXMatrix4x4FScaleGet(ref PXMatrix4x4F matrixResult, ref PXVector3F pxVector3F);

        [DllImport("PXUltima.dll", CallingConvention = CallingConvention.StdCall)] private static extern void PXMatrix4x4FOrthographic(ref PXMatrix4x4F matrix4x4F, float left, float right, float bottom, float top, float nearPlane, float farPlane);
        [DllImport("PXUltima.dll", CallingConvention = CallingConvention.StdCall)] private static extern void PXMatrix4x4FPerspective(ref PXMatrix4x4F matrix4x4F, float fielfOfView, float aspectRatio, float nearPlane, float farPlane);

        [DllImport("PXUltima.dll", CallingConvention = CallingConvention.StdCall)] private static extern byte PXMatrix4x4FInverse(ref PXMatrix4x4F matrix4x4FInput);
        [DllImport("PXUltima.dll", CallingConvention = CallingConvention.StdCall)] private static extern void PXMatrix4x4FTranpose(ref PXMatrix4x4F matrix4x4FInput);
        [DllImport("PXUltima.dll", CallingConvention = CallingConvention.StdCall)] private static extern void PXMatrix4x4FLookAt(ref PXMatrix4x4F matrix4x4FInput, ref PXVector3F eye, ref PXVector3F center, ref PXVector3F up);


        internal PXMatrix4x4F _pxMatrix4x4F = new PXMatrix4x4F();

        public Vector3 Position
        {
            get
            {
                Vector3 vector3 = new Vector3();

                PXMatrix4x4FPositionGet(ref _pxMatrix4x4F, ref vector3._pxVector3F);

                return vector3;
            }
            set
            {
                PXMatrix4x4FPositionSet(ref _pxMatrix4x4F, ref value._pxVector3F);
            }
        }

        public Vector3 Scale
        {
            get
            {
                Vector3 vector3 = new Vector3();

                PXMatrix4x4FScaleGet(ref _pxMatrix4x4F, ref vector3._pxVector3F);

                return vector3;
            }
            set
            {
                PXMatrix4x4FScaleSet(ref _pxMatrix4x4F, ref value._pxVector3F);
            }
        }

        public void Identity()
        {
            PXMatrix4x4FIdentity(ref _pxMatrix4x4F);
        }

        public void ResetAxisW()
        {
            PXMatrix4x4FResetAxisW(ref _pxMatrix4x4F);
        }

        public static Matrix4x4 operator +(Matrix4x4 a, Matrix4x4 b)
        {
            Matrix4x4 matrix4X4 = new Matrix4x4();

            //PXMatrix4x4FMultiply(ref a._pxMatrix4x4F, ref b._pxMatrix4x4F, ref matrix4X4._pxMatrix4x4F);

            return matrix4X4;
        }

        public static Matrix4x4 operator -(Matrix4x4 a, Matrix4x4 b)
        {
            Matrix4x4 matrix4X4 = new Matrix4x4();

            //  PXMatrix4x4FMultiply(ref a._pxMatrix4x4F, ref b._pxMatrix4x4F, ref matrix4X4._pxMatrix4x4F);

            return matrix4X4;
        }

        public static Matrix4x4 operator *(Matrix4x4 a, Matrix4x4 b)
        {
            Matrix4x4 matrix4X4 = new Matrix4x4();

            PXMatrix4x4FMultiply(ref a._pxMatrix4x4F, ref b._pxMatrix4x4F, ref matrix4X4._pxMatrix4x4F);

            return matrix4X4;
        }

        public void Rotate(float x, float y, float z)
        {
            PXMatrix4x4FRotate(ref _pxMatrix4x4F, x, y, z);
        }

        public void Orthographic(float left, float right, float bottom, float top, float nearPlane, float farPlane)
        {
            PXMatrix4x4FOrthographic(ref _pxMatrix4x4F, left, right, bottom, top, nearPlane, farPlane);
        }

        public void Perspective(float fielfOfView, float aspectRatio, float nearPlane, float farPlane)
        {
            PXMatrix4x4FPerspective(ref _pxMatrix4x4F, fielfOfView, aspectRatio, nearPlane, farPlane);
        }

        public void Inverse()
        {
            PXMatrix4x4FInverse(ref _pxMatrix4x4F);
        }

        public void Tranpose()
        {
            PXMatrix4x4FTranpose(ref _pxMatrix4x4F);
        }

        public void LookAt(Vector3 eye, Vector3 center, Vector3 up)
        {
            PXMatrix4x4FLookAt(ref _pxMatrix4x4F, ref eye._pxVector3F, ref center._pxVector3F, ref up._pxVector3F);
        }
    }
}