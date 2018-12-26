boolean getCommand() {
  static byte i = 0;
  static char stringChar[20];
  boolean newCommand = false;
  COMstring = "";
  COMvalue = 0;
  if (Serial.available()) {
    char COMinput = Serial.read();
    if (COMinput == terminator) newCommand = true;
    else if (COMinput > 47) {
      stringChar[i] = COMinput;
      i++;
    }
    if (newCommand) {
      for (byte j = 0; j < i; j++) {
        if (stringChar[j] > 57) COMstring += String(stringChar[j]);
        else COMvalue = COMvalue * 10 + stringChar[j] - '0';
        stringChar[j] = 0;
      }
      i = 0;
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
  if (SerialSoft.available()) {
    char COMinput = SerialSoft.read();
    if (COMinput == terminator) newCommand = true;
    else if (COMinput > 47) {
      stringChar[i] = COMinput;
      i++;
    }
    if (newCommand) {
      for (byte j = 0; j < i; j++) {
        if (stringChar[j] > 57) COMstring += String(stringChar[j]);
        else COMvalue = COMvalue * 10 + stringChar[j] - '0';
        stringChar[j] = 0;
      }
      i = 0;
    }
  }
  return newCommand;
}



void parseCommand() {   //Serial Commander
  //ASCOM Driver Methods / Queries
  if (COMstring == "cd") setPos(1, COMvalue);
  else if (COMstring == "cr") setPos(0, COMvalue);
  else if (COMstring == "d") Serial.print("q" + String(decCurrPos()) + "/");
  else if (COMstring == "D"){
    Serial.print("q");
    unsigned long pos = decCurrPos();
    Serial.write(pos % 256);
    Serial.write((pos >> 8) % 256);
    Serial.write((pos >> 16) % 256);
    Serial.write((pos >> 24) % 256);
  }
  else if (COMstring == "r") Serial.print("q" + String(raPosArcsecs) + "/");
  else if (COMstring == "R") {
    Serial.print("q");
    Serial.write(raPosArcsecs % 256);
    Serial.write((raPosArcsecs >> 8) % 256);
    Serial.write((raPosArcsecs >> 16) % 256);
  }
  else if (COMstring == "td") decTargetPosition = (COMvalue + decCoorOffset) * decStepsPerArcsec;       //Set target DEC coordinates
  else if (COMstring == "H") returnIsHome();                   //Query is home
  else if (COMstring == "t") raMotorOn = COMvalue;                    //Tracking on/off
  else if (COMstring == "T") Serial.print(queryMotorOn() + "/");     //Query is tracking
  else if (COMstring == "tr") {
    raTargetArcsecs = COMvalue;            //RA slew to coordinate
    raSlewing = 1;
  }
  else if (COMstring == "td") decTargetPosition = (COMvalue + decCoorOffset) * decStepsPerArcsec;
  else if (COMstring == "sq") Serial.print(querySlewing() + "/");     //Query is slewing
  else if (COMstring == "sw") int i = 0;                        //Set slew settle time (s)
  else if (COMstring == "swq") int i = 0;                       //Query slew settle time (s)
  else if (COMstring == "mn") decMoveDir = 1;
  else if (COMstring == "ms") decMoveDir = -1;
  else if (COMstring == "mw") {
    raTargetArcsecs -= 30000;
    if (raTargetArcsecs < 0) raTargetArcsecs += raTotalArcsecs;
    raSlewing = 1;
  }
  else if (COMstring == "me") {
    raTargetArcsecs += 30000;
    if (raTargetArcsecs > raTotalArcsecs) raTargetArcsecs -= raTotalArcsecs;
    raSlewing = 1;
  }
  else if (COMstring == "md") decMoveDir = 0;
  else if (COMstring == "mr") {
    raTargetArcsecs = raPosArcsecs;
    raSlewing = 0;
  }
  else if (COMstring == "rr") convertDelay(COMvalue);     //Right ascension rate
  else if (COMstring == "rd") int i = 0;     //DEC rate
  else if (COMstring == "pn") setPulseGuide(1, 1, COMvalue);
  else if (COMstring == "ps") setPulseGuide(1, -1, COMvalue);
  else if (COMstring == "pw") setPulseGuide(0, 1, COMvalue);
  else if (COMstring == "pe") setPulseGuide(0, -1, COMvalue);
  else if (COMstring == "P") printIsPulseGuiding();
  else if (COMstring == "pgsr") raPulseRate = COMvalue;
  else if (COMstring == "pgsd") decPGrate = COMvalue;
  else if (COMstring == "pgqr") Serial.print(raPulseRate + "/");
  else if (COMstring == "pgqd") Serial.print(decPGrate + "/");
  else if (COMstring == "dd") decReverse = COMvalue;

  //Focus Motor
  else if (COMstring == "fq") {
    if (focusTargetPos == focusCurrPos) Serial.print("n/");
    else Serial.print("y/");
  }
  else if (COMstring == "fpq") {
    Serial.print(focusCurrPos);
    Serial.print("/");
  }
  else if (COMstring == "fh") {
    focusTargetPos = focusCurrPos;
    Serial.print("fh");
    Serial.print(focusCurrPos);
    Serial.print("/");
  }
  else if (COMstring == "f") focusTargetPos = COMvalue;

  //Debugging & Non-Driver Queries
  else if (COMstring == "info") displayInfo = 1;
  else if (COMstring == "sn") decTargetPosition += COMvalue * decStepsPerArcsec;
  else if (COMstring == "ss") decTargetPosition -= COMvalue * decStepsPerArcsec;
  else if (COMstring == "rap") raStepPosition = COMvalue * 7500;
  else if (COMstring == "ins") decSlewRate = COMvalue;
  else if (COMstring == "ras") raSlewRate = COMvalue;
  else if (COMstring == "eso") ignoreRAEndstop();

  else if (COMstring == "C") Serial.print(conMsg);

  else {
    Serial.print(F(" Command not recognized: "));
    Serial.println(COMstring);
  }
  Serial.println();
}

void parseCommandSoft() {   //Serial Commander
  //ASCOM Driver Methods / Queries
  static unsigned int raDistance = 10;
  static unsigned int decDistance = 100;
  
  if (COMstring == "w") {
    raTargetArcsecs -= raDistance;
    if (raTargetArcsecs < 0) raTargetArcsecs += raTotalArcsecs;
    raSlewing = 1;
  }
  else if (COMstring == "e") {
    raTargetArcsecs += raDistance;
    if (raTargetArcsecs > raTotalArcsecs) raTargetArcsecs -= raTotalArcsecs;
    raSlewing = 1;
  }
  else if (COMstring == "n") decTargetPosition = decCurrPos() + decDistance * decStepsPerArcsec;
  else if (COMstring == "s") decTargetPosition = decCurrPos() - decDistance * decStepsPerArcsec;
  else if (COMstring == "r") raDistance = COMvalue;
  else if (COMstring == "d") decDistance = COMvalue;
  else parseCommand();
 
}


void printDebugInfo() {
  long temp;
  switch (displayInfo) {
    case 1:
      Serial.println(F("Millis: "));
      break;
    case 2:
      Serial.println(millis());
      break;
    case 3:
      Serial.print(" ");
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
      Serial.print(F("raPosArcsecs: "));
      Serial.println(raPosArcsecs);
      break;
    case 26:
      Serial.print(F("raTargetArcsecs: "));
      Serial.println(raTargetArcsecs);
      break;
    case 27:
      Serial.print(F("raSlewing: "));
      Serial.println(raSlewing);
      break;
    case 28:
      Serial.print(F("dec Target Position: "));
      Serial.println(decTargetPosition);
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
      break;
    case 37:
      Serial.print("RA Clock / Step Time Offset: ");
      break;
    case 38:
      break;
    case 39:
      Serial.print("raCoorOffset ");
      break;
    case 40:

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
