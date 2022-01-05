//
// ================
// Shared Resources
// ================
//
// This class is a container for all shared resources that may be needed
// by the drivers served by the Local Server. 
//
// NOTES:
//
//	* ALL DECLARATIONS MUST BE STATIC HERE!! INSTANCES OF THIS CLASS MUST NEVER BE CREATED!
//
// Written by:	Bob Denny	29-May-2007
// Modified by Chris Rowland and Peter Simpson to hamdle multiple hardware devices March 2011
//
using System;
using System.Collections.Generic;
using System.Text;
using ASCOM;
using ASCOM.Utilities;
using System.Windows.Forms;

namespace ASCOM.DeE
{
    /// <summary>
    /// The resources shared by all drivers and devices, in this example it's a serial port with a shared SendMessage method
    /// an idea for locking the message and handling connecting is given.
    /// In reality extensive changes will probably be needed.
    /// Multiple drivers means that several applications connect to the same hardware device, aka a hub.
    /// Multiple devices means that there are more than one instance of the hardware, such as two focusers.
    /// In this case there needs to be multiple instances of the hardware connector, each with it's own connection count.
    /// </summary>
    public static class SharedResources
    {
        // object used for locking to prevent multiple drivers accessing common code at the same time
        private static readonly object lockObject = new object();

        // Shared serial port. This will allow multiple drivers to use one single serial port.
        private static ASCOM.Utilities.Serial s_sharedSerial = new ASCOM.Utilities.Serial();        // Shared serial port
        private static int s_z = 0;     // counter for the number of connections to the serial port

        private static TraceLogger traceLogger;

        public static Boolean telescopeRASlewing = true;
        public static Boolean telescopeRAcorrect;
        public static double telescopeRAPosition;
        public static Boolean telescopeDECSlewing = true;
        public static Boolean telescopeDECcorrect;
        public static double telescopeDECPosition;

        public static Boolean telescopeWasPulseGuiding = false;

        public static int focuserPosition = 0;
        public static int focuserSteps = 520000;
        public static Boolean focuserWasMoving = false;

        public static Boolean rising = true; //Rising or Falling Telescope
        public static Boolean db = false; //Sends debug param to controller

        public static Boolean telescopeParked; //Sends debug param to controller

        //
        // Public access to shared resources
        //

        public static TraceLogger tl
        {
            get
            {
                if (traceLogger == null)
                {
                    traceLogger = new TraceLogger("", "DeE.LocalServer");
                    traceLogger.Enabled = true;
                }
                return traceLogger;
            }
        }



        #region single serial port connector
        //
        // this region shows a way that a single serial port could be connected to by multiple 
        // drivers.
        //
        // Connected is used to handle the connections to the port.
        //
        // SendMessage is a way that messages could be sent to the hardware without
        // conflicts between different drivers.
        //
        // All this is for a single connection, multiple connections would need multiple ports
        // and a way to handle connecting and disconnection from them - see the
        // multi driver handling section for ideas.
        //

        /// <summary>
        /// Shared serial port
        /// </summary>
        public static ASCOM.Utilities.Serial SharedSerial { get { return s_sharedSerial; } }

        /// <summary>
        /// number of connections to the shared serial port
        /// </summary>
        public static int connections { get { return s_z; } set { s_z = value; } }

        /// <summary>
        /// Example of a shared SendMessage method, the lock
        /// prevents different drivers tripping over one another.
        /// It needs error handling and assumes that the message will be sent unchanged
        /// and that the reply will always be terminated by a "#" character.
        /// </summary>
        /// <param name="message"></param>
        /// <returns></returns>

