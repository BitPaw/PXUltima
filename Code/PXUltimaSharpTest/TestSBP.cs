using System;
using System.Threading;

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
            sBPServer.OnChunkReceived += ServerOnChunkReceived;
            sBPServer.OnMessageReceived += ServerOnMessageReceived;
            sBPServer.OnMessageUpdated += ServerOnMessageUpdated;

            // Client events
            sBPClient.OnChunkReceived += ClientOnChunkReceived;
            sBPClient.OnMessageReceived += ClientOnMessageReceived;
            sBPClient.OnMessageUpdated += ClientOnMessageUpdated;

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


            Thread.Sleep(1000);

            Console.WriteLine("[#] Done");

            sBPServer.Dispose();
            sBPClient.Dispose();
        }

        private static void ClientOnMessageUpdated(string message)
        {
            Console.WriteLine("[i] MESSAGE: " + message);
        }

        private static void ClientOnMessageReceived(string message)
        {
            Console.WriteLine("[i] MESSAGE: " + message);
        }

        private static void ClientOnChunkReceived(string message)
        {
            Console.WriteLine("[i] MESSAGE: " + message);
        }

        private static void ServerOnChunkReceived(string message)
        {
            Console.WriteLine("[i] MESSAGE: " + message);
        }

        private static void ServerOnMessageReceived(string message)
        {
            Console.WriteLine("[i] MESSAGE: " + message);
        }

        private static void ServerOnMessageUpdated(string message)
        {
            Console.WriteLine("[i] MESSAGE: " + message);
        }
    }
}