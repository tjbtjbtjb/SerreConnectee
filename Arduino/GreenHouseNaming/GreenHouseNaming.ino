
/**
 * @file         GreenHouseNaming.cpp
 * @Author       Tristan Beau ( tristan.beau@univ-paris-diderot.fr )
 * @date         May, 2019
 * @LastRevDate  May, 2019
 * @brief        Set the name of a given board into the EEPROM
 * 
 * Detailed description
 * 
 * ...
 * 
 */
 
#include <EEPROM.h>

#define ADDRESS_OFFSET 128

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  while (!Serial) {
    delay(100);
    ; // wait for serial port to connect.
  }
}

struct NameObject {
  byte h1,h2;
  char string[12];
};

void loop() {
  bool b=true;
  NameObject n;
  String s ;
  
  n.h1=75;
  n.h2=13;

  while(b) {
    s=Serial.readString();
    if (s != NULL) {
      snprintf(n.string,12,"%s",s.c_str());
      EEPROM.put(ADDRESS_OFFSET,n);
      Serial.println("done");
      b=false;
    }
    else {
      Serial.println("Waiting...");
      delay(2000);
    }
  }

  while(1) {
    Serial.println("do nothing now...");
    delay(10000);
  }
}
