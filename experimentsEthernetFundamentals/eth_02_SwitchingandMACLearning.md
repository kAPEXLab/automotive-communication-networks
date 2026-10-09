# Exercise 2 - Switching and MAC Learning

## Exercise Objective

Understand how Ethernet devices identify each other and how Ethernet switches forward traffic within a local network.

After completing this exercise, the following concepts should be clear:

- MAC address identification
- ARP operation
- ARP table entries
- Layer 2 forwarding
- MAC learning
- Switch forwarding table
- Broadcast traffic
- Unicast traffic
- Unknown unicast behaviour

---

## Experiments

1. Experiment 2.1 - MAC Address Discovery
2. Experiment 2.2 - ARP Communication
3. Experiment 2.3 - Switch Learning Behaviour
4. Experiment 2.4 - Broadcast vs Unicast Traffic

---

# Experiment 2.1 - MAC Address Discovery

## Objective

Identify and analyse Ethernet MAC addresses of network interfaces.

---

## Concepts Covered

- MAC Address
- Ethernet Interface
- Layer 2 Addressing
- Unicast Address
- Vendor OUI

---

## Theory

Every Ethernet interface is assigned a unique MAC address.

Example:

```text
00:1A:2B:3C:4D:5E
```

A MAC address is:

- 48 bits long
- Represented using hexadecimal digits
- Used for Layer 2 communication

Unlike an IP address, a MAC address is used only inside a local Ethernet network.

---

## Hardware Setup

```text
PC 1 ---- Ethernet Switch ---- PC 2
```

---

## Procedure

### Windows

Execute:

```cmd
getmac /v
```

or

```cmd
ipconfig /all
```

---

### Ubuntu/Linux

Execute:

```bash
ip link show
```

---

## Expected Observation

Example:

```text
Ethernet Adapter

MAC Address:
3C-52-82-12-34-56
```

---

## Practical Activity

Record the Ethernet MAC addresses.

| Device | MAC Address |
|----------|----------|
| PC 1 | |
| PC 2 | |

---

## Key Learning

- MAC addresses uniquely identify Ethernet interfaces.
- MAC addressing is used at Layer 2.
- MAC addresses are different from IP addresses.

---

## Review Questions

1. What is the length of a MAC address?
2. Why is MAC addressing required?
3. What is the difference between MAC and IP addresses?

---

## Conclusion

Ethernet interfaces were identified using their unique MAC addresses.

---

# Experiment 2.2 - ARP Communication

## Objective

Observe how an IP address is resolved to a MAC address using ARP.

---

## Concepts Covered

- ARP
- ARP Request
- ARP Reply
- ARP Cache
- IP-to-MAC Resolution

---

## Theory

Ethernet communication requires MAC addresses.

When an application wants to communicate using IP:

```text
Application
    ↓
IPv4 Address
    ↓
ARP Resolution
    ↓
MAC Address
```

ARP is used to discover the MAC address associated with an IP address.

---

## Hardware Setup

```text
PC 1 ---- Ethernet Switch ---- PC 2
```

IP Configuration:

```text
PC 1 : 192.168.10.10

PC 2 : 192.168.10.20
```

---

## Procedure

### Step 1

Clear ARP cache.

Windows:

```cmd
arp -d *
```

Linux:

```bash
sudo ip neigh flush all
```

---

### Step 2

Start Wireshark capture.

---

### Step 3

Execute:

```bash
ping 192.168.10.20
```

---

### Step 4

Apply Wireshark filter:

```text
arp
```

---

## Expected Observation

ARP Request:

```text
Who has 192.168.10.20?
Tell 192.168.10.10
```

ARP Reply:

```text
192.168.10.20 is at
XX:XX:XX:XX:XX:XX
```

---

## Practical Activity

Record observations.

| Parameter | Value |
|------------|------------|
| Source IP | |
| Destination IP | |
| Target MAC before ARP | |
| MAC learned | |

---

## Key Learning

- ARP converts IP addresses into MAC addresses.
- ARP Request uses broadcast communication.
- ARP Reply uses unicast communication.

---

## Review Questions

1. Why is ARP necessary?
2. Is ARP a Layer 2 or Layer 3 protocol?
3. What happens if ARP fails?

