
/**
 * @file      GreenHouseManager.h
 * @Author    Tristan Beau ( tristan.beau@univ-paris-diderot.fr )
 * @date      November, 2017
 * @brief     Main source for the Green House management code
 * 
 * Detailed description
 * 
 * 
 */
  
#include "Service.h"

#include "LiveTimeInfo.h"
#include "DummySensor.h"
#include "VoltageSensor.h"
#include "DigitalSensor.h"
#include "LightSensor.h"
#include "TemperatureSensor.h"
#include "AirHumiditySensor.h"
//#include "O2Sensor.h"
//#include "CO2GroveSensor.h"
//#include "CO2Sensor.h"
//#include "TemperatureCO2Sensor.h"
#include "CO2PulseSensor.h"
#include "ThermoCoupleSensor.h"

//#include "DummyActuator.h"
#include "DigitalActuator.h"
#include "MotorActuator.h"
//#include "Buzzer.h"

Service          *pSvc;

#define A_TEST 0
#define A_GREEN 1
// Which arduino ? 
//#define MY_ARDUINO A_GREEN
#define MY_ARDUINO A_TEST

#if MY_ARDUINO == A_TEST 
// Live time
LiveTimeInfo           lLiveTimeInfo("LIVETIME");
DummySensor            lDummySensor("DUMMY");
// Sensors
//VoltageSensor        lVoltageSensor("VOLT",A2);
//TemperatureSensor    lTemperatureSensor("TEMP",A0);
//AirHumiditySensor    lAirHumiditySensor("HUMIDITY",&lTemperatureSensor);
//LightSensor          lLightSensor("LIGHT",A4);
//O2Sensor             lO2Sensor("O2RATE",A12);
//CO2GroveSensor       lCO2Sensor("CO2PPM",A8,A9);
//TemperatureCO2Sensor lTempCO2Sensor("TEMP2",&lCO2Sensor);
DigitalSensor          lBit23("AUTO",23);  //is auto positive logic
DigitalSensor          lBit25("HEAT",25,1);  //Chauffe negative logic
DigitalSensor          lBit27("FAN",27,1);   //Fan negative logic
DigitalSensor          lBit29("EAU",29,1);   //EV Eau negative logic
DigitalSensor          lBit31("BRUME",31);   //Brume positive logic
DigitalSensor          lBit33("LUX",33);     //Lumiere positive logic 
DigitalSensor          lBit35("ALARM",35);   //
DigitalSensor          lBit37("Butee",37,1); //butee negative logic

#elif MY_ARDUINO == A_GREEN // Green house arduino -----------------------------------

LiveTimeInfo         lLiveTimeInfo("LIVETIME");
// Sensors

LightSensor          lLightSensor("LUX",A3);
TemperatureSensor    lIntTemperatureSensor("INTTEMP",A2);
TemperatureSensor    lOutTemperatureSensor("OUTTEMP",A15); 
AirHumiditySensor    lIntAirHumiditySensor("INTAIRHR",&lIntTemperatureSensor);
AirHumiditySensor    lOutAirHumiditySensor("OUTAIRHR",&lOutTemperatureSensor);
VoltageSensor        lGroundHumiditySensor("GNDHR",A1);   //in volts
DigitalSensor        lStopStepperSensor("STOPSTEP",9);
VoltageSensor        lThermalFlux("FLUX",A0,1./400./12.1e-6);   // W / m2
ThermoCoupleSensor   lThermoCouple("THERMO",10);
CO2PulseSensor       lCO2Sensor("CO2PPM",2);

// Dual Digital Actuator & Sensor 
DigitalSensor        lAutoMode("AUTO",23);

DigitalActuator      lAlarm("ALARM",A8,0);
DigitalSensor        lLedAlarm("ALARM",35);

DigitalActuator      lFan("FAN",5,0,-1);
DigitalSensor        lLedFan("FAN",27,1);

DigitalActuator      lWater("WATER",6,0,-1);
DigitalSensor        lLedWater("WATER",29,1);

DigitalActuator      lMist("MIST",7,0);
DigitalSensor        lLedMist("MIST",31);

DigitalActuator      lLight("LIGHT",8,0);
DigitalSensor        lLedLight("LIGHT",33);

DigitalActuator      lHeat("HEAT",4,0,-1);
DigitalSensor        lLedHeat("HEAT",25,1);

//MotorActuator        lMotor("MOTOR",0x0f);

#endif

void setup() {
  pSvc = Service::getInstance(13); // 24, the pin for the WD LED. If no arg, flashes the internal led.

#if MY_ARDUINO == A_TEST
  pSvc->addSensor(&lLiveTimeInfo);
  pSvc->addSensor(&lDummySensor);
  //pSvc->addSensor(&lVoltageSensor);
  //pSvc->addSensor(&lLightSensor);
  //pSvc->addSensor(&lTemperatureSensor);
  //pSvc->addSensor(&lAirHumiditySensor);
  //pSvc->addSensor(&lO2Sensor);
  //pSvc->addSensor(&lCO2Sensor);
  //pSvc->addSensor(&lTempCO2Sensor);

  pSvc->addSensor(&lBit23);
  pSvc->addSensor(&lBit25);
  pSvc->addSensor(&lBit27);
  pSvc->addSensor(&lBit29);
  pSvc->addSensor(&lBit31);
  pSvc->addSensor(&lBit33);
  pSvc->addSensor(&lBit35);
  pSvc->addSensor(&lBit37);
  
#elif MY_ARDUINO == A_GREEN
  pSvc->addSensor(&lLiveTimeInfo);
  pSvc->addSensor(&lLightSensor);
  pSvc->addSensor(&lIntTemperatureSensor);
  pSvc->addSensor(&lOutTemperatureSensor);
  pSvc->addSensor(&lIntAirHumiditySensor);
  pSvc->addSensor(&lOutAirHumiditySensor);
  pSvc->addSensor(&lGroundHumiditySensor);
  pSvc->addSensor(&lStopStepperSensor);
  pSvc->addSensor(&lThermalFlux);
  pSvc->addSensor(&lThermoCouple);
  pSvc->addSensor(&lCO2Sensor);

  pSvc->addSensor(&lAutoMode);
  
  pSvc->addActuator(&lAlarm);
  pSvc->addSensor(&lLedAlarm);
  
  pSvc->addActuator(&lFan);
  pSvc->addSensor(&lLedFan);

  pSvc->addActuator(&lWater);
  pSvc->addSensor(&lLedWater);

  pSvc->addActuator(&lMist);
  pSvc->addSensor(&lLedMist);
  
  pSvc->addActuator(&lLight);
  pSvc->addSensor(&lLedLight);
  
  pSvc->addActuator(&lHeat);
  pSvc->addSensor(&lLedHeat);
  
  //pSvc->addActuator(&lMotor);

#endif
}

void loop() {
  pSvc->doLoop();
}

void serialEvent() {
  pSvc->doSerialEvent();
}
