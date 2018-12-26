//By Tyler Gerritsen
//Last Revision 2017-01-18

//1.3158% Slow (2017-07-07)


#define hardwareSerialSpeed 57600
#define softwareSerialSpeed 9600

#define DEC_STEPS_PER_ARCSEC 7
#define DEC_MOTOR_MAX_SPEED 150
#define STEP_PIN_HIGH_TIME 2

//Enter calculated data
  //Speed Table
  //Remember to change to 85 tooth driven pulley!
  PROGMEM const unsigned long delayChangeTable[] = {10370,25305,24875,24464,24073,23698,23338,22994,22664,22347,22040,21747,21462,21189,20924,20669,20422,20182,19951,19726,19507,19297,19091,18891,18697,18508,18325,18145,17972,17801,17635,17474,17316,17161,17011,16864,16720,16580,16442,16308,16175,16047,15920,15797,15675,15556,15440,15325,15213,15103,14995,14889,14784,14682,14582,14483,14385,14290,14196,14104,14013,13923,13835,13749,13664,13580,13497,13415,13336,13256,13179,13102,13026,12952,12878,12806,12734,12664,12594,12526,12458,12391,12325,12260,12196,12132,12070,12008,11946,11887,11826,11768,11710,11652,11595,11539,11483,11429,11374,11321,11267,11215,11163,11112,11061,11011,10961,10912,10863,10815,10768,10720,10673,10628,10581,10536,10492,10447,10403,10359,10317,10274,10231,10190,10148,10106,10066,10026,9986,9946,9907,9868,9829,9791,9754,9715,9679,9641,9605,9569,9533,9497,9461,9427,9392,9357,9323,9289,9256,9222,9189,9156,9123,9091,9059,9027,8996,8964,8934,8902,8871,8842,8811,8781,8751,8722,8693,8663,8635,8606,8578,8549,8521,8494,8465,8439,8411,8384,8357,8330,8304,8278,8251,8225,8200,8174,8149,8123,8098,8073,8049,8023,8000,7974,7951,7927,7903,7879,7856,7832,7809,7786,7763,7740,7718,7695,7672,7651,7628,7606,7584,7563,7541,7519,7498,7477,7456,7435,7414,7393,7372,7353,7331,7312,7291,7271,7252,7231,7212,7192,7173,7154,7134,7115,7096,7077,7058,7039,7021,7002,6984,6965,6948,6929,6911,6893,6875,6857,6840,6822,6804,6788,6770,6752,6736,6718,6702,6684,6668,6651,6635,6618,6601,6585,6569,6552,6537,6520,6505,6488,6473,6456,6442,6425,6410,6395,6379,6364,6348,6334,6318,6304,6288,6274,6258,6244,6230,6214,6200,6186,6171,6157,6143,6128,6114,6100,6086,6072,6059,6044,6031,6017,6003,5989,5976,5963,5949,5936,5922,5910,5896,5882,5870,5857,5844,5831,5818,5805,5792,5780,5767,5755,5742,5729,5717,5705,5692,5680,5668,5656,5643,5631,5620,5607,5595,5584,5572,5559,5548,5537,5525,5513,5501,5490,5479,5467,5456,5444,5433,5422,5411,5399,5388,5378,5366,5355,5344,5334,5322,5312,5301,5290,5279,5269,5258,5247,5237,5227,5216,5205,5195,5185,5174,5164,5154,5144,5134,5123,5113,5103,5094,5083,5073,5064,5053,5044,5033,5024,5015,5004,4995,4986,4975,4966,4957,4947,4937,4929,4918,4910,4900,4890,4882,4872,4863,4854,4845,4835,4827,4817,4808,4800,4790,4781,4773,4764,4754,4746,4738,4728,4720,4711,4702,4694,4685,4677,4667,4660,4651,4642,4634,4626,4617,4609,4600,4592,4584,4576,4567,4559,4551,4543,4535,4526,4519,4510,4503,4494,4487,4478,4471,4462,4455,4447,4439,4432,4423,4416,4408,4400,4393,4385,4377,4370,4362,4355,4347,4339,4332,4325,4317,4310,4302,4295,4287,4280,4273,4266,4258,4251,4243,4237,4229,4223,4215,4207,4201,4194,4187,4179,4173,4165,4159,4151,4145,4138,4131,4124,4117,4110,4103,4097,4090,4083,4076,4070,4062,4057,4049,4043,4036,4030,4023,4016,4010,4003,3997,3991,3983,3978,3971,3964,3958,3952,3945,3939,3932,3927,3920,3913,3908,3901,3895,3888,3883,3876,3870,3864,3858,3851,3846,3839,3834,3827,3821,3815,3810,3803,3797,3791,3786,3779,3774,3767,3762,3756,3750,3744,3738,3733,3726,3721,3716,3709,3704,3698,3692,3687,3681,3675,3670,3664,3658,3653,3647,3642,3636,3630,3625,3620,3614,3608,3603,3598,3592,3587,3581,3576,3570,3565,3560,3555,3549,3543,3539,3533,3528,3522,3518,3512,3506,3502,3496,3492,3486,3481,3475,3471,3465,3461,3455,3450,3445,3440,3435,3430,3425,3419,3415,3410,3405,3400,3395,3390,3385,3380,3375,3370,3366,3360,3356,3351,3346,3341,3336,3332,3326,3322,3318,3312,3308,3303,3298,3293,3289,3284,3279,3275,3270,3266,3260,3257,3251,3247,3242,3238,3233,3229,3224,3219,3215,3210,3206,3202,3197,3192,3188,3183,3179,3175,3170,3166,3161,3157,3153,3148,3144,3139,3135,3131,3126,3122,3118,3113,3109,3105,3101,3096,3092,3088,3083,3080,3075,3071,3066,3063,3058,3054,3050,3046,3042,3037,3034,3029,3025,3021,3017,3013,3009,3005,3001,2996,2993,2989,2984,2981,2976,2973,2968,2965,2961,2956,2953,2949,2945,2941,2937,2933,2929,2925,2921,2918,2914,2909,2906,2902,2899,2894,2891,2886,2883,2880,2875,2872,2867,2865,2860,2856,2853,2849,2846,2841,2838,2835,2830,2827,2823,2820,2815,2813,2808,2805,2801,2798,2794,2790,2787,2783,2780,2776,2772,2769,2765,2762,2758,2755,2751,2748,2744,2740,2738,2733,2730,2727,2723,2720,2716,2713,2709,2706,2702,2699,2696,2692,2689,2685,2682,2678,2676,2671,2669,2665,2662,2658,2655,2652,2648,2645,2642,2638,2636,2631,2629,2625,2622,2619,2616,2612,2609,2605,2603,2599,2596,2593,2590,2586,2583,2580,2577,2574,2570,2567,2564,2561,2558,2555,2551,2549,2545,2542,2539,2536,2533,2530,2526,2524,2521,2517,2514,2512,2508,2505,2502,2499,2496,2493,2490,2487,2484,2481,2478,2475,2472,2469,2466,2463,2460,2457,2454,2452,2448,2445,2443,2439,2437,2433,2431,2428,2425,2422,2419,2416,2414,2410,2408,2405,2402,2399,2396,2393,2391,2388,2384,2382,2380,2376,2374,2371,2368,2365,2362,2360,2357,2354,2351,2349,2346,2343,2340,2338,2335,2332,2329,2327,2324,2321,2319,2316,2313,2310,2308,2305,2303,2299,2297,2295,2292,2289,2286,2284,2281,2279,2276,2273,2270,2268,2266,2263,2260,2258,2255,2252,2250,2247,2245,2242,2240,2237,2234,2232,2229,2227,2224,2222,2219,2217,2214,2212,2209,2206,2204,2202,2199,2197,2194,2191,2190,2186,2184,2182,2179,2177,2175,2172,2169,2167,2165,2162,2160,2157,2155,2152,2150,2148,2145,2143,2140,2139,2135,2134,2130,2129,2126,2124,2121,2119,2117,2115,2112,2109,2108,2105,2102,2101,2098,2095,2094,2091,2088,2087,2084,2082,2080,2077,2075,2072,2071,2068,2066,2064,2061,2059,2057,2054,2053,2050,2048,2045,2044,2041,2039,2037,2034,2032,2030,2028,2026,2023,2022,2019,2017,2014,2013,2010,2008,2006,2004,2002,1999,1997,1996,1993,1991,1988,1987,1984,1983,1980,1978,1976,1974,1971,1970,1967,1966,1963,1961,1959,1957,1955,1953,1951,1948,1947,1944,1943,1940,1938,1937,1934,1932,1930,1928,1926,1924,1922,1920,1918,1916,1913,1912,1910,1908,1905,1904,1902,1900,1897,1896,1894,1891,1890,1888,1886,1884,1881,1880,1878,1876,1874,1872,1870,1868,1867,1864,1862,1860,1859,1856,1855,1852,1851,1849,1847,1845,1843,1841,1839,1837,1836,1833,1832,1829,1828,1826,1824,1822,1821,1818,1817,1814,1813,1811,1809,1807,1806,1803,1802,1800,1798,1796,1794,1793,1790,1789,1787,1785,1783,1782,1780,1777,1776,1775,1772,1771,1769,1767,1765,1763,1762,1760,1758,1756,1755,1753,1751,1749,1747,1746,1744,1742,1741,1738,1737,1735,1734,1731,1730,1728,1727,1725,1723,1721,1719,1718,1716,1715,1712,1711,1709,1708,1706,1704,1702,1701,1699,1697,1696,1694,1692,1691,1688,1688,1685,1684,1682,1681,1679,1677,1675,1674,1672,1671,1669,1667,1666,1664,1662,1661,1659,1657,1656,1654,1653,1651,1649,1647,1647,1644,1643,1641,1640,1637,1637};

  const unsigned long raStepLimit = 4018969;               //Total steps to end of rod
  int delayChange = 1;                                   //Change in delay time per delay change
  unsigned long initMotorStepDelay = 2675;                      //Starting motor step delay
  unsigned long currReadStepDelay = initMotorStepDelay;
  unsigned long currMotorStepDelay;



