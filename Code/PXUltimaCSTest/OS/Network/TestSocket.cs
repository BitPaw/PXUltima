using System;

namespace PX.Test
{
    internal class TestSocket
    {
        public static void ConnectAndSend()
        {
            PX.Server server = new PX.Server();
            server.OnCreating += OnCreating;
            server.OnCreated += OnCreated;
            server.OnClosed += OnClosed;
            server.OnConnected += OnConnected;
            server.OnDisconnected += OnDisconnected;
            server.OnStateChanged += OnStateChanged;
            server.OnDataSend += OnDataSend;
            server.OnDataReceive += OnDataReceive;

            PX.Client client = new PX.Client();
            client.OnCreating += OnCreating;
            client.OnCreated += OnCreated;
            client.OnClosed += OnClosed;
            client.OnConnected += OnConnected;
            client.OnDisconnected += OnDisconnected;
            client.OnStateChanged += OnStateChanged;
            client.OnDataSend += OnDataSend;
            client.OnDataReceive += OnDataReceive;

#if true
            server.Start(25565, ProtocolMode.ProtocolModeTCP);
#endif

            System.Threading.Thread.Sleep(200);

#if false
            client.ConnectToServer("127.0.0.1", 25565);

            System.Threading.Thread.Sleep(200);

            client.SendData("Hello_it_me");
#endif

            while (true) { }

            client.Dispose();
        }

        private static void OnDataReceive(SocketDataReceivedEventData socketDataReceivedEventData)
        {
            Console.WriteLine("[C#] OnDataReceive : " + socketDataReceivedEventData.ToString());
        }

        private static void OnDataSend(SocketDataSendEventData socketDataSendEventData)
        {
            Console.WriteLine("Client OnDataSend : " + socketDataSendEventData.ToString());
        }

        private static void OnStateChanged(Socket pxSocket, SocketState oldState, SocketState newState)
        {
            Console.WriteLine("[C#] OnStateChanged <" + oldState.ToString() + "> -> <" + newState.ToString() + ">");
        }

        private static void OnDisconnected(Socket serverSocket, Socket clientSocket)
        {
            Console.WriteLine("Client OnDisconnected");
        }

        private static void OnConnected(Socket serverSocket, Socket clientSocket)
        {
            Console.WriteLine("[C#] OnConnected : " + serverSocket.ToString()  + " --> "  + clientSocket.ToString());
        }

        private static void OnClosed(Socket pxSocket)
        {
            Console.WriteLine("Client OnClosed");
        }

        private static void OnCreated(Socket pxSocket)
        {
            Console.WriteLine("Client OnCreated");
        }

        private static void OnCreating(Socket pxSocket, ref bool use)
        {
            Console.WriteLine("Client OnCreating");
        }
    }
}
