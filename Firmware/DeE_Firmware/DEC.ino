void decMotor() {
  static byte stepPinHigh;
  static unsigned long nextStepTime;
  static char dir;

  /*Motion types (in priority):
     Move               High speed, no end
     Slew               high speed, end after distance
     PulseGuide         Low speed, end after timer
  */

  if (micros() > nextStepTime) {

    //Calculate dir and next step time
    if (decTargetPosition != decStepPosition) nextStepTime += decSlewRate;    //Default delay time for moving & slewing
    else if (decMoveDir != 0) {
      nextStepTime += decSlewRate;
      decTargetPosition += decMoveDir;      //Moving
    }
    else if (millis() < decPGtimer) {                       //Pulse Guiding & not Slewing
      nextStepTime += decPGrate;
      decTargetPosition += decPGdir;
    }
    else nextStepTime += 100000;

    //Run Motor
    if (decTargetPosition > decStepPosition) {
      if (!dir) digitalWrite(decDirPin, 1 - decReverse);
      dir = 1;
      decStepPosition++;
      digitalWrite(decStepPin, 1);
      delayMicroseconds(motorDriverStepDelay);
      digitalWrite(decStepPin, 0);
    }
    else if (decTargetPosition < decStepPosition) {
      if (dir) digitalWrite(decDirPin, decReverse);
      dir = 0;
      decStepPosition--;
      digitalWrite(decStepPin, 1);
      delayMicroseconds(motorDriverStepDelay);
      digitalWrite(decStepPin, 0);
    }
  }
}
