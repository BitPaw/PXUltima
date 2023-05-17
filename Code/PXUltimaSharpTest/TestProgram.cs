using System;

namespace PX.Test
{
    class Hello
    {       
        static void Main(string[] args)
        {
            Console.WriteLine("-----[C# TEST]-----");

            TestSBP.Test();

            TestDictionary.Test();

            TestFile.Test();

            TestPXOpenGL.Test();

            TestWindow.Test();

            Console.WriteLine("-----[Test Finished]-----");
        }
    }
}