using System;
using PXUltimaCSTest;

namespace PXText
{
    internal class Program
    {
        public static int Main()
        {
            PX.Window window = new PX.Window();
            //  window.Create(900, 200, UIUtility.GetWindowWidthPixels(), UIUtility.GetWindowHeightPixels(), PX.User.Name); // window.Width..?
            window.Create(PX.User.Name);

            PX.Graphic graphic = new PX.Graphic();
            graphic.Initialize(window, PX.GraphicSystem.OpenGL);

            graphic.Select();

            PX.UIElement element = new PX.UIElement();
            graphic.UIElementCreate(element);
            graphic.UIElementTypeSet(element, PX.UIElementType.Panel);
            graphic.UIElementSizeSet(element, 0, 0, 0, 0);


            Rect red = Rect.RectWithPixelSize(-.5f, -0.5f, 100, UIUtility.GetWindowHeightPixels());
            Rect green = Rect.RectWithPixelPositionAndSize(UIUtility.GetWindowWidthPixels() - 100, UIUtility.GetWindowHeightPixels(), 100, 100);

            green.SetMarginRight(10);
            red.SetPositionByPixelCoords(new Vector2(0, UIUtility.GetWindowHeightPixels()));

            red.SetColor(new Vector4(9, 100, 255, 255));
            red.SetBorderColor(new Vector4(255, 0, 0, 255));
            red.SetBorder(new RectProperties(1, 1, 1, 1));
            red.SetPadding(new RectProperties(5, 35, 5, 35));

            red.SetMarginBottom(10);

            red.SetMarginLeft(10);
            red.SetMarginTop(10);

            green.SetBorder(new RectProperties(1, 1, 1, 1));
            green.SetBorderColor(new Vector4(255, 0, 255, 255));
            //red.SetMarginRight(10);


            while (true)
            {
                graphic.Clear(0.2f, 0.2f, 0.5f, 1);

                red.Draw(graphic);
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
