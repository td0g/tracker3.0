//tabs=4
// --------------------------------------------------------------------------------
//
// ASCOM Focuser driver for DeE
//
// Description:	Lorem ipsum dolor sit amet, consetetur sadipscing elitr, sed diam 
//				nonumy eirmod tempor invidunt ut labore et dolore magna aliquyam 
//				erat, sed diam voluptua. At vero eos et accusam et justo duo 
//				dolores et ea rebum. Stet clita kasd gubergren, no sea takimata 
//				sanctus est Lorem ipsum dolor sit amet.
//
// Implements:	ASCOM Focuser interface version: <To be completed by driver developer>
// Author:		(VTG) Your N. Here <vtgerritsen@gmail.com>
//
// Edit Log:
//
// Date			Who	Vers	Description
// -----------	---	-----	-------------------------------------------------------
// 2017-09-16	VTG	2.0 	Converted driver to local server format (See Archangel Focuser for prior version)
// --------------------------------------------------------------------------------
//


// This is used to define code in the template that is specific to one class implementation
// unused code canbe deleted and this definition removed.
#define Focuser

using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Text;
using System.Runtime.InteropServices;

using ASCOM;
using ASCOM.Astrometry;
using ASCOM.Astrometry.AstroUtils;
using ASCOM.Utilities;
using ASCOM.DeviceInterface;
using System.Globalization;
using System.Collections;

namespace ASCOM.DeE
{
    //
    // Your driver's DeviceID is ASCOM.DeE.Focuser
    //
    // The Guid attribute sets the CLSID for ASCOM.DeE.Focuser
    // The ClassInterface/None addribute prevents an empty interface called
    // _DeE from being created and used as the [default] interface
    //
    //

    /// <summary>
    /// ASCOM Focuser Driver for DeE.
    /// </summary>
    [Guid("c65a3f21-85ad-418c-93fd-a06482d889a2")]
    [ProgId("ASCOM.DeE.Focuser")]
    [ServedClassName("DeE Focuser")]
    [ClassInterface(ClassInterfaceType.None)]
    public class Focuser : IFocuserV2
    {
        /// <summary>
        /// ASCOM DeviceID (COM ProgID) for this driver.
        /// The DeviceID is used by ASCOM applications to load the driver at runtime.
        /// </summary>
        internal static string driverID = "ASCOM.DeE.Focuser";

        /// <summary>
        /// Driver description that displays in the ASCOM Chooser.
        /// </summary>
        private static string driverDescription = "ASCOM Focuser Driver for DeE.";

        private static string driverShortName = "DeE Focuser";

        /// <summary>
        /// Private variable to hold the connected state
        /// </summary>
        private bool connectedState;

        /// <summary>
        /// Private variable to hold an ASCOM Utilities object
        /// </summary>
        private Util utilities;

        /// <summary>
        /// Private variable to hold an ASCOM AstroUtilities object to provide the Range method
        /// </summary>
        private AstroUtils astroUtilities;

        public Focuser()
        {
            SharedResources.tl.LogMessage("Focuser", "Starting initialisation");
            driverID = Marshal.GenerateProgIdForType(this.GetType());
            connectedState = false; // Initialise connected to false
            utilities = new Util(); //Initialise util object
            astroUtilities = new AstroUtils(); // Initialise astro utilities object
            SharedResources.tl.LogMessage("Focuser", "Completed initialisation");
        }


        //
        // PUBLIC COM INTERFACE IFocuserV2 IMPLEMENTATION
        //

        #region Common properties and methods.

        /// <summary>
        /// Displays the Setup Dialog form.
        /// If the user clicks the OK button to dismiss the form, then
        /// the new settings are saved, otherwise the old values are reloaded.
        /// THIS IS THE ONLY PLACE WHERE SHOWING USER INTERFACE IS ALLOWED!
        /// </summary>
        public void SetupDialog()
        {
            if (SharedResources.SharedSerial.Connected)
                {
                    System.Windows.Forms.MessageBox.Show("Already connected, just press OK");
                }
                else
            {
                using (SetupDialog setupSerial = new SetupDialog())         //THIS IS ITT!!!!!  Problems happen here
                {
                    System.Windows.Forms.MessageBox.Show("1a");
                    setupSerial.ShowDialog();

                    System.Windows.Forms.MessageBox.Show("2a");
                }
                }
            }

