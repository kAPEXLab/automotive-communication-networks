# Ethernet Communication Investigation and Traffic Analysis using Linux

## Industry Context

Ethernet has become the backbone of modern communication networks and is increasingly adopted in automotive systems for high-bandwidth communication between sensors, controllers, infotainment systems, data loggers, and central computing platforms.

Before developing Ethernet-based applications, engineers must understand network architecture, packet flow, addressing mechanisms, and communication protocols used in Ethernet networks.

---

## Problem Statement

As a Network Engineer, you have been assigned the task of investigating Ethernet communication on a Linux platform.

Your objective is to study, analyse, and document how Ethernet communication occurs between networked systems and how different protocols interact within the TCP/IP stack.

The project should focus on understanding network fundamentals through practical experimentation and traffic analysis using Linux networking tools and Wireshark.

This is an individual project. Each student shall maintain all project artifacts in their own GitHub repository.

---

## Objectives

The student shall:

- Understand Ethernet architecture and frame structure
- Study MAC and IP addressing mechanisms
- Investigate TCP/IP communication
- Analyse network traffic using Wireshark
- Perform network diagnostics using Linux networking tools
- Document findings and observations

---

## Investigation Activities

Study and document the following:

1. Ethernet Frame Structure
2. MAC Addressing
3. IPv4 Addressing
4. TCP Protocol
5. UDP Protocol
6. ARP Protocol
7. ICMP Protocol
8. Client-Server Communication
9. Role of Network Interfaces

---

## Practical Activities

Using Linux networking tools, investigate and document the purpose and outputs of:

```bash
ip addr
ip route
arp -a
ping
ss
netstat
hostname -I
```

---

## Traffic Analysis Activities

Using Wireshark, capture and analyse network traffic.

### Activity 1: ARP Analysis

Investigate:

- ARP Request
- ARP Reply
- MAC Address Resolution Process

### Activity 2: ICMP Analysis

Generate ICMP traffic using ping and analyse:

- Echo Request
- Echo Reply
- Source IP Address
- Destination IP Address

### Activity 3: TCP Analysis

Capture TCP traffic and identify:

- Three-Way Handshake
- Source Port
- Destination Port
- Data Transfer

### Activity 4: UDP Analysis

Capture UDP traffic and identify:

- Source Port
- Destination Port
- Payload Transfer
- Connectionless Communication

---

## Analysis Tasks

Prepare a report explaining:

1. Difference between MAC and IP addresses
2. Difference between TCP and UDP protocols
3. Purpose of ARP protocol
4. Packet flow during a ping operation
5. Ethernet frame structure
6. Encapsulation and de-encapsulation process in TCP/IP communication

---

## Validation Activities

Demonstrate and verify:

- Successful network connectivity
- Successful packet capture
- Identification of ARP, ICMP, TCP, and UDP packets
- Correct interpretation of packet fields

---

## Deliverables

The GitHub repository shall contain:

```text
README.md
docs/
├── Study_Report.pdf
├── Analysis_Report.pdf

screenshots/
├── arp_analysis.png
├── icmp_analysis.png
├── tcp_analysis.png
├── udp_analysis.png

wireshark/
├── arp_capture.pcapng
├── icmp_capture.pcapng
├── tcp_capture.pcapng
└── udp_capture.pcapng
```

---

## Expected Learning Outcomes

Upon successful completion, the student should be able to:

- Explain Ethernet communication fundamentals
- Interpret Ethernet frames
- Understand MAC and IP addressing
- Analyse common network protocols
- Use Wireshark effectively
- Troubleshoot basic networking issues on Linux