/*Pins  
 From closest to FTDI Header
    Driver 1
      Step    4
      Dir     5  
      M1, M2  3
    Driver 2
      Step    6
      Dir     7
    Driver 3
      Step    2
      Dir     8
    Driver 4
      Step    9
      Dir     10
    
 */


/*
Sidereal day = 86164.09164 s
Sidereal rate = 239.344699 s / deg
              = 0.004178075 deg / s
              = 15.04106845 arcsec / s
*/

//Both Axis
unsigned int slewSettleTime = 2000;           //Slew settle time (ms)

//RA Motor
const byte raDirPin = 5;
const byte raStepPin = 4;
unsigned long raStepPosition;                 //Motor step position         Start position = 0    Home position = 2
int64_t raStepTimeTotal;                //Total time travelled by RA
unsigned long raActualTimeTotal;              //Copies from micros()
unsigned int raSlewRate = 250;                //RA slewing speed (us / step)
const unsigned int raSlewRateMax = 200;       //Max RA slew speed
unsigned int raPulseRate = 500;               //RA pulse guide rate (500 = 5.00% sidereal rate)
const long raMicrosPerArcsec = 1000000;         //Convert arcsecs to steps           66485 for 360 deg, 1000000 for 24hr
const unsigned long raTotalArcsecs = 86400;
char raMoveDir;                               //Move direction (1 = west, -1 = east)
const byte raSpeedPin = 3;
const byte raSlewFactor = 4;
unsigned long raPGtimer;
char raPGdir;

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
const byte decDirPin = 7;
const byte decStepPin = 6;
unsigned int decSlewRate = DEC_MOTOR_MAX_SPEED;            //DEC slewing speed
unsigned int decPGrate = 40000;        //DEC pulse guide speed (40,000 = 1 arcsec / sec, 1,000,000 = 4,000 = 10 arcsec / sec)
unsigned long decStepPosition = 20000000;                     //DEC motor position
unsigned long decGotoPosition = 20000000;                   //DEC motor target position
long decJog;                              //Set target offset of DEC
char decMoveDir;                        //Direction of movement (1 = north, -1 = south)
byte raMotorOn = 0;
unsigned long decPGtimer;
char decPGdir = 0;
byte decNorth = 1;
byte decSouth = 0;

