namespace PX
{
    public class Type
    {
        public static ushort Int16MakeEndianLittle(byte a, byte b)
        {
            return (ushort)(a | (b << 8));
        }

        public static ushort Int16MakeEndianBig(byte a, byte b)
        {
            return (ushort)((a << 8) | b);
        }

        public static uint Int24MakeEndianLittle(byte a, byte b, byte c)
        {
            return (uint)((a) | (b << 8 | (c << 16)));
        }

        public static uint Int24MakeEndianBig(byte a, byte b, byte c)
        {
            return (uint)((a << 16) | (b << 8) | (c));
        }

        public static uint Int32MakeEndianLittle(byte a, byte b, byte c, byte d)
        {
            return (uint)((a) | (b << 8 | (c << 16) | (d << 24)));
        }

        public static uint Int32MakeEndianBig(byte a, byte b, byte c, byte d)
        {
            return (uint)((a << 24) | (b << 16) | (c << 8) | (d));
        }

        public static ulong Int40MakeEndianLittle(byte a, byte b, byte c, byte d)
        {
            return (uint)((a) | (b << 8 | (c << 16) | (d << 24)));
        }

        public static ulong Int40MakeEndianBig(byte a, byte b, byte c, byte d)
        {
            return (uint)((a << 24) | (b << 16) | (c << 8) | (d));
        }

        public static ulong Int48MakeEndianLittle(byte a, byte b, byte c, byte d)
        {
            return (uint)((a) | (b << 8 | (c << 16) | (d << 24)));
        }

        public static ulong Int48MakeEndianBig(byte a, byte b, byte c, byte d)
        {
            return (uint)((a << 24) | (b << 16) | (c << 8) | (d));
        }

        public static ulong Int56MakeEndianLittle(byte a, byte b, byte c, byte d)
        {
            return (uint)((a) | (b << 8 | (c << 16) | (d << 24)));
        }

        public static ulong Int56MakeEndianBig(byte a, byte b, byte c, byte d)
        {
            return (uint)((a << 24) | (b << 16) | (c << 8) | (d));
        }

        public static ulong Int64MakeEndianLittle(byte a, byte b, byte c, byte d)
        {
            return (uint)((a) | (b << 8 | (c << 16) | (d << 24)));
        }

        public static ulong Int64MakeEndianBig(byte a, byte b, byte c, byte d)
        {
            return (uint)((a << 24) | (b << 16) | (c << 8) | (d));
        }
    }
}
