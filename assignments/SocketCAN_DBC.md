# Problem Statement 3
# AI-Assisted DBC Generation and CAN Data Visualization Using SocketCAN

## Background

In automotive and embedded systems, raw CAN messages are difficult to interpret without a message database. Engineers use **DBC (Database CAN) files** to define message structures, signals, scaling, units, and decoding rules.

Traditionally, engineers manually create DBC files, but modern AI tools can assist in generating, validating, documenting, and improving DBC definitions.

In this assignment, you will use an AI tool of your choice to assist in creating a DBC file, implement a SocketCAN-based CAN network, and visualize decoded CAN data using a CAN analysis tool.

---

## Objective

Learn how CAN messages are transformed from raw hexadecimal data into meaningful engineering values using a DBC file.

Design a DBC database, generate CAN traffic through SocketCAN, and visualize the decoded signals using an appropriate CAN analysis tool.

---

## Learning Outcomes

After completing this assignment, students should be able to:

- Understand the purpose of DBC files.
- Create CAN message and signal definitions.
- Use AI tools to assist CAN database development.
- Validate DBC definitions.
- Generate CAN traffic over SocketCAN.
- Decode raw CAN messages.
- Visualize engineering signals using a CAN tool.
- Understand how OEMs and Tier-1 suppliers define CAN communication databases.

---

# System Description

Design a simple **Vehicle Information Network** consisting of the following data signals:

| Signal | Unit |
|----------|----------|
| Vehicle Speed | km/h |
| Engine RPM | rpm |
| Coolant Temperature | °C |
| Fuel Level | % |
| Battery Voltage | V |

The values shall be transmitted using CAN messages and decoded using a DBC file.

---

# Task 1: Understanding DBC Files

Investigate and document:

- What is a DBC file?
- Why is it required?
- What information does it contain?
- How do CAN tools use DBC files?

**Deliverable:** 1-page summary.

---

# Task 2: CAN Message Design

Design CAN messages for the selected signals.

For each signal define:

- CAN Identifier
- Start Bit
- Signal Length
- Data Type
- Scaling
- Offset
- Minimum Value
- Maximum Value
- Unit

**Deliverable:** Signal definition document.

---

# Task 3: AI-Assisted DBC Development

Use any AI tool to assist in generating a DBC file.

Examples:

- Microsoft Copilot
- ChatGPT
- Claude
- Gemini
- Other AI assistants

The AI tool may be used for:

- DBC syntax generation
- Signal layout suggestions
- Validation
- Documentation generation
- Review of message definitions

Students must verify and correct any AI-generated content before use.

---

## AI Usage Report

Document:

- Tool used
- Prompts given
- AI-generated outputs
- Corrections made
- Lessons learned

**Deliverable:** AI usage report.

---

# Task 4: DBC Validation

Validate that:

- All messages decode correctly.
- Signal ranges are correct.
- Units are correct.
- Scaling factors are correct.

Document any issues found and how they were corrected.

---

# Task 5: SocketCAN Integration

Implement a SocketCAN-based transmitter that periodically sends:

### Vehicle Speed

```text
0 - 120 km/h
```

### Engine RPM

```text
800 - 5000 rpm
```

### Coolant Temperature

```text
20 - 120 °C
```

### Fuel Level

```text
0 - 100 %
```

### Battery Voltage

```text
11 - 15 V
```

Values should change realistically during execution.

---

# Task 6: CAN Data Visualization

Import the DBC file into a CAN analysis tool.

Examples may include:

- BUSMASTER
- SavvyCAN
- CANalyzer (if available)
- Any equivalent CAN visualization tool

Configure the tool to display:

- Decoded signal names
- Engineering values
- Units
- Message activity

---

# Task 7: Signal Monitoring Dashboard

Create a monitoring view that displays:

```text
Speed               : 65 km/h
Engine RPM          : 2450 rpm
Coolant Temperature : 87 °C
Fuel Level          : 72 %
Battery Voltage     : 12.5 V
```

The display should update automatically as CAN messages are received.

---

# Challenge 1: Raw Data vs Decoded Data

Capture the same CAN traffic in two forms:

### View 1

Raw CAN frames

Example:

```text
ID: 0x100
DATA: 2C 01 45 00
```

### View 2

DBC-decoded signals

Example:

```text
Vehicle Speed : 30 km/h
Coolant Temp  : 69 °C
```

Explain the difference.

---

# Challenge 2: Modify the DBC

Change one signal definition.

Examples:

- Scaling
- Offset
- Unit
- Signal length

Observe:

- What changes in the displayed values?
- What happens if the DBC is incorrect?

Document findings.

---

# Challenge 3: Add a New Signal

Add:

```text
Ambient Temperature
```

or

```text
Gear Position
```

Update:

- Message definition
- DBC file
- Transmitter application
- Visualization tool

Demonstrate successful decoding.

---

# Challenge 4: AI Review

Use an AI tool to review your DBC file and identify:

- Possible errors
- Inefficient signal layout
- Missing documentation
- Naming improvements

Evaluate whether the suggestions are useful.

---

# Deliverables

## 1. DBC File

Complete and validated CAN database.

---

## 2. Source Code

- CAN transmitter
- CAN data generator
- Supporting utilities

---

## 3. AI Usage Report

Include:

- AI tool selected
- Prompts used
- Generated outputs
- Corrections performed
- Final observations

---

## 4. Technical Report

Maximum 5 pages.

Include:

- Signal design
- DBC structure
- Validation process
- SocketCAN integration
- Visualization screenshots
- Lessons learned

---

## 5. Demonstration

Demonstrate:

1. SocketCAN message transmission
2. Raw CAN traffic
3. DBC import
4. Signal decoding
5. Signal visualization
6. DBC modification and impact
7. AI-assisted DBC generation process

---

## Expected Learning Outcomes

By completing this assignment, students will understand the complete CAN workflow:

```text
Signal Definition
        ↓
CAN Message Design
        ↓
DBC Creation
        ↓
SocketCAN Communication
        ↓
Raw CAN Frames
        ↓
DBC-Based Decoding
        ↓
Human Readable Signals
        ↓
Visualization & Analysis
```

This mirrors the workflow used by automotive OEMs and Tier-1 suppliers during CAN network development, calibration, diagnostics, and validation activities.