        public ArrayList SupportedActions
        {
            get
            {
                SharedResources.tl.LogMessage("SupportedActions Get", "Returning empty arraylist");
                return new ArrayList();
            }
        }

        public string Action(string actionName, string actionParameters)
        {
            LogMessage("", "Action {0}, parameters {1} not implemented", actionName, actionParameters);
            throw new ASCOM.ActionNotImplementedException("Action " + actionName + " is not implemented by this driver");
        }

        public void CommandBlind(string command, bool raw)
        {
            CheckConnected("CommandString");
            SharedResources.SendMessage(command, false);
        }

        public bool CommandBool(string command, bool raw)
        {

            CheckConnected("CommandString");
            if (SharedResources.SendMessageString(command) == "y") return true;
            else return false;
        }

        public string CommandString(string command, bool raw)
        {
            CheckConnected("CommandString");
            return SharedResources.SendMessageString(command);
        }

        public void Dispose()
        {
            utilities.Dispose();
            utilities = null;
            astroUtilities.Dispose();
            astroUtilities = null;
        }

        public bool Connected
        {
            get { return IsConnected; }
            set
            {
                {
                    SharedResources.tl.LogMessage(driverShortName + " Connected Set", value.ToString());
                    if (value == IsConnected)
                        return;

                    if (value)
                    {
                        if (IsConnected) return;
                        SharedResources.Connected = true;
                        connectedState = SharedResources.Connected;
                    }
                    else
                    {
                        connectedState = false;
                        SharedResources.Connected = false;
                        SharedResources.tl.LogMessage(driverShortName + " Switch Connected Set", "Disconnected, " + SharedResources.connections + " connections left");
                    }
                }
            }
        }

        public string Description
        {
            // TODO customise this device description
            get
            {
                SharedResources.tl.LogMessage("Description Get", driverDescription);
                return driverDescription;
            }
        }

        public string DriverInfo
        {
            get
            {
                Version version = System.Reflection.Assembly.GetExecutingAssembly().GetName().Version;
                string driverInfo = "DeE Focuser Driver Version: 2.0" + String.Format(CultureInfo.InvariantCulture, "{0}.{1}", version.Major, version.Minor);
                SharedResources.tl.LogMessage("DriverInfo Get", driverInfo);
                return driverInfo;
            }
        }

        public string DriverVersion
        {
            get
            {
                Version version = System.Reflection.Assembly.GetExecutingAssembly().GetName().Version;
                string driverVersion = String.Format(CultureInfo.InvariantCulture, "{0}.{1}", version.Major, version.Minor);
                SharedResources.tl.LogMessage("DriverVersion Get", driverVersion);
                return driverVersion;
            }
        }

        public short InterfaceVersion
        {
            // set by the driver wizard
            get
            {
                LogMessage("InterfaceVersion Get", "2");
                return Convert.ToInt16("2");
            }
        }

        public string Name
        {
            get
            {
                string name = "DeE Focuser";
                SharedResources.tl.LogMessage("Name Get", name);
                return name;
            }
        }

        #endregion

        #region IFocuser Implementation

        public bool Absolute
        {
            get
            {
                SharedResources.tl.LogMessage("Absolute Get", true.ToString());
                return true; // This is an absolute focuser
            }
        }

        public void Halt()
        {
            string response = CommandString("fh", true);
            int startPos = response.IndexOf("fh");
            response = response.Substring(2 + startPos, response.Length - startPos - 2);
            SharedResources.focuserPosition = Int32.Parse(response);
            SharedResources.tl.LogMessage("Halt", "Postition: " + SharedResources.focuserPosition);
        }

