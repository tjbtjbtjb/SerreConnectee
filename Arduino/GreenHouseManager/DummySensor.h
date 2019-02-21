
/**
 * @file      DummySensor.h
 * @Author    Tristan Beau ( tristan.beau@univ-paris-diderot.fr )
 * @date      November, 2017
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
    DummySensor(String s) : Sensor(s) {} ;
    virtual ~DummySensor() {} ;
    float getValue() {
       delay(5000);
       return 12.345;
    } ;
};

#endif
