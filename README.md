# stm32_hal_can_minimal_demo
winxos 20200612

This demo include the use of transmit and received and mask filter, coding in stm32cubeide.

The key point is the right config of the baudrate, and check the RX0 interrupt, then initialized,

```c
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
```

TO received, you need implement the callback function below

```c
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
```

attention that the callback is for fifo0

TO send standard frame

```c
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
```

The demo code had tested in stm32f103c8t6, stm32cubeide,

The main code is in `setup.c`

For more detail, look for the code, That's all.

