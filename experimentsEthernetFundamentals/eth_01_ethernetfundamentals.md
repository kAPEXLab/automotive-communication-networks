# 📘 Exercise 1 - Ethernet Fundamentals

## 1. 🎯 Lab Objective

This exercise introduces the basic operation of an Ethernet network. It explains how two computers are connected through an Ethernet switch and how communication proceeds from physical link establishment to frame-level transmission.

By the end of this exercise, the reader should be able to explain:

- how a physical Ethernet link is established
- how link status, speed, and duplex are identified
- how IPv4 addresses are configured
- how end-to-end connectivity is tested with Ping
- how Ethernet, IPv4, and ICMP work together
- how Ethernet frames contain source and destination MAC addresses
- how Wireshark is used to inspect traffic

---

## 2. 🔖 Learning Outcomes

After completing this exercise, the reader will be able to:

- verify that an Ethernet interface is connected and active
- check network link speed and duplex mode
- assign IPv4 addresses and subnet masks
- confirm connectivity using Ping
- explain the role of MAC addresses and frame headers
- use Wireshark to inspect Ethernet packets

---

## 3. Required Hardware and Software

### Hardware

- Two computers with Ethernet adapters
- One unmanaged Ethernet switch
- Two Cat5e or Cat6 Ethernet cables
- Power supply for the switch

### Software

- Windows 10/11 or Ubuntu/Linux
- PowerShell / Command Prompt / Linux terminal
- Wireshark

---

## 4. 📡 Network Topology

```text
+------+       +-----------------+       +------+
| PC 1 |-------| Ethernet Switch |-------| PC 2 |
+------+       +-----------------+       +------+
```

### Cable Connections

```text
PC 1 Ethernet port ---- Switch Port 1
PC 2 Ethernet port ---- Switch Port 2
```

---

## 5. ⚠️ Lab Safety and Instructions

1. Use the same Ethernet interfaces throughout the lab.
2. Temporarily disable Wi-Fi so the test traffic uses the wired Ethernet link.
3. Do not connect the lab switch to the organisation network or Internet.
4. Use only the IP addresses assigned by the instructor.
5. Record the actual values observed on the lab machines.
6. Restore the original network configuration after completing the exercise.

---

## 6. 📌 Experiment 1.1 - Physical Link Establishment

### Objective

Establish and verify the physical Ethernet link between two computers connected through a switch.

### Background

Ethernet communication begins with the physical layer. A link is considered active only after the NIC and switch detect each other and agree on common communication parameters.

The process typically includes the following steps:

1. detection of the link partner
2. auto-negotiation
3. selection of a common speed and duplex mode
4. indication of an active link

IP configuration is not required for the physical link to exist.

### Procedure

#### Step 1 - Check the disconnected state

Keep both Ethernet cables disconnected and inspect the interface status.

##### Windows

```powershell
Get-NetAdapter
```

or open:

```text
Control Panel > Network and Internet > Network Connections
```

##### Ubuntu/Linux

```bash
ip link show
```

Note the Ethernet interface name. Typical values include:

```text
eth0
enp2s0
enp3s0
eno1
```

---

#### Step 2 - Connect PC 1

Connect the Ethernet cable from PC 1 to an available switch port.

Wait a few seconds and observe:

- link LEDs on PC 1
- link LEDs on the switch port
- interface status in the operating system

---

#### Step 3 - Connect PC 2

Connect the Ethernet cable from PC 2 to another switch port.

Observe the same indicators again.

---

#### Step 4 - Verify the link status

##### Windows

```powershell
Get-NetAdapter
```

Example output:

```text
Name      Status   LinkSpeed
Ethernet  Up       1 Gbps
```

Additional details:

```powershell
Get-NetAdapter | Format-List Name, Status, LinkSpeed, MacAddress
```

##### Ubuntu/Linux

```bash
ip link show <interface>
```

Example:

```bash
ip link show enp2s0
```

Look for:

```text
state UP
```

If available, also check:

```bash
sudo ethtool <interface>
```

Look for:

```text
Speed: 1000Mb/s
Duplex: Full
Link detected: yes
```

---

#### Step 5 - Test link interruption

Disconnect the cable from PC 1 and observe the changes in:

- link indicators
- switch port status
- interface state in the OS

Reconnect the cable and confirm the link returns.

Repeat the procedure for PC 2.

### Expected Observation

The expected observations are as follows:

