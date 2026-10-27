# CAN Feature Demonstrations
## Arbitration, Bus Load and Filtering

This document contains hands-on exercises focused on core CAN bus features used in automotive systems.

---

# Exercise 1: Arbitration Demo

## Objective

Understand how CAN resolves simultaneous transmissions without collisions.

Demonstrate:

```text
Lower CAN ID = Higher Priority
```

---

## Hardware

### Node A

- STM32F446RE
- SN65HVD230

### Node B

- STM32F446RE
- SN65HVD230

### Analyzer

- PCAN-USB
- PCAN-View

---

## CAN Network

```text
          PCAN

            |
            |
---------------------------------
|                               |
|                               |
STM32 A                    STM32 B
ID=0x100                   ID=0x200
```

---

## Node A Code

```c
while (1)
{
    uint8_t msg[8] = {0};

    CAN_SendMessage(0x100,
                    msg,
                    8);
}
```

---

## Node B Code

```c
while (1)
{
    uint8_t msg[8] = {0};

    CAN_SendMessage(0x200,
                    msg,
                    8);
}
```

---

## Procedure

1. Program both nodes.
2. Start both nodes simultaneously.
3. Observe messages in PCAN.

---

## Observation

Messages from:

```text
0x100
```

appear before:

```text
0x200
```

---

## Discussion

During arbitration:

```text
Node A -> 0x100
Node B -> 0x200
```

CAN compares IDs bit-by-bit.

Since:

```text
0x100 < 0x200
```

Node A wins arbitration.

Node B automatically retries.

---

## Learning Outcome

Students understand:

- Priority-based communication
- Non-destructive arbitration
- Why CAN does not suffer collisions

---

# Exercise 2: Bus Load Demo

## Objective

Understand the effect of transmission rate on CAN network utilization.

---

## Hardware

- STM32F446RE
- SN65HVD230
- PCAN-USB

---

## Code

```c
while (1)
{
    uint8_t msg[8] = {0};

    CAN_SendMessage(0x100,
                    msg,
                    8);

    HAL_Delay(1000);
}
```

---

## Experiment 1

### Transmission Rate

```text
1000 ms
```

### Expected

```text
1 Message / Second
```

Observe in PCAN:

```text
Low Bus Load
```

---

## Experiment 2

Change to:

```c
HAL_Delay(100);
```

### Expected

```text
10 Messages / Second
```

Observe:

```text
Higher Bus Load
```

---

## Experiment 3

Change to:

```c
HAL_Delay(10);
```

### Expected

```text
100 Messages / Second
```

Observe:

```text
Much Higher Bus Load
```

---

## Discussion

Increasing message rate causes:

```text
Higher Bus Utilization
More Network Traffic
Reduced Available Bandwidth
```

---

## Learning Outcome

Students understand:

- Periodic Messages
- Bus Utilization
- Message Frequency
- Network Capacity

---

## Automotive Example

Consider an instrument cluster.

It may require only:

```text
Vehicle Speed
Engine RPM
Fuel Level
```

Messages related to:

```text
Door Status
Window Control
Headlamp Control
```

can be ignored.

---
# Summary

| Demonstration | Concept Learned |
|---------------|------------------|
| Arbitration | Lower ID = Higher Priority |
| Bus Load | Message Rate vs Network Utilization |
| Filtering | ECU Accepts Only Required Messages |

---

# Interview Questions

## Arbitration

1. Why does lower CAN ID have higher priority?
2. What happens to the losing node during arbitration?

---

## Bus Load

1. What happens if bus load becomes very high?
2. Why are message periods important?

---

## Filtering

1. Why do ECUs use CAN filters?
2. Does every ECU process every message on the bus?

---

# Expected Industry Understanding

After completing these exercises, students should be able to explain:

✅ CAN Priority Mechanism
✅ Arbitration Process
✅ Bus Utilization
✅ Message Frequency
✅ CAN Filtering
✅ Real Automotive ECU Communication
