#include <Arduino.h>
#include <Wire.h>
#include <BH1750.h>

#define debug true // If true - debug info will be shown in console

BH1750 lightSensor;
// Function to get light level in lux from BH1750 sensor with auto enviroment adjustment
float getLight();

// Function in loop cycle called only if "debug" is true
void debugLoop(int);

void setup() {
  Serial.begin(9600);

  Wire.begin(); // I2C for BH1750
  lightSensor.begin(BH1750::ONE_TIME_HIGH_RES_MODE);


}

void loop() {
  if (debug) debugLoop(1000);
}

float getLight(){
  float lux = lightSensor.readLightLevel(true);
  // AutoLight measurement time adjustment for different lihgt enviroment
  if(lux < 0) Serial.println("Error reading ligt level");
  else {
    if (lux > 40000.0){
      // reduce measurement time - when on direct sun light
      if (lightSensor.setMTreg(32)){
        if (debug) Serial.println(F("Setting MTReg to low value for high light environment"));
      }
      else {
        if(debug) Serial.println(F("Error setting MTReg to low value for high light environment"));
      }
    }
    else {
      if (lux > 10.0){
        // regular light enviroment
        if (lightSensor.setMTreg(69)){
          if (debug) Serial.println(F("Setting MTReg to default value for normal light environment"));
        }
        else {
          if (debug) Serial.println(F("Error setting MTReg to default value for normal light environment"));
        }
      }
      else {
        if (lux <= 10.0){
          // very low light enviroment
          if (lightSensor.setMTreg(138)){
            if (debug) Serial.println(F("Setting MTReg to high value for low light environment"));
          }
          else {
            if (debug) Serial.println(F("Error setting MTReg to high value for low light environment"));
          }
        }
      }
    }
  }
  return lux;
}

void debugLoop(int delayTime){
  Serial.println("Light level : " + String(getLight()) + " lux");
  delay(delayTime);
}


