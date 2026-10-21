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
