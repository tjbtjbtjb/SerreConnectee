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
    Sensor(String s,bool disp=false, unsigned int prec=0) : Device(s),m_lastTime(0),m_lastVal(-1.) {
      setPrecision(prec); setDisplay(disp);   // by default no display on screen
    };
    float getLastValue() { 
      unsigned long now = millis();
      if ( now - m_lastTime > sm_maxTime ) {
        m_lastVal = getValue();
        m_lastTime = now; 
      }
      return m_lastVal;
    }
    unsigned int getPrecision() const { return m_precision; } ;
    unsigned int setPrecision(unsigned int p) { m_precision = p; return getPrecision(); } ;
    bool getDisplay() const { return m_display; } ;
    bool setDisplay(bool d) { m_display = d; return getDisplay(); } ;
    virtual ~Sensor() {} ;
    
  protected : 
    virtual float getValue() = 0 ;  // protected in order to force the use of getLastValue()
     
  private:
    float                      m_lastVal;
    unsigned long              m_lastTime;
    const static unsigned long sm_maxTime ;
    unsigned int               m_precision;
    bool                       m_display;
};


#endif
