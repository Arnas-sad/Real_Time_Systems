#include "utils/policies/srp/srp.h"
#include "utils/data_types/stack.h"
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"

// stack to keep track of the current system priority ceiling
static Stack globalCeilingStackInstance;
Stack * globalCeilingStack = &globalCeilingStackInstance;
// mutex to protect access to the stack
SemaphoreHandle_t stackMutex = NULL;
// TODO: declare any global variables you may need here
// ...


BaseType_t xSRPInit() 
{
    // TODO: Implement any necessary initialization of you global variables/data structures here
    // ...

    // initialize the mutex used to protect the stack
    stackMutex = xSemaphoreCreateMutex();
    // initialize the global priority ceilings stack
    if(globalCeilingStack != NULL && xSemaphoreTake(stackMutex, portMAX_DELAY) == pdTRUE) {
        init(globalCeilingStack);
        xSemaphoreGive(stackMutex);
        return pdPASS;
    } else {
        printf("Error: Stack initialization failed\n");
        return pdFAIL;
    }
}

// Function to create and initialize an SRP semaphore
SRPSemaphoreHandle_t xCreateSemaphoreSRP()
{
    SRPSemaphoreHandle_t newSem = pvPortMalloc(sizeof(struct SRPSemaphore));
    newSem->owner = NULL;
    newSem->ceiling = 0;
    newSem->isLocked = pdFALSE;
    return newSem;
}

// Function to declare a task accesses a resource and update the resource's priority ceiling
void vSRPBindTaskToResource(SRPSemaphoreHandle_t srpSem, TaskHandle_t taskHandle) 
{
    // TODO: Implement this function
    // HINT1: This function should update the semaphore priority ceiling
    // HINT2: You can use uxTaskPriorityGet to get the priority of the task
}

// Function to check if a task can start executing under SRP
BaseType_t xSRPExecutionStart()
{
    // TODO: Implement this function
    // HINT1: This function should check the current system ceiling
    // HINT2: You can use peek to get the top value of the stack
    // HINT3: Don't forget to protect against race conditions when accessing the stack
    return pdFAIL;
}

// Function to lock a resource using SRP
BaseType_t xSRPLock(SRPSemaphoreHandle_t srpSem) 
{
    // TODO: Implement this function
    // HINT1: This function should update the system ceiling
    // HINT2: You can use push to add a value to the stack
    // HINT3: Don't forget to protect against race conditions
    return pdFAIL;
}

// Function to unlock a resource using SRP
BaseType_t xSRPUnlock(SRPSemaphoreHandle_t srpSem) 
{
    // TODO: Implement this function
    // HINT1: This function should update the system ceiling
    // HINT2: You can use pop to remove the top value from the stack
    // HINT3: Don't forget to protect against race conditions
    return pdFAIL;
}
