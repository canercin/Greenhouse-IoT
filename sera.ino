#include "DHT.h"

#define DHTPIN 2
#define DHTTYPE DHT11
#define LDRPIN A0
#define SOIL_MOISTUREPIN A1
#define WATER_LEVELPIN A2
#define RELAY1_PIN 8

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

void readHumidity() {
  float humidity = dht.readHumidity();
  if (!isnan(humidity)) {
    Serial.print("Humidity %");
    Serial.println(humidity);
  } else {
    Serial.println("Failed to read humidity from DHT sensor!");
  }
}

void readTemperatureCelcius() {
  float celcius = dht.readTemperature();
  if (!isnan(celcius)) {
    Serial.print("Celcius °C");
    Serial.println(celcius);
  } else {
    Serial.println("Failed to read temperature as celcius from DHT sensor!");
  }
}

void readTemperatureFahrenayt() {
  float celcius = dht.readTemperature(true);
  if (!isnan(celcius)) {
    Serial.print("Fahrenayt °F");
    Serial.println(celcius);
  } else {
    Serial.println("Failed to read temperature as fahrenayt from DHT sensor!");
  }
}
