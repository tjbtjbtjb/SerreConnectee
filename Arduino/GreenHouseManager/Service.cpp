
/**
 * @file         Service.cpp
 * @Author       Tristan Beau ( tristan.beau@univ-paris-diderot.fr )
 * @date         November, 2017
 * @LastRevDate  May, 2019
 * @brief        Class implementation for Green House services
 * 
 * Detailed description
 * 
 * Nota : here we define the maximum managed sensors.
 * 
 */
 
#include "Service.h"

#include <EEPROM.h>
struct NameObject {
  byte h1,h2;
  char string[12];
};

// --- init of members -------------------------------------------
void (*Service::softReset)(void)        = 0;  // function pointer to NULL address
Service* Service::sm_instance           = 0;  // instance init on demand
const int Service::sm_maxStringLength   = 63; 

// --- Creator ------------------------------------------------------------
Service::Service(int wd_pin) {
  // Comm' init
  delay(500);
  Serial.begin(9600); // init serial comm'  
  delay(100);
  Wire.begin();
  delay(500);
  Serial.println("");
  Serial.println("GreenHouse Service Manager started. Waiting for commands now.");
  m_inputString = "";
  m_stringComplete = false;
  m_lastInputString = "RESET"; // used for LCD info
  m_lastStatusString = "ACK INIT OK";
  Serial.println(m_lastStatusString);
  delay(5);

  // retreive in EPROM the board name...
  NameObject n;
  EEPROM.get(128,n);
  if ( strlen(n.string) > 0) n.string[strlen(n.string)-1]='\0';
  m_stringInitLCD = String((n.h1 == 75 && n.h2 == 13)?n.string:"GreenHouse") + String(" v") + String(sm_softVersion); 
  //Service::m_stringInitLCD = "Arduino0 v" xstr(SOFTWARE_VERSION) ; //" \\(^-^)/  Serre USPC" ; // see e.g. http://1lineart.kulaone.com/#/
  
  // About WD
  wdt_enable(WDTO_8S);  // 4S ? 
  m_pinWdLed = wd_pin;
  pinMode(m_pinWdLed,OUTPUT);
  m_WdLedValue = LOW;
  digitalWrite(m_pinWdLed,m_WdLedValue);

  //LCD init 
  initLCD();
  delay(200);

  //Keypad init
  mp_keyBoard = new Keypad(makeKeymap(sm_keys),sm_pinLine,sm_pinColumn,sm_keypad_lines,sm_keypad_columns);
  m_kindex = 0;
 
  // About sensors
  m_sensorCnt = 0;
  m_sensorArray = new Sensor* [sm_maxSensorCnt];
  m_actuatorCnt = 0;
  m_actuatorArray = new Actuator* [sm_maxActuatorCnt];
  m_alarmCnt = 0;
  m_alarmArray = new Alarm* [sm_maxAlarmCnt];
  mp_mainAlarm = 0;
}

void Service::initLCD() {
//LCD init 
  m_LCD.CleanAll(WHITE);    //Erase all
  m_LCD.WorkingModeConf(OFF, ON, WM_CharMode);  // Logo OFF, ScreenLight ON
  m_LCD.CleanAll(WHITE);    //Erase all
  m_LCD.CharGotoXY(0, 0);      //Set the start coordinate.
  m_LCD.FontModeConf(Font_6x12, FM_MNL_AAA, WHITE_BAC);
  //m_LCD.print("  ==> Serre USPC <==");m_LCD.print(HFILL_LINE);
  //Serial.println(m_stringInitLCD);
  m_LCD.print(m_stringInitLCD + HFILL_LINE);
  m_LCD.FontModeConf(Font_6x8, FM_MNL_AAA, BLACK_BAC); // manual newline  
}

// --- getInstance() -------------------------------------------------------
Service* Service::getInstance(int wd_pin)  {
  if ( ! sm_instance ) {
    sm_instance = new Service(wd_pin); 
  }
  return sm_instance;
}

