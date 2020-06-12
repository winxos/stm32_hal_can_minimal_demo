/*
 * setup.c
 *
 *  Created on: Jun 10, 2020
 *      Author: winxo
 */
#include "can.h"
#include <stm32f1xx_hal.h>
uint16_t id=0x0601;
void can_send(uint8_t *data,uint8_t sz)
{
	CAN_TxHeaderTypeDef tx_header;
	uint32_t mail_id;
	tx_header.RTR = CAN_RTR_DATA;
	tx_header.IDE = CAN_ID_STD;
	tx_header.StdId=id;
	tx_header.TransmitGlobalTime = DISABLE;
	tx_header.DLC = sz;
	if (HAL_CAN_AddTxMessage(&hcan, &tx_header, data, &mail_id) != HAL_OK)
	{
	   /* Transmission request Error */
	   Error_Handler();
	}
}
void can_ack(uint16_t id)
{
	CAN_TxHeaderTypeDef tx_header;
	uint8_t tx[] = {0x00};
	uint32_t mail_id;
	tx_header.RTR = CAN_RTR_REMOTE;
	tx_header.IDE = CAN_ID_STD;
	tx_header.StdId=id;
	tx_header.TransmitGlobalTime = DISABLE;
	if (HAL_CAN_AddTxMessage(&hcan, &tx_header, tx, &mail_id) != HAL_OK)
	{
	   /* Transmission request Error */
	   Error_Handler();
	}
}
void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan)
{
	CAN_RxHeaderTypeDef   rx_header;
	uint8_t	rx[8];
	if (HAL_CAN_GetRxMessage(hcan, CAN_RX_FIFO0, &rx_header, rx) != HAL_OK)
	{
	/* Reception Error */
		Error_Handler();
	}
	if ((rx_header.StdId == id) && (rx_header.IDE == CAN_ID_STD) && rx_header.DLC == 2)
	{
		if(rx[0]==0x01)
		{
			HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, rx[1]);
			can_ack(0x602);
		}
	}
}
void setup()
{
	CAN_FilterTypeDef  sFilterConfig;
	sFilterConfig.FilterBank = 0;
	sFilterConfig.FilterMode = CAN_FILTERMODE_IDMASK;
	sFilterConfig.FilterScale = CAN_FILTERSCALE_32BIT;
//	sFilterConfig.FilterIdHigh = 0x601<<5;
	sFilterConfig.FilterIdHigh = 0x0000;
	sFilterConfig.FilterIdLow = 0x0000;
	sFilterConfig.FilterMaskIdHigh = 0x0000;
	sFilterConfig.FilterMaskIdLow = 0x0000;
	sFilterConfig.FilterFIFOAssignment = CAN_FILTER_FIFO0;
	sFilterConfig.FilterActivation = ENABLE;

	if (HAL_CAN_ConfigFilter(&hcan, &sFilterConfig) != HAL_OK)
	{
		Error_Handler();
	}
	if (HAL_CAN_Start(&hcan) != HAL_OK)
	{
		Error_Handler();
	}
	if (HAL_CAN_ActivateNotification(&hcan, CAN_IT_RX_FIFO0_MSG_PENDING) != HAL_OK)
	{
		Error_Handler();
	}

	while(1)
	{
		HAL_Delay(1000);
	}

}
