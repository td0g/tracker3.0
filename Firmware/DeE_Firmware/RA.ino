void raMotor() {
  /*Motion types (in priority):
     Move               High speed, no end
     Slew               high speed, end after distance
     PulseGuide         Low speed, end after timer

     raSlew is the distance to slew (us * sidereal rate = 0.00001504106845 arcsecs =
                                                           66484.6   = 1 arcsec
                                                          (raMicrosPerArcsec)


  */

  static unsigned long nextStepTime;
  static long remainder;

  if (micros() > nextStepTime){
    if (raSlewing){
      nextStepTime += raSlewRate;
      long distance = raPosArcsecs - raTargetArcsecs;
      if (distance < 0) distance += raTotalArcsecs;
      if (distance && distance > raTotalArcsecs - distance){   //Slewing east
          if (raStepPosition > 2 || !RA_ENDSTOP){
          raStepMotor(0, 4);
          remainder += currMotorStepDelay * raSlewFactor - raSlewRate;
        }
        else {
          raTargetArcsecs = raPosArcsecs;
          raSlewing = 0;
        }
      }
      else if (distance){    //Slewing west
        raStepMotor(1, 4);
        remainder -= currMotorStepDelay * raSlewFactor + raSlewRate;
      }
      else raSlewing = 0;
    }
    else if (raMotorOn){                      //Tracking
      raStepMotor(1, 1);
      nextStepTime += currMotorStepDelay;
      if (millis() < raPGtimer){
        long temp = currMotorStepDelay * raPGdir * raPulseRate;
        temp /= 10000;
        nextStepTime -= temp;
      }
    }
    else {                                    //Parked
      remainder += currMotorStepDelay;
      nextStepTime += currMotorStepDelay;
    }

      //Calculate RA position in arcseconds
    while (remainder >= raMicrosPerArcsec) {
      remainder -= raMicrosPerArcsec;
      raPosArcsecs += 1;
      if (raPosArcsecs > 86400) raPosArcsecs -= 86400;
    }
    while (remainder <= -raMicrosPerArcsec){
      remainder += raMicrosPerArcsec;
      raPosArcsecs -= 1;
      if (raPosArcsecs < 0) raPosArcsecs += 86400;
    }
  }
}



void raStepMotor(char raDir, byte steps) {
  static byte raStepPinHigh;
  static char raDirLast;
  static byte raStepsLast;

  //Set speed pin (1/32 or 1/8 stepping)
  if (steps != raStepsLast) {
    if (steps == 4) digitalWrite(raSpeedPin, 0);
    else digitalWrite(raSpeedPin, 1);
    raStepsLast = steps;
  }

  //Set dir pin
  if (raDir != raDirLast) {
    digitalWrite(raDirPin, raDir);
    raDirLast = raDir;
  }

  //Set step pin and calculate position
  digitalWrite(raStepPin, 1);
  if (raDir) raStepPosition += steps;
  else if (raStepPosition > steps + 1) raStepPosition -= steps;
  else raStepPosition = 2;
  updateDelay();
  digitalWrite(raStepPin, 0);
}


void updateDelay() {
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

  while (raStepPosition < lastDelayChangeStep && delayChangePhase > 1) {
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

void convertDelay(unsigned long newSpeed) {
  static unsigned long raSpeed;
  if (newSpeed) raSpeed = newSpeed;
  currMotorStepDelay = currReadStepDelay * 10000;
  currMotorStepDelay /= raSpeed;
}
