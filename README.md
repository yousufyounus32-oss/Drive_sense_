Title: Drive Sense — Multi-Node Vehicle Monitoring System

# Drive Sense — Multi-Node Vehicle Monitoring System

A distributed, CAN-bus-based vehicle monitoring system built on three ARM7 (NXP LPC2129) microcontroller nodes. 

Each node independently senses a different vehicle parameter and broadcasts it over CAN, while a central dashboard node receives, processes, and displays all readings in real time.

This project simulates how modern vehicle ECUs communicate with each other over a shared CAN bus — each "sensor node" behaves like a simplified automotive control unit.

## Overview

Drive Sense is a 3-node embedded network:

- **Node A — Engine/Obstacle Node**: Monitors temperature and obstacle distance, with an audible buzzer warning.
- **Node B — Fuel & Light Node**: Monitors fuel level and ambient light, with an LED status indicator.
- **Node C — Dashboard Node**: Acts as the central receiver, displaying all incoming sensor data on an LCD dashboard and a UART serial console.

All three nodes communicate over a shared **CAN2 bus at 125 kbps**, each node identified by a unique CAN message ID.

## System Architecture

```
 ┌────────────┐        ┌────────────┐
 │   Node A   │        │   Node B   │
 │ Temp + US  │        │ Fuel + LDR │
 │ ID: 0x120  │        │ ID: 0x150  │
 └─────┬──────┘        └─────┬──────┘
       │                     │
       └─────────┬───────────┘
                  │  CAN2 Bus (125 kbps)
                  │
            ┌─────┴──────┐
            │   Node C   │
            │ Dashboard  │
            │ (Receiver) │
            └────────────┘
```

### Node A — Temperature & Obstacle Sensing
- Reads ambient/engine temperature using an **LM35 sensor** via the onboard **10-bit ADC** (8-sample averaging for stability).
- Measures obstacle distance using an **HC-SR04 ultrasonic sensor**, timed via Timer1 pulse-width capture.
- Triggers a **buzzer** when an obstacle is detected within 20 cm.
- Displays live temperature and distance on a **16x2 LCD**.
- Transmits both readings as a CAN data frame (**ID 0x120**).

### Node B — Fuel & Light Sensing ("Yousuf Node")
- Reads fuel level via ADC and converts it to a 0–100% scale.
- Reads ambient light using an **LDR** in digital GPIO mode, toggling an onboard LED.
- Streams live readings over **UART0 (115200 baud)** for debugging.
- Transmits fuel level and light status as a CAN data frame (**ID 0x150**).

### Node C — Dashboard / Receiver Node
- Continuously listens on the CAN bus for messages from both Node A and Node B.
- Parses incoming frames by CAN ID and updates a unified **16x2 LCD dashboard** showing Temperature, Distance, Fuel %, and LED/light status.
- Mirrors all data to a UART serial terminal for diagnostics.
- Displays a project splash screen on boot.

## Hardware Used

| Component | Purpose |
|---|---|
| NXP LPC2129 (ARM7TDMI-S) | Core microcontroller — one per node |
| LM35 Temperature Sensor | Analog temperature input (Node A) |
| HC-SR04 Ultrasonic Sensor | Obstacle distance measurement (Node A) |
| Buzzer | Obstacle proximity alert (Node A) |
| LDR (Light Dependent Resistor) | Ambient light detection (Node B) |
| Fuel-level sensor / potentiometer | Fuel percentage simulation (Node B) |
| LED | Light/status indicator (Node B, Node C) |
| 16x2 Character LCD | Local display (Node A, Node C) |
| CAN Transceiver (e.g. TJA1050) | Physical layer for CAN2 bus |
| USB-to-UART module | Serial debug console (Node B, Node C) |

## Technical Stack

- **Language:** Embedded C
- **MCU Family:** ARM7TDMI-S (LPC2129)
- **Communication Protocols:** CAN 2.0 (CAN2 peripheral), UART (115200 baud)
- **Peripherals used:** 10-bit ADC, GPIO, Timer (capture-based ultrasonic timing), 16x2 LCD (4-bit parallel interface)
- **Toolchain:** Keil µVision / ARM7 compatible IDE

## Key Implementation Details

- **Custom drivers written from scratch** for CAN2, UART0, LCD, and software delay — no HAL/library dependency, giving full register-level control.
- **CAN configuration:** CAN2 controller initialized at 125 kbps (PCLK = 60 MHz), configured to accept all incoming data and remote frames.
- **CAN message structure:** Each frame carries an ID, DLC, RTR flag, and two data payload fields, used to pack two sensor values per transmission for bandwidth efficiency.
- **Ultrasonic distance measurement** implemented via Timer1 in capture mode, measuring echo pulse width directly in hardware for accurate, non-blocking timing.
- **Averaged ADC sampling** on the temperature channel to reduce noise and improve reading stability.

## File Structure

```
├── Node_A.c            # Temperature + ultrasonic sensing node
├── Node_B.c             # Fuel + light sensing node
├── Node_C.c             # Dashboard receiver node
├── can2_driver.h         # CAN2 peripheral driver (init, tx, rx)
├── uart_driver.h          # UART0 driver (init, tx string/int/hex)
├── lcd1.h                  # 16x2 LCD driver (4-bit mode)
├── delay_header.h          # Timer-based delay functions
└── header.h                  # Common project definitions
```

## Future Improvements

- Add CAN acceptance filtering instead of accepting all frames
- Add error/bus-off handling and message retry logic
- Migrate dashboard fuel/distance values to dedicated LCD lines (currently UART-prioritized)
- Add data logging via SD card or GSM-based remote telemetry
- Expand to additional vehicle parameters (RPM, battery voltage, tire pressure)

## Author

Built and developed by **Mohamed yousuf** as a hands-on embedded systems project exploring multi-node CAN bus communication on ARM7 microcontrollers.
