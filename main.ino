#include <WiFi.h>
#include <WebServer.h>
#include <HTTPClient.h>
#include <SinricPro.h>
#include <SinricProFanUS.h>

// ============================================================
//  CONFIG
// ============================================================
const char* WIFI_SSID      = "Ahan_iPhone";
const char* WIFI_PASS      = "ahan2005";

const char* TS_WRITE_API   = "P49N9THD223T0OQ2";
const char* TS_READ_API    = "S4H8NAMA81TVAE79";
const unsigned long TS_CHANNEL_ID = 3322916UL;

#define SINRIC_APP_KEY    "9dc95f05-529f-4fa9-a66b-a40d5f7c6d8e"
#define SINRIC_APP_SECRET "29a13132-700d-4361-a96b-958bc53eca73-98eac770-4bfe-4abc-857f-d5c5588662ee"
#define FAN_DEVICE_ID     "69cd4ede17b32c0941e11cfa"
// ============================================================

WebServer server(80);

int fanSpeed = 0;
const char* speedNames[] = {"OFF", "LOW", "MEDIUM", "HIGH"};

// LED Pins
#define POWER_LED 2
#define LED_LOW   18
#define LED_MED   19
#define LED_HIGH  21

// ── Motor PWM (Core 3.x) ──────────────────────────────────
#define MOTOR_PIN  23
#define PWM_FREQ   2000
#define PWM_RES    8

const int pwmDuty[] = {0, 84, 128, 166};
// ─────────────────────────────────────────────────────────

// ThingSpeak write control
unsigned long lastTSWrite     = 0;
bool          tsDirty         = true;
int           lastMirroredSpeed = -1;

// ThingSpeak command polling
unsigned long lastPoll        = 0;
int           lastCommandSeen = -1;

// ── LED Startup sequence ────────────────────────────────────────
void runLEDStartupTest() {
  Serial.println();
  Serial.println(F("  [ LED TEST ] Starting startup LED check..."));

  // POWER LED (active LOW)
  digitalWrite(POWER_LED, LOW);
  delay(300);
  digitalWrite(POWER_LED, HIGH);
  delay(150);

  // LOW LED
  digitalWrite(LED_LOW, HIGH);
  delay(300);
  digitalWrite(LED_LOW, LOW);
  delay(150);

  // MED LED
  digitalWrite(LED_MED, HIGH);
  delay(300);
  digitalWrite(LED_MED, LOW);
  delay(150);

  // HIGH LED
  digitalWrite(LED_HIGH, HIGH);
  delay(300);
  digitalWrite(LED_HIGH, LOW);
  delay(150);

  Serial.println(F("  [ LED TEST ] All LEDs ON"));

  digitalWrite(POWER_LED, LOW);   // active LOW
  digitalWrite(LED_LOW, HIGH);
  digitalWrite(LED_MED, HIGH);
  digitalWrite(LED_HIGH, HIGH);

  delay(700);

  Serial.println(F("  [ LED TEST ] Completed."));
  Serial.println();
}

// ── Serial Helpers ────────────────────────────────────────
void printDivider() {
  Serial.println(F("  ----------------------------------------"));
}

void printHeader(const char* title) {
  Serial.println();
  Serial.println(F("  ========================================"));
  Serial.print(F("  | "));
  Serial.print(title);
  int padding = 38 - strlen(title);
  for (int i = 0; i < padding; i++) Serial.print(' ');
  Serial.println(F("|"));
  Serial.println(F("  ========================================"));
}

void printSpeedBar(int spd) {
  // Prints a simple visual bar: [####....] for speed 0-3
  Serial.print(F("  Speed Bar  : ["));
  for (int i = 1; i <= 3; i++) Serial.print(i <= spd ? '#' : '.');
  Serial.println(F("]"));
}

