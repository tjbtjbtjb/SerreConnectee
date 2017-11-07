
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

#include "DummySensor.h"
#include "VoltageSensor.h"
#include "TemperatureSensor.h"

#include "DummyActuator.h"
#include "DigitalActuator.h"

Service          *pSvc;

// Sensors
DummySensor       lDummySensor("DUMMY");
VoltageSensor     lVoltageSensor("VOLT",A2);
TemperatureSensor lTemperatureSensor("TEMP");

// Actuators
DummyActuator     lDummyActuator("DUMMY");
DigitalActuator   lGreenLed("GREEN",10);
DigitalActuator   lYellowLed("YELLOW",11);
DigitalActuator   lRedLed("RED",12);

void setup() {
  pSvc = Service::getInstance();
  
  pSvc->addSensor(&lDummySensor);
  pSvc->addSensor(&lVoltageSensor);
  pSvc->addSensor(&lTemperatureSensor);

  pSvc->addActuator(&lDummyActuator);
  pSvc->addActuator(&lGreenLed);
  pSvc->addActuator(&lYellowLed);
  pSvc->addActuator(&lRedLed);
}

void loop() {
  pSvc->doLoop();
}

void serialEvent() {
  pSvc->doSerialEvent();
}

