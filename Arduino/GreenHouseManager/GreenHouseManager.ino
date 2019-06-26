
/**
 * @file      GreenHouseManager.h
 * @Author    Tristan Beau ( tristan.beau@univ-paris-diderot.fr )
 * @date      November, 2017
 * @lastRev   March, 2019
 * @brief     Main source for the Green House management code
 * 
 * Detailed description
 * 
 * 
 */

#define SOFTWARE_VERSION 190626 

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
#include "WaterFlowSensor.h"
#include "CapacitiveSoilSensor.h"

//#include "DummyActuator.h"
#include "DigitalActuator.h"
//#include "MotorActuator.h"
//#include "Buzzer.h"

#include "ThresholdAlarm.h"

// --- Static constant definitions

const unsigned long Service::sm_softVersion       = SOFTWARE_VERSION;
const int Service::sm_maxSensorCnt      = 32;
const int Service::sm_maxActuatorCnt    = 16;
const int Service::sm_maxAlarmCnt       = 8;
const unsigned long Sensor::sm_maxTime = 6000 ; // in milliseconds
const unsigned long Service::sm_delayDeepLoop = 200; 
const int Service::sm_loopsBtwDisplayUpdates = 20; 
const int Service::sm_loopsBtwLCDClean = 5000;

uint8_t I2C_LCD_ADDRESS = 0x51;  //Device address setting, default: 0x51

Service          *pSvc;

#define A_TEST 0
#define A_GREEN 1
// Which arduino ? 
#define MY_ARDUINO A_GREEN
//#define MY_ARDUINO A_TEST

#if MY_ARDUINO == A_TEST 
// Live time
LiveTimeInfo           lLiveTimeInfo("LIVETIME");
DummySensor            lDummySensor("DUMMY",3.14);
DigitalSensor          lBit23("BIT23",23);

#elif MY_ARDUINO == A_GREEN // Green house arduino -----------------------------------

LiveTimeInfo         lLiveTimeInfo("LIVETIME");
// Sensors
DummySensor          lVersionSensor("VERSION",(SOFTWARE_VERSION));
LightSensor          lLightSensor("LUX",A3);
TemperatureSensor    lIntTemperatureSensor("INTTEMP",A2);
TemperatureSensor    lOutTemperatureSensor("OUTTEMP",A15); 
AirHumiditySensor    lIntAirHumiditySensor("INTAIRHR",&lIntTemperatureSensor);
AirHumiditySensor    lOutAirHumiditySensor("OUTAIRHR",&lOutTemperatureSensor);
//VoltageSensor        lGroundHumiditySensor("GNDHR",A1,100./5.,"%"); // try to give in %
CapacitiveSoilSensor lGroundHumiditySensor("GNDHR",A1,575,254); 
DigitalSensor        lStopStepperSensor("STOPSTEP",9);
VoltageSensor        lThermalFlux("FLUX",A0,1./400./12.1e-6,"W/m2");   // in W / m2 ? 
ThermoCoupleSensor   lThermoCouple("THERMO",10);
CO2PulseSensor       lCO2Sensor("CO2PPM",2);
WaterFlowSensor      lWaterFlowSensor("FLOW",3); 

// Dual Digital Actuator & Sensor 
DigitalSensor        lAutoMode("AUTO",23); // ! AUTO : Reserved name, see in service

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

ThresholdAlarm       lAlarmMaxGndTmp("MAXTMP",&lThermoCouple,31,ThresholdAlarm::isMax,&lHeat,0);  // Above 31 degrees, set heat to 0 periodically
ThresholdAlarm       lAlarmMinGndTmp("MINTMP",&lThermoCouple,17,ThresholdAlarm::isMin,&lHeat,1);  // Below 17 degrees, set heat to 1 periodically
//ThresholdAlarm       lAlarmMaxGndHr("MAXHR",  &lGroundHumiditySensor, 95, ThresholdAlarm::isMax,&lFan,1); // Above 95%, set fan to 1 periodically
//ThresholdAlarm       lAlarmMinGndHr("MINHR",  &lGroundHumiditySensor, 0.5, ThresholdAlarm::isMin); // Below 0.5%, do nothing.
//ThresholdAlarm       lAlarmMAxCo2("MAXCO2", &lCO2Sensor, 500, ThresholdAlarm::isMax,&lFan,1); // 
#endif

void setup() {
  
  pSvc = Service::getInstance();//22); // 22, the pin for the WD LED. If no arg, flashes the internal led.
  
#if MY_ARDUINO == A_TEST
  pSvc->addSensor(&lLiveTimeInfo);
  pSvc->addSensor(&lDummySensor);
  pSvc->addSensor(&lBit23);
  
#elif MY_ARDUINO == A_GREEN

  //Adding alarms
  pSvc->setMainAlarm(&lAlarm);
  
  pSvc->addAlarm(&lAlarmMaxGndTmp);
  pSvc->addAlarm(&lAlarmMinGndTmp);
  //pSvc->addAlarm(&lAlarmMaxGndHr);
  //pSvc->addAlarm(&lAlarmMinGndHr);

  //Adding sensors / actuators
  pSvc->addSensor(&lLiveTimeInfo);
  pSvc->addSensor(&lVersionSensor); 
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
  pSvc->addSensor(&lWaterFlowSensor);

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

  //display default modification (Anne recommandation)
  lOutTemperatureSensor.setDisplay(false);
  lGroundHumiditySensor.setDisplay(true); lGroundHumiditySensor.setPrecision(1); // change default value
  lThermoCouple.setDisplay(true); lThermoCouple.setPrecision(1);
  lThermalFlux.setDisplay(true); lThermalFlux.setPrecision(1);

#endif
}

void loop() {
  pSvc->doLoop();
}

void serialEvent() {
  pSvc->doSerialEvent();
}
