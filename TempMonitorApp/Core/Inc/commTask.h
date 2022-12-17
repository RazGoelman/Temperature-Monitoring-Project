/*
 * commTask.h
 *
 *  Created on: Oct 20, 2022
 *      Author: student
 */

#ifndef INC_COMMTASK_H_
#define INC_COMMTASK_H_

#include "main.h"

#ifdef __cplusplus
extern "C" {
#endif

int commTaskReady(UART_HandleTypeDef* huart2);
int commTask();
void handleCommand();

#ifdef __cplusplus
}
#endif

#endif /* INC_COMMTASK_H_ */
