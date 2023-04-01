using System.Runtime.InteropServices;

namespace PX
{
    [StructLayout(LayoutKind.Explicit)]
    public unsafe struct MyStruct
    {
        [FieldOffset(0)] volatile void* Thing;
    }

    public class ClipBoard
    {

    }
}