---

## Conclusion

ARP communication was successfully captured and analysed.

---

# Experiment 2.3 - Switch Learning Behaviour

## Objective

Observe how Ethernet switches learn MAC addresses and forward traffic.

---

## Concepts Covered

- Switch Learning
- MAC Address Table
- Flooding
- Unicast Forwarding

---

## Theory

An Ethernet switch builds a MAC Address Table automatically.

Example:

```text
MAC Address          Port

00:11:22:33:44:55   Port 1

AA:BB:CC:DD:EE:FF   Port 2
```

Initially the table is empty.

Unknown traffic is flooded.

Once learned, traffic is forwarded only to the correct port.

---

## Hardware Setup

```text
PC 1 ----+
          |
          |---- Ethernet Switch
          |
PC 2 ----+
```

---

## Procedure

### Step 1

Power cycle the switch or clear the MAC table if supported.

### Step 2

Start packet exchange using:

```bash
ping 192.168.10.20
```

### Step 3

Observe switch MAC table.

Managed switch:

```text
MAC Address Table
```

---

## Expected Observation

Initially:

```text
Empty Table
```

After communication:

```text
PC1_MAC -> Port 1

PC2_MAC -> Port 2
```

---

## Practical Activity

Record entries.

| MAC Address | Learned Port |
|-------------|-------------|
| | |
| | |

---

## Key Learning

- Switches learn MAC addresses automatically.
- Traffic flooding occurs when destination MAC is unknown.
- Learned MAC addresses improve forwarding efficiency.

---

## Review Questions

1. What is MAC learning?
2. Why does flooding occur?
3. What happens when a switch receives an unknown frame?

---

## Conclusion

Switch MAC learning behaviour was successfully observed.

---

# Experiment 2.4 - Broadcast vs Unicast Traffic

## Objective

Differentiate between broadcast traffic and unicast traffic.

---

## Concepts Covered

- Broadcast
- Unicast
- Broadcast MAC Address
- Ethernet Forwarding
- Layer 2 Communication

---

## Theory

Broadcast traffic is sent to all devices.

Broadcast MAC:

```text
FF:FF:FF:FF:FF:FF
```

Examples:

- ARP Request
- DHCP Discover

Unicast traffic is sent to a specific destination.

Example:

```text
PC 1 → PC 2
```

---

## Hardware Setup

```text
PC 1
   \
    \
     Ethernet Switch
    /
   /
PC 2
```

---

## Procedure

### Step 1

Start Wireshark capture.

### Step 2

Clear ARP cache.

Windows:

```cmd
arp -d *
```

Linux:

```bash
sudo ip neigh flush all
```

### Step 3

Execute:

```bash
ping 192.168.10.20
```

### Step 4

Observe ARP packets.

### Step 5

Observe ICMP packets.

---

## Expected Observation

ARP Request:

```text
Destination MAC

FF:FF:FF:FF:FF:FF
```

Broadcast.

---

ICMP Packet:

```text
Destination MAC

XX:XX:XX:XX:XX:XX
```

Unicast.

---

## Practical Activity

Record observations.

| Traffic Type | Destination MAC |
|--------------|----------------|
| ARP Request | |
| ARP Reply | |
| ICMP Request | |
| ICMP Reply | |

---

## Key Learning

- Broadcast reaches all devices.
- Unicast reaches only the intended device.
- ARP Request is broadcast.
- ARP Reply is unicast.
- Most regular Ethernet communication is unicast.

---

## Review Questions

1. What is a broadcast MAC address?
2. Why is ARP Request broadcast?
3. Why is ARP Reply unicast?
4. Which traffic type generates more network load?

---

## Conclusion

Broadcast and unicast Ethernet traffic were captured and analysed successfully.

---

# Exercise Summary

The following communication sequence should now be understood:

```text
Application
      ↓
IP Address
      ↓
ARP Request (Broadcast)
      ↓
ARP Reply (Unicast)
      ↓
MAC Address Learned
      ↓
Switch Learns MAC Address
      ↓
Frame Forwarding
      ↓
Unicast Communication
```

The concepts learned in this exercise form the foundation for:

- Ethernet switching
- VLANs
- Network segmentation
- Automotive Ethernet communication
