/**
 * @file      Service.h
 * @Author    Tristan Beau ( tristan.beau@univ-paris-diderot.fr )
 * @date      November, 2017
 * @brief     Class definition for Green House services
 * 
 * Detailed description
 * 
 * 
 */
 
#ifndef _SERVICE_H_
#define _SERVICE_H_

#include <Arduino.h>                    //to get Serial object
#include <Wire.h>
#include <avr/wdt.h>

#include "Sensor.h"
#include "Actuator.h"
#include <I2C_LCD.h>
#define HFILL_LINE "                                \n"

class Service {                             // nota : this is a singleton class
  private:
    static Service*   sm_instance;          // storage
    Service(int);                              // init, private constructor to prevent instancing

  public:
    static Service*   getInstance(int wd_pin = LED_BUILTIN);        // static access method
    void              doSerialEvent();      // inside the serialEvent main function
    void              doLoop();             // inside the loop main function
    static void     (*softReset)(void);     // reset function at address 0
    void              printAll();           // serial print of everything (!)
    void              printHelp();          // serial print the help of commands

    void              addSensor(Sensor *) ; 
    int               getSensorCnt() const ;
    Sensor*           getSensor(int) const; 
    Sensor*           getSensor(String)const; //retrieve it by string name

    void              addActuator(Actuator *) ; 
    int               getActuatorCnt() const ;
    Actuator*         getActuator(int) const; 
    Actuator*         getActuator(String)const; //retrieve it by string name

    void              initLCD();
    
  private:
    String            m_inputString ;       // a string to hold incoming data
    String            m_lastInputString ;   // a last command received
    String            m_lastStatusString ;  // a string with status
    
    boolean           m_stringComplete ;    // whether the string is complete
    const static int  sm_maxStringLength;
    
    Sensor**          m_sensorArray;        // array of pointer of sensors
    int               m_sensorCnt;          // nb of sensors  
    const static int  sm_maxSensorCnt;      // max nb of sensors

    Actuator**        m_actuatorArray;        // array of pointer of actuators
    int               m_actuatorCnt;          // nb of sensors  
    const static int  sm_maxActuatorCnt;      // max nb of sensors

    int               m_pinWdLed;
    int               m_WdLedValue;

    static const unsigned long     sm_delayDeepLoop; // delay in milliseconds in the main deep loop (related to wd reset)
    static const int  sm_loopsBtwDisplayUpdates; // nb of loop between update of sensors on lcd display
    static const int  sm_loopsBtwLCDClean;

    I2C_LCD           m_LCD;
   
  private:
    void              analyzeCommand(); 
};

#endif
