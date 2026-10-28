# Automotive CAN-to-SOME/IP Exercises

This laboratory series introduces students to **CAN communication**, **Gateway ECU concepts**, **SOME/IP**, **automotive Ethernet**, **data logging**, and **fault diagnostics**.

---

# Exercise 1: CAN Frame Validation

## Objective

To understand CAN communication by transmitting temperature data from a **Sensor ECU** and monitoring the CAN frames using **Technica CM CAN Combo**.

## Block Diagram

```text
      LM35 Sensor
           │
           ▼
    Arduino Mega
      Sensor ECU
           │
          SPI
           │
           ▼
       MCP2515
           │
        CAN Bus
           │
     ┌─────┴─────────┐
     │               │
     ▼               ▼
CM CAN Combo     CAN Monitor
```

## What the Student Will Learn

- ✅ Sensor acquisition
- ✅ CAN message creation
- ✅ CAN ID structure
- ✅ Bus communication
- ✅ CAN frame monitoring

## Procedure

### Step 1: Connect the LM35 Sensor

```text
OUT → A0
VCC → 5V
GND → GND
```

### Step 2: Upload Sensor ECU Code

Upload the Sensor ECU code to the **Arduino Mega**.

### Step 3: Generate Temperature Values

Example:

```text
Temperature = 30°C
```

### Step 4: Transmit the CAN Message

Use the following CAN ID:

```text
CAN ID = 0x100
```

### Step 5: Monitor the CAN Frame

Open **CM CAN Combo** and observe:

- Timestamp
- CAN ID
- Data Length
- Payload

## Expected Result

The following CAN information should be visible inside **CM CAN Combo**:

```text
CAN ID: 0x100
Data:   30
```

---

# Exercise 2: Gateway ECU Validation

## Objective

To convert a **CAN message** into a **SOME/IP message** using a **Raspberry Pi Gateway ECU**.

## Block Diagram

```text
        LM35
          │
          ▼
    Arduino Mega
          │
          ▼
       MCP2515
          │
       CAN Bus
          │
          ▼
    Raspberry Pi
     Gateway ECU
          │
┌─────────────────────┐
│ CAN Frame Received  │
│ SOME/IP Generated   │
└─────────────────────┘
```

## Learning Outcomes

Students will understand:

- ✅ Gateway ECU
- ✅ Network translation
- ✅ CAN-to-Ethernet migration
- ✅ Modern vehicle architecture

## Procedure

### Step 1: Start the Gateway ECU Software

Start the Gateway ECU software on the Raspberry Pi.

### Step 2: Verify CAN Frame Reception

Example:

```text
CAN ID: 0x100
```

### Step 3: Convert the CAN Frame to SOME/IP

Use:

```text
Service ID = 0x1234
Method ID  = 0x0001
```

### Step 4: Display the Output

Example:

```text
Temperature = 30
SOME/IP Message Sent
```

## Expected Result

For every CAN frame received:

```text
1 CAN Frame → 1 SOME/IP Packet Generated
```

---

# Exercise 3: SOME/IP Analysis

## Objective

To observe and decode **SOME/IP communication** using **ANDi**.

## Block Diagram

```text
    Gateway ECU
   (Raspberry Pi)
         │
         ▼
  Ethernet Switch
         │
         ▼
      ANDi PC
```

## Protocol Stack to Capture

```text
Ethernet
   │
   ▼
 IPv4
   │
   ▼
  UDP
   │
   ▼
SOME/IP
```

## Learning Outcomes

Students will understand:

- ✅ Service-Oriented Architecture
- ✅ SOME/IP
- ✅ Event communication
- ✅ Request-response messages

## Procedure

### Step 1: Start ANDi

Launch **ANDi**.

### Step 2: Select the Ethernet Interface

Select the Ethernet interface connected to the network.

### Step 3: Start Packet Capture

Begin capturing Ethernet traffic.

### Step 4: Observe the SOME/IP Packet

Monitor the following information:

```text
Source IP
Destination IP
Service ID
Message ID
Length
Payload
```

## Expected Result

The student should capture a SOME/IP packet containing:

```text
SOME/IP Packet

Service ID = 0x1234
Method ID  = 0x0001
```

---

# Exercise 4: Data Logging

## Objective

To store received **SOME/IP data** for later analysis.

## Block Diagram

```text
    Gateway ECU
         │
      SOME/IP
         │
         ▼
Raspberry Pi Logger
         │
         ▼
      CSV File
         or
      Database
```

## Learning Outcomes

Students will understand:

- ✅ Vehicle data logging
- ✅ Fleet data collection
- ✅ Black-box logging
- ✅ Diagnostics recording

## Procedure

### Step 1: Start the Logger ECU

Start the Logger ECU software.

### Step 2: Receive SOME/IP Packets

The Logger ECU receives SOME/IP packets from the Gateway ECU.

