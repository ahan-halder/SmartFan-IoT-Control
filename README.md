# SmartFan-IoT-Control

> Internet-based IoT simulation for controlling a ceiling fan with four speed states (**OFF, LOW, MEDIUM, HIGH**) via a web interface and cloud integration.

---

## Overview

**SmartFan-IoT-Control** is an IoT-based simulation system that enables remote monitoring and control of a ceiling fan using an **ESP32 WROOM** microcontroller, **ThingSpeak**, and **SinricPro**.

⚠️ **Important Note:**  
This project is a **simulation-focused implementation**:
- No actual ceiling fan or high-voltage AC hardware is used
- Fan speed states are emulated using **ESP32 GPIO outputs** and LEDs

The system demonstrates:
- IoT communication (ESP32 ↔ Cloud)
- Web-based fan control
- Cloud-based state synchronization
- Real-time feedback using LEDs and Serial Monitor

This project was developed as part of the **EE427 Computer Networks mini project at NITK Surathkal**.

---

# Features

- 🌐 Remote fan control via local web dashboard
- ⚡ Four fan speed states
  - OFF
  - LOW
  - MEDIUM
  - HIGH
- 📡 Wi-Fi communication using ESP32
- ☁️ Cloud integration with ThingSpeak
- 🏠 Smart home control using SinricPro
- 🎛 Web-based user interface
- 🧪 Fully simulation-friendly

---

# System Architecture

```
User Interface (Web / SinricPro / ThingSpeak)
           │
           ▼
        ESP32 WROOM
           │
           ▼
   Simulated Output (GPIO / LEDs)
```

### Workflow

1. User sends command via web dashboard or SinricPro
2. ESP32 updates fan state
3. GPIO outputs are changed to represent fan speed
4. State is logged to ThingSpeak
5. LEDs provide visual feedback

---

# Technologies Used

## Hardware

- ESP32 WROOM
- 3 LEDs
- Breadboard
- Jumper wires
- Resistors (3.3 ohms)

## Software

- Arduino IDE
- ThingSpeak IoT Platform
- SinricPro
- ESP32 WiFi Library
- WebServer Library
- HTTPClient Library

## Communication Protocols

- HTTP REST API
- Cloud-based device control

---

# Hardware Setup

## Components

| Component | Description |
|-----------|-------------|
| ESP32 WROOM | Wi-Fi enabled microcontroller |
| 3 LEDs | Represent fan speed states |
| Breadboard | Prototyping platform |
| Jumper Wires | Circuit connections |
| Resistors | Current limiting |

---

## State Representation

| Fan State | GPIO Output |
|----------|------------|
| OFF | All LEDs OFF |
| LOW | Low-speed LED ON |
| MEDIUM | Low + Medium LEDs ON |
| HIGH | Low + Medium + High LEDs ON |

The onboard power LED indicates whether the fan is ON or OFF.

---

# ThingSpeak Channel Design

The system uses a **ThingSpeak channel** with multiple fields:

| Field | Purpose |
|------|--------|
| Field 1 | Fan speed value |
| Field 2 | Fan state name |

This allows logging and monitoring of the current fan state.

---

# Installation

## 1. Clone the repository

```bash
git clone https://github.com/yourusername/SmartFan-IoT-Control.git
cd SmartFan-IoT-Control
```

---

## 2. Install Arduino Libraries

- WiFi
- WebServer
- HTTPClient
- SinricPro
- SinricProFanUS

---

## 3. Configure WiFi Credentials

```cpp
const char* WIFI_SSID = "YOUR_WIFI_NAME";
const char* WIFI_PASS = "YOUR_WIFI_PASSWORD";
```

---

## 4. Add ThingSpeak and SinricPro Keys

```cpp
const char* TS_WRITE_API = "YOUR_WRITE_API_KEY";
const unsigned long TS_CHANNEL_ID = YOUR_CHANNEL_ID;

#define SINRIC_APP_KEY    "YOUR_APP_KEY"
#define SINRIC_APP_SECRET "YOUR_APP_SECRET"
#define FAN_DEVICE_ID     "YOUR_DEVICE_ID"
```

---

## 5. Upload Firmware

Upload the code to the ESP32 using Arduino IDE.

---

# Simulation Mode

This project is designed to run **without physical fan hardware**.

You can test using:
- Serial Monitor output
- LEDs on GPIO pins
- Local browser-based dashboard

---

# Security Considerations

- Keep API keys private
- Avoid hardcoding production credentials
- Use secure network access where possible

---

# Future Improvements

- 🌡 Automatic temperature-based control
- 📱 Mobile app interface
- 📊 Advanced analytics dashboard
- 🗣 Voice assistant integration

---

# License

This project is released under the **MIT License**.
