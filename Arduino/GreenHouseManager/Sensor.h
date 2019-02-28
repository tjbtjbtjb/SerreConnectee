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
    Sensor(String s) : Device(s),m_lastTime(0),m_lastVal(-1.) {} ;
    float getLastValue() { 
      unsigned long now = millis();
      if ( now - m_lastTime > sm_maxTime ) {
        m_lastVal = getValue();
        m_lastTime = now; 
      }
      return m_lastVal;
    }
    virtual ~Sensor() {} ;
    
  protected : 
    virtual float getValue() = 0 ;  // protected in order to force the use of getLastValue()
     
  private:
    float m_lastVal;
    unsigned long m_lastTime;
    const static unsigned long sm_maxTime ;
};


#endif
