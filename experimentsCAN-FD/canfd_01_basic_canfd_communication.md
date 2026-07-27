# Basic CAN FD Communication

## Objective

Verify CAN FD communication between two PCAN-USB FD modules.

## Hardware Setup

```text
PCAN-USB FD #1  <---- CAN FD bus ---->  PCAN-USB FD #2
```

**Wiring**
* Node 1 CAN_H  ----  Node 2 CAN_H
* Node 1 CAN_L  ----  Node 2 CAN_L
* Node 1 GND    ----  Node 2 GND


**DB9 pinout**

* Pin 7 = CAN_H
* Pin 2 = CAN_L
* Pin 3 = CAN_GND

**Termination**

* Use two 120 ohm resistors.
* 120 ohm between CAN_H and CAN_L near Node 1
* 120 ohm between CAN_H and CAN_L near Node 2
* Expected resistance between CAN_H and CAN_L with power OFF: CAN_H to CAN_L ≈ 60 ohm

**PCAN-View Configuration**

Open two PCAN-View windows.

Configure both nodes:

* Mode:             CAN FD
* Nominal bit rate: 500 kbit/s
* Data bit rate:    2 Mbit/s

**Transmit Frame**

From Node 1, send:

````text
ID:      100
Length:  8
Type:    CAN FD
BRS:     ON
RTR:     OFF
Data:    11 22 33 44 55 66 77 88
````

**Expected Observation**

On Node 2:

````text
ID:      0x100
Length:  8
Data:    11 22 33 44 55 66 77 88
Type:    CAN FD
BRS:     ON
````

**Troubleshooting**

If the frame is not received, check:

1. Both PCAN-USB FD modules are initialized successfully.
2. Both nodes are configured in CAN FD mode.
3. Nominal bit rate is same on both nodes.
4. Data bit rate is same on both nodes.
5. CAN_H is connected to CAN_H.
6. CAN_L is connected to CAN_L.
7. GND is connected to GND.
8. Termination is correct.
9. Measured resistance between CAN_H and CAN_L is approximately 60 ohm.

**Recovery**

If PCAN-View shows Error Passive or Bus-Off:

1. Stop transmission.
2. Close both PCAN-View windows.
3. Check wiring and termination.
4. Reopen both PCAN-View windows.
5. Reinitialize both nodes with identical CAN FD settings.
6. Transmit the test frame again.

**Conclusion**

Basic CAN FD communication is working between two PCAN-USB FD modules.
