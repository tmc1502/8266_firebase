
#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <FirebaseESP8266.h>

#define WIFI_SSID "311 HHN LAU 2"//chua va odc wiffi
#define WIFI_PASSWORD "@@1234abcdlau2"
#define FIREBASE_HOST "test-web-19188-default-rtdb.firebaseio.com"
#define FIREBASE_AUTH "CYLaYgWBR6pMghRCtCUbaQoDNfMmhLU9N68fcxhu"

FirebaseData firebaseData;
FirebaseConfig firebaseConfig;
FirebaseAuth firebaseAuth;

void setup() {
  Serial.begin(115200);

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();

  Serial.println(":");
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
    int ledStatus[9];
    sscanf(data.c_str(), "%d,%d,%d,%d", &ledStatus[0], &ledStatus[1], &ledStatus[2], &ledStatus[3]);

    int test = ledStatus[9];
    Serial.println(":");

      Firebase.setInt(firebaseData, "/A1", ledStatus[0]);
      Firebase.setInt(firebaseData, "/A2", ledStatus[1]);
      Firebase.setInt(firebaseData, "/A3", ledStatus[2]);
      Firebase.setInt(firebaseData, "/A4", ledStatus[3]);
      Firebase.setInt(firebaseData, "/A5", ledStatus[4]);

      Firebase.setInt(firebaseData, "/B1", ledStatus[5]);
      Firebase.setInt(firebaseData, "/B2", ledStatus[6]);
      Firebase.setInt(firebaseData, "/B3", ledStatus[7]);
      Firebase.setInt(firebaseData, "/B4", ledStatus[8]);
      Firebase.setInt(firebaseData, "/B5", ledStatus[9]);

  }

  delay(1000);
}
