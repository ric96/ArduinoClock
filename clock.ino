#include "LedControlMS.h"
#include <Wire.h>
#include "RTClib.h"
#include <dht.h>

dht DHT;

#define DHT11_PIN 2 
RTC_DS3231 RTC;
/*
 Now we need a LedControl to work with.
 ***** These pin numbers will probably not work with your hardware *****
 pin 12 is connected to the DataIn 
 pin 11 is connected to the CLK 
 pin 10 is connected to LOAD 
 We have only a single MAX72XX.
 */
#define NBR_MTX 4 
LedControl lc=LedControl(12,11,10, NBR_MTX);
/* we always wait a bit between updates of the display */
unsigned long delaytime=300;
int cl=0, weekday, temp, hum, d, m ,y, i, j;
long previousMillis = 0;
long interval = 1000;
void setup() {
  /*
   The MAX72XX is in power-saving mode on startup,
   we have to do a wakeup call
   */
  Serial.begin (9600);
  Serial.println("Setup");
  for (int i=0; i< NBR_MTX; i++){
    lc.shutdown(i,false);
  /* Set the brightness to a medium values */
    lc.setIntensity(i,15);
  /* and clear the display */
    lc.clearDisplay(i);
  }
  pinMode(17,OUTPUT);
  pinMode(16,OUTPUT);
  pinMode(8,OUTPUT);
  pinMode(7,INPUT);
  pinMode(9,OUTPUT);
  digitalWrite(17, HIGH);
  digitalWrite(16, LOW);
  digitalWrite(8, LOW);
  digitalWrite(7, HIGH);
  Wire.begin();
  RTC.begin();
 
  if (! RTC.begin()) {
    Serial.println("RTC is NOT running!");
    // following line sets the RTC to the date & time this sketch was compiled

  }
    //RTC.adjust(DateTime(__DATE__, __TIME__));

  
  

}

void loop() { 
  unsigned long currentMillis = millis();
  if(currentMillis - previousMillis > interval) 
  {
    previousMillis = currentMillis;
  analogWrite(9,0);
  DateTime now = RTC.now();
  lc.displayChar(0,now.minute()%10); //minute ones
  lc.displayChar(1,now.minute()/10); //minutes tens
  lc.displayChar(2,now.hour()%10); //hour ones
  lc.displayChar(3,now.hour()/10); //hour ones
  for(int i=now.second(), j=0; i>=0, j<=7; i=i/2, j++)
  {
    int bin=i%2;
    if (bin != 0 )
    {
      lc.setLed(2,6,j,true);
    }
    else 
    {
      lc.setLed(2,6,j,false);
    }
  }
  if(now.second() == 00 && now.minute() == 0)
  {
    analogWrite(9,80);
  }
  //delay(1000);
  }
//########END MAIN CLOCK##########
  
  
  if (digitalRead(7) == LOW)
  {
    for (cl=0;cl<=7;cl++)
  {
    lc.setLed(2,6,cl,false);
  }
  DateTime now = RTC.now();
  lc.displayChar(0,now.month()%10); //month ones
  lc.displayChar(1,now.month()/10); //month tens
  lc.displayChar(2,now.day()%10); //day ones
  lc.displayChar(3,now.day()/10); //day ones
  lc.setLed(2,6,7,true);
  lc.setLed(2,6,6,true);
  delay(3000);
  lc.setLed(2,6,7,false);
  lc.setLed(2,6,6,false);
  lc.displayChar(0,now.year()%10); //month ones
  lc.displayChar(1,(now.year()/10)%10); //month tens
  lc.displayChar(2,(now.year()/100)%10); //day ones
  lc.displayChar(3,now.year()/1000); //day ones
  delay(3000);
  d = now.day();
  m = now.month();
  y = now.year();
  weekday  = (d+=m<3?y--:y-2,23*m/9+d+4+y/4-y/100+y/400)%7  ;
  while(weekday>=10)
    {
        weekday = weekday / 10;
    }
  showday(weekday);
  delay(3000);
  lc.clearAll();
  DHT.read11(DHT11_PIN);
  temp = (int) DHT.temperature;
  lc.displayChar(2,temp%10);
  lc.displayChar(3,temp/10);
  lc.setLed(1,0,7,true);
  lc.setLed(1,0,6,true);
  lc.setLed(1,1,7,true);
  lc.setLed(1,1,6,true);
  lc.displayChar(0,17);
  delay(3000);
  lc.clearAll();
  hum = (int) DHT.humidity;
  lc.displayChar(2,hum%10);
  lc.displayChar(3,hum/10);
  lc.setLed(1,0,7,true);
  lc.setLed(1,0,6,true);
  lc.setLed(1,1,7,true);
  lc.setLed(1,1,6,true);
  lc.setLed(1,5,2,true);
  lc.setLed(1,5,1,true);
  lc.setLed(1,6,2,true);
  lc.setLed(1,6,1,true);
  for(i=6; i>=0; i--)
  {
    lc.setLed(1,i,i,true);
  }
  lc.displayChar(0,22);
  delay(3000);
  lc.clearAll();
  }
  
}

void showday(int w) {
  lc.clearAll();
  switch(w)
  {
 
    case 0: lc.displayChar(3,33); //S
            lc.displayChar(2,35); //U
            lc.displayChar(1,28); //N
            break;
    
    case 1: lc.displayChar(3,27); //M
            lc.displayChar(2,29); //O
            lc.displayChar(1,28); //N
            break;
            
    case 2: lc.displayChar(3,34); //T
            lc.displayChar(2,35); //U
            lc.displayChar(1,19); //E
            break;
    
    case 3: lc.displayChar(3,37); //W
            lc.displayChar(2,19); //E
            lc.displayChar(1,18); //D
            break;
    
    case 4: lc.displayChar(3,34); //T
            lc.displayChar(2,22); //H
            lc.displayChar(1,35); //U
            break;
    
    case 5: lc.displayChar(3,20); //F
            lc.displayChar(2,32); //R
            lc.displayChar(1,23); //I
            break;
            
    case 6: lc.displayChar(3,33); //S
            lc.displayChar(2,15); //A
            lc.displayChar(1,34); //T
            break;
  }
}
