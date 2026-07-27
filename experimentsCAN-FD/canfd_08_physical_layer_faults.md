# Physical Layer Fault Demonstrations

## Objective

Demonstrate common CAN FD physical-layer faults and observe their effect on communication.

This experiment covers:

```text
1. Missing termination
2. CAN_H open
3. CAN_L open
4. CAN_H shorted to CAN_L
5. CAN_H shorted to GND
6. CAN_L shorted to GND
```

---

## Setup

Use two PCAN-USB FD modules connected as shown below:

```text
PCAN-USB FD #1  <---- CAN FD bus ---->  PCAN-USB FD #2
```

Configure both nodes in PCAN-View:

```text
Mode:             CAN FD
Nominal bit rate: 500 kbit/s
Data bit rate:    2 Mbit/s
```

---

## Healthy Wiring

Use the following wiring:

```text
Node 1 CAN_H  ----  Node 2 CAN_H
Node 1 CAN_L  ----  Node 2 CAN_L
Node 1 GND    ----  Node 2 GND
```

D-Sub pinout:

```text
Pin 7 = CAN_H
Pin 2 = CAN_L
Pin 3 = CAN_GND
```

---

## Correct Termination

Use two 120 ohm termination resistors:

```text
120 ohm between CAN_H and CAN_L near Node 1
120 ohm between CAN_H and CAN_L near Node 2
```

Before powering or initializing the interface, verify resistance between CAN_H and CAN_L:

```text
Expected resistance: approximately 60 ohm
```

---

## Baseline Test Frame

Before creating any fault, verify normal communication.

From **Node 1**, transmit the following cyclic CAN FD frame:

```text
ID:      901
Length:  8
Type:    CAN FD
BRS:     ON
RTR:     OFF
Cycle:   100 ms
Data:    11 22 33 44 55 66 77 88
```

Expected observation on **Node 2**:

```text
ID 0x901 received successfully.
```

---

# Fault 1: Remove One Termination

## Action

Remove one 120 ohm termination resistor.

The bus now has only one termination resistor.

```text
One 120 ohm resistor present
One 120 ohm resistor removed
```

---

## Expected Observation

Possible observations:

```text
Communication may continue.
Errors may or may not appear.
Bus load may still appear normal.
```

---

## Explanation

With a very short lab cable, communication may continue even with one missing termination.

However, this is not a correct or robust CAN bus condition.

>> Working communication does not always mean reliable communication.


---

## Key Point

```text
One missing termination reduces signal integrity margin.
The bus may work in a short lab setup but may fail in real wiring conditions.
```

---

# Fault 2: Remove Both Terminations

## Action

Stop cyclic transmission first.

Remove both 120 ohm termination resistors.

The bus now has no proper termination.

```text
No 120 ohm termination present
```

Restart the cyclic transmission from Node 1.

---

## Expected Observation

Possible observations:

```text
Error Passive Warning
Bus warning
Unstable communication
Frame loss
```

---

## Explanation

Without termination, signal reflections increase and the differential signal becomes unreliable.

CAN FD is more sensitive to termination quality because the data phase may run at higher speed.

---

## Recovery

```text
1. Stop cyclic transmission.
2. Restore both 120 ohm termination resistors.
3. Verify CAN_H to CAN_L resistance is approximately 60 ohm.
4. Reinitialize both PCAN-View windows if required.
5. Verify normal communication again.
```

---

# Fault 3: Disconnect CAN_H

## Action

Open the CAN_H line.

```text
Node 1 CAN_H  ----x----  Node 2 CAN_H
```

Keep CAN_L and GND connected.

---

## Expected Observation

Possible observations:

```text
Error Passive Warning
Bus warning
No reception
Unstable communication
```

---

## Explanation

CAN uses a differential pair.

If CAN_H is disconnected, the receiver cannot correctly detect the differential signal between CAN_H and CAN_L.

---

## Recovery

```text
1. Reconnect CAN_H.
2. Stop and restart transmission if required.
3. Reinitialize PCAN-View if the error state remains.
4. Verify normal communication again.
```

---

# Fault 4: Disconnect CAN_L

## Action

Open the CAN_L line.

```text
Node 1 CAN_L  ----x----  Node 2 CAN_L
```

Keep CAN_H and GND connected.

---

## Expected Observation

Possible observations:

```text
Error Passive Warning
Bus warning
No reception
Unstable communication
```

---

## Explanation

CAN_L is also part of the differential pair.

If CAN_L is disconnected, the differential signal becomes invalid or unreliable.

---

## Recovery

```text
1. Reconnect CAN_L.
2. Stop and restart transmission if required.
3. Reinitialize PCAN-View if the error state remains.
4. Verify normal communication again.
```

---

# Fault 5: Short CAN_H to CAN_L

## Action

Briefly short CAN_H and CAN_L.

```text
CAN_H shorted to CAN_L
```

