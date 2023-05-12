using System;

namespace PX.Test
{
    internal class TestSBP
    {
        internal static void Test()
        {
            Console.WriteLine("[#] Start test");

            PX.SBPServer sBPServer = new PX.SBPServer();
            PX.SBPClient sBPClient = new PX.SBPClient();


            // Server events


            // Client events
            sBPClient.OnMessageReceived += OnMessageReceived;

            // Start Server
            Console.Write("[i] Start Server : ");
            ActionResult actionResult1 = sBPServer.Start(25565);

            if (actionResult1 != ActionResult.Successful)
            {
                Console.WriteLine("[failed]");
                return;
            }

            Console.WriteLine("[OK]");

            // Start Client
            Console.Write("[!] Client connect  : ");
            PX.ActionResult actionResult = sBPClient.ConnectToServer("127.0.0.1", 25565);

            if (actionResult != ActionResult.Successful)
            {
                Console.WriteLine("[failed] -> " + actionResult.ToString());
                return;
            }

            Console.WriteLine("[OK]");


            sBPClient.SendData("PXEngine test from C# Wrapper");


            Console.WriteLine("[#] Done");

            sBPServer.Dispose();
            sBPClient.Dispose();
        }

        private static void OnMessageReceived(string message)
        {
            Console.WriteLine(message);
        }
    }
}