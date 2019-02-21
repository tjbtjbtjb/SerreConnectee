/**
   @file      DigitalSensor.h
   @Author    Tristan Beau ( tristan.beau@univ-paris-diderot.fr )
   @date      January, 2018
   @brief     Digital voltage sensor class

   Detailed description


*/

#ifndef _DIGITAL_SENSOR_H_
#define _DIGITAL_SENSOR_H_

#include "Sensor.h"

class DigitalSensor : public Sensor {
  public:
    DigitalSensor(String s, int pin, int negative=0) : Sensor(s), m_pin(pin), m_neg(negative) { pinMode(pin,INPUT); } ;
    virtual ~DigitalSensor() {} ;
    float getValue() {
      return (digitalRead(m_pin)== ( (m_neg)?HIGH:LOW) )?0.:1. ;
    } ;
  private:
    int m_pin;
    int m_neg;
};

#endif
