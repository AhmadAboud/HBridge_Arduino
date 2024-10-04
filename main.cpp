#include <Arduino.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "Arduino.h"

#include "Print.h"

#include <SPI.h>

    int in1 = 9;
    int Dis1 = 8;
    int NDis1 = 7;
    int in2 = 10;

    const bool on = HIGH;
    const bool off = LOW;
    bool Dir;
    int counter=0;
 

  void setup (void)
    {
      pinMode(in1, OUTPUT);    
      pinMode(Dis1, OUTPUT);
      pinMode(NDis1, OUTPUT);
      pinMode(24, OUTPUT);
      pinMode(in2, OUTPUT);
      Serial.begin (9600);
      Serial.println ();

      digitalWrite(SS, HIGH);  // ensure SS stays high for now
      SPI.begin ();

      // Slow down the master a bit
      SPI.setClockDivider(SPI_CLOCK_DIV8);
    }  

  uint16_t transferAndWait (const uint16_t what)
    {
      SPI.beginTransaction(SPISettings((uint32_t)13700, MSBFIRST, SPI_MODE1));
       delay(100);
      digitalWrite(24, LOW);
       delay(100);
      uint16_t a = SPI.transfer16(what);    // Send aus dem Master
      
      delay(100);
      digitalWrite(24, HIGH);
       delay(100);
      SPI.endTransaction();
      return a;
    }


void HB_Activation()
{ 
  digitalWrite(Dis1, HIGH);
  delay(250);
  digitalWrite(Dis1, LOW);
  Serial.println(transferAndWait(100100000000000), BIN); 

}
void HB_Activated(bool Dir, int a , int b)
{ 
  _delay_us(10);
  digitalWrite(in2, HIGH);
  _delay_us(10);
  digitalWrite(in2, LOW);
  _delay_us(10);
 digitalWrite(NDis1, HIGH); 
  
  
  pinMode(in1, OUTPUT);
  TCCR2A = _BV(COM2A0) | _BV(COM2B1) | _BV(WGM21) | _BV(WGM20);
  OCR2A = a;
  OCR2B = b;
  digitalWrite(in2, Dir);
  delay (100);  
}

void HB_Reset()
{
  
 Serial.println(transferAndWait(0x2401), HEX);
  delay(100);
 Serial.println(transferAndWait(0x2201), HEX);
  delay(100);
 Serial.println(transferAndWait(0x2000), HEX);  
  delay(100);
 Serial.println(transferAndWait(0111000000000011), BIN);    
}

void loop (void)
{   int a; int b;
    a=180;
    b=40;
    int Sek;

    HB_Reset();
    HB_Activation();

    // H_Brücke Aus
    HB_Activated(Dir, 100 , 5);
    // Auf Raumtemperatur AbkühlungsZeit
    delay(10000);

    // 10 Sek Erwärmungszeit
    Sek =10;
    int i=10*Sek;
    Dir =on;
    while (i--)
    { 
      HB_Activated(Dir, 100 , 1*i);
      // Serial.println(transferAndWait(1000000000000000), BIN);
      Serial.println( i );
    } 
    

    // delay(15000);

  Dir =off;
    i=10*Sek;
    while (i--)
    { 
      HB_Activated(Dir, 100 , 1*i);
      // Serial.println(transferAndWait(1000000000000000), BIN);
      Serial.println( i );
    } 


   

    
}  






