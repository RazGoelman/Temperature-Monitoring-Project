/*
 * commTask.c

 */

#include "commTask.h"
#include "SystemManager.h"
#include "main.h"
#include "Cli.h"
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <iostream>
#include <typeinfo>
using namespace std;

uint8_t cmdbuffer					[MAX_BUFFER_LENGTH];


extern CliContainer container;
extern UART_HandleTypeDef huart2;

int CommTASK::commTask()
{
	uint8_t ch;
	HAL_StatusTypeDef Status = HAL_UART_Receive(&huart2, &ch, 1, 10);
	if (Status != HAL_OK) {
		if ((huart2.Instance->ISR & USART_ISR_ORE) != 0) {
			__HAL_UART_CLEAR_OREFLAG(&huart2);
		}
		// here we have a time to print the command
		while (_cmdprint < _cmdcount) {
			HAL_UART_Transmit(&huart2, &cmdbuffer[_cmdprint++], 1, 0xFFFF);
		}
		return 0;
	}
	if (ch != '\r' && ch != '\n') {
		if (_cmdcount >= MAX_BUFFER_LENGTH) {
			_cmdcount = 0;
			_cmdprint = 0;
		}
		cmdbuffer[_cmdcount++] = ch;
		return 0;
	}
	// here we have a time to print the command
	while (_cmdprint < _cmdcount) {
		HAL_UART_Transmit(&huart2, &cmdbuffer[_cmdprint++], 1, 0xFFFF);
	}

	HAL_UART_Transmit(&huart2, (uint8_t*) "\r\n", 2, 0xFFFF);

	cmdbuffer[_cmdcount] = 0;
	_cmdcount 			 = 0;
	_cmdprint 			 = 0;
	return 1;
}
void CommTASK::handleCommand()
{
	char cmd[20];
	char param[50];
	sscanf((const char*) cmdbuffer, "%s %s", cmd, param);

	container.doCommand(cmd,param);
}
