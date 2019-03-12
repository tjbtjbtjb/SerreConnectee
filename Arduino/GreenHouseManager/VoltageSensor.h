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
    VoltageSensor(String s,int pin,float coef=1.,String unit="V") : Sensor(s,false,3), m_pin(pin), m_coef(coef),m_unit(unit) {
      } ;
    virtual ~VoltageSensor() {} ;
    float getValue() {return m_coef*5.*analogRead(m_pin)/1023;} ;
    String getLastValueAsString() { 
      return String(getLastValue(),getPrecision()) + " " + m_unit;
    }
  private:
    int m_pin;
    float m_coef;
    String m_unit;
};

#endif
