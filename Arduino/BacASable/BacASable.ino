/*
  Filename: BacASable.ino
  Author: Tristan Beau ( tristan.beau@univ-paris-diderot.fr )
  Date: November 2017
  Description: Some trials...
*/
#include "Manager.h"

enum ledColor { green, yellow, red };
const int ledPin[] = {10,11,12} ;
int gState=0;

String inputString = "";         // a string to hold incoming data
boolean stringComplete = false;  // whether the string is complete

void (*softReset) (void) = 0; //declare reset function at address 0

void setLed(enum ledColor col, int v) {
  digitalWrite(ledPin[col],v);
  gState &= v << col;
}

Manager m;

// --- Init----- -------------------------------------------------
void setup(void)
{
  int i;
  
  Serial.begin(9600); //init serial comm
  inputString.reserve(200);
   
  pinMode(LED_BUILTIN,OUTPUT); //init of ledPin
  for (i=green; i<=red; i++) {
    pinMode(ledPin[i],OUTPUT);
    setLed((enum ledColor)i,LOW);
  }

  delay(200); //wait a bit
}


// --- Main loop -------------------------------------------------
void loop(void)
{
   if (stringComplete) {
    m.go();
    Serial.println(m.truc);
    if ( ! strncmp ( inputString.c_str(), "SET ", 4 ) ) {
      Serial.println("ACK SET");
    }
    else if ( !strncmp ( inputString.c_str(), "GET ", 4 ) ) {
      Serial.println("ACK GET");
      char b[500];
      char *str,*p;
      p=b;
      sprintf(b,"%s",inputString.c_str());
      while ((str = strtok_r(p, " ", &p)) != NULL) // delimiter is the semicolon
        Serial.println(str);
    }
    else if ( !strncmp ( inputString.c_str(), "RST", 3 ) ) {
      Serial.println("ACK RESET");
      softReset();
    }
    else {
      Serial.println("UKN");
    }
    // clear the string:
    inputString = "";
    stringComplete = false;
  }
}

void serialEvent() {
  while (Serial.available()) {
    // get the new byte:
    char inChar = (char)Serial.read();
    // add it to the inputString:
    inputString += inChar;
    // if the incoming character is a newline, set a flag
    // so the main loop can do something about it:
    if (inChar == '\n') {
      stringComplete = true;
      inputString.toUpperCase();
    }
  }
}


