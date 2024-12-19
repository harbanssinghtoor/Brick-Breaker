#include "periodic_callbacks.h"
#include <stdio.h>

#include "FreeRTOS.h"
#include "task.h"

/******************************************************************************
 * Your board will reset if the periodic function does not return within its deadline
 * For 1Hz, the function must return within 1000ms
 * For 1000Hz, the function must return within 1ms
 */
void periodic_callbacks__initialize(void) {
  // This method is invoked once when the periodic tasks are created
}

void periodic_callbacks__1Hz(uint32_t callback_count) {
  // Add your code here
  printf("%4d: 1Hz (%d)\n", xTaskGetTickCount(), callback_count);
}

// 10Hz - 100ms
void periodic_callbacks__10Hz(uint32_t callback_count) {
  // Add your code here
  printf("%4d: 10Hz (%d)\n", xTaskGetTickCount(), callback_count);
}

// Assume this function takes 1ms (deadline is 10ms)
// In 1000ms, how much time is this task expected to take:? 100ms of CPU
// F = 1 / time
void periodic_callbacks__100Hz(uint32_t callback_count) {
  // gpio__toggle(board_io__get_led2());
  // You want 50Hz functionality (20ms)
  if (0 == (callback_count % 2)) {
  }
}

/**
 * @warning
 * This is a very fast 1ms task and care must be taken to use this
 * This may be disabled based on intialization of periodic_scheduler__initialize()
 */
void periodic_callbacks__1000Hz(uint32_t callback_count) {
  // gpio__toggle(board_io__get_led3());
  // Add your code here
}