        public static string SendMessage(string message, Boolean reply)
        {
            lock (lockObject)
            {
                tl.LogMessage("DeE Controller", "Lock Object");

                string msg = message + "\n";

                if (SharedSerial.Connected && !String.IsNullOrEmpty(msg))
                {
                    tl.LogMessage("DeE", "Send Msg: " + msg);

                    SharedSerial.ClearBuffers();
                    SharedSerial.Transmit(msg);
                    if (reply)
                    {
                        string strRec = SharedSerial.ReceiveTerminated("/");
                        SharedSerial.ClearBuffers();

                        strRec = strRec.Substring(0, strRec.Length - 1);

                        tl.LogMessage("DeE", "Response Msg: " + strRec);
                        
                        return strRec;
                    }
                    else return "";
                }
                else
                {
                    tl.LogMessage("DeE Controller", "Not Connected or Empty Send Msg: " + message);
                    return "";
                }
            }
        }

        public static int SendMessageValue(string message, byte responseLength)
        {
            lock (lockObject)
            {
                tl.LogMessage("DeE Controller", "Lock Object");
                if (SharedSerial.Connected && !String.IsNullOrEmpty(message))
                {
                    tl.LogMessage("DeE", "SendMessageValue: " + message + "\n");
                    SharedSerial.ClearBuffers();
                    SharedSerial.Transmit(message + "\n");
                    byte response = SharedSerial.ReceiveByte();
                    if (response == 113)
                    {
                        int val;
                        val = SharedSerial.ReceiveByte();
                        if (responseLength > 1) val += SharedSerial.ReceiveByte() * 256;
                        if (responseLength > 2) val += SharedSerial.ReceiveByte() * 256 * 256;
                        if (responseLength > 3) val += SharedSerial.ReceiveByte() * 256 * 256 * 256;

                        SharedSerial.ClearBuffers();
                        tl.LogMessage("DeE", "Response Value: " + val.ToString());

                        return val;
                    }
                    else
                    {
                        tl.LogMessage("DeE Controller", "Response failed: " + response);
                        return 0;
                    }
                }
                else
                {
                    tl.LogMessage("DeE Controller", "Not Connected or Empty Send Msg: " + message);
                    return 0;
                }
            }
        }



        /// <summary>
        /// Example of handling connecting to and disconnection from the
        /// shared serial port.
        /// Needs error handling
        /// the port name etc. needs to be set up first, this could be done by the driver
        /// checking Connected and if it's false setting up the port before setting connected to true.
        /// It could also be put here.
        /// </summary>
        public static bool Connected
        {
            set
            {
                lock (lockObject)
                {
                    if (value)
                    {
                        if (s_z == 0)
                        {
                            try
                            {
                                SharedSerial.PortName = ASCOM.DeE.Properties.Settings.Default.COMport;
                                SharedSerial.ReceiveTimeoutMs = 2000;
                                SharedSerial.Speed = ASCOM.Utilities.SerialSpeed.ps57600;
                                //                                SharedSerial.Handshake = ASCOM.Utilities.SerialHandshake.None;
                                SharedSerial.Connected = true;
                                
                                SharedSerial.Transmit("C");
                                
                                string answer = SharedSerial.ReceiveTerminated("/");
                                if (!answer.Contains("Ready"))
                                {
                                    MessageBox.Show("DeE device not detected at port " + SharedResources.SharedSerial.PortName, "Device not detected", MessageBoxButtons.OK, MessageBoxIcon.Error);
                                    SharedResources.tl.LogMessage("Connected answer", "Wrong answer " + answer);
                                }

                                if (rising) SharedResources.SendMessage("dd1", false);
                                else SharedResources.SendMessage("dd0", false);
                                SharedSerial.ReceiveTimeoutMs = 500;
                                if (db == true) SharedResources.SendMessage("eso", false);
                                string temp = SendMessageString("T");
                                temp = temp.Substring(temp.Length - 1, 1);
                                SharedResources.tl.LogMessage("Parked Response:", temp);
                                if (temp == "n") telescopeParked = true;
                                else telescopeParked = false;
                            }
                            catch (System.IO.IOException exception)
                            {
                                MessageBox.Show("DeE Serial port not opened for " + SharedResources.SharedSerial.PortName, "Invalid port state", MessageBoxButtons.OK, MessageBoxIcon.Error);
                                SharedResources.tl.LogMessage("Serial port not opened", exception.Message);
                            }
                            catch (System.UnauthorizedAccessException exception)
                            {
                                MessageBox.Show("DeE Access denied to serial port " + SharedResources.SharedSerial.PortName, "Access denied", MessageBoxButtons.OK, MessageBoxIcon.Error);
                                SharedResources.tl.LogMessage("Access denied to serial port", exception.Message);
                            }
                            catch (ASCOM.DriverAccessCOMException exception)
                            {
                                MessageBox.Show("DeE ASCOM driver exception: " + exception.Message, "ASCOM driver exception", MessageBoxButtons.OK, MessageBoxIcon.Error);
                            }
                            catch (System.Runtime.InteropServices.COMException exception)
                            {
                                MessageBox.Show("DeE Serial port read timeout for port " + SharedResources.SharedSerial.PortName, "Timeout", MessageBoxButtons.OK, MessageBoxIcon.Error);
                                SharedResources.tl.LogMessage("DeE Serial port read timeout", exception.Message);
                            }
                        }
                        s_z++;
                    }
                    else
                    {
                        s_z--;
                        if (s_z <= 0)
                        {
                            SharedSerial.Connected = false;
                            traceLogger.Enabled = false;
                            traceLogger.Dispose();
                            traceLogger = null;
                        }
                    }
                }
            }
            get { return SharedSerial.Connected; }
        }

