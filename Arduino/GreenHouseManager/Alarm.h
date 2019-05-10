/**
 * @file      Alarm.h
 * @Author    Tristan Beau ( tristan.beau@univ-paris-diderot.fr )
 * @date      May, 2019
 * @brief     Header file for the Alarm master class
 * 
 * Detailed description
 * 
 * 
 */

#ifndef _ALARM_H_
#define _ALARM_H_

#include "Sensor.h"

class Alarm : public Sensor {
  public:
    Alarm(String s) : Sensor(s) { setPrecision(0); } ;  
    virtual ~Alarm() {} ;
  protected:
    float getValue() { 
      if ( getStatus() ) {
        setDisplay(true); return 1.;
      }
      else {
        setDisplay(false);
        return 0.;
      }
    }
    virtual bool getStatus() = 0;
};

#endif