// ── LED Indicators ────────────────────────────────────────
void updateLEDs() {
  digitalWrite(POWER_LED, fanSpeed > 0 ? LOW : HIGH);

  switch (fanSpeed) {
    case 0:
      digitalWrite(LED_LOW,  LOW);
      digitalWrite(LED_MED,  LOW);
      digitalWrite(LED_HIGH, LOW);
      break;
    case 1:
      digitalWrite(LED_LOW,  HIGH);
      digitalWrite(LED_MED,  LOW);
      digitalWrite(LED_HIGH, LOW);
      break;
    case 2:
      digitalWrite(LED_LOW,  HIGH);
      digitalWrite(LED_MED,  HIGH);
      digitalWrite(LED_HIGH, LOW);
      break;
    case 3:
      digitalWrite(LED_LOW,  HIGH);
      digitalWrite(LED_MED,  HIGH);
      digitalWrite(LED_HIGH, HIGH);
      break;
  }
}

// ── Motor PWM output ─────────────────────────────────────
void updateMotor() {
  ledcWrite(MOTOR_PIN, pwmDuty[fanSpeed]);

  Serial.println(F("  [ MOTOR ]"));
  Serial.printf ("  PWM Duty   : %d / 255\n", pwmDuty[fanSpeed]);
  Serial.printf ("  Est. Voltage: %.2f V  (at 5V supply)\n",
                 pwmDuty[fanSpeed] / 255.0f * 5.0f);
}

// ── ThingSpeak mirror ─────────────────────────────────────
void queueThingSpeakSync() {
  tsDirty = true;
}

void mirrorThingSpeakIfNeeded() {
  if (!tsDirty) return;
  if (millis() - lastTSWrite < 16000UL) return;

  Serial.println();
  Serial.println(F("  [ THINGSPEAK ] Uploading current fan state..."));

  HTTPClient http;
  String url = "http://api.thingspeak.com/update?api_key=";
  url += TS_WRITE_API;
  url += "&field1=" + String(fanSpeed);
  url += "&field2=" + String(speedNames[fanSpeed]);

  http.begin(url);
  int    code = http.GET();
  String body = http.getString();
  http.end();

  if (code > 0 && body.toInt() > 0) {
    Serial.printf("  Status     : OK  (HTTP %d)\n", code);
    Serial.printf("  Entry #    : %s\n", body.c_str());
    Serial.printf("  Uploaded   : Speed = %s (%d)\n", speedNames[fanSpeed], fanSpeed);
  } else {
    Serial.printf("  Status     : FAILED  (HTTP %d, Response: %s)\n", code, body.c_str());
  }

  lastTSWrite = millis();
  if (code > 0 && body.toInt() > 0) {
    tsDirty = false;
    lastMirroredSpeed = fanSpeed;
  }
}

// ── Fan control (single source of truth) ─────────────────
void applyFanSpeed(int spd) {
  spd = constrain(spd, 0, 3);
  if (spd == fanSpeed) return;

  fanSpeed = spd;
  updateLEDs();
  updateMotor();

  Serial.println();
  printDivider();
  Serial.println(F("  [ FAN ] Speed Changed"));
  printDivider();
  Serial.printf ("  New Speed  : %s  (Level %d)\n", speedNames[fanSpeed], fanSpeed);
  printSpeedBar(fanSpeed);
  printDivider();

  queueThingSpeakSync();
}

// ── SinricPro callbacks ───────────────────────────────────
bool onPowerState(const String& id, bool& on) {
  Serial.println();
  Serial.println(F("  [ SINRIC ] Power command received"));
  Serial.printf ("  Command    : Turn %s\n", on ? "ON" : "OFF");
  applyFanSpeed(on ? 1 : 0);
  return true;
}

bool onRangeValue(const String& id, int& val) {
  Serial.println();
  Serial.println(F("  [ SINRIC ] Speed command received"));
  Serial.printf ("  Requested  : Level %d (%s)\n", val, speedNames[constrain(val, 0, 3)]);
  applyFanSpeed(val);
  val = fanSpeed;
  return true;
}

