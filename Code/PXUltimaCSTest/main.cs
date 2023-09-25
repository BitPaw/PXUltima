using System;
using PXUltimaCSTest;

namespace PXText
{
    internal class Program
    {
        public static int Main()
        {
            PX.Window window = new PX.Window();
            window.Create(900, 200, UIUtility.GetWindowWidthPixels(), UIUtility.GetWindowHeightPixels(), PX.User.Name);


            PX.Graphic graphic = new PX.Graphic();
            graphic.Initialize(window, PX.GraphicSystem.OpenGL);

            graphic.Select();

            Rect red = Rect.RectWithPixelSize(-.5f, -0.5f, 100, UIUtility.GetWindowHeightPixels());

            Rect green = Rect.RectWithPixelPositionAndSize(UIUtility.GetWindowWidthPixels() - 100, UIUtility.GetWindowHeightPixels(), 100, 100);

            green.SetMarginRight(10);

            red.SetPositionByPixelCoords(new Vector2(0, UIUtility.GetWindowHeightPixels()));
            red.SetMarginBottom(10);
            red.SetMarginLeft(10);
            red.SetMarginTop(10);
            //red.SetMarginRight(10);


            while (true)
            {
                graphic.Clear(0.2f, 0.2f, 0.5f, 1);
                graphic.DrawColor(1, 0, 0);
                red.Draw(graphic);

                graphic.DrawColor(0, 1, 0);
                green.Draw(graphic);


                graphic.SceneDeploy();
            }






            /*           OpenGL openGL = new OpenGL();
            openGL.CreateWindowless(100,100);


            PX.Client client = new PX.Client();
            client.ConnectToServer("127.0.0.1", 25565);


            BingBong vingBong = new BingBong();

            var x = PX.Serializer.FromObject<BingBong>(vingBong);

            PX.Processor processor = new PX.Processor();
            processor.Fetch();

            Console.WriteLine("CPU " + processor.ToString());

           // Test.TestSBP.TestConnect();

            while (true) { Test.TestSocket.ConnectAndSend(); }
            */

            return 0;
        }
    }
}
