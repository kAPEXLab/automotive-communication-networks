# Lab : CAN Combo Capture and Analysis with Wireshark

## Aim, Outcomes, and Bench

### Learning outcomes

By the end of the lab, you will be able to:

1. Explain why a CAN Combo is a passive capture module that forwards CAN frames to a PC inside TECMP over Ethernet (LO1).
2. Configure a CAN Combo channel and capture its output in Wireshark (LO2).
3. Decode CAN IDs and payload bytes into engineering values such as °C and ADC counts (LO4).
4. Measure cycle time, end-to-end latency, and bus load from hardware timestamps (LO5).
5. Diagnose bus faults from a capture: wrong bitrate, missing termination, and missing ACK (LO6).

### Bench setup

Each bench has one CAN bus at **500 kbit/s**, with three nodes and one or two CAN Combos listening on it.

| Item | Role on the bench |
|---|---|
| ECU 1: Arduino Mega 1 + MCP2515 | Reads the potentiometer (A0) and LM35 (A1); sends IDs `0x100` and `0x101`. |
| Processing ECU: Raspberry Pi 1 / Arduino Mega 3 / Uno + CAN HAT | Reads `0x100`, applies the rule **pot > 500 = LED on**, and sends `0x200`. |
| ECU 2: Arduino Mega 2 + MCP2515 | Receives `0x200` and switches the LED on pin 13. |
| CAN Combo A (and B for TC-06) | Listens on the bus, timestamps every frame, and sends it out as TECMP. |
| Enhanced Ethernet Switch (EES) | Connects the CAN Combos and the PC. |
| Lab PC | Wireshark 4.x with Npcap; the ANDi tool is optional for comparison. |
| 120 Ω resistors (2) | Termination at the two ends of the bus only. |

## Bench Wiring: 3 ECUs, 2 Capture Points

Only the three ECUs drive the bus; the CAN Combos copy frames to the PC through the EES. Combo B (shown as dashed in the diagram) is needed only for TC-06.

![Bench wiring diagram showing three ECUs on the CAN bus, CAN Combo capture points, an enhanced Ethernet switch, and the lab PC](https://github.com/kAPEXLab/automotive-communication-networks/blob/9b25f658b99b048d39b11eabba0186cf52024d65/images/bench-wiring.png)
![Bench wiring diagram showing three ECUs on the CAN bus, CAN Combo capture points, an enhanced Ethernet switch, and the lab PC]([images/bench-wiring.png](https://github.com/kAPEXLab/automotive-communication-networks/blob/b8940f92c59a8350b922b38ab654e969e32b0424/experimentsTechnica/bench-wiring.png))

## CAN ID Map

All payloads are **big-endian (high byte first)**. Confirm the cycle times against the ECU sketches before the lab.

| CAN ID | Sender | DLC | Payload | Cycle |
|---|---|---:|---|---|
| `0x100` | ECU 1 | 2 | Potentiometer value, 0–1023 ADC counts | 100 ms |
| `0x101` | ECU 2 | 2 | Engine temperature × 100, in °C (`0x0929` = 23.45 °C) | 500 ms |
| `0x200` | Raspberry Pi | 1 | LED command: `0x01` = on, `0x00` = off | Sent after each `0x100` |

> **Safety:** Power off the bus before adding or removing a resistor or a node. Never connect RS232 or 12 V lines to `CAN_H` or `CAN_L`.

## Part A: Configure the CAN Combo (Technica Engineering) and Take the First Capture

The CAN Combo never sends on the CAN bus. It copies each frame, adds a hardware timestamp, and sends it to the PC inside a TECMP frame (EtherType `0x99FE` or as observed in the capture).

### Steps

1. Power the bus with all three ECUs and both 120 Ω resistors in place.
2. Open the CAN Combo web page from the lab PC (check the IP address mentioned on the bottom side). For the channel wired to your bus, set the bitrate to **500 kbit/s**, keep it passive (**listen only**), and switch its internal termination off because the bus is already terminated.
3. Start Wireshark on the network card connected to the EES. Apply the display filter `tecmp`.
4. Expand one frame and fill in the table below.
5. Apply `can.id == 0x100`. If Wireshark rejects it, right-click the CAN ID field in any frame and choose **Apply as Filter** to get the exact field name for your version.
6. Save the capture as `bench<N>_partA.pcapng`.

### A1. TECMP Fields

| Field | Size (bytes) | Value in your capture | What it tells you |
|---|---:|---|---|
| Ether Type | 2 |  |  |
| Device ID | 2 |  |  |
| Counter | 2 |  |  |
| Message Type | 1 |  |  |
| Data Type | 2 |  |  |
| Interface ID | 4 |  |  |
| Timestamp | 8 |  |  |
| Length | 2 |  |  |

**Observations:**

_Write your observations here._

### A2. Counter Analysis

Write down the Counter values of five consecutive frames. What would a jump of 2 or more between two frames mean?

**Response:**

_Write your response here._

### A3. CAN ID Counts

Which CAN IDs do you see, and how many frames of each do you observe in 10 seconds? Use **Statistics → Conversations** or count with a filter.

**Response:**

_Write your response here._
