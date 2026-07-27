# CAN FD Diagnostic-Style Transfer

## Objective

Demonstrate how CAN FD helps carry larger diagnostic or calibration data in fewer frames.

## Setup

Use two PCAN-USB FD modules connected as:

```text
PCAN-USB FD #1  <---- CAN FD bus ---->  PCAN-USB FD #2
```

Configure both nodes as:

```text
Mode:             CAN FD
Nominal bit rate: 500 kbit/s
Data bit rate:    2 Mbit/s
```

Logical roles:

* Node 1 = Diagnostic tester
* Node 2 = ECU

**Important Note**

The words request and response are used at application level. They do not mean RTR.

# Tests 

**Use normal CAN FD data frames:**

* RTR: OFF
* Request Frame

**From Node 1, transmit:**

````
ID:      7E0
Length:  8
Type:    CAN FD
BRS:     ON
RTR:     OFF
Data:    03 22 F1 90 00 00 00 00
````

Meaning
* 03       = Three useful bytes follow
* 22       = ReadDataByIdentifier service
* F1 90    = Example data identifier
* 00...    = Padding bytes

**Expected Observation**

On Node 2:

````text
ID:      0x7E0
Length:  8
Data:    03 22 F1 90 00 00 00 00
Type:    CAN FD
BRS:     ON
````

**Response Frame, From Node 2, transmit:**

````text
ID:      7E8
Length:  32
Type:    CAN FD
BRS:     ON
RTR:     OFF
Data:    1F 62 F1 90 4B 50 49 54 2D 41 50 45 58 2D 43 41 4E 46 44 2D 4C 41 42 2D 32 30 32 36 00 00 00 00
````

Meaning
* 1F          = 31 useful bytes follow
* 62          = Positive response to service 22
* F1 90       = Data identifier
* Remaining   = Example diagnostic payload


**ASCII payload portion**: KPIT-APEX-CANFD-LAB-2026

Expected Observation, On Node 1:

````text
ID:      0x7E8
Length:  32
Type:    CAN FD
BRS:     ON
Data:    1F 62 F1 90 ...
````

Optional 64-Byte Response, From Node 2, transmit:

````text
ID:      7E8
Length:  64
Type:    CAN FD
BRS:     ON
RTR:     OFF
Data:    3F 62 F1 90 4B 50 49 54 2D 41 50 45 58 2D 43 41 4E 46 44 2D 44 49 41 47 4E 4F 53 54 49 43 2D 44 45 4D 4F 2D 32 30 32 36 2D 43 41 4E 2D 46 44 2D 4C 41 42 00 00 00 00 00 00 00 00 00 00 00
````

**Classical CAN Comparison**

* Classical CAN: Maximum payload = 8 bytes
* CAN FD: Maximum payload = 64 bytes

**For 64 bytes of raw data:**

* Classical CAN requires multiple frames.
* CAN FD can carry the data in one frame.


In real diagnostics, Classical CAN also needs transport protocol segmentation for larger payloads.

## Key Observations
1. Diagnostic request and response are normal CAN/CAN FD data frames.
2. RTR is not used in this experiment.
3. CAN FD can carry larger diagnostic payloads in fewer frames.
4. BRS can further reduce data transfer time.

## Discussion Points
1. Why is RTR not used for this diagnostic-style transfer?
2. Why is CAN FD useful for diagnostics?
3. How does CAN FD reduce segmentation overhead?
4. Why is CAN FD useful for ECU flashing and calibration?

## Conclusion

CAN FD improves diagnostic and calibration data transfer by allowing larger payloads per frame and faster data-phase transmission when BRS is enabled.
