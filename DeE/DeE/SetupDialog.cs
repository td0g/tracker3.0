using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using ASCOM.Utilities;
using System.Windows.Forms;

namespace ASCOM.DeE
{
    public partial class SetupDialog : Form
    {
        private List<COMPortInfo> comPorts;
        private COMPortInfo comPort;
        private ASCOM.Utilities.Serial serPort;

        public SetupDialog()
        {
            
            InitializeComponent();
            comPorts = COMPortInfo.GetCOMPortsInfo();
            if (comPorts.Count > 0) comPort = comPorts[0];
            comboBoxRisingFalling.Items.Clear();
            comboBoxRisingFalling.Items.Add("Rising");
            comboBoxRisingFalling.Items.Add("Falling");
            comboBoxRisingFalling.SelectedItem = "Rising";
            ComPortComboBox.SelectedIndex = ComPortComboBox.FindStringExact(Properties.Settings.Default.COMport);
        }

        private void cmdCancel_Click(object sender, EventArgs e)
        {
            Close();
        }

        private void cmdOK_Click(object sender, EventArgs e)
        {
            Properties.Settings.Default.trace = chkTrace.Checked;
            comPort = comPorts[ComPortComboBox.SelectedIndex];
            if (comPort != null) ASCOM.DeE.Properties.Settings.Default.COMport = comPort.Name;
            Properties.Settings.Default.Save();
            if (comboBoxRisingFalling.SelectedIndex == 1) SharedResources.rising = false;

            SharedResources.db = chkDebug.Checked;
        }

        private void ComPortComboBox_SelectedIndexChanged(object sender, EventArgs e)
        {
        }

        private void button1_Click(object sender, EventArgs e)
        {
            try
            {

                serPort = new ASCOM.Utilities.Serial();
                
                serPort.PortName = comPort.Name;
                serPort.ReceiveTimeoutMs = 2000;
                serPort.Speed = ASCOM.Utilities.SerialSpeed.ps57600;

                StatusLabel.Text = "Opening port " + serPort.PortName + " ...";
                serPort.Connected = true;
                StatusLabel.Text = "Port " + serPort.PortName + " opened";
                MessageBox.Show("Port opened successfully!", "Port opened", MessageBoxButtons.OK, MessageBoxIcon.Information);
                StatusLabel.Text = "Detecting QuidneArduino Controlled device at port " + serPort.PortName + " ... ";
                Refresh();
                System.Threading.Thread.Sleep(1000);
                serPort.ClearBuffers();
                serPort.Transmit("C/");
                string answer = serPort.ReceiveTerminated("/");

                if (answer != null && answer.Contains("Ready"))
                {
                    StatusLabel.Text = "QuidneArduino Controller detected";
                    MessageBox.Show("QuidneArduino Controller detected!", "QuidneArduino Controller detected", MessageBoxButtons.OK, MessageBoxIcon.Information);
                }
                else
                {
                    StatusLabel.Text = "QuidneArduino Controller not detected";
                    MessageBox.Show("QuidneArduino Controller has not been detected.", "QuidneArduino Controller not detected", MessageBoxButtons.OK, MessageBoxIcon.Error);
                }

            }
            catch (System.IO.IOException ioe)
            {
                MessageBox.Show("IO Exception has been thrown: " + ioe.Message, "Port not opened", MessageBoxButtons.OK, MessageBoxIcon.Error);
            }
            catch (System.UnauthorizedAccessException uae)
            {
                MessageBox.Show("Unauthorized access exception has been thrown: " + uae.Message, "Port not opened", MessageBoxButtons.OK, MessageBoxIcon.Error);
            }
            catch (SystemException se)
            {
                MessageBox.Show("System exception has been thrown: " + se.Message, "Port not opened", MessageBoxButtons.OK, MessageBoxIcon.Error);
            }
            finally
            {
                if (serPort.Connected) serPort.Connected = false;
                StatusLabel.Text = "Port " + serPort.PortName + " closed";
                serPort.Dispose();
            }
        }

        private void SetupDialog_Load(object sender, EventArgs e)
        {
            ComPortComboBox.Items.Clear();
            foreach (COMPortInfo comPort in comPorts)
            {
                ComPortComboBox.Items.Add(string.Format("{0}  {1}", comPort.Name, comPort.Description));
            }
            chkTrace.Checked = Properties.Settings.Default.trace;
            ComPortComboBox.SelectedIndex = ComPortComboBox.FindStringExact(Properties.Settings.Default.COMport);
        }
    }
}
