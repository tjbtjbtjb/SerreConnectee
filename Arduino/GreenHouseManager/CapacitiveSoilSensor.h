/**
 * @file      CapacitiveSoilSensor.h
 * @Author    Tristan Beau ( tristan.beau@univ-paris-diderot.fr )
 * @date      june 2019
 * @brief     See https://wiki.dfrobot.com/Capacitive_Soil_Moisture_Sensor_SKU_SEN0193 for ref
 * 
 * Detailed description
 * 
 * 
 */

#ifndef _CAPACITIVE_SOIL_SENSOR_H_
#define _CAPACITIVE_SOIL_SENSOR_H_

#include "Sensor.h"

class CapacitiveSoilSensor : public Sensor {
  public:
    CapacitiveSoilSensor(String s,int pin,int dry,int wat) : Sensor(s,false,3), m_pin(pin),m_dry(dry),m_water(wat) {
      } ;
    virtual ~CapacitiveSoilSensor() {} ;
    float getValue() {
      int v=analogRead(m_pin);
      return (float(m_dry-v))/(m_dry-m_water)*100;
    } ;
    String getLastValueAsString() { 
      return String(getLastValue(),getPrecision()) + " %";
    }
  private:
    int m_pin;
    int m_dry;
    int m_water;
};

#endif
