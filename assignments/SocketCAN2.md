# Problem Statement 2
# Design and Implement a Distributed System Using SocketCAN

## Background

Modern automotive, industrial, and IoT systems consist of multiple electronic control units (ECUs) that communicate over a network to exchange information and coordinate actions. Before actual hardware becomes available, engineers often prototype, simulate, verify, and validate such systems using software-only communication frameworks.

In this assignment, your team will act as a system engineering and software development team responsible for designing, implementing, testing, and demonstrating a complete distributed system using **SocketCAN**.

You must select a real-world application, design its CAN communication architecture, implement all nodes, and validate the overall system behavior.

---

## Objective

Design, implement, and test a complete distributed system consisting of multiple software ECUs communicating through SocketCAN.

The system shall demonstrate:

- System engineering
- CAN network design
- Distributed software development
- Diagnostics
- Fault handling
- Verification and validation

---

## System Selection

Choose **one** of the following systems or propose your own with instructor approval:

### Option 1: Smart Greenhouse

Functions:

- Temperature monitoring
- Humidity monitoring
- Water pump control
- Fan control

---

### Option 2: Smart Home Automation

Functions:

- Lighting control
- Door monitoring
- Security alarm
- Energy monitoring

---

### Option 3: Vehicle Body Control System

Functions:

- Door status monitoring
- Interior light control
- Window operation
- Vehicle diagnostics

---

### Option 4: Elevator Control System

Functions:

- Floor request handling
- Door control
- Motor control
- Fault monitoring

---

### Option 5: EV Battery Monitoring System

Functions:

- Cell voltage monitoring
- Temperature monitoring
- Charge/discharge monitoring
- Fault detection

---

### Option 6: Student-Proposed System

The system must be approved before implementation.

---

## System Design Requirements

### Task 1: Functional Analysis

Identify:

- System objectives
- Inputs
- Outputs
- Sensors
- Actuators
- User interactions

**Deliverable:** Functional block diagram.

---

### Task 2: ECU Identification

Divide the system into independent ECUs.

Examples:

```text
Sensor ECU
Controller ECU
Actuator ECU
Diagnostic ECU
```

Minimum requirements:

- 4 ECUs
- Each ECU implemented as a separate application
- Each ECU must have a clearly defined responsibility

**Deliverable:** ECU architecture diagram.

---

### Task 3: CAN Network Design

Design the communication network.

Define:

- Message IDs
- Message ownership
- Receivers
- Transmission rates
- Signal definitions

Minimum requirements:

- 8 unique CAN messages
- At least 2 periodic messages
- At least 2 event-triggered messages

**Deliverable:** CAN matrix.

Example:

| CAN ID | Transmitter | Receiver | Signal |
|----------|----------|----------|----------|
| 0x100 | Sensor ECU | Controller ECU | Temperature |
| 0x101 | Sensor ECU | Controller ECU | Humidity |
| 0x200 | Controller ECU | Actuator ECU | Fan Command |

---

### Task 4: Message Definition

Define encoding rules for all signals.

Examples:

- Temperature
- Speed
- Voltage
- Door Status

Document:

- Units
- Resolution
- Range
- Encoding strategy

**Deliverable:** Signal specification document.

---

## Implementation Requirements

### Task 5: ECU Development

Implement all ECUs as independent software applications.

Examples:

```text
sensor_ecu
controller_ecu
actuator_ecu
diagnostic_ecu
```

Each ECU shall:

- Send CAN messages
- Receive CAN messages
- Process received information
- Perform its assigned function

---

### Task 6: Human Machine Interface (HMI)

Develop a simple interface that allows observation of system behavior.

Possible implementations:

- Console-based dashboard
- GUI application
- Web-based dashboard

The interface should display:

- Current system status
- Sensor values
- Actuator states
- Fault information

---

## Verification Challenges

### Challenge 1: Normal Operation

Demonstrate correct system behavior.

Verify:

- All ECUs communicate successfully
- System objectives are achieved
- All expected messages are transmitted and received

---

### Challenge 2: Missing Message Detection

Simulate loss of communication from one ECU.

Verify:

- The system detects missing messages
- Appropriate warnings are generated

Document the detection mechanism.

---

### Challenge 3: Sensor Fault

Introduce an invalid sensor value.

Examples:

```text
Temperature = 200°C
Humidity = -10%
Battery Voltage = 100V
```

Verify:

- Fault detection
- Fault reporting
- Safe system behavior

---

### Challenge 4: Node Failure

Stop one ECU unexpectedly.

Verify:

- Remaining ECUs continue operating
- Diagnostic information is generated
- System response is documented

---

### Challenge 5: Recovery

Restore the failed ECU.

Verify:

- Communication resumes
- System returns to normal operation

---

## Diagnostics Requirements

Develop a dedicated Diagnostic ECU.

Responsibilities:

- Monitor network traffic
- Detect missing messages
- Detect abnormal values
- Generate fault reports
- Maintain event logs

Example output:

```text
WARNING:
Sensor ECU Timeout

Timestamp:
15:34:25

Action:
Controller switched to safe mode
```

---

## Testing Requirements

Create test cases covering:

### Functional Tests

- Message transmission
- Message reception
- Data processing
- User interface behavior

### Fault Tests

- Missing message
- Invalid value
- ECU shutdown
- ECU recovery

### Integration Tests

- End-to-end system validation
- Multi-ECU interaction

---

## System Architecture Example

```text
+------------+
| Sensor ECU |
+------------+
       |
       |
       v
   SocketCAN
       |
+-------------+
| Controller  |
|     ECU     |
+-------------+
       |
       |
       v
+-------------+
| Actuator    |
|    ECU      |
+-------------+
       |
       |
       v
+-------------+
| Diagnostic  |
|    ECU      |
+-------------+
```

---

## Deliverables

### 1. System Design Report

Maximum 10 pages.

Include:

- Problem description
- Functional architecture
- ECU architecture
- CAN matrix
- Signal definitions
- Design decisions

---

### 2. Source Code

All software modules.

Include:

- Build instructions
- Execution instructions
- Project structure

---

### 3. Test Report

Include:

- Test cases
- Screenshots
- Results
- Observations
- Conclusions

---

### 4. Demonstration

Teams shall demonstrate:

1. Distributed system operation
2. CAN traffic between ECUs
3. Diagnostic functionality
4. Fault injection
5. Fault recovery
6. End-to-end system behavior

---

## Expected Learning Outcomes

After completing this assignment, students should be able to:

- Apply SocketCAN in a realistic engineering project.
- Design a distributed CAN-based system.
- Develop multiple communicating ECUs.
- Create and use a CAN matrix.
- Implement diagnostics and fault handling.
- Perform system-level testing and validation.
- Think like a systems engineer rather than an individual software developer.
