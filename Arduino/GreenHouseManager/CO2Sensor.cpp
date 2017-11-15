/**
 * @file      CO2Sensor.cpp
 * @Author    Tristan Beau ( tristan.beau@univ-paris-diderot.fr )
 * @date      November, 2017
 * @brief     CO2 sensor class implementation
 * 
 * Detailed description
 * 
 * 
 */

#include "CO2Sensor.h"
      
CO2Sensor::CO2Sensor(String s, int tx, int rx) 
  : Sensor(s), m_tx(tx), m_rx(rx) {   
    m_pSerialSensor = new SoftwareSerial(tx,rx);
    m_pSerialSensor->begin(9600);
} ;

bool CO2Sensor::readData() {
  int i = 0;

  //transmit command data
  for (i = 0; i < sizeof(m_cmdGetSensor); i++) {
    m_pSerialSensor->write(m_cmdGetSensor[i]);
  }
  delay(10);
  //begin reveiceing data
  if (m_pSerialSensor->available()) {
    while (m_pSerialSensor->available()) {
      for (int i = 0; i < 9; i++) {
        m_data[i] = m_pSerialSensor->read();
      }
    }
  }

  if ((i != 9) || (1 + (0xFF ^ (byte)(m_data[1] + m_data[2] + m_data[3] + m_data[4] 
                                    + m_data[5] + m_data[6] + m_data[7])))
                  != m_data[8]) {
    return false;
  }

  return true;
}
