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
    LiveTimeInfo(String s) : Sensor(s) {  } ;
    virtual ~LiveTimeInfo() {} ;
    float getValue() {
      return ( millis()/1000 ) ;
    } ;
//  private:
};

#endif
