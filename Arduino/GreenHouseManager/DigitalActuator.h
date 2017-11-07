/**
 * @file      DigitalActuator.h
 * @Author    Tristan Beau ( tristan.beau@univ-paris-diderot.fr )
 * @date      November, 2017
 * @brief     Simple actuator to set high or low value on a pin
 * 
 * Detailed description
 * 
 * 
 */

#ifndef _DIGITAL_ACTUATOR_H_
#define _DIGITAL_ACTUATOR_H_

#include "Actuator.h"

class DigitalActuator : public Actuator {
  public:
    DigitalActuator(String s,int pin) : Actuator(s), m_pin(pin) { pinMode(m_pin,OUTPUT); } ;
    virtual ~DigitalActuator() {} ;
    void setValue(int val) const { digitalWrite(m_pin,val); } ;
    void setValue(float val) const { setValue((val)?HIGH:LOW); }
  private:
    int m_pin;
};

#endif
