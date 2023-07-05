/********************************************************************************************************************************
   Embedded FW, Kienhoefer, UCSC, WK 3, Summer 2023, ESP32S3 RTOS Semaphore Demo, Herbe Chun
   Script demo's two semaphores - task1 and task2.
   1.  task 1  last for 6 ticks while task 2 last for 7.
   2.  task 2 starts after 2 ticks while task1 starts immediately.
   3.  Once task 2 is started it is blocked by task 1.
   3.  After being blocked for 4 ticks, task1 is completed and task 2 is unblocked.
   4.  After being unlocked, task2 is completed in 3 ticks.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_chip_info.h"
#include "esp_flash.h"
#include "esp_log.h"
#include "freertos/semphr.h" //On vanilla FreeRTOS

#define TRUE 1

SemaphoreHandle_t muxSemaphore;

void task1(void * params);
void task2(void * params);

void app_main(void) {
	muxSemaphore = xSemaphoreCreateMutex();
	xTaskCreate(&task1, "task1", 2048, "task1", 1, NULL);
	xTaskCreate(&task2, "task2", 2048, "task2", 2, NULL);
}

void task1(void * params) {
	
	while(TRUE) {
		xSemaphoreTake(muxSemaphore, portMAX_DELAY);
		printf("Task1 - P1 <-- \n");
		
		int count = 0;
		while (count < 6) {
			vTaskDelay(1000 / portTICK_PERIOD_MS);
			count++;
			if (count % 1 == 0) {
				printf("counter 1 = %d\n", count);
			}
		}
		
		printf("Task1 - P1 --> \n");
		xSemaphoreGive(muxSemaphore);
		
		vTaskDelay(1 / portTICK_PERIOD_MS);
	}
}

void task2(void * params) {
	
    int count = 0;
	while(TRUE) {

		while (count < 2) {
			vTaskDelay(1000 / portTICK_PERIOD_MS);
			count++;
			if (count % 1 == 0) {
				printf("counter 2 = %d\n", count);
			}
		}
		xSemaphoreTake(muxSemaphore, portMAX_DELAY);
		printf("	Task2 - P2 <-- \n");
		
		count = 0;
		while (count < 7) {
			vTaskDelay(1000 / portTICK_PERIOD_MS);
			count++;
			if (count % 1 == 0) {
				printf("counter 3 = %d\n", count);
			}
		}
		
		printf("	Task2- P2 --> \n");
		xSemaphoreGive(muxSemaphore);

		vTaskDelay(1 / portTICK_PERIOD_MS);
	}
}
