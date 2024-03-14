//
// Created by fish on 2024/3/6.
//

#include "uuart.h"

UART_HandleTypeDef *UARTh;
void UartInit(UART_HandleTypeDef *h) {
  UARTh = h;
}

// printf 重定向
int fputc(int ch, FILE *f) {
  HAL_UART_Transmit(UARTh, (uint8_t *)&ch, 1, 0xffff);
  return ch;
}