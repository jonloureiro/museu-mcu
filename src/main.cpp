#include <Arduino.h>
#include <WiFiClientSecure.h>

void connectWiFi();

const char* ssid         = "YOURSSID";
const char* password     = "YOURPASSWORD";

void setup() {
  Serial.begin(9600);
  delay(1000);

  connectWiFi();
}

void loop() {
  // put your main code here, to run repeatedly:
}

void connectWiFi() {
  Serial.print("\nAttempting to connect to SSID ");
  Serial.print(ssid);
  WiFi.begin(ssid, password);

  for(int i = 0; i < 10; i++) {
    if (WiFi.status() != WL_CONNECTED) {
      Serial.print("\nConnected");
      return;
    }
    Serial.print(".");
    delay(500);
  }

  connectWiFi();
}

