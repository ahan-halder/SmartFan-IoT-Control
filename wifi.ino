#include <WiFi.h>

const char* ssid = "Ahan_iPhone";
const char* password = "ahan2005";

void setup() {
  Serial.begin(115200);
  delay(1000);

  WiFi.mode(WIFI_STA);     // important for iPhone
  WiFi.begin(ssid, password);

  Serial.print("Connecting to Hotspot");

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("Connected!");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());
}

void loop() {}
