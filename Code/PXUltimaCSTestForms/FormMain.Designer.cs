using System.Drawing;
using System.Windows.Forms;

namespace PXUltimaCSTestForms
{
    partial class FormMain
    {
        /// <summary>
        ///  Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        ///  Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        ///  Required method for Designer support - do not modify
        ///  the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            components = new System.ComponentModel.Container();
            ButtonClientConnect = new Button();
            ButtonClientDisconnect = new Button();
            ButtonServerStart = new Button();
            ButtonServerStop = new Button();
            TextBoxClientIP = new TextBox();
            TextBoxClientPort = new TextBox();
            TextBoxServerPort = new TextBox();
            label1 = new Label();
            label2 = new Label();
            label3 = new Label();
            label4 = new Label();
            label5 = new Label();
            label6 = new Label();
            RichTextBoxClientInput = new RichTextBox();
            RichTextBoxLog = new RichTextBox();
            ButtonClientSend = new Button();
            TimerLogFetch = new Timer(components);
            SuspendLayout();
            // 
            // ButtonClientConnect
            // 
            ButtonClientConnect.Location = new Point(12, 109);
            ButtonClientConnect.Name = "ButtonClientConnect";
            ButtonClientConnect.Size = new Size(194, 23);
            ButtonClientConnect.TabIndex = 0;
            ButtonClientConnect.Text = "Connect";
            ButtonClientConnect.UseVisualStyleBackColor = true;
            ButtonClientConnect.Click += ButtonClientConnect_Click;
            // 
            // ButtonClientDisconnect
            // 
            ButtonClientDisconnect.Location = new Point(12, 138);
            ButtonClientDisconnect.Name = "ButtonClientDisconnect";
            ButtonClientDisconnect.Size = new Size(194, 23);
            ButtonClientDisconnect.TabIndex = 1;
            ButtonClientDisconnect.Text = "Disconnect";
            ButtonClientDisconnect.UseVisualStyleBackColor = true;
            ButtonClientDisconnect.Click += ButtonClientDisconnect_Click;
            // 
            // ButtonServerStart
            // 
            ButtonServerStart.Location = new Point(234, 109);
            ButtonServerStart.Name = "ButtonServerStart";
            ButtonServerStart.Size = new Size(194, 23);
            ButtonServerStart.TabIndex = 2;
            ButtonServerStart.Text = "Start";
            ButtonServerStart.UseVisualStyleBackColor = true;
            ButtonServerStart.Click += ButtonServerStart_Click;
            // 
            // ButtonServerStop
            // 
            ButtonServerStop.Location = new Point(234, 138);
            ButtonServerStop.Name = "ButtonServerStop";
            ButtonServerStop.Size = new Size(194, 23);
            ButtonServerStop.TabIndex = 3;
            ButtonServerStop.Text = "Stop";
            ButtonServerStop.UseVisualStyleBackColor = true;
            ButtonServerStop.Click += ButtonServerStop_Click;
            // 
            // TextBoxClientIP
            // 
            TextBoxClientIP.Location = new Point(12, 32);
            TextBoxClientIP.Name = "TextBoxClientIP";
            TextBoxClientIP.Size = new Size(150, 23);
            TextBoxClientIP.TabIndex = 5;
            // 
            // TextBoxClientPort
            // 
            TextBoxClientPort.Location = new Point(12, 61);
            TextBoxClientPort.Name = "TextBoxClientPort";
            TextBoxClientPort.Size = new Size(150, 23);
            TextBoxClientPort.TabIndex = 6;
            // 
            // TextBoxServerPort
            // 
            TextBoxServerPort.Location = new Point(234, 61);
            TextBoxServerPort.Name = "TextBoxServerPort";
            TextBoxServerPort.Size = new Size(150, 23);
            TextBoxServerPort.TabIndex = 7;
            // 
            // label1
            // 
            label1.AutoSize = true;
            label1.Location = new Point(12, 9);
            label1.Name = "label1";
            label1.Size = new Size(38, 15);
            label1.TabIndex = 8;
            label1.Text = "Client";
            // 
            // label2
            // 
            label2.AutoSize = true;
            label2.Location = new Point(390, 64);
            label2.Name = "label2";
            label2.Size = new Size(29, 15);
            label2.TabIndex = 9;
            label2.Text = "Port";
            // 
            // label3
            // 
            label3.AutoSize = true;
            label3.Location = new Point(168, 35);
            label3.Name = "label3";
            label3.Size = new Size(17, 15);
            label3.TabIndex = 10;
            label3.Text = "IP";
            // 
            // label4
            // 
            label4.AutoSize = true;
            label4.Location = new Point(234, 9);
            label4.Name = "label4";
            label4.Size = new Size(39, 15);
            label4.TabIndex = 11;
            label4.Text = "Server";
            // 
            // label5
            // 
            label5.AutoSize = true;
            label5.Location = new Point(168, 64);
            label5.Name = "label5";
            label5.Size = new Size(29, 15);
            label5.TabIndex = 12;
            label5.Text = "Port";
            // 
            // label6
            // 
            label6.AutoSize = true;
            label6.Location = new Point(448, 9);
            label6.Name = "label6";
            label6.Size = new Size(27, 15);
            label6.TabIndex = 13;
            label6.Text = "Log";
            // 
            // RichTextBoxClientInput
            // 
            RichTextBoxClientInput.Location = new Point(12, 181);
            RichTextBoxClientInput.Name = "RichTextBoxClientInput";
            RichTextBoxClientInput.Size = new Size(194, 96);
            RichTextBoxClientInput.TabIndex = 14;
            RichTextBoxClientInput.Text = "";
            // 
            // RichTextBoxLog
            // 
            RichTextBoxLog.Location = new Point(448, 35);
            RichTextBoxLog.Name = "RichTextBoxLog";
            RichTextBoxLog.Size = new Size(300, 271);
            RichTextBoxLog.TabIndex = 15;
            RichTextBoxLog.Text = "";
            // 
            // ButtonClientSend
            // 
            ButtonClientSend.Location = new Point(12, 283);
            ButtonClientSend.Name = "ButtonClientSend";
            ButtonClientSend.Size = new Size(194, 23);
            ButtonClientSend.TabIndex = 16;
            ButtonClientSend.Text = "Send";
            ButtonClientSend.UseVisualStyleBackColor = true;
            ButtonClientSend.Click += ButtonClientSend_Click;
            // 
            // TimerLogFetch
            // 
            TimerLogFetch.Enabled = true;
            TimerLogFetch.Tick += TimerLogFetch_Tick;
            // 
            // FormMain
            // 
            AutoScaleDimensions = new SizeF(7F, 15F);
            AutoScaleMode = AutoScaleMode.Font;
            ClientSize = new Size(758, 318);
            Controls.Add(ButtonClientSend);
            Controls.Add(RichTextBoxLog);
            Controls.Add(RichTextBoxClientInput);
            Controls.Add(label6);
            Controls.Add(label5);
            Controls.Add(label4);
            Controls.Add(label3);
            Controls.Add(label2);
            Controls.Add(label1);
            Controls.Add(TextBoxServerPort);
            Controls.Add(TextBoxClientPort);
            Controls.Add(TextBoxClientIP);
            Controls.Add(ButtonServerStop);
            Controls.Add(ButtonServerStart);
            Controls.Add(ButtonClientDisconnect);
            Controls.Add(ButtonClientConnect);
            Name = "FormMain";
            Text = "SBP Tester";
            ResumeLayout(false);
            PerformLayout();
        }

        #endregion

        private Button ButtonClientConnect;
        private Button ButtonClientDisconnect;
        private Button ButtonServerStart;
        private Button ButtonServerStop;
        private TextBox TextBoxClientIP;
        private TextBox TextBoxClientPort;
        private TextBox TextBoxServerPort;
        private Label label1;
        private Label label2;
        private Label label3;
        private Label label4;
        private Label label5;
        private Label label6;
        private RichTextBox RichTextBoxClientInput;
        private RichTextBox RichTextBoxLog;
        private Button ButtonClientSend;
        private Timer TimerLogFetch;
    }
}