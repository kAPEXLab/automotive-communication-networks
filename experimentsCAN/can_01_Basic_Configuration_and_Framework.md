# CAN Framework for STM32F446RE + SN65HVD230 + PCAN

## Objective

This framework hides HAL CAN complexity from students.

Students should focus on:

- CAN Message ID
- DLC
- Data Bytes
- Signal Encoding
- Arbitration
- Filtering
- Bus Load
- Diagnostics

Students should **not spend time on**:

- CAN Filters
- Mailboxes
- GPIO Alternate Functions
- Bit Timing Calculations
- HAL Internals

---

# Hardware Required

## Node

- NUCLEO-F446RE
- SN65HVD230 CAN Transceiver

## Analyzer

- PCAN-USB
- PCAN-View

---

# Hardware Connections

## STM32 ↔ SN65HVD230

```text
PB9  -> CTX
PB8  -> CRX
3.3V -> VCC
GND  -> GND
```

## SN65HVD230 ↔ PCAN

```text
CANH -> CANH
CANL -> CANL
GND  -> GND
```

## PCAN DB9 Pins

```text
Pin 7 -> CANH
Pin 2 -> CANL
Pin 3 -> GND
```

---

# STM32CubeMX Configuration

## Board Selection

```text
NUCLEO-F446RE
```

---

## CAN1 Configuration

```text
Connectivity
    └── CAN1
            Mode = Normal/Activated
```

---

## Pin Assignment

```text
PB8 = CAN1_RX
PB9 = CAN1_TX
```

---

## Clock Configuration

Verify:

```text
PCLK1 = 42 MHz
```

---

## CAN Bit Timing (500 kbps) in Parameter Settings

```text
Prescaler = 7
SJW        = 1 TQ
BS1        = 9 TQ
BS2        = 2 TQ
```

---

## CAN Parameters

```text
Time Triggered Mode     = Disable
Auto Bus-Off            = Disable
Auto Wake-Up            = Disable
Auto Retransmission     = Enable
Receive FIFO Locked     = Disable
Transmit FIFO Priority  = Disable
```

---

# Project Structure

```text
Core
│
├── Inc
│   └── can_manager.h
│
├── Src
│   └── can_manager.c
│
└── main.c
```

---

# File: can_manager.h

```c
#ifndef CAN_MANAGER_H
#define CAN_MANAGER_H

#include "main.h"

void CAN_InitFramework(void);

void CAN_SendMessage(uint16_t id,
                     uint8_t *data,
                     uint8_t len);

#endif
```

---

# File: can_manager.c

```c
#include "can_manager.h"

extern CAN_HandleTypeDef hcan1;

static CAN_TxHeaderTypeDef txHeader;
static uint32_t txMailbox;

void CAN_InitFramework(void)
{
    CAN_FilterTypeDef filter;

    filter.FilterBank = 0;
    filter.FilterMode = CAN_FILTERMODE_IDMASK;
    filter.FilterScale = CAN_FILTERSCALE_32BIT;

    filter.FilterIdHigh = 0;
    filter.FilterIdLow = 0;

    filter.FilterMaskIdHigh = 0;
    filter.FilterMaskIdLow = 0;

    filter.FilterFIFOAssignment = CAN_FILTER_FIFO0;
    filter.FilterActivation = ENABLE;
    filter.SlaveStartFilterBank = 14;

    if (HAL_CAN_ConfigFilter(&hcan1, &filter) != HAL_OK)
    {
        Error_Handler();
    }

    if (HAL_CAN_Start(&hcan1) != HAL_OK)
    {
        Error_Handler();
    }
}

void CAN_SendMessage(uint16_t id,
                     uint8_t *data,
                     uint8_t len)
{
    txHeader.StdId = id;
    txHeader.ExtId = 0;

    txHeader.IDE = CAN_ID_STD;
    txHeader.RTR = CAN_RTR_DATA;

    txHeader.DLC = len;

    txHeader.TransmitGlobalTime = DISABLE;

    HAL_CAN_AddTxMessage(&hcan1,
                         &txHeader,
                         data,
                         &txMailbox);
}
```

---

# Modifications in main.c

## Add Include

```c
#include "can_manager.h"
```

---

## Initialize Framework

In main Add:

```c
CAN_InitFramework();
```

---

# Example 1: Fixed CAN Message

```c

    uint8_t msg[8] =
    {
        0x11,
        0x22,
        0x33,
        0x44,
        0x55,
        0x66,
        0x77,
        0x88
    };

while (1)
{
    CAN_SendMessage(0x100,
                    msg,
                    8);

    HAL_Delay(1000);
}
```

---

# Example 2: Vehicle Speed

```c
uint8_t speed = 0;
uint8_t msg[8] = {0};

while (1)
{
    msg[0] = speed++;

    CAN_SendMessage(0x100,
                    msg,
                    8);

    HAL_Delay(100);
}
```

### PCAN Output

```text
ID = 0x100

00
01
02
03
04
...
```

---

# Example 3: Dashboard Message

```c
uint8_t speed = 60;
uint8_t gear = 3;
uint8_t temperature = 90;
uint8_t fuel = 75;
uint8_t msg[8] = {0};

msg[0] = speed;
msg[1] = gear;
msg[2] = temperature;
msg[3] = fuel;

while(1)
    CAN_SendMessage(0x100,
                    msg,
                    8);

    HAL_Delay(100);
}
```

---

# Example 4: Engine RPM (True Multi-byte Signal)

```c
uint16_t rpm = 2500;
uint8_t msg[8] = {0};

while(1)
{
    msg[0] = rpm & 0xFF;
    msg[1] = rpm >> 8;

    CAN_SendMessage(0x101,
                    msg,
                    8);

    HAL_Delay(100);
}
```

---
