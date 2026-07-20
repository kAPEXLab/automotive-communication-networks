# CAN Diagnostics and DTC Demonstration

## Objective

Understand how ECUs detect faults and communicate diagnostic information over CAN.

Students will learn:

- What is a DTC
- How faults are detected
- How an ECU reports faults
- How a dashboard reacts

---

# Automotive Scenario

Suppose the Engine ECU monitors:

```text
Engine Temperature
```

Normal:

```text
90 °C
```

Fault Condition:

```text
Temperature > 110 °C
```

The ECU detects overheating.

---

# Normal Operation

## CAN ID

```text
0x100
```

## Signals

| Byte | Signal |
|--------|---------|
| 0 | Speed |
| 1 | Temperature |
| 2 | Engine Status |

---

## Example

```text
Speed       = 60 km/h
Temperature = 90 °C
Status      = 0
```

---

## PCAN Output

```text
3C 5A 00 00 00 00 00 00
```

---

# Fault Condition

Engine temperature rises.

```text
Temperature = 120 °C
```

Status:

```text
1 = Fault Present
```

---

## PCAN Output

```text
3C 78 01 00 00 00 00 00
```

---

# STM32 Code

```c
uint8_t speed = 60;
uint8_t temperature = 90;

while (1)
{
    uint8_t msg[8] = {0};

    if (temperature > 110)
    {
        msg[2] = 1;
    }

    msg[0] = speed;
    msg[1] = temperature;

    CAN_SendMessage(0x100,
                    msg,
                    8);

    HAL_Delay(1000);
}
```

---

# DTC Concept

In production vehicles, ECUs store:

```text
Diagnostic Trouble Codes
```

Examples:

```text
P0115 Engine Coolant Temperature Sensor
P0300 Engine Misfire
P0128 Coolant Temperature Below Threshold
```

---

# Training Example

| Value | Meaning |
|---------|---------|
| 0 | No Fault |
| 1 | Engine Overheat |
| 2 | Sensor Failure |
| 3 | Communication Fault |

---

# Exercise 1

Transmit:

```text
3C 5A 00
```

Students decode:

```text
Speed = 60
Temperature = 90
Fault = No
```

---

# Exercise 2

Transmit:

```text
3C 78 01
```

Students decode:

```text
Speed = 60
Temperature = 120
Fault = Engine Overheat
```

---

# Dashboard Behaviour

When:

```text
Fault = 0
```

Dashboard:

```text
No Warning
```

When:

```text
Fault = 1
```

Dashboard:

```text
Engine Warning Lamp ON
```

---

# Exercise 3

Create a Fault Indicator

```c
msg[2] = 1;
```

Observe:

```text
Warning Condition Active
```

---

# Communication Fault Example

Suppose the Engine ECU stops transmitting.

Dashboard waits:

```text
100 ms
200 ms
300 ms
...
```

No message received.

Dashboard reports:

```text
Communication Fault
```

---

# Industry Relevance

Real ECUs monitor:

```text
Missing Messages
Sensor Failures
Power Issues
Overtemperature
Overvoltage
Communication Errors
```

and store DTCs.

---

# Learning Outcomes

Students understand:

✅ Diagnostic Trouble Codes (DTC)

✅ Fault Status Signals

✅ Warning Indicators

✅ Communication Faults

✅ Fault Monitoring

✅ ECU Diagnostics Basics

---

# Discussion Questions

1. Why should an ECU transmit fault status?
2. What happens if a critical message is lost?
3. How does a dashboard know a sensor has failed?
4. Why are DTCs stored?

---

# Key Takeaway

CAN is not only used to transmit data.

CAN is also used to communicate:

```text
Faults
Warnings
Health Status
Diagnostics
```

which are essential for safe and reliable vehicle operation.
