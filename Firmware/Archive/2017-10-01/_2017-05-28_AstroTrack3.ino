//Arduino Uno code for Astrophotography Tracking Mount
//

/*
      Serial commands:

          m = Change motor speed in 0.1%s (1000 = 100%)
          ie = Change intervalometer exposure time (seconds)
          io = Change intervalometer wait time (seconds)
          i = Turn on intervalometer for XXX shots
 */

//
//By Tyler Gerritsen
//Last Revision 2017-01-18

//Enter calculated data
    //Speed Table
    PROGMEM const unsigned long delayChangeTable[] = {3139,3691,3697,3703,3708,3715,3720,3725,3732,3738,3743,3749,3756,3761,3767,3773,3779,3785,3791,3797,3803,3809,3815,3822,3827,3834,3840,3845,3853,3858,3864,3871,3877,3884,3889,3896,3903,3908,3916,3921,3928,3934,3941,3948,3953,3961,3967,3973,3980,3987,3993,4000,4007,4013,4020,4027,4034,4040,4047,4054,4061,4068,4074,4082,4088,4095,4103,4109,4116,4124,4130,4138,4145,4152,4159,4166,4173,4181,4188,4195,4203,4210,4217,4225,4232,4239,4247,4255,4262,4269,4277,4285,4292,4300,4308,4315,4323,4331,4339,4346,4355,4362,4370,4378,4385,4394,4402,4410,4418,4426,4434,4443,4450,4459,4467,4475,4484,4492,4500,4509,4517,4525,4534,4543,4551,4560,4568,4577,4586,4594,4603,4612,4621,4629,4639,4647,4656,4666,4674,4684,4692,4702,4710,4720,4730,4738,4748,4757,4767,4776,4785,4795,4804,4814,4824,4833,4843,4853,4863,4872,4882,4892,4902,4912,4922,4932,4942,4953,4962,4973,4983,4993,5004,5014,5024,5035,5046,5056,5067,5077,5089,5099,5109,5121,5132,5142,5154,5164,5176,5187,5198,5209,5221,5232,5244,5254,5267,5278,5290,5301,5313,5325,5337,5348,5361,5372,5385,5397,5409,5421,5434,5446,5458,5471,5483,5496,5509,5521,5534,5547,5560,5573,5586,5599,5613,5625,5639,5652,5666,5679,5693,5706,5720,5734,5748,5762,5776,5790,5804,5818,5833,5847,5862,5876,5891,5905,5920,5935,5951,5965,5980,5995,6011,6026,6042,6057,6073,6088,6105,6120,6136,6153,6168,6185,6201,6218,6234,6251,6268,6285,6301,6319,6336,6353,6370,6389,6405,6424,6441,6459,6477,6495,6514,6532,6551,6569,6588,6607,6625,6645,6665,6683,6703,6723,6743,6762,6782,6803,6823,6843,6864,6885,6906,6926,6948,6969,6991,7012,7034,7056,7079,7100,7123,7145,7168,7191,7215,7237,7261,7285,7308,7333,7356,7381,7406,7430,7455,7481,7506,7531,7557,7583,7610,7635,7663,7689,7716,7744,7771,7799,7827,7855,7884,7913,7941,7971,8000,8030,8061,8090,8121,8152,8184,8215,8246,8279,8311,8344,8376,8411,8443,8478,8513,8546,8582,8618,8653,8689,8726,8763,8800,8838,8876,8914,8953,8993,9033,9072,9114,9155,9196,9239,9281,9324,9368,9412,9457,9502,9548,9594,9641,9689,9737,9786,9835,9886,9936,9987,10040,10093,10146,10200,10256,10311,10368,10426,10484,10543,10604,10664,10727,10789,10854,10918,10985,11051,11120,11190,11259,11332,11405,11478,11555,11632,11710,11790,11871,11955,12039,12125,12213,12303,12395,12489,12584,12682,12781,12884,12988,13095,13204,13317,13431,13549,13669,13794,13920,14051,14186,14323,14466,14612,14762,14918,15079,15244,15415,15592,15776,15966,16163,16369,16581,16804,17035,17277,17529,17795,18073,18365,18674,19000,19345,19711,20101,20520,20967,21450,21974,22543,23168,23857,24625,25489,26472,27607,28942,30550,32548,35131,38690,44101,54056,89213,0,79255,0,1,0,1,70152};
    
    const unsigned long totalSteps = 4018969;               //Total steps to end of rod
    int delayChange = -1;                                   //Change in delay time per delay change
    unsigned long currReadStepDelay = 3989;                      //Starting motor step delay
    unsigned long currMotorStepDelay = currReadStepDelay;


 

//Decalare Adjustable Variables
unsigned int timeRate = 1000;           //0.1%s
const unsigned int timeRateMax = 2000;  //Max Speed
const String timeRateMaxWarning = "NOPE!";
unsigned long iExposure = 1000;
unsigned long iOffTime = 5000;          //Inter-Shot Wait Time
unsigned long iChangeTime;
unsigned int iSubs;
boolean iOnOff;
boolean motorOn = true;


