
using System;

namespace PX.Text
{
    internal class Program
    {
        public static int Main()
        {
            Console.WriteLine("Welcome " + PX.User.Name);

            PX.Processor processor = new PX.Processor();
            processor.Fetch();

            Console.WriteLine("CPU " + processor.ToString());

           // Test.TestSBP.TestConnect();

            while (true) { Test.TestSocket.ConnectAndSend(); }

         


            return 0;
        }
    }
}
