# Development of an Ethernet-Based Vehicle Monitoring System using Linux Socket Programming

## Industry Context

Modern embedded and automotive systems rely on Ethernet-based communication for information exchange between software components running on different Electronic Control Units (ECUs).

Engineers must understand how applications exchange information over a network using socket programming before working with higher-level automotive protocols.

This project focuses on implementing Ethernet communication using Linux socket programming and analysing the generated network traffic.

---

## Problem Statement

A Vehicle ECU application shall periodically generate vehicle information and transmit it over an Ethernet network.

A Dashboard ECU application shall receive the transmitted information and display it to the user.

Develop a complete Vehicle Monitoring System using Linux Socket Programming.

The system shall support both TCP and UDP communication mechanisms.

Both applications shall execute on the same Ubuntu machine in separate terminals.

This is an individual project. Each student shall maintain all project artifacts in their own GitHub repository.

---

## Objectives

The student shall:

- Understand socket programming concepts
- Implement TCP-based communication
- Implement UDP-based communication
- Analyse communication using Wireshark
- Compare TCP and UDP behaviour
- Validate communication reliability
- Document implementation and findings

---

## System Description

Design and implement the following software components:

### Vehicle ECU

The Vehicle ECU shall periodically generate and transmit vehicle data.

### Dashboard ECU

The Dashboard ECU shall receive, process, and display vehicle data.

---

## Vehicle Data

The system shall transmit at least the following information:

| Signal | Description |
|----------|----------|
| Vehicle Speed | Speed of the vehicle |
| Engine RPM | Engine rotational speed |
| Steering Angle | Steering wheel angle |

Students may extend the signal list with additional parameters if desired.

---

## Design Activities

Create and document:

### System Architecture

```text
Vehicle ECU
    |
 Ethernet
    |
Dashboard ECU
```

### Communication Flow Diagram

Show:

- Sender
- Receiver
- Socket Communication
- Data Flow

---

## Implementation Activities

### Part A - UDP Communication

Implement:

#### Vehicle ECU

- Generate vehicle data every second
- Transmit data using UDP sockets

#### Dashboard ECU

- Receive UDP packets
- Display received information

---

### Part B - TCP Communication

Implement the same system using TCP sockets.

The solution should support:

- Connection establishment
- Data transfer
- Connection termination

---

## Traffic Analysis Activities

Using Wireshark, analyse the generated communication.

### UDP Analysis

Identify:

- Source IP Address
- Destination IP Address
- Source Port
- Destination Port
- Payload Data

### TCP Analysis

Identify:

- SYN Packet
- SYN-ACK Packet
- ACK Packet
- Data Transfer Packets
- Connection Termination Sequence

---

## Validation Activities

Demonstrate and document:

### Scenario 1

Normal communication between Vehicle ECU and Dashboard ECU.

### Scenario 2

Dashboard ECU is not running.

### Scenario 3

Continuous transmission for an extended duration.

### Scenario 4

Invalid IP address configuration.

### Scenario 5

Comparison between TCP and UDP implementations.

---

## Comparative Study

Prepare a comparison between TCP and UDP based on:

| Parameter |
|------------|
| Reliability |
| Speed |
| Overhead |
| Error Handling |
| Connection Establishment |
| Typical Usage Scenarios |

---

## Deliverables

The GitHub repository shall contain:

```text
README.md

docs/
├── Design_Document.pdf
├── Validation_Report.pdf
├── TCP_UDP_Comparison.pdf

src/
├── vehicle_ecu.c
├── dashboard_ecu.c
├── Makefile

screenshots/

wireshark/
├── tcp_capture.pcapng
└── udp_capture.pcapng
```

---

## Expected Learning Outcomes

Upon successful completion, the student should be able to:

- Develop socket-based applications on Linux
- Implement TCP communication
- Implement UDP communication
- Analyse network traffic using Wireshark
- Understand Ethernet communication at the application layer
- Validate and document networked software systems
