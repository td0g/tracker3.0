//By Tyler Gerritsen
//Last Revision 2017-10-01

/*
    3.03  2017-10-01  Added byte array data transfer, more efficient than string
 */

//1.3158% Slow (2017-07-07)


//Pins
#define PIN_RA_SPEED 3
#define PIN_RA_STEP 4
#define PIN_RA_DIR 5
#define PIN_DEC_STEP 6
#define PIN_DEC_DIR 7

#define hardwareSerialSpeed 57600
#define softwareSerialSpeed 9600

#define DEC_STEPS_PER_ARCSEC 10
#define DEC_MOTOR_MAX_SPEED 120
#define DEC_PULSE_GUIDE_ARCSEC_PER_SEC 1
#define RA_PULSE_GUIDE_ARCSEC_PER_SEC 300000     //Was 50000
#define STEP_PIN_HIGH_TIME 2
#define RA_MOTOR_MAX_SPEED 500
#define FOCUS_MOTOR_SPEED 800;
#define RA_ENDSTOP_IGNORE_DISTANCE 1000

//Enter calculated data
//Speed Table
PROGMEM const unsigned long delayChangeTable[] = {
5099,10671,10919,11178,11449,11732,12030,12340,12668,13011,13374,13755,14159,14585,15038,15519,16031,16577,17162,17789,18465,19193,19984,20843,21784,22817,23958,25229,26652,28263,30103,32231,34730,37719,41379,46003,52099,60654,73952,99193,203527,0,1,0,1,0,134971,0,1,0,1,228261,123785,98404,84967,76274,70043,65285,61493,58373,55745,53489,51524,49788,48241,46850,45587,44435,43378,42401,41496,40653,39865,39126,38432,37775,37157,36568,36010,35479,34972,34488,34024,33580,33153,32744,32350,31969,31603,31249,30908,30576,30256,29946,29645,29352,29068,28793,28524,28263,28009,27761,27519,27283,27053,26828,26609,26395,26184,25979,25779,25582,25389,25200,25016,24834,24657,24482,24310,24143,23978,23815,23657,23499,23346,23195,23046,22900,22756,22614,22474,22337,22202,22069,21938,21808,21681,21556,21431,21310,21189,21071,20953,20838,20724,20611,20500,20391,20282,20176,20070,19965,19863,19761,19660,19561,19462,19366,19270,19175,19081,18988,18896,18806,18716,18628,18539,18453,18367,18281,18198,18114,18031,17950,17869,17789,17710,17631,17554,17477,17400,17325,17250,17176,17103,17030,16957,16887,16815,16746,16676,16607,16538,16471,16404,16337,16272,16206,16141,16076,16013,15950,15887,15825,15763,15701,15641,15581,15521,15462,15403,15344,15286,15229,15172,15116,15059,15003,14948,14894,14838,14785,14730,14678,14624,14572,14520,14468,14416,14365,14315,14264,14214,14165,14115,14066,14018,13969,13921,13874,13826,13779,13732,13686,13640,13594,13548,13503,13458,13414,13369,13325,13281,13238,13195,13151,13109,13066,13024,12983,12940,12899,12858,12817,12776,12736,12696,12655,12616,12577,12537,12498,12460,12421,12382,12344,12307,12269,12231,12194,12157,12120,12083,12047,12011,11975,11939,11903,11868,11833,11797,11763,11728,11693
};

const unsigned long raStepLimit = 4018969;               //Total steps to end of rod
int delayChange = 1;                                   //Change in delay time per delay change
unsigned long initMotorStepDelay = 1745;                      //Starting motor step delay
unsigned long currReadStepDelay = initMotorStepDelay;
unsigned long currMotorStepDelay;


/*
  Sidereal day = 86164.09164 s
  Sidereal rate = 239.344699 s / deg
              = 0.004178075 deg / s
              = 15.04106845 arcsec / s
*/

//Both Axis
unsigned int slewSettleTime = 2000;           //Slew settle time (ms)

//RA Motor
const byte raDirPin = PIN_RA_DIR;
const byte raStepPin = PIN_RA_STEP;
unsigned long raStepPosition;                 //Motor step position         Start position = 0    Home position = 2
unsigned long raActualTimeTotal;              //Copies from micros()
unsigned int raSlewRate = RA_MOTOR_MAX_SPEED;                //RA slewing speed (us / step)
const unsigned int raSlewRateMax = 200;       //Max RA slew speed
unsigned int raPulseRate = (long)RA_PULSE_GUIDE_ARCSEC_PER_SEC / 100;               //RA pulse guide rate (500 = 5.00% sidereal rate)
const long raMicrosPerArcsec = 1000000;         //Convert arcsecs to steps           66485 for 360 deg, 1000000 for 24hr
const unsigned long raTotalArcsecs = 86400;
const byte raSpeedPin = PIN_RA_SPEED;
const byte raSlewFactor = 4;
unsigned long raPGtimer;
char raPGdir;
byte raMotorOn = 0;
byte raSlewing = 0;