Using D-Sub pins:

```text
Pin 7 shorted to Pin 2
```

Keep the short only for a short duration.

---

## Expected Observation

Possible observations:

```text
Bus-Off
Error Passive Warning
Bus warning
No reception
```

---

## Explanation

When CAN_H and CAN_L are shorted together, the differential voltage collapses.

The receiver cannot distinguish dominant and recessive bits correctly.

This is a severe physical-layer fault.

---

## Recovery

```text
1. Remove the short immediately.
2. Stop cyclic transmission.
3. Close both PCAN-View windows if Bus-Off remains.
4. Restore correct wiring and termination.
5. Reopen both PCAN-View windows.
6. Reinitialize both nodes with identical CAN FD settings.
7. Verify normal communication again.
```

---

# Fault 6: Short CAN_H to GND

## Action

Briefly short CAN_H to CAN_GND.

```text
CAN_H shorted to CAN_GND
```

Using D-Sub pins:

```text
Pin 7 shorted to Pin 3
```

Keep the short only for a short duration.

---

## Expected Observation

Possible observations:

```text
Bus-Off
Error Passive Warning
Bus warning
No reception
```

---

## Explanation

During a dominant bit, CAN_H is expected to rise.

If CAN_H is shorted to GND, CAN_H cannot rise properly.

The differential voltage becomes invalid and communication errors occur.

---

## Recovery

```text
1. Remove the short immediately.
2. Stop cyclic transmission.
3. Close both PCAN-View windows if required.
4. Restore correct wiring and termination.
5. Reopen both PCAN-View windows.
6. Verify normal communication again.
```

---

# Fault 7: Short CAN_L to GND

## Action

Briefly short CAN_L to CAN_GND.

```text
CAN_L shorted to CAN_GND
```

Using D-Sub pins:

```text
Pin 2 shorted to Pin 3
```

Keep the short only for a short duration.

---

## Expected Observation

Possible observations:

```text
Communication may continue in a short lab setup.
Error Passive Warning may appear.
Bus warning may appear.
No reception may occur in some setups.
```

---

## Explanation

CAN_L to GND is still an invalid physical-layer condition.

In a short lab setup, communication may appear to continue because CAN_H may still provide enough detectable signal for the transceiver.

However, this should not be treated as a safe or acceptable condition.

```text
CAN_L to GND is a physical-layer fault even if communication appears to continue.
```

---

## Recovery

```text
1. Remove the short immediately.
2. Stop cyclic transmission.
3. Restore healthy wiring.
4. Reinitialize PCAN-View if required.
5. Verify normal communication again.
```

---

## Summary Table

```text
Fault Condition              Expected / Possible Observation
-------------------------------------------------------------
One termination removed      Communication may continue
Both terminations removed    Error Passive Warning / unstable bus
CAN_H disconnected           Error Passive Warning / no reception
CAN_L disconnected           Error Passive Warning / no reception
CAN_H shorted to CAN_L       Bus-Off / severe error
CAN_H shorted to GND         Bus-Off / severe error
CAN_L shorted to GND         May continue or show errors
```

---

## General Recovery Procedure

After every fault experiment:

```text
1. Remove the fault.
2. Stop cyclic transmission.
3. Restore CAN_H, CAN_L, GND, and both terminations.
4. Verify CAN_H to CAN_L resistance is approximately 60 ohm.
5. Close and reopen PCAN-View if required.
6. Reinitialize both nodes with identical CAN FD settings.
7. Send a simple test frame.
```

Use this recovery test frame from Node 1:

```text
ID:      999
Length:  8
Type:    CAN FD
BRS:     ON
RTR:     OFF
Data:    11 22 33 44 55 66 77 88
```

Expected observation on Node 2:

```text
ID 0x999 received successfully.
```

---

## Key Observations

```text
1. CAN FD depends on a healthy differential pair.
2. Proper termination is required for reliable communication.
3. A short lab cable may hide some termination problems.
4. CAN_H to CAN_L short is a severe fault.
5. CAN_H to GND is usually a severe fault.
6. CAN_L to GND may not always fail immediately, but it is still invalid.
7. Bus-Off is a protection state after repeated severe errors.
```

---

## Discussion Points

```text
1. Why can communication continue with one termination removed?
2. Why does removing both terminations create errors?
3. Why does shorting CAN_H to CAN_L cause Bus-Off?
4. Why can CAN_L to GND behave differently from CAN_H to GND?
5. Why is CAN FD more sensitive to physical-layer quality than Classical CAN?
6. Why should fault conditions not be kept active for a long time?
```

---

## Conclusion

CAN FD communication depends on correct wiring, healthy differential signaling, and proper termination.

Fault behavior may vary depending on cable length, bit rate, termination, and transceiver design.

A bus that works in a short lab setup may still be electrically incorrect and unreliable for real applications.
