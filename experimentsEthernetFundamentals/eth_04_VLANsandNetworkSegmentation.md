# 📘 Exercise 4 - VLANs and Network Segmentation

## 1. 🎯 Lab Objective

This exercise introduces VLANs and logical segmentation in Ethernet networks. It explains how a single physical switch can carry multiple logical networks and how VLAN tagging separates traffic.

---

## 2. 🔖 Learning Outcomes

After completing this exercise, the reader will be able to:

- explain what a VLAN is and why it is used
- assign ports to different VLANs
- verify inter-VLAN isolation
- interpret IEEE 802.1Q VLAN tags in captured frames
- understand how PCP supports QoS in Ethernet traffic

---

## 3. Required Hardware

- Managed Ethernet switch
- PC 1, PC 2, PC 3
- Ethernet cables
- Wireshark

### Topology

```text
PC 1 ---- Port 1 (VLAN 10)
PC 2 ---- Port 2 (VLAN 20)
PC 3 ---- Port 3 (VLAN 10)
```

---

## 4. Lab Instructions

1. Use a managed switch that supports VLAN configuration.
2. Confirm the devices are connected to the correct ports before testing.
3. Use separate IP ranges for different VLANs.
4. Save the final switch configuration if required by the instructor.

---

# 📌 Experiment 4.1 - VLAN Creation

## Objective

Create multiple VLANs on a managed Ethernet switch and confirm logical network separation.

## Background

A VLAN allows multiple logical networks to coexist on a single physical switch. This reduces broadcast domains and improves traffic control without requiring extra physical hardware.

## Example Configuration

```text
Port 1 -> VLAN 10
Port 2 -> VLAN 20
Port 3 -> VLAN 10
```

## IP Plan

```text
PC 1: 192.168.10.10 /24
PC 2: 192.168.20.10 /24
PC 3: 192.168.10.20 /24
```

## Procedure

1. Open the switch management interface.
2. Create VLAN 10 and VLAN 20.
3. Assign switch ports to the VLANs as required.
4. Configure ports as access ports.
5. Verify the configuration in the switch GUI or CLI.

## Observation Table

| Device | Switch Port | VLAN |
|--------|--------------|------|
| PC 1 | | |
| PC 2 | | |
| PC 3 | | |

## Review Questions

1. What is a VLAN?
2. Why are VLANs used in Ethernet networks?
3. Does a VLAN require separate physical switches?

---

# 📌 Experiment 4.2 - Inter-VLAN Isolation

## Objective

Verify that devices in the same VLAN can communicate while devices in different VLANs cannot communicate directly.

## Background

Each VLAN behaves as an independent Layer 2 domain. Devices in the same VLAN can communicate directly at Layer 2, while communication between VLANs requires a router, Layer 3 switch, or other Layer 3 forwarding device.

## Procedure

### Test 1 - Same VLAN communication

From PC 1:

```bash
ping 192.168.10.20
```

Expected result:

```text
Success
```

### Test 2 - Different VLAN communication

From PC 1:

```bash
ping 192.168.20.10
```

Expected result:

```text
Timeout
```

## Expected Observation

```text
PC 1 <-> PC 3: communication allowed
PC 1 <-> PC 2: communication blocked
```

## Observation Table

| Source | Destination | Result |
|--------|-------------|--------|
| PC 1 | PC 3 | |
| PC 1 | PC 2 | |
| PC 2 | PC 3 | |

## Review Questions

1. Why can PC 1 communicate with PC 3?
2. Why can PC 1 not communicate with PC 2 directly?
3. What is required for communication between VLANs?

---

# 📌 Experiment 4.3 - VLAN Tag Observation

## Objective

Capture and inspect IEEE 802.1Q VLAN-tagged Ethernet frames.

## Background

A VLAN tag is added to an Ethernet frame so that switches can identify which VLAN the frame belongs to. The tag is carried inside the Ethernet frame using IEEE 802.1Q.

## Relevant Fields

The VLAN tag contains the following fields:

- PCP (Priority Code Point)
- DEI
- VID (VLAN ID)

## Procedure

1. Start Wireshark.
2. Generate traffic in VLAN 10.
3. Apply the filter:

```text
vlan
```

4. Inspect the packet details and note the VLAN ID and priority values.

## Expected Observation

Example:

```text
802.1Q Virtual LAN
Priority: 0
VLAN ID: 10
```

## Observation Table

| Parameter | Value |
|-----------|-------|
| VLAN ID | |
| EtherType | |
| Frame Length | |
| PCP Value | |

## Review Questions

1. What is IEEE 802.1Q?
2. What does the VLAN ID represent?
3. Why is a tagged frame different from an untagged frame?

---

# 📌 Experiment 4.4 - QoS Priority using PCP

## Objective

Observe how the Priority Code Point (PCP) field in the VLAN tag can be used for traffic prioritization.

## Background

PCP is a 3-bit field inside the 802.1Q VLAN tag and is used by QoS mechanisms. It allows switches to prioritize certain traffic classes such as real-time control or safety-critical communication.

PCP priorities range from:

```text
0 = lowest
7 = highest
```

## Procedure

### Step 1 - Create two traffic streams

Traffic A:

```text
Priority 0
```

Traffic B:

```text
Priority 7
```

### Step 2 - Generate traffic simultaneously

### Step 3 - Capture packets with Wireshark

Apply:

```text
vlan
```

### Step 4 - Inspect PCP values

## Expected Observation

Different Ethernet frames should show different PCP values depending on the traffic class.

Example:

```text
Priority: 0
```

and

```text
Priority: 7
```

## Observation Table

| Traffic Stream | VLAN | PCP |
|----------------|------|-----|
| Stream A | | |
| Stream B | | |

## Example Automotive Priorities

| Function | Suggested PCP |
|----------|----------------|
| Brake Control | 7 |
| Steering | 6 |
| Camera Stream | 5 |
| Diagnostics | 2 |
| Software Update | 0 |

## Review Questions

1. What is PCP?
2. How many priority levels are available in the VLAN tag?
3. Why is QoS important in Ethernet networks?

---

## Conclusion

This exercise demonstrates that VLANs provide logical segmentation, while PCP enables QoS inside Ethernet traffic. Together, these features allow networks to be both secure and efficient in real-world deployments.