        public bool IsMoving
        {
            get
            {
                if (!SharedResources.focuserWasMoving)
                {
                    SharedResources.tl.LogMessage("IsMoving Get", "False");
                    return false;
                }
                else {
                    Boolean moving = CommandBool("fq", true);
                    SharedResources.tl.LogMessage("IsMoving Get", moving.ToString());
                    SharedResources.focuserWasMoving = moving;
                    return moving; // This focuser always moves instantaneously so no need for IsMoving ever to be True
                }
             }
        }

        public bool Link
        {
            get
            {
                SharedResources.tl.LogMessage("Link Get", this.Connected.ToString());
                return this.Connected; // Direct function to the connected method, the Link method is just here for backwards compatibility
            }
            set
            {
                SharedResources.tl.LogMessage("Link Set", value.ToString());
                this.Connected = value; // Direct function to the connected method, the Link method is just here for backwards compatibility
            }
        }

        public int MaxIncrement
        {
            get
            {
                SharedResources.tl.LogMessage("MaxIncrement Get", SharedResources.focuserSteps.ToString());
                return SharedResources.focuserSteps; // Maximum change in one move
            }
        }

        public int MaxStep
        {
            get
            {
                SharedResources.tl.LogMessage("MaxStep Get", SharedResources.focuserSteps.ToString());
                return SharedResources.focuserSteps; // Maximum extent of the focuser, so position range is 0 to 10,000
            }
        }

        public void Move(int Position)
        {
            if (Position >= 0 && Position <= MaxStep)
            {
                SharedResources.focuserWasMoving = true;
                CommandBlind("f" + Position, true);
                SharedResources.tl.LogMessage("Move", Position.ToString());
                SharedResources.focuserPosition = Position;
            }
            else
            {
                SharedResources.tl.LogMessage("Move", "INVALID: " + Position.ToString());
            }
        }

        public int Position
        {
            get
            {
                return SharedResources.focuserPosition; // Return the focuser position
            }
        }

        public double StepSize
        {
            get
            {
                SharedResources.tl.LogMessage("StepSize Get", "Not implemented");
                throw new ASCOM.PropertyNotImplementedException("StepSize", false);
            }
        }

        public bool TempComp
        {
            get
            {
                SharedResources.tl.LogMessage("TempComp Get", false.ToString());
                return false;
            }
            set
            {
                SharedResources.tl.LogMessage("TempComp Set", "Not implemented");
                throw new ASCOM.PropertyNotImplementedException("TempComp", false);
            }
        }

        public bool TempCompAvailable
        {
            get
            {
                SharedResources.tl.LogMessage("TempCompAvailable Get", false.ToString());
                return false; // Temperature compensation is not available in this driver
            }
        }

        public double Temperature
        {
            get
            {
                SharedResources.tl.LogMessage("Temperature Get", "Not implemented");
                throw new ASCOM.PropertyNotImplementedException("Temperature", false);
            }
        }

        #endregion

        #region Private properties and methods
        // here are some useful properties and methods that can be used as required
        // to help with driver development

        /// <summary>
        /// Returns true if there is a valid connection to the driver hardware
        /// </summary>
        private bool IsConnected
        {
            get
            {
                // TODO check that the driver hardware connection exists and is connected to the hardware
                return connectedState;
            }
        }

        /// <summary>
        /// Use this function to throw an exception if we aren't connected to the hardware
        /// </summary>
        /// <param name="message"></param>
        private void CheckConnected(string message)
        {
            if (!IsConnected)
            {
                throw new ASCOM.NotConnectedException(message);
            }
        }

        /// <summary>
        /// Log helper function that takes formatted strings and arguments
        /// </summary>
        /// <param name="identifier"></param>
        /// <param name="message"></param>
        /// <param name="args"></param>
        internal static void LogMessage(string identifier, string message, params object[] args)
        {
            var msg = string.Format(message, args);
            SharedResources.tl.LogMessage(identifier, msg);
        }
        #endregion
    }
}
