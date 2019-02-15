
// ----------------------------------------------


#include <Adafruit_MAX31856.h>
Adafruit_MAX31856 max = Adafruit_MAX31856(10, 11, 12, 13);

// Capteur humidite air -------------------------------------------------
#include "DHT.h"
#define DHTPIN A2     // what pin we're connected to
#define DHTTYPE DHT22   // DHT 22  (AM2302)

DHT dht(DHTPIN, DHTTYPE);

// Capteur lumiere ------------------------------------------------------
const int pinLight = A3;
const int sensorPin = 2;     // pour le CO2
long ppm=0;
unsigned long duration;  // pour le CO2
int sensorPinSol = A1; // select the input pin humidite du sol
int hrsol = 0; // variable to store the value HRSOL
float t_sol=0;
const int  f= A0;
int Valeurflux ;
 float tab[10] ; 
 float somme,sommeT = 0 ;

const int Chauffe = 4 ;     // Chauffe
const int Ventilo = 5;     // Aeration
const int E_vanne = 6;     // Irrigation
const int Brume   = 7;     // Vapeur
const int Lumiere = 8;     //Lumiere serre
const int Butee  =  9;     // Butee moteur PaP


// --- Initialisation ---------------------------------------------------------------------
void setup(void)
{
 Serial.begin(9600);
 
   dht.begin();            //init humidite
   pinMode(sensorPin, INPUT);
  max.begin();                                 // Lance ampli TH du fluxmetre
  max.setThermocoupleType(MAX31856_TCTYPE_T);  // Choix du TC a lire 
   
  
      pinMode(Chauffe, OUTPUT);
          digitalWrite(Chauffe,HIGH); //Coupe la chauffe
      pinMode(Ventilo, OUTPUT);
           digitalWrite(Ventilo,HIGH); //Coupe ventilo
      pinMode(E_vanne, OUTPUT);
            digitalWrite(E_vanne,HIGH); //Ferme l eau
      pinMode(Brume, OUTPUT);
             digitalWrite(Brume,LOW); //pas de brume
      pinMode(Lumiere, OUTPUT);
              digitalWrite(Lumiere,LOW); //Eteindre la lampe 
      pinMode(Butee, INPUT);
   Serial.println( " TESTS CHAUFFE SERRE MANU :");
    Serial.println("       FLUX       TempSOL      HR_SOL     HR_AIR    TempAir     " );   
   Serial.println (); Serial.println (); Serial.println ();
   
}

// --- Boucle principale -------------------------------------------------------------------
void loop(void)
{
  long i = 0;
  char s[1024];
  float concentrationO2 = 0;
  float h, t,hrsol;
  int l=0;
   /* --- Boucle lecture capteurs --- */
  while (1) {
    // Lecture capteurs
  
    h = dht.readHumidity(); t = dht.readTemperature();
    if (isnan(t) || isnan(h)) {
      t = h = -999;
    }
     // CO2
/*----------------------------------------------------*/
    duration = pulseIn(sensorPin,HIGH,2000000);
               ppm = 5000 * (((duration)/1000)-2)/1000;
               if ( ppm > 5000) {ppm=0;}
   // Serial.println("CO2 = " + String(ppm) + " ppm");
/*------------------------------------------------------*/
   //H¨% SOL
              hrsol = analogRead(sensorPinSol);
              
  // TEMPERATURE DU SOL du fluxmetre 
  /*--------------------------------------------------*/
                                                                  //t_sol= max.readThermocoupleTemperature();
  for (int j = 0 ; j < 10 ; j++) 
              { 
                tab[j]= max.readThermocoupleTemperature();
                       sommeT=sommeT+tab[j] ; //somme des valeurs  du tableau 
                delay(150);
               }
           
float t_sol = sommeT / 10.0 ; //valeur moyenne
             sommeT=0;
               

 /*--------------------------------------------------*/             
           
 // FLUX SOL
 /*----------------------------------------------------*/
for (int i = 0 ; i < 10 ; i++) 
              { 
                tab[i]=analogRead(f) * 5.0 / 1023;
                       somme=somme+tab[i] ; //somme des valeurs  du tableau 
                delay(150);
               }
           
float Fmoyen = somme / 10.0 ; //valeur moyenne
             somme=0;
/*------------------------------------------------------*/  
    Serial.print(Fmoyen,4); Serial.print("      ");
        Serial.print(t_sol,1); Serial.print("      ");
            Serial.print(hrsol,1); Serial.print("      ");
                Serial.print(h,1); Serial.print("      ");
                    Serial.println(t,1); Serial.print("      ");                                          
                       }
    delay(5000);
  }


// --- Diverses fonctions -----------------------------------------------------------------------

float readLight() {
  return analogRead(pinLight)*100./1023/.73;

                  }
 
