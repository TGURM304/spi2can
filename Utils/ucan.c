//
// Created by fish on 2024/3/2.
//

#include "ucan.h"
#include "common.h"

void CanInit(CAN_HandleTypeDef *h) {
  CAN_FilterTypeDef can_filter_st;
  can_filter_st.FilterActivation = CAN_FILTER_ENABLE;
  can_filter_st.FilterBank = 0;
  can_filter_st.FilterMode = CAN_FILTERMODE_IDMASK;
  can_filter_st.FilterScale = CAN_FILTERSCALE_32BIT;
  can_filter_st.FilterFIFOAssignment = CAN_FilterFIFO0;
  can_filter_st.FilterIdHigh = 0x0000;
  can_filter_st.FilterIdLow = 0x0000;
  can_filter_st.FilterMaskIdHigh = 0x0000;
  can_filter_st.FilterMaskIdLow = 0x0000;
  can_filter_st.SlaveStartFilterBank = 14;
  HAL_CAN_ConfigFilter(h, &can_filter_st);
  HAL_CAN_Start(h);
  HAL_CAN_ActivateNotification(h, CAN_IT_RX_FIFO0_MSG_PENDING);
}

void CanSend(uint8_t *data, uint8_t StdId) {
  CAN_TxHeaderTypeDef can_tx_message;
  uint32_t send_mail_box;
  can_tx_message.StdId = StdId;
  can_tx_message.IDE = CAN_ID_STD;
  can_tx_message.RTR = CAN_RTR_DATA;
  can_tx_message.DLC = 0x08;
  HAL_CAN_AddTxMessage(&hcan, &can_tx_message, data, &send_mail_box);
}

uint8_t can_rx[8], can_fl;
extern uint8_t spi_tx[BUFFER_SIZE], spi_tx_tmp[BUFFER_SIZE];

uint8_t GetStartPos(uint8_t StdId) {
  switch(StdId) {
    case 0x01:
      return 0;
    case 0x02:
      return 8;
    case 0x03:
      return 16;
    case 0x04:
      return 24;
    case 0x05:
      return 32;
    case 0x06:
      return 40;
    default:
      return 50;
  }
}

void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *h) {
  CAN_RxHeaderTypeDef rh;
  HAL_CAN_GetRxMessage(h, CAN_RX_FIFO0, &rh, can_rx);
  uint8_t st = GetStartPos(rh.StdId);
  for(int i = 0; i < 8; i++) spi_tx_tmp[st+i] = can_rx[i];
  spi_tx_tmp[BUFFER_SIZE - 1] = 1;
}