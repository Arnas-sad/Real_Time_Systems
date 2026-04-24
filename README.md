# Real-Time Systems — FreeRTOS on Raspberry Pi Pico (RP2040)

Three lab assignments from the Real-Time Systems course at TU/e. All labs run FreeRTOS on the RP2040 (ARM Cortex-M0+) using the Pico SDK.

## Lab 1 — Task Creation

Implemented FreeRTOS task creation, priority assignment, and scheduling on the RP2040. Explored how tasks with different priorities are dispatched by the FreeRTOS scheduler, including delay behavior and high-priority task preemption.

**Key files:** `src/main.c`, `src/utils/highPrioTask.c`, `src/utils/delay.c`

## Lab 2 — Fixed Priority vs Earliest Deadline First

Compared Fixed Priority (Rate Monotonic / Deadline Monotonic) and EDF scheduling. Implemented a tiebreaker mechanism for tasks with equal priority. Analyzed schedulability and observed deadline misses under different policies.

**Key files:** `src/main.c`, `src/utils/tiebreak.c`, `include/utils/tiebreak.h`

## Lab 3 — Shared Resource Access Protocols

Implemented Non-Preemptive Protocol (NPP) and Stack Resource Policy (SRP) to handle shared resources and prevent priority inversion and deadlocks. Includes a custom stack data structure for SRP ceiling tracking.

**Key files:** `src/main.c`, `src/main_srp.c`, `src/utils/policies/npp/npp.c`, `src/utils/policies/srp/srp.c`

## Dependencies

- [FreeRTOS Kernel](https://github.com/FreeRTOS/FreeRTOS-Kernel) (not included — clone separately)
- [Pico SDK 2.0.0](https://github.com/raspberrypi/pico-sdk)
- ARM GCC toolchain

## Build

Set environment variables `FREERTOS_PATH` and `PICO_SDK_PATH`, then:

```bash
cd Lab1_Task_Creation  # or Lab2, Lab3
mkdir build && cd build
cmake ..
make
```

Flash the resulting `.uf2` file to the Pico.