const byte motorDriverStepDelay STEP_PIN_HIGH_TIME;

//Coordinate System
int64_t raCoorOffset = 324000;         //180 degrees
unsigned long decCoorOffset = 324000;

//Endstops
const byte raEndStopPin = 8;
const byte decEndStopPin = 13;

//Clutches
const byte decClutchReleasePin = 9;
const byte decClutchEngagePin = 10;
const byte raClutchReleasePin = 11;
const byte raClutchEngagePin = 12;
byte raClutch = 1;
byte decClutch = 1;
byte raClutchPos = 1;
byte decClutchPos = 1;
unsigned long clutchTimer = 1000;
unsigned int clutchPower = 200;

//Controller
const byte controllerPin = 14;

//Intervalometer
byte iActive = 0;
const unsigned long afterDitheringDelay = 1000;
const byte camPin = 4;
unsigned int iShots;
unsigned long iExp;
unsigned long iDelay;
unsigned long iDelayInit;
byte intervalometerActive = 0;
unsigned int iShotsLeft;
unsigned long nextCamTime;
unsigned long iDitherDistance;
byte dithering;
unsigned long iDitherW;
unsigned long iDitherN;


//Serial Commander

String COMstring;
unsigned long COMvalue;
const char terminator = '/';


//Misc
byte displayInfo = 51;

const byte indPin = 2;
unsigned int indDelay = 1000;

const byte voltagePin = 15;


//WiFi Control
#include <SoftwareSerial.h>
const byte softSerPinTX = 16;
const byte softSerPinRX = 17;
SoftwareSerial SerialSoft(softSerPinRX, softSerPinTX);



