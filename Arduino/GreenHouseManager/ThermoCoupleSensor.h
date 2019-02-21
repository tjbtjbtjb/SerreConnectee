/**
 * @file      ThermoCoupleSensor.h
 * @Author    Tristan Beau ( tristan.beau@univ-paris-diderot.fr )
 * @date      November, 2017
 * @brief     ThermoCouple sensor linked to the flux sensor
 * 
 * Detailed description
 * 
 * 
 */

#ifndef _THERMOCOUPLE_SENSOR_H_
#define _THERMOCOUPLE_SENSOR_H_

#include "Sensor.h"
#include <Adafruit_MAX31856.h>

class ThermoCoupleSensor : public Sensor {
  public:
    ThermoCoupleSensor(String s,int pin) : Sensor(s), m_pin(pin) {
        mp_max = new Adafruit_MAX31856(pin,pin+1,pin+2,pin+3);
        mp_max->begin();
        mp_max->setThermocoupleType(MAX31856_TCTYPE_K);
      } ;
    virtual ~ThermoCoupleSensor() {} ;
    float getValue() {return mp_max->readThermocoupleTemperature(); } ;
  private:
    int m_pin;
    Adafruit_MAX31856 *mp_max;
};

#endif
