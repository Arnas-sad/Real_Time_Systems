/**
 * Copyright (c) 2020 Raspberry Pi (Trading) Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "pico/stdlib.h"
#include <stdio.h>
#include <string.h>
#include "FreeRTOS.h"
#include "task.h"
#include "utils/tiebreak.h"

// Pico W devices use a GPIO on the WIFI chip for the LED,
// so when building for Pico W, CYW43_WL_GPIO_LED_PIN will be defined
#ifdef CYW43_WL_GPIO_LED_PIN
#include "pico/cyw43_arch.h"
#endif

#include "utils/traces/traces.h"
#include "utils/highPrioTask.h"
#include "utils/delay.h"

// function that makes the LED blink
void task1(void *pvParameters) {
    const uint32_t taskID = 1;
    const TickType_t xFrequency = pdMS_TO_TICKS(20);
    
    // Initialize to 0 so the first period starts from scheduler start time
    TickType_t previousWakeTime = 0;
    
    for (;;) {
        // Delay first to align all tasks from scheduler start
        vTaskDelayUntil(&previousWakeTime, xFrequency);
        
        // record the time at which the task started the execution of a job
        logEvent(taskID, JOB_START, (uint32_t)(xTaskGetTickCount() * portTICK_PERIOD_MS));
        
        busyDelay(pdMS_TO_TICKS(3));
        printf("task1 exec\n");
        logEvent(taskID, JOB_COMPLETION, (uint32_t)(xTaskGetTickCount() * portTICK_PERIOD_MS));
    }
    // should never reach here
    vTaskDelete(NULL);
}

// function that makes the LED blink
void task2(void *pvParameters) {
    const uint32_t taskID = 2;
    const TickType_t xFrequency = pdMS_TO_TICKS(50);
    
    // Initialize to 0 so the first period starts from scheduler start time
    TickType_t previousWakeTime = 0;
    
    for (;;) {
        // Delay first to align all tasks from scheduler start
        vTaskDelayUntil(&previousWakeTime, xFrequency);
        
        // record the time at which the task started the execution of a job
        logEvent(taskID, JOB_START, (uint32_t)(xTaskGetTickCount() * portTICK_PERIOD_MS));
        
        busyDelay(pdMS_TO_TICKS(6));
        printf("task2 exec\n");
        logEvent(taskID, JOB_COMPLETION, (uint32_t)(xTaskGetTickCount() * portTICK_PERIOD_MS));
    }
    // should never reach here
    vTaskDelete(NULL);
}

// function that makes the LED blink
void task3(void *pvParameters) {
    const uint32_t taskID = 3;
    const TickType_t xFrequency = pdMS_TO_TICKS(80);
    
    // Initialize to 0 so the first period starts from scheduler start time
    TickType_t previousWakeTime = 0;
    
    for (;;) {
        // Delay first to align all tasks from scheduler start
        vTaskDelayUntil(&previousWakeTime, xFrequency);
        
        // record the time at which the task started the execution of a job
        logEvent(taskID, JOB_START, (uint32_t)(xTaskGetTickCount() * portTICK_PERIOD_MS));
        
        busyDelay(pdMS_TO_TICKS(10));
        printf("task3 exec\n");
        logEvent(taskID, JOB_COMPLETION, (uint32_t)(xTaskGetTickCount() * portTICK_PERIOD_MS));
    }
    // should never reach here
    vTaskDelete(NULL);
}

// function that makes the LED blink
void task4(void *pvParameters) {
    const uint32_t taskID = 4;
    const TickType_t xFrequency = pdMS_TO_TICKS(200);
    
    // Initialize to 0 so the first period starts from scheduler start time
    TickType_t previousWakeTime = 0;

    for (;;) {
        // Delay first to align all tasks from scheduler start
        vTaskDelayUntil(&previousWakeTime, xFrequency);
        
        // record the time at which the task started the execution of a job
        logEvent(taskID, JOB_START, (uint32_t)(xTaskGetTickCount() * portTICK_PERIOD_MS));
        
        busyDelay(pdMS_TO_TICKS(20));
        printf("task4 exec\n");
        logEvent(taskID, JOB_COMPLETION, (uint32_t)(xTaskGetTickCount() * portTICK_PERIOD_MS));
    }
    // should never reach here
    vTaskDelete(NULL);
}

int main() {
    stdio_init_all();

 
    xTaskCreate(task1, "task1", configMINIMAL_STACK_SIZE,
                NULL, 3, NULL);
    
    xTaskCreate(task2, "task2", configMINIMAL_STACK_SIZE,
                NULL, 4, NULL);

    xTaskCreate(task3, "task3", configMINIMAL_STACK_SIZE,
                NULL, 2, NULL); 

    xTaskCreate(task4, "task4", configMINIMAL_STACK_SIZE,
                NULL, 1, NULL); 
 
                
    // wait for the usb connection to be completed
    while (!stdio_usb_connected()) {
        sleep_ms(100);
    }
    printf("Connected to Raspberry Pi Pico\n");

    if (cyw43_arch_init()) {
        printf("Wi-Fi init failed");
        return -1;
    }   

    printf("Menu:\ns -> start the scheduler\nq -> quit\n");

    int input_char;

    while (true) {
        input_char = getchar();
        switch (input_char) {
            case 's':
                // turn off the LED before starting the scheduler
                cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, 0);
                // initialize the logger and start the logging task
                initLogger();
                // start a high priority task running at priority 20
                // addHighPriorityTask();
                
                // create the LED blink task (executing vLedBlink at priority 2)
                // xTaskCreate(...);
                
                // start the scheduler
                printf("Scheduler started\n");
                vTaskStartScheduler();
                break;
            case 'q':
                printf("Program stopped\n");
                cyw43_arch_deinit();
                return 0;
            default:
                printf("Invalid input\n");
        }
    }
    return 0;
}