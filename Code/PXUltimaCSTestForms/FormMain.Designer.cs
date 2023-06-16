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
            this.components = new System.ComponentModel.Container();
            this.ButtonClientConnect = new System.Windows.Forms.Button();
            this.ButtonClientDisconnect = new System.Windows.Forms.Button();
            this.ButtonServerStart = new System.Windows.Forms.Button();
            this.ButtonServerStop = new System.Windows.Forms.Button();
            this.TextBoxClientIP = new System.Windows.Forms.TextBox();
            this.TextBoxClientPort = new System.Windows.Forms.TextBox();
            this.TextBoxServerPort = new System.Windows.Forms.TextBox();
            this.label1 = new System.Windows.Forms.Label();
            this.label2 = new System.Windows.Forms.Label();
            this.label3 = new System.Windows.Forms.Label();
            this.label4 = new System.Windows.Forms.Label();
            this.label5 = new System.Windows.Forms.Label();
            this.label6 = new System.Windows.Forms.Label();
            this.RichTextBoxClientInput = new System.Windows.Forms.RichTextBox();
            this.RichTextBoxLog = new System.Windows.Forms.RichTextBox();
            this.ButtonClientSend = new System.Windows.Forms.Button();
            this.TimerLogFetch = new System.Windows.Forms.Timer(this.components);
            this.SuspendLayout();
            // 
            // ButtonClientConnect
            // 
            this.ButtonClientConnect.Location = new System.Drawing.Point(21, 182);
            this.ButtonClientConnect.Margin = new System.Windows.Forms.Padding(5, 5, 5, 5);
            this.ButtonClientConnect.Name = "ButtonClientConnect";
            this.ButtonClientConnect.Size = new System.Drawing.Size(333, 38);
            this.ButtonClientConnect.TabIndex = 0;
            this.ButtonClientConnect.Text = "Connect";
            this.ButtonClientConnect.UseVisualStyleBackColor = true;
            // 
            // ButtonClientDisconnect
            // 
            this.ButtonClientDisconnect.Location = new System.Drawing.Point(21, 230);
            this.ButtonClientDisconnect.Margin = new System.Windows.Forms.Padding(5, 5, 5, 5);
            this.ButtonClientDisconnect.Name = "ButtonClientDisconnect";
            this.ButtonClientDisconnect.Size = new System.Drawing.Size(333, 38);
            this.ButtonClientDisconnect.TabIndex = 1;
            this.ButtonClientDisconnect.Text = "Disconnect";
            this.ButtonClientDisconnect.UseVisualStyleBackColor = true;
            // 
            // ButtonServerStart
            // 
            this.ButtonServerStart.Location = new System.Drawing.Point(401, 182);
            this.ButtonServerStart.Margin = new System.Windows.Forms.Padding(5, 5, 5, 5);
            this.ButtonServerStart.Name = "ButtonServerStart";
            this.ButtonServerStart.Size = new System.Drawing.Size(333, 38);
            this.ButtonServerStart.TabIndex = 2;
            this.ButtonServerStart.Text = "Start";
            this.ButtonServerStart.UseVisualStyleBackColor = true;
            this.ButtonServerStart.Click += new System.EventHandler(this.ButtonServerStart_Click_1);
            // 
            // ButtonServerStop
            // 
            this.ButtonServerStop.Location = new System.Drawing.Point(401, 230);
            this.ButtonServerStop.Margin = new System.Windows.Forms.Padding(5, 5, 5, 5);
            this.ButtonServerStop.Name = "ButtonServerStop";
            this.ButtonServerStop.Size = new System.Drawing.Size(333, 38);
            this.ButtonServerStop.TabIndex = 3;
            this.ButtonServerStop.Text = "Stop";
            this.ButtonServerStop.UseVisualStyleBackColor = true;
            // 
            // TextBoxClientIP
            // 
            this.TextBoxClientIP.Location = new System.Drawing.Point(21, 53);
            this.TextBoxClientIP.Margin = new System.Windows.Forms.Padding(5, 5, 5, 5);
            this.TextBoxClientIP.Name = "TextBoxClientIP";
            this.TextBoxClientIP.Size = new System.Drawing.Size(254, 31);
            this.TextBoxClientIP.TabIndex = 5;
            // 
            // TextBoxClientPort
            // 
            this.TextBoxClientPort.Location = new System.Drawing.Point(21, 102);
            this.TextBoxClientPort.Margin = new System.Windows.Forms.Padding(5, 5, 5, 5);
            this.TextBoxClientPort.Name = "TextBoxClientPort";
            this.TextBoxClientPort.Size = new System.Drawing.Size(254, 31);
            this.TextBoxClientPort.TabIndex = 6;
            // 
            // TextBoxServerPort
            // 
            this.TextBoxServerPort.Location = new System.Drawing.Point(401, 102);
            this.TextBoxServerPort.Margin = new System.Windows.Forms.Padding(5, 5, 5, 5);
            this.TextBoxServerPort.Name = "TextBoxServerPort";
            this.TextBoxServerPort.Size = new System.Drawing.Size(254, 31);
            this.TextBoxServerPort.TabIndex = 7;
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(21, 15);
            this.label1.Margin = new System.Windows.Forms.Padding(5, 0, 5, 0);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(67, 25);
            this.label1.TabIndex = 8;
            this.label1.Text = "Client";
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(669, 107);
            this.label2.Margin = new System.Windows.Forms.Padding(5, 0, 5, 0);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(51, 25);
            this.label2.TabIndex = 9;
            this.label2.Text = "Port";
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(288, 58);
            this.label3.Margin = new System.Windows.Forms.Padding(5, 0, 5, 0);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(31, 25);
            this.label3.TabIndex = 10;
            this.label3.Text = "IP";
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.Location = new System.Drawing.Point(401, 15);
            this.label4.Margin = new System.Windows.Forms.Padding(5, 0, 5, 0);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(75, 25);
            this.label4.TabIndex = 11;
            this.label4.Text = "Server";
            // 
            // label5
            // 
            this.label5.AutoSize = true;
            this.label5.Location = new System.Drawing.Point(288, 107);
            this.label5.Margin = new System.Windows.Forms.Padding(5, 0, 5, 0);
            this.label5.Name = "label5";
            this.label5.Size = new System.Drawing.Size(51, 25);
            this.label5.TabIndex = 12;
            this.label5.Text = "Port";
            // 
            // label6
            // 
            this.label6.AutoSize = true;
            this.label6.Location = new System.Drawing.Point(768, 15);
            this.label6.Margin = new System.Windows.Forms.Padding(5, 0, 5, 0);
            this.label6.Name = "label6";
            this.label6.Size = new System.Drawing.Size(48, 25);
            this.label6.TabIndex = 13;
            this.label6.Text = "Log";
            // 
            // RichTextBoxClientInput
            // 
            this.RichTextBoxClientInput.Location = new System.Drawing.Point(21, 302);
            this.RichTextBoxClientInput.Margin = new System.Windows.Forms.Padding(5, 5, 5, 5);
            this.RichTextBoxClientInput.Name = "RichTextBoxClientInput";
            this.RichTextBoxClientInput.Size = new System.Drawing.Size(330, 157);
            this.RichTextBoxClientInput.TabIndex = 14;
            this.RichTextBoxClientInput.Text = "";
            // 
            // RichTextBoxLog
            // 
            this.RichTextBoxLog.Location = new System.Drawing.Point(768, 58);
            this.RichTextBoxLog.Margin = new System.Windows.Forms.Padding(5, 5, 5, 5);
            this.RichTextBoxLog.Name = "RichTextBoxLog";
            this.RichTextBoxLog.Size = new System.Drawing.Size(511, 449);
            this.RichTextBoxLog.TabIndex = 15;
            this.RichTextBoxLog.Text = "";
            // 
            // ButtonClientSend
            // 
            this.ButtonClientSend.Location = new System.Drawing.Point(21, 472);
            this.ButtonClientSend.Margin = new System.Windows.Forms.Padding(5, 5, 5, 5);
            this.ButtonClientSend.Name = "ButtonClientSend";
            this.ButtonClientSend.Size = new System.Drawing.Size(333, 38);
            this.ButtonClientSend.TabIndex = 16;
            this.ButtonClientSend.Text = "Send";
            this.ButtonClientSend.UseVisualStyleBackColor = true;
            // 
            // TimerLogFetch
            // 
            this.TimerLogFetch.Interval = 10;
            // 
            // FormMain
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(12F, 25F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(1299, 530);
            this.Controls.Add(this.ButtonClientSend);
            this.Controls.Add(this.RichTextBoxLog);
            this.Controls.Add(this.RichTextBoxClientInput);
            this.Controls.Add(this.label6);
            this.Controls.Add(this.label5);
            this.Controls.Add(this.label4);
            this.Controls.Add(this.label3);
            this.Controls.Add(this.label2);
            this.Controls.Add(this.label1);
            this.Controls.Add(this.TextBoxServerPort);
            this.Controls.Add(this.TextBoxClientPort);
            this.Controls.Add(this.TextBoxClientIP);
            this.Controls.Add(this.ButtonServerStop);
            this.Controls.Add(this.ButtonServerStart);
            this.Controls.Add(this.ButtonClientDisconnect);
            this.Controls.Add(this.ButtonClientConnect);
            this.Margin = new System.Windows.Forms.Padding(5, 5, 5, 5);
            this.Name = "FormMain";
            this.Text = "SBP Tester";
            this.Load += new System.EventHandler(this.FormMain_Load);
            this.ResumeLayout(false);
            this.PerformLayout();

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
        private System.Windows.Forms.Timer TimerLogFetch;
    }
}