
/**
 * @file      DummySensor.h
 * @Author    Tristan Beau ( tristan.beau@univ-paris-diderot.fr )
 * @date      November, 2017
 * @revDate   March, 2019
 * @brief     Dummy prototype for a sensor
 * 
 * Detailed description
 * 
 * 
 */

#ifndef _DUMMY_SENSOR_H_
#define _DUMMY_SENSOR_H_

#include "Sensor.h"

class DummySensor : public Sensor {
  public:
    DummySensor(String s, unsigned long val) : Sensor(s,false,0),m_value(val) {} ;
    virtual ~DummySensor() {} ;
    float getValue() {
       delay(50);
       return 1.*m_value;
    } ;
    String getLastValueAsString() {
      return String(m_value);
    }
    private:
      unsigned long m_value;
};

#endif
