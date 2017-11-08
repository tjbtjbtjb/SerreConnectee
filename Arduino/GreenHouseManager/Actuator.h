/**
 * @file      Actuator.h
 * @Author    Tristan Beau ( tristan.beau@univ-paris-diderot.fr )
 * @date      November, 2017
 * @brief     Header file for the Actuator virtual class
 * 
 * Detailed description
 * 
 * 
 */

#ifndef _ACTUATOR_H_
#define _ACTUATOR_H_

#include "Device.h"

class Actuator : public Device {
  public:
    Actuator(String s) : Device(s) {} ;
    virtual void setValue(int ) const = 0 ;
    virtual void setValue(float ) const = 0 ;
    virtual ~Actuator() {} ;
};

#endif
