namespace ASCOM.DeE
{
    partial class SetupDialog
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
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
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.ComPortComboBox = new System.Windows.Forms.ComboBox();
            this.comboBoxRisingFalling = new System.Windows.Forms.ComboBox();
            this.label2 = new System.Windows.Forms.Label();
            this.cmdCancel = new System.Windows.Forms.Button();
            this.button1 = new System.Windows.Forms.Button();
            this.cmdOK = new System.Windows.Forms.Button();
            this.chkTrace = new System.Windows.Forms.CheckBox();
            this.chkDebug = new System.Windows.Forms.CheckBox();
            this.statusStrip1 = new System.Windows.Forms.StatusStrip();
            this.StatusLabel = new System.Windows.Forms.ToolStripStatusLabel();
            this.picASCOM = new System.Windows.Forms.PictureBox();
            ((System.ComponentModel.ISupportInitialize)(this.picASCOM)).BeginInit();
            this.SuspendLayout();
            // 
            // ComPortComboBox
            // 
            this.ComPortComboBox.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.ComPortComboBox.FormattingEnabled = true;
            this.ComPortComboBox.Location = new System.Drawing.Point(12, 41);
            this.ComPortComboBox.Name = "ComPortComboBox";
            this.ComPortComboBox.Size = new System.Drawing.Size(230, 21);
            this.ComPortComboBox.TabIndex = 12;
            this.ComPortComboBox.SelectedIndexChanged += new System.EventHandler(this.ComPortComboBox_SelectedIndexChanged);
            // 
            // comboBoxRisingFalling
            // 
            this.comboBoxRisingFalling.FormattingEnabled = true;
            this.comboBoxRisingFalling.Location = new System.Drawing.Point(12, 135);
            this.comboBoxRisingFalling.Name = "comboBoxRisingFalling";
            this.comboBoxRisingFalling.Size = new System.Drawing.Size(90, 21);
            this.comboBoxRisingFalling.TabIndex = 8;
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(238)));
            this.label2.Location = new System.Drawing.Point(9, 15);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(91, 13);
            this.label2.TabIndex = 11;
            this.label2.Text = "DeE serial port";
            // 
            // cmdCancel
            // 
            this.cmdCancel.Anchor = System.Windows.Forms.AnchorStyles.Right;
            this.cmdCancel.DialogResult = System.Windows.Forms.DialogResult.Cancel;
            this.cmdCancel.Location = new System.Drawing.Point(12, 97);
            this.cmdCancel.Name = "cmdCancel";
            this.cmdCancel.Size = new System.Drawing.Size(69, 25);
            this.cmdCancel.TabIndex = 10;
            this.cmdCancel.Text = "Cancel";
            this.cmdCancel.UseVisualStyleBackColor = true;
            this.cmdCancel.Click += new System.EventHandler(this.cmdCancel_Click);
            // 
            // button1
            // 
            this.button1.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.button1.Location = new System.Drawing.Point(173, 68);
            this.button1.Name = "button1";
            this.button1.Size = new System.Drawing.Size(69, 24);
            this.button1.TabIndex = 8;
            this.button1.Text = "Test port";
            this.button1.UseVisualStyleBackColor = true;
            this.button1.Click += new System.EventHandler(this.button1_Click);
            // 
            // cmdOK
            // 
            this.cmdOK.Anchor = System.Windows.Forms.AnchorStyles.Right;
            this.cmdOK.DialogResult = System.Windows.Forms.DialogResult.OK;
            this.cmdOK.Location = new System.Drawing.Point(173, 98);
            this.cmdOK.Name = "cmdOK";
            this.cmdOK.Size = new System.Drawing.Size(69, 24);
            this.cmdOK.TabIndex = 9;
            this.cmdOK.Text = "OK";
            this.cmdOK.UseVisualStyleBackColor = true;
            this.cmdOK.Click += new System.EventHandler(this.cmdOK_Click);
            // 
            // chkTrace
            // 
            this.chkTrace.AutoSize = true;
            this.chkTrace.Location = new System.Drawing.Point(12, 75);
            this.chkTrace.Name = "chkTrace";
            this.chkTrace.Size = new System.Drawing.Size(91, 17);
            this.chkTrace.TabIndex = 16;
            this.chkTrace.Text = "trace enabled";
            this.chkTrace.UseVisualStyleBackColor = true;
            // 
            // chkDebug
            // 
            this.chkDebug.AutoSize = true;
            this.chkDebug.Location = new System.Drawing.Point(173, 139);
            this.chkDebug.Name = "chkDebug";
            this.chkDebug.Size = new System.Drawing.Size(108, 17);
            this.chkDebug.TabIndex = 6;
            this.chkDebug.Text = "Debugging Mode";
            this.chkDebug.UseVisualStyleBackColor = true;
            // 
            // statusStrip1
            // 
            this.statusStrip1.Location = new System.Drawing.Point(0, 181);
            this.statusStrip1.Name = "statusStrip1";
            this.statusStrip1.Size = new System.Drawing.Size(326, 22);
            this.statusStrip1.TabIndex = 17;
            this.statusStrip1.Text = "statusStrip1";
            // 
            // StatusLabel
            // 
            this.StatusLabel.Name = "StatusLabel";
            this.StatusLabel.Size = new System.Drawing.Size(102, 17);
            this.StatusLabel.Text = "ASCOM DeE Setup";
            // 
            // picASCOM
            // 
            this.picASCOM.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.picASCOM.Cursor = System.Windows.Forms.Cursors.Hand;
            this.picASCOM.Location = new System.Drawing.Point(267, 15);
            this.picASCOM.Name = "picASCOM";
            this.picASCOM.Size = new System.Drawing.Size(48, 56);
            this.picASCOM.SizeMode = System.Windows.Forms.PictureBoxSizeMode.StretchImage;
            this.picASCOM.TabIndex = 13;
            this.picASCOM.TabStop = false;
            //this.picASCOM.Image = global::ASCOM.DeE.Properties.Resources.ASCOM;
            // 
            // SetupDialog
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(326, 203);
            this.Controls.Add(this.statusStrip1);
            this.Controls.Add(this.chkTrace);
            this.Controls.Add(this.ComPortComboBox);
            this.Controls.Add(this.label2);
            this.Controls.Add(this.cmdCancel);
            this.Controls.Add(this.button1);
            this.Controls.Add(this.cmdOK);
            this.Controls.Add(this.comboBoxRisingFalling);
            this.Controls.Add(this.chkDebug);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedSingle;
            this.MaximizeBox = false;
            this.MinimizeBox = false;
            this.Name = "SetupDialog";
            this.Text = "ASCOM DeE Setup";
            this.Load += new System.EventHandler(this.SetupDialog_Load);
            ((System.ComponentModel.ISupportInitialize)(this.picASCOM)).EndInit();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.ComboBox ComPortComboBox;
        private System.Windows.Forms.ComboBox comboBoxRisingFalling;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.Button cmdCancel;
        private System.Windows.Forms.Button button1;
        private System.Windows.Forms.Button cmdOK;
        private System.Windows.Forms.CheckBox chkTrace;
        private System.Windows.Forms.CheckBox chkDebug;
        private System.Windows.Forms.StatusStrip statusStrip1;
        private System.Windows.Forms.ToolStripStatusLabel StatusLabel;
        private System.Windows.Forms.PictureBox picASCOM;
    }
}