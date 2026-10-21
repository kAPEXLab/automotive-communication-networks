# LIN Fault Injection and Troubleshooting Lab

## Safety first
Never short LIN to ground or VBAT. Power down before changing wiring. Do not disconnect/reconnect powered wiring unless your hardware procedure permits it. Introduce one fault at a time and restore the known-good configuration after each test.

## Baseline
Paste into the existing `while (1)` loop:

```c
uint8_t payload[4] = {0x11, 0x22, 0x33, 0x44};
LIN_Status status = LIN_SendFrame(0x01, payload, 4U);
(void)status; // Set a debugger breakpoint to inspect status
HAL_Delay(500U);
```

Confirm data `11 22 33 44` appears in PLIN-View Pro before proceeding.

## Fault 1 — Baud-rate mismatch
Change the analyzer baud rate away from the STM32 configuration. Frames may fail to decode or appear invalid. Restore both to the reference 19200 baud. A decode failure does not prove the MCU failed to transmit.

## Fault 2 — Transceiver sleep control
The supplied driver drives PB0 high as `SLP_N`. Power down and verify PB0 is wired to SLP_N and configured as GPIO output. Ensure `LIN_WakeTransceiver()` is called after `MX_GPIO_Init()`, because CubeMX may set PB0 low during GPIO initialization. A sleeping transceiver may stop passing traffic.

## Fault 3 — Wrong frame ID
Compare the baseline `LIN_SendFrame(0x01, payload, 4U)` with `LIN_SendFrame(0x02, payload, 4U)`. The payload is associated with ID `0x02`; a subscriber expecting ID `0x01` may appear to have stopped receiving the expected frame.

## Fault 4 — Payload length mismatch
Send four bytes, then deliberately configure the observing frame definition for a different length if the analyzer supports that. Decode may be inconsistent or flagged; exact behavior depends on tool configuration. Restore the matching length.

## Fault 5 — No slave response
Only test `LIN_MasterRequest()` when a real slave is configured to respond to the requested ID. Without a responder it may return `LIN_ERR_NO_RESPONSE`; a partial response may return `LIN_ERR_RX_TIMEOUT`; a wrong checksum may return `LIN_ERR_CHECKSUM`. Do not use master request to read a sensor attached locally to the master.

## Fault 6 — Invalid software parameters
Safe software-only test in the debugger:

```c
uint8_t payload[1] = {0};
LIN_Status bad_id = LIN_SendFrame(0x40, payload, 1U); // ID > 0x3F
LIN_Status bad_len = LIN_SendFrame(0x01, payload, 0U); // Length 0
```

Both should return `LIN_ERR_PARAM`.

## Debugging order
1. Power and common ground.
2. LIN bus wire and analyzer connection.
3. PB0 configured; SLP_N high during normal operation.
4. USART1 in LIN mode at the intended baud rate.
5. Master actually calls `LIN_SendFrame()` or `LIN_Schedule_Step()`.
6. Analyzer baud rate and observed frame ID.
7. Inspect `LIN_Status` in the debugger.
8. Restore baseline before the next test.

## Discussion
1. Why introduce only one fault at a time?
2. How is no frame different from a frame with an unexpected ID?
3. Why does a successful API return not guarantee the analyzer decoded it?
