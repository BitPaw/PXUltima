namespace PX.Test
{
    internal class TestWindow
    {
        internal static void Test()
        {
            PX.Window window = new PX.Window();
            window.Create("Hellom it me");

            while (true)
            {
                //window.FrameBufferSwap();
            }
        }
    }
}