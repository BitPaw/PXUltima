using System;
using System.Collections.Generic;
using System.Runtime.InteropServices;

namespace PXTest
{
    class Hello
    {
        static void Main(string[] args)
        {
            ///Window window = new Window();

            //window.Create("Test");

#if false

            Console.WriteLine("Test");
            DataStream dataStream = new DataStream();
            ActionResult x = dataStream.MapToMemoryReadOnly("B:/Daten/Hello.txt");
#endif

            // System.IO.File.

            // PX.File.DoesExist();
            Console.WriteLine("C# TEST");

            PX.Window window = new PX.Window();
            window.Create("Hellom it me");

            while (true)
            {
                //window.FrameBufferSwap();
            }


            /*

            PX.Dictionary<int, string> dictionary = new PX.Dictionary<int, string>();

            PX.OpenGL openGL = new PX.OpenGL();
            openGL.ContextCreateWindowless(1024,1024);

            PX.Server server = new PX.Server();
            server.OnDataReceive += SocketDataReceiveEvent;
            server.OnDataSend += SocketDataSendEvent;
            server.Start(25565);
            
            */

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