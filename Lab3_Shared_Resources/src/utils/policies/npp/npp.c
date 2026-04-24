#include "utils/policies/npp/npp.h"

#define NPP_CEILING_PRIORITY configMAX_PRIORITIES - 1
#define NPP_SEM_COUNT 2
#define SEMAPHORE_WAIT_MS pdMS_TO_TICKS(10)

// Definition of a global semaphore used by the NPP protocol
typedef struct NPPSemaphore {
    TaskHandle_t owner; // current owner of the semaphore
    UBaseType_t originalPriority; // original priority of the owner task
    SemaphoreHandle_t mutex; // mutex to protect access to the semaphore
} NPPSemaphore_t;

NPPSemaphore_t nppSem[NPP_SEM_COUNT] = {0};


// Function to initialize the NPP semaphore
void initNPP(void) {
    for (int sem_idx = 0; sem_idx < NPP_SEM_COUNT; sem_idx++) {
        nppSem[sem_idx].owner = NULL;
        nppSem[sem_idx].originalPriority = 0;
        nppSem[sem_idx].mutex = xSemaphoreCreateMutex();

        if (nppSem[sem_idx].mutex == NULL) {
            printf("Failed to semaphore: %d!\r\n", sem_idx);
        }
    }
}

// non-preemptive protocol: lock function
BaseType_t NPPlock(int sem_idx) {

    // checks passed semaphore value
    if ((sem_idx < 0) || (sem_idx >= NPP_SEM_COUNT)) {
        return pdFAIL;
    }

    // Locks the semaphore and checks if it is not already locked
    if (xSemaphoreTake(nppSem[sem_idx].mutex, SEMAPHORE_WAIT_MS) != pdPASS) {
        return pdFAIL;
    }

    nppSem[sem_idx].owner = xTaskGetCurrentTaskHandle();
    nppSem[sem_idx].originalPriority = uxTaskPriorityGet(nppSem[sem_idx].owner);
    vTaskPrioritySet(nppSem[sem_idx].owner, NPP_CEILING_PRIORITY);

    return pdPASS;
}

// non-preemptive protocol: unlock function
BaseType_t NPPunlock(int sem_idx) {

    // checks passed semaphore value
    if ((sem_idx < 0) || (sem_idx >= NPP_SEM_COUNT)) {
        return pdFAIL;
    }

    if (nppSem[sem_idx].owner != xTaskGetCurrentTaskHandle()) {
        printf("Different task tries to unlock semaphore! \r\n");
        return pdFAIL;
    }

    // Resets structure variables before releasing semaphore
    vTaskPrioritySet(nppSem[sem_idx].owner, nppSem[sem_idx].originalPriority);
    nppSem[sem_idx].owner = NULL;
    nppSem[sem_idx].originalPriority = 0;

    if (xSemaphoreGive(nppSem[sem_idx].mutex) != pdPASS) {
        printf("Failed to unlock the semaphore!\r\n");
        return pdFAIL;
    }

    return pdPASS;
}