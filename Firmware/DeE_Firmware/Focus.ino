void focusMotor() {
  static byte dirPinHigh;
  static unsigned long stepTimer;
  static unsigned long sleepTimer;

  if (micros() > stepTimer) {

    if (focusTargetPos != focusCurrPos) { //Move focus motor
      
      if (sleepTimer) { //Take motor driver out of sleep
        sleepTimer = 0;
        digitalWrite(focusSleepPin, LOW);
        stepTimer += focusMotorDelay;
      }
      
      if (focusCurrPos < focusTargetPos && !dirPinHigh) {digitalWrite(focusDirPin, HIGH); dirPinHigh = 1;}      //Moving out
      else if (focusCurrPos > focusTargetPos && dirPinHigh) {digitalWrite(focusDirPin, LOW); dirPinHigh = 0;}   //Moving in

      digitalWrite(focusStepPin, 1);
      if (focusTargetPos > 52000) focusTargetPos = 52000;           //Wasting some time
      else if (focusTargetPos < -52000) focusTargetPos = -52000;    //Wasting more time
      stepTimer += focusMotorDelay;
      if (dirPinHigh) focusCurrPos++;
      else focusCurrPos--;
      digitalWrite(focusStepPin, 0);
    }

    else {
      stepTimer += 500000;
      if (!sleepTimer) sleepTimer = millis()+ focusSleepDelay;                                                    //Not moving, sleep timer is zero
      else if (millis() > sleepTimer  && digitalRead(focusSleepPin)) {                              //Not moving, sleep timer has expired
        digitalWrite(focusSleepPin, HIGH);
      }
    }
  }
}
