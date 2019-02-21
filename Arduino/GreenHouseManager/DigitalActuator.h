/**
 * @file      DigitalActuator.h
 * @Author    Tristan Beau ( tristan.beau@univ-paris-diderot.fr )
 * @date      November, 2017
 * @brief     Simple actuator to set high or low value on a pin
 *            Set value according to previously stored value in EEPROM
 * 
 * Detailed description
 * 
 * 
 */

#ifndef _DIGITAL_ACTUATOR_H_
#define _DIGITAL_ACTUATOR_H_

#include <EEPROM.h>
#include "Actuator.h"

class DigitalActuator : public Actuator {
  public:
    DigitalActuator(String s,int pin,int v_init=0,int direct=0) : Actuator(s), m_pin(pin), m_direct(direct) { 
      int l_init = EEPROM.read(m_pin); // assume value stored at the m_pin location
      if ( l_init != 0 && l_init != 1 ) l_init = v_init ; // if dummy value detected, uses v_init;
      pinMode(m_pin,OUTPUT); 
      setValue(l_init);
    } ;
    virtual ~DigitalActuator() {} ;
    void setValue(int val) { 
      int v = (m_direct<0)?!val:val;
      digitalWrite(m_pin,v);
      EEPROM.update(m_pin,v); // re-write value only if value is updated (better for EEPROM life time according to documentation
    } ;
    void setValue(float val) { setValue((val)?HIGH:LOW); }
  private:
    int m_pin;
    int m_direct;
};

#endif
