using PX;
using System;
using System.Collections.Generic;
using System.Windows.Forms;

namespace PXUltimaCSTestForms
{
    public partial class FormMain : Form
    {
        private PX.SBPServer _sbpServer = new PX.SBPServer();
        private PX.SBPClient _sbpClient = new PX.SBPClient();

        private Queue<string> _messageQueue = new Queue<string>();

        public FormMain()
        {
            InitializeComponent();

            TextBoxClientIP.Text = "127.0.0.1";
            TextBoxClientPort.Text = "25565";
            TextBoxServerPort.Text = "25565";

            _sbpClient.OnMessageUpdated += OnClientMessageUpdated;
            _sbpClient.OnMessageReceived += OnClientMessageReceived;

            _sbpServer.OnMessageUpdated += OnServerMessageUpdated;
            _sbpServer.OnMessageReceived += OnServerMessageReceived;

            TimerLogFetch.Start();
        }

        private void OnClientMessageUpdated(SBPMessage sbpMessage)
        {
            AddToLog("Client message updated : " + sbpMessage.ToString());
        }

        private void OnClientMessageReceived(SBPMessage sbpMessage)
        {
            AddToLog("Client message received : " + sbpMessage.ToString());
        }

        private void OnServerMessageUpdated(SBPMessage sbpMessage)
        {
            AddToLog("Server message updated : " + sbpMessage.ToString());
        }

        private void OnServerMessageReceived(SBPMessage sbpMessage)
        {
            AddToLog("Server message received : " + sbpMessage.ToString());
        }

        private void AddToLog(string message)
        {
            _messageQueue.Enqueue(message + "\n");

            //RichTextBoxLog.AppendText(message);
            //RichTextBoxLog.AppendText("\n");
        }

        private void ButtonClientConnect_Click(object sender, EventArgs e)
        {
            string ip = TextBoxClientIP.Text;
            string portText = TextBoxClientPort.Text;

            ushort portID = ushort.Parse(portText);

            PX.ActionResult connectResult = _sbpClient.ConnectToServer(ip, portID);

            AddToLog("Client connect : " + connectResult);
        }

        private void ButtonClientDisconnect_Click(object sender, EventArgs e)
        {
            PX.ActionResult connectResult = _sbpClient.DisconnectFromServer();

            AddToLog("Client disconnect : " + connectResult);
        }

        private void ButtonClientSend_Click(object sender, EventArgs e)
        {
            string message = RichTextBoxClientInput.Text;

            PX.ActionResult connectResult = _sbpClient.SendData(message);

            if (connectResult == PX.ActionResult.Successful)
            {
                RichTextBoxClientInput.Clear();
            }

            AddToLog("Client send : " + connectResult);
        }

        private void ButtonServerStart_Click(object sender, EventArgs e)
        {
            string portText = TextBoxServerPort.Text;

            ushort portID = ushort.Parse(portText);

            PX.ActionResult startResult = _sbpServer.Start(portID);

            AddToLog("Server start : " + startResult);
        }

        private void ButtonServerStop_Click(object sender, EventArgs e)
        {
            PX.ActionResult stopResult = _sbpServer.Stop();

            AddToLog("Server stop : " + stopResult);
        }

        private void TimerLogFetch_Tick(object sender, EventArgs e)
        {
            if(_messageQueue.Count > 0)
            {
                string message = _messageQueue.Dequeue();

                RichTextBoxLog.AppendText(message);
            }
        }

        private void FormMain_Load(object sender, EventArgs e)
        {

        }

        private void ButtonServerStart_Click_1(object sender, EventArgs e)
        {

        }
    }
}