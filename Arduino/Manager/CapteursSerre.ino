
// A propos de l'ecran ----------------------------------------------
#include <Wire.h>
#include <I2C_LCD.h>
I2C_LCD LCD;
extern GUI_Bitmap_t bmlogo_uspc_big;      //Declare bitmap data packet.
uint8_t I2C_LCD_ADDRESS = 0x51;  //Device address setting, default: 0x51
// LCD_FontSort f[] = {Font_6x8, Font_6x12,Font_8x16_1,Font_8x16_2,Font_10x20,Font_12x24,Font_16x32};

// Capteur O2 -------------------------------------------------------
const float VRefer_O2 = 3.3;       // voltage of adc reference
const int pinAdc_O2   = A13;

// Capteur CO2 --------------------------------------------------------
#include <SoftwareSerial.h>
SoftwareSerial s_serial(A8, A9);     // TX, RX
#define sensorCO2 s_serial
const unsigned char cmd_get_sensor[] =
{
  0xff, 0x01, 0x86, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x79
};
unsigned char dataRevice[9];
int tempCO2;
int CO2PPM;

// Capteur humidite air -------------------------------------------------
#include "DHT.h"
#define DHTPIN A0     // what pin we're connected to

// Uncomment whatever type you're using!
//#define DHTTYPE DHT11   // DHT 11
#define DHTTYPE DHT22   // DHT 22  (AM2302)
//#define DHTTYPE DHT21   // DHT 21 (AM2301)

// Connect pin 1 (on the left) of the sensor to +5V
// Connect pin 2 of the sensor to whatever your DHTPIN is
// Connect pin 4 (on the right) of the sensor to GROUND
// Connect a 10K resistor from pin 2 (data) to pin 1 (power) of the sensor

DHT dht(DHTPIN, DHTTYPE);

// Capteur lumiere ------------------------------------------------------
const int pinLight = A4;

// --- Initialisation ---------------------------------------------------------------------
void setup(void)
{
  Wire.begin();           //I2C controler init.
  sensorCO2.begin(9600);  //init CO2
  dht.begin();            //init humidite
}

// --- Boucle principale -------------------------------------------------------------------
void loop(void)
{
  long i = 0;
  char s[1024];

  float concentrationO2 = 0;
  float h, t;
  int l=0;

  /* --- Presentation sur l'ecran --- */
  //clean
  LCD.CleanAll(WHITE);    //Erase all.
  delay(1000);            //Delay for 1s.

  //logo USPC
  LCD.WorkingModeConf(ON, ON, WM_BitmapMode); // Pour une image
  LCD.DrawScreenAreaAt(&bmlogo_uspc_big, 0 , 0);
  delay(3000);

  //passage mode texte et choix de la font
  LCD.WorkingModeConf(ON, ON, WM_CharMode);
  LCD.FontModeConf(Font_6x8, FM_ANL_AAA, BLACK_BAC);

  /* --- Boucle lecture capteurs --- */
  while (1) {
    // Nettoyage ecran
    LCD.CleanAll(WHITE);
    LCD.CharGotoXY(0, 0);      //Set the start coordinate.

    // Lecture capteurs
    concentrationO2 = readO2Concentration();
    if ( ! dataCO2Recieve() ) {
      tempCO2 = CO2PPM = -999;
    }
    h = dht.readHumidity(); t = dht.readTemperature();
    if (isnan(t) || isnan(h)) {
      t = h = -999;
    }
    l = readLight();

    sprintf(s,"Uptime : ~ %ld s\n\nO2 : \t\t  ",(i++)*5); 
    dtostrf(concentrationO2,4,1,s+strlen(s)); // nota : sprintf ok avec %d mais pas %f
    sprintf(s,"%s %%\n",s);
    sprintf(s,"%sCO2 : \t\t%d.0 PPM\nT(CO2) : \t%d.0 deg C\n",s,CO2PPM,tempCO2);
    sprintf(s,"%sHumidite : "); dtostrf(h,4,1,s+strlen(s));
    sprintf(s,"%s %%\nT(hum) : \t",s); dtostrf(t,4,1,s+strlen(s)); 
    sprintf(s,"%s deg C\nLight :\t\t",s); dtostrf(l,4,1,s+strlen(s));
    sprintf(s,"%s %%au\n",s);

    LCD.print(s);
    delay(5000);
  }
}

// --- Diverses fonctions -----------------------------------------------------------------------

float readO2Vout()
{
  long sum = 0;
  for (int i = 0; i < 32; i++)
  {
    sum += analogRead(pinAdc_O2);
  }

  sum >>= 5;

  float MeasuredVout = sum * (VRefer_O2 / 1023.0);
  return MeasuredVout;
}

float readO2Concentration()
{
  // Vout samples are with reference to 3.3V
  float MeasuredVout = readO2Vout();

  //float Concentration = FmultiMap(MeasuredVout, VoutArray,O2ConArray, 6);
  //when its output voltage is 2.0V,
  float Concentration = MeasuredVout * 0.21 / 2.0;
  float Concentration_Percentage = Concentration * 100;
  return Concentration_Percentage;
}

bool dataCO2Recieve(void)
{
  byte data[9];
  int i = 0;

  //transmit command data
  for (i = 0; i < sizeof(cmd_get_sensor); i++)
  {
    sensorCO2.write(cmd_get_sensor[i]);
  }
  delay(10);
  //begin reveiceing data
  if (sensorCO2.available())
  {
    while (sensorCO2.available())
    {
      for (int i = 0; i < 9; i++)
      {
        data[i] = sensorCO2.read();
      }
    }
  }

  if ((i != 9) || (1 + (0xFF ^ (byte)(data[1] + data[2] + data[3] + data[4] + data[5] + data[6] + data[7]))) != data[8])
  {
    return false;
  }

  CO2PPM = (int)data[2] * 256 + (int)data[3];
  tempCO2 = (int)data[4] - 40;

  return true;
}

float readLight() {
  return analogRead(pinLight)*100./1023/.73;
}

