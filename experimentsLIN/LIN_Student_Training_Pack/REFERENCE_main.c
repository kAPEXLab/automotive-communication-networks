#include "LIN.h"

#define SENSOR_FRAME_LEN  2    /* <-- change ONLY this to resize the payload */

/* Now generic: works whether SENSOR_FRAME_LEN is 1, 2, 3, or 4 (or up to 8
 * if you want to pad further, though an ADC reading rarely needs that many).
 * "len" here will always match whatever the slot was given, so there's no
 * second place to keep in sync. */
static void ReadSensor(uint8_t *data, uint8_t len)
{
    HAL_ADC_Start(&hadc1);
    HAL_ADC_PollForConversion(&hadc1, 10);
    uint16_t raw = HAL_ADC_GetValue(&hadc1);   /* 0-4095, exact 12-bit reading */

    LIN_PackUint32(data, len, (uint32_t)raw);
}

static LIN_ScheduleSlot schedule[] = {
    { .id = 0x01, .len = SENSOR_FRAME_LEN, .type = LIN_SLOT_PUBLISH,
      .source = ReadSensor, .data = {0} },
};

  while (1)
  {
    LIN_Schedule_Step();   /* reads sensor, then publishes it as 01h */
    HAL_Delay(50);
  }
}