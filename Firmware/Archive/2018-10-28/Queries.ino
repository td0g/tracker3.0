void setPos(byte axis, long pos) {     //axis 0 = RA, 1 = DEC
  if (!axis){
    if (raSlewing) raTargetArcsecs = raTargetArcsecs - raPosArcsecs + pos;
    else raTargetArcsecs = pos;
    raPosArcsecs = pos;
  }
  else  decCoorOffset =  decStepPosition / decStepsPerArcsec - pos;     //DEC
}

long decCurrPos() {
  static unsigned long pos;
    pos = decStepPosition;
    pos /= decStepsPerArcsec;
    pos -= decCoorOffset;
    return pos;
}

void returnIsHome() {
  if (raStepPosition == 2) Serial.print("y/");
  else Serial.print("n/");
}

void setPulseGuide(byte axis, char dir, unsigned long timer) {
  if (!axis) {
    raPGdir = dir;
    raPGtimer = millis() + timer;
  }
  else {
    decPGdir = dir;
    decPGtimer = millis() + timer;
  }
}

void printIsPulseGuiding() {
  if (millis() > decPGtimer && millis() > raPGtimer) Serial.print("n/");
  else Serial.print("y/");
}

String queryMotorOn() {
  if (raMotorOn) return "y";
  else return "n";
}

String querySlewing() {
  if (raSlewing) return "y";
  else return "n";
}
