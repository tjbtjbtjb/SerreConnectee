/**
 * @file      CO2PulseSensor.h
 * @Author    Tristan Beau ( tristan.beau@univ-paris-diderot.fr )
 * @date      February, 2018
 * @brief     CO2 Pulse sensor class (for new CO2 sensor, golden color)
 * 
 * Detailed description
 * 
 * For the MH-Z14A sensor
 * 
 */

#ifndef _CO2_PULSE_SENSOR_H_
#define _CO2_PULSE_SENSOR_H_

#include "Sensor.h"

class CO2PulseSensor : public Sensor {
  public:
    CO2PulseSensor(String s,int pin) : Sensor(s), m_pin(pin) { }; 
    virtual ~CO2PulseSensor() {} ;
    float getValue() {
      unsigned long t_high,t_period; //t_low
      long ppm;
      t_high = pulseIn(m_pin,HIGH,2000000); //in microseconds
      t_period = 1004000;
      ppm = 2000. * (1.*t_high - 2000) /(t_period - 4000);
      //if ( ppm > 2000) ppm=0; 

      return 1.*ppm ; 
    } ;
  private:
    int m_pin;
};

#endif
