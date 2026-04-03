# SmartFan-IoT-Control

> IoT-based ESP32 fan-speed simulation with local web control, SinricPro integration, and ThingSpeak logging.

---

## Overview

**SmartFan-IoT-Control** is a simulation-first IoT mini project built using **ESP32 WROOM**.

It supports fan speed control with four states:

- `0 = OFF`
- `1 = LOW`
- `2 = MEDIUM`
- `3 = HIGH`

⚠️ **Simulation Note**  
This project does **not** switch real AC fan hardware.  
Fan behavior is emulated using GPIO outputs and LEDs.

---

## What the current firmware (`main.ino`) actually does

### Control Inputs (active)

1. **ESP32 local dashboard** (`WebServer` on ESP32)
   - Route: `/set?speed=0..3`
2. **SinricPro**
   - Power callback (`onPowerState`)
   - Range callback (`onRangeValue`)

Both inputs call the same function: `applyFanSpeed()`.

### Outputs (active)

- GPIO LED state update (`updateLEDs()`)
- Serial logs
- ThingSpeak update (`pushThingSpeak()`), throttled to ~16 seconds

---

## System Architecture (accurate to current code)

```text
                ┌──────────────────────┐
                │  Browser (ESP32 UI)  │
                │   GET /set?speed=n   │
                └──────────┬───────────┘
                           │
                           ▼
                    ┌──────────────┐
                    │    ESP32     │
                    │  main.ino    │
                    └──────┬───────┘
                           │
        ┌──────────────────┼──────────────────┐
        ▼                  ▼                  ▼
  GPIO / LEDs        Serial Monitor      ThingSpeak (write)
 (speed emulation)   (state logs)      field1, field2 updates
                           ▲
                           │
                   ┌───────┴────────┐
                   │   SinricPro    │
                   │ (cloud control)│
                   └────────────────┘
```

---

## About `index.html` in this repo

The standalone `index.html` is a **ThingSpeak-based UI** that:

- **writes** command to `field3`
- **reads** current speed from `field1`

### Important compatibility note

Current `main.ino` **does not read/poll `field3`**, so `index.html` commands will not change ESP32 speed unless firmware polling for `field3` is added.

So currently:

- `field1` and `field2` are actively written by ESP32 ✅
- `field3` is used by `index.html` only (no firmware consumer) ⚠️

---

## ThingSpeak Field Mapping

| Field | Used By | Direction | Purpose |
|------|---------|-----------|---------|
| Field 1 | `main.ino` | ESP32 → ThingSpeak | Numeric speed (`0..3`) |
| Field 2 | `main.ino` | ESP32 → ThingSpeak | Speed label (`OFF/LOW/MEDIUM/HIGH`) |
| Field 3 | `index.html` | UI → ThingSpeak | Command speed (`0..3`) |

---

## Hardware Setup

### Components

- ESP32 WROOM
- 3 external LEDs (`LOW`, `MEDIUM`, `HIGH`)
- Onboard LED (`POWER` status indication)
- Breadboard + jumper wires
- Current-limiting resistors

### GPIO behavior

- `POWER_LED` (GPIO 2): fan ON/OFF indication
- `LED_LOW` (GPIO 18)
- `LED_MED` (GPIO 19)
- `LED_HIGH` (GPIO 21)

State mapping:

| Fan State | LED Output |
|----------|------------|
| OFF | All speed LEDs OFF |
| LOW | LOW LED ON |
| MEDIUM | LOW + MEDIUM ON |
| HIGH | LOW + MEDIUM + HIGH ON |

---

## Technologies Used

### Firmware

- Arduino IDE
- `WiFi.h`
- `WebServer.h`
- `HTTPClient.h`
- `SinricPro.h`
- `SinricProFanUS.h`

### Cloud / UI

- ThingSpeak
- SinricPro
- Browser-based dashboard (ESP32-served + standalone `index.html`)

---

## Installation (firmware path)

1. Open `main.ino` in Arduino IDE.
2. Install required libraries:
   - WiFi
   - WebServer
   - HTTPClient
   - SinricPro
   - SinricProFanUS
3. Configure credentials and keys in `main.ino`:
   - Wi-Fi SSID/password
   - ThingSpeak write API key and channel ID
   - SinricPro app/device credentials
4. Upload to ESP32.
5. Open Serial Monitor and note ESP32 local IP.
6. Open that IP in browser for local fan control UI.

---

## Current limitations

- No real AC fan switching (simulation only)
- ThingSpeak write rate throttled (~16 s)
- Standalone `index.html` command path depends on firmware support for reading `field3` (not present in current `main.ino`)

---

## Future Improvements

- Add firmware polling of ThingSpeak `field3` for full cloud-command loop
- Temperature-based automatic speed control
- OTA update support
- Stronger credential handling (secrets outside source)

---

## License

MIT License
