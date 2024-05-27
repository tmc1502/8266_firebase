#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <FirebaseESP8266.h>

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
    String data = Serial.readStringUntil('\n');
    data.trim();
    Serial.println("Received data: " + data); 

    int ledStatus[10];
    sscanf(data.c_str(), "A:%d,%d,%d,%d,%d B:%d,%d,%d,%d,%d", &ledStatus[0], &ledStatus[1], &ledStatus[2], &ledStatus[3], &ledStatus[4], &ledStatus[5], &ledStatus[6], &ledStatus[7], &ledStatus[8], &ledStatus[9]);

    Serial.println("Updating Firebase...");

    // Create a JSON object to hold the LED statuses
    FirebaseJson json;
    json.set("/A1", ledStatus[0]);
    json.set("/A2", ledStatus[1]);
    json.set("/A3", ledStatus[2]);
    json.set("/A4", ledStatus[3]);
    json.set("/A5", ledStatus[4]);
    json.set("/B1", ledStatus[5]);
    json.set("/B2", ledStatus[6]);
    json.set("/B3", ledStatus[7]);
    json.set("/B4", ledStatus[8]);
    json.set("/B5", ledStatus[9]);

    // Update Firebase in one go
    if (Firebase.updateNode(firebaseData, "/", json)) {
      Serial.println("Firebase updated successfully");
    } else {
      Serial.println("Error updating Firebase: " + firebaseData.errorReason());
    }
  }

  delay(500);
}
