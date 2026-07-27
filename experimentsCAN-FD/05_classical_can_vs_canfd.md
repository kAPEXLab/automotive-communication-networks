# Classical CAN vs CAN FD

## Objective

Compare Classical CAN payload limitation with CAN FD payload capability.

## Setup

Use two PCAN-USB FD modules connected as:

```text
PCAN-USB FD #1  <---- CAN FD bus ---->  PCAN-USB FD #2
```

# Part A: Classical CAN Mode

Configure both nodes in PCAN-View:

```text
Mode:     Classical CAN
Bit rate: 500 kbit/s
```

## Test 1: Valid Classical CAN Frame

Transmit from Node 1:

```text
ID:      401
Length:  8
RTR:     OFF
Data:    11 22 33 44 55 66 77 88
```

## Expected Observation

On Node 2:

```text
ID 0x401 received successfully.
Length = 8 bytes.
```

## Test 2: Try More Than 8 Bytes in Classical CAN

Try to transmit from Node 1:

```text
ID:      402
Length:  12
Data:    00 01 02 03 04 05 06 07 08 09 0A 0B
```

## Expected Observation

Possible tool behavior:

```text
PCAN-View may reject the frame.
```

or

```text
PCAN-View may limit the length to 8 bytes.
```

## Explanation

Classical CAN supports a maximum payload of 8 data bytes.

Therefore, 12-byte, 32-byte, or 64-byte payloads are not valid in Classical CAN mode.

# Part B: CAN FD Mode

Close both PCAN-View windows.

Reopen both nodes with CAN FD configuration:

```text
Mode:             CAN FD
Nominal bit rate: 500 kbit/s
Data bit rate:    2 Mbit/s
```

## Test 3: 64-Byte CAN FD Frame

Transmit from Node 1:

```text
ID:      403
Length:  64
Type:    CAN FD
BRS:     ON
RTR:     OFF
Data:    00 01 02 03 04 05 06 07 08 09 0A 0B 0C 0D 0E 0F 10 11 12 13 14 15 16 17 18 19 1A 1B 1C 1D 1E 1F 20 21 22 23 24 25 26 27 28 29 2A 2B 2C 2D 2E 2F 30 31 32 33 34 35 36 37 38 39 3A 3B 3C 3D 3E 3F
```

## Expected Observation

On Node 2:

```text
ID 0x403 received successfully.
Length = 64 bytes.
Type = CAN FD.
BRS = ON.
```

## Key Comparison

```text
Classical CAN:
Maximum payload = 8 bytes

CAN FD:
Maximum payload = 64 bytes
```

## Important Note

CAN FD-capable hardware can transmit Classical CAN frames.

However, a Classical CAN-only node cannot decode CAN FD frames.

## Optional Compatibility Check

Configure Node 1 as:

```text
Mode: CAN FD
Nominal bit rate: 500 kbit/s
Data bit rate: 2 Mbit/s
```

Configure Node 2 as:

```text
Mode: Classical CAN
Bit rate: 500 kbit/s
```

From Node 1, first transmit a Classical CAN frame:

```text
ID:      404
Length:  8
RTR:     OFF
Data:    11 22 33 44 55 66 77 88
```

Expected:

```text
Node 2 receives the frame.
```

Now transmit a CAN FD frame from Node 1:

```text
ID:      405
Length:  16
Type:    CAN FD
BRS:     ON
RTR:     OFF
Data:    00 01 02 03 04 05 06 07 08 09 0A 0B 0C 0D 0E 0F
```

Expected:

```text
Node 2 does not properly receive the CAN FD frame.
Error Passive Warning or bus error may appear.
```

## Recovery

After the compatibility check:

```text
1. Stop transmission.
2. Close both PCAN-View windows.
3. Reopen both nodes in CAN FD mode.
4. Use identical CAN FD timing on both nodes.
5. Verify normal communication again.
```

Use:

```text
Mode:             CAN FD
Nominal bit rate: 500 kbit/s
Data bit rate:    2 Mbit/s
```

## Discussion Points

```text
1. Why does Classical CAN support only 8 bytes?
2. Why can CAN FD transmit 64 bytes?
3. Can a CAN FD node transmit Classical CAN frames?
4. Can a Classical CAN-only node decode CAN FD frames?
5. What happens if an old Classical CAN ECU is present on a CAN FD bus?
```

## Conclusion

Classical CAN supports up to 8 data bytes per frame. CAN FD extends the payload up to 64 bytes and can improve data transfer efficiency. However, CAN FD frames require CAN FD-capable nodes on the bus.
