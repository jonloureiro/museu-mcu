#include <Arduino.h>
#include <WiFiClientSecure.h>


void connectWiFi();
void requestToken();
bool connectServer();
void responseToken();


const char* ssid     = "YOURSSID";
const char* password = "YOURPASSWORD";
const char* server   = "jonloureiro-museu.herokuapp.com";


WiFiClientSecure client;
String token;


void setup() {
  Serial.begin(9600);
  delay(1000);

  connectWiFi();
  requestToken();
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
      Serial.println("\nSucess\n");
      return;
    }
    Serial.print(".");
    delay(500);
  }

  connectWiFi();
}


void requestToken() {
  if (!connectServer()) {
    Serial.println("Connection failed");
    return;
  }
  Serial.println("Sending request...\n");
  client.println("POST /mcu/connect HTTP/1.1");
  client.println("Host: jonloureiro-museu.herokuapp.com");
  client.println("Content-Type: application/json");
  client.println("Accept: */*");
  client.println("Content-Length: 19");
  client.println();
  client.println("{\"name\": \"esp32\"}");
  client.println();
  responseToken();
  client.stop();
}


bool connectServer() {
  for (int i = 0; !client.connect(server, 443); i++) {
    if (i == 4) return false;
    delay(3000);
  }
  Serial.print("Connected to ");
  Serial.println(server);
  Serial.println();
  return true;
}


void responseToken() {
  while (client.connected()) {
    String line = client.readStringUntil('\n');
    // Serial.println(line);
    if (line == "\r") break;
  }


  if (client.available())
    token = client.readString();
  
  Serial.print("Response: ");
  Serial.println(token);
}
