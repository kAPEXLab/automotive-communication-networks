# CAN Combo Hands-on Labs: Single LM35 Node



Bench: Arduino Mega + MCP2515 with LM35 → CAN Combo (Device ID 0x42, channel_78) → EES → lab PC with Wireshark and ANDi.


## Task 1: One frame in Wireshark and ANDi

**Aim.** Decode one captured CAN frame field by field in both tools, and explain every byte between the Ethernet header and the CAN data.

**Steps**

1.  Start Wireshark on the network card connected to the EES and apply the filter `tecmp`.

2.  Pick a frame from source `TechnicaEngi_80:e7:20`. Expand *Technically Enhanced Capture Module Protocol* and its *Payload (CAN Data)*.

3.  Right-click *ID (11bit): 0x100* and choose *Apply as Filter → Selected*. Note the filter text Wireshark writes; you will reuse it in later tasks. (`can.id` does not work, because the CAN frame sits inside TECMP.)

4.  In ANDi, open the Traffic Viewer for channel_78 and select a frame with the same CAN ID.

5.  Fill in the table from both tools.

**Observation table**

| Field                  | Wireshark name and value | ANDi name and value | Meaning |
|------------------------|--------------------------|---------------------|---------|
| Device ID              |                          |                     |         |
| Counter                |                          |                     |         |
| Message Type           |                          |                     |         |
| Data Type              |                          |                     |         |
| Device / Probe Flags   |                          |                     |         |
| Interface / Channel ID |                          |                     |         |
| Timestamp              |                          |                     |         |
| Length                 |                          |                     |         |
| Data Flags             |                          |                     |         |
| CAN ID                 |                          |                     |         |
| Payload length         |                          |                     |         |
| Data                   |                          |                     |         |
| Frame length (bytes)   |                          |                     |         |

**Questions**

1.  ANDi shows the CAN ID as 256 and Wireshark as 0x100. Are they the same? Convert to show it.

2.  Wireshark says the frame is 60 bytes and ANDi says 51. Account for every byte of the 51, then explain the other 9.

3.  The timestamp shows a date in 1970. Is the module broken? What is the timestamp counting?

4.  Wireshark also shows a second Technica source, `80:e7:1c`. Find its Device ID and what Data Type it sends.

## Task 2: Find the encoding and decode engine temperature

**Aim.** Work out what the two data bytes mean using only the capture and a reference thermometer, then confirm it by changing the sender. This is what engineers do when no signal database (DBC) is available.

**Part 2A: test the hypotheses**

1.  Note the room temperature from a reference thermometer.

2.  Read the latest data bytes for ID 0x100 and convert them to decimal.

3.  Compute the temperature under each hypothesis. The LM35 gives 10 mV per °C.

| Hypothesis               | Formula                | Temperature (°C) | Plausible? |
|--------------------------|------------------------|------------------|------------|
| Raw ADC, 5 V reference   | raw × 5000 / 1023 / 10 |                  |            |
| Raw ADC, 1.1 V reference | raw × 1100 / 1023 / 10 |                  |            |
| Whole degrees            | raw                    |                  |            |
| Degrees × 100            | raw / 100              |                  |            |

State which hypothesis fits and why.

**Part 2B: confirm by changing the sender**

1.  Flash `lm35_can_lab.ino` with `ENCODING = 0`. Record five values from the capture and the Serial Monitor.

2.  Change to `ENCODING = 1`, flash again, and record five more.

3.  Decode both sets to °C. They should agree within one ADC step.

**Part 2C: heat test**

1.  With `ENCODING = 1`, hold the LM35 between your fingers for 30 s, then let go. Capture for 2 minutes.

2.  In Wireshark, right-click the data bytes and choose *Apply as Column*, then *File → Export Packet Dissections → As CSV*. In ANDi, use *Export Displayed*.

3.  Plot temperature against TECMP time in a spreadsheet. Mark where you held the sensor and where you let go.

**Questions**

1.  The original capture flickered between 42 and 43. How many °C is one ADC step with the 5 V reference, and with the 1.1 V reference?

2.  `ENCODING = 1` sends °C × 100. Does that make the reading 100 times more precise? Explain.

## Task 3: Cycle time and jitter from hardware timestamps

**Aim.** Measure how regularly the node sends, using the CAN Combo's timestamps, and find out why the original sketch sends every 1.01 s instead of 1.00 s.

**Steps**

1.  Capture 60 s with the original sketch. Note ANDi's Delta column for 10 frames.

