#include "Services.h"
#include <SoftwareSerial.h>
#include <PubSubClient.h>
#include <TinyGsmClient.h>
#include <SoftwareSerial.h>
#include <LiquidCrystal.h>

SoftwareSerial SerialSIM(ideaBoard_TX, ideaBoard_RX);
TinyGsm modem(SerialSIM);
TinyGsmClient client(modem);
PubSubClient mqtt(client);

String clientId = "";
char getmsg[100];
char setmsg[100];
String on = "{\"action\":\"open\",\"param\":{\"mac\":\"9647632068185842\"}}";
String off = "{\"action\":\"close\",\"param\":{\"mac\":\"9647632068185842\"}}";
String cpsi;

void Services:: initBoard(){
    pinMode(ideaBoard_PWRKEY, OUTPUT);
    digitalWrite(ideaBoard_PWRKEY, HIGH);
    delay(10);
    
    SerialSIM.begin(4800);
    modem.setNetworkMode(13);  // 13-GSM 38-NB-IoT
    //modem.setPreferredMode(2); // for GSM commend this..

    modem.restart();
    delay(2000);
}
void Services:: initSensor(){
  pinMode(SENSOR1, INPUT);
  digitalWrite(SENSOR1, HIGH);
  pinMode(SENSOR2, INPUT);
  digitalWrite(SENSOR2, HIGH);
}
void callBack(char* t, byte* payload, unsigned int l){
    for (unsigned int i = 0; i < l; i++){
        getmsg[i] = (char)payload[i];
    }
    if(on == getmsg){
        Serial.println(F("Action : OPEN"));
        // servo Open functions here...

    }else if(off == getmsg){
        Serial.println(F("Action : CLOSE"));
        // servo close functions here...

    }
    delay(1000);
    //memset(getmsg, 0, sizeof(getmsg));
}
void Services::gprsConnection(){
    if(!modem.gprsConnect(NET_APN,APN_USER,APN_PASS)){
        Serial.println(F("GPRS Fail..."));
    }else{
        Serial.println(F("GPRS Connected."));
        Serial.println(getCpsi());
        mqtt.setServer(MQTT_BROKER,MQTT_PORT);
        mqtt.setCallback(callBack);
        // mqtt.setCallback([this] (char* topic, byte* payload, unsigned int length) {
        //     this->callBack(topic, payload, length); 
        // });
    }
    
}
boolean Services:: findNetwork() {
    Serial.println(F("Finding Network..."));
    
    if(!modem.waitForNetwork()) {
        Serial.println(F("Network Fail"));
        //while(true);
        return false;
        
    }else {
        Serial.println(F("Network identified."));
        Serial.print(F("Signal Strength : "));
        Serial.println(modem.getSignalQuality());
        // gprsConnection();
        return true;
    }
}
String setMsg( double r1, double r2){
    String msg = "{\"eventName\":\"Home\",\"status\":\"\",\"r1\":";
    msg += String(r1,2);
    msg += ",\"r2\":";
    msg += String(r2,2);
    msg += ",\"mac\":\"9647632068185842\"}";
    return msg;
}

void Services:: sendToPortal(double& r1, double& r2){
//sprintf(setmsg,"{\"eventName\":\"Home\",\"status\":\"\",\"r1\":%uf,\"r2\":%s,\"mac\":\"9647632068185842\"}",R,String(r2,2));
    if (!mqtt.publish(PUB_TOPIC, setMsg(r1,r2).c_str())){
        Serial.println(F("Failed."));
    }else{
        Serial.println(setMsg(r1,r2));
        Serial.println(F("SEND:OK"));
    }
    delay(1000);
}
boolean Services::connectToMqtt(){
    randomSeed(analogRead(5)); //analog pin 5 used to genarate random number
    clientId = "TEST-" + String(millis()) + String(random(0, 100000000), HEX);
  if (!mqtt.connect(clientId.c_str(), MQTT_USERNAME, MQTT_PASSWORD)){
    Serial.println("MQTT fail");
    return false;
  }
  Serial.println(SUB_TOPIC);
  mqtt.subscribe(SUB_TOPIC);
  return mqtt.connected();
}
boolean Services:: isMqttConnected(){
    if(mqtt.connected()){
        mqtt.loop();
        return true;
    }else{
        return false;
    }
}
String Services::getCpsi(){
    SerialSIM.println("AT+CPSI?");
    delay(500);
    while (SerialSIM.available()){
        cpsi = SerialSIM.readString();
    }
    delay(500);
    return cpsi;
}
