# CAN FD Payload and DLC

## Objective

Demonstrate that CAN FD supports payloads larger than 8 bytes and understand CAN FD DLC mapping.

## Setup

Use two PCAN-USB FD modules connected.

Both nodes should be configured as:

```
Mode:             CAN FD
Nominal bit rate: 500 kbit/s
Data bit rate:    2 Mbit/s
```

## Test 1: 8-Byte CAN FD Frame

**Transmit from Node 1:**

````
ID:      101
Length:  8
Type:    CAN FD
BRS:     ON
RTR:     OFF
Data:    11 22 33 44 55 66 77 88
````

**Expected Observation On Node 2:**

````
ID:      0x101
Length:  8
Data:    11 22 33 44 55 66 77 88
Type:    CAN FD
BRS:     ON
````

## Test 2: 64-Byte CAN FD Frame

**Transmit from Node 1:**

```
ID:      102
Length:  64
Type:    CAN FD
BRS:     ON
RTR:     OFF
Data:    00 01 02 03 04 05 06 07 08 09 0A 0B 0C 0D 0E 0F 10 11 12 13 14 15 16 17 18 19 1A 1B 1C 1D 1E 1F 20 21 22 23 24 25 26 27 28 29 2A 2B 2C 2D 2E 2F 30 31 32 33 34 35 36 37 38 39 3A 3B 3C 3D 3E 3F
```

**Expected Observation On Node 2:**

```
ID:      0x102
Length:  64
Data:    00 01 02 ... 3F
Type:    CAN FD
BRS:     ON
```

## Test 3: 12-Byte CAN FD Frame

**Transmit from Node 1:**

```
ID:      103
Length:  12
Type:    CAN FD
BRS:     ON
RTR:     OFF
Data:    00 11 22 33 44 55 66 77 88 99 AA BB
```

**Expected Observation On Node 2:**

```
ID:      0x103
Length:  12
Data:    00 11 22 33 44 55 66 77 88 99 AA BB
Type:    CAN FD
BRS:     ON
```

## CAN FD DLC Mapping

In Classical CAN, DLC directly represents 0 to 8 data bytes.

In CAN FD, DLC values above 8 are mapped as follows:

* DLC 0  = 0 bytes
* DLC 1  = 1 byte
* DLC 2  = 2 bytes
* DLC 3  = 3 bytes
* DLC 4  = 4 bytes
* DLC 5  = 5 bytes
* DLC 6  = 6 bytes
* DLC 7  = 7 bytes
* DLC 8  = 8 bytes
* DLC 9  = 12 bytes
* DLC 10 = 16 bytes
* DLC 11 = 20 bytes
* DLC 12 = 24 bytes
* DLC 13 = 32 bytes
* DLC 14 = 48 bytes
* DLC 15 = 64 bytes

## Key Observations
1. CAN FD can transmit more than 8 bytes in one frame.
2. Maximum CAN FD payload is 64 bytes.
3. DLC values above 8 do not increase linearly.
4. DLC 15 represents 64 bytes, not 15 bytes.

## Discussion Points
1. Why does Classical CAN not support 64-byte payload?
2. Why does CAN FD use special DLC mapping above 8 bytes?
3. How many Classical CAN frames are required to transmit 64 bytes?
4. What is the advantage of transmitting 64 bytes in a single CAN FD frame?

## Conclusion

CAN FD extends the payload capacity from 8 bytes to 64 bytes. This reduces the number of frames required for large data transfers and improves communication efficiency.
