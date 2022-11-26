/*
 * DhtManager.h

 */

#ifndef INC_DHTMANAGER_H_
#define INC_DHTMANAGER_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "main.h"

#define ONE_SECOND 1000
#define DHT_DEVICE_ADDR 0xD0
#define THRESHOLDS_PAGE_256 0x08080000
#define BANK_IN_USED 2
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

#endif /* INC_DHTMANAGER_H_ */
