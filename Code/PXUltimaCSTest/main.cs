
using System;

namespace PX.Text
{
    internal class BingBong
    {
        public int x;
        public float y;
    }

    internal class Program
    {
        public static int Main()
        {
            Console.WriteLine("Welcome " + PX.User.Name);


            BingBong vingBong = new BingBong();

            var x = PX.Serializer.FromObject<BingBong>(vingBong);


            PX.Processor processor = new PX.Processor();
            processor.Fetch();

            Console.WriteLine("CPU " + processor.ToString());

           // Test.TestSBP.TestConnect();

            while (true) { Test.TestSocket.ConnectAndSend(); }

         


            return 0;
        }
    }
}
