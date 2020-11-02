#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <AutoConnect.h>
ESP8266WebServer Server;
AutoConnect Portal(Server);

boolean led1State = false; //default off

void setup() {
  delay(1000);
  Serial.begin(115200);
  Serial.println();
  
  //Pins
  pinMode(LED_BUILTIN, OUTPUT);
  
  //Pages
  Server.on("/", rootPage);
  Server.on("/led1/on", led1On);
  Server.on("/led1/off", led1Off);
  Server.on("/led1/state", led1CheckState);
  
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
  digitalWrite(LED_BUILTIN, LOW);
  led1State = true;
  Server.send(200, "application/json", ledResponse(true));
}

void led1Off() {
  digitalWrite(LED_BUILTIN, HIGH);
  led1State = false;
  Server.send(200, "application/json", ledResponse(false));
}

void led1CheckState() {
  Server.send(200, "application/json", ledResponse(led1State));
}

String ledResponse(boolean state){
  String content = "off";
  if(state) { content = "on";}
  return "{\"ledState\":\"" + content + "\"}";
}

String defResponse(String content){
  return "{\"Message\":\"" + content + "\"}";
}