// --- doLoop() ------------------------------------------------------------
void Service::doLoop() {
  static int i=0,j=0;
  int k,x;
  char key;

  if (m_stringComplete) {
    analyzeCommand();
    m_inputString = "";
    m_stringComplete = false;
  }

  //keyPad read --------------------------
  key = mp_keyBoard->getKey();
  if (key != NO_KEY){
    if(key=='*') { //force rewrite data
      i=0; 
    }
    if(key=='#') { //force init LCD
      j=0;
    }
    if(key=='A') { // display on LCD alarms
      m_LCD.CharGotoXY(0,13);
      m_LCD.print("  Alarm status ");m_LCD.print(allAlarmsStatus());
      for (k=0; k<getAlarmCnt(); k++) {
        m_LCD.print(getAlarm(k)->getID());m_LCD.print("[");m_LCD.print(k);m_LCD.print("]");m_LCD.print(" -> ");m_LCD.println(getAlarm(k)->getLastValue());
      }
    }
    if(key=='B') { // display on LCD sensors; and clear keypad buffer
      m_LCD.CharGotoXY(0,13); 
      for(k=m_alarmCnt;k<m_sensorCnt;k++) { // the first m_alarmCnt are pseudo sensors…
          m_LCD.print(getSensor(k)->getID());m_LCD.print("[");m_LCD.print(k-m_alarmCnt);m_LCD.print("] ");
          if (k%2) m_LCD.print(HFILL_LINE); // impression 2 par ligne
      }
    }
    if( m_kindex>0 && (key=='C' || key=='D') ) { // either display the sensor (D) or clear it (C)
      int id;
      if (m_kindex==1) id = m_kbuf[0];
      if (m_kindex==2) id = m_kbuf[0]*10+m_kbuf[1];
      if (id<m_sensorCnt-m_alarmCnt) { //ignore the command if not
        if (key=='C') { //clear
          getSensor(id+m_alarmCnt)->setDisplay(0);
         }
        else { //then display
          getSensor(id+m_alarmCnt)->setDisplay(1);
        }
        i=0; // force rewrite data
      }
      m_kindex=0;
    }
    if(key>='0' && key<='9') {// do something clever too (!)
      if(m_kindex >= 2) m_kindex=0;
      m_kbuf[m_kindex++]=(int)(key-'0');
    }
    else m_kindex=0;
  }     

  // clean LCD
  if ( !j) initLCD();
   
  // sensor value update and screen display
  if ( !i ) {
    m_LCD.CharGotoXY(0,13);      //Set the start coordinate.
    // print last command and status
    m_LCD.print("@");m_LCD.print(m_lastInputString);m_LCD.print(char(29));m_LCD.print(m_lastStatusString);m_LCD.print(HFILL_LINE);
    for (k=0,x=0;k<m_sensorCnt;k++) {
      String vs = getSensor(k)->getLastValueAsString();
      // display on LCD screen
      if ( getSensor(k)->getDisplay() == true && x<5 ) { // (only 5 lines available)
          m_LCD.print(getSensor(k)->getID());m_LCD.print(": " HFILL_LINE);
          m_LCD.CharGotoXY(55,(21+8*x++)%60); // align value display
          m_LCD.print(vs);m_LCD.print(HFILL_LINE);
      }
    }
    for (k=x;k<5;k++) m_LCD.print(HFILL_LINE); // to blank screen after last displayed sensor info
    
    if ( mp_mainAlarm ) mp_mainAlarm->setValue( (allAlarmsStatus())?1:0 );
         
    // if in manual mode, force the state of the actuator to the sensor value. Assume sensor name = actuator name
    if ( ! getSensor("AUTO")->getLastValue() ) {
      for (k=0;k<m_actuatorCnt;k++) {
        Sensor *s=0; 
        s=getSensor(getActuator(k)->getID());
        if ( s ) {
          getActuator(k)->setValue(s->getLastValue());
        }
      }
    }
  }

  if ( i++ > sm_loopsBtwDisplayUpdates ) i=0; // roughly 5 seconds if delay(200) in wd stuff just below

  if ( j++ > sm_loopsBtwLCDClean) j=0;

  //wd stuff
  delay(sm_delayDeepLoop);
  m_WdLedValue = ! m_WdLedValue;
  digitalWrite(m_pinWdLed,m_WdLedValue);
  wdt_reset();
}

// --- doSerialEvent() : read serial input if any ---------------------------
void Service::doSerialEvent() {
  char inChar;
  while (Serial.available()) {
    // get the new byte:
    inChar = (char)Serial.read();
    // add it to the inputString:
    m_inputString += inChar;
    // if the incoming character is a newline, set a flag
    // so the main loop can do something about it:
    if (inChar == '\n') {
      m_stringComplete = true;
      m_inputString.toUpperCase();
    }
  }
  delay(10);
}

