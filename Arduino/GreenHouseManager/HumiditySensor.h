/**
 * @file      HumiditySensor.h
 * @Author    Tristan Beau ( tristan.beau@univ-paris-diderot.fr )
 * @date      November, 2017
 * @brief     Humidity sensor class based on the Grove temperature sensor class
 * 
 * Detailed description
 * 
 * 
 */

#ifndef _HUMIDITY_SENSOR_H_
#define _HUMIDITY_SENSOR_H_

#include "Sensor.h"
#include "TemperatureSensor.h"

class HumiditySensor : public Sensor {
  public:
    HumiditySensor(String s,TemperatureSensor *t):Sensor(s,false,1) { m_pTemp = t; }; 
    virtual ~HumiditySensor() {} ;
    float getValue() {return m_pTemp->getDht()->readHumidity(); } ;
    String getLastValueAsString() { 
      return String(getLastValue(),getPrecision()) + " %";
    }
  private:
    TemperatureSensor *m_pTemp;
};

#endif
