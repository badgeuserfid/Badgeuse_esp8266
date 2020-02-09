#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>

#ifndef STASSID
#define STASSID "" // Wifi SSID
#define STAPSK ""  // Wifi password
#endif

// Set the Wifi ssid and password to connect to
const char* ssid = STASSID;
const char* password = STAPSK;
// Set the mqtt server to connect to
const char* mqtt_server = "test.mosquitto.org";
const int mqtt_port = 1883;

// Define the wifi client and mqtt Classes
WiFiClient espClient;
PubSubClient client(espClient);

// the mqtt message container
char msg[200];

// Set the Byte size allocated for JSON
StaticJsonDocument<200> doc;

// Request number increments for every new request send
unsigned int request = 0;

// Delay without delay
unsigned long previousMillis = 0;
const long interval = 1000;

// callback for subscribed mqtt topics
void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived ["); // DEBUG
  Serial.print(topic); // DEBUG
  Serial.print("] "); // DEBUG
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]); // DEBUG
  }
  Serial.println(); // DEBUG
}

void reconnect() {
  /*
   * Do it with delay w/o delay
   */
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection..."); // DEBUG
    // Attempt to connect
    if (client.connect("ESP8266Client-12222")) {
      Serial.println("connected"); // DEBUG
      // Once connected, publish an announcement...
      client.publish("mytopic", "hello world");
      // ... and resubscribe
      client.subscribe("inTopic");
    } else {
      Serial.print("failed, rc="); // DEBUG
      Serial.print(client.state()); // DEBUG
      Serial.println(" try again in 5 seconds"); // DEBUG
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}


void setup() {
  // put your setup code here, to run once:

  // DEBUG: Serial
  Serial.begin(115200); // DEBUG

  // Initiate Wifi connexion
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  // Wait for wifi connexion established
  while(WiFi.status() != WL_CONNECTED) {
    Serial.print("."); // DEBUG
    delay(500);
  }

  Serial.println(); // DEBUG
  Serial.println("Connected!"); // DEBUG
  Serial.println(); // DEBUG

  // Create the connexion to the mqtt server
  client.setServer(mqtt_server, mqtt_port);
  // set the callback function for subscribed topics
  client.setCallback(callback);

}

void loop() {
  // put your main code here, to run repeatedly:
  unsigned long currentMillis = millis(); // Dw/oD

  // Check if wifi is still connected
  if (WiFi.status() != WL_CONNECTED) {
    ESP.restart();
  }
  if (!client.connected()) {
    reconnect();
  }
  // allow client to process incoming messages and maintain connexion
  client.loop();

  if (currentMillis - previousMillis > interval) {
    // Create a JSON object to send data
    JsonObject toSend = doc.to<JsonObject>();
    // Increment request by 1
    request++;
    // Fill the JSON object to send
    toSend["request"] = request; // generate a request number
    toSend["uid"] = 123469046; // the RFID tag UID to be send
    toSend["token"] = "yay"; // put esp token here
    // stringify JSON object
    serializeJson(toSend, msg);

    client.publish("mytopic", msg);
    previousMillis = currentMillis;
  }

}
