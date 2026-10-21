# LIN Alive Counter and Frame Supervision

## Objective
Add a rolling counter to a periodic LIN frame. The counter is an application signal, not the frame ID and not a replacement for the LIN checksum.

## Exercise 1 — Rolling counter
Add under `USER CODE BEGIN PV`:

```c
static uint8_t alive_counter = 0U;
```

Paste inside the existing `while (1)` loop:

```c
uint8_t payload[2] = {0};
payload[0] = 1U;             // Example system status: active
payload[1] = alive_counter;  // Counter byte changes every frame
LIN_Status status = LIN_SendFrame(0x05, payload, 2U);
(void)status;
alive_counter++;
HAL_Delay(100U);
```

Expected payloads: `01 00`, `01 01`, `01 02`, `01 03`, and so on. The driver calculates the LIN checksum automatically.

## Exercise 2 — Four-bit counter
Replace `alive_counter++` with:

```c
alive_counter = (uint8_t)((alive_counter + 1U) & 0x0FU);
```

Expected sequence: `00, 01, 02, ... 0E, 0F, 00, 01...` in byte 1.

## Exercise 3 — Restart observation
1. Observe the counter incrementing in PLIN-View Pro.
2. Press STM32 reset.
3. Observe the counter restart at zero.
4. Discuss how a receiver might distinguish reset from normal wraparound.

A counter restart alone is not definitive proof of a reset. Production systems combine sequence monitoring with timeout supervision, plausibility checks, and fault handling.

## Optional schedule callback
Add under `USER CODE BEGIN 0`:

```c
static uint8_t schedule_alive_counter = 0U;
static void FillStatusFrame(uint8_t *data, uint8_t len)
{
    if (len < 2U) return; // Schedule must configure at least two bytes
    data[0] = 1U;
    data[1] = schedule_alive_counter++;
}
```

Add under `USER CODE BEGIN PV`:

```c
static LIN_ScheduleSlot schedule[] = {
    {.id=0x05, .len=2U, .type=LIN_SLOT_PUBLISH,
     .source=FillStatusFrame, .data={0}}
};
```

After GPIO and USART initialization:

```c
LIN_WakeTransceiver();
LIN_Schedule_Init(schedule, (uint8_t)(sizeof(schedule) / sizeof(schedule[0])));
```

Inside the existing loop:

```c
LIN_Schedule_Step();
HAL_Delay(100U);
```

Use either direct-send or scheduler mode while testing, not both unless you intentionally want multiple frames.

## Discussion
1. Does a valid checksum prove a fresh sensor sample was generated?
2. What if the frame arrives but the counter never changes?
3. Why should a receiver use a timeout as well as a counter?
4. What additional evidence would confirm a transmitter reset?
