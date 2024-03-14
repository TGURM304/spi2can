//
// Created by fish on 2024/3/2.
//

#ifndef SPI_CAN_UCAN_H
#define SPI_CAN_UCAN_H

#include "can.h"
#include "spi.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

void CanInit(CAN_HandleTypeDef *h);
void CanSend(uint8_t *data, uint8_t StdId);

#endif//SPI_CAN_UCAN_H
