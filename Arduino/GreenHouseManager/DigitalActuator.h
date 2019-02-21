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
    DigitalActuator(String s,int pin,int v_init=0,int direct=0) : Actuator(s), m_pin(pin), m_direct(direct) { 
      pinMode(m_pin,OUTPUT); 
      setValue(v_init);
    } ;
    virtual ~DigitalActuator() {} ;
    void setValue(int val) { digitalWrite(m_pin,(m_direct<0)?!val:val); } ;
    void setValue(float val) { setValue((val)?HIGH:LOW); }
  private:
    int m_pin;
    int m_direct;
};

#endif
