#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <PubSubClient.h>
#include <SPI.h>
#include <MFRC522.h>
#include <ArduinoJson.h>
#include <jled.h>

// MFRC522
#define SS_PIN 4  // D2 (node) => SS_pin (rc522)
#define RST_PIN 5 // D1 (node) => RST_pin (rc522)
// MOSI D7, MISO D6, CLK D5


// Wifi
#ifndef STASSID
#define STASSID "AndroidAP2127" // Wifi SSID
#define STAPSK "0104fcb7665b"  // Wifi password
#endif


// MQTT
#ifndef MQTT_SERV
#define MQTT_SERV ""
#define MQTT_PORT 1883
#endif

// LEDs and Relay
#define L_RED D4
#define L_GREEN D3
#define RELAY D8


////////////////////////////////////////////////////////////
// MFRC52 
////////////////////////////////////////////////////////////

// Create MFRC522 instance
MFRC522 mfrc522(SS_PIN, RST_PIN);


////////////////////////////////////////////////////////////
// JSON 
////////////////////////////////////////////////////////////

// Calculate the capacity neede for JSON objects (3 objects and 64 bytes for data)
//const size_t json_size = JSON_OBJECT_SIZE(3) + 64;


////////////////////////////////////////////////////////////
// Wifi 
////////////////////////////////////////////////////////////

// Set the Wifi ssid and password
const char* ssid = STASSID;
const char* wifi_password = STAPSK;

void wifiSetup() {
  // Initiate Wifi connexion
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, wifi_password);
  // Wait for wifi connexion established
  while(WiFi.status() != WL_CONNECTED) {
    Serial.print("."); // DEBUG
    delay(500);
  }
  Serial.println("\nWifi : Connected!\n"); // DEBUG
}


////////////////////////////////////////////////////////////
// MQTT
////////////////////////////////////////////////////////////
/*
// Set the mqtt server
const char* mqtt_server = MQTT_SERV;
const int mqtt_port = MQTT_PORT;

// Define the wifi client and mqtt Objects
WiFiClient espClient;
PubSubClient client(espClient);

// Request number increments for every new request send
long client_request = 0;

//--------------------------------------------------------//
// Callback
//--------------------------------------------------------//
// This function handle incomming message
void mqttCallback(const char* topic, byte* payload_byte, unsigned int length) {
  Serial.print("MQTT in : "); // DEBUG
  Serial.print(topic); // DEBUG
  Serial.print(" => "); // DEBUG
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload_byte[i]); // DEBUG
  }
  Serial.println(); // DEBUG

  if (strcmp(topic, "")) {
    String payload_str = "";
    // Create a JSON object
    DynamicJsonDocument to_recieve(json_size);
    // Convert the byte* message into String
    for (int i = 0; i < length; i++) {
      payload_str.concat((char)payload_byte[i]);
    }
    // parse JSON string
    deserializeJson(to_recieve, payload_str);
  }
}

void mqttSetup() {
  // Create the connexion to the mqtt server
  client.setServer(mqtt_server, mqtt_port);
  // set the callback function for subscribed topics
  client.setCallback(mqttCallback);
}
*/

////////////////////////////////////////////////////////////
// LEDs and Relay
////////////////////////////////////////////////////////////

// LEDs Effects
// Close
JLed l_close_arr[] = {
  JLed(L_RED).On(),
  JLed(L_GREEN).Off()
};
JLedSequence l_close(JLedSequence::eMode::PARALLEL, l_close_arr);
// Open 2s
JLed l_open_arr[] = {
  JLed(L_RED).Blink(2000, 1).Repeat(1).LowActive(),
  JLed(L_GREEN).Blink(2000, 1).Repeat(1)
};
JLedSequence l_open(JLedSequence::eMode::PARALLEL, l_open_arr);
// Refuse 5s
auto l_refuse = JLed(L_RED).Blink(500, 500).Repeat(5).LowActive();
// New card 10s
JLed l_newcard_arr[] = {
  JLed(L_RED).Blink(500, 500).Repeat(10).LowActive(),
  JLed(L_GREEN).Blink(500, 500).Repeat(10)
};
JLedSequence l_newcard(JLedSequence::eMode::PARALLEL, l_newcard_arr);
// Card added 1.2s
JLed l_cardadded_arr[] = {
  JLed(L_RED).Blink(200, 200).Repeat(3),
  JLed(L_GREEN).Blink(200, 200).Repeat(3),
  JLed(L_RED).On().DelayBefore(1200)
};
JLedSequence l_cardadded(JLedSequence::eMode::PARALLEL, l_cardadded_arr);


////////////////////////////////////////////////////////////
// Setup
////////////////////////////////////////////////////////////
 
void setup() {

  // avoid the leds sequences initiated to run at startup.
  l_close.Stop();
  l_open.Stop();
  l_refuse.Stop();
  l_newcard.Stop();
  l_cardadded.Stop();
  
  Serial.begin(115200); Serial.println();
  
  SPI.begin(); // Initiate  SPI bus
  mfrc522.PCD_Init(); // Initiate MFRC522

  wifiSetup();

  //mqttSetup();

  pinMode(RELAY, OUTPUT);

  l_close.Reset();
}


////////////////////////////////////////////////////////////
// Loop
////////////////////////////////////////////////////////////

void loop() {
  // Check if wifi is still connected
  /*if (WiFi.status() != WL_CONNECTED) {
    ESP.restart();
  }
  // allow client to process incoming messages and maintain connexion
  client.loop();
  */
  // Refresh LEDs status
  
  l_close.Update();
  l_open.Update();
  l_refuse.Update();
  l_newcard.Update();
  l_cardadded.Update();
  
  // If card is detected and readable
  // Reset the loop if no new card present on the sensor/reader. This saves the entire process when idle.
  if ( ! mfrc522.PICC_IsNewCardPresent()) {
    return;
  }

  // Select one of the cards
  if ( ! mfrc522.PICC_ReadCardSerial()) {
    return;
  }
  
  // Store the RFID tag UID in uid
  String uid = "";
  //byte letter;
  for (byte i = 0; i < mfrc522.uid.size; i++) {
    uid.concat(String(mfrc522.uid.uidByte[i], HEX));
  }
  mfrc522.PICC_HaltA(); // Stop reading
  Serial.print("MFRC : UID: "); // DEBUG
  Serial.println(uid); // DEBUG

  l_open.Reset();
}