2.  Flash `lm35_can_lab.ino` with `CYCLE_MS = 1000` and capture 60 s.

3.  Change to `CYCLE_MS = 100` and capture 30 s.

4.  For each run, take the TECMP Timestamp of 11 consecutive frames and compute the 10 gaps.

**Observation table**

| Run        | Sketch setting                          | Min gap (ms) | Mean gap (ms) | Max gap (ms) | Max − min = jitter (ms) |
|------------|-----------------------------------------|--------------|---------------|--------------|-------------------------|
| A          | Original sketch                         |              |               |              |                         |
| B          | `CYCLE_MS = 1000`                         |              |               |              |                         |
| C          | `CYCLE_MS = 100`                          |              |               |              |                         |
| C, PC time | Same frames as C, Wireshark Time column |              |               |              |                         |

**Questions**

1.  The original sketch most likely uses `delay(1000)` after reading and sending. Why does that give 1.01 s and not 1.00 s?

2.  The reference sketch schedules with `last += CYCLE_MS`. Why does its mean stay at the set value even though each loop takes some time?

3.  Compare run C with its PC-time row. Which has more jitter, and where does the extra jitter come from?

4.  Over one hour, how far would the original sketch drift from a true 1 s schedule?

5.  Why does a receiving ECU care about cycle time? Name one fault a receiver can detect from it.

## Task 4: Frame format and bus load

**Aim.** See how the CAN ID type, the data length and the cycle time change what goes on the bus, and calculate the bus load each causes.

**Steps.** Change one constant at a time, flash, capture 20 s, and record what Wireshark shows under *ID Field* and *Payload Length*.

| Run | Change                             | ID Field (hex) | ID type shown | Payload Length | Frames in 10 s |
|-----|------------------------------------|----------------|---------------|----------------|----------------|
| 1   | Defaults (`CYCLE_MS = 1000`)         |                |               |                |                |
| 2   | `MSG_ID = 0x0C8`                     |                |               |                |                |
| 3   | `EXTENDED_ID = true`, `MSG_ID = 0x100` |                |               |                |                |
| 4   | `PAD_TO_DLC = 8`                     |                |               |                |                |
| 5   | `CYCLE_MS = 10`                      |                |               |                |                |

**Bus load calculation**

A classic CAN frame is about 47 + 8 × DLC bits with an 11-bit ID, and 67 + 8 × DLC bits with a 29-bit ID, before bit stuffing. At 500 kbit/s:

```
Bus load (%) = (bits per frame × frames per second) ÷ 500 000 × 100
```

| Run                            | Bits per frame | Frames per second | Bus load (%) |
|--------------------------------|----------------|-------------------|--------------|
| 1                              |                |                   |              |
| 3                              |                |                   |              |
| 4                              |                |                   |              |
| 5                              |                |                   |              |
| Run 1 at 1 ms (calculate only) |                |                   |              |

**Questions**

1.  In run 2 the ID is smaller. If two nodes sent 0x0C8 and 0x100 at the same moment, which would win arbitration, and why?

2.  Which bit of Wireshark's ID Field changed in run 3? Why does a 29-bit ID cost 20 more bits per frame?

## Task 5: ACK, bitrate and termination faults with a lone node

**Aim.** With only one sending node, the CAN Combo is the only other device on the bus. Find out what it does for that node, and what the capture and the sender each show when the bus goes wrong.

**Safety.** Power off before changing any wiring or jumper. Restore the bench after each run.

**Steps.** Use `CYCLE_MS = 1000` and keep the Serial Monitor open: it shows the sender's count of failed sends. For each run, capture 20 s and fill in a row.

| Run | Change                                                                                              | Frames in 10 s (Wireshark) | Data Flags | Failed sends (Serial) | What happened on the bus |
|-----|-----------------------------------------------------------------------------------------------------|----------------------------|------------|-----------------------|--------------------------|
| 1   | None (baseline)                                                                                     |                            |            |                       |                          |
| 2   | On the Combo's web page, switch the channel to pure listen-only if an option exists (note its name) |                            |            |                       |                          |
| 3   | Combo channel bitrate set to 250 kbit/s; the Mega stays at 500                                      |                            |            |                       |                          |
| 4   | All termination removed (see table below)                                                           |                            |            |                       |                          |

**Termination.** With power off, measure the resistance between CAN_H and CAN_L for each combination of the MCP2515 module's 120 Ω jumper (if fitted) and the Combo channel's termination setting.