void setup() {
  pinMode(indPin, OUTPUT);
  digitalWrite(indPin, LOW);
  setPos(0, 43200);         //Set RA position to 12 hrs
  setPos(1, 324000);        //Set DEC position to 0 deg
  

  Serial.begin(hardwareSerialSpeed);
  SerialSoft.begin(softwareSerialSpeed);
  pinMode(raDirPin, OUTPUT);
  pinMode(raStepPin, OUTPUT);
  pinMode(raSpeedPin, OUTPUT);
  pinMode(decDirPin, OUTPUT);
  pinMode(decStepPin, OUTPUT);
  pinMode(camPin, OUTPUT);
  
  digitalWrite(raEndStopPin, HIGH);
  pinMode(raEndStopPin, INPUT);
  digitalWrite(raEndStopPin, HIGH);         //Need to fix this?
  pinMode(decEndStopPin, INPUT_PULLUP);
  
  digitalWrite(raSpeedPin, HIGH);
  digitalWrite(camPin, LOW);
  digitalWrite(raDirPin, LOW);
  digitalWrite(decDirPin, LOW);
  convertDelay(10000);
  delay(10);
  Serial.println("Tracker V3 Ready");
  digitalWrite(indPin, HIGH);
}



void loop() {
  if (getCommand()){
    raMotor();
    parseCommand();
  }
  if (getCommandSoft()){
    raMotor();
    parseCommand();
  }
  raMotor();
  decMotor();
  runIntervalometer();
  printDebugInfo();
  runIndicatorLED();
  readController();
}



int raSlew(byte query, long distance){   //Query: 0 = arcseconds, 1 = micros, 2 = zero everything
  static long slewArcsecs;
  static long slewMicros;
  if (!query) slewMicros += distance;                                                            //Add to micros
  else if (query == 1) {
    slewArcsecs += distance;                                                  //Add to arcsecs
  }
  else if (distance > 0 && slewMicros > 0 || distance < 0 && slewMicros < 0 || !distance) {      //Zero (optional if distance > 0, only if + and if distance < 0 then only if -)
    slewMicros = 0;
    slewArcsecs = 0;
  }
  while (slewMicros < 10000000 && slewArcsecs > 0){
    slewMicros += raMicrosPerArcsec;
    slewArcsecs--;
  }
  while (slewMicros > -10000000 && slewArcsecs < 0){
    slewMicros -= raMicrosPerArcsec;
    slewArcsecs++;
  }
  if (slewMicros > 0) return 1;
  else if (slewMicros < 0) return -1;
  else return 0;
}



void slewToPos(unsigned long targetPos){
  static unsigned long currentPos;
  static unsigned long distance;
  static long tempValue;
  raSlew(2,0);
  currentPos = currPos(0);
  if (currentPos > targetPos) distance = currentPos - targetPos;
  else distance = targetPos - currentPos;
  tempValue = abs(raTotalArcsecs - distance);
  if (distance < tempValue){
    Serial.println("Case 0");
    tempValue = currentPos - targetPos;
    raSlew(1,(tempValue));
  }
  else {
    if (currentPos > targetPos) raSlew(1, distance - raTotalArcsecs);
    else raSlew(1, raTotalArcsecs - distance);
  }
}







void raMotor(){
              /*Motion types (in priority):
               * Move               High speed, no end
               * Slew               high speed, end after distance
               * PulseGuide         Low speed, end after timer
               * 
               * raSlew is the distance to slew (us * sidereal rate = 0.00001504106845 arcsecs = 
               *                                                       66484.6   = 1 arcsec
               *                                                      (raMicrosPerArcsec)
               *                                                      
               *
                */

  static unsigned long nextStepTime;
  if (micros() > nextStepTime){    //If timer is up and we are not at end of rod
    byte steps = 0;
    byte direction = 1;
    if (!raMotorOn) nextStepTime = micros() + 500;                  //Just keep timer updated
    else {
      if (raSlew(0,0) < 0 || raMoveDir > 0) {     //Slewing east
        if (digitalRead(decEndStopPin)){          //Check endstop, if depressed then set home position and stop
          raStepPosition = 2;
          raSlew(2, 0);
        }
        else { 
          steps = 4;
          direction = 0;
          steps = raSlewFactor;
          if (raStepPosition == 2) raSlew(2,0);                          //Set current position to home
          nextStepTime += raSlewRate;
          int raSlewChange = currMotorStepDelay * raSlewFactor + raSlewRate;
          raSlew(0,raSlewChange);
          raSlew(2,1);
          raStepTimeTotal -= currMotorStepDelay * steps;
        }
      }
      else if (raStepPosition < raStepLimit) {                            //Not slewing east
        if (raSlew(0,0)|| raMoveDir < 0) {                               //Slewing west
          steps = 4;
          nextStepTime += raSlewRate;
          int raSlewChange = raSlewRate - currMotorStepDelay * raSlewFactor;
          raSlew(0,raSlewChange);
          raSlew(2,-1);
          if (raStepPosition == 2) raSlew(2,0);
        }
        else {                                                          //No slewing
          steps = 1;
          nextStepTime += currMotorStepDelay;
          if (millis() < raPGtimer){                                    //Pulse guiding
            long temp = currMotorStepDelay * raPGdir * raPulseRate;
            temp /= 10000;
            nextStepTime -= temp;
          }
        }
        raStepTimeTotal += currMotorStepDelay * steps;
      }
      if (steps){
        raStepMotor(direction, steps);
        updateDelay();
      }
    }
    raActualTimeTotal = micros();
  }
}



