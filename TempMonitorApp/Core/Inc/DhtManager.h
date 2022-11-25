/*
 * DhtManager.h

 */

#ifndef INC_DHTMANAGER_H_
#define INC_DHTMANAGER_H_

#ifdef __cplusplus
extern "C" {
#endif

#define ONE_SECOND 1000
#define DHT_DEVICE_ADDR 0xD0
#define THRESHOLDS_PAGE_256 0x08080000
#define BANK_IN_USED 2
#define WORNINGTEMPERATURE 20.00
#define CRITICALTEMPERATURE 29.00

void measureTemp(void *argument);
void managerInit();
void LedTask(void *argument);

#ifdef __cplusplus
}
#endif

#endif /* INC_DHTMANAGER_H_ */
