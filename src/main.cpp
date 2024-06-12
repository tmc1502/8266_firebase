#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <FirebaseESP8266.h>
#include <ArduinoJson.h>

// Wi-Fi and Firebase credentials
#define WIFI_SSID "311 HHN LAU 2" // Replace with your Wi-Fi SSID
#define WIFI_PASSWORD "@@1234abcdlau2" // Replace with your Wi-Fi password
#define FIREBASE_HOST "test-web-19188-default-rtdb.firebaseio.com" // Replace with your Firebase database URL
#define FIREBASE_AUTH "CYLaYgWBR6pMghRCtCUbaQoDNfMmhLU9N68fcxhu" // Replace with your Firebase authentication token

FirebaseData firebaseData;
FirebaseConfig firebaseConfig;
FirebaseAuth firebaseAuth;

void setup() {
  Serial.begin(115200);

  // Connect to Wi-Fi
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();

  // Configure Firebase
  firebaseConfig.host = FIREBASE_HOST;
  firebaseConfig.signer.tokens.legacy_token = FIREBASE_AUTH;

  Firebase.begin(&firebaseConfig, &firebaseAuth);
  Firebase.reconnectWiFi(true);
}

void loop() {
  if (Serial.available() > 0) {
    String jsonData = Serial.readStringUntil('\n');
    jsonData.trim();
    Serial.println("Received data: " + jsonData); 

    // Parse the JSON data
    StaticJsonDocument<256> jsonDoc;
    DeserializationError error = deserializeJson(jsonDoc, jsonData);

    if (error) {
      Serial.print("JSON parsing failed: ");
      Serial.println(error.f_str());
      return;
    }

    JsonObject ledStatus = jsonDoc["ledStatus"];
    Serial.println("Updating Firebase...");

    // Create a JSON object for Firebase
    FirebaseJson firebaseJson;
    for (JsonPair kv : ledStatus) {
      firebaseJson.set("/" + String(kv.key().c_str()), kv.value().as<int>());
    }

    // Update Firebase in one go
    if (Firebase.updateNode(firebaseData, "/", firebaseJson)) {
      Serial.println("Firebase updated successfully");
    } else {
      Serial.println("Error updating Firebase: " + firebaseData.errorReason());
    }
  }

  delay(500);
}