// ── Web dashboard HTML ────────────────────────────────────
String buildPage() {
  const char* dur[] = {"0s", "2.5s", "1.2s", "0.45s"};

  String h = R"====(
<!DOCTYPE html><html><head>
<meta name='viewport' content='width=device-width,initial-scale=1'>
<title>Fan Controller</title>
<style>
  body{margin:0;background:#111827;color:#f3f4f6;font-family:Arial,sans-serif;
       display:flex;flex-direction:column;align-items:center;justify-content:center;
       min-height:100vh;gap:10px;}
  h2{color:#93c5fd;letter-spacing:1px;margin:0}
  .badge{font-size:2em;font-weight:bold;color:#fbbf24;min-width:120px;text-align:center}
  svg{overflow:visible}
  @keyframes spin{to{transform:rotate(360deg)}}
  #blades{transform-origin:60px 60px;animation:spin var(--dur,0s) linear infinite}
  .btns{display:flex;gap:10px;flex-wrap:wrap;justify-content:center;margin-top:8px}
  button{padding:12px 22px;font-size:1em;font-weight:bold;border:2px solid transparent;
         border-radius:8px;cursor:pointer;transition:.15s}
  button:hover{filter:brightness(1.15)}
  .b0{background:#374151;color:#d1d5db} .b1{background:#059669;color:#fff}
  .b2{background:#d97706;color:#fff}    .b3{background:#dc2626;color:#fff}
  button.active{border-color:#fff;box-shadow:0 0 0 2px #fff4}
  .ts{margin-top:16px;font-size:.8em;color:#6b7280}
  .ts a{color:#93c5fd}
  .info{font-size:.75em;color:#4b5563;max-width:340px;text-align:center;margin-top:4px}
</style></head><body>
<h2>&#127744; Wi-Fi Ceiling Fan</h2>
<div class='badge' id='lbl'>)====";

  h += speedNames[fanSpeed];
  h += R"====(</div>
<svg viewBox='0 0 120 120' width='150' height='150'>
  <circle cx='60' cy='60' r='6' fill='#fbbf24' opacity='.6'/>
  <g id='blades' style='--dur:)====";
  h += dur[fanSpeed];
  h += R"====('>
    <ellipse cx='60' cy='28' rx='9' ry='24' fill='#60a5fa' opacity='.8'/>
    <ellipse cx='60' cy='28' rx='9' ry='24' fill='#60a5fa' opacity='.8' transform='rotate(90 60 60)'/>
    <ellipse cx='60' cy='28' rx='9' ry='24' fill='#60a5fa' opacity='.8' transform='rotate(180 60 60)'/>
    <ellipse cx='60' cy='28' rx='9' ry='24' fill='#60a5fa' opacity='.8' transform='rotate(270 60 60)'/>
  </g>
  <circle cx='60' cy='60' r='5' fill='#fbbf24'/>
</svg>
<div class='btns'>
  <button class='b0)====";
  h += (fanSpeed == 0 ? " active" : ""); h += "' onclick='go(0)'>OFF</button>";
  h += "<button class='b1"; h += (fanSpeed == 1 ? " active" : ""); h += "' onclick='go(1)'>LOW</button>";
  h += "<button class='b2"; h += (fanSpeed == 2 ? " active" : ""); h += "' onclick='go(2)'>MEDIUM</button>";
  h += "<button class='b3"; h += (fanSpeed == 3 ? " active" : ""); h += "' onclick='go(3)'>HIGH</button>";
  h += R"====(</div>
<div class='ts'>Logged to <a href='https://thingspeak.com/channels/)====";
  h += String(TS_CHANNEL_ID);
  h += R"====(' target='_blank'>ThingSpeak</a></div>
<div class='info'>EE427 — Wi-Fi Ceiling Fan Simulator | ESP32</div>
<script>
const durs=['0s','2.5s','1.2s','0.45s'];
const names=['OFF','LOW','MEDIUM','HIGH'];
function go(s){
  fetch('/set?speed='+s).then(r=>r.text()).then(()=>{
    document.getElementById('lbl').textContent=names[s];
    document.getElementById('blades').style.setProperty('--dur',durs[s]);
    document.querySelectorAll('button').forEach((b,i)=>
      b.classList.toggle('active',i===s));
  });
}
</script></body></html>)====";
  return h;
}

// ── HTTP Routes ───────────────────────────────────────────
void handleRoot() {
  server.send(200, "text/html", buildPage());
}

void handleSet() {
  if (server.hasArg("speed")) {
    int requested = server.arg("speed").toInt();
    Serial.println();
    Serial.println(F("  [ WEB ] Speed change via dashboard"));
    Serial.printf ("  Requested  : Level %d (%s)\n",
                   requested, speedNames[constrain(requested, 0, 3)]);
    applyFanSpeed(requested);
  }
  server.send(200, "text/plain", "OK");
}

// ── ThingSpeak command polling ────────────────────────────
void pollThingSpeakCommand() {
  if (millis() - lastPoll < 16000UL) return;
  lastPoll = millis();

  HTTPClient http;
  String url = "http://api.thingspeak.com/channels/";
  url += TS_CHANNEL_ID;
  url += "/fields/3/last.json?api_key=";
  url += TS_READ_API;

  http.begin(url);
  int code = http.GET();

  if (code == 200) {
    String body = http.getString();
    int idx = body.indexOf("\"field3\":\"");
    if (idx != -1) {
      int start = idx + 10;
      int end   = body.indexOf("\"", start);
      int cmd   = body.substring(start, end).toInt();
      if (cmd >= 0 && cmd <= 3 && cmd != lastCommandSeen) {
        Serial.println();
        Serial.println(F("  [ THINGSPEAK ] Remote command received!"));
        Serial.printf ("  Command    : Set speed to Level %d (%s)\n",
                       cmd, speedNames[cmd]);
        lastCommandSeen = cmd;
        applyFanSpeed(cmd);
      }
    }
  }
  http.end();
}

// ── Setup ─────────────────────────────────────────────────
void setup() {
  Serial.begin(115200);
  delay(200);

  printHeader("  Wi-Fi Ceiling Fan  |  EE427  ");

  pinMode(POWER_LED, OUTPUT);
  pinMode(LED_LOW,   OUTPUT);
  pinMode(LED_MED,   OUTPUT);
  pinMode(LED_HIGH,  OUTPUT);
  runLEDStartupTest();
  updateLEDs();
  Serial.println(F("  LEDs       : Initialized"));

  ledcAttach(MOTOR_PIN, PWM_FREQ, PWM_RES);
  ledcWrite(MOTOR_PIN, 0);
  Serial.println(F("  Motor PWM  : Initialized  (duty = 0, motor OFF)"));

  Serial.println();
  Serial.print(F("  WiFi       : Connecting to \""));
  Serial.print(WIFI_SSID);
  Serial.print(F("\"  "));

  WiFi.begin(WIFI_SSID, WIFI_PASS);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print('.');
  }

  Serial.println(F("  Connected!"));
  Serial.print(F("  IP Address : "));
  Serial.println(WiFi.localIP());

  server.on("/",    handleRoot);
  server.on("/set", handleSet);
  server.begin();
  Serial.println(F("  Web Server : Running on port 80"));

  SinricProFanUS &fan = SinricPro[FAN_DEVICE_ID];
  fan.onPowerState(onPowerState);
  fan.onRangeValue(onRangeValue);
  SinricPro.begin(SINRIC_APP_KEY, SINRIC_APP_SECRET);
  Serial.println(F("  SinricPro  : Connected"));

  queueThingSpeakSync();

  Serial.println();
  Serial.println(F("  ========================================"));
  Serial.println(F("  System is READY. Waiting for commands..."));
  Serial.println(F("  ========================================"));
  Serial.println();
}

// ── Loop ──────────────────────────────────────────────────
void loop() {
  server.handleClient();
  SinricPro.handle();
  mirrorThingSpeakIfNeeded();
  pollThingSpeakCommand();
}
