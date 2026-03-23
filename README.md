# SmartFan-IoT-Control

> Internet-based IoT simulation for controlling a ceiling fan with four speed states (**OFF, LOW, MEDIUM, HIGH**) via a web interface.

---

## Overview

**SmartFan-IoT-Control** is an IoT-based simulation system that enables remote monitoring and control of a ceiling fan using the **ESP8266 NodeMCU microcontroller**, the **ThingSpeak IoT cloud platform**, and **MATLAB analytics**.

⚠️ **Important Note:**  
This project is a **simulation-focused implementation**:
- No actual ceiling fan or high-voltage AC hardware is used  
- Fan speed states are **emulated using ESP8266 GPIO outputs** (LEDs / virtual signals)

The system demonstrates:
- IoT communication (ESP8266 ↔ Cloud)
- Cloud-based command handling (ThingSpeak TalkBack)
- Real-time control and feedback systems
- MATLAB-based visualization and analytics

This project was developed as part of the **EE427 Computer Networks mini project at NITK Surathkal**.

---

# Features

- 🌐 Remote fan control via internet
- ⚡ Four fan speed states
  - OFF
  - LOW
  - MEDIUM
  - HIGH
- 📡 Wi-Fi communication using ESP8266
- ☁️ Cloud integration with ThingSpeak
- 📊 Data analytics using MATLAB
- 🔁 Closed-loop feedback system
- 🎛 Web-based user interface
- 🧪 Fully simulation-friendly (no high-voltage hardware required)

---

# System Architecture

```
User Interface (Web / MATLAB App)
           │
           ▼
     ThingSpeak Cloud
           │
           ▼
     ESP8266 NodeMCU
           │
           ▼
   Simulated Output (GPIO / LEDs)
```

### Workflow

1. User sends command via web interface / MATLAB
2. Command is written to ThingSpeak
3. ESP8266 reads command (HTTP/MQTT)
4. GPIO pins are updated to represent fan state
5. Status is sent back to ThingSpeak

---

# Technologies Used

## Hardware

- ESP8266 NodeMCU
- LEDs (optional for visualization)
- Breadboard & wires

## Software

- Arduino IDE
- ThingSpeak IoT Platform
- MATLAB Analytics
- ESP8266WiFi Library
- ThingSpeak Arduino Library
- PubSubClient (MQTT)

## Communication Protocols

- HTTP REST API
- MQTT Messaging Protocol

---

# Hardware Setup

## Components

| Component | Description |
|-----------|-------------|
| ESP8266 NodeMCU | Wi-Fi enabled microcontroller |
| LEDs (optional) | Represent fan speed states |
| Breadboard & Wires | Prototyping |

⚠️ No relay modules, capacitors, or real AC fan are used.

---

## State Representation

| Fan State | GPIO Output |
|----------|------------|
| OFF | All pins LOW |
| LOW | GPIO D1 HIGH |
| MEDIUM | GPIO D2 HIGH |
| HIGH | GPIO D3 HIGH |

These outputs simulate fan speeds and can be visualized using LEDs or Serial Monitor.

---

# ThingSpeak Channel Design

The system uses a **ThingSpeak channel** with multiple fields:

| Field | Purpose |
|------|--------|
| Field 1 | Fan Control Command |
| Field 2 | Fan Status Feedback |
| Field 3 | Energy (simulated) |

This enables a **closed-loop IoT system** where the cloud reflects the actual device state.

---

# Project Workflow

## Phase 1 — Environment Setup
- Create ThingSpeak channel
- Configure API keys
- Install Arduino libraries

## Phase 2 — Simulation Setup
- Configure GPIO outputs
- Test using LEDs or Serial Monitor

## Phase 3 — Network Integration
- Connect ESP8266 to WiFi
- Implement ThingSpeak communication

## Phase 4 — MATLAB Interface
- Build GUI using MATLAB App Designer
- Visualize system data

## Phase 5 — Simulation / Emulation
- Use **Wokwi ESP8266 simulator** (recommended)
- Validate cloud communication

## Phase 6 — Testing
- Measure latency
- Verify command-response cycle

---

# Installation

## 1. Clone the repository

```bash
git clone https://github.com/yourusername/SmartFan-IoT-Control.git
cd SmartFan-IoT-Control
```

---

## 2. Install Arduino Libraries

- ESP8266WiFi
- ThingSpeak
- PubSubClient

---

## 3. Configure WiFi Credentials

```cpp
const char* ssid = "YOUR_WIFI_NAME";
const char* password = "YOUR_WIFI_PASSWORD";
```

---

## 4. Add ThingSpeak API Keys

```cpp
unsigned long channelID = YOUR_CHANNEL_ID;
const char* writeAPIKey = "YOUR_WRITE_API_KEY";
const char* readAPIKey = "YOUR_READ_API_KEY";
```

---

## 5. Upload Firmware

Upload the code to the ESP8266 using Arduino IDE.

---

# MATLAB Integration

MATLAB is used for:

- Data visualization
- Usage analysis
- Control interface (optional)

Example functions:

```
thingSpeakRead()
thingSpeakWrite()
```

---

# Simulation Mode

This project is designed to run **without physical hardware dependencies**.

You can test using:

- 🔹 Serial Monitor output
- 🔹 LEDs on GPIO pins
- 🔹 Wokwi ESP8266 Simulator

### Advantages

- Safe (no high voltage)
- Easy to demo
- Fully portable
- Faster development

---

# Security Considerations

- Use HTTPS for ThingSpeak communication
- Keep API keys private
- Use secure MQTT if enabled

---

# Future Improvements

- 🌡 Temperature-based auto control
- 🧠 AI-based optimization
- 🗣 Voice control (Google Assistant + IFTTT)
- 📱 Mobile app interface
- 📊 Advanced analytics dashboard

---

# License

This project is released under the **MIT License**.
