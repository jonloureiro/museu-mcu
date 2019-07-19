#include <Arduino.h>
#include <WiFiClientSecure.h>


void connectWiFi();
String request(const char* server, const char* end_point, String body);


const char* ssid     = "YOURSSID";
const char* password = "YOURPASSWORD";
const char* server   = "jonloureiro-museu.herokuapp.com";


WiFiClientSecure client;
String token;


void setup() {
  Serial.begin(9600);
  delay(1000);

  connectWiFi();

  if (!client.connect(server, 443)) {
    Serial.println("Connection failed");
    return;
  }
  token = request(server, "/mcu/connect", "{\"name\": \"esp32\"}");
  Serial.println("Authenticated MCU");
  client.stop();
}


void loop() {
  // put your main code here, to run repeatedly:
}


void connectWiFi() {
  Serial.print("\nAttempting to connect to SSID ");
  Serial.print(ssid);
  WiFi.begin(ssid, password);

  for (int i = 0; i < 10; i++) {
    if (WiFi.status() == WL_CONNECTED) {
      Serial.println("\nConnected to WiFi");
      return;
    }
    Serial.print(".");
    delay(500);
  }

  connectWiFi();
}


String request(const char* server, const char* end_point, String body) {
  client.print  ("POST ");
  client.print  (end_point);
  client.println(" HTTP/1.1");
  client.print  ("Host: ");
  client.println(server);
  client.println("Content-Type: application/json");
  client.println("Accept: */*");
  client.print  ("Content-Length: ");
  client.println(body.length());
  client.println();
  client.println(body);
  client.println();

  while (client.connected()) {
    String line = client.readStringUntil('\n');
    // Serial.println(line);
    if (line == "\r") break;
  }

  if (client.available())
    return client.readString();
  return String("");
}