// --- Service::printAll ----------------------------------------------------
void Service::printAll() {
  int i,x;
  String s="\n  Connected devices ... \n";
  s += " * Found " ; s+= m_sensorCnt ; s+= " sensors :\n";
  for (i=0;i<m_sensorCnt;i++) {
    s += "Sensor ";
    s += i ;
    s += " (" ;
    s += getSensor(i)->getID();
    s += ") : " ;
    s += getSensor(i)->getLastValue();
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
  s += " * Displayed sensors on LCD screen (only first 5 are actually displayed) :\n";
  for (i=0,x=0;i<m_sensorCnt;i++) 
   if (getSensor(i)->getDisplay() ) {
    if (x>=5) s+= "[";
    s += "Display ";
    s += x ;
    s += " (" ;
    s += getSensor(i)->getID();
    s += ") : precision " ;
    s += getSensor(i)->getPrecision();
    if (x++>=5) s+= "]";
    s += "\n";
  }
  Serial.println(s);
  m_lastStatusString = "ACK ALL";
}

void Service::printHelp() {
  String s="\n Arduino command help ... \n\n";
  s+= " HELP : this help.\n";
  s+= " ALL : reply with all sensor list and values, and all actuator list.\n";
  s+= " GET <sensor name> : get value of the sensor.\n";
  s+= " DISPLAY <sensor name> <0|1> : display (1) or not (0) value on the LCD screen.\n";
  s+= " SET <actuator name> <value> : set value for the specified actuator.\n";
  s+= " RESET : send a software reset.\n\n";
  s+= "If a command is correctly understood, reply with a line begining with ACK.\n";
  s+= "The line may contain the ERR string if an error occurs.\n\n";
  Serial.println(s);
  m_lastStatusString = "ACK HELP";
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

// --- Service::getActuatorCnt ------------------------------------------------
int Service::getActuatorCnt() const {
  return m_actuatorCnt;
}

// --- Service::getActuator(int) ----------------------------------------------
Actuator* Service::getActuator(int i) const {
  if ( i >= getActuatorCnt() ) return NULL;
  return m_actuatorArray[i];
}

// --- Service::getActuator(String) -------------------------------------------
Actuator* Service::getActuator(String s) const {
  int i=0;
  while (i<getActuatorCnt()){
    if ( s == m_actuatorArray[i]->getID() ) break;
    i++;
  }
  return getActuator(i);
}

// --- Service::addAlarm ---------------------------------------------------
void Service::addAlarm(Alarm *s) {
  if (m_alarmCnt < sm_maxAlarmCnt) {
    m_alarmArray[m_alarmCnt++] = s;
    addSensor(s);
  }
   else 
    abort();
}

// --- Service::getAlarmCnt ------------------------------------------------
int Service::getAlarmCnt() const {
  return m_alarmCnt;
}

// --- Service::getAlarm(int) ----------------------------------------------
Alarm* Service::getAlarm(int i) const {
  if ( i >= getAlarmCnt() ) return NULL;
  return m_alarmArray[i];
}

// --- Service::getAlarm(String) -------------------------------------------
Alarm* Service::getAlarm(String s) const {
  int i=0;
  while (i<getAlarmCnt()){
    if ( s == m_alarmArray[i]->getID() ) break;
    i++;
  }
  return getAlarm(i);
}

// --- compute alarm status ------------------------------------------------
unsigned long Service::allAlarmsStatus() const {
  int i;
  unsigned long s = 0;
  for (i=0;i<getAlarmCnt();i++) 
    if ( getAlarm(i)->getLastValue() ) 
      s = s | (0x1<<i);
  
  return s;
}

// --- Service::analyzeCommand() --------------------------------------------
void Service::analyzeCommand() {
  char *word;
  int i=0;
  boolean bContinue=true, bGet=false, bSet=false, bDisp=false;
  char *s;
  char s0[sm_maxStringLength];
  Actuator *lA = NULL;
  Sensor *lS = NULL;

  s = (char*)s0;
  snprintf(s,sm_maxStringLength,"%s",m_inputString.c_str());
  s[strlen(s)-1]='\0'; //suppress the ending \n
  m_lastInputString=s;

  m_lastStatusString = "ACK BAD CMD";
  while ( (word = strtok_r(s," ",&s)) != NULL ) { // split command line in words
    switch(i++) {
      case 0: // first word analysis
        if      ( ! strncmp(word,"SET",3) ) bSet=true;
        else if ( ! strncmp(word,"DISPLAY",3) ) bDisp=true;
        else if ( ! strncmp(word,"GET",3) ) bGet=true;
	      else if ( ! strncmp(word,"ALL",3) ) { printAll(); bContinue=false; }
        else if ( ! strncmp(word,"HELP",3) ) { printHelp(); bContinue=false; }
        else if ( ! strncmp(word,"RESET",3) ) softReset();
        else {
          m_lastStatusString = "ACK UNK CMD";
          bContinue=false;
        }
        break;
      case 1: // second word analysis        
        if (bGet) {
          lS = getSensor(word);
          if ( lS == NULL ) { // that sensor does not exist
            m_lastStatusString = "ACK ERR SENSOR";
            bContinue = false;
            break;
          }
          m_lastStatusString = "ACK " + String(lS->getLastValue(), 3);// + " [" + lS->getLastValueAsString() + "]" ; 
          bContinue = false;
        }
        else if (bSet) {
          lA = getActuator(word);
          if ( lA == NULL ) { // that actuator does not exist
            m_lastStatusString = "ACK ERR ACTUATOR";
            bContinue = false;
            break;
          }
          // and then needs 3rd arg
        }
        else if (bDisp) {
          lS = getSensor(word);
          if ( lS == NULL ) { // that sensor does not exist
            m_lastStatusString = "ACK ERR SENSOR";
            bContinue = false;
            break;
          }
          // and then needs 3rd arg
        }
        break;
      case 2: //third word analysis
        if (bSet) {
          lA->setValue( (float) atof(word) );
          m_lastStatusString = "ACK SET OK" ;
        }
        else if (bDisp) {
          lS->setDisplay( (int)atoi(word)?true:false );
          m_lastStatusString = "ACK DISP OK";
        }
        else {
          m_lastStatusString = "ACK LOST";
        }
        break;
      default:
        m_lastStatusString = "ACK LOST";
        break;
    }
    if ( !bContinue ) break;
  }
  Serial.println(m_lastStatusString.c_str());
  Serial.println(" ");
  Serial.flush();
}
