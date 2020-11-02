#include <CayenneMQTTESP8266.h>
#include <LedREST.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <AutoConnect.h>
ESP8266WebServer Server;
AutoConnect Portal(Server);

#define proxSensorPin D1

boolean led1State = false; //default off


const char* ssid = "........";
const char* password = "........";
const char* mqtt_server = "localhost:1883";

WiFiClient espClient;
PubSubClient client(espClient);
unsigned long lastMsg = 0;
#define MSG_BUFFER_SIZE  (50)
char msg[MSG_BUFFER_SIZE];


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
    client.setServer(mqtt_server, 1883);
    client.setCallback(callback);
  } 
}
void loop() {
  Portal.handleClient();
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  unsigned long now = millis();
  if (now - lastMsg > 2000) {
    lastMsg = now;
    ++value;
    snprintf (msg, MSG_BUFFER_SIZE, "hello world #%ld", value);
    Serial.print("Publish message: ");
    Serial.println(msg);
    client.publish("outTopic", msg);
  }
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

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Create a random client ID
    String clientId = "ESP8266Client-";
    clientId += String(random(0xffff), HEX);
    // Attempt to connect
    if (client.connect(clientId.c_str())) {
      Serial.println("connected");
      // Once connected, publish an announcement...
      client.publish("outTopic", "hello world");
      // ... and resubscribe
      client.subscribe("inTopic");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();

  // Switch on the LED if an 1 was received as first character
  if ((char)payload[0] == '1') {
    digitalWrite(BUILTIN_LED, LOW);   // Turn the LED on (Note that LOW is the voltage level
    // but actually the LED is on; this is because
    // it is active low on the ESP-01)
  } else {
    digitalWrite(BUILTIN_LED, HIGH);  // Turn the LED off by making the voltage HIGH
  }

}
