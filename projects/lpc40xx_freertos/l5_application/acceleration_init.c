#include "acceleration_init.h"
#include "acceleration.h"
#include <stdbool.h>

void acceleration__init_device(void) {
  // Initialize the accelerometer
  if (acceleration__init()) {
    printf("Accelerometer initialized successfully!\n");
  } else {
    printf("Failed to initialize accelerometer.\n");
  }
}

acceleration_position_e acceleration__get_position(void) {
  acceleration_position_e acceleration_position = NONE;

  // Read accelerometer data
  acceleration__axis_data_s accel_data = acceleration__get_data();

  // Thresholds for determining movement direction
  const int16_t threshold_left = -200; // Adjust based on sensitivity
  const int16_t threshold_right = 200;

  if (accel_data.x < threshold_left) {
    acceleration_position = ACCELERATION_LEFT;
  } else if (accel_data.x > threshold_right) {
    acceleration_position = ACCELERATION_RIGHT;
  } else {
    acceleration_position = NONE;
  }

  return acceleration_position;
}
