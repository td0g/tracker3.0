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
  static unsigned int cycleCounter;

  if (millis() > tmr){
    if (!cycleCounter){
      if (voltage() < 100) cycleCounter = 2;
      else if (voltage() < 120) cycleCounter = 4;
      else if (voltage() < 140) cycleCounter = 6;
      else cycleCounter = 8;
    }
    cycleCounter--;
    if (cycleCounter & 0b00000001){
      digitalWrite(indPin, 1);
      tmr += 50;
    }
    else {
      digitalWrite(indPin, 0);
      tmr += 250;
      if (!cycleCounter) tmr += 4500;
    }
  }
}
