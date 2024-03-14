//
// Created by fish on 2024/3/6.
//

#ifndef SPI_CAN_UUART_H
#define SPI_CAN_UUART_H

#include "common.h"
#include "stdio.h"

void UartInit(UART_HandleTypeDef *h);
int fputc(int ch, FILE *f);

#endif//SPI_CAN_UUART_H
