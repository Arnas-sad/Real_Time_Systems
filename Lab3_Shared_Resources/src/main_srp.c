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

// Pico W devices use a GPIO on the WIFI chip for the LED,
// so when building for Pico W, CYW43_WL_GPIO_LED_PIN will be defined
#ifdef CYW43_WL_GPIO_LED_PIN
#include "pico/cyw43_arch.h"
#endif

#include "utils/traces/traces.h"
#include "utils/delay.h"
#include "utils/policies/srp/srp.h"

SRPSemaphoreHandle_t sem1;
SRPSemaphoreHandle_t sem2;

void vTask1(void *pvParameters)
{
    const uint32_t taskID = 1;
    const TickType_t xFrequency = pdMS_TO_TICKS(250); 
    TickType_t xLastWakeTime;
    xLastWakeTime = xTaskGetTickCount();
    for(;;)
    {
        // check if the task can start executing, wait otherwise
        xSRPExecutionStart();
        logEvent(taskID, JOB_START, (uint32_t)(xTaskGetTickCount() * portTICK_PERIOD_MS));
        busyDelay(10);
        // lock the resource
        if (xSRPLock(sem1) == pdPASS) 
        {
            printf("Task 1 accessed shared resource\n");
            busyDelay(30);
            // unlock the resource
            xSRPUnlock(sem1);
        }
        else
        {
            printf("Task 1 couldn't access shared resource\n");
        }
        busyDelay(20);
        logEvent(taskID, JOB_COMPLETION, (uint32_t)(xTaskGetTickCount() * portTICK_PERIOD_MS));
        vTaskDelayUntil(&xLastWakeTime, xFrequency);
    }
    // should never reach here
    vTaskDelete(NULL);
}

void vTask2(void *pvParameters)
{
    const uint32_t taskID = 2;
    logEvent(taskID, JOB_START, (uint32_t)(xTaskGetTickCount() * portTICK_PERIOD_MS));
    const TickType_t xFrequency = pdMS_TO_TICKS(500); 
    TickType_t xLastWakeTime;
    xLastWakeTime = xTaskGetTickCount();
    for(;;)
    {
        // check if the task can start executing, wait otherwise
        xSRPExecutionStart();
        logEvent(taskID, JOB_START, (uint32_t)(xTaskGetTickCount() * portTICK_PERIOD_MS));
        // we are allowed to start executing now
        printf("Task 2 starts\n");
        busyDelay(50);
        // lock the resource
        if (xSRPLock(sem2) == pdPASS) 
        {
            printf("Task 2 accessed shared resource\n");
            busyDelay(50);
            // unlock the resource
            xSRPUnlock(sem2);
        }
        else
        {
            printf("Task 2 couldn't access shared resource\n");
        }
        busyDelay(10);
        logEvent(taskID, JOB_COMPLETION, (uint32_t)(xTaskGetTickCount() * portTICK_PERIOD_MS));
        vTaskDelayUntil(&xLastWakeTime, xFrequency);
    }
    // should never reach here
    vTaskDelete(NULL);
}

void vTask3(void *pvParameters)
{
    const uint32_t taskID = 3;
    logEvent(taskID, JOB_START, (uint32_t)(xTaskGetTickCount() * portTICK_PERIOD_MS));
    const TickType_t xFrequency = pdMS_TO_TICKS(900); 
    TickType_t xLastWakeTime;
    xLastWakeTime = xTaskGetTickCount();
    for(;;)
    {
        // check if the task can start executing, wait otherwise
        xSRPExecutionStart();
        logEvent(taskID, JOB_START, (uint32_t)(xTaskGetTickCount() * portTICK_PERIOD_MS));
        printf("Task 3 starts\n");
        busyDelay(50);
        // lock the resource
        if (xSRPLock(sem1) == pdPASS) 
        {
            printf("Task 3 accessed shared resource\n");
            busyDelay(100);
            // unlock the resource
            xSRPUnlock(sem1);
        }
        else
        {
            printf("Task 3 couldn't access shared resource\n");
        }
        busyDelay(20);
        logEvent(taskID, JOB_COMPLETION, (uint32_t)(xTaskGetTickCount() * portTICK_PERIOD_MS));
        vTaskDelayUntil(&xLastWakeTime, xFrequency);
    }
    // should never reach here
    vTaskDelete(NULL);
}

void vTask4(void *pvParameters)
{
    const uint32_t taskID = 4;
    logEvent(taskID, JOB_START, (uint32_t)(xTaskGetTickCount() * portTICK_PERIOD_MS));
    const TickType_t xFrequency = pdMS_TO_TICKS(5000); 
    TickType_t xLastWakeTime;
    xLastWakeTime = xTaskGetTickCount();
    for(;;)
    {
        // check if the task can start executing, wait otherwise
        xSRPExecutionStart();
        logEvent(taskID, JOB_START, (uint32_t)(xTaskGetTickCount() * portTICK_PERIOD_MS));
        printf("Task 4 starts\n");
        busyDelay(200);
        // lock the resource
        if (xSRPLock(sem2) == pdPASS) 
        {
            printf("Task 4 accessed shared resource\n");
            busyDelay(50);
            // unlock the resource
            xSRPUnlock(sem2);
        }
        else
        {
            printf("Task 4 couldn't access shared resource\n");
        }
        busyDelay(10);
        logEvent(taskID, JOB_COMPLETION, (uint32_t)(xTaskGetTickCount() * portTICK_PERIOD_MS));
        vTaskDelayUntil(&xLastWakeTime, xFrequency);
    }
    // should never reach here
    vTaskDelete(NULL);
}

int main() {
    stdio_init_all();
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
                
                TaskHandle_t t1Handle, t2Handle, t3Handle, t4Handle;        
                xSRPInit();
                // create the LED blink task (executing vLedBlink at priority 2)
                xTaskCreate(vTask1, "Task 1", 8192, NULL, 4, &t1Handle);
                xTaskCreate(vTask2, "Task 2", 8192, NULL, 3, &t2Handle);
                xTaskCreate(vTask3, "Task 3", 8192, NULL, 2, &t3Handle);
                xTaskCreate(vTask4, "Task 4", 8192, NULL, 1, &t4Handle);
                printf("Tasks created\n");
                
                // create semaphores and bind tasks to those semaphores
                sem1 = xCreateSemaphoreSRP();
                sem2 = xCreateSemaphoreSRP();
                vSRPBindTaskToResource(sem1, t1Handle);
                vSRPBindTaskToResource(sem2, t2Handle);
                vSRPBindTaskToResource(sem1, t3Handle);
                vSRPBindTaskToResource(sem2, t4Handle);
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