### Step 3: Store the Data

Record the following:

```text
Timestamp
Temperature
Service ID
Packet Number
```

### Step 4: Generate a CSV File

Store the received information in CSV format.

## Expected Output

```csv
Timestamp,Temp,ServiceID
10:00:00,30,0x1234
10:00:01,31,0x1234
10:00:02,32,0x1234
```

---

# Exercise 5: Fault Injection

## Objective

To study network behaviour when communication faults occur.

## Block Diagram

```text
    Gateway ECU
         │
         ▼
  Ethernet Switch
         │
         X
     Disconnect
         │
         ▼
     Logger ECU
```

## Learning Outcomes

Students will understand:

- ✅ Network failures
- ✅ Vehicle diagnostics
- ✅ Recovery behaviour
- ✅ Robust system design

---

## Procedure A: Ethernet Failure

### Step 1: Start Communication

Start normal communication between the Gateway ECU and Logger ECU.

### Step 2: Disconnect Ethernet

Disconnect the Ethernet cable.

### Step 3: Observe the Failure

Observe:

```text
Packet Loss
Communication Failure
```

### Step 4: Reconnect Ethernet

Reconnect the Ethernet cable.

### Step 5: Measure Recovery

Measure:

```text
Recovery Time
```

---

## Procedure B: CAN Failure

### Step 1: Introduce the CAN Fault

Disconnect:

```text
CAN_H
```

### Step 2: Observe CAN Communication

Observe:

```text
No CAN Frames
CAN Errors
```

## Expected Result

The student should identify:

```text
Communication Loss
Error Condition
Recovery Event
```

---

# Final Integrated Demonstration

The complete setup combines all the laboratory exercises into an end-to-end vehicle communication architecture.

## System Block Diagram

```text
             LM35
               │
               ▼
         Arduino Mega
               │
               ▼
            MCP2515
               │
               ▼
            CAN BUS
               │
       ┌───────┴────────► CM CAN Combo
       │
       ▼
    Raspberry Pi-1
     Gateway ECU
       │
       ▼
  Ethernet Switch
       │
       ├───────────────► ANDi Analyzer
       │
       ▼
  Media Converter
       │
       ▼
    Fiber Link
       │
       ▼
  Media Converter
       │
       ▼
    Raspberry Pi-2
      Logger ECU
       │
       ▼
   CSV / Database
```

---

# Student Journey

| Exercise | Student Understands |
|---|---|
| **Exercise 1** | CAN Communication |
| **Exercise 2** | Gateway ECU Concept |
| **Exercise 3** | SOME/IP Protocol |
| **Exercise 4** | Data Logging |
| **Exercise 5** | Network Diagnostics |
| **Full Demonstration** | Modern Vehicle E/E Architecture |

---

# Overall Learning Flow

```text
Sensor Measurement
       │
       ▼
CAN Frame Generation
       │
       ▼
CAN Bus Communication
       │
       ▼
CAN Frame Monitoring
       │
       ▼
Gateway ECU
       │
       ▼
CAN-to-SOME/IP Conversion
       │
       ▼
Automotive Ethernet
       │
       ▼
SOME/IP Analysis
       │
       ▼
Vehicle Data Logging
       │
       ▼
Fault Injection & Diagnostics
       │
       ▼
Modern Vehicle E/E Architecture
```

---

# End-to-End Communication Flow

```text
LM35 Sensor
    │
    ▼
Arduino Mega
(Sensor ECU)
    │
    ▼
MCP2515
    │
    ▼
CAN Bus
    │
    ├────────► CM CAN Combo
    │
    ▼
Raspberry Pi-1
(Gateway ECU)
    │
    │ CAN → SOME/IP
    ▼
Ethernet Switch
    │
    ├────────► ANDi Analyzer
    │
    ▼
Media Converter
    │
    ▼
Fiber Link
    │
    ▼
Media Converter
    │
    ▼
Raspberry Pi-2
(Logger ECU)
    │
    ▼
CSV / Database
```

---

# Conclusion

This laboratory sequence takes students from a simple **LM35 temperature sensor reading** through:

1. **Sensor data acquisition**
2. **CAN frame generation**
3. **CAN bus communication**
4. **CAN frame monitoring**
5. **Gateway ECU operation**
6. **CAN-to-SOME/IP conversion**
7. **Automotive Ethernet communication**
8. **SOME/IP packet analysis**
9. **Vehicle data logging**
10. **Fault injection and diagnostics**

The complete communication path is:

**LM35 Sensor → Arduino Mega → MCP2515 → CAN Bus → Gateway ECU → SOME/IP → Ethernet Network → Logger ECU → CSV/Database**

The complete demonstration therefore provides students with an end-to-end view of a vehicle communication architecture, progressing from traditional **CAN-based communication** towards a **service-oriented Software-Defined Vehicle (SDV) architecture**.
