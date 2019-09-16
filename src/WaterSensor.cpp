#include "WaterSensor.h"

WaterSensor:: WaterSensor(){
  // initiating object goes here
}

void WaterSensor:: pulseCounter(){

  pulseCount++; // for interrupt 1 (sensorpin 1)
}

/* number of pulses per second per units of measure 
(litres/minute in this case) from sensor */
double WaterSensor::flowRate(volatile int &pluseCount){  
  
  return ((1000.0 / (millis() - oldTime)) * pluseCount) / CALIFACT;
}

   //calculating how many ML pass through the Sensor
unsigned int WaterSensor::flowML(double &flowVal){  
    //divide by 60 to find within 1 sec interval
    return (flowVal / 60) * 1000;     // multiply by 1000 to calculate in ML
}
double WaterSensor::totalFlow(unsigned int &flowMlVal){

  totalMlVal += flowMlVal;
  return totalMlVal;
}
void WaterSensor:: printFunc(){

  Serial.print(F("Flow Rate : "));
  Serial.println(flowVal);
  Serial.print(F("Flow ML :"));
  Serial.print(flowMlVal);
  Serial.println(F("L/min"));
  Serial.print(F("Total ML :"));
  Serial.print(totalMlVal);
  Serial.println(F("mL"));
  Serial.println(F("*********************"));

}

