# CAN Hardware Filtering

## Objective

Understand how automotive ECUs receive only the CAN messages they require and automatically ignore all others.

---

# Background

Every ECU connected to a CAN network can physically see all CAN messages.

Example:

```text
0x100
0x200
0x300
```

However, most ECUs need only a subset of these messages.

Instead of processing every message in software, CAN controllers use hardware filters.

---

# Automotive Example

## Instrument Cluster

Needs:

```text
0x100 Vehicle Speed
0x101 Engine RPM
```

Does NOT need:

```text
0x200 Door Status
0x300 Wiper Status
0x400 Headlamp Status
```

The CAN filter rejects unwanted messages before they reach the CPU.

---

# Hardware Required

## Transmitter

- STM32F446RE
- SN65HVD230

## Receiver

- STM32F446RE
- SN65HVD230

## Analyzer

- PCAN-USB (Optional)

---

# Network Diagram

```text
                PCAN

                  |
                  |
----------------------------------
|                                |
|                                |
TX Node                      RX Node
STM32                        STM32
```

---

# Transmitter Configuration

Transmit three messages:

```c
CAN_SendMessage(0x100, msg, 8);
CAN_SendMessage(0x200, msg, 8);
CAN_SendMessage(0x300, msg, 8);
```

---

# Step 1: No Filter

Receiver accepts all messages.

Filter:

```c
FilterIdHigh      = 0x0000;
FilterMaskIdHigh  = 0x0000;
```

This is the configuration currently used in the framework.

---

## Expected Result

Receiver receives:

```text
0x100
0x200
0x300
```

---

# Step 2: Accept Only ID 0x100

Receiver filter configuration:

```c
filter.FilterBank = 0;
filter.FilterMode = CAN_FILTERMODE_IDMASK;
filter.FilterScale = CAN_FILTERSCALE_32BIT;

filter.FilterIdHigh = (0x100 << 5);

filter.FilterIdLow = 0x0000;

filter.FilterMaskIdHigh = (0x7FF << 5);

filter.FilterMaskIdLow = 0x0000;

filter.FilterFIFOAssignment = CAN_FILTER_FIFO0;
filter.FilterActivation = ENABLE;
```

---

# Receiver Application

Toggle LED when a message is received:

```c
HAL_GPIO_TogglePin(LD2_GPIO_Port,
                   LD2_Pin);
```

---

# Expected Result

Messages transmitted:

```text
0x100
0x200
0x300
```

Messages accepted:

```text
0x100
```

Messages ignored:

```text
0x200
0x300
```

---

# Demonstration

## Before Filter

```text
0x100 -> Received
0x200 -> Received
0x300 -> Received
```

---

## After Filter

```text
0x100 -> Received
0x200 -> Rejected
0x300 -> Rejected
```

---

# Student Discussion Questions

## Question 1

Why should an ECU reject unwanted messages?

### Answer

```text
Reduce CPU load
Reduce interrupt load
Improve efficiency
```

---

## Question 2

Does filtering stop messages from appearing on the CAN bus?

### Answer

```text
No
```

All messages remain on the CAN bus.

Filtering only controls which messages an ECU accepts.

---

## Question 3

Can two ECUs use different filters?

### Answer

```text
Yes
```

Each ECU can accept a different subset of messages.


---

# Key Takeaway

Filtering does NOT stop messages from being transmitted.

Filtering determines which messages an ECU chooses to process.

```text
CAN Bus  -> Everyone can hear

CAN Filter -> Determines who listens
```
