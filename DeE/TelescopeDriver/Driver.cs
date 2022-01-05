//tabs=4
// --------------------------------------------------------------------------------
//
// ASCOM Telescope driver for DeE
//
// Description:	Lorem ipsum dolor sit amet, consetetur sadipscing elitr, sed diam 
//				nonumy eirmod tempor invidunt ut labore et dolore magna aliquyam 
//				erat, sed diam voluptua. At vero eos et accusam et justo duo 
//				dolores et ea rebum. Stet clita kasd gubergren, no sea takimata 
//				sanctus est Lorem ipsum dolor sit amet.
//
// Implements:	ASCOM Telescope interface version: <To be completed by driver developer>
// Author:		(VTG) Your N. Here <vtgerritsen@gmail.com>
//
// Edit Log:
//
// Date			Who	Vers	Description
// -----------	---	-----	-------------------------------------------------------
// 2017-09-16	VTG	3.0 	Converted driver to local server format (See DeEMnt for prior version)
// 2017-09-23   VTG 3.01    Modified code to reduce UART traffic, Added SlewToTarget support
// 2017-09-29   VTG 3.02    FIxed RA and DEC sync display bug
// 2018-12-14   VTG 3.1     Updated comm protocol
// --------------------------------------------------------------------------------
//


// This is used to define code in the template that is specific to one class implementation
// unused code canbe deleted and this definition removed.
#define Telescope

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
    // Your driver's DeviceID is ASCOM.TelescopeDriver.Telescope
    //
    // The Guid attribute sets the CLSID for ASCOM.TelescopeDriver.Telescope
    // The ClassInterface/None addribute prevents an empty interface called
    // _TelescopeDriver from being created and used as the [default] interface
    //
    // TODO Replace the not implemented exceptions with code to implement the function or
    // throw the appropriate ASCOM exception.
    //

    /// <summary>
    /// ASCOM Telescope Driver for TelescopeDriver.
    /// </summary>
    [Guid("6061e06b-5f98-4e07-985e-7843a97b9eaa")]
    [ProgId("ASCOM.DeE.Telescope")]
    [ServedClassName("DeE Telescope")]
    [ClassInterface(ClassInterfaceType.None)]
    public class Telescope : ReferenceCountedObjectBase, ITelescopeV3
    {
        /// <summary>
        /// ASCOM DeviceID (COM ProgID) for this driver.
        /// The DeviceID is used by ASCOM applications to load the driver at runtime.
        /// </summary>
        internal static string driverID = "ASCOM.DeE.Telescope";
        // TODO Change the descriptive string for your driver then remove this line
        /// <summary>
        /// Driver description that displays in the ASCOM Chooser.
        /// </summary>
        private static string driverDescription = "ASCOM Telescope Driver for TelescopeDriver.";

        private static string driverShortName = "DeE Telescope";

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

        /// <summary>
        /// Variable to hold the trace logger object (creates a diagnostic log file with information that you specify)
        /// </summary>
        //internal static TraceLogger SharedResources.tl;

        /// <summary>
        /// Initializes a new instance of the <see cref="TelescopeDriver"/> class.
        /// Must be public for COM registration.
        /// </summary>
        /// 


        private double targetDEC;
        private double targetRA;


        public Telescope()
        {
            SharedResources.tl.LogMessage("Telescope", "Starting initialisation");
            driverID = Marshal.GenerateProgIdForType(this.GetType());
            connectedState = false; // Initialise connected to false
            utilities = new Util(); //Initialise util object
            astroUtilities = new AstroUtils(); // Initialise astro utilities object
            SharedResources.tl.LogMessage("Telescope", "Completed initialisation");
        }


        //
        // PUBLIC COM INTERFACE ITelescopeV3 IMPLEMENTATION
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
            // consider only showing the setup dialog if not connected
            // or call a different dialog if connected
            if (SharedResources.SharedSerial.Connected)
            {
                System.Windows.Forms.MessageBox.Show("Already connected, just press OK");
            }
            else
            {
                using (SetupDialog setupSerial = new SetupDialog())
                {
                    setupSerial.ShowDialog();
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
            SharedResources.SendMessageBlind(command);
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
                        //SharedResources.SharedResources.tl.LogMessage(driverShortName + "Connected Set", "Connecting to port " + DeE.Properties.Settings.Default.ComPort);
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
                string driverInfo = "Telescope Driver for DeE Version: " + String.Format(CultureInfo.InvariantCulture, "{0}.{1}", version.Major, version.Minor);
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
                LogMessage("InterfaceVersion Get", "3");
                return Convert.ToInt16("3");
            }
        }

        public string Name
        {
            get
            {
                string name = "DeE Telescope";
                SharedResources.tl.LogMessage("Name Get", name);
                return name;
            }
        }

        #endregion

        #region ITelescope Implementation
        public void AbortSlew()
        {
            //SharedResources.tl.LogMessage("AbortSlew", "Not implemented");
            //throw new ASCOM.MethodNotImplementedException("AbortSlew");
            CommandBlind("mr", false);
            CommandBlind("md", false);
            SharedResources.telescopeRASlewing = false;
            SharedResources.telescopeDECSlewing = false;
        }

        public AlignmentModes AlignmentMode
        {
            get
            {
                SharedResources.tl.LogMessage("AlignmentMode Get", "Not implemented");
                throw new ASCOM.PropertyNotImplementedException("AlignmentMode", false);
            }
        }

        public double Altitude      //This gets called by APT
        {
            get
            {
                SharedResources.tl.LogMessage("Altitude", "Not implemented");
                throw new ASCOM.PropertyNotImplementedException("Altitude", false);
            }
        }

        public double ApertureArea
        {
            get
            {
                SharedResources.tl.LogMessage("ApertureArea Get", "Not implemented");
                throw new ASCOM.PropertyNotImplementedException("ApertureArea", false);
            }
        }

        public double ApertureDiameter
        {
            get
            {
                SharedResources.tl.LogMessage("ApertureDiameter Get", "Not implemented");
                throw new ASCOM.PropertyNotImplementedException("ApertureDiameter", false);
            }
        }

        public bool AtHome
        {
            get
            {
                if (!SharedResources.telescopeParked) return false;
                return CommandBool("H", false);
            }
        }

        public bool AtPark
        {
            get
            {
                SharedResources.tl.LogMessage("DeE Telescope", "AtPark " + SharedResources.telescopeParked);
                return SharedResources.telescopeParked;
            }
        }

        public IAxisRates AxisRates(TelescopeAxes Axis)
        {
            SharedResources.tl.LogMessage("AxisRates", "Get - " + Axis.ToString());
            //return new AxisRates(Axis);
            throw new ASCOM.PropertyNotImplementedException("AxisRates", false);
        }

        public double Azimuth       //CAlled by APT
        {
            get
            {
                SharedResources.tl.LogMessage("Azimuth Get", "Not implemented");
                throw new ASCOM.PropertyNotImplementedException("Azimuth", false);
            }
        }

        public bool CanFindHome
        {
            get
            {
                return true;
            }
        }

        public bool CanMoveAxis(TelescopeAxes Axis)
        {
            SharedResources.tl.LogMessage("CanMoveAxis", "Get - " + Axis.ToString());
            switch (Axis)
            {
                case TelescopeAxes.axisPrimary: return true;
                case TelescopeAxes.axisSecondary: return true;
                case TelescopeAxes.axisTertiary: return false;
                default: throw new InvalidValueException("CanMoveAxis", Axis.ToString(), "0 to 2");
            }
        }

        public bool CanPark
        {
            get
            {
                return true;
            }
        }

        public bool CanPulseGuide
        {
            get
            {
                return true;
            }
        }

        public bool CanSetDeclinationRate
        {
            get
            {
                SharedResources.tl.LogMessage("CanSetDeclinationRate", "Get - " + false.ToString());
                return false;
            }
        }

        public bool CanSetGuideRates
        {
            get
            {
                SharedResources.tl.LogMessage("CanSetGuideRates", "Get - " + false.ToString());
                return false;
            }
        }

        public bool CanSetPark
        {
            get
            {
                return true;
            }
        }

        public bool CanSetPierSide
        {
            get
            {
                SharedResources.tl.LogMessage("CanSetPierSide", "Get - " + false.ToString());
                return false;
            }
        }

        public bool CanSetRightAscensionRate
        {
            get
            {
                SharedResources.tl.LogMessage("CanSetRightAscensionRate", "Get - " + false.ToString());
                return false;
            }
        }

        public bool CanSetTracking
        {
            get
            {
                return true;
            }
        }

        public bool CanSlew
        {
            get
            {
                SharedResources.tl.LogMessage("CanSlew", "Get - " + true.ToString());
                return true;
            }
        }

        public bool CanSlewAltAz
        {
            get
            {
                SharedResources.tl.LogMessage("CanSlewAltAz", "Get - " + false.ToString());
                return false;
            }
        }

        public bool CanSlewAltAzAsync
        {
            get
            {
                SharedResources.tl.LogMessage("CanSlewAltAzAsync", "Get - " + false.ToString());
                return false;
            }
        }

        public bool CanSlewAsync
        {
            get
            {
                SharedResources.tl.LogMessage("CanSlewAsync", "Get - " + true.ToString());
                return true;
            }
        }

        public bool CanSync
        {
            get
            {
                return true;
            }
        }

        public bool CanSyncAltAz
        {
            get
            {
                SharedResources.tl.LogMessage("CanSyncAltAz", "Get - " + false.ToString());
                return false;
            }
        }

        public bool CanUnpark
        {
            get
            {
                return true;
            }
        }

        public double Declination
        {
            get
            {
                SharedResources.tl.LogMessage("DeE Telescope", "Get Declination");
                if (SharedResources.telescopeDECcorrect) return SharedResources.telescopeDECPosition;
                double declination = SharedResources.SendMessageValue("D", 4);
                declination /= 3600;
                declination -= 90;
                SharedResources.telescopeDECPosition = declination;
                if (!SharedResources.telescopeDECSlewing) SharedResources.telescopeDECcorrect = true;
                return declination;
            }
        }

        public double DeclinationRate
        {
            get
            {
                double declination = 0.0;
                SharedResources.tl.LogMessage("DeclinationRate", "Get - " + declination.ToString());
                return declination;
            }
            set
            {
                SharedResources.tl.LogMessage("DeclinationRate Set", "Not implemented");
                throw new ASCOM.PropertyNotImplementedException("DeclinationRate", true);
            }
        }

        public PierSide DestinationSideOfPier(double RightAscension, double Declination)
        {
            SharedResources.tl.LogMessage("DestinationSideOfPier Get", "Not implemented");
            throw new ASCOM.PropertyNotImplementedException("DestinationSideOfPier", false);
        }

        public bool DoesRefraction
        {
            get
            {
                SharedResources.tl.LogMessage("DoesRefraction Get", "Not implemented");
                throw new ASCOM.PropertyNotImplementedException("DoesRefraction", false);
            }
            set
            {
                SharedResources.tl.LogMessage("DoesRefraction Set", "Not implemented");
                throw new ASCOM.PropertyNotImplementedException("DoesRefraction", true);
            }
        }

        public EquatorialCoordinateType EquatorialSystem
        {
            get
            {
                EquatorialCoordinateType equatorialSystem = EquatorialCoordinateType.equLocalTopocentric;
                SharedResources.tl.LogMessage("DeclinationRate", "Get - " + equatorialSystem.ToString());
                return equatorialSystem;
            }
        }

        public void FindHome()
        {
            SharedResources.tl.LogMessage("FindHome", "Not implemented");
            throw new ASCOM.MethodNotImplementedException("FindHome");
        }

        public double FocalLength
        {
            get
            {
                SharedResources.tl.LogMessage("FocalLength Get", "Not implemented");
                throw new ASCOM.PropertyNotImplementedException("FocalLength", false);
            }
        }

        public double GuideRateDeclination
        {
            get
            {
                string response = CommandString("pgdq", false);
                double rate;
                rate = Int32.Parse(response);
                rate *= 3600;
                rate /= 1000;
                rate = 1 / rate;
                return rate;
            }
            set
            {
                value = 1 / value;
                value = 1000 * value;
                value /= 3600;
                int valueInt = (int)value;
                string command = "pgds";
                command += valueInt.ToString();
                CommandBlind(command, false);
            }
        }

        public double GuideRateRightAscension
        {
            get
            {
                string response = CommandString("pgrq", false);
                double rate;
                rate = Int32.Parse(response);
                rate *= 0.4178075;
                return rate;
            }
            set
            {
                value = value / 0.4178075;
                int valueInt = (int)value;
                string command = "pgrs";
                command += valueInt.ToString();
                CommandBlind(command, false);
            }
        }

        public bool IsPulseGuiding
        {
            get
            {
                Boolean value;
                if (!SharedResources.telescopeWasPulseGuiding) value = false;
                else
                {
                    value = CommandBool("P", false);
                    SharedResources.telescopeWasPulseGuiding = value;
                }

                SharedResources.tl.LogMessage("DeE Telescope", "GetPulseGuiding " + value);
                return value;
            }
        }

        public void MoveAxis(TelescopeAxes Axis, double Rate)
        {
            string command = "m";
            if (Axis == 0)
            {
                SharedResources.telescopeRASlewing = true;
                SharedResources.telescopeRAcorrect = false;
                if (Rate > 0) command += "e";
                else if (Rate < 0) command += "w";
                else
                {
                    command += "r";
                    SharedResources.telescopeRASlewing = false;
                }

            }
            else
            {
                SharedResources.telescopeDECSlewing = true;
                SharedResources.telescopeDECcorrect = false;
                if (Rate > 0) command += "n";
                else if (Rate < 0) command += "s";
                else
                {
                    SharedResources.telescopeDECSlewing = false;
                    command += "d";
                }
            }
            CommandBlind(command, false);
        }

        public void Park()
        {
            SharedResources.tl.LogMessage("DeE Telescope", "Set Parked");
            SharedResources.telescopeParked = true;
            SharedResources.telescopeRAcorrect = false;
            CommandBlind("t0", false);
        }

        public void PulseGuide(GuideDirections Direction, int Duration)
        {

            SharedResources.telescopeWasPulseGuiding = true;
            SharedResources.tl.LogMessage("DeE Telescope", "PulseGuide " + Direction + Duration);
            string command = "p";

            if (Direction == GuideDirections.guideEast) command += "e";
            else if (Direction == GuideDirections.guideWest) command += "w";
            else if (Direction == GuideDirections.guideNorth) command += "n";
            else if (Direction == GuideDirections.guideSouth) command += "s";
            command += Duration;
            CommandBlind(command, false);

        }

        public double RightAscension
        {
            get
            {
                if (SharedResources.telescopeRAcorrect) return SharedResources.telescopeRAPosition;
                double rightAscension = SharedResources.SendMessageValue("R", 3);
                rightAscension /= 3600;
                SharedResources.telescopeRAPosition = rightAscension;
                if (!SharedResources.telescopeRASlewing && !SharedResources.telescopeParked) SharedResources.telescopeRAcorrect = true;
                return rightAscension;
            }
        }

        public double RightAscensionRate
        {
            get
            {
                double rightAscensionRate = 0.0;
                SharedResources.tl.LogMessage("RightAscensionRate", "Get - " + rightAscensionRate.ToString());
                return rightAscensionRate;
            }
            set
            {
                SharedResources.tl.LogMessage("RightAscensionRate Set", "Not implemented");
                throw new ASCOM.PropertyNotImplementedException("RightAscensionRate", true);
            }
        }

        public void SetPark()
        {
            CommandBlind("t0", false);
            SharedResources.telescopeParked = true;
            SharedResources.telescopeRAcorrect = false;
            SharedResources.tl.LogMessage("DeE Telescope", "Set Parked");
        }

        public PierSide SideOfPier
        {
            get
            {
                SharedResources.tl.LogMessage("SideOfPier Get", "Not implemented");
                throw new ASCOM.PropertyNotImplementedException("SideOfPier", false);
            }
            set
            {
                SharedResources.tl.LogMessage("SideOfPier Set", "Not implemented");
                throw new ASCOM.PropertyNotImplementedException("SideOfPier", true);
            }
        }

        public double SiderealTime
        {
            get
            {
                // get greenwich sidereal time: https://en.wikipedia.org/wiki/Sidereal_time
                //double siderealTime = (18.697374558 + 24.065709824419081 * (utilities.DateUTCToJulian(DateTime.UtcNow) - 2451545.0));

                // alternative using NOVAS 3.1
                double siderealTime = 0.0;
                using (var novas = new ASCOM.Astrometry.NOVAS.NOVAS31())
                {
                    var jd = utilities.DateUTCToJulian(DateTime.UtcNow);
                    novas.SiderealTime(jd, 0, novas.DeltaT(jd),
                        ASCOM.Astrometry.GstType.GreenwichApparentSiderealTime,
                        ASCOM.Astrometry.Method.EquinoxBased,
                        ASCOM.Astrometry.Accuracy.Reduced, ref siderealTime);
                }
                // allow for the longitude
                siderealTime += SiteLongitude / 360.0 * 24.0;
                // reduce to the range 0 to 24 hours
                siderealTime = siderealTime % 24.0;
                SharedResources.tl.LogMessage("SiderealTime", "Get - " + siderealTime.ToString());
                return siderealTime;
            }
        }

        public double SiteElevation
        {
            get
            {
                SharedResources.tl.LogMessage("SiteElevation Get", "Not implemented");
                throw new ASCOM.PropertyNotImplementedException("SiteElevation", false);
            }
            set
            {
                SharedResources.tl.LogMessage("SiteElevation Set", "Not implemented");
                throw new ASCOM.PropertyNotImplementedException("SiteElevation", true);
            }
        }

        public double SiteLatitude
        {
            get
            {
                SharedResources.tl.LogMessage("SiteLatitude Get", "Not implemented");
                throw new ASCOM.PropertyNotImplementedException("SiteLatitude", false);
            }
            set
            {
                SharedResources.tl.LogMessage("SiteLatitude Set", "Not implemented");
                throw new ASCOM.PropertyNotImplementedException("SiteLatitude", true);
            }
        }

        public double SiteLongitude         //Called by APT
        {
            get
            {
                SharedResources.tl.LogMessage("SiteLongitude Get", "Not implemented");
                throw new ASCOM.PropertyNotImplementedException("SiteLongitude", false);
            }
            set
            {
                SharedResources.tl.LogMessage("SiteLongitude Set", "Not implemented");
                throw new ASCOM.PropertyNotImplementedException("SiteLongitude", true);
            }
        }

        public short SlewSettleTime
        {
            get
            {
                SharedResources.tl.LogMessage("SlewSetSharedResources.tleTime Get", "Not implemented");
                throw new ASCOM.PropertyNotImplementedException("SlewSetSharedResources.tleTime", false);
            }
            set
            {
                SharedResources.tl.LogMessage("SlewSetSharedResources.tleTime Set", "Not implemented");
                throw new ASCOM.PropertyNotImplementedException("SlewSetSharedResources.tleTime", true);
            }
        }

        public void SlewToAltAz(double Azimuth, double Altitude)
        {
            SharedResources.tl.LogMessage("SlewToAltAz", "Not implemented");
            throw new ASCOM.MethodNotImplementedException("SlewToAltAz");
        }

        public void SlewToAltAzAsync(double Azimuth, double Altitude)
        {
            SharedResources.tl.LogMessage("SlewToAltAzAsync", "Not implemented");
            throw new ASCOM.MethodNotImplementedException("SlewToAltAzAsync");
        }

        public void SlewToCoordinates(double RightAscension, double Declination)
        {
            SharedResources.tl.LogMessage("SlewToCoordinates", "Not implemented");
            throw new ASCOM.MethodNotImplementedException("SlewToCoordinates");
        }

        public void SlewToCoordinatesAsync(double RightAscension, double Declination)
        {
            SharedResources.tl.LogMessage("DeE Telescope", "SlewToCoordinatesAsync " + RightAscension + " " + Declination);
            Declination *= 3600;
            Declination += 324000;
            RightAscension *= 3600;
            int temp = (int)Declination;
            string dec = "td" + temp.ToString();
            temp = (int)RightAscension;
            string ra = "tr" + temp.ToString();
            CommandBlind(dec, false);
            CommandBlind(ra, false);
            SharedResources.telescopeDECSlewing = true;
            SharedResources.telescopeRASlewing = true;
            SharedResources.telescopeRAcorrect = false;
            SharedResources.telescopeDECcorrect = false;
        }

        public void SlewToTarget()
        {
            SharedResources.tl.LogMessage("SlewToTarget", "Not implemented");
            throw new ASCOM.MethodNotImplementedException("SlewToTarget");
        }

        public void SlewToTargetAsync()
        {
            SlewToCoordinates(targetRA, targetDEC);
        }

        public bool Slewing
        {
            get
            {
                SharedResources.tl.LogMessage("DeE Telescope", "IsSLewing Get");
                if (!SharedResources.telescopeDECSlewing && !SharedResources.telescopeRASlewing)
                {
                    return false;
                }
                else
                {
                    Boolean value = CommandBool("sq", false);
                    if (!value)
                    {
                        SharedResources.telescopeDECSlewing = false;
                        SharedResources.telescopeRASlewing = false;
                    }
                    return value;
                }
            }
        }

        public void SyncToAltAz(double Azimuth, double Altitude)
        {
            SharedResources.tl.LogMessage("SyncToAltAz", "Not implemented");
            throw new ASCOM.MethodNotImplementedException("SyncToAltAz");
        }

        public void SyncToCoordinates(double RightAscension, double Declination)
        {

            SharedResources.tl.LogMessage("DeE Telescope", "SyncToCoordinates " + RightAscension + " " + Declination);
            SharedResources.telescopeRAPosition = RightAscension;
            SharedResources.telescopeDECPosition = Declination;
            Declination *= 3600;
            Declination += 324000;
            RightAscension *= 3600;
            int temp = (int)Declination;
            string dec = "cd" + temp.ToString();
            temp = (int)RightAscension;
            string ra = "cr" + temp.ToString();
            CommandBlind(dec, false);
            CommandBlind(ra, false);
        }

        public void SyncToTarget()
        {
            SharedResources.tl.LogMessage("SyncToTarget", "Not implemented");
            throw new ASCOM.MethodNotImplementedException("SyncToTarget");
        }

        public double TargetDeclination
        {
            get
            {
                return targetDEC;
            }
            set
            {
                targetDEC = value;
                SharedResources.tl.LogMessage("DeE Telescope", "Set TargetDEC " + value);
            }
        }

        public double TargetRightAscension
        {
            get
            {
                return targetRA;
            }
            set
            {
                targetRA = value;
                SharedResources.tl.LogMessage("DeE Telescope", "Set TargetRA " + value);
            }
        }

        public bool Tracking
        {
            get
            {

                SharedResources.tl.LogMessage("DeE Telescope", "Get Tracking " + SharedResources.telescopeParked);
                return SharedResources.telescopeParked;
            }
            set
            {

                SharedResources.tl.LogMessage("DeE Telescope", "Set Tracking " + value);
                if (value == SharedResources.telescopeParked)
                {
                    if (value == true) CommandBlind("t1", false);
                    else
                    {
                        CommandBlind("t0", false);
                        SharedResources.telescopeRAcorrect = false;
                    }
                    SharedResources.telescopeParked = !value;
                }
            }
        }

        public DriveRates TrackingRate
        {
            get
            {
                SharedResources.tl.LogMessage("TrackingRate Get", "Not implemented");
                throw new ASCOM.PropertyNotImplementedException("TrackingRate", false);
            }
            set
            {
                SharedResources.tl.LogMessage("TrackingRate Set", "Not implemented");
                throw new ASCOM.PropertyNotImplementedException("TrackingRate", true);
            }
        }

        public ITrackingRates TrackingRates
        {
            get
            {
                throw new ASCOM.PropertyNotImplementedException("Tracking Rates", false);
                //ITrackingRates trackingRates = new TrackingRates();
                //SharedResources.tl.LogMessage("TrackingRates", "Get - ");
                //foreach (DriveRates driveRate in trackingRates)
                //{
                //    SharedResources.tl.LogMessage("TrackingRates", "Get - " + driveRate.ToString());
                //}
                //return trackingRates;
            }
        }

        public DateTime UTCDate
        {
            get
            {
                DateTime utcDate = DateTime.UtcNow;
                SharedResources.tl.LogMessage("TrackingRates", "Get - " + String.Format("MM/dd/yy HH:mm:ss", utcDate));
                return utcDate;
            }
            set
            {
                SharedResources.tl.LogMessage("UTCDate Set", "Not implemented");
                throw new ASCOM.PropertyNotImplementedException("UTCDate", true);
            }
        }

        public void Unpark()
        {
            SharedResources.tl.LogMessage("DeE Telescope", "SetUnParked");
            SharedResources.telescopeParked = false;
            CommandBlind("t1", false);
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