void raStepMotor(char raDir, byte steps){
  static byte raStepPinHigh;
  static char raDirLast;
  static byte raStepsLast;
  
      //Set speed pin (1/32 or 1/8 stepping)
  if (steps != raStepsLast){
    if (steps == 4) digitalWrite(raSpeedPin, 0);  
    else digitalWrite(raSpeedPin, 1);      
    raStepsLast = steps;
  }

      //Set dir pin
  if (raDir != raDirLast){
    digitalWrite(raDirPin, raDir);
    raDirLast = raDir;
  }
      
      //Set step pin and calculate position
  if (raStepPinHigh) raStepPinHigh = 0;
  else {
    raStepPinHigh = 1;
    if (raDir) raStepPosition += steps;
    else if (raStepPosition > steps + 2) raStepPosition -= steps;
    else raStepPosition = 2;
  }
  digitalWrite(raStepPin, raStepPinHigh);
}




void decMotor(){
  static byte stepPinHigh;
  static unsigned long nextStepTime;
  static char dir;
  static unsigned long nextStepDelay;

              /*Motion types (in priority):
               * Move               High speed, no end
               * Slew               high speed, end after distance
               * PulseGuide         Low speed, end after timer
                */
                
  if (micros() > nextStepTime){

    //Calculate dir and next step time
    nextStepDelay = decSlewRate;                                                                  //Default delay time for moving & slewing
    if (decGotoPosition == decStepPosition && decMoveDir != 0)decGotoPosition += decMoveDir;      //Moving
    else if (millis() < decPGtimer && decGotoPosition == decStepPosition){                        //Pulse Guiding & not Slewing
      nextStepDelay = decPGrate;
      decGotoPosition += decPGdir;
    }
    nextStepTime += nextStepDelay;          //Add determined value to timer

    //Run Motor
    if (decGotoPosition > decStepPosition){
      if (!dir) digitalWrite(decDirPin, decNorth);
      dir = 1;
      decStepPosition++;
      digitalWrite(decStepPin, 1);
      delayMicroseconds(motorDriverStepDelay);
      digitalWrite(decStepPin, 0);
    }
    else if (decGotoPosition < decStepPosition){
      if (dir) digitalWrite(decDirPin, decSouth);
      dir = 0;
      decStepPosition--;
      digitalWrite(decStepPin, 1);
      delayMicroseconds(motorDriverStepDelay);
      digitalWrite(decStepPin, 0);
    }
  }
}



void updateDelay(){
  static unsigned long nextDelayChangeStep = 1;
  static unsigned long lastDelayChangeStep;
  static unsigned long delayChangePhase;
  while (raStepPosition > nextDelayChangeStep) {
    currReadStepDelay += delayChange;
    convertDelay(0);
    delayChangePhase++;
    unsigned int tableValue = pgm_read_dword_near(delayChangeTable + delayChangePhase);
    if (!tableValue) {
      delayChange *= -1;
      delayChangePhase++;
      tableValue = pgm_read_dword_near(delayChangeTable + delayChangePhase);
    }
    lastDelayChangeStep = raStepPosition;
    nextDelayChangeStep += tableValue;
  }
  
  while (raStepPosition < lastDelayChangeStep) {
    currReadStepDelay -= delayChange;
    convertDelay(0);
    delayChangePhase--;
    unsigned int tableValue = pgm_read_dword_near(delayChangeTable + delayChangePhase);
    if (!tableValue) {
      delayChange *= -1;
      delayChangePhase--;
      tableValue = pgm_read_dword_near(delayChangeTable + delayChangePhase);
    }
    nextDelayChangeStep = raStepPosition;
    lastDelayChangeStep -= tableValue;
  }
}

void convertDelay(unsigned long newSpeed){
  static unsigned long raSpeed;
  if (newSpeed) raSpeed = newSpeed;
  currMotorStepDelay = currReadStepDelay * 5000;                                       //Divide by 2 to account for pin up / pin down
  currMotorStepDelay /= raSpeed;
}



void setPos(byte axis, long pos){      //axis 0 = RA, 1 = DEC
  if (!axis)raCoorOffset = pos - (raActualTimeTotal - raStepTimeTotal) / raMicrosPerArcsec;
  else  decCoorOffset =  decStepPosition / decStepsPerArcsec - pos;     //DEC
}



