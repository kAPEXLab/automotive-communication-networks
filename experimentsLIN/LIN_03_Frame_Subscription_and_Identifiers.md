# LIN Frame IDs, Subscribers, and Acceptance

## Objective
Understand frame IDs and which frame a tool or node observes.

LIN's master initiates a header. A configured publisher supplies the response data, and subscribers consume it. LIN does not use CAN's multi-master arbitration. The supplied driver has no STM32 CAN-style hardware filter; do not paste bxCAN filter code into this LIN project.

## Exercise 1 — Observe one ID
Paste into the existing `while (1)` loop:

```c
uint8_t payload[2] = {0x2A, 0x64};
LIN_Status status = LIN_SendFrame(0x01, payload, 2U);
(void)status;
HAL_Delay(250U);
```

Configure PLIN-View Pro for the correct interface/baud rate and observe/subscribe to ID `0x01`. Confirm data `2A 64`. Change the transmit ID to `0x02`; an observation focused on `0x01` should no longer identify it as ID `0x01`. Configure observation of `0x02` to see the changed frame. UI wording can vary by PLIN-View Pro version.

## Exercise 2 — Same data, different ID

```c
uint8_t payload[2] = {0xAA, 0x55};
LIN_SendFrame(0x01, payload, 2U); // Test A
HAL_Delay(500U);
LIN_SendFrame(0x02, payload, 2U); // Test B: only ID changed
HAL_Delay(500U);
```

The payload is unchanged but frame identity differs. A real vehicle network specification defines what each ID means.

## Optional second STM32 node
True board-to-board data sharing requires receiver/slave firmware that detects the master header and validates the response. The current attached source files include the master driver and sensor `main.c`, but not a slave receiver implementation. If using the separate `LIN_Slave.h` and `LIN_Slave.c` described in the earlier framework, add them to a separate project and follow their instructions; the master driver alone does not implement a slave receiver.

## Discussion
1. Does changing the ID change payload bytes?
2. Does a subscriber transmit or consume a frame?
3. Why must each frame have a defined publisher?
4. Why is CAN hardware-filter code inappropriate here?
