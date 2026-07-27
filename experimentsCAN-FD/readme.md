# CAN FD Experimentation using Two PCAN-USB FD Interfaces

## Purpose

This manual is focused on CAN FD experimentation using two PCAN-USB FD modules.

Each PCAN-USB FD module provides one CAN FD channel.

```text
PCAN-USB FD #1  <---- CAN FD bus ---->  PCAN-USB FD #2
```

**Required Hardware**
1. PCAN-USB FD module - Node 1
2. PCAN-USB FD module - Node 2
3. CAN cable / jumper wiring
4. Two 120 ohm termination resistors
5. Windows PC with PCAN-View installed

**Required Software**
PCAN-View
PEAK device drivers

**CAN FD Configuration Used**

Unless mentioned otherwise, use:

* Mode:             CAN FD
* Nominal bit rate: 500 kbit/s
* Data bit rate:    2 Mbit/s
* BRS:              ON where required
* RTR:              OFF

**D-Sub CAN Pinout**

* Pin 7 = CAN_H
* Pin 2 = CAN_L
* Pin 3 = CAN_GND

**Wiring**

* PCAN-USB FD #1 Pin 7  ----  PCAN-USB FD #2 Pin 7    CAN_H
* PCAN-USB FD #1 Pin 2  ----  PCAN-USB FD #2 Pin 2    CAN_L
* PCAN-USB FD #1 Pin 3  ----  PCAN-USB FD #2 Pin 3    CAN_GND

**Termination**

* Use two 120 ohm resistors.
* 120 ohm between CAN_H and CAN_L near Node 1
* 120 ohm between CAN_H and CAN_L near Node 2
* Expected resistance between CAN_H and CAN_L with power OFF: Approximately 60 ohm

Experiment Files
01_basic_canfd_communication.md
02_payload_and_dlc.md
03_brs_and_busload.md
04_timing_mismatch_error.md
05_classical_can_vs_canfd.md
06_diagnostic_style_transfer.md
07_arbitration_and_ids.md
08_ack_error_demo.md
09_physical_layer_faults.md
10_observation_template.md

General Safety
1. Do not keep fault conditions active for long.
2. Stop cyclic transmission before changing wiring.
3. After Bus-Off, remove the fault and reinitialize PCAN-View.
4. Always restore correct termination after fault experiments.

Recommended Execution Order
1. Start with basic CAN FD communication.
2. Demonstrate 8-byte and 64-byte payloads.
3. Demonstrate BRS and bus load.
4. Demonstrate CAN FD timing mismatch.
5. Compare Classical CAN and CAN FD.
6. Demonstrate diagnostic-style transfer.
7. Demonstrate arbitration and identifier formats.
8. Demonstrate ACK error.
9. Demonstrate physical-layer faults.
10. Record observations.

Important Notes
1. CAN FD communication requires matching CAN FD bit timing on both nodes.
2. For BRS-based experiments, both nodes must use the same data bit rate.
3. A CAN/CAN FD transmitter requires ACK from at least one active receiver.
4. CAN FD fault behavior may vary depending on cable length, termination, bit rate, and transceiver behavior.
5. If PCAN-View shows Error Passive or Bus-Off, remove the fault and reinitialize the interface.
