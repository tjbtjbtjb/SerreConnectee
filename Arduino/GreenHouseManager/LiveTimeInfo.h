/**
   @file      LiveTimeInfo.h
   @Author    Tristan Beau ( tristan.beau@univ-paris-diderot.fr )
   @date      February, 2019
   @RevDate   March, 2019
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

#define DIVMOD(X) X=r/sm_ ## X; r=r % sm_ ## X
      DIVMOD(d); DIVMOD(h); DIVMOD(m); s=r;   // iterative computation of d/h/m/s from nb of seconds

#define toS(X) String(X) + String( #X ) 
#define toS0(X) String( (X<10) ? "0" : "") + toS(X)  /* with leading 0 if needed */

      return toS(d) + toS0 (h) + toS0(m) + toS0(s) ;  // formating the output
    };
  private:
    static const unsigned long sm_m = 60;    // minutes
    static const unsigned long sm_h = 3600;  // hours
    static const unsigned long sm_d = 86400; // day
};

#endif
