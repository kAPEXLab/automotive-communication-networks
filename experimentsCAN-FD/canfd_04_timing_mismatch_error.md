# CAN FD Timing Mismatch Error

## Objective

Demonstrate that CAN FD communication requires matching nominal bit rate and data bit rate.

## Setup

Use two PCAN-USB FD modules connected as as before.

**Initial Working Configuration**

Configure Node 1:

```
Mode:             CAN FD
Nominal bit rate: 500 kbit/s
Data bit rate:    2 Mbit/s
```

Configure Node 2:
```
Mode:             CAN FD
Nominal bit rate: 500 kbit/s
Data bit rate:    2 Mbit/s
```

## Verify Normal Communication

**From Node 1, transmit:**
```
ID:      301
Length:  8
Type:    CAN FD
BRS:     ON
RTR:     OFF
Data:    11 22 33 44 55 66 77 88
```

**Expected observation on Node 2:**
```
ID 0x301 received successfully.
```

## Fault Setup: Data Bit Rate Mismatch

Close PCAN-View for Node 2.

Reopen Node 2 with wrong data bit rate:
```
Mode:             CAN FD
Nominal bit rate: 500 kbit/s
Data bit rate:    4 Mbit/s
```

Node 1 remains configured as:
```
Mode:             CAN FD
Nominal bit rate: 500 kbit/s
Data bit rate:    2 Mbit/s
```

Now the setup is:
```
Node 1: Nominal = 500 kbit/s, Data = 2 Mbit/s
Node 2: Nominal = 500 kbit/s, Data = 4 Mbit/s
```

## Transmit CAN FD Frame with BRS ON

**From Node 1, transmit:**
```
ID:      302
Length:  64
Type:    CAN FD
BRS:     ON
RTR:     OFF
Data:    00 01 02 03 04 05 06 07 08 09 0A 0B 0C 0D 0E 0F 10 11 12 13 14 15 16 17 18 19 1A 1B 1C 1D 1E 1F 20 21 22 23 24 25 26 27 28 29 2A 2B 2C 2D 2E 2F 30 31 32 33 34 35 36 37 38 39 3A 3B 3C 3D 3E 3F
```

**Expected Observation**

Possible observations:
```
Frame not received
Error Passive Warning
Bus warning
Bus-Off
Transmit error
```

## Explanation

The nominal bit rate is same on both nodes, so arbitration can start correctly.

However, when BRS is enabled, Node 1 switches to 2 Mbit/s during the data phase.

Node 2 expects 4 Mbit/s during the data phase.

Because of this mismatch, Node 2 samples the data field at the wrong timing and fails to decode the frame correctly.

## Recovery Procedure
1. Stop transmission on Node 1.
2. Disconnect both PCAN-View windows.
3. Reconnect Node 1 and Node 2.
4. Configure both nodes with identical CAN FD timing.


Use:
```
Mode:             CAN FD
Nominal bit rate: 500 kbit/s
Data bit rate:    2 Mbit/s
```

**Verify Recovery**

From Node 1, transmit:

```
ID:      303
Length:  8
Type:    CAN FD
BRS:     ON
RTR:     OFF
Data:    11 22 33 44 55 66 77 88
```

Expected observation:

Node 2 receives the frame successfully.

## Key Observations
1. CAN FD requires matching nominal bit rate.
2. CAN FD with BRS ON also requires matching data bit rate.
3. Data bit rate mismatch can cause Error Passive or Bus-Off.
4. After Bus-Off, remove the fault and reinitialize the interface.

## Discussion Points
1. Why did communication work when both nodes used 2 Mbit/s data bit rate?
2. Why did communication fail when Node 2 used 4 Mbit/s data bit rate?
3. What is the role of BRS in this failure?
4. What would happen if BRS was OFF?

## Conclusion

For CAN FD communication, all participating nodes must use compatible timing. When BRS is enabled, both nominal bit rate and data bit rate must match.
