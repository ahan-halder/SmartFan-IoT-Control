# SmartFan-IoT-Control

> Internet-based IoT simulation for controlling a ceiling fan with four speed states (**OFF, LOW, MEDIUM, HIGH**) via a web interface.

---

## Overview

**SmartFan-IoT-Control** is an IoT-based system that enables remote monitoring and control of a ceiling fan using the **ESP8266 NodeMCU microcontroller**, the **ThingSpeak IoT cloud platform**, and **MATLAB analytics**.

The system demonstrates how household appliances can be integrated into a smart home ecosystem using **cloud APIs, wireless communication, and automation logic**.

Users can control the fan speed remotely through a web interface, while the device communicates with the cloud to synchronize commands and feedback in real time.

This project was developed as part of the **EE427 Computer Networks mini project at NITK Surathkal** and focuses on IoT networking, cloud integration, and smart device control.

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
- 🤖 Optional AI-based smart speed optimization

---

# System Architecture

```
User Interface (Web / MATLAB App)
           │
           ▼
     ThingSpeak Cloud
  (Channel + TalkBack API)
           │
           ▼
     ESP8266 NodeMCU
           │
           ▼
      Relay Module
           │
           ▼
      Ceiling Fan
```

The system operates using the following workflow:

1. The user sends a command through the web interface.
2. The command is written to a ThingSpeak channel.
3. The ESP8266 polls the channel or TalkBack queue.
4. The NodeMCU activates the appropriate relay.
5. The fan speed changes accordingly.
6. The system sends feedback to the cloud to confirm execution.

---

# Technologies Used

## Hardware

- ESP8266 NodeMCU
- 4-Channel Relay Module
- Ceiling Fan (AC Motor)
- Capacitors for Speed Control
- 5V Power Supply
- Optional: DHT22 Temperature Sensor

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
| Relay Module | Switches AC power to control fan speed |
| Capacitors | Used for AC fan speed regulation |
| Power Supply | Provides regulated 5V DC |
| Breadboard & Wires | Prototyping |

## Relay Logic

| Speed | Relay Configuration |
|------|--------------------|
| OFF | All relays OFF |
| LOW | Relay 1 active |
| MEDIUM | Relay 2 active |
| HIGH | Relay 3 active |

Only **one relay is activated at a time** to prevent short circuits.

---

# ThingSpeak Channel Design

The system uses a **ThingSpeak channel** with multiple fields:

| Field | Purpose |
|------|--------|
| Field 1 | Fan Control Command |
| Field 2 | Fan Status Feedback |
| Field 3 | Energy Consumption |

This allows the system to maintain a **closed-loop IoT architecture**, where the cloud reflects the actual device state.

---

# Project Workflow

## Phase 1 — Environment Setup
- Create ThingSpeak channel
- Configure API keys
- Install Arduino libraries

## Phase 2 — Hardware Prototyping
- Connect NodeMCU to relay module
- Test relay switching logic

## Phase 3 — Network Integration
- Connect ESP8266 to WiFi
- Implement ThingSpeak communication

## Phase 4 — MATLAB Interface
- Build GUI using MATLAB App Designer
- Visualize fan usage statistics

## Phase 5 — Simulation
- Simulate ESP8266 + relays using **Wokwi or Proteus**

## Phase 6 — Testing
- Measure command latency
- Analyze network packets
- Validate system performance

---

# Installation

## 1. Clone the repository

```bash
git clone https://github.com/yourusername/SmartFan-IoT-Control.git
cd SmartFan-IoT-Control
```

---

## 2. Install Arduino Libraries

Install the following libraries in **Arduino IDE**:

- ESP8266WiFi
- ThingSpeak
- PubSubClient

---

## 3. Configure WiFi Credentials

Update the firmware:

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

Upload the firmware to the **NodeMCU using Arduino IDE**.

---

# MATLAB Integration

MATLAB is used for:

- Data visualization
- Energy usage analysis
- Smart automation logic

Example MATLAB functions used:

```
thingSpeakRead()
thingSpeakWrite()
```

A **MATLAB App Designer interface** can provide:

- Speed controls
- Live monitoring
- Usage graphs

---

# Security Considerations

- Use HTTPS connections for ThingSpeak communication
- Separate Read and Write API keys
- Use secure MQTT connections when available
- Ensure relay modules include opto-isolation

---

# Simulation

The project can also run entirely in **simulation mode** using:

- Wokwi ESP8266 simulator
- Proteus IoT simulation

This allows testing of:

- Cloud communication
- Relay switching logic
- Network packets

without using physical hardware.

---

# Future Improvements

Possible extensions include:

- 🌡 Temperature-based automatic fan speed
- 🧠 Reinforcement learning optimization
- 🗣 Voice control via Google Assistant
- ⚡ Energy consumption prediction
- 📱 Mobile application interface

---

# License

This project is released under the **MIT License**.
