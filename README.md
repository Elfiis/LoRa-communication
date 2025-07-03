# 📡 LoRa-Based Secure Data Transmission System

This Arduino project uses two E32-433T20D LoRa modules to perform wireless data communication with enhanced reliability using JSON formatting, checksum validation, and ACK/NACK responses.

---

## 🔧 Features

- JSON-formatted message transmission  
- Checksum verification for data integrity  
- ACK/NACK response system  
- Timeout handling  
- Real-time monitoring via Serial Monitor

---

## 🧩 Hardware Requirements

- 2x Arduino boards (Uno or Nano)  
- 2x E32-433T20D LoRa modules  
- PL2303 or similar USB-to-UART converter  
- Breadboard and jumper wires  

**Example Wiring:**

| LoRa Pin | Arduino Pin |
|----------|-------------|
| M0       | D6          |
| M1       | D7          |
| RX       | D11         |
| TX       | D10         |
| VCC      | 5V          |
| GND      | GND         |

> LoRa module must operate in normal mode (M0 = LOW, M1 = LOW)

---

## 📂 Files

- `Transmitter.ino` — Sends JSON data with checksum  
- `Receiver.ino` — Receives and validates data, sends ACK/NACK

---

## 🧪 Example Message

```json
{"id":1, "mesaj":"selam elfis", "checksum":1093}
