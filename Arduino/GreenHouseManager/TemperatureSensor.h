/**
 * @file      TemperatureSensor.h
 * @Author    Tristan Beau ( tristan.beau@univ-paris-diderot.fr )
 * @date      November, 2017
 * @brief     Temperature sensor class based on Humidity/Temp Groove module
 * 
 * Detailed description
 * 
 * 
 */

#ifndef _TEMPERATURE_SENSOR_H_
#define _TEMPERATURE_SENSOR_H_

#include "Sensor.h"

class TemperatureSensor : public Sensor {
  public:
    TemperatureSensor(String s) : Sensor(s)  {} ;
    virtual ~TemperatureSensor() {} ;
    float getValue() const {return 99.99;} ;
};

#endif
