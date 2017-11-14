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

#ifndef _BUZZER_H_
#define _BUZZER_H_

#include "Actuator.h"

class Buzzer : public Actuator {
  public:
    Buzzer(String s,int pin) : Actuator(s), m_pin(pin) { pinMode(m_pin,OUTPUT); } ;
    virtual ~Buzzer() {} ;
    void setValue(int val) { tone(m_pin,val); } ;
    void setValue(float val) { if (val>0) setValue((int)floor(val)); else noTone(m_pin); }
  private:
    int m_pin;
};

#endif
