#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>
#include <SPI.h>
#include <MFRC522.h>

#ifndef STASSID
#define STASSID ""
#define STAPSK ""
#endif

#define SS_PIN 4  //D2
#define RST_PIN 5 //D1

const char* ssid = STASSID;
const char* password = STAPSK;
// ESP8266WiFiMulti WiFiMulti;

const String host = "";
// const String port = "";

MFRC522 mfrc522(SS_PIN, RST_PIN);   // Create MFRC522 instance.
int successRead;
// String uid = "";

int getId() {
  if ( ! mfrc522.PICC_IsNewCardPresent()) {
    return 0;
  }
  if ( ! mfrc522.PICC_ReadCardSerial()) {
    return 0;
  }

  // uid = "";
  byte letter;
  for (byte i = 0; i < mfrc522.uid.size; i++) {
  //  uid.concat(String(mfrc522.uid.uidByte[i], HEX));
  }
  mfrc522.PICC_HaltA(); // Stop reading
  return 1;
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  // Serial.setDebugOutput(true);
  Serial.println();
  Serial.println();
  Serial.println();

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  SPI.begin();      // Initiate  SPI bus
  mfrc522.PCD_Init();   // Initiate MFRC522

  while(WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println();
  Serial.println("Connected!");
  Serial.println();
}

void loop() {
  // put your main code here, to run repeatedly:

  if (WiFi.status() != WL_CONNECTED) {
    ESP.restart();
  }

  //do{
  //  successRead = getId();
  //} while (!successRead);

  if ( ! mfrc522.PICC_IsNewCardPresent()) {
    return;
  }
  if ( ! mfrc522.PICC_ReadCardSerial()) {
    return;
  }

  String uid = "";
  byte letter;
  for (byte i = 0; i < mfrc522.uid.size; i++) {
    uid.concat(String(mfrc522.uid.uidByte[i], HEX));
  }
  mfrc522.PICC_HaltA(); // Stop reading

  Serial.print("UID: ");
  Serial.println(uid);
  
  WiFiClient client;
  HTTPClient http;  //Declare an object of class HTTPClient

  String url = "http://";
  url += host;
  // url += ":";
  // url += port;
  url += "/?uid=";
  url += uid;
  if (http.begin(client, url)) {  //Specify request destination
    int httpCode = http.GET();  //Send the request

    if (httpCode > 0) { //Check the returning code
      // file found at server
      if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY) {
        String payload = http.getString();
        Serial.println(payload);
      }
    } else {
      Serial.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
    }
    http.end();   //Close connection
  } else {
    Serial.printf("[HTTP} Unable to connect\n");
  }
    
   Serial.println();
    
  delay(3000);    //Wait 30 seconds
}
