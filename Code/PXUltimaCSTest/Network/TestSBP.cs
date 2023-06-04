using System;
using System.Threading;

namespace PX.Test
{
    internal class TestSBP
    {
        internal static void TestConnect()
        {
            PX.SBPServer sbpServer = new PX.SBPServer();
            sbpServer.OnMessageUpdated += OnMessageUpdated;
            sbpServer.OnMessageReceived += OnMessageReceived;

            sbpServer.Start(25565);

            Thread.Sleep(200);

            PX.SBPClient client = new PX.SBPClient();
            client.ConnectToServer(25565);

            Thread.Sleep(200);

            client.SendData("Hello my name is bob");

            Thread.Sleep(200);

            client.DisconnectFromServer();

            client.Dispose();
            sbpServer.Dispose();
        }

        private static void OnMessageReceived(SBPMessage sbpMessage)
        {
            Console.WriteLine("[C#] OnMessageReceived : " + sbpMessage);
        }

        private static void OnMessageUpdated(SBPMessage sbpMessage)
        {
            Console.WriteLine("[C#] OnMessageUpdated : " + sbpMessage);
        }
    }
}
