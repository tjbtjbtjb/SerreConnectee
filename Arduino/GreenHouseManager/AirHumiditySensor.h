/**
 * @file      AirHumiditySensor.h
 * @Author    Tristan Beau ( tristan.beau@univ-paris-diderot.fr )
 * @date      November, 2017
 * @brief     Air humidity sensor class based on the Grove temperature sensor class
 * 
 * Detailed description
 * 
 * 
 */

#ifndef _AIR_HUMIDITY_SENSOR_H_
#define _AIR_HUMIDITY_SENSOR_H_

#include "Sensor.h"
#include "TemperatureSensor.h"

class AirHumiditySensor : public Sensor {
  public:
    AirHumiditySensor(String s,TemperatureSensor *t):Sensor(s) { m_pTemp = t; }; 
    virtual ~AirHumiditySensor() {} ;
    float getValue() {return m_pTemp->getDht()->readHumidity(); } ;
    String getLastValueAsString() {
      return String(getLastValue(),getPrecision()) + " %";
    }
  private:
    TemperatureSensor *m_pTemp;
};

#endif
