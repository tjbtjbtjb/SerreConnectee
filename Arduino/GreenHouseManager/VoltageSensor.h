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
    VoltageSensor(String s,int pin,float coef=1.) : Sensor(s), m_pin(pin), m_coef(coef) {
      } ;
    virtual ~VoltageSensor() {} ;
    float getValue() {return m_coef*5.*analogRead(m_pin)/1023;} ;
  private:
    int m_pin;
    float m_coef;
};

#endif
