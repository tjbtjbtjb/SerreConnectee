/**
 * @file      VoltageSensor.h
 * @Author    Tristan Beau ( tristan.beau@univ-paris-diderot.fr )
 * @date      November, 2017
 * @brief     Simple analogic voltage sensor class
 * 
 * Detailed description
 * 
 * 
 */

#ifndef _VOLTAGE_SENSOR_H_
#define _VOLTAGE_SENSOR_H_

#include "Sensor.h"

class VoltageSensor : public Sensor {
  public:
    VoltageSensor(String s,int pin) : Sensor(s), m_pin(pin) {} ;
    virtual ~VoltageSensor() {} ;
    float getValue() const {return 5.*analogRead(m_pin)/1023;} ;
  private:
    int m_pin;
};

#endif
