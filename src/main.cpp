#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <WiFiManager.h>  
#include <ESP8266WebServer.h>
#include <DNSServer.h>
#include <OneWire.h>
#include <DallasTemperature.h>

ESP8266WebServer server(80);
#define ONE_WIRE_BUS 5

// Setup a oneWire instance to communicate with any OneWire devices
OneWire oneWire(ONE_WIRE_BUS);

// Pass our oneWire reference to Dallas Temperature sensor 
DallasTemperature sensors(&oneWire);

int loopCount = 0;

float temperature = 0; 
void promData(){
  String result = "temperature " + String(temperature);
  server.send(200, "text/plain", result);
}

void setup(){
  Serial.begin(115200);
  WiFiManager wifiManager;
  wifiManager.autoConnect("Thermometer");

  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  server.on("/data", promData);
  server.begin();
  sensors.begin();
}

void loop(){
  server.handleClient();
  if(loopCount % 20 == 0){
  sensors.requestTemperatures(); 
  temperature = sensors.getTempCByIndex(0);
  Serial.println(temperature, 1);
  }
  delay(100);
  loopCount++;
}