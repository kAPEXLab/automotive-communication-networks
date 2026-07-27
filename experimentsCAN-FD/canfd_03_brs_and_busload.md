# BRS and Bus Load

## Objective

Demonstrate the effect of Bit Rate Switch, BRS, on CAN FD bus load.

## Setup

Use two PCAN-USB FD modules connected as before.

Both nodes should be configured as:

* Mode:             CAN FD
* Nominal bit rate: 500 kbit/s
* Data bit rate:    2 Mbit/s

## Concept

CAN FD uses two bit-rate regions when BRS is enabled.

* **Nominal bit rate**: Used during arbitration phase.
* **Data bit rate**: Used during data phase when BRS is ON.


## For this experiment:

* Nominal bit rate = 500 kbit/s
* Data bit rate    = 2 Mbit/s

**BRS OFF Meaning**
* Arbitration phase = 500 kbit/s
* Data phase        = 500 kbit/s

**BRS ON Meaning**
* Arbitration phase = 500 kbit/s
* Data phase        = 2 Mbit/s

## Test 1: 64-Byte CAN FD Frame with BRS OFF

**From Node 1, transmit cyclic frame:**
```
ID:      201
Length:  64
Type:    CAN FD
BRS:     OFF
RTR:     OFF
Cycle:   10 ms
Data:    00 01 02 03 04 05 06 07 08 09 0A 0B 0C 0D 0E 0F 10 11 12 13 14 15 16 17 18 19 1A 1B 1C 1D 1E 1F 20 21 22 23 24 25 26 27 28 29 2A 2B 2C 2D 2E 2F 30 31 32 33 34 35 36 37 38 39 3A 3B 3C 3D 3E 3F
```

**Observe bus load in PCAN-View.**

Record:

BRS OFF bus load = ______ %

## Test 2: 64-Byte CAN FD Frame with BRS ON

Stop the previous cyclic transmission.

**From Node 1, transmit cyclic frame:**

```
ID:      202
Length:  64
Type:    CAN FD
BRS:     ON
RTR:     OFF
Cycle:   10 ms
Data:    00 01 02 03 04 05 06 07 08 09 0A 0B 0C 0D 0E 0F 10 11 12 13 14 15 16 17 18 19 1A 1B 1C 1D 1E 1F 20 21 22 23 24 25 26 27 28 29 2A 2B 2C 2D 2E 2F 30 31 32 33 34 35 36 37 38 39 3A 3B 3C 3D 3E 3F
```

**Observe bus load in PCAN-View.**

Record:

BRS ON bus load = ______ %

**Expected Observation**
>> BRS OFF bus load > BRS ON bus load


This happens because the payload and cycle time are same, but the data phase is transmitted faster when BRS is ON.

## Oscilloscope Observation

If an oscilloscope is available, probe: `CAN_H with respect to CAN_GND` or `CAN_L with respect to CAN_GND`

Expected waveform observation:

* Before BRS: wider bits
* After BRS:  narrower bits


For this configuration:

* 500 kbit/s bit time = 2 us
* 2 Mbit/s bit time   = 0.5 us

## Key Observations
1. BRS does not change the arbitration bit rate.
2. BRS changes the data phase bit rate.
3. For the same payload and cycle time, BRS ON reduces bus occupation.
4. Lower bus occupation results in lower bus load.

## Discussion Points
1. Why is arbitration not performed at the higher data bit rate?
2. Why does BRS ON reduce bus load?
3. What will happen if one node uses 2 Mbit/s data bit rate and another uses 4 Mbit/s?
4. Why is CAN FD useful for high-data applications such as diagnostics and flashing?

## Conclusion

BRS allows CAN FD to keep reliable arbitration at the nominal bit rate while transmitting the data field at a higher bit rate. This improves throughput and reduces bus load.
