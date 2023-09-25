using System;

namespace PXText
{
    internal class Program
    {
        public static int Main()
        {
            PX.Window window = new PX.Window();
            window.Create(200, 200, 500, 500, PX.User.Name);
            


            PX.Graphic graphic = new PX.Graphic();
            graphic.Initialize(window, PX.GraphicSystem.OpenGL);

            graphic.Select();

            //PX.ShaderProgram shader = new PX.ShaderProgram();
            //graphic.ShaderProgramCreateFromString(shader, "", "");

            while (true)
            {

                
                graphic.Clear(0.2f, 0.2f, 0.5f, 1);
                graphic.DrawColor(1,0,0);
                //   graphic.RectangleDraw(-0.5f, -0.8f, 0.5f, 0.2f);
                graphic.RectangleDrawOffset(00, 0, 0, 0f);
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