        public static object Properties { get; private set; }

        #endregion

        #region Multi Driver handling
        // this section illustrates how multiple drivers could be handled,
        // it's for drivers where multiple connections to the hardware can be made and ensures that the
        // hardware is only disconnected from when all the connected devices have disconnected.

        // It is NOT a complete solution!  This is to give ideas of what can - or should be done.
        //
        // An alternative would be to move the hardware control here, handle connecting and disconnecting,
        // and provide the device with a suitable connection to the hardware.
        //
        /// <summary>
        /// dictionary carrying device connections.
        /// The Key is the connection number that identifies the device, it could be the COM port name,
        /// USB ID or IP Address, the Value is the DeviceHardware class
        /// </summary>
        private static Dictionary<string, DeviceHardware> connectedDevices = new Dictionary<string, DeviceHardware>();

        /// <summary>
        /// This is called in the driver Connect(true) property,
        /// it add the device id to the list of devices if it's not there and increments the device count.
        /// </summary>
        /// <param name="deviceId"></param>
        public static void Connect(string deviceId)
        {
            lock (lockObject)
            {
                if (!connectedDevices.ContainsKey(deviceId))
                    connectedDevices.Add(deviceId, new DeviceHardware());
                connectedDevices[deviceId].count++;       // increment the value
            }
        }

        public static void Disconnect(string deviceId)
        {
            lock (lockObject)
            {
                if (connectedDevices.ContainsKey(deviceId))
                {
                    connectedDevices[deviceId].count--;
                    if (connectedDevices[deviceId].count <= 0)
                        connectedDevices.Remove(deviceId);
                }
            }
        }

        public static bool IsConnected(string deviceId)
        {
            if (connectedDevices.ContainsKey(deviceId))
                return (connectedDevices[deviceId].count > 0);
            else
                return false;
        }

        #endregion


        /// <summary>
        /// Skeleton of a hardware class, all this does is hold a count of the connections,
        /// in reality extra code will be needed to handle the hardware in some way
        /// </summary>
        public class DeviceHardware
        {
            internal int count { set; get; }

            internal DeviceHardware()
            {
                count = 0;
            }
        }

        //public static string rawCommand(string function, string command)
        //{
        //   return rawCommand(function, command, false);
        //}

        public static string SendMessageString(string message)
        {
            try
            {
                string answer = SharedResources.SendMessage(message, true);
                return answer;
            }
            catch
            {
                return "";
            }
           
        }

        //Add new public methods here

        public static void SendMessageBlind(string message)
        {
            try
            {
                string answer = SharedResources.SendMessage(message, false);
            }
            catch
            { }
        }

    }

 }