/*
          //dec Motor
          //Arm length = 275mm
          //0.8mm / rotation
          //16 / 75 reduction
          //3200 steps per rotation
          //    = 18750 steps / mm
          //    = 0.2083487 deg/mm
          //    = 89,993.34 steps / deg
          //    = 25 steps / arcsec

*/

const unsigned long decStepsPerArcsec = DEC_STEPS_PER_ARCSEC;
const byte decDirPin = PIN_DEC_DIR;
const byte decStepPin = PIN_DEC_STEP;
unsigned int decSlewRate = DEC_MOTOR_MAX_SPEED;            //DEC slewing speed
unsigned int decPGrate = raMicrosPerArcsec * DEC_PULSE_GUIDE_ARCSEC_PER_SEC / DEC_STEPS_PER_ARCSEC;  //was 10,000  us / step...        us / step = s / arcsec / step / arcsec * 1000000 us / s =     
unsigned long decStepPosition = 20000000;                     //DEC motor position
unsigned long decTargetPosition = decStepPosition;                   //DEC motor target position
char decMoveDir;                        //Direction of movement (1 = north, -1 = south)
unsigned long decPGtimer;
char decPGdir = 0;
byte decReverse = 0;

const byte motorDriverStepDelay = STEP_PIN_HIGH_TIME;

//Coordinate System
long raPosArcsecs = 43200;         //180 degrees
long raTargetArcsecs = raPosArcsecs;
unsigned long decCoorOffset = 324000;

//Endstops
const byte raEndStopPin = 8;
#define RA_ENDSTOP (PINB & 1)
const byte decEndStopPin = 12;  //Originally 13, burnt pin out so moved to 12

//Controller
const byte controllerPin = 14;

//Focuser
const byte focusStepPin = 10;
const byte focusDirPin = 11;
const byte focusSleepPin = 9;
unsigned int focusMotorDelay = FOCUS_MOTOR_SPEED;
const unsigned int focusMotorDelayStart = 1000;
const unsigned int focusMotorDelayChange = 5;

const unsigned int focusSleepDelay = 600;

long focusCurrPos;
long focusTargetPos;

//Serial Commander

String COMstring;
unsigned long COMvalue;
const char terminator = '/';


//Misc
byte displayInfo = 51;

const byte indPin = 2;
unsigned int indDelay = 1000;

const byte voltagePin = 15;

String conMsg = "Tracker V3 Ready/";


//WiFi Control
#include <SoftwareSerial.h>
const byte softSerPinTX = 16;
const byte softSerPinRX = 17;
SoftwareSerial SerialSoft(softSerPinRX, softSerPinTX);



void setup() {
  pinMode(indPin, OUTPUT);
  digitalWrite(indPin, LOW);
  pinMode(focusSleepPin, OUTPUT);
  digitalWrite(focusSleepPin, 0);
  setPos(0, 43200);         //Set RA position to 12 hrs
  setPos(1, 324000);        //Set DEC position to 0 deg


  Serial.begin(hardwareSerialSpeed);
  SerialSoft.begin(softwareSerialSpeed);
  pinMode(raDirPin, OUTPUT);
  pinMode(raStepPin, OUTPUT);
  pinMode(raSpeedPin, OUTPUT);
  pinMode(decDirPin, OUTPUT);
  pinMode(decStepPin, OUTPUT);
  pinMode(focusStepPin, OUTPUT);
  digitalWrite(focusStepPin, LOW);
  pinMode(focusDirPin, OUTPUT);
  digitalWrite(focusDirPin, LOW);

  digitalWrite(raEndStopPin, HIGH);
  pinMode(raEndStopPin, INPUT);
  digitalWrite(raEndStopPin, HIGH);         //Need to fix this?
  pinMode(decEndStopPin, INPUT_PULLUP);

  digitalWrite(raSpeedPin, HIGH);
  digitalWrite(raDirPin, LOW);
  digitalWrite(decDirPin, LOW);
  convertDelay(10000);
  delay(10);
  Serial.println(conMsg);
  digitalWrite(indPin, HIGH);
}

void loop() { //Times tested on 2017-09-11
  static unsigned int i;
  if (getCommand()) { //16 - 24 us
    raMotor();
    parseCommand();
  }
  i++;
  if (i == 100) {
    if (getCommandSoft()) { //32 us
      raMotor();
      parseCommandSoft();
    }
  }
  raMotor();  //20 - 24 us, 12 - 64 tracking, 80 - 84 slewing west
  decMotor(); //8 us, 28 - 32 when moving
  printDebugInfo(); //8us
  runIndicatorLED();  //8us
  focusMotor();
}

