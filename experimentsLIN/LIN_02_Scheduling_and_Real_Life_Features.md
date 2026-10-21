# LIN Scheduling and Real-Life Features

## Objective
See how a LIN master schedules frames, unlike CAN's multi-master arbitration. In the supplied project the STM32 is master and PLIN-View Pro observes its published frames.

## Exercise 1 — Slow and fast frame updates
Paste into the existing `while (1)` loop in `main.c`:

```c
uint8_t payload[2] = {0x12, 0x34};
LIN_Status status = LIN_SendFrame(0x01, payload, 2U);
(void)status;
HAL_Delay(1000U); // Then test 100U and 20U
```

Observe ID `0x01` in PLIN-View Pro. Data should be `12 34`. Compare update intervals; delay is approximate because execution and frame time add overhead.

## Exercise 2 — Round-robin schedule
Add under `USER CODE BEGIN PV`:

```c
static LIN_ScheduleSlot schedule[] = {
    {.id=0x01, .len=1U, .type=LIN_SLOT_PUBLISH, .source=0, .data={60U}},
    {.id=0x02, .len=2U, .type=LIN_SLOT_PUBLISH, .source=0, .data={3U, 1U}}
};
```

After GPIO and USART initialization, under `USER CODE BEGIN 2`:

```c
LIN_WakeTransceiver();
LIN_Schedule_Init(schedule, (uint8_t)(sizeof(schedule) / sizeof(schedule[0])));
```

Inside the existing loop:

```c
LIN_Schedule_Step(); // One slot per call: 0x01, then 0x02, then repeat
HAL_Delay(100U);
```

Expected data sequence: ID `01` -> `3C`; ID `02` -> `03 01`; then repeat. With two slots and a 100 ms loop delay, each ID recurs roughly every 200 ms plus processing time.

## Exercise 3 — Automotive-style body electronics
Use these fictional teaching definitions:

| LIN ID | Payload | Meaning |
|---|---|---|
| `0x01` | Byte 0 = 0..100 | Window position (%) |
| `0x02` | Byte 0 = 0 or 1 | Switch released/pressed |
| `0x03` | Byte 0 = 0 or 1 | Lamp off/on |

Add a third publish slot with ID `0x03` and observe the round-robin sequence. Actual vehicle schedules and signal definitions come from the network design.

## Publisher vs request
`LIN_SendFrame()` publishes data from the master. `LIN_MasterRequest()` sends a header and waits for a slave response. Use request only when another configured node is expected to respond. A local sensor attached to the master should normally be published, not requested. A request with no responder can time out.

## Discussion
1. Why does LIN need a master schedule?
2. What happens if a request has no slave responder?
3. How does the schedule differ from CAN arbitration?
