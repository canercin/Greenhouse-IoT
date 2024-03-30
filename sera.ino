//import external libraries
#include "DHT.h"

//define pins
#define DHTPIN 2
#define DHTTYPE DHT11
#define LDRPIN A0
#define SOIL_MOISTUREPIN A1
#define WATER_LEVELPIN A2
#define RELAY1_PIN 8

//define variables
const unsigned long routineHttpSendInterval = 900000;
unsigned long previousTime = 0;
const unsigned float temperatureThresholdValue = 20.0;
const unsigned float lightAmountThresholdValue = 35.0;


//define dht
DHT dht(DHTPIN, DHTTYPE);

void setup() {
  // put your setup code here, to run once:
  pinMode(13, OUTPUT);
  digitalWrite(13, LOW);
  Serial.begin(9600);
  dht.begin();
  pinMode(RELAY1_PIN, OUTPUT);
  digitalWrite(RELAY1_PIN, OUTPUT);
}

void loop() {
  //get current time milis
  unsigned long currentTime = millis();
  //routine data sender
  if (currentTime - previousTime >= routineHttpSendInternal) {
    /*
    * send data with opcode 0
    * */
    previousTime = currentTime;
  }
  //if the temperature is lower than the threshold, start the heat engine and send data
  if (readTemperatureCelcius() <= temperatureThresholdValue) {
    /*
      send data with opcode 1 and run heat engine
    */
  }
  //If the amount of light is low, turn on the light and send the data
  if (readLightLevel() <= lightAmountThresholdValue) {
    /*
      send data with opcode 2 and open lights
    */
  }
  readHumidity();
  readTemperatureCelcius();
  readTemperatureFahrenayt();

  Serial.print("Light level: ");
  Serial.println(map(analogRead(LDRPIN), 0, 1023, 0, 100));

  Serial.print("Soil Moisture: ");
  Serial.println(map(analogRead(SOIL_MOISTUREPIN), 0, 1023, 100, 0));

  Serial.print("Water level: ");
  Serial.println(map(analogRead(WATER_LEVELPIN), 0, 1023, 0, 100));

  Serial.println();

  delay(5000);
}
/*

*/
float readHumidity() {
  float humidity = dht.readHumidity();
  if (!isnan(humidity)) {
    Serial.print("Humidity %");
    Serial.println(humidity);
    return humidity;
  } else {
    Serial.println("Failed to read humidity from DHT sensor!");
    return NAN;
  }
}

float readTemperatureCelcius() {
  float celcius = dht.readTemperature();
  if (!isnan(celcius)) {
    Serial.print("Celcius °C");
    Serial.println(celcius);
    return celcius;
  } else {
    Serial.println("Failed to read temperature as celcius from DHT sensor!");
    return NAN;
  }
}

float readTemperatureFahrenheit() {
  float celcius = dht.readTemperature(true);
  if (!isnan(celcius)) {
    // Fahrenheit cinsinden sıcaklık değerini Serial'e yazdırın
    float fahrenheit = (celcius * 9.0 / 5.0) + 32.0;
    Serial.print("Fahrenheit °F");
    Serial.println(fahrenheit);
    return fahrenheit;
  } else {
    Serial.println("Failed to read temperature as fahrenheit from DHT sensor!");
    return NAN;
  }
}

void readLightLevel() {
  int lightLevel = map(analogRead(LDRPIN), 0, 1023, 0, 100);
  Serial.print("Light level: ");
  Serial.println(lightLevel);
}

void readSoilMoisture() {
  int soilMoisture = map(analogRead(SOIL_MOISTUREPIN), 0, 1023, 100, 0);
  Serial.print("Soil Moisture: ");
  Serial.println(soilMoisture);
}

void readWaterLevel() {
  int waterLevel = map(analogRead(WATER_LEVELPIN), 0, 1023, 0, 100);
  Serial.print("Water level: ");
  Serial.println(waterLevel);
}
