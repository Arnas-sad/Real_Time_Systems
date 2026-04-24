#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "utils/data_types/stack.h"

// Definition of a SRP semaphore and its handle
struct SRPSemaphore
{
    TaskHandle_t owner;
    UBaseType_t ceiling;
    BaseType_t isLocked;
};
typedef struct SRPSemaphore* SRPSemaphoreHandle_t;

// Function to create and initialize an SRP semaphore
SRPSemaphoreHandle_t xCreateSemaphoreSRP();
// Function to check if a task can start executing under SRP. Returns pdPASS if it can, pdFAIL otherwise.
BaseType_t xSRPExecutionStart();
// Function to lock an SRP semaphore. Returns pdPASS if successful, pdFAIL otherwise.
BaseType_t xSRPLock(SRPSemaphoreHandle_t srpSem);
// Function to unlock an SRP semaphore. Returns pdPASS if successful, pdFAIL otherwise.
BaseType_t xSRPUnlock(SRPSemaphoreHandle_t srpSem);
// Function to declare a task accesses a resource and update the resource's priority ceiling
void vSRPBindTaskToResource(SRPSemaphoreHandle_t srpSem, TaskHandle_t taskHandle);
// Function to initialize the SRP protocol. Returns pdPASS if successful, pdFAIL otherwise.
BaseType_t xSRPInit();