/**
 * @file      Sensor.h
 * @Author    Tristan Beau ( tristan.beau@univ-paris-diderot.fr )
 * @date      November, 2017
 * @brief     Header file for the Sensor virtual class
 * 
 * Detailed description
 * 
 * 
 */

#ifndef _SENSOR_H_
#define _SENSOR_H_

#include "Device.h"

class Sensor : public Device {
  public:
    Sensor(String s) : Device(s) {} ;
    virtual float getValue() = 0 ;
    virtual ~Sensor() {} ;
};

#endif
