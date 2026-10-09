# Exercise 4 - VLANs and Network Segmentation

## Exercise Objective

Understand how Ethernet networks can be logically segmented using Virtual LANs (VLANs).

Observe how VLAN tagging separates traffic, prevents communication between unrelated devices, and enables traffic prioritisation.

After completing this exercise, the following concepts should be clear:

- VLAN
- VLAN Identifier (VID)
- Tagged Frames
- Untagged Frames
- Access Ports
- Trunk Ports
- Inter-VLAN Isolation
- IEEE 802.1Q
- Priority Code Point (PCP)
- Quality of Service (QoS)

---

## Experiments

1. Experiment 4.1 - VLAN Creation
2. Experiment 4.2 - Inter-VLAN Isolation
3. Experiment 4.3 - VLAN Tag Observation
4. Experiment 4.4 - QoS Priority using PCP

---

# Experiment 4.1 - VLAN Creation

## Objective

Create multiple VLANs on a managed Ethernet switch and verify logical network separation.

---

## Concepts Covered

- VLAN
- VLAN ID
- Logical Segmentation
- Access Port

---

## Theory

A VLAN allows multiple logical networks to coexist on a single physical Ethernet switch.

Without VLANs:

```text
All ports belong to one network.
```

With VLANs:

```text
VLAN 10 → Network A

VLAN 20 → Network B
```

Traffic remains separated even though the same switch infrastructure is used.

---

## Hardware Required

- Managed Ethernet Switch
- PC 1
- PC 2
- PC 3

---

## Hardware Setup

```text
PC 1 ---- Port 1 (VLAN 10)

PC 2 ---- Port 2 (VLAN 20)

PC 3 ---- Port 3 (VLAN 10)
```

---

## Switch Configuration

Configure:

```text
Port 1 → VLAN 10

Port 2 → VLAN 20

Port 3 → VLAN 10
```

All ports should operate as Access Ports.

---

## IP Configuration

```text
PC 1 : 192.168.10.10

PC 2 : 192.168.20.10

PC 3 : 192.168.10.20
```

---

## Verification

Verify VLAN assignment using the switch management interface.

---

## Practical Activity

Record VLAN assignments.

| Device | Switch Port | VLAN |
|----------|----------|----------|
| PC 1 | | |
| PC 2 | | |
| PC 3 | | |

---

## Key Learning

- VLANs create logical networks.
- Multiple VLANs can exist on the same switch.
- Physical topology remains unchanged.

---

## Review Questions

1. What is a VLAN?
2. Why are VLANs used?
3. Does a VLAN require separate switches?

---

## Conclusion

Multiple VLANs were successfully created on a single Ethernet switch.

---

# Experiment 4.2 - Inter-VLAN Isolation

## Objective

Verify communication within a VLAN and observe communication failure between different VLANs.

---

## Concepts Covered

- VLAN Isolation
- Broadcast Domain
- Traffic Separation

---

## Theory

Devices belonging to the same VLAN can communicate directly.

Devices belonging to different VLANs require:

```text
Router

or

Layer 3 Switch
```

to exchange traffic.

---

## Hardware Setup

Same as Experiment 4.1.

---

## Procedure

### Test 1

From PC 1:

```bash
ping 192.168.10.20
```

Expected:

```text
Success
```

---

### Test 2

From PC 1:

```bash
ping 192.168.20.10
```

Expected:

```text
Timeout
```

---

## Expected Observation

Communication allowed:

```text
PC1 ↔ PC3
```

Communication blocked:

```text
PC1 ↔ PC2
```

---

## Practical Activity

Record observations.

| Source | Destination | Result |
|----------|----------|----------|
| PC 1 | PC 3 | |
| PC 1 | PC 2 | |
| PC 2 | PC 3 | |

---

## Key Learning

- VLANs isolate traffic.
- Devices in different VLANs cannot communicate directly.
- Each VLAN behaves like an independent Ethernet network.

---

## Review Questions

1. Why can PC 1 communicate with PC 3?
2. Why can PC 1 not communicate with PC 2?
3. What is required for communication between VLANs?

---

## Conclusion

Traffic isolation between VLANs was verified successfully.

---

# Experiment 4.3 - VLAN Tag Observation

## Objective

Capture and analyse IEEE 802.1Q VLAN-tagged Ethernet frames.

---

## Concepts Covered

- IEEE 802.1Q
- VLAN Tag
- VLAN Identifier
- Tagged Frames

---

## Theory

VLAN information is carried inside an Ethernet frame using the 802.1Q tag.

Simplified structure:

```text
Destination MAC

Source MAC

802.1Q Tag

EtherType

Payload
```

The 802.1Q tag contains:

```text
PCP

DEI

VID
```

---

## Hardware Setup

A trunk connection capable of carrying VLAN traffic is required.

---

## Procedure

### Step 1

Start Wireshark.

### Step 2

Generate traffic within VLAN 10.

### Step 3

Apply filter:

```text
vlan
```

---

## Expected Observation

Example:

```text
802.1Q Virtual LAN

Priority: 0

VLAN ID: 10
```

---

## Practical Activity

Record observed values.

| Parameter | Value |
|------------|------------|
| VLAN ID | |
| EtherType | |
| Frame Length | |
| PCP Value | |

---

## Key Learning

- VLAN information is embedded inside Ethernet frames.
- VLAN tags are added and removed by network devices.
- VLAN IDs identify logical networks.

---

## Review Questions

1. What is IEEE 802.1Q?
2. What is a VLAN ID?
3. What is a tagged frame?

---

## Conclusion

VLAN-tagged Ethernet frames were captured and analysed successfully.

---

# Experiment 4.4 - QoS Priority using PCP

## Objective

Observe traffic prioritisation using the Priority Code Point (PCP) field of the VLAN tag.

---

## Concepts Covered

- QoS
- PCP
- Traffic Prioritisation
- Traffic Classes

---

## Theory

IEEE 802.1Q contains:

```text
PCP
Priority Code Point
```

PCP uses:

```text
3 Bits
```

providing:

```text
Priority 0 → Lowest

Priority 7 → Highest
```

Higher-priority traffic can be forwarded before lower-priority traffic.

This concept forms the foundation of Time Sensitive Networking (TSN).

---

## Hardware Setup

Managed switch supporting QoS.

---

## Procedure

### Step 1

Create two traffic streams.

Traffic A:

```text
Priority 0
```

Traffic B:

```text
Priority 7
```

---

### Step 2

Generate traffic simultaneously.

---

### Step 3

Capture packets using Wireshark.

Apply:

```text
vlan
```

---

### Step 4

Inspect the PCP field.

---

## Expected Observation

Example:

```text
Priority: 0
```

and

```text
Priority: 7
```

for different traffic streams.

---

## Practical Activity

Record observations.

| Traffic Stream | VLAN | PCP |
|---------------|------|------|
| Stream A | | |
| Stream B | | |

---

## Automotive Mapping

Example vehicle traffic:

| Function | Suggested PCP |
|------------|------------|
| Brake Control | 7 |
| Steering | 6 |
| Camera Stream | 5 |
| Diagnostics | 2 |
| Software Update | 0 |

---

## Key Learning

- Not all Ethernet traffic requires equal priority.
- PCP allows prioritisation inside Ethernet networks.
- QoS improves handling of critical traffic.
- PCP is the foundation for deterministic automotive Ethernet communication.

---

## Review Questions

1. What is PCP?
2. How many priority levels are available?
3. Why is traffic prioritisation important?
4. Which vehicle functions require higher priorities?

---

## Conclusion

QoS prioritisation using PCP was observed successfully.

---

# Exercise
