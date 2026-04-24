#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"

// Function to initialize the NPP protocol
void initNPP(void);

// non-preemptive protocol: lock function
BaseType_t NPPlock(int sem);

// non-preemptive protocol: unlock function
BaseType_t NPPunlock(int sem);