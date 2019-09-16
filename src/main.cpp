#include <Arduino.h>
#include "constants.h"
#include "WaterSensor.h"
#include "Services.h"
#include <Wire.h>
#include <LiquidCrystal.h>

WaterSensor room1;
WaterSensor room2;
Services service;
LiquidCrystal lcd(12, 11, 9, 6, 5, 4); //(rs, enable, d4, d5, d6, d7)
// 7,8 are sim7000 TX,RX PIN //do not use for others will using sim module
void isr1(){
  room1.pulseCounter();
  //pulseCount++;   // for interrupt 0 (sensorpin 2)
}
void isr2(){
  room2.pulseCounter();
  //pulseCount++;   // for interrupt 1 (sensorpin 3)
}
void setup(){
  
  Serial.begin(115200);  //serial baud
  lcd.begin(16, 2);

  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Service: ");
  service.initBoard();
  service.initSensor();
  
  
  if(service.findNetwork()){
    service.gprsConnection();
    lcd.setCursor(9,0);
    lcd.print("Online");
  }else{
    lcd.setCursor(9,0);
    lcd.print("Offline");
    while(true);
  }
  
  attachInterrupt(INTRUPT1, isr1, FALLING);
  attachInterrupt(INTRUPT2, isr2, FALLING);
  
  lcd.setCursor(0,1);
  lcd.print("Flow :");

}

void loop(){
  if(!service.isMqttConnected()){
    Serial.println("Connecting MQTT...");
    digitalWrite(ideaBoard_PWRKEY, HIGH);
    delay(180);
    digitalWrite(ideaBoard_PWRKEY, LOW);
    delay(1000);
    service.connectToMqtt();
    delay(100);
  }
  if((millis() - room1.oldTime) > 1000) {   // counter per second 
    detachInterrupt(INTRUPT1); 
    detachInterrupt(INTRUPT2);
    room1.flowVal = room1.flowRate(room1.pulseCount);
    room2.flowVal = room2.flowRate(room2.pulseCount);
    room1.oldTime = millis();
    room2.oldTime = millis();
    room1.flowMlVal = room1.flowML(room1.flowVal);
    room2.flowMlVal = room2.flowML(room2.flowVal);
    room1.totalMlVal = room1.totalFlow(room1.flowMlVal);
    room2.totalMlVal = room2.totalFlow(room2.flowMlVal);
    Serial.println(F("ROOM 1 Reading..."));
    room1.printFunc();
    Serial.println(F("ROOM 2 Reading..."));
    room2.printFunc();
    double r1 = (double) room1.flowMlVal;
    double r2 = (double) room2.flowMlVal;
    //lcd.setCursor(9,1);
    //lcd.print(r1);
    service.sendToPortal(r1, r2);
    lcd.setCursor(6,1);
    lcd.print(room1.flowMlVal);
    lcd.setCursor(10,1);
    lcd.print("ml/s");

    room1.pulseCount = 0;
    room2.pulseCount = 0;
    attachInterrupt(INTRUPT1, isr1, RISING);
    attachInterrupt(INTRUPT2, isr2, RISING);

  }
}
