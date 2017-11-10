/**
 * @file      O2Sensor.h
 * @Author    Tristan Beau ( tristan.beau@univ-paris-diderot.fr )
 * @date      O2 concentration sensor class
 * 
 * Detailed description
 * 
 * 
 */

#ifndef _O2_SENSOR_H_
#define _O2_SENSOR_H_

#include "Sensor.h"

class O2Sensor : public Sensor {
  public:
    O2Sensor(String s,int pin) : Sensor(s), m_pin(pin) {} ;
    virtual ~O2Sensor() {} ;
    float getValue() const {return readO2Vout() * 0.21 / 0.21 * 100; /* in percent */ } ;
  private:
    float readO2Vout() const {
      long sum = 0;
      for (int i = 0; i < 32; i++) {
        sum += analogRead(m_pin);
      }
      sum >>= 5;
      float MeasuredVout = sum * (m_vRef / 1023.0);
      return MeasuredVout;
    }

  private:
    int m_pin;
    const float m_vRef = 3.3; // voltage of adc reference
};

#endif
