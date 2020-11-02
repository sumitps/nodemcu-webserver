#include <LedREST.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <AutoConnect.h>
ESP8266WebServer Server;
AutoConnect Portal(Server);

#define proxSensorPin D1

boolean led1State = false; //default off

LedREST led(LED_BUILTIN);

void setup() {
  delay(1000);
  Serial.begin(115200);
  Serial.println();
  
  //Pins
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(proxSensorPin, INPUT);
  
  //Pages
  Server.on("/", rootPage);
  Server.on("/led1/on", led1On);
  Server.on("/led1/off", led1Off);
  Server.on("/led1/state", led1CheckState);
  Server.on("/proximity", proximityValue);
  
  //Begin Portal
  if (Portal.begin()) {
    Serial.println("HTTP server:" + WiFi.localIP().toString());
  }
}
void loop() {
  Portal.handleClient();
}

void rootPage() {
  Server.send(200, "application/json", defResponse("Hello World!"));
}

void led1On() {
  Server.send(200, "application/json", ledResponse(led.on()));
}

void led1Off() {
  Server.send(200, "application/json", ledResponse(led.off()));
}

void led1CheckState() {
  Server.send(200, "application/json", ledResponse(led.state()));
}

void proximityValue() {
  int state = digitalRead(proxSensorPin);
  if (state == HIGH) {
    Server.send(200, "application/json", proximityResponse("true"));
  } else {
    Server.send(200, "application/json", proximityResponse("false"));
  }
}

String ledResponse(boolean state){
  String content = "off";
  if(state) { content = "on";}
  return "{\"ledState\":\"" + content + "\"}";
}

String proximityResponse(String state){
  return "{\"proximityState\":\"" + state + "\"}";
}

String defResponse(String content){
  return "{\"Message\":\"" + content + "\"}";
}
