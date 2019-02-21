/**
 * @file      TemperatureCO2Sensor.h
 * @Author    Tristan Beau ( tristan.beau@univ-paris-diderot.fr )
 * @date      November, 2017
 * @brief     Humidity sensor class based on the Grove temperature sensor class
 * 
 * Detailed description
 * 
 * 
 */

#ifndef _TEMPERATURECO2_SENSOR_H_
#define _TEMPERATURECO2_SENSOR_H_

#include "CO2GroveSensor.h"

class TemperatureCO2Sensor : public Sensor {
  public:
    TemperatureCO2Sensor(String s,CO2GroveSensor *c):Sensor(s) { m_pCO2 = c; }; 
    virtual ~TemperatureCO2Sensor() {} ;
    float getValue() {return m_pCO2->getTemperature(); } ;
  private:
    CO2GroveSensor *m_pCO2;
};

#endif
