# CAN Signal Encoding Demonstration

## Objective

Understand how real vehicle signals are encoded into CAN messages.

Students will learn:

- Signal Encoding
- Signal Decoding
- Multi-byte Signals
- Endianness
- Signal Packing

---

# Automotive Context

A CAN message carries bytes.

Engineers interpret those bytes as signals.

Example:

```text
CAN ID = 0x100

Data:

3C 03 5A 46 00 00 00 00
```

Actual meaning:

```text
Speed       = 60 km/h
Gear        = 3
Temperature = 90 °C
Fuel Level  = 70 %
```

---

# Exercise 1: Single Signal

## Vehicle Speed

```c
uint8_t speed = 60;

while (1)
{
    uint8_t msg[8] = {0};

    msg[0] = speed;

    CAN_SendMessage(0x100,
                    msg,
                    8);

    HAL_Delay(100);
}
```

---

## PCAN Output

```text
3C 00 00 00 00 00 00 00
```

---

## Decoding

```text
0x3C = 60
```

Vehicle Speed:

```text
60 km/h
```

---

# Exercise 2: Multiple Signals

## Dashboard Frame

```c
uint8_t speed = 60;
uint8_t gear = 3;
uint8_t temperature = 90;
uint8_t fuel = 70;
```

```c
msg[0] = speed;
msg[1] = gear;
msg[2] = temperature;
msg[3] = fuel;
```

---

## PCAN Output

```text
3C 03 5A 46 00 00 00 00
```

---

## Decoding

```text
0x3C = 60
0x03 = 3
0x5A = 90
0x46 = 70
```

Result:

```text
Speed       = 60 km/h
Gear        = 3
Temperature = 90 °C
Fuel        = 70 %
```

---

# Exercise 3: Engine RPM

## Problem

RPM can exceed:

```text
255
```

Therefore one byte is not enough.

---

## Transmission

```c
uint16_t rpm = 2500;

msg[0] = rpm & 0xFF;
msg[1] = rpm >> 8;
```

---

## PCAN Output

```text
C4 09
```

---

## Decoding

```text
rpm =
(Byte1 << 8) | Byte0
```

Calculation:

```text
RPM =
(0x09 << 8) | 0xC4

RPM = 2500
```

---

# Exercise 4: Signal Scaling

## Example

Temperature resolution:

```text
0.5 °C
```

Actual temperature:

```text
90 °C
```

Transmitted value:

```text
90 / 0.5 = 180
```

---

## Transmission

```c
uint8_t temp_raw = 180;

msg[0] = temp_raw;
```

---

## PCAN Output

```text
B4
```

---

## Decoding

```text
Temperature =
Raw × 0.5
```

```text
180 × 0.5

= 90 °C
```

---

# Exercise 5: Signal Offset

## Battery Temperature

Range:

```text
-40 °C to +215 °C
```

Offset:

```text
+40
```

---

## Actual Value

```text
25 °C
```

---

## Encoded Value

```text
25 + 40

= 65
```

---

## Transmission

```c
msg[0] = 65;
```

---

## Decoding

```text
Temperature =
Raw - 40
```

```text
65 - 40

= 25 °C
```

---

# Exercise 6: Vehicle Status Message

## Signals

| Byte | Signal |
|------|---------|
| 0 | Vehicle Speed |
| 1 | Gear |
| 2 | Temperature |
| 3 | Fuel |
| 4 | Ignition |
| 5 | Alive Counter |
| 6 | Reserved |
| 7 | Reserved |

---

## Code

```c
msg[0] = speed;
msg[1] = gear;
msg[2] = temperature;
msg[3] = fuel;
msg[4] = ignition;
msg[5] = aliveCounter++;
```

---

# Alive Counter Demonstration

Many automotive ECUs include:

```text
Alive Counter
Sequence Counter
Rolling Counter
```

---

## Example

```text
00
01
02
03
04
05
```

---

## Purpose

Detect:

```text
Lost Messages
Frozen ECU
Communication Issues
```

---

# Industry Example

A real vehicle may have:

```text
ID = 0x100

Byte0 = Vehicle Speed
Byte1 = Gear
Byte2 = Engine Temperature
Byte3 = Fuel Level
Byte4 = Ignition Status
Byte5 = Alive Counter
Byte6 = Checksum
Byte7 = Reserved
```

---

# Discussion Questions

## Question 1

Why can RPM not be stored in one byte?

### Answer

```text
One byte can store only 0–255.
```

---

## Question 2

Why are scaling and offset used?

### Answer

```text
Save bandwidth.
Avoid floating-point transmission.
```

---

## Question 3

What is an Alive Counter?

### Answer

```text
A counter that increments every message
to detect communication faults.
```

---

# Learning Outcomes

✅ Signal Encoding
✅ Signal Decoding
✅ Multi-byte Signals
✅ Endianness
✅ Scaling
✅ Offset
✅ Alive Counters
✅ Automotive Signal Engineering

---

# Key Takeaway

CAN transmits bytes.

Automotive engineers interpret those bytes as signals.

```text
Bytes
  ↓
Signals
  ↓
Vehicle Information
```

This is the foundation of DBC files and modern automotive communication systems.