long currPos(byte axis){
  static int64_t pos;
  if (!axis){   //RA pos = (micros() - raStepTimeTotal) / raMicrosPerArcsec - raCoorOffset
    pos = raActualTimeTotal;
    pos -= raStepTimeTotal;
    pos /= raMicrosPerArcsec;
    pos += raCoorOffset;
    while (pos < 0) {
      pos += raTotalArcsecs;
      raCoorOffset += raTotalArcsecs;
    }
    while (pos > raTotalArcsecs) {
      pos -= raTotalArcsecs;
      raCoorOffset -= raTotalArcsecs;
    }
    return pos;
  }
  else {        //DEC
    pos = decStepPosition;
    pos /= decStepsPerArcsec;
    pos -= decCoorOffset;
    return pos;
  }
}



void returnIsHome(){
  if (raStepPosition == 2) Serial.print("y/");
  else Serial.print("n/");
}


void setPulseGuide(byte axis, char dir, unsigned long timer){
  if (!axis){
    raPGdir = dir;
    raPGtimer = millis() + timer;
  }
  else {
    decPGdir = dir;
    decPGtimer = millis() + timer;
  }
}

void printIsPulseGuiding(){
  if (millis() > decPGtimer && millis() > raPGtimer) Serial.print("n/");
  else Serial.print("y/");
}


String queryMotorOn(){
  if (raMotorOn) return "y";
  else return "n";
}



String querySlewing(){
  if (decGotoPosition != decStepPosition || raSlew(0,0)) return "y";
  else return "n";
}



void targetControl(byte axis, unsigned long pos){    //axis 0 = ra,  1 = dec, 2 = methods
  static unsigned long raTargetPos;
  static unsigned long decTargetPos;
  if (!axis) raTargetPos = pos;
  else if (axis == 1) decTargetPos = pos;
  else {
    switch (pos){
      case 0:              //cancel slew
        decGotoPosition = decStepPosition;
        raSlew(2,0);
      break;
      case 1:                   //Start slew
        raSlew(1, (currPos(0) - raTargetPos));
        decGotoPosition = (decTargetPos + decCoorOffset) * decStepsPerArcsec;
      break;
      case 2:
        setPos(0, raTargetPos);    //target pos is current pos
        setPos(1, decTargetPos);
      break;
    }
  }
}



 byte voltage(){                            //Returns battery voltage in 0.1v (120 = 12.0v)
  unsigned int v = analogRead(voltagePin);
  v *= 30;
  v /= 46;
  return v;
 }




void readController(){
  static unsigned long timer;
  static unsigned int input;
  
  input = analogRead(controllerPin);
  if (input > 500){
    if (!timer) timer = millis();
  }
  else if (timer){
    timer = 0;
    unsigned long buttonTime = millis() - timer;
    if (buttonTime > 2000) {
      byte temp = decNorth;
      decNorth = decSouth;
      decSouth = temp;
    }
    else if (buttonTime > 100) {
      if (raClutch){
        raClutch = 0;
        decClutch = 0;
      }
      else {
        raClutch = 1;
        decClutch = 1;
      }
    }
  }
}


boolean getCommand() {
  static byte i = 0;
  static char stringChar[20];
  boolean newCommand = false;
  COMstring = "";
  COMvalue = 0;
  if (Serial.available()){
    char COMinput = Serial.read();
    if (COMinput == terminator) newCommand = true;
    else if (COMinput > 47){
      stringChar[i] = COMinput;
      i++;
    }
    if (newCommand){
      for(byte j=0; j<i; j++){
        if (stringChar[j] > 57) COMstring += String(stringChar[j]);
        else COMvalue = COMvalue * 10 + stringChar[j] - '0';
        stringChar[j] = 0;
      }
      i=0;
    }
  }
  return newCommand;
}


boolean getCommandSoft() {
  static byte i = 0;
  static char stringChar[20];
  boolean newCommand = false;
  COMstring = "";
  COMvalue = 0;
  if (SerialSoft.available()){
    char COMinput = SerialSoft.read();
    if (COMinput == terminator) newCommand = true;
    else if (COMinput > 47){
      stringChar[i] = COMinput;
      i++;
    }
    if (newCommand){
      for(byte j=0; j<i; j++){
        if (stringChar[j] > 57) COMstring += String(stringChar[j]);
        else COMvalue = COMvalue * 10 + stringChar[j] - '0';
        stringChar[j] = 0;
      }
      i=0;
    }
  }
  return newCommand;
}



