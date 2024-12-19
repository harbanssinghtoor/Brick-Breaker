
#include "led_driver.h"
#include "FreeRTOS.h"
#include "gpio.h"
#include "semphr.h"
#include <stdio.h>
#include <string.h>

static gpio_s R1;
static gpio_s G1;
static gpio_s B1;
static gpio_s R2;
static gpio_s G2;
static gpio_s B2;
static gpio_s A;
static gpio_s B;
static gpio_s C;
static gpio_s D;
static gpio_s E;
static gpio_s CLK;
static gpio_s OE;
static gpio_s LAT;

static volatile uint8_t led_matrix[64][64];
uint8_t led_driver__get_led_matix_value(uint8_t row, uint8_t col) { return led_matrix[row][col]; }

void led_driver__disable_latch(void) { gpio__reset(LAT); }

void led_driver__enable_latch(void) { gpio__set(LAT); }

void led_driver__disable_output(void) { gpio__set(OE); }

void led_driver__enable_output(void) { gpio__reset(OE); }

void led_driver__clock_toggle(void) {
  gpio__set(CLK);
  gpio__reset(CLK);
}

void led_driver__init_gpio_pins_for_matrix(void) {
  R1 = gpio__construct_as_output(GPIO__PORT_1, 14);
  G1 = gpio__construct_as_output(GPIO__PORT_4, 29);
  B1 = gpio__construct_as_output(GPIO__PORT_0, 7);
  R2 = gpio__construct_as_output(GPIO__PORT_0, 9);
  G2 = gpio__construct_as_output(GPIO__PORT_0, 25);
  B2 = gpio__construct_as_output(GPIO__PORT_1, 30);

  A = gpio__construct_as_output(GPIO__PORT_0, 6);
  B = gpio__construct_as_output(GPIO__PORT_0, 8);
  C = gpio__construct_as_output(GPIO__PORT_0, 26);
  D = gpio__construct_as_output(GPIO__PORT_1, 31);
  E = gpio__construct_as_output(GPIO__PORT_1, 20);
  CLK = gpio__construct_as_output(GPIO__PORT_1, 23);
  OE = gpio__construct_as_output(GPIO__PORT_2, 4);
  LAT = gpio__construct_as_output(GPIO__PORT_1, 29);

  gpio__reset(R1);
  gpio__reset(G1);
  gpio__reset(B1);
  gpio__reset(R2);
  gpio__reset(G2);
  gpio__reset(B2);
  gpio__reset(A);
  gpio__reset(B);
  gpio__reset(C);
  gpio__reset(D);
  gpio__reset(E);
  gpio__reset(CLK);
  led_driver__disable_output();
  led_driver__disable_latch();
}

void led_driver__map_color_code_to_color_select_pins_top(color_code_s color) {
  switch (color) {
  case BLACK:
    gpio__reset(R1);
    gpio__reset(G1);
    gpio__reset(B1);
    break;
  case RED:
    gpio__set(R1);
    gpio__reset(G1);
    gpio__reset(B1);
    break;
  case YELLOW:
    gpio__set(R1);
    gpio__set(G1);
    gpio__reset(B1);
    break;
  case GREEN:
    gpio__reset(R1);
    gpio__set(G1);
    gpio__reset(B1);
    break;
  case CYAN:
    gpio__reset(R1);
    gpio__set(G1);
    gpio__set(B1);
    break;
  case BLUE:
    gpio__reset(R1);
    gpio__reset(G1);
    gpio__set(B1);
    break;
  case PURPLE:
    gpio__set(R1);
    gpio__reset(G1);
    gpio__set(B1);
    break;
  case WHITE:
    gpio__set(R1);
    gpio__set(G1);
    gpio__set(B1);
    break;
  default:
    gpio__reset(R1);
    gpio__reset(G1);
    gpio__reset(B1);
    break;
  }
}

void led_driver__map_color_code_to_color_select_pins_bottom(color_code_s color) {
  switch (color) {
  case BLACK:
    gpio__reset(R2);
    gpio__reset(G2);
    gpio__reset(B2);
    break;
  case RED:
    gpio__set(R2);
    gpio__reset(G2);
    gpio__reset(B2);
    break;
  case YELLOW:
    gpio__set(R2);
    gpio__set(G2);
    gpio__reset(B2);
    break;
  case GREEN:
    gpio__reset(R2);
    gpio__set(G2);
    gpio__reset(B2);
    break;
  case CYAN:
    gpio__reset(R2);
    gpio__set(G2);
    gpio__set(B2);
    break;
  case BLUE:
    gpio__reset(R2);
    gpio__reset(G2);
    gpio__set(B2);
    break;
  case PURPLE:
    gpio__set(R2);
    gpio__reset(G2);
    gpio__set(B2);
    break;
  case WHITE:
    gpio__set(R2);
    gpio__set(G2);
    gpio__set(B2);
    break;
  default:
    gpio__reset(R2);
    gpio__reset(G2);
    gpio__reset(B2);
    break;
  }
}

void led_driver__set_pixel(uint8_t row, uint8_t col, color_code_s color) { led_matrix[row][col] = color; }

void led_driver__set_row(uint8_t row, color_code_s color) {
  for (int i = 0; i < 64; i++) {
    led_matrix[row][i] = color;
  }
}
void led_driver__update_display(void) {
  for (int i = 0; i < 32; i++) {
    led_driver__disable_latch();
    led_driver__enable_output();
    for (int j = 0; j < 64; j++) {
      led_driver__map_color_code_to_color_select_pins_top(led_matrix[i][j]);
      led_driver__map_color_code_to_color_select_pins_bottom(led_matrix[i + 32][j]);
      led_driver__clock_toggle();
    }
    led_driver__enable_latch();
    led_driver__disable_output();
    ((i)&0x01) ? gpio__set(A) : gpio__reset(A);
    ((i)&0x02) ? gpio__set(B) : gpio__reset(B);
    ((i)&0x04) ? gpio__set(C) : gpio__reset(C);
    ((i)&0x08) ? gpio__set(D) : gpio__reset(D);
    ((i)&0x10) ? gpio__set(E) : gpio__reset(E);
  }
}

void led_driver__set_display_matrix(uint8_t matrix[64][64]) {
  for (int row = 0; row < 64; row++) {
    for (int col = 0; col < 64; col++) {
      led_matrix[row][col] = matrix[row][col];
    }
  }
}