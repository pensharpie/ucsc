/*
 * Embedded FW, Kienhoefer, UCSC, WK 3, Summer 2023, ESP32S3 RTOS Task Demo, Herbe Chun
 * Script demo's two RTOS task being created and deleted.
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

#define FALSE 0
#define TRUE 1

bool GO1 = TRUE;
bool GO2 = TRUE;

int count1 =  0;
int count2  = 0;

void task1(void * params);
void task2(void * params); 

TaskHandle_t xtask1 = NULL;
TaskHandle_t xtask2 = NULL;

void app_main(void) {
	xTaskCreate(&task1, "Task1", 2048,  "task 1", 2, &xtask1); 
	xTaskCreate(&task2, "Task2", 2048,  "task 2", 1, &xtask2);
}

void task1(void * params) {
	while(GO1) {
		printf("task 1 counter - %d\n", count1++);		
		vTaskDelay(1000 / portTICK_PERIOD_MS);
		vPortYield();
		if (count1 > 10) {
				printf("task 1 - delete\n");
				vTaskDelete(xtask1);
		}
	}
}

void task2(void * params) {
	while(GO2) {
		printf("task 2 counter - %d\n", count2++);		
		vTaskDelay(1000 / portTICK_PERIOD_MS);
		vPortYield();
		if (count2 > 20) {
				printf("task 2 - delete\n");
				vTaskDelete(xtask2);
		}
	}
}
