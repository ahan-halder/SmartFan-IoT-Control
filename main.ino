#include <ESP8266WiFi.h>
#include <ThingSpeak.h>

WiFiClient client;

// ===== WIFI =====
const char* ssid = "YOUR_WIFI";
const char* password = "YOUR_PASS";

// ===== THINGSPEAK =====
unsigned long channelID = YOUR_CHANNEL_ID;
const char* readAPIKey = "YOUR_READ_KEY";
const char* writeAPIKey = "YOUR_WRITE_KEY";

// ===== GPIO PINS =====
#define LOW_PIN D1
#define MED_PIN D2
#define HIGH_PIN D3

int currentState = -1;

void setFanState(int state) {
  // Turn everything OFF first
  digitalWrite(LOW_PIN, LOW);
  digitalWrite(MED_PIN, LOW);
  digitalWrite(HIGH_PIN, LOW);

  if (state == 1) {
    digitalWrite(LOW_PIN, HIGH);
  }
  else if (state == 2) {
    digitalWrite(MED_PIN, HIGH);
  }
  else if (state == 3) {
    digitalWrite(HIGH_PIN, HIGH);
  }

  currentState = state;
}

void setup() {
  Serial.begin(115200);

  pinMode(LOW_PIN, OUTPUT);
  pinMode(MED_PIN, OUTPUT);
  pinMode(HIGH_PIN, OUTPUT);

  WiFi.begin(ssid, password);

  Serial.print("Connecting...");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nConnected!");
  ThingSpeak.begin(client);
}

void loop() {

  int command = ThingSpeak.readIntField(channelID, 1, readAPIKey);

  if (command != currentState) {
    Serial.print("New Command: ");
    Serial.println(command);

    setFanState(command);

    // Send feedback
    ThingSpeak.writeField(channelID, 2, command, writeAPIKey);
  }

  delay(5000); // Poll every 5 sec
}