- the NIC shows an active link when connected
- the switch port indicates connectivity
- the OS reports the Ethernet interface as Up
- link speed and duplex appear valid
- removing the cable causes the link to disappear and reappear when reconnected

### Observation Table

| Device | Cable Status | Link State | Link Speed | Duplex |
|--------|--------------|------------|------------|--------|
| PC 1 | Connected | | | |
| PC 1 | Disconnected | | | |
| PC 2 | Connected | | | |
| PC 2 | Disconnected | | | |

### Troubleshooting

- If the interface remains Down, check the cable and switch port.
- If the speed is incorrect, inspect the adapter, cable, and switch capabilities.
- If there is no link LED, confirm the port is active and the device is powered on.

### Review Questions

1. Why does Ethernet require a physical link before IP communication can occur?
2. What is auto-negotiation?
3. Why do both speed and duplex matter in Ethernet communication?

---

## 7. 📌 Experiment 1.2 - IP Configuration

### Objective

Assign valid IPv4 addresses and subnet masks to both computers and prepare the network for communication.

### Background

A valid Ethernet link allows frames to be exchanged at Layer 2, but devices still need valid Layer 3 addressing to communicate using IP.

### Procedure

#### Windows

```powershell
New-NetIPAddress -InterfaceAlias "Ethernet" -IPAddress 192.168.10.10 -PrefixLength 24
```

#### Ubuntu/Linux

```bash
sudo ip addr add 192.168.10.10/24 dev <interface>
```

Repeat the same configuration on the second PC with the second IP address.

### Example IP Plan

```text
PC 1: 192.168.10.10 /24
PC 2: 192.168.10.20 /24
```

### Verification

Check the configured IP:

```bash
ip addr show
```

or on Windows:

```powershell
ipconfig
```

### Expected Observation

Each computer should have a valid IPv4 address in the same subnet and should be able to communicate at the IP layer.

### Review Questions

1. Why must both devices be in the same subnet?
2. What is the purpose of the subnet mask?
3. What happens if the IP configuration is incorrect?

---

## 8. 📌 Experiment 1.3 - Ping Communication

### Objective

Verify end-to-end connectivity between the two computers using Ping.

### Background

Ping uses ICMP echo requests and echo replies. It confirms that the physical path, IP configuration, and protocol stack are working correctly.

### Procedure

From PC 1, ping PC 2:

```bash
ping 192.168.10.20
```

or on Windows:

```powershell
ping 192.168.10.20
```

### Expected Observation

Successful response similar to:

```text
Reply from 192.168.10.20: bytes=32 time<1ms TTL=128
```

### Observation Table

| Test | Source | Destination | Result |
|------|--------|-------------|--------|
| 1 | PC 1 | PC 2 | |
| 2 | PC 2 | PC 1 | |

### Review Questions

1. What does Ping test at the network layer?
2. Why is Ping useful for troubleshooting?
3. What is the relationship between Ethernet, IPv4, and ICMP?

---

## 9. 📌 Experiment 1.4 - Ethernet Frame Analysis

### Objective

Capture and inspect Ethernet frames to understand encapsulation and address fields.

### Background

Once Ping is active, the traffic is transmitted as Ethernet frames. Each Ethernet frame includes:

- destination MAC address
- source MAC address
- EtherType
- payload
- frame check sequence

### Procedure

1. Start Wireshark.
2. Select the correct Ethernet interface.
3. Generate traffic with Ping.
4. Apply a capture filter or inspect packets in real time.
5. Find an ICMP packet and inspect its Ethernet header.

### Wireshark Filters

```text
icmp
arp
eth.addr == xx:xx:xx:xx:xx:xx
```

### What to Observe

- destination MAC address
- source MAC address
- EtherType value
- frame length
- ICMP payload

### Observation Table

| Field | Value |
|-------|-------|
| Source MAC | |
| Destination MAC | |
| EtherType | |
| Frame Length | |
| Protocol | |

### Review Questions

1. What is the purpose of the destination MAC address?
2. Why are source and destination MAC addresses required in Ethernet?
3. How does Ethernet differ from IPv4?

---

## 10. Conclusion

This exercise demonstrates the full flow of Ethernet communication: physical link detection, IP configuration, connectivity testing, and packet inspection. It shows how a simple Ethernet link supports higher-layer communication and how packet headers are used to deliver data correctly.

---

## 11. Final Reflection

Write a short summary describing:

- how the physical link was established
- how the IP configuration was verified
- what Ping proved
- what you observed in the Wireshark capture

Use your own recorded values, not copied examples.
