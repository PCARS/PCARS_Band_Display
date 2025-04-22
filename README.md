# PCARS W4MLB Club Station Band Display

## Technical Project Summary

This project was developed for the Platinum Coast Amateur Radio Club (PCARS) to improve operational coordination at the W4MLB Club Station. It features a shared LED display that shows the active operating band and mode for two radio stations (Station 1 and Station 2), helping operators avoid frequency conflicts and manage dual-station operation more effectively.

---

## System Overview

The Band Display system consists of:

1. **ESP32-Based Control Unit**  
2. **Single Shared 64x64 RGB LED Matrix Display**  
3. **Audible Conflict Alert System**

---

## Hardware Details

### Microcontroller
- **ESP32 DevKit V1**
  - Dual-core 32-bit microcontroller with Wi-Fi and Bluetooth
  - Interfaces with both radios via CI-V communication over UART
  - Processes band/mode data and drives the shared LED matrix display and alert system

### Display System
- **Single 64x64 RGB LED Matrix (HUB75 interface)**
  - Displays operating band and mode for both stations side-by-side
  - Color-coded segments for easy visual distinction between Station 1 and Station 2
  - Driven using the [ESP32-HUB75-MatrixPanel-I2S-DMA library](https://github.com/mrfaptastic/ESP32-HUB75-MatrixPanel-I2S-DMA) for high-performance, flicker-free display updates

### Radio Interface (Band/Mode Input)
- **CI-V Serial Bus Communication**
  - Each radio communicates band/mode data to the ESP32 via CI-V protocol
  - CI-V signals are connected to separate UART RX/TX pairs on the ESP32
  - Enables automated and accurate detection of each radio’s current state

### Power Supply
- **5V / 5A DC Input via 2.5mm Barrel Jack and Inline Mini Fuse Holder**
- **2-Position Screw Terminal for powering the LED matrix directly**, ensuring stable high-current delivery

### Alert System
- **Passive Buzzer (controlled via GPIO)**
  - Activates after 3 seconds if both stations are tuned to the same band
  - Logic controlled by the ESP32 to help operators avoid interference

### Connectors
- **2x8 Female Header (Bottom) + 2x8 Male Ribbon Header (Top)**
  - Used to interface the ESP32 Control Board to the LED matrix
  - Bottom connector allows direct plug-in to the matrix input (back-side mounting)
  - Top connector allows connection via standard ribbon cable

### PCB Design
- **2-layer Custom PCB**
  - Top layer features a +5V copper fill for power distribution
  - Bottom layer contains a solid ground pour with signal traces routed within
  - Compact and purpose-built layout for minimal noise and easy integration with the LED matrix

---

## Key Features
- Shared 64x64 LED matrix with real-time band/mode display for both stations  
- CI-V-based band/mode input over UART for accurate radio status monitoring  
- Conflict alert system with 3-second delay buzzer  
- ESP32-driven logic with efficient HUB75 display handling  
- Clean power distribution with dedicated matrix power input





![20250328_100946~2](https://github.com/user-attachments/assets/d5262754-5991-46af-958d-b24738c66b2d)
