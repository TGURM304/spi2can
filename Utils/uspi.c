//
// Created by fish on 2024/3/5.
//

#include "uspi.h"
#include "common.h"
#include "uuart.h"

uint8_t spi_tx[BUFFER_SIZE], spi_rx[BUFFER_SIZE], spi_tx_tmp[BUFFER_SIZE];

extern uint8_t can_fl;
char fmt[50];

void HAL_SPI_TxRxCpltCallback(SPI_HandleTypeDef *hspi) {
  // CAN_LIM = 8
  // SPI_LIM = 100

  if(spi_tx_tmp[BUFFER_SIZE - 1] == 1) {
    // 确定数据完整之后把缓存的数据搬过来
    for(int i = 0; i < BUFFER_SIZE - 1; i++) spi_tx[i] = spi_tx_tmp[i];
    spi_tx_tmp[BUFFER_SIZE - 1] = 0;
  }

  HAL_SPI_TransmitReceive_DMA(&hspi2, spi_tx, spi_rx, BUFFER_SIZE);

  uint8_t control = spi_rx[0], len = spi_rx[1];

  uint8_t _xor = 0; // 异或校验码
  for(int i = 2; i < len + 2; i++) {
    _xor ^= spi_rx[i];
  }
  if(_xor ^ spi_rx[BUFFER_SIZE - 1] || !len || (control ^ 0 && control ^ 1)){
    // xor 校验不通过，亮红灯
    ledSet(25, 0, 0);
    HAL_UART_Transmit(&huart3, (uint8_t *)"xor error\r\n", sizeof "xor error\r\n", 0xffff);
    return;
  }

  if(control == 0) {
    // can 亮绿灯
    ledSet(0, 25, 0);
    uint8_t can_tx[8] = {0};
    for(int i = 0; i < min(len, 8); i++) can_tx[i] = spi_rx[i+2];
    CanSend(can_tx, 0x01);
    return;
  }

  if(control == 1) {
    // uart 亮蓝灯
    ledSet(0, 0, 50);
    HAL_UART_Transmit(&huart3, spi_rx+2, len, 0xffff);
    return;
  }

  // 啥也不是（可能数据错误），亮红灯
  ledSet(25, 0, 0);

  //  HAL_SPI_DMAResume(hspi);
  //  HAL_SPI_Receive_DMA(hspi, spi_rx, BUFFER_LIMIT+1);
  //  HAL_SPI_TransmitReceive_DMA(&hspi2, spi_tx, spi_rx, BUFFER_SIZE);
}