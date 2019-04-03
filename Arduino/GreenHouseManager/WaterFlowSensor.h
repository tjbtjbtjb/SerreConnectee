
/**
 * @file      WaterFlowSensor.h
 * @Author    Tristan Beau ( tristan.beau@univ-paris-diderot.fr )
 * @date      April, 2018
 * @brief     Water flow sensor class 
 * 
 * Detailed description
 * 
 * See http://wiki.seeedstudio.com/G1-8_Water_Flow_Sensor/
 * (source of inspiration of the code)
 * 
 */

#ifndef _WATER_FLOW_SENSOR_H_
#define _WATER_FLOW_SENSOR_H_

#include "Sensor.h"

class WaterFlowSensor : public Sensor {
  public:
    WaterFlowSensor(String s,int pin) : Sensor(s), m_pin(pin) { pinMode(m_pin,INPUT); }; 
    virtual ~WaterFlowSensor() {} ;
    float getValue() {
      int    Htime = pulseIn(m_pin,HIGH);      //read high time
      int    Ltime = pulseIn(m_pin,LOW) ;        //read low time
      float  Ttime = 1.e-6 * (Htime+Ltime);
      return (Ttime!=0.) ? 1. / 7.5 / Ttime : 0. ; 
    } ;
    String getLastValueAsString() { 
      return String(getLastValue(),getPrecision()) + " L/min";
    };
  private:
    int m_pin;
};

#endif
