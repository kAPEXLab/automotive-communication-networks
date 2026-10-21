/*
 * LIN.c -- STM32F446RE + NXP TJA1021T2C LIN master driver (USART1)
 *
 * Adds a "data source" callback on publish slots so the scheduler can
 * pull a fresh sensor reading right before each frame goes out.
 */
#include "main.h"
#include "LIN.h"

extern UART_HandleTypeDef huart1;

/* =========================================================================
 * SECTION 1: Wake the transceiver (register level)
 *
 * Same two-register trick as before: enable GPIOB's clock, then set PB0
 * (SLP_N) high via BSRR. This function only ever SETS the bit, never
 * clears it, so calling it more than once is always safe.
 *
 * Why call it twice (once in main() before MX_GPIO_Init(), once right
 * after): CubeMX's generated MX_GPIO_Init() explicitly writes PB0 LOW
 * (HAL_GPIO_WritePin(..., GPIO_PIN_RESET)) immediately before switching the
 * pin to output mode. That overwrites anything you set before it runs. The
 * call AFTER MX_GPIO_Init() is the one that actually sticks.
 * =======================================================================*/
void LIN_WakeTransceiver(void)
{
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN;   /* GPIOB clock on */
    GPIOB->BSRR = GPIO_PIN_0;               /* PB0 = SLP_N -> HIGH */
}

/* =========================================================================
 * SECTION 2: Low-level byte I/O (polled, with real timeouts)
 * =======================================================================*/
static LIN_Status lin_putc(uint8_t b)
{
    uint32_t t0 = HAL_GetTick();
    while (!(huart1.Instance->SR & USART_SR_TXE)) {
        if ((HAL_GetTick() - t0) > LIN_TX_TIMEOUT_MS) return LIN_ERR_TX_TIMEOUT;
    }
    huart1.Instance->DR = b;
    return LIN_OK;
}

static LIN_Status lin_getc(uint8_t *b, uint32_t timeout_ms)
{
    uint32_t t0 = HAL_GetTick();
    while (!(huart1.Instance->SR & USART_SR_RXNE)) {
        if ((HAL_GetTick() - t0) > timeout_ms) return LIN_ERR_RX_TIMEOUT;
    }
    *b = (uint8_t)huart1.Instance->DR;   /* reading SR then DR also clears error flags */
    return LIN_OK;
}

static void lin_flush_rx(void)
{
    while (huart1.Instance->SR & (USART_SR_RXNE | USART_SR_ORE)) {
        (void)huart1.Instance->DR;
    }
}

/* =========================================================================
 * SECTION 3: Protocol math (unchanged)
 * =======================================================================*/
static uint8_t LIN_PID(uint8_t id)
{
    id &= 0x3F;
    uint8_t b0=(id>>0)&1, b1=(id>>1)&1, b2=(id>>2)&1;
    uint8_t b3=(id>>3)&1, b4=(id>>4)&1, b5=(id>>5)&1;
    uint8_t p0 = b0 ^ b1 ^ b2 ^ b4;
    uint8_t p1 = !(b1 ^ b3 ^ b4 ^ b5);
    return id | (p0 << 6) | (p1 << 7);
}

/* Enhanced checksum (LIN 2.x): includes the PID. Confirmed matching what
 * PLIN-View Pro already shows ("Checksum Type: Enhanced") for your frames. */
static uint8_t LIN_Checksum(uint8_t pid, const uint8_t *data, uint8_t len)
{
    uint16_t sum = pid;
    for (uint8_t i = 0; i < len; i++) {
        sum += data[i];
        if (sum > 0xFF) sum -= 0xFF;
    }
    return (uint8_t)~sum;
}

/* Sends break + sync + PID. Every publish and every request starts here. */
static LIN_Status lin_send_header(uint8_t pid)
{
    lin_flush_rx();

    HAL_LIN_SendBreak(&huart1);
    uint32_t t0 = HAL_GetTick();
    while (huart1.Instance->CR1 & USART_CR1_SBK) {
        if ((HAL_GetTick() - t0) > LIN_TX_TIMEOUT_MS) return LIN_ERR_BREAK;
    }

    LIN_Status s;
    if ((s = lin_putc(0x55)) != LIN_OK) return s;   /* sync */
    if ((s = lin_putc(pid))  != LIN_OK) return s;   /* protected ID */
    return LIN_OK;
}

/* =========================================================================
 * SECTION 4: Public single-frame operations
 * =======================================================================*/

/* Master PUBLISHES data: header, then data bytes, then checksum.
 * This is the direction you want for sensor data -- your STM32 HAS the
 * reading, so it just announces it on the bus. PLIN-View Pro (configured
 * as Subscriber Automatic, like your other frame IDs already are) picks
 * it up and displays it in the Receive table with no extra setup on its
 * side. */