| MCP2515 jumper | Combo termination | Measured (Ω) |
|----------------|-------------------|--------------|
| On             | On                |              |
| On             | Off               |              |
| Off            | On                |              |
| Off            | Off               |              |

**Questions**

1.  In run 1 every frame is Ack'ed although only one ECU is on the bus. Who sent the ACK, and what does that tell you about the Combo channel's mode?

2.  Explain run 3 from the sender's side: why does a wrong bitrate on the *capture* device make the *sender* fail?

3.  In a real car, why must a test tool on the bus be truly listen-only?

4.  Which run would you use to demonstrate "error passive" to a class, and what would you show?

## Task 6: Alive counter and checksum

**Aim.** Add the two protections real ECUs put on safety-relevant signals, and use them to tell a lost frame, a restarted sender and a corrupted payload apart from the capture alone.

**Message layout** with `ENCODING = 1`, `ADD_ALIVE = true`, `ADD_CHECKSUM = true` (DLC 4):

| Byte | Content                                 |
|------|-----------------------------------------|
| 0–1  | Temperature × 100, big-endian           |
| 2    | Alive counter, 0 to 15, then wraps to 0 |
| 3    | Checksum: byte 0 XOR byte 1 XOR byte 2  |

**Part 6A: verify.** Flash with the settings above. For three consecutive frames, write the four data bytes, check the alive counter steps by 1, and recompute the checksum by hand.

**Part 6B: restart the sender.** Hold the Mega's reset button for about 3 s, then release. In the capture, find the gap and the alive counter value right after it. How does a receiver tell "sender restarted" from "frames lost"?

**Part 6C: paper checks.** For each received frame, say whether it is valid, and if not, what went wrong.

| Received data bytes | Previous alive value | Valid? | Problem |
|---------------------|----------------------|--------|---------|
| 00 2A 05 2F         | 4                    |        |         |
| 09 29 0B 2B         | 9                    |        |         |
| 0B B8 0C B7         | 11                   |        |         |
| 0B B8 0D BE         | 13                   |        |         |

**Questions**

1.  CAN already has a 15-bit CRC (you saw CRC15 in Task 1). Why add a checksum in the data as well?

2.  AUTOSAR End-to-End (E2E) protection does the same job on CAN and on SOME/IP. Which two faults from 6B and 6C does it catch?

## Task 7: TECMP counter and capture integrity

**Aim.** Decide whether a capture is complete. Learn what the TECMP Counter really counts, so a gap is not mistaken for lost data.

**Steps**

1.  Capture 5 minutes with the display filter `tecmp`. Do not filter on data type.

2.  Right-click *Message Type* and *Counter* and choose *Apply as Column* for each. Do the same for *Device ID*.

3.  Filter on Device ID 0x42 only. List every Message Type you see and how many of each.

4.  Now filter on Device ID 0x42 *and* Logging Stream. Record the Counter of 10 consecutive frames.

**Observation table**

| Message Type (name and number) | Count in 5 min | What it carries |
|--------------------------------|----------------|-----------------|
|                                |                |                 |
|                                |                |                 |
|                                |                |                 |

| View                       | Counter step between consecutive frames | Any gaps? |
|----------------------------|-----------------------------------------|-----------|
| All TECMP from Device 0x42 |                                         |           |
| Logging Stream only        |                                         |           |

## Mini-project: Engine temperature dashboard

**Brief.** Build a live web page that shows the engine temperature from your node, read straight from the TECMP stream on the PC's network card. It closes the loop from LM35 to screen, and it is the first step of the CAN-to-SOME/IP gateway in Lab 2.

**Requirements**

1.  A Python script (Scapy or pyshark) captures frames with EtherType 0x99FE from Device ID 0x42.

2.  It keeps only Logging Stream frames with Data Type CAN and CAN ID 0x100, and decodes the Task 6 layout.

3.  It rejects frames with a bad checksum and counts lost frames from the alive counter.

4.  A web page shows the temperature as a gauge, a 5-minute trend, and the lost-frame and bad-checksum counts. It turns red above 40 °C.

5.  Every received value is logged to CSV with its TECMP timestamp.

**Stretch.** Re-publish the temperature as a SOME/IP event (Service 0x1234, Event 0x8001) and watch it in the Lab 2 monitor.

**Demonstration.** Heat the LM35 until the gauge turns red, press the Mega's reset button, and show the counters react.
