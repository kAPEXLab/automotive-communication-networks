# CAN Alive Counter Demonstration

## Objective

Understand how automotive ECUs detect:

- Missing Messages
- Stuck Controllers
- Communication Failure
- ECU Reset

---

# Background

Most automotive ECUs periodically transmit a counter.

Example:

```text
0
1
2
3
4
5
...
```

This is called:

```text
Alive Counter
Rolling Counter
Sequence Counter
```

---

# Why Is It Needed?

Suppose an Engine ECU transmits:

```text
RPM
Temperature
```

every:

```text
100 ms
```

How does the Dashboard ECU know the Engine ECU is still alive?

Using an Alive Counter.

---

# Message Design

## CAN ID

```text
0x100
```

## Signals

| Byte | Signal |
|--------|----------|
| 0 | Speed |
| 1 | Gear |
| 2 | Temperature |
| 3 | Fuel |
| 4 | Ignition |
| 5 | Alive Counter |

---

# STM32 Code

```c
uint8_t speed = 60;
uint8_t gear = 3;
uint8_t temperature = 90;
uint8_t fuel = 70;
uint8_t ignition = 1;
uint8_t aliveCounter = 0;

while (1)
{
    uint8_t msg[8] = {0};

    msg[0] = speed;
    msg[1] = gear;
    msg[2] = temperature;
    msg[3] = fuel;
    msg[4] = ignition;
    msg[5] = aliveCounter++;

    CAN_SendMessage(0x100,
                    msg,
                    8);

    HAL_Delay(1000);
}
```

---

# Expected PCAN Output

```text
3C 03 5A 46 01 00 00 00
3C 03 5A 46 01 01 00 00
3C 03 5A 46 01 02 00 00
3C 03 5A 46 01 03 00 00
3C 03 5A 46 01 04 00 00
```

---

# Student Observation

Question:

```text
Which byte changes?
```

Answer:

```text
Byte 5
```

---

# Fault Injection

Press RESET on STM32.

Observe:

```text
00
01
02
03
...
```

suddenly become:

```text
00
01
02
```

again.

---

# Discussion

Receiver ECU detects:

```text
Alive Counter restarted
```

Possible reasons:

- ECU Reset
- Power Interruption
- Watchdog Reset

---

# Advanced Exercise

Use a 4-bit counter.

```c
aliveCounter++;

aliveCounter &= 0x0F;
```

Result:

```text
0
1
2
...
14
15
0
1
2
...
```

---

# Automotive Examples

## Engine ECU

```text
Engine_Status
```

contains:

```text
Alive Counter
```

---

## ABS ECU

```text
Wheel Speed Message
```

contains:

```text
Alive Counter
```

---

## Steering ECU

```text
Steering Angle Message
```

contains:

```text
Alive Counter
```

---

# Learning Outcomes

Students understand:

✅ Alive Counter

✅ Rolling Counter

✅ Message Supervision

✅ ECU Health Monitoring

✅ Automotive Diagnostics Concepts

---

# Key Takeaway

An ECU must not only send data.

An ECU must also prove:

```text
I am alive.
I am transmitting correctly.
I have not frozen.
```

Alive counters are one of the simplest and most widely used techniques for achieving this.
