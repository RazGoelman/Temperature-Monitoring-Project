/*
 * DhtManager.h

 */

#ifndef INC_SYSTEMMANAGER_H_
#define INC_SYSTEMMANAGER_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "main.h"

#define ONE_SECOND 1000
#define DHT_DEVICE_ADDR 0xD0

#define WORNINGTEMPERATURE 20.00
#define CRITICALTEMPERATURE 29.00

void StartDht(void *argument);
void StartcommTask(void *argument);
void managerInit();
void StartLedTask(void *argument);
void StartFlashTask(void *argument);
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin);
#ifdef __cplusplus
}
#endif

#endif /* INC_SYSTEMMANAGER_H_ */