LIN_Status LIN_SendFrame(uint8_t id, const uint8_t *data, uint8_t len)
{
    if (id > 0x3F || len == 0 || len > LIN_MAX_DATA_LEN) return LIN_ERR_PARAM;

    uint8_t pid = LIN_PID(id);
    LIN_Status s = lin_send_header(pid);
    if (s != LIN_OK) return s;

    for (uint8_t i = 0; i < len; i++) {
        if ((s = lin_putc(data[i])) != LIN_OK) return s;
    }
    uint8_t ck = LIN_Checksum(pid, data, len);
    return lin_putc(ck);
}

/* Master REQUESTS data: header only, then a slave is supposed to answer.
 * Kept for the case where you later add a real second node on the bus
 * that holds data your STM32 doesn't have locally. Not needed for
 * reading your own sensors. */
LIN_Status LIN_MasterRequest(uint8_t id, uint8_t *data, uint8_t len)
{
    if (id > 0x3F || len == 0 || len > LIN_MAX_DATA_LEN) return LIN_ERR_PARAM;

    uint8_t pid = LIN_PID(id);
    LIN_Status s = lin_send_header(pid);
    if (s != LIN_OK) return s;

    uint8_t rx[LIN_MAX_DATA_LEN + 1];   /* data bytes + checksum */
    for (uint8_t i = 0; i <= len; i++) {
        s = lin_getc(&rx[i], LIN_RX_TIMEOUT_MS);
        if (s != LIN_OK) {
            return (i == 0) ? LIN_ERR_NO_RESPONSE : LIN_ERR_RX_TIMEOUT;
        }
    }

    if (rx[len] != LIN_Checksum(pid, rx, len)) return LIN_ERR_CHECKSUM;

    for (uint8_t i = 0; i < len; i++) data[i] = rx[i];
    return LIN_OK;
}

/* =========================================================================
 * SECTION 5: Scheduler
 *
 * NEW: before sending a PUBLISH slot, if it has a "source" callback set,
 * the scheduler calls it to refresh slot->data with a live reading. This
 * is the actual sensor-integration point -- everything else in this file
 * is unchanged plumbing around it.
 * =======================================================================*/
static LIN_ScheduleSlot *s_slots = 0;
static uint8_t s_slot_count = 0;
static uint8_t s_slot_index = 0;

void LIN_Schedule_Init(LIN_ScheduleSlot *slots, uint8_t count)
{
    s_slots = slots;
    s_slot_count = count;
    s_slot_index = 0;
}

void LIN_Schedule_Step(void)
{
    if (s_slots == 0 || s_slot_count == 0) return;   /* not initialised */

    LIN_ScheduleSlot *slot = &s_slots[s_slot_index];

    if (slot->type == LIN_SLOT_PUBLISH) {
        /* Pull a fresh sensor reading right before sending it, if this
         * slot has a source attached. A NULL source just sends whatever
         * is already sitting in slot->data (fine for fixed/test frames). */
        if (slot->source != 0) {
            slot->source(slot->data, slot->len);
        }
        LIN_SendFrame(slot->id, slot->data, slot->len);
    } else {
        LIN_MasterRequest(slot->id, slot->data, slot->len);
        /* result lands back in slot->data regardless of success; on
         * failure the old data from last time is left untouched, so a
         * one-off dropped frame doesn't erase the last good reading */
    }

    s_slot_index = (uint8_t)((s_slot_index + 1) % s_slot_count);
}

/* =========================================================================
 * SECTION 6: Modular payload packing
 *
 * Writes "value" into "data" as "len" big-endian bytes. Big-endian (most
 * significant byte first) is chosen so that whatever reads this frame on
 * the bus -- a human eyeballing hex in PLIN-View Pro, or another MCU --
 * sees the bytes in the same left-to-right order as the number itself.
 *
 * This loop works for any len from 1 to 4 without being told which one in
 * advance: "len - 1 - i" picks off the right byte of "value" each time,
 * starting from the most significant end. If len is smaller than the
 * value actually needs, the high bits are simply dropped (truncation) --
 * same as casting a uint32_t down to a smaller type.
 * =======================================================================*/
void LIN_PackUint32(uint8_t *data, uint8_t len, uint32_t value)
{
    for (uint8_t i = 0; i < len; i++) {
        data[i] = (uint8_t)(value >> (8 * (len - 1 - i)));
    }
}

/* =========================================================================
 * SECTION 7: Status text, for debug prints
 * =======================================================================*/
const char *LIN_StatusStr(LIN_Status s)
{
    switch (s) {
    case LIN_OK:              return "OK";
    case LIN_ERR_PARAM:       return "bad id/length";
    case LIN_ERR_BREAK:       return "break never finished";
    case LIN_ERR_TX_TIMEOUT:  return "TX timeout";
    case LIN_ERR_NO_RESPONSE: return "no slave response";
    case LIN_ERR_RX_TIMEOUT:  return "slave stalled mid-response";
    case LIN_ERR_CHECKSUM:    return "bad checksum";
    default:                  return "?";
    }
}
