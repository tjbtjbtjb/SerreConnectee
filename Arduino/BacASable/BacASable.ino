/*
  Filename: BacASable.ino
  Author: Tristan Beau ( tristan.beau@univ-paris-diderot.fr )
  Date: November 2017
  Description: Some trials...
*/

unsigned long i=0; //loop iteration

void (*resetFunc) (void) = 0; //declare reset function at address 0

// --- Init----- -------------------------------------------------
void setup(void)
{
  Serial.begin(9600); //init serial comm
  pinMode(LED_BUILTIN,OUTPUT); //init of ledPin
}

// --- Main loop -------------------------------------------------
void loop(void)
{
  digitalWrite(LED_BUILTIN, HIGH);
  delay(500);
  digitalWrite(LED_BUILTIN, LOW);
  
  Serial.println(i++);
  delay(1000);
  
  if ( i%10 == 9 ) {
    resetFunc();
    Serial.println("coucou");
  }
}

