#include "FreeRTOS.h"
#include "acceleration_init.h"
#include "game_screens.h"
#include "game_state_machine.h"
#include "led_driver.h"
#include "task.h"
#include <stdio.h>

static uint8_t trans = 24;

static void update_display_task(void *pv) {
  for (;;) {
    led_driver__update_display();
    vTaskDelay(3);
  }
}

static void game_state_machine_task(void *pv) {
  for (;;) {
    game_state_machine__run_game();
    if (game_state_machine__get_current_state() == MAINMENU_s) {
      led_driver__clear_matrix(); // Clear all LEDs // Apply changes to the matrix
    }
    vTaskDelay(trans);
  }
}

static void update_paddle_position(void *pv) {
  static acceleration_position_e acceleration_position = NONE; // Change to acceleration_position_e type
  static game_states_e current_state = MAINMENU_s;
  for (;;) {
    // Get the current position from the accelerometer
    acceleration_position = acceleration__get_position();

    current_state = game_state_machine__get_current_state();

    // Use the accelerometer position for controlling paddle position
    game_state_machine__update_paddle_position(acceleration_position);

    vTaskDelay(36);
  }
}

int main(void) {
  led_driver__init_gpio_pins_for_matrix();
  game_screens__set_matrix_to(GAME);
  game_state_machine__test_setup();
  acceleration__init_device();

  xTaskCreate(update_paddle_position, "joystick_task", 4096 / sizeof(void *), NULL, PRIORITY_MEDIUM, NULL);
  xTaskCreate(update_display_task, "update_display_task", 4096 / sizeof(void *), NULL, PRIORITY_HIGH, NULL);
  xTaskCreate(game_state_machine_task, "game_state_machine_task", 10000 / sizeof(void *), NULL, PRIORITY_HIGH, NULL);
  puts("Starting RTOS scheduler.\n");
  vTaskStartScheduler();
}
