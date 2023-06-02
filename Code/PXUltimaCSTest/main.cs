
using System;

internal class Program
{
    public static int Main()
    {
        Console.WriteLine("Welcome " + PX.User.Name);

        PX.Processor processor = new PX.Processor();
        processor.Fetch();

        Console.WriteLine("CPU " + processor.ToString());

        PX.Client client = new PX.Client();
       

        return 0;
    }
}