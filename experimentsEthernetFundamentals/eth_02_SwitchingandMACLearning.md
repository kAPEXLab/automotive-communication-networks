# 📘 Exercise 2 - Switching and MAC Learning

## 1. 🎯 Lab Objective

This exercise explains how Ethernet devices identify each other and how Ethernet switches learn and forward traffic within a local network.

The lab focuses on the relationship between the following concepts:

- MAC addresses
- ARP
- Layer 2 switching
- broadcast and unicast traffic

---

## 2. 🔖 Learning Outcomes

After completing this exercise, the reader will be able to:

- identify the MAC address of an interface
- explain how ARP resolves an IP address to a MAC address
- describe how a switch learns MAC addresses
- distinguish between broadcast and unicast traffic
- explain why a switch forwards traffic selectively

---

## 3. 📡 Equipment and Topology

### Hardware

- Two PCs with Ethernet interfaces
- One Ethernet switch
- Appropriate Ethernet cables

### Topology

```text
PC 1 ---- Ethernet Switch ---- PC 2
```

### IP Plan

```text
PC 1: 192.168.10.10 /24
PC 2: 192.168.10.20 /24
```

---

## 4. Lab Instructions

1. Use the same wired Ethernet links for all experiments in this exercise.
2. Disable Wi-Fi to ensure traffic is only sent on the Ethernet interface.
3. Start Wireshark before performing the ARP test.
4. Record actual values observed on the lab system.

---

# 📌 Experiment 2.1 - MAC Address Discovery

## Objective

Identify and analyse the MAC addresses assigned to Ethernet interfaces.

## Background

Each Ethernet interface has a unique Layer 2 address called the MAC address. The MAC address is used inside the local Ethernet network to identify the sender and receiver.

### Relevant Facts

- MAC address length: 48 bits
- format: hexadecimal
- used for communication within the local Ethernet segment
- different from an IP address

## Procedure

### Windows

```cmd
getmac /v
```

or

```cmd
ipconfig /all
```

### Ubuntu/Linux

```bash
ip link show
```

## Expected Observation

The output should show a unique MAC address for each network interface, for example:

```text
3C-52-82-12-34-56
```

## Observation Table

| Device | MAC Address |
|--------|-------------|
| PC 1 | |
| PC 2 | |

## Review Questions

1. What is the purpose of a MAC address?
2. Why is a MAC address different from an IP address?
3. What is the length of a MAC address?

---

# 📌 Experiment 2.2 - ARP Communication

## Objective

Observe how an IP address is resolved to a MAC address using ARP.

## Background

An Ethernet frame must contain a source and destination MAC address. A switch forwards frames using the destination MAC address and its learned MAC table; it does not inspect the IP address for forwarding decisions. When a host knows an IP address but not the MAC address, it uses ARP to discover the correct mapping.

### ARP Flow

```text
Application
   ↓
IPv4 address
   ↓
ARP resolution
   ↓
MAC address
```

## Procedure

### Step 1 - Clear the ARP cache

#### Windows

```cmd
arp -d *
```

#### Linux

```bash
sudo ip neigh flush all
```

### Step 2 - Start Wireshark

Capture all Ethernet traffic.

### Step 3 - Ping the other host

```bash
ping 192.168.10.20
```

### Step 4 - Apply an ARP filter

```text
arp
```

## Expected Observation

The key observations are as follows:

- an ARP request is sent as a broadcast at Layer 2 to FF:FF:FF:FF:FF:FF
- an ARP reply is sent from the destination host
- the destination MAC address becomes known to the sender for subsequent unicast traffic

Example:

```text
Who has 192.168.10.20?
Tell 192.168.10.10
```

and

```text
192.168.10.20 is at XX:XX:XX:XX:XX:XX
```

## Observation Table

| Field | Value |
|-------|-------|
| Source IP | |
| Destination IP | |
| Target MAC before ARP | |
| MAC learned | |

## Review Questions

1. Why is ARP necessary?
2. Is ARP considered a Layer 2 protocol or a Layer 2/3 boundary protocol?
3. What happens if ARP fails?

---

# Experiment 2.3 - Switch Learning Behaviour

## Objective

Observe how an Ethernet switch learns MAC addresses and maintains a forwarding table.

## Background

When a frame arrives at a switch, the switch inspects the source MAC address and records which port it was learned on. This is called MAC learning.

## Procedure

1. Start a packet capture on the switch-connected interface if supported.
2. Generate traffic from PC 1 to PC 2.
3. Observe the switch behavior for the first and subsequent frames.
4. Repeat the test with traffic in both directions.

## Expected Observation

The switch learns the MAC addresses of connected devices and forwards frames based on the destination MAC address.

## Review Questions

1. What does a switch learn from incoming frames?
2. Why does a switch not forward every frame everywhere?
3. What is a forwarding table?

---

# Experiment 2.4 - Broadcast vs Unicast Traffic

## Objective

Differentiate between broadcast and unicast communication in Ethernet.

## Background

- Unicast traffic is sent to one specific receiver.
- Broadcast traffic is sent to all devices on the local network.
- ARP requests are typically broadcast messages.

## Procedure

1. Use Wireshark to inspect traffic while pinging another host.
2. Identify broadcast frames and unicast frames.
3. Note the difference in destination MAC addresses.

## Expected Observation

- an ARP request uses a broadcast destination MAC address
- a normal ping reply uses a unicast destination MAC address

## Observation Table

| Traffic Type | Destination MAC Address | Purpose |
|--------------|-------------------------|---------|
| Broadcast | | |
| Unicast | | |

## Review Questions

1. Why are ARP requests broadcast?
2. What is the difference between broadcast and unicast traffic?
3. Why is broadcast use limited in Ethernet networks?

---

## Conclusion

This exercise shows that Ethernet switching depends on MAC learning and address resolution. ARP allows devices to find each other, while switching ensures traffic is forwarded efficiently rather than blindly to every port.

