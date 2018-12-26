byte voltage() {                           //Returns battery voltage in 0.1v (120 = 12.0v)
  unsigned int v = analogRead(voltagePin);
  v *= 30;
  v /= 46;
  return v;
}




void readController() {
  static unsigned long timer;
  static unsigned int input;

  input = analogRead(controllerPin);
  if (input > 500) {
    if (!timer) timer = millis();
  }
  else if (timer) {
    timer = 0;
    unsigned long buttonTime = millis() - timer;
  }
}



void ignoreRAEndstop(){
  if (RA_ENDSTOP){
    pinMode(raEndStopPin, OUTPUT);
    digitalWrite(raEndStopPin, 0);
  }
}



void runIndicatorLED() {
  static unsigned long tmr;
  static byte onoff;
  static unsigned int cycleCounter;

  if (cycleCounter == 10000) {
    if (voltage() < 100) {
      indDelay = 250;
    }
    else if (!raMotorOn) indDelay = 1000;
    else indDelay = 10000;
    cycleCounter = 0;
  }
  cycleCounter++;

  if (millis() > tmr) {
    if (onoff && millis() > tmr) {
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
