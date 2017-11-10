/**
 * @file      LightSensor.h
 * @Author    Tristan Beau ( tristan.beau@univ-paris-diderot.fr )
 * @date      Light sensor class, based on the voltage class
 * 
 * Detailed description
 * 
 * 
 */

#ifndef _LIGHT_SENSOR_H_
#define _LIGHT_SENSOR_H_

#include "Sensor.h"

class LightSensor : public Sensor {
  public:
    LightSensor(String s,int pin) : Sensor(s), m_pin(pin) {} ;
    virtual ~LightSensor() {} ;
    float getValue() const {return 100.*analogRead(m_pin)/1023./0.74;} ;
  private:
    int m_pin;
};

#endif
