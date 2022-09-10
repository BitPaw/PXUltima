using PX;

namespace HelloWorld
{
    class Hello
    {
        static void Main(string[] args)
        {
            PX.Directory.Create("Hallo i bims");

            Image image = new Image();

            int result = image.Load("B:/Layers.png");

            Console.WriteLine("Result:" + result + "  Width:" + image.Width + "  Height:" + image.Height);
        }
    }
}