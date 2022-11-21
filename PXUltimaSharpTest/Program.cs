using PX;
using System;

namespace PXTest
{
    class Hello
    {
        static void Main(string[] args)
        {

            Window window = new Window();

            window.Create(UInt32.MaxValue, UInt32.MaxValue, "Test");

            Console.WriteLine("Test");

            while (true)
            {

            }


                /*

            PX.Directory.Create("Hallo i bims");

            Image image = new Image();

            int result = image.Load("B:/Layers.png");

            Console.WriteLine("Result:" + result + "  Width:" + image.Width + "  Height:" + image.Height);
                */
        }
    }
}