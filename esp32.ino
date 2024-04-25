#include <WiFi.h>
#include <HTTPClient.h>

#define RXPIN 16
#define TXPIN 17

const char* ssid = "ailab";
String serverName = "http://192.168.5.111:8080/api/greenhouse/save";

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial2.begin(9600, SERIAL_8N1, RXPIN, TXPIN);
  WiFi.begin(ssid);
}

void loop() {
    if(Serial2.available()){
      if(WiFi.status() == WL_CONNECTED){
        WiFiClient client;
        HTTPClient http;
        http.begin(client, serverName);

        http.addHeader("Content-Type", "application/json");
        String json = Serial2.readString();
        int httpResponse = http.POST(json);
        Serial.print("HTTP RESPONSE CODE ");
        Serial.println(httpResponse);
        http.end();
      }else{
        Serial.println("wifi diss");
      }
    }
}
