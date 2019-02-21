
/**
 * @file      Service.cpp
 * @Author    Tristan Beau ( tristan.beau@univ-paris-diderot.fr )
 * @date      November, 2017
 * @brief     Class implementation for Green House services
 * 
 * Detailed description
 * 
 * Nota : here we define the maximum managed sensors.
 * 
 */

#include "Service.h"

// --- init of members -------------------------------------------
void (*Service::softReset)(void)        = 0;  // function pointer to NULL address
Service* Service::sm_instance           = 0;  // instance init on demand
const int Service::sm_maxSensorCnt      = 32;
const int Service::sm_maxActuatorCnt    = 16;
const int Service::sm_maxStringLength   = 63;   

// --- Creator ------------------------------------------------------------
Service::Service() {
  Serial.begin(9600); // init serial comm'  
  delay(100);
  Wire.begin();
  delay(500);
  Serial.println("");
  Serial.println("GreenHouse Service Manager started. Waiting for commands now.");
  delay(5);
  wdt_enable(WDTO_4S);
  //pinMode(LED_BUILTIN,OUTPUT);
  m_sensorCnt = 0;
  m_sensorArray = new Sensor* [sm_maxSensorCnt];
  m_actuatorCnt = 0;
  m_actuatorArray = new Actuator* [sm_maxActuatorCnt];
}

// --- getInstance() -------------------------------------------------------
Service* Service::getInstance() {
  if ( ! sm_instance ) {
    sm_instance = new Service(); 
  }
  return sm_instance;
}

// --- doLoop() ------------------------------------------------------------
void Service::doLoop() {
  static int i=0;
  if (m_stringComplete) {
    analyzeCommand();
    m_inputString = "";
    m_stringComplete = false;
  }
  wdt_reset();
/*  digitalWrite(LED_BUILTIN,HIGH);
  delay(200);
  digitalWrite(LED_BUILTIN,LOW);
  delay(200);*/
}

// --- doSerialEvent() : read serial input if any ---------------------------
void Service::doSerialEvent() {
  while (Serial.available()) {
    // get the new byte:
    char inChar = (char)Serial.read();
    // add it to the inputString:
    m_inputString += inChar;
    // if the incoming character is a newline, set a flag
    // so the main loop can do something about it:
    if (inChar == '\n') {
      m_stringComplete = true;
      m_inputString.toUpperCase();
    }
  }
}

// --- Service::printAll ----------------------------------------------------
void Service::printAll() {
  int i;
  String s="\n  Connected devices ... \n";
  s += " * Found " ; s+= m_sensorCnt ; s+= " sensors :\n";
  for (i=0;i<m_sensorCnt;i++) {
    s += "Sensor ";
    s += i ;
    s += " (" ;
    s += getSensor(i)->getID();
    s += ") : " ;
    s += getSensor(i)->getValue();
    s += "\n";
  }
  s += " * Found " ; s+= m_actuatorCnt; s+= " actuators :\n";
  for (i=0;i<m_actuatorCnt;i++) {
    s += "Actuator " ;
    s += i ;
    s += " (";
    s += getActuator(i)->getID() ;
    s += ") \n";
  }
  Serial.println(s);
}

// --- Service::addSensor ---------------------------------------------------
void Service::addSensor(Sensor *s) {
  if (m_sensorCnt < sm_maxSensorCnt) 
    m_sensorArray[m_sensorCnt++] = s;
   else 
    abort();
}

// --- Service::getSensorCnt ------------------------------------------------
int Service::getSensorCnt() const {
  return m_sensorCnt;
}

// --- Service::getSensor(int) ----------------------------------------------
Sensor* Service::getSensor(int i) const {
  if ( i >= getSensorCnt() ) return NULL;
  return m_sensorArray[i];
}

// --- Service::getSensor(String) -------------------------------------------
Sensor* Service::getSensor(String s) const {
  int i=0;
  while (i<getSensorCnt()){
    if ( s == m_sensorArray[i]->getID() ) break;
    i++;
  }
  return getSensor(i);
}


// --- Service::addActuator ---------------------------------------------------
void Service::addActuator(Actuator *s) {
  if (m_actuatorCnt < sm_maxActuatorCnt) 
    m_actuatorArray[m_actuatorCnt++] = s;
   else 
    abort();
}

// --- Service::getSensorCnt ------------------------------------------------
int Service::getActuatorCnt() const {
  return m_actuatorCnt;
}

// --- Service::getSensor(int) ----------------------------------------------
Actuator* Service::getActuator(int i) const {
  if ( i >= getActuatorCnt() ) return NULL;
  return m_actuatorArray[i];
}

// --- Service::getSensor(String) -------------------------------------------
Actuator* Service::getActuator(String s) const {
  int i=0;
  while (i<getActuatorCnt()){
    if ( s == m_actuatorArray[i]->getID() ) break;
    i++;
  }
  return getActuator(i);
}
// --- Service::analyzeCommand() --------------------------------------------
void Service::analyzeCommand() {
  char *word;
  int i=0;
  boolean bContinue=true, bGet=false, bSet=false;
  char *s = new char[sm_maxStringLength];
  Actuator *lA = NULL;
  Sensor *lS = NULL;
  
  snprintf(s,sm_maxStringLength,"%s",m_inputString.c_str());
  s[strlen(s)-1]='\0'; //suppress the ending \n
 
  while ( (word = strtok_r(s," ",&s)) != NULL ) { // split command line in words
    switch(i++) {
      case 0: // first word analysis
        if      ( ! strncmp(word,"SET",3) ) bSet=true;
        else if ( ! strncmp(word,"GET",3) ) bGet=true;
	      else if ( ! strncmp(word,"ALL",3) ) { printAll(); bContinue=false; }
        else if ( ! strncmp(word,"RESET",3) ) softReset();
        else {
          Serial.println("UNK");
          bContinue=false;
        }
        break;
      case 1: // second word analysis        
        if (bGet) {
          lS = getSensor(word);
          if ( lS == NULL ) { // that sensor does not exist
            Serial.println("ERR");
            bContinue = false;
            break;
          }
          Serial.print("ACK "); 
          Serial.println(lS->getValue(),3); // 3 digit afeter decimal separator          
        }
        else if (bSet) {
          lA = getActuator(word);
          if ( lA == NULL ) { // that sensor does not exist
            Serial.println("ERR");
            bContinue = false;
            break;
          }
          // and then needs 3rd arg
        }
        break;
      case 2: //third word analysis
        if (bSet) {
          lA->setValue( (float) atof(word) );
          Serial.println("ACK");
        }
      default:
        break;//Serial.println("Not implemented yet");
    }
    if ( !bContinue ) break;
  }
}