//Declare global variables
unsigned long currStepPosition;
unsigned long nextStepTime;
unsigned int delayChangePhase=1;
unsigned long nextDelayChangeStep;


//Define pins
const byte iPin = 13;
const byte button = 12;
const byte a = 3;
const byte b = 4;
byte c = 5;
byte d = 6;

  //Serial Commander
String COMstring;
unsigned long COMvalue;
const char terminator = '/';

    
void setup() {
  pinMode(iPin, OUTPUT);
  digitalWrite(iPin, LOW);

  Serial.begin(115200);
        
  nextDelayChangeStep = pgm_read_dword_near(delayChangeTable);

  pinMode(a, OUTPUT);
  digitalWrite(a, LOW);
  pinMode(b, OUTPUT);
  digitalWrite(b, LOW);
  pinMode(c, OUTPUT);
  digitalWrite(c, LOW);
  pinMode(d, OUTPUT);
  digitalWrite(d, LOW);
  pinMode(button, INPUT_PULLUP);

  delay(100);
  Serial.println("Ready!");
  nextStepTime = micros();
}



void loop() {
  if (currStepPosition < totalSteps && micros() > nextStepTime && motorOn){
    currStepPosition++;
    moveMotor();
    updateDelay();
    nextStepTime += currMotorStepDelay;
  }
  operateCamera();
  parseCommand();
}



void moveMotor(){
    switch (currStepPosition % 8){
      case 0:
        digitalWrite(a, HIGH);
      break;
      case 1:
        digitalWrite(d, LOW);
      break;
      case 2:
        digitalWrite(c, HIGH);
      break;
      case 3:
        digitalWrite(a, LOW);
      break;
      case 4:
        digitalWrite(b, HIGH);
      break;
      case 5:
        digitalWrite(c, LOW);
      break;
      case 6:
        digitalWrite(d, HIGH);
      break;
      case 7:
        digitalWrite(b, LOW);
      break; 
    }
}



void updateDelay(){
  if (currStepPosition == nextDelayChangeStep){
    currReadStepDelay += delayChange;
    currMotorStepDelay = currReadStepDelay * 1000;
    currMotorStepDelay /= timeRate;
    delayChangePhase++;
    unsigned int tableValue = pgm_read_dword_near(delayChangeTable + delayChangePhase);
    if (!tableValue) {
      delayChange *= -1;
      delayChangePhase++;
      tableValue = pgm_read_dword_near(delayChangeTable + delayChangePhase);
    }
    nextDelayChangeStep += tableValue;
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
  Serial.print(COMstring);
  Serial.println(COMvalue);
}




void parseCommand(){
  if (getCommand()){                          //Serial Commander
    if (COMstring == "m"){
      if (COMvalue <= timeRateMax && COMvalue){
        if (!motorOn) nextStepTime = micros();
        motorOn = true;
        timeRate = COMvalue;
        currMotorStepDelay = currReadStepDelay * 1000 / timeRate;
        Serial.print("New Rate: ");
        Serial.println(timeRate);
      }
      else{
        Serial.println(timeRateMaxWarning);
      }
    }
    else if (COMstring == "mo"){
      motorOn = false;
      digitalWrite(a, LOW);
      digitalWrite(b, LOW);
      digitalWrite(c, LOW);
      digitalWrite(d, LOW);
    }
    else if (COMstring == "ie"){
      iExposure = COMvalue * 1000;
      Serial.print("Exp Tm: ");
      Serial.println(COMvalue);
    }
    else if (COMstring == "io"){
      iOffTime = COMvalue * 1000;
      Serial.print("Exp Off Tm: ");
      Serial.println(COMvalue);
    }
    else if (COMstring == "i"){
      if (!COMvalue)iSubs = 64000;
      else iSubs = COMvalue;
      Serial.print("Subs: ");
      Serial.println(iSubs);
      Serial.print("   iChangeTime: ");
      Serial.println(iChangeTime);
    }
  //else if (COMstring == "ba") speedTEST(COMvalue, 1, 0);
    else {
      Serial.print(" Command not recognized: ");
      Serial.println(COMstring);
    }
  }
}



void operateCamera(){
  if (button){
    if (iSubs){
      if (!iChangeTime) iChangeTime = millis();
      if (millis() > iChangeTime){
        if (iOnOff) {
          iOnOff = false;
          Serial.println("Xps OFF");
          digitalWrite(iPin, LOW);   //Toggle iPin
          iSubs--;
          if (iSubs) iChangeTime += iOffTime;
          else {
            iChangeTime = 0;
            Serial.println("IntValMtr Complete!");
          }
        }
        else {
          iOnOff = true;
          Serial.println("Xpsg");
          digitalWrite(iPin, HIGH);                    //Toggle iPin
          iChangeTime += iExposure;
        }
      }
    }
  }
  else digitalWrite(iPin, HIGH);
}

