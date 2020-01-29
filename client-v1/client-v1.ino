#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <SPI.h>
#include <MFRC522.h>
#include <ArduinoJson.h>

#ifndef STASSID
#define STASSID "" // Wifi SSID
#define STAPSK ""  // Wifi password
#endif

#define SS_PIN 4  // D2 (esp) => SS_pin (rc522)
#define RST_PIN 5 // D1 (esp) => RST_pin (rc522)

// Set the Wifi ssid and password to connect to
const char* ssid = STASSID;
const char* password = STAPSK;

// Create MFRC522 instance
MFRC522 mfrc522(SS_PIN, RST_PIN);

// Set the Byte size allocated for JSON
StaticJsonDocument<200> doc;

// Request number increments for every new request send
unsigned int request = 0;

void setup() {
  // put your setup code here, to run once:

  // DEBUG: Serial
  Serial.begin(115200); // DEBUG
  Serial.println(); // DEBUG
  Serial.println(); // DEBUG
  Serial.println(); // DEBUG

  // Initiate Wifi connexion
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  SPI.begin(); // Initiate  SPI bus
  mfrc522.PCD_Init(); // Initiate MFRC522

  // Wait for wifi connexion established
  while(WiFi.status() != WL_CONNECTED) {
    Serial.print("."); // DEBUG
    delay(500);
  }
  
  Serial.println(); // DEBUG
  Serial.println("Connected!"); // DEBUG
  Serial.println(); // DEBUG
  
}

void loop() {
  // put your main code here, to run repeatedly:

  // Check if wifi is still connected
  if (WiFi.status() != WL_CONNECTED) {
    ESP.restart();
  }

  // If card is detected and readable
  if ( ! mfrc522.PICC_IsNewCardPresent()) {
    if ( ! mfrc522.PICC_ReadCardSerial()) {

      // Store the RFID tag UID in uid
      String uid = "";
      byte letter;
      for (byte i = 0; i < mfrc522.uid.size; i++) {
        uid.concat(String(mfrc522.uid.uidByte[i], HEX));
      }
      mfrc522.PICC_HaltA(); // Stop reading
    
      Serial.print("UID: "); // DEBUG
      Serial.println(uid); // DEBUG

      // Send data to server in JSON format

      // Create a JSON object to send data
      JsonObject toSend = doc.to<JsonObject>();
      // Increment request by 1
      request++;
      // Fill the JSON object to send
      toSend["request"] = request; // generate a request number
      toSend["uid"] = uid; // the RFID tag UID to be send
      toSend["token"] = "yay"; // put esp token here
      
    }
  }

}
