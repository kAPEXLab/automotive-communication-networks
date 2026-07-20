# CAN Fault Injection Demonstration

## Objective

Understand common CAN network failures and learn systematic debugging techniques.

---

# Hardware Required

- STM32F446RE
- SN65HVD230
- PCAN-USB
- PCAN-View

---

# Network Setup

```text
STM32
   |
SN65HVD230
   |
CAN Bus
   |
PCAN
```

Verify normal communication before starting.

Expected:

```text
ID=0x100
Data=11 22 33 44 55 66 77 88
```

visible in PCAN-View.

---

# Fault 1: Bit Rate Mismatch

## Objective

Understand the importance of matching CAN bit rates.

---

## Normal Configuration

```text
STM32 = 500 kbps
PCAN  = 500 kbps
```

Communication works.

---

## Inject Fault

Change PCAN:

```text
500 kbps → 250 kbps
```

Keep STM32 at:

```text
500 kbps
```

---

## Observation

```text
No messages visible
```

or

```text
Bus Errors
```

---

## Discussion

All CAN nodes must use the same bit rate.

---

# Fault 2: Missing Ground

## Objective

Understand why CAN nodes require a common reference.

---

## Normal Connection

```text
CANH
CANL
GND
```

connected.

---

## Inject Fault

Disconnect:

```text
GND
```

---

## Observation

```text
Communication failure
```

or

```text
Intermittent operation
```

---

## Discussion

CANH and CANL are differential signals, but nodes still require a common electrical reference.

---

# Fault 3: Missing Termination

## Objective

Understand the role of termination resistors.

---

## Normal Bus

```text
120Ω ---- CAN BUS ---- 120Ω
```

Measure:

```text
60 Ω
```

between CANH and CANL.

---

## Inject Fault

Remove one termination resistor.

---

## Observation

Possible:

```text
Communication errors
Reduced reliability
```

---

## Discussion

Termination prevents signal reflections.

---

# Fault 4: Wrong CAN ID

## Objective

Understand message identification.

---

## Normal Message

```c
CAN_SendMessage(0x100,
                msg,
                8);
```

---

## Inject Fault

Change to:

```c
CAN_SendMessage(0x101,
                msg,
                8);
```

---

## Observation

Message appears under a different ID.

Applications expecting:

```text
0x100
```

may stop working.

---

## Discussion

CAN ID acts as the message identifier.

---

# Fault 5: Hardware Filter Mismatch

## Receiver Filter

Accept only:

```text
0x100
```

---

## Inject Fault

Transmit:

```text
0x200
```

---

## Observation

No messages received.

---

## Discussion

The message is present on the bus but rejected by the receiving ECU.

---

# Summary

| Fault | Expected Observation |
|---------|---------------------|
| Wrong Bit Rate | No Communication |
| Missing Ground | Unstable Communication |
| Missing Termination | Errors / Reliability Issues |
| Wrong CAN ID | Unexpected Message |
| Filter Mismatch | Message Ignored |

---

# Key Takeaway

A CAN engineer should not only know how to transmit a message.

A CAN engineer should be able to identify why communication fails.

Most real-world debugging starts with checking:

```text
1. Power
2. Ground
3. CANH/CANL
4. Termination
5. Bit Rate
6. Message ID
7. Filters
```
