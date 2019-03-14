/**
   @file      LiveTimeInfo.h
   @Author    Tristan Beau ( tristan.beau@univ-paris-diderot.fr )
   @date      February, 2019
   @brief     Live Time Info pseudo sensor, inderectly used for the watch dog approach

   Detailed description


*/

#ifndef _LIVE_TIME_INFO_H_
#define _LIVE_TIME_INFO_H_

#include "Sensor.h"

class LiveTimeInfo : public Sensor {
  public:
    LiveTimeInfo(String s) : Sensor(s,true,0) {  } ;  // by default, display on screen
    virtual ~LiveTimeInfo() {} ;
    float getValue() {
      return ( millis()/1000 ) ;
    } ;
    String getLastValueAsString() {
      unsigned int d,h,m,s;
      unsigned long r=getLastValue();
      d=r/sm_day;  r=r%sm_day;
      h=r/sm_hour; r=r%sm_hour;
      m=r/sm_min;  s=r%sm_min;
      return String(d) + "d" + String(h) +"h" + String(m) + "m" + String(s) + "s";
    }
  private:
    static const unsigned long sm_min = 60;
    static const unsigned long sm_hour = 3600;
    static const unsigned long sm_day = 86400;
};

#endif
