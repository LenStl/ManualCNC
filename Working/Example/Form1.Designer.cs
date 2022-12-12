namespace Example
{
    partial class Form1
    {
        /// <summary>
        /// Erforderliche Designervariable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Verwendete Ressourcen bereinigen.
        /// </summary>
        /// <param name="disposing">True, wenn verwaltete Ressourcen gelöscht werden sollen; andernfalls False.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Vom Windows Form-Designer generierter Code

        /// <summary>
        /// Erforderliche Methode für die Designerunterstützung.
        /// Der Inhalt der Methode darf nicht mit dem Code-Editor geändert werden.
        /// </summary>
        private void InitializeComponent()
        {
            this.components = new System.ComponentModel.Container();
            this.button = new System.Windows.Forms.Button();
            this.labelInfo = new System.Windows.Forms.Label();
            this.button1 = new System.Windows.Forms.Button();
            this.label1 = new System.Windows.Forms.Label();
            this.timerStatus = new System.Windows.Forms.Timer(this.components);
            this.button2 = new System.Windows.Forms.Button();
            this.button3 = new System.Windows.Forms.Button();
            this.button4 = new System.Windows.Forms.Button();
            this.button5 = new System.Windows.Forms.Button();
            this.button6 = new System.Windows.Forms.Button();
            this.button7 = new System.Windows.Forms.Button();
            this.button8 = new System.Windows.Forms.Button();
            this.label2 = new System.Windows.Forms.Label();
            this.comboBox1 = new System.Windows.Forms.ComboBox();
            this.label3 = new System.Windows.Forms.Label();
            this.label4 = new System.Windows.Forms.Label();
            this.label5 = new System.Windows.Forms.Label();
            this.label6 = new System.Windows.Forms.Label();
            this.buttonMoveXP = new System.Windows.Forms.Button();
            this.buttonMoveXM = new System.Windows.Forms.Button();
            this.buttonMoveYP = new System.Windows.Forms.Button();
            this.buttonMoveYM = new System.Windows.Forms.Button();
            this.buttonMoveZP = new System.Windows.Forms.Button();
            this.buttonMoveZM = new System.Windows.Forms.Button();
            this.buttonMoveXPYP = new System.Windows.Forms.Button();
            this.buttonMoveXMYP = new System.Windows.Forms.Button();
            this.buttonMoveXMYM = new System.Windows.Forms.Button();
            this.buttonMoveXPYM = new System.Windows.Forms.Button();
            this.textBox1 = new System.Windows.Forms.TextBox();
            this.label7 = new System.Windows.Forms.Label();
            this.label8 = new System.Windows.Forms.Label();
            this.SuspendLayout();
            // 
            // button
            // 
            this.button.Location = new System.Drawing.Point(12, 12);
            this.button.Name = "button";
            this.button.Size = new System.Drawing.Size(266, 28);
            this.button.TabIndex = 0;
            this.button.Text = "1. Load configuration";
            this.button.UseVisualStyleBackColor = true;
            this.button.Click += new System.EventHandler(this.buttonConnect_Click);
            // 
            // labelInfo
            // 
            this.labelInfo.AutoSize = true;
            this.labelInfo.Location = new System.Drawing.Point(20, 52);
            this.labelInfo.Name = "labelInfo";
            this.labelInfo.Size = new System.Drawing.Size(13, 13);
            this.labelInfo.TabIndex = 1;
            this.labelInfo.Text = "?";
            // 
            // button1
            // 
            this.button1.Location = new System.Drawing.Point(12, 78);
            this.button1.Name = "button1";
            this.button1.Size = new System.Drawing.Size(130, 28);
            this.button1.TabIndex = 2;
            this.button1.Text = "2. Connect";
            this.button1.UseVisualStyleBackColor = true;
            this.button1.Click += new System.EventHandler(this.button1_Click);
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(20, 119);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(13, 13);
            this.label1.TabIndex = 3;
            this.label1.Text = "?";
            // 
            // timerStatus
            // 
            this.timerStatus.Enabled = true;
            this.timerStatus.Tick += new System.EventHandler(this.timerStatus_Tick);
            // 
            // button2
            // 
            this.button2.Location = new System.Drawing.Point(148, 78);
            this.button2.Name = "button2";
            this.button2.Size = new System.Drawing.Size(130, 28);
            this.button2.TabIndex = 4;
            this.button2.Text = "Disable connection";
            this.button2.UseVisualStyleBackColor = true;
            this.button2.Click += new System.EventHandler(this.button2_Click);
            // 
            // button3
            // 
            this.button3.Location = new System.Drawing.Point(12, 152);
            this.button3.Name = "button3";
            this.button3.Size = new System.Drawing.Size(130, 28);
            this.button3.TabIndex = 5;
            this.button3.Text = "3. Reference move";
            this.button3.UseVisualStyleBackColor = true;
            this.button3.Click += new System.EventHandler(this.button3_Click);
            // 
            // button4
            // 
            this.button4.Location = new System.Drawing.Point(148, 152);
            this.button4.Name = "button4";
            this.button4.Size = new System.Drawing.Size(130, 62);
            this.button4.TabIndex = 6;
            this.button4.Text = "Abort";
            this.button4.UseVisualStyleBackColor = true;
            this.button4.Click += new System.EventHandler(this.button4_Click);
            // 
            // button5
            // 
            this.button5.Location = new System.Drawing.Point(12, 186);
            this.button5.Name = "button5";
            this.button5.Size = new System.Drawing.Size(130, 28);
            this.button5.TabIndex = 7;
            this.button5.Text = "4. Job start";
            this.button5.UseVisualStyleBackColor = true;
            this.button5.Click += new System.EventHandler(this.button5_Click);
            // 
            // button6
            // 
            this.button6.Location = new System.Drawing.Point(12, 285);
            this.button6.Name = "button6";
            this.button6.Size = new System.Drawing.Size(130, 28);
            this.button6.TabIndex = 8;
            this.button6.Text = "Switch on";
            this.button6.UseVisualStyleBackColor = true;
            this.button6.Click += new System.EventHandler(this.button6_Click);
            // 
            // button7
            // 
            this.button7.Location = new System.Drawing.Point(12, 319);
            this.button7.Name = "button7";
            this.button7.Size = new System.Drawing.Size(130, 28);
            this.button7.TabIndex = 9;
            this.button7.Text = "Switch off";
            this.button7.UseVisualStyleBackColor = true;
            this.button7.Click += new System.EventHandler(this.button7_Click);
            // 
            // button8
            // 
            this.button8.Location = new System.Drawing.Point(12, 353);
            this.button8.Name = "button8";
            this.button8.Size = new System.Drawing.Size(130, 28);
            this.button8.TabIndex = 10;
            this.button8.Text = "smc5d s/n read";
            this.button8.UseVisualStyleBackColor = true;
            this.button8.Click += new System.EventHandler(this.button8_Click);
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(20, 388);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(13, 13);
            this.label2.TabIndex = 11;
            this.label2.Text = "?";
            // 
            // comboBox1
            // 
            this.comboBox1.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.comboBox1.FormattingEnabled = true;
            this.comboBox1.Items.AddRange(new object[] {
            "1",
            "2",
            "3",
            "4",
            "5",
            "6",
            "7",
            "8",
            "9",
            "10",
            "11",
            "12",
            "13",
            "14",
            "15",
            "16",
            "17",
            "18",
            "19",
            "20",
            "21",
            "22",
            "23",
            "24"});
            this.comboBox1.Location = new System.Drawing.Point(148, 292);
            this.comboBox1.Name = "comboBox1";
            this.comboBox1.Size = new System.Drawing.Size(76, 21);
            this.comboBox1.TabIndex = 12;
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(20, 221);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(13, 13);
            this.label3.TabIndex = 13;
            this.label3.Text = "?";
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.Location = new System.Drawing.Point(20, 241);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(13, 13);
            this.label4.TabIndex = 14;
            this.label4.Text = "?";
            // 
            // label5
            // 
            this.label5.AutoSize = true;
            this.label5.Location = new System.Drawing.Point(20, 261);
            this.label5.Name = "label5";
            this.label5.Size = new System.Drawing.Size(13, 13);
            this.label5.TabIndex = 15;
            this.label5.Text = "?";
            // 
            // label6
            // 
            this.label6.AutoSize = true;
            this.label6.Location = new System.Drawing.Point(148, 327);
            this.label6.Name = "label6";
            this.label6.Size = new System.Drawing.Size(13, 13);
            this.label6.TabIndex = 16;
            this.label6.Text = "?";
            // 
            // buttonMoveXP
            // 
            this.buttonMoveXP.Location = new System.Drawing.Point(463, 156);
            this.buttonMoveXP.Name = "buttonMoveXP";
            this.buttonMoveXP.Size = new System.Drawing.Size(62, 62);
            this.buttonMoveXP.TabIndex = 17;
            this.buttonMoveXP.Text = "X+";
            this.buttonMoveXP.UseVisualStyleBackColor = true;
            this.buttonMoveXP.MouseDown += new System.Windows.Forms.MouseEventHandler(this.buttonMoveXP_MouseDown);
            this.buttonMoveXP.MouseUp += new System.Windows.Forms.MouseEventHandler(this.buttonMoveXP_MouseUp);
            // 
            // buttonMoveXM
            // 
            this.buttonMoveXM.Location = new System.Drawing.Point(327, 156);
            this.buttonMoveXM.Name = "buttonMoveXM";
            this.buttonMoveXM.Size = new System.Drawing.Size(62, 62);
            this.buttonMoveXM.TabIndex = 18;
            this.buttonMoveXM.Text = "X-";
            this.buttonMoveXM.UseVisualStyleBackColor = true;
            this.buttonMoveXM.MouseDown += new System.Windows.Forms.MouseEventHandler(this.buttonMoveXM_MouseDown);
            this.buttonMoveXM.MouseUp += new System.Windows.Forms.MouseEventHandler(this.buttonMoveXP_MouseUp);
            // 
            // buttonMoveYP
            // 
            this.buttonMoveYP.Location = new System.Drawing.Point(395, 88);
            this.buttonMoveYP.Name = "buttonMoveYP";
            this.buttonMoveYP.Size = new System.Drawing.Size(62, 62);
            this.buttonMoveYP.TabIndex = 19;
            this.buttonMoveYP.Text = "Y+";
            this.buttonMoveYP.UseVisualStyleBackColor = true;
            this.buttonMoveYP.MouseDown += new System.Windows.Forms.MouseEventHandler(this.buttonMoveYP_MouseDown);
            this.buttonMoveYP.MouseUp += new System.Windows.Forms.MouseEventHandler(this.buttonMoveXP_MouseUp);
            // 
            // buttonMoveYM
            // 
            this.buttonMoveYM.Location = new System.Drawing.Point(395, 224);
            this.buttonMoveYM.Name = "buttonMoveYM";
            this.buttonMoveYM.Size = new System.Drawing.Size(62, 62);
            this.buttonMoveYM.TabIndex = 20;
            this.buttonMoveYM.Text = "Y-";
            this.buttonMoveYM.UseVisualStyleBackColor = true;
            this.buttonMoveYM.MouseDown += new System.Windows.Forms.MouseEventHandler(this.buttonMoveYM_MouseDown);
            this.buttonMoveYM.MouseUp += new System.Windows.Forms.MouseEventHandler(this.buttonMoveXP_MouseUp);
            // 
            // buttonMoveZP
            // 
            this.buttonMoveZP.Location = new System.Drawing.Point(537, 118);
            this.buttonMoveZP.Name = "buttonMoveZP";
            this.buttonMoveZP.Size = new System.Drawing.Size(62, 62);
            this.buttonMoveZP.TabIndex = 21;
            this.buttonMoveZP.Text = "Z+";
            this.buttonMoveZP.UseVisualStyleBackColor = true;
            this.buttonMoveZP.MouseDown += new System.Windows.Forms.MouseEventHandler(this.buttonMoveZP_MouseDown);
            this.buttonMoveZP.MouseUp += new System.Windows.Forms.MouseEventHandler(this.buttonMoveXP_MouseUp);
            // 
            // buttonMoveZM
            // 
            this.buttonMoveZM.Location = new System.Drawing.Point(537, 186);
            this.buttonMoveZM.Name = "buttonMoveZM";
            this.buttonMoveZM.Size = new System.Drawing.Size(62, 62);
            this.buttonMoveZM.TabIndex = 22;
            this.buttonMoveZM.Text = "Z-";
            this.buttonMoveZM.UseVisualStyleBackColor = true;
            this.buttonMoveZM.MouseDown += new System.Windows.Forms.MouseEventHandler(this.buttonMoveZM_MouseDown);
            this.buttonMoveZM.MouseUp += new System.Windows.Forms.MouseEventHandler(this.buttonMoveXP_MouseUp);
            // 
            // buttonMoveXPYP
            // 
            this.buttonMoveXPYP.Location = new System.Drawing.Point(463, 88);
            this.buttonMoveXPYP.Name = "buttonMoveXPYP";
            this.buttonMoveXPYP.Size = new System.Drawing.Size(62, 62);
            this.buttonMoveXPYP.TabIndex = 23;
            this.buttonMoveXPYP.Text = "X+Y+";
            this.buttonMoveXPYP.UseVisualStyleBackColor = true;
            this.buttonMoveXPYP.MouseDown += new System.Windows.Forms.MouseEventHandler(this.buttonMoveXPYP_MouseDown);
            this.buttonMoveXPYP.MouseUp += new System.Windows.Forms.MouseEventHandler(this.buttonMoveXP_MouseUp);
            // 
            // buttonMoveXMYP
            // 
            this.buttonMoveXMYP.Location = new System.Drawing.Point(327, 88);
            this.buttonMoveXMYP.Name = "buttonMoveXMYP";
            this.buttonMoveXMYP.Size = new System.Drawing.Size(62, 62);
            this.buttonMoveXMYP.TabIndex = 24;
            this.buttonMoveXMYP.Text = "X+Y+";
            this.buttonMoveXMYP.UseVisualStyleBackColor = true;
            this.buttonMoveXMYP.MouseDown += new System.Windows.Forms.MouseEventHandler(this.buttonMoveXMYP_MouseDown);
            this.buttonMoveXMYP.MouseUp += new System.Windows.Forms.MouseEventHandler(this.buttonMoveXP_MouseUp);
            // 
            // buttonMoveXMYM
            // 
            this.buttonMoveXMYM.Location = new System.Drawing.Point(327, 224);
            this.buttonMoveXMYM.Name = "buttonMoveXMYM";
            this.buttonMoveXMYM.Size = new System.Drawing.Size(62, 62);
            this.buttonMoveXMYM.TabIndex = 25;
            this.buttonMoveXMYM.Text = "X-Y-";
            this.buttonMoveXMYM.UseVisualStyleBackColor = true;
            this.buttonMoveXMYM.MouseDown += new System.Windows.Forms.MouseEventHandler(this.buttonMoveXMYM_MouseDown);
            this.buttonMoveXMYM.MouseUp += new System.Windows.Forms.MouseEventHandler(this.buttonMoveXP_MouseUp);
            // 
            // buttonMoveXPYM
            // 
            this.buttonMoveXPYM.Location = new System.Drawing.Point(463, 224);
            this.buttonMoveXPYM.Name = "buttonMoveXPYM";
            this.buttonMoveXPYM.Size = new System.Drawing.Size(62, 62);
            this.buttonMoveXPYM.TabIndex = 26;
            this.buttonMoveXPYM.Text = "X+Y-";
            this.buttonMoveXPYM.UseVisualStyleBackColor = true;
            this.buttonMoveXPYM.MouseDown += new System.Windows.Forms.MouseEventHandler(this.buttonMoveXPYM_MouseDown);
            this.buttonMoveXPYM.MouseUp += new System.Windows.Forms.MouseEventHandler(this.buttonMoveXP_MouseUp);
            // 
            // textBox1
            // 
            this.textBox1.Location = new System.Drawing.Point(429, 301);
            this.textBox1.Name = "textBox1";
            this.textBox1.Size = new System.Drawing.Size(100, 20);
            this.textBox1.TabIndex = 27;
            this.textBox1.Text = "10";
            // 
            // label7
            // 
            this.label7.AutoSize = true;
            this.label7.Location = new System.Drawing.Point(385, 304);
            this.label7.Name = "label7";
            this.label7.Size = new System.Drawing.Size(38, 13);
            this.label7.TabIndex = 28;
            this.label7.Text = "Speed";
            // 
            // label8
            // 
            this.label8.AutoSize = true;
            this.label8.Location = new System.Drawing.Point(534, 304);
            this.label8.Name = "label8";
            this.label8.Size = new System.Drawing.Size(23, 13);
            this.label8.TabIndex = 29;
            this.label8.Text = "mm";
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(655, 479);
            this.Controls.Add(this.label8);
            this.Controls.Add(this.label7);
            this.Controls.Add(this.textBox1);
            this.Controls.Add(this.buttonMoveXPYM);
            this.Controls.Add(this.buttonMoveXMYM);
            this.Controls.Add(this.buttonMoveXMYP);
            this.Controls.Add(this.buttonMoveXPYP);
            this.Controls.Add(this.buttonMoveZM);
            this.Controls.Add(this.buttonMoveZP);
            this.Controls.Add(this.buttonMoveYM);
            this.Controls.Add(this.buttonMoveYP);
            this.Controls.Add(this.buttonMoveXM);
            this.Controls.Add(this.buttonMoveXP);
            this.Controls.Add(this.label6);
            this.Controls.Add(this.label5);
            this.Controls.Add(this.label4);
            this.Controls.Add(this.label3);
            this.Controls.Add(this.comboBox1);
            this.Controls.Add(this.label2);
            this.Controls.Add(this.button8);
            this.Controls.Add(this.button7);
            this.Controls.Add(this.button6);
            this.Controls.Add(this.button5);
            this.Controls.Add(this.button4);
            this.Controls.Add(this.button3);
            this.Controls.Add(this.button2);
            this.Controls.Add(this.label1);
            this.Controls.Add(this.button1);
            this.Controls.Add(this.labelInfo);
            this.Controls.Add(this.button);
            this.Name = "Form1";
            this.Text = "smc5d.core example";
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Button button;
        private System.Windows.Forms.Label labelInfo;
        private System.Windows.Forms.Button button1;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.Timer timerStatus;
        private System.Windows.Forms.Button button2;
        private System.Windows.Forms.Button button3;
        private System.Windows.Forms.Button button4;
        private System.Windows.Forms.Button button5;
        private System.Windows.Forms.Button button6;
        private System.Windows.Forms.Button button7;
        private System.Windows.Forms.Button button8;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.ComboBox comboBox1;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.Label label4;
        private System.Windows.Forms.Label label5;
        private System.Windows.Forms.Label label6;
        private System.Windows.Forms.Button buttonMoveXP;
        private System.Windows.Forms.Button buttonMoveXM;
        private System.Windows.Forms.Button buttonMoveYP;
        private System.Windows.Forms.Button buttonMoveYM;
        private System.Windows.Forms.Button buttonMoveZP;
        private System.Windows.Forms.Button buttonMoveZM;
        private System.Windows.Forms.Button buttonMoveXPYP;
        private System.Windows.Forms.Button buttonMoveXMYP;
        private System.Windows.Forms.Button buttonMoveXMYM;
        private System.Windows.Forms.Button buttonMoveXPYM;
        private System.Windows.Forms.TextBox textBox1;
        private System.Windows.Forms.Label label7;
        private System.Windows.Forms.Label label8;
    }
}