void parseCommand(){    //Serial Commander
                                              //ASCOM Driver Methods / Queries
    if (COMstring == "cd") setPos(1, COMvalue);
    else if (COMstring == "cr") setPos(0, COMvalue);
    else if (COMstring == "cdq") Serial.print("cdq" + String(currPos(1)) + "/");
    else if (COMstring == "crq") Serial.print("crq" + String(currPos(0)) + "/");
    else if (COMstring == "ct") targetControl(2, 2);              //Set target position to current position
    else if (COMstring == "tr") targetControl(0, COMvalue);       //Set target RA coordinate
    else if (COMstring == "td") targetControl(1, COMvalue);       //Set target DEC coordinates
    else if (COMstring == "hh") raMoveDir = 1;                  //Go Home                                       ###Remove
    else if (COMstring == "hq") returnIsHome();                   //Query is home
    else if (COMstring == "pp") raMotorOn = 0;                    //Park
    else if (COMstring == "up") raMotorOn = 1;                    //Unpark
    else if (COMstring == "pq") Serial.print(queryMotorOn() + "/");     // Query park
    else if (COMstring == "ps") int i = 0;                        //Set park position to current position
    else if (COMstring == "sr") slewToPos(COMvalue);            //RA slew to coordinate
    else if (COMstring == "sd") decGotoPosition = (COMvalue + decCoorOffset) * decStepsPerArcsec;
    else if (COMstring == "st") targetControl(2, 1);              //GoTo target coordinates
    else if (COMstring == "sq") Serial.print(querySlewing() + "/");     //Query is slewing
    else if (COMstring == "ss") targetControl(2, 0);              //Stop Slew
    else if (COMstring == "sw") int i = 0;                        //Set slew settle time (s)
    else if (COMstring == "swq") int i = 0;                       //Query slew settle time (s)
    else if (COMstring == "mn") decMoveDir = 1;
    else if (COMstring == "ms") decMoveDir = -1;
    else if (COMstring == "mw") raMoveDir = -1;
    else if (COMstring == "me") raMoveDir = 1;
    else if (COMstring == "md") decMoveDir = 0;
    else if (COMstring == "mr") raMoveDir = 0;
    else if (COMstring == "gt") raMotorOn = 1;     //Tracking on
    else if (COMstring == "gf") raMotorOn = 0;     //Tracking off
    else if (COMstring == "gq") Serial.print(queryMotorOn() + "/");     //Query is tracking
    else if (COMstring == "rr") convertDelay(COMvalue);     //Right ascension rate
    else if (COMstring == "rd") int i = 0;     //DEC rate 
    else if (COMstring == "pgn") setPulseGuide(1, 1, COMvalue);
    else if (COMstring == "pgs") setPulseGuide(1, -1, COMvalue);
    else if (COMstring == "pgw") setPulseGuide(0, 1, COMvalue);
    else if (COMstring == "pge") setPulseGuide(0, -1, COMvalue);
    else if (COMstring == "pgq") printIsPulseGuiding();
    else if (COMstring == "pgsr") raPulseRate = COMvalue;
    else if (COMstring == "pgsd") decPGrate = COMvalue;
    else if (COMstring == "pgqr") Serial.print(raPulseRate + "/");
    else if (COMstring == "pgqd") Serial.print(decPGrate + "/");

    //Debugging & Non-Driver Queries
    else if (COMstring == "info") displayInfo = 1;
    
    else if (COMstring == "is") intervalometer(0, COMvalue);
    else if (COMstring == "ie") intervalometer(1, COMvalue);
    else if (COMstring == "id") intervalometer(2, COMvalue);
    else if (COMstring == "ii") intervalometer(3, COMvalue);
    else if (COMstring == "i") intervalometer(5, 0);
    else if (COMstring == "sw") raSlew(1, COMvalue);       //Recieve distance in arcsecs
    else if (COMstring == "se") raSlew(1, -1 * COMvalue);  //REcieve distance in arcsecs
    else if (COMstring == "sn") decGotoPosition += COMvalue * decStepsPerArcsec;
    else if (COMstring == "ss") decGotoPosition -= COMvalue * decStepsPerArcsec;
    else if (COMstring == "rap") raStepPosition = COMvalue * 7500;
    else if (COMstring == "ins") decSlewRate = COMvalue;
    else if (COMstring == "ras") raSlewRate = COMvalue;
    else if (COMstring == "cldr") decClutch = 0;
    else if (COMstring == "clde") decClutch = 1;
    else if (COMstring == "cldeo") decClutchPos = 0;
    else if (COMstring == "cldro") decClutchPos = 1;
    else if (COMstring == "clrr") raClutch = 0;
    else if (COMstring == "clre") raClutch = 1;
    else if (COMstring == "clr") {raClutch = 0; decClutch = 0;}
    else if (COMstring == "cle") {raClutch = 1; decClutch = 1;}
    else if (COMstring == "clp") clutchPower = COMvalue;
    else if (COMstring == "clt") clutchTimer = COMvalue;
    else if (COMstring == "conn") Serial.print("Ready");
    
    else {
      Serial.print(F(" Command not recognized: "));
      Serial.println(COMstring);
    }
    Serial.println();
}



void intervalometer(byte param, unsigned long value){
  if (param == 5) {
    iShotsLeft = iShots;
    nextCamTime = millis() + iDelayInit;
    iDitherW = 0;
    iDitherN = 0;
    Serial.println(F("Beginning Imaging"));
  }
  else {
    if (param == 0) iShots = value;
    else if (param == 1) iExp = value * 1000;
    else if (param == 2) iDelay = value * 1000;
    else if (param == 3) iDelayInit = value * 1000;
    displayInfo=1;
  }
}



