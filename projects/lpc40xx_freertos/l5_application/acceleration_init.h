#pragma once

typedef enum {
  ACCELERATION_LEFT = 0,
  ACCELERATION_RIGHT,
  NONE,
} acceleration_position_e;

void acceleration__init_device(void);
acceleration_position_e acceleration__get_position(void);
