# LIN Signal Encoding and Decoding Exercises

## Objective
Learn how data bytes become meaningful vehicle-like signals. These are teaching examples, not a production vehicle signal database.

## Setup and paste locations
Use the supplied `LIN.h` and `LIN.c`, STM32F446RE + TJA1021T2C, USART1 LIN mode at 19200 baud, and PLIN-View Pro. In `main.c`, include `LIN.h` under `USER CODE BEGIN Includes`; declare variables under `USER CODE BEGIN PV`; put helper functions under `USER CODE BEGIN 0`; put initialization under `USER CODE BEGIN 2`; and paste loop statements into the existing `while (1)` under `USER CODE BEGIN 3`. Do not add a second `main()`.

The argument to `LIN_SendFrame()` is the 6-bit frame ID; the driver calculates PID parity. `LIN_PackUint32()` writes big-endian bytes.

## Exercise 1 — One-byte speed
Paste inside the existing `while (1)` loop, replacing other transmit code while testing:

```c
uint8_t speed_kph = 60U;
uint8_t payload[1] = { speed_kph };  // Byte 0 = speed in km/h
LIN_Status status = LIN_SendFrame(0x01, payload, 1U);
(void)status;                        // Inspect in debugger if desired
HAL_Delay(100U);
```

Expected payload: `3C` (hex) = 60 decimal. Change speed to 0, 100, and 180 and predict each byte.

## Exercise 2 — Multiple signals
Paste inside the existing loop:

```c
uint8_t payload[4] = {0};
payload[0] = 60U;  // Speed: km/h
payload[1] = 3U;   // Gear number
payload[2] = 90U;  // Example engine temperature in degrees C
payload[3] = 70U;  // Fuel percentage
LIN_Status status = LIN_SendFrame(0x02, payload, 4U);
(void)status;
HAL_Delay(100U);
```

Expected bytes: `3C 03 5A 46`. Document each byte layout; changing byte order changes the meaning.

## Exercise 3 — Two-byte RPM
Paste in the existing loop:

```c
uint16_t engine_rpm = 2500U;
uint8_t payload[2];
LIN_PackUint32(payload, 2U, (uint32_t)engine_rpm); // Big-endian
LIN_Status status = LIN_SendFrame(0x03, payload, 2U);
(void)status;
HAL_Delay(100U);
```

Expected bytes: `09 C4`; decode as `(payload[0] << 8) | payload[1]` = 2500. Try 1000, 3000, and 6500 RPM. Do not copy little-endian CAN examples into this big-endian helper without adapting the decoder.

## Exercise 4 — Scaling
Represent 25.3 °C as integer 253, with a documented scale of 0.1 °C/count. Use a two-byte payload and `LIN_PackUint32(payload, 2U, 253U)`. Receiver calculation: `temperature_C = raw / 10.0`. The checksum does not define the signal's scale; the interface specification does.

## Exercise 5 — ADC callback / live sensor
Add under `USER CODE BEGIN 0` (ADC1 must be configured in CubeMX):

```c
static void ReadAdcSensor(uint8_t *data, uint8_t len)
{
    uint32_t raw = 0U;
    if (len == 0U) return;
    if (HAL_ADC_Start(&hadc1) == HAL_OK) {
        if (HAL_ADC_PollForConversion(&hadc1, 10U) == HAL_OK)
            raw = HAL_ADC_GetValue(&hadc1); // Usually 0..4095 for 12-bit ADC
        HAL_ADC_Stop(&hadc1);
    }
    LIN_PackUint32(data, len, raw); // Keep payload length configurable
}
```

Add under `USER CODE BEGIN PV`:

```c
#define SENSOR_FRAME_LEN 2U
static LIN_ScheduleSlot schedule[] = {
    {.id=0x01, .len=SENSOR_FRAME_LEN, .type=LIN_SLOT_PUBLISH,
     .source=ReadAdcSensor, .data={0}}
};
```

After GPIO and USART initialization, under `USER CODE BEGIN 2`:

```c
LIN_WakeTransceiver();
LIN_Schedule_Init(schedule, (uint8_t)(sizeof(schedule) / sizeof(schedule[0])));
```

Inside the existing loop:

```c
LIN_Schedule_Step(); // Refresh ADC data, then publish ID 0x01
HAL_Delay(50U);
```

This transmits raw ADC counts, not calibrated voltage or temperature. Ensure the ADC channel and sensor wiring match CubeMX configuration.

## Discussion
1. Why document a frame's byte layout?
2. Why does RPM need more than one byte?
3. Why must sender and receiver agree on endianness and scaling?
4. Does a valid checksum prove the physical sensor reading is correct?
