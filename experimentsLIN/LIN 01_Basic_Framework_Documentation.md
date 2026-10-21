# LIN Framework for STM32F446RE + TJA1021T2C + PCAN

## Objective

This framework hides LIN protocol byte-level detail (break timing, parity bits, checksum math)
behind simple function calls, the same way the CAN framework hides HAL CAN mailbox/filter
detail.

Students should focus on:

- Frame ID
- Payload length and data bytes
- Master vs. Slave role
- Publisher vs. Subscriber direction
- Checksum type (Enhanced vs. Classic)
- Sensor-to-payload encoding
- Bus wake/sleep (SLP_N)
- Diagnostics (error codes)

Students should not spend time on:

- Break-field bit timing
- Protected ID parity bit derivation
- USART register-level break detection
- HAL LIN internals

## Hardware Required

**Node**
- NUCLEO-F446RE
- NXP TJA1021T2C LIN Transceiver
- +12V power supply (for the transceiver's VBAT)
- LM35 analog temperature sensor (for the sensor demo)

**Analyzer**
- PEAK PCAN-USB Pro FD
- PLIN-View Pro

**Optional, for the two-node data-sharing demo**
- A second NUCLEO-F446RE + TJA1021T2C, wired identically, acting as a pure listener

## Hardware Connections

### STM32 <-> TJA1021T2C

```
PA9  (USART1_TX) -> TXD
PA10 (USART1_RX) <- RXD
PB0  (GPIO out)  -> SLP_N
3.3V             -> (logic side only -- TJA1021 logic I/O is 3.3V/5V tolerant)
GND              -> GND
```

### TJA1021T2C Power and Bus

```
+12V supply -> VBAT  (+ 100nF ceramic capacitor across VBAT/GND, at the chip)
GND         -> GND   (common with STM32 GND AND the 12V supply return)
LIN pin     -> LIN bus wire (single wire, shared by every node)
WAKE_N      -> tie to VBAT or leave floating
INH         -> leave open
```

### TJA1021T2C <-> PCAN-USB Pro FD

```
LIN pin -> LIN-H (PCAN LIN connector)
GND     -> GND
```

### Master-node termination (add only at ONE node on the bus)

```
+12V -> diode (anode to +12V) -> 1kOhm resistor -> LIN pin
```

## STM32CubeMX Configuration

### Board Selection

```
NUCLEO-F446RE
```

### USART1 Configuration

```
Connectivity
    |-- USART1
            Mode = LIN            (NOT "Asynchronous")
```

### Pin Assignment

```
PA9  = USART1_TX
PA10 = USART1_RX
PB0  = GPIO_Output   (used as SLP_N, assigned manually in Pinout view)
```

### Parameter Settings (USART1)

```
Baud Rate    = 19200
Word Length  = 8 Bits
Stop Bits    = 1
Parity       = None
Break Detect = 11-bit (CubeMX default for LIN mode)
```

> **Takeaway:** CubeMX only generates GPIO init code for pins you explicitly assign a mode to in
> the Pinout view. If PB0 is never set to `GPIO_Output` there, `MX_GPIO_Init()` silently skips
> it entirely, and any code that writes to PB0 has no electrical effect at all -- the pin stays
> in its reset state (floating input). Always confirm PB0 appears inside `MX_GPIO_Init()` after
> generating code.

### ADC Configuration (only needed for the LM35 sensor demo)

```
Analog
    |-- ADC1
            IN1 (PA1)         = enabled
Resolution            = 12 bits
Data Alignment        = Right
Continuous Conversion = Disabled
Sampling Time         = 144 Cycles
```

## Project Structure

```
Core
|
|-- Inc
|   |-- LIN.h
|   |-- LIN_Slave.h        (only on the second/listener board)
|
|-- Src
|   |-- LIN.c
|   |-- LIN_Slave.c        (only on the second/listener board)
|
|-- main.c
```

## File: `LIN.h`  (master board -- publish and request)

```c
/*
 * LIN.h -- STM32F446RE + NXP TJA1021T2C LIN master driver (USART1)
 *
 * This version adds a "data source" hook to publish slots, so the
 * scheduler can pull a fresh sensor reading right before each frame is
 * sent instead of always sending the same fixed bytes.
 *
 * NOTE ON DIRECTION: your STM32 is the LIN MASTER (it sends the headers).
 * Getting sensor data INTO PLIN-View Pro only needs LIN_SendFrame()
 * (publish) -- the same thing your working 01h frame already does. You do
 * NOT need LIN_MasterRequest() / a slave response for this. Request/
 * response is only for when some OTHER node on the bus holds data your
 * STM32 needs to ask for. Your sensor is attached to your own STM32, so
 * publishing it is both simpler and the correct direction.
 *
 * CubeMX setup is unchanged from before:
 *   USART1 Mode = LIN, Baud = 19200, PB0 = GPIO_Output (SLP_N)
 */
#ifndef LIN_H
#define LIN_H

#include <stdint.h>

/* ---------- Tunables: change these to match your bus ---------- */
#define LIN_MAX_DATA_LEN   8U     /* LIN hard limit per frame             */
#define LIN_TX_TIMEOUT_MS  10U    /* per-byte transmit timeout             */
#define LIN_RX_TIMEOUT_MS  15U    /* per-byte response timeout (request)   */

/* ---------- Result of every LIN operation ---------- */
typedef enum {
    LIN_OK = 0,
    LIN_ERR_PARAM,         /* bad id (>0x3F) or bad length (0 or >8)      */
    LIN_ERR_BREAK,         /* break never finished -> clock/USART problem */
    LIN_ERR_TX_TIMEOUT,    /* a byte never left the transmit register     */
    LIN_ERR_NO_RESPONSE,   /* MasterRequest: slave never answered at all  */
    LIN_ERR_RX_TIMEOUT,    /* MasterRequest: slave answered, then stalled */
    LIN_ERR_CHECKSUM       /* MasterRequest: response checksum is wrong   */
} LIN_Status;

/* ---------- One entry in the round-robin schedule ---------- */
typedef enum {
    LIN_SLOT_PUBLISH,      /* master sends data (like LIN_SendFrame)      */
    LIN_SLOT_REQUEST       /* master asks a slave to send data            */
} LIN_SlotType;

/* A data source fills "data" (exactly "len" bytes) with a fresh reading
 * right before a PUBLISH slot is sent. This is where you plug in your
 * sensor: an ADC read, an I2C read from the MPU6050, whatever you have.
 * Set to NULL on a slot to just keep sending whatever is already in
 * slot->data (useful for REQUEST slots, or a publish with fixed/test
 * bytes that never changes). */
typedef void (*LIN_DataSource)(uint8_t *data, uint8_t len);

typedef struct {
    uint8_t        id;          /* 6-bit LIN frame ID                       */
    uint8_t        len;         /* data length, 1-8                         */
    LIN_SlotType   type;        /* publish or request                       */
    LIN_DataSource source;      /* publish only: called to refresh data     */
                                  /* before sending. NULL = send as-is.      */
    uint8_t        data[LIN_MAX_DATA_LEN]; /* publish: payload (fixed, or
                                             refreshed by "source" above).
                                             request: last response lands
                                             here after a successful read. */
} LIN_ScheduleSlot;

/* ---------- Setup ---------- */
/* Call TWICE: once before MX_GPIO_Init() (harmless), once right after it
 * (this is the call that actually matters -- see LIN.c for why). */
void LIN_WakeTransceiver(void);

/* ---------- Single-frame operations ---------- */
LIN_Status LIN_SendFrame(uint8_t id, const uint8_t *data, uint8_t len);
LIN_Status LIN_MasterRequest(uint8_t id, uint8_t *data, uint8_t len);

/* ---------- Scheduler: cycle through several frames automatically ---------- */
void LIN_Schedule_Init(LIN_ScheduleSlot *slots, uint8_t count);
void LIN_Schedule_Step(void);   /* call this once per loop iteration */

/* ---------- Helpers ---------- */
const char *LIN_StatusStr(LIN_Status s);

/* Packs "value" into exactly "len" bytes, big-endian (most significant
 * byte first), zero-padded on the left if the value is smaller than what
 * "len" bytes can hold. This is the piece that makes payload length
 * MODULAR: your sensor-read function stops hardcoding "2 bytes" and just
 * asks for however many bytes the slot says to use.
 *
 *   len=1 -> value truncated to 0-255        (coarse, e.g. a percentage)
 *   len=2 -> value truncated to 0-65535       (full 12-bit ADC fits easily)
 *   len=3 -> value truncated to 0-16777215    (room for e.g. microvolts)
 *   len=4 -> value truncated to 0-4294967295  (full 32-bit precision)
 *
 * Change a slot's .len in main.c and your ReadSensor() function below
 * keeps working without modification -- it just ends up sending more or
 * fewer bytes of the same underlying reading. */
void LIN_PackUint32(uint8_t *data, uint8_t len, uint32_t value);

#endif /* LIN_H */

```

## File: `LIN.c`  (master board -- publish and request)

```c
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

```

## File: `LIN_Slave.h`  (second/listener board only)

```c
/*
 * LIN_Slave.h -- passive LIN listener for a SECOND STM32F446RE board
 *
 * This is for a DIFFERENT physical board than the one running LIN.c. Where
 * LIN.c is the MASTER (it sends headers, drives the bus timing), this file
 * is a pure SLAVE: it never sends a header, it only watches the bus for a
 * specific frame ID and reads out its data when it sees one.
 *
 * This is what turns the project from "one board talks to a PC" into
 * "one board's sensor reading actually reaches a second, independent
 * board" -- real LIN data sharing between two nodes, no PC software
 * involved in the data path at all.
 *
 * Hardware/CubeMX setup is IDENTICAL to the master board:
 *   USART1 Mode = LIN, Baud = 19200, PB0 = GPIO_Output (SLP_N)
 * Both boards' TJA1021 LIN pins connect to the SAME physical bus wire.
 * Both boards' SLP_N must be driven HIGH -- a sleeping transceiver can't
 * pass bus traffic through to its RXD pin either, so "just receiving"
 * does NOT mean you can skip waking the transceiver.
 */
#ifndef LIN_SLAVE_H
#define LIN_SLAVE_H

#include <stdint.h>

#define LIN_SLAVE_MAX_DATA_LEN  8U

typedef enum {
    LIN_SLAVE_OK = 0,
    LIN_SLAVE_NO_FRAME,      /* nothing seen before the timeout expired      */
    LIN_SLAVE_NOT_FOR_US,    /* a frame was seen, but for a different ID     */
    LIN_SLAVE_RX_TIMEOUT,    /* header matched, but data bytes never arrived */
    LIN_SLAVE_CHECKSUM       /* data arrived, but checksum didn't validate   */
} LIN_SlaveStatus;

/* Same register-level wake trick as the master board. Call TWICE: once
 * before MX_GPIO_Init(), once right after -- see LIN_Slave.c for why. */
void LIN_Slave_WakeTransceiver(void);

/* Blocks until EITHER a valid, matching frame arrives OR timeout_ms passes.
 *   expected_id - the 6-bit ID you're listening for (must match the
 *                 publisher's ID on the other board, e.g. 0x01)
 *   data        - filled with the frame's payload on LIN_SLAVE_OK
 *   len         - how many data bytes to expect for this ID
 *   timeout_ms  - give up and return LIN_SLAVE_NO_FRAME after this long
 *
 * Call this in a loop in main() -- each call either catches one frame or
 * times out, so you can mix it with other work (blinking an LED, checking
 * a button) between calls. */
LIN_SlaveStatus LIN_Slave_Receive(uint8_t expected_id, uint8_t *data,
                                   uint8_t len, uint32_t timeout_ms);

const char *LIN_Slave_StatusStr(LIN_SlaveStatus s);

#endif /* LIN_SLAVE_H */

```

## File: `LIN_Slave.c`  (second/listener board only)

```c
/*
 * LIN_Slave.c -- passive LIN listener for a SECOND STM32F446RE board
 *
 * Unlike LIN.c (the master), this file never calls HAL_LIN_SendBreak() --
 * it only watches for a break arriving FROM the bus, then reads whatever
 * header and data follow it.
 */
#include "main.h"
#include "LIN_Slave.h"

extern UART_HandleTypeDef huart1;

/* =========================================================================
 * SECTION 1: Wake the transceiver -- identical to the master board.
 *
 * Still required even though this board only listens: SLP_N controls
 * whether the TJA1021 passes bus activity through to its RXD pin at all.
 * Asleep, it hears nothing, same as a sleeping person not reacting to
 * sound. "Receive-only" does not mean "skip the wake sequence."
 * =======================================================================*/
void LIN_Slave_WakeTransceiver(void)
{
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN;
    GPIOB->BSRR = GPIO_PIN_0;   /* PB0 = SLP_N -> HIGH */
}

/* =========================================================================
 * SECTION 2: Low-level byte read, with timeout (same pattern as LIN.c)
 * =======================================================================*/
static LIN_SlaveStatus lin_getc(uint8_t *b, uint32_t timeout_ms)
{
    uint32_t t0 = HAL_GetTick();
    while (!(huart1.Instance->SR & USART_SR_RXNE)) {
        if ((HAL_GetTick() - t0) > timeout_ms) return LIN_SLAVE_RX_TIMEOUT;
    }
    *b = (uint8_t)huart1.Instance->DR;
    return LIN_SLAVE_OK;
}

/* =========================================================================
 * SECTION 3: Protocol math -- same formulas as the master, duplicated here
 * on purpose so this file can be dropped into a second project on its own,
 * without needing to share LIN.c/.h between two separate STM32CubeIDE
 * projects.
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

static uint8_t LIN_Checksum(uint8_t pid, const uint8_t *data, uint8_t len)
{
    uint16_t sum = pid;
    for (uint8_t i = 0; i < len; i++) {
        sum += data[i];
        if (sum > 0xFF) sum -= 0xFF;
    }
    return (uint8_t)~sum;
}

/* =========================================================================
 * SECTION 4: The actual listener
 *
 * USART's LBD (LIN Break Detection) flag is hardware support for exactly
 * this job -- it sets itself the instant an 11-bit (or longer) dominant
 * period is seen on RX, which is precisely what a LIN break looks like.
 * We don't have to bit-bang break detection ourselves; the peripheral,
 * already in LIN mode from HAL_LIN_Init(), does it for us.
 * =======================================================================*/
LIN_SlaveStatus LIN_Slave_Receive(uint8_t expected_id, uint8_t *data,
                                   uint8_t len, uint32_t timeout_ms)
{
    uint8_t expected_pid = LIN_PID(expected_id);
    uint32_t t0 = HAL_GetTick();

    while ((HAL_GetTick() - t0) < timeout_ms) {

        if (huart1.Instance->SR & USART_SR_LBD) {
            __HAL_UART_CLEAR_LBDFLAG(&huart1);   /* acknowledge the break */

            uint8_t sync, pid;

            /* A few ms is plenty -- sync and PID follow the break almost
             * immediately on a correctly-timed bus. */
            if (lin_getc(&sync, 5) != LIN_SLAVE_OK) continue;
            if (lin_getc(&pid,  5) != LIN_SLAVE_OK) continue;

            if (pid != expected_pid) {
                /* Some OTHER frame ID went by -- not an error, just not
                 * ours. Go back to waiting for the next break rather than
                 * trying to read data bytes we don't know the length of. */
                continue;
            }

            uint8_t rx[LIN_SLAVE_MAX_DATA_LEN + 1];
            for (uint8_t i = 0; i <= len; i++) {
                if (lin_getc(&rx[i], 10) != LIN_SLAVE_OK) return LIN_SLAVE_RX_TIMEOUT;
            }

            if (rx[len] != LIN_Checksum(pid, rx, len)) return LIN_SLAVE_CHECKSUM;

            for (uint8_t i = 0; i < len; i++) data[i] = rx[i];
            return LIN_SLAVE_OK;
        }
    }

    return LIN_SLAVE_NO_FRAME;
}

const char *LIN_Slave_StatusStr(LIN_SlaveStatus s)
{
    switch (s) {
    case LIN_SLAVE_OK:         return "OK";
    case LIN_SLAVE_NO_FRAME:   return "no frame seen";
    case LIN_SLAVE_NOT_FOR_US: return "frame was for a different ID";
    case LIN_SLAVE_RX_TIMEOUT: return "header matched, data never arrived";
    case LIN_SLAVE_CHECKSUM:   return "bad checksum";
    default:                   return "?";
    }
}

```

## Modifications in `main.c` (Master Board)

### Add Include

```c
#include "LIN.h"
```

### Wake the Transceiver (TWICE -- see takeaway below)

```c
int main(void)
{
  HAL_Init();

  /* USER CODE BEGIN 1 */
  LIN_WakeTransceiver();          /* 1st call: before MX_GPIO_Init() */
  /* USER CODE END 1 */

  SystemClock_Config();
  MX_GPIO_Init();
  LIN_WakeTransceiver();          /* 2nd call: AFTER MX_GPIO_Init() -- this is the one that sticks */
  MX_USART1_UART_Init();
```

> **Takeaway:** CubeMX's generated `MX_GPIO_Init()` explicitly drives every configured output pin
> LOW immediately before switching it to output mode. Any earlier HIGH write to that pin is
> silently overwritten. Calling the wake function a second time, after `MX_GPIO_Init()`, is what
> actually leaves SLP_N driven high.

## Example 1: Fixed LIN Message (Publish)

```c
uint8_t msg[2] = {0x11, 0x22};

while (1)
{
    LIN_SendFrame(0x01, msg, 2);
    HAL_Delay(100);
}
```

**PLIN-View Pro output**

```
ID = 01h
Data = 11 22
Checksum Type = Enhanced
```

## Example 2: Incrementing Counter

```c
uint8_t counter = 0;
uint8_t msg[2] = {0};

while (1)
{
    msg[0] = counter++;
    msg[1] = 0x55;

    LIN_SendFrame(0x01, msg, 2);
    HAL_Delay(100);
}
```

## Example 3: Master Requests Data From a Slave

```c
uint8_t response[2];
LIN_Status st = LIN_MasterRequest(0x02, response, 2);

if (st == LIN_OK)
{
    /* response[0], response[1] now hold the slave's answer */
}
else
{
    /* st tells you exactly why: LIN_ERR_NO_RESPONSE, LIN_ERR_RX_TIMEOUT,
       or LIN_ERR_CHECKSUM -- see LIN_StatusStr(st) for a human-readable
       string to print over your debug UART */
}
```

> **Takeaway:** `LIN_MasterRequest()` is only the right tool when some OTHER node on the bus
> holds the data. If the data originates on your own board (a sensor wired directly to it),
> use `LIN_SendFrame()` (publish) instead -- that is the simpler and correct direction.

## Example 4: Real Sensor Payload (LM35 Temperature, Modular Length)

```c
#define SENSOR_FRAME_LEN  2    /* change ONLY this to resize the payload */

static void ReadSensor(uint8_t *data, uint8_t len)
{
    HAL_ADC_Start(&hadc1);
    HAL_ADC_PollForConversion(&hadc1, 10);
    uint32_t raw = HAL_ADC_GetValue(&hadc1);        /* 0-4095 */

    /* raw ADC count -> millivolts -> hundredths of a degree C
       LM35: 10mV = 1C, so (mV * 10) = centi-degrees C */
    uint32_t temp_centiC = (raw * 33000UL) / 4095UL;

    LIN_PackUint32(data, len, temp_centiC);
}

static LIN_ScheduleSlot schedule[] = {
    { .id = 0x01, .len = SENSOR_FRAME_LEN, .type = LIN_SLOT_PUBLISH,
      .source = ReadSensor, .data = {0} },
};

int main(void)
{
  /* ... wake + init as shown above ... */

  LIN_Schedule_Init(schedule, 1);

  while (1)
  {
    LIN_Schedule_Step();
    HAL_Delay(50);
  }
}
```

**PLIN-View Pro output (approximately 25C room temperature)**

```
ID = 01h
Data = 09 C4     (2500 decimal = 25.00 C)
Checksum Type = Enhanced
Count = (climbing every 50ms)
```

> **Takeaway:** A sensor reading pinned at `0F FF` / `0F FC` (near the ADC's 12-bit maximum,
> 4095) almost always means the ADC was never initialized, or the sensor's ground is not common
> with the STM32's ground -- not that the sensor is genuinely reporting an extreme value. Verify
> with a jumper wire shorting the ADC input directly to GND: the reading should drop to near
> zero if the ADC and firmware are both working correctly.

## Example 5: Two-Board Data Sharing (Master publishes, Slave board reacts)

**Master board** -- same as Example 4 (publishes the LM35 reading as ID `0x01`).

**Second board** (`main.c`, using `LIN_Slave.h`/`LIN_Slave.c`):

```c
#include "LIN_Slave.h"

int main(void)
{
  HAL_Init();

  /* USER CODE BEGIN 1 */
  LIN_Slave_WakeTransceiver();
  /* USER CODE END 1 */

  SystemClock_Config();
  MX_GPIO_Init();
  LIN_Slave_WakeTransceiver();
  MX_USART1_UART_Init();

  while (1)
  {
    uint8_t temp_bytes[2];
    LIN_SlaveStatus st = LIN_Slave_Receive(0x01, temp_bytes, 2, 200);

    if (st == LIN_SLAVE_OK)
    {
        uint16_t temp_centiC = (temp_bytes[0] << 8) | temp_bytes[1];

        if (temp_centiC > 3000)   /* > 30.00 C */
            HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_SET);
        else
            HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_RESET);
    }
  }
}
```

> **Takeaway:** This is what makes the demonstration real "data sharing" rather than just
> "STM32 talks to a PC." Board A's physical sensor reading changes Board B's physical LED
> state, entirely over the LIN bus, with no PC software anywhere in that data path. Both
> boards' TJA1021 LIN pins must be tied to the same physical wire, with termination at only
> ONE node (the master).

## Key Concept Takeaways

- **LIN has exactly one master.** Only the master ever sends a header (break + sync + PID).
  Slaves never initiate anything.
- **Publisher vs. Subscriber is per-frame-ID, not per-node.** A single node can publish some
  IDs and subscribe to others.
- **Enhanced checksum includes the PID; classic checksum does not.** LIN 2.x frames use
  Enhanced. Only the two reserved diagnostic IDs (`0x3C`, `0x3D`) use Classic.
- **SLP_N must be driven HIGH on every node**, publisher or listener, master or slave --
  a sleeping transceiver passes no traffic in either direction.
- **A reading pinned at a hardware extreme (0 or max) is a wiring/init signature**, not a
  sensor behavior -- check ground continuity and peripheral initialization first.
- **Exactly one publisher per frame ID.** Two sources answering the same ID produces a bus
  fault (`OtherResponse` in PLIN-View Pro), not just confusing data.
- **Payload length should be modular, not hardcoded**, so changing a sensor's resolution is a
  one-line change (`LIN_PackUint32`) rather than a rewrite of the packing logic.

---

# Student Exercise Pack: Integration Notes

These exercises complement the supplied `LIN.h`, `LIN.c`, and `main.c`; they do not replace the driver.

## Where code belongs in STM32CubeIDE

- `Core/Inc/LIN.h`: complete header file.
- `Core/Src/LIN.c`: complete driver source file.
- `Core/Src/main.c`: retain CubeMX initialization and add snippets only in the marked `USER CODE` sections.
- Add `#include "LIN.h"` under `/* USER CODE BEGIN Includes */`.
- Put helper functions under `/* USER CODE BEGIN 0 */`, global variables and schedule arrays under `/* USER CODE BEGIN PV */`, setup calls under `/* USER CODE BEGIN 2 */`, and loop statements inside the existing `while (1)` under `/* USER CODE BEGIN 3 */`.
- Never paste a second `main()` into an existing CubeMX `main.c`.

## Important limits of the supplied code

1. STM32 is the LIN master and sends each header. `LIN_SendFrame()` publishes data; PLIN-View Pro must observe/subscribe to the matching ID.
2. `LIN_Schedule_Step()` advances one schedule slot per call. `HAL_Delay()` controls loop pacing approximately; it is not a precision schedule timer.
3. `LIN_PackUint32()` encodes multi-byte values big-endian (most significant byte first).
4. Current `LIN.c` always uses enhanced checksum. Do not use it as-is for diagnostic IDs `0x3C` and `0x3D`, which require classic checksum.
5. `LIN_MasterRequest()` waits for a slave response. It is not needed to publish a sensor attached to the master.
6. The ADC example requires ADC1 configured in CubeMX and a generated `hadc1` handle.

Suggested sequence: fixed frame -> signal encoding -> scheduling -> alive counter -> fault diagnosis -> subscriber/master-slave discussion.
