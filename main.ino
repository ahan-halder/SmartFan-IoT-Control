#include <WiFi.h>
#include <WebServer.h>
#include <HTTPClient.h>
#include <SinricPro.h>
#include <SinricProFanUS.h>

// ============================================================
//  FILL IN YOUR CONFIG HERE
// ============================================================
const char* WIFI_SSID      = "Ahan_iPhone";
const char* WIFI_PASS      = "ahan2005";
const char* TS_WRITE_API   = "P49N9THD223T0OQ2";
const unsigned long TS_CHANNEL_ID = 3322916UL;

#define SINRIC_APP_KEY    "9dc95f05-529f-4fa9-a66b-a40d5f7c6d8e"
#define SINRIC_APP_SECRET "29a13132-700d-4361-a96b-958bc53eca73-98eac770-4bfe-4abc-857f-d5c5588662ee"
#define FAN_DEVICE_ID     "69cd4ede17b32c0941e11cfa"
// ============================================================

WebServer server(80);

int fanSpeed = 0;   // 0=OFF 1=LOW 2=MEDIUM 3=HIGH
const char* speedNames[] = {"OFF", "LOW", "MEDIUM", "HIGH"};

// LED Pins
#define POWER_LED 2   // Blue onboard LED
#define LED_LOW  18
#define LED_MED  19
#define LED_HIGH 21

unsigned long lastTS = 0;

// ------ ThingSpeak -----------------------------------------------
void pushThingSpeak() {
  if (millis() - lastTS < 16000) return;
  lastTS = millis();

  HTTPClient http;
  String url = "http://api.thingspeak.com/update?api_key=";
  url += TS_WRITE_API;
  url += "&field1=" + String(fanSpeed);
  url += "&field2=" + String(speedNames[fanSpeed]);

  http.begin(url);
  int code = http.GET();
  Serial.printf("[ThingSpeak] POST -> HTTP %d\n", code);
  http.end();
}

// ------ LED Indicators -------------------------------------------
void updateLEDs() {

  // Blue LED shows fan ON/OFF
  digitalWrite(POWER_LED, fanSpeed > 0 ? LOW : HIGH);

  // Speed LEDs
  switch (fanSpeed) {
    case 0:
      digitalWrite(LED_LOW, LOW);
      digitalWrite(LED_MED, LOW);
      digitalWrite(LED_HIGH, LOW);
      break;

    case 1:
      digitalWrite(LED_LOW, HIGH);
      digitalWrite(LED_MED, LOW);
      digitalWrite(LED_HIGH, LOW);
      break;

    case 2:
      digitalWrite(LED_LOW, HIGH);
      digitalWrite(LED_MED, HIGH);
      digitalWrite(LED_HIGH, LOW);
      break;

    case 3:
      digitalWrite(LED_LOW, HIGH);
      digitalWrite(LED_MED, HIGH);
      digitalWrite(LED_HIGH, HIGH);
      break;
  }
}

// ------ Fan control -----------------------------------------------
void applyFanSpeed(int spd) {
  fanSpeed = constrain(spd, 0, 3);

  updateLEDs();

  Serial.printf("[FAN] Speed changed to: %s (%d)\n",
                speedNames[fanSpeed], fanSpeed);

  lastTS = 0;
  pushThingSpeak();
}

// ------ SinricPro callbacks --------------------------------------
bool onPowerState(const String& id, bool& on) {
  applyFanSpeed(on ? 1 : 0);
  return true;
}

bool onRangeValue(const String& id, int& val) {
  applyFanSpeed(val);
  val = fanSpeed;
  return true;
}

// ------ Web dashboard HTML ---------------------------------------
String buildPage() {
  // Speed → CSS animation duration for fan blades
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
  h += (fanSpeed==0?" active":""); h += "' onclick='go(0)'>OFF</button>";
  h += "<button class='b1"; h += (fanSpeed==1?" active":""); h += "' onclick='go(1)'>LOW</button>";
  h += "<button class='b2"; h += (fanSpeed==2?" active":""); h += "' onclick='go(2)'>MEDIUM</button>";
  h += "<button class='b3"; h += (fanSpeed==3?" active":""); h += "' onclick='go(3)'>HIGH</button>";
  h += R"====(</div>
<div class='ts'>Logged to <a href='https://thingspeak.com/channels/)====";
  h += String(TS_CHANNEL_ID);
  h += R"====(  ' target='_blank'>ThingSpeak</a></div>
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

// ------ HTTP Routes ----------------------------------------------
void handleRoot() {
  server.send(200, "text/html", buildPage());
}

void handleSet() {
  if (server.hasArg("speed")) {
    applyFanSpeed(server.arg("speed").toInt());
  }
  server.send(200, "text/plain", "OK");
}

// ------ Setup ----------------------------------------------------
void setup() {

  Serial.begin(115200);

  pinMode(POWER_LED, OUTPUT);
  pinMode(LED_LOW, OUTPUT);
  pinMode(LED_MED, OUTPUT);
  pinMode(LED_HIGH, OUTPUT);

  updateLEDs();

  Serial.print("Connecting WiFi");

  WiFi.begin(WIFI_SSID, WIFI_PASS);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nConnected!");
  Serial.println(WiFi.localIP());

  server.on("/", handleRoot);
  server.on("/set", handleSet);

  server.begin();

  SinricProFanUS &fan = SinricPro[FAN_DEVICE_ID];
  fan.onPowerState(onPowerState);
  fan.onRangeValue(onRangeValue);

  SinricPro.begin(SINRIC_APP_KEY, SINRIC_APP_SECRET);

  Serial.println("System Ready");
}

// ------ Loop -----------------------------------------------------
void loop() {

  server.handleClient();
  SinricPro.handle();
  pushThingSpeak();
}
