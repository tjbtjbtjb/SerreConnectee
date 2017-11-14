
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

//#include "DummySensor.h"
#include "VoltageSensor.h"
#include "LightSensor.h"
#include "TemperatureSensor.h"
#include "HumiditySensor.h"
#include "O2Sensor.h"
#include "CO2Sensor.h"
#include "TemperatureCO2Sensor.h"

//#include "DummyActuator.h"
#include "DigitalActuator.h"
//#include "Buzzer.h"

Service          *pSvc;

// Sensors
VoltageSensor        lVoltageSensor("VOLT",A2);
TemperatureSensor    lTemperatureSensor("TEMP",A0);
HumiditySensor       lHumiditySensor("HUMIDITY",&lTemperatureSensor);
LightSensor          lLightSensor("LIGHT",A4);
O2Sensor             lO2Sensor("O2RATE",A12);
CO2Sensor            lCO2Sensor("CO2PPM",A8,A9);
TemperatureCO2Sensor lTempCO2Sensor("TEMP2",&lCO2Sensor);

// Actuators
DigitalActuator   lBuiltinLed("LED",LED_BUILTIN);

void setup() {
  pSvc = Service::getInstance();
  
  pSvc->addSensor(&lVoltageSensor);
  pSvc->addSensor(&lLightSensor);
  pSvc->addSensor(&lTemperatureSensor);
  pSvc->addSensor(&lHumiditySensor);
  pSvc->addSensor(&lO2Sensor);
  pSvc->addSensor(&lCO2Sensor);
  pSvc->addSensor(&lTempCO2Sensor);

  pSvc->addActuator(&lBuiltinLed);
}

void loop() {
  pSvc->doLoop();
}

void serialEvent() {
  pSvc->doSerialEvent();
}