void runIndicatorLED(){
  static unsigned long tmr;
  static byte onoff;
  static unsigned int cycleCounter;

  if (cycleCounter == 10000){
    if (voltage() < 100){
      indDelay = 250;
    }
    else if (!raMotorOn) indDelay = 1000;
    else indDelay = 10000;
    cycleCounter = 0;
  }
  cycleCounter++;
  
  if (millis() > tmr){
    if (onoff && millis() > tmr){
      onoff = 0;
      tmr += indDelay;
      digitalWrite(indPin, HIGH);
    }
    else {
      onoff = 1;
      tmr += 3;
      digitalWrite(indPin, LOW);
    }
  }
}




void runIntervalometer(){
  static byte shutterOpen;
  if (iShotsLeft){
    if (millis() > nextCamTime){
      if (shutterOpen){
        shutterOpen = 0;
        digitalWrite(camPin, LOW);
        iShotsLeft--;
        nextCamTime += iDelay;
        Serial.print(iShotsLeft);
        Serial.println(F(" shots remaining"));
        if (iDitherDistance){
          unsigned int temp = random(-1 * iDitherDistance, iDitherDistance);
          raSlew(0,temp - iDitherW);
          iDitherW = temp;
          temp = random(-1 * iDitherDistance, iDitherDistance);
          decJog = temp - iDitherN;
          iDitherN = temp;
          dithering = 1;
        }
      }
      else if (dithering){
        if (!raSlew(0,0) && !decJog){
          nextCamTime = millis() + afterDitheringDelay;
          dithering = 0;
          Serial.println(F("End Dither"));
        }
      }
      else  {
        digitalWrite(camPin, HIGH);
        shutterOpen = 1;
        nextCamTime = millis() + iExp;
        Serial.println(F("Exposing"));
      }
    }
  }
}



void printDebugInfo(){
  long temp;
  switch (displayInfo){
   case 1: 
    Serial.println(F("Millis: "));
   break;
   case 2:
    Serial.print(millis());
   break;
   case 3:
    Serial.print(" ");
   break;
   case 4:
    Serial.print(iExp);
   break;
   case 5:
    Serial.print(" ");
   break;
   case 6:
    Serial.print(iDelay);
   break;
   case 7:
    Serial.print(" ");
   break;
   case 8:
    Serial.print(iDelayInit);
   break;
   case 9:
    Serial.print(" ");
   break;
   case 10:
    Serial.println(iDitherDistance);
  break;
    case 21:
      Serial.print(F("RA Curr Pos: "));
    break;
    case 22:
      Serial.println(raStepPosition);
    break;
    case 23:
      Serial.print(F("RA Motor Speed: "));
    break;
    case 24:
      Serial.println(currMotorStepDelay);
    break;
    case 25:
      Serial.print(F("RA Slew: "));
    break;
    case 26:
      Serial.println(raSlew(0,0));
    break;
    case 27:
      Serial.print(F("dec Target Position: "));
    break;
    case 28:
      Serial.println(decGotoPosition);
    break;
    case 29:
      Serial.print(F("dec Current Position: "));
    break;
    case 30:
      Serial.println(decStepPosition);
    break;
    case 31:
      Serial.print("decCoorOffset: ");
    break;
    case 32:
      Serial.println(decCoorOffset);
    break;
    case 33:
      Serial.print("Current Clock Time: ");
    break;
    case 34:
      Serial.println(raActualTimeTotal);
    break;    
    case 35:
      Serial.print("Current TOtal RA Step Time: ");
    break;
    case 36:
      temp = raStepTimeTotal;
      Serial.println(temp);
    break;
    case 37:
      Serial.print("RA Clock / Step Time Offset: ");
    break;
    case 38:
    temp = raActualTimeTotal - raStepTimeTotal;
      Serial.println(temp);
    break;
    case 39:
      Serial.print("raCoorOffset ");
    break;
    case 40:
      temp = raCoorOffset;
      Serial.println(temp);
    break;
    case 41:
      Serial.print("Voltage: ");
    break;
    case 42:
      Serial.println(voltage());
    break;
    case 43:
      Serial.print("raHomePin: ");
    break;
    case 44:
      Serial.println(digitalRead(raEndStopPin));
    break;
    case 45:
      Serial.print("decHomePin: ");
    break;
    case 46:
      Serial.println(digitalRead(decEndStopPin));
    break;
    case 47:
      Serial.print("Controller Pin Read: ");
    break;
    case 48:
      Serial.println(analogRead(controllerPin));
    break;
    case 49:
      Serial.print("indDelay: ");
    break;
    case 50:
      Serial.print(indDelay);
    break;
    case 51:
      displayInfo--;
    break;
  }
  displayInfo++;
}
