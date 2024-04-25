// import external libraries
#include "DHT.h"
#include "ArduinoJson.h"

// define pins
#define DHTPIN 2
#define DHTTYPE DHT11
#define LDRPIN A0
#define SOIL_MOISTUREPIN A1
#define WATER_LEVELPIN A2
#define RELAY1_PIN1 8   // heat pump
#define RELAY1_PIN2 9   // cooler fan
#define RELAY2_PIN1 10  // light
#define RELAY2_PIN2 11  // water pump

// define limit values
const float lightLimitValue = 25.0;
const float soilMoistureLimitValue = 15.0;
const float soilMoistureNormalValue = 45.0;
const float waterLevelLimitValue = 10.0;
const float temperatureUpperLimitValue = 45.0;
const float temperatureNormalValue = 32.5;
const float temperatureLowerLimitValue = 20.0;

// define dht
DHT dht(DHTPIN, DHTTYPE);

// define multitasking variables
const int timeInterval = 10000;
unsigned long previousMillis = 0;

void setup() {
  pinMode(RELAY1_PIN1, OUTPUT);
  pinMode(RELAY1_PIN2, OUTPUT);
  pinMode(RELAY2_PIN1, OUTPUT);
  pinMode(RELAY2_PIN2, OUTPUT);
  pinMode(13, OUTPUT);
  // Since the relay works when triggered with low level, here it is triggered with high level.
  digitalWrite(RELAY1_PIN1, HIGH);
  digitalWrite(RELAY1_PIN2, HIGH);
  digitalWrite(RELAY2_PIN1, HIGH);
  digitalWrite(RELAY2_PIN2, HIGH);
  digitalWrite(13, LOW);
  Serial.begin(9600);
  dht.begin();
}

void loop() {
  // assign json datas with sensor datas
  float humidity = readHumidity();
  float celcius = readTemperatureCelcius();
  float fahrenheit = readTemperatureFahrenheit();
  float lightLevel = readLightLevel();
  float soilMoisture = readSoilMoisture();
  float waterLevel = readWaterLevel();

  StaticJsonDocument<200> jsonData;
  jsonData["humidity"] = humidity;
  jsonData["temperatureAsC"] = celcius;
  jsonData["temperatureAsF"] = fahrenheit;
  jsonData["lightAmount"] = lightLevel;
  jsonData["soilMoisture"] = soilMoisture;
  jsonData["waterLevel"] = waterLevel;
  //jsonData["operationType"] = 0;  // when multitasking is done this line will gonna remove

  char serializedData[512];
  
  unsigned long currentMillis = millis();
  //Serial.println(timeInterval)

  // periodic data sending
  if (currentMillis - previousMillis >= timeInterval) {
    // set json operationType to 0
    // write serializedData to serial
    previousMillis = currentMillis;
    jsonData["operationType"] = 0;
    serializeJson(jsonData, serializedData);
    Serial.println(serializedData);
  }
  // actions when the temperature is low
  if (celcius <= temperatureLowerLimitValue) {
    // set json operationType to 1
    // write serializedData to serial
    // open heater
    jsonData["operationType"] = 1;
    serializeJson(jsonData, serializedData);
    Serial.println(serializedData);
    // turn on the heater until the temperature returns to normal
    digitalWrite(RELAY1_PIN1, LOW);
    while (readTemperatureCelcius() >= temperatureNormalValue) {
      delay(2000);
    }
    digitalWrite(RELAY1_PIN1, HIGH);
  }
  // actions when the temperature is high
  if (celcius >= temperatureUpperLimitValue) {
    // set json operationType to 2
    // write serializedData to serial
    // open cooler fan
    jsonData["operationType"] = 2;
    serializeJson(jsonData, serializedData);
    Serial.println(serializedData);
    // turn on the cooler fan until the temperature returns to normal
    digitalWrite(RELAY1_PIN2, LOW);
    while (readTemperatureCelcius() <= temperatureNormalValue) {
      delay(2000);
    }
    digitalWrite(RELAY1_PIN2, HIGH);
  }
  // actions when the light is low/high
  if (lightLevel <= lightLimitValue) {
    // set json operationType to 3
    // write serializedData to serial
    // open lights
    jsonData["operationType"] = 3;
    serializeJson(jsonData, serializedData);
    Serial.println(serializedData);
    digitalWrite(RELAY2_PIN1, LOW);
  } else {
    digitalWrite(RELAY2_PIN1, HIGH);
  }
  // actions when the soil moisture is low
  if (soilMoisture <= soilMoistureLimitValue) {
    // set json operationType to 4
    // write serializedData to serial
    // open water pump
    jsonData["operationType"] = 4;
    serializeJson(jsonData, serializedData);
    Serial.println(serializedData);
    // turn on the water pump until the soil moisture returns normal
    digitalWrite(RELAY2_PIN2, LOW);
    while(readSoilMoisture() <= soilMoistureNormalValue) {
      delay(2000);
    }
    digitalWrite(RELAY2_PIN2, HIGH);
  }
  // actions when the water level in tank is low
  if (waterLevel <= waterLevelLimitValue){
    // set json operationType to 5
    // write serializedData to serial
    // open water pump//
    jsonData["operationType"] = 5;
    serializeJson(jsonData, serializedData);
    Serial.println(serializedData);
    digitalWrite(13,HIGH);
    while(readWaterLevel() <= waterLevelLimitValue) {
      delay(2000);
    }
    digitalWrite(13, LOW);
  }
  delay(2000);
}

// sensor data read functions
float readHumidity() {
  float humidity = dht.readHumidity();
  if (!isnan(humidity)) {
    return humidity;
  } else {
    Serial.println("Failed to read humidity from DHT sensor!");
    return NAN;
  }
}

float readTemperatureCelcius() {
  float celcius = dht.readTemperature();
  if (!isnan(celcius)) {
    return celcius;
  } else {
    Serial.println("Failed to read temperature as celcius from DHT sensor!");
    return NAN;
  }
}

float readTemperatureFahrenheit() {
  float fahrenheit = dht.readTemperature(true);
  if (!isnan(fahrenheit)) {
    return fahrenheit;
  } else {
    Serial.println("Failed to read temperature as fahrenheit from DHT sensor!");
    return NAN;
  }
}

float readLightLevel() {
  float lightLevel = (analogRead(LDRPIN) / 1023.0) * 100;
  return lightLevel;
}

float readSoilMoisture() {
  float soilMoisture = 100.0 - ((analogRead(SOIL_MOISTUREPIN) / 1023.0) * 100);
  return soilMoisture;
}

float readWaterLevel() {
  float waterLevel = (analogRead(WATER_LEVELPIN) / 1023.0) * 100;
  return waterLevel;
}