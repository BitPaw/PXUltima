using PX;
using System;

namespace PXTest
{
    class Hello
    {
        static void Main(string[] args)
        {

            ///Window window = new Window();

            //window.Create("Test");

            Console.WriteLine("Test");
            DataStream dataStream = new DataStream();
            ActionResult x = dataStream.MapToMemoryReadOnly("B:/Daten/Hello.txt");



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