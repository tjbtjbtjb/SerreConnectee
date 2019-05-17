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

#include "Actuator.h"
#include "Sensor.h"

class Alarm : public Sensor {
  public:
    Alarm(String s,Actuator *a=NULL,int v=0) : Sensor(s),mp_Actuator(a),m_AlarmActuatorValue(v) { setPrecision(0); } ;  
    virtual ~Alarm() {} ;
  protected:
    float getValue() { 
      if ( getStatus() ) {
        setDisplay(true);
        if (mp_Actuator != NULL) mp_Actuator->setValue(m_AlarmActuatorValue);
        return 1.;
      }
      else {
        setDisplay(false);
        return 0.;
      }
    }
    virtual bool getStatus() = 0;
  private: 
    Actuator* mp_Actuator;
    int       m_AlarmActuatorValue;
};

#endif
