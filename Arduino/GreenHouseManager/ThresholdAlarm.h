/**
 * @file      ThresholdAlarm.h
 * @Author    Tristan Beau ( tristan.beau@univ-paris-diderot.fr )
 * @date      May, 2019
 * @brief     Header file for the ThresholdAlarm class, which inherits from Alarm class
 * 
 * Detailed description
 * 
 * 
 */

#ifndef _THRESHOLD_ALARM_H_
#define _THRESHOLD_ALARM_H_

#include "Sensor.h"
#include "Alarm.h"

class ThresholdAlarm : public Alarm {
  public:
    enum Type {
      isUnknown,
      isMin,
      isMax
    };
    ThresholdAlarm(String s, Sensor *sens, float thr, ThresholdAlarm::Type t) : Alarm(s), mp_Sensor(sens) { setThreshold(thr); setType(t); } ;
    void setThreshold(float thr) { m_Threshold = thr; };
    void setType(ThresholdAlarm::Type t) { m_Type = t;};
    float getThreshold() { return m_Threshold; };
    virtual ~ThresholdAlarm() {} ;
  protected:
    virtual bool getStatus() {
      bool b;
      if(m_Type==isMin) {
          b = (mp_Sensor->getLastValue() < m_Threshold);
      } else if (m_Type==isMax) {
          b = (mp_Sensor->getLastValue() > m_Threshold);
      }
      else { 
          b = false;
      }
      return b;
    };
  private:
    ThresholdAlarm::Type m_Type;
    float                m_Threshold;
    Sensor              *mp_Sensor;
};

#endif
