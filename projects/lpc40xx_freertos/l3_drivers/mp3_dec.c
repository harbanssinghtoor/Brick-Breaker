#include "mp3_dec.h"
#include "clock.h"
#include <stdio.h>

static const uint8_t cmd_start = 0x7E;
static const uint8_t version = 0xFF;
static const uint8_t cmd_length = 6;
static const uint8_t no_feedback = 0x00;
static const uint8_t from_index = 0x03;
static const uint8_t cmd_end = 0xEF;
static const uint8_t no_data = 0x00;
static uart_e uart = UART__2;

static void intialize_command_message(mp3_decoder__msg_t *mp3_decoder_message);
static void set_command_and_data(uint8_t command, uint8_t data_0, uint8_t data_1);
static void select_storage_device(void);
static void send_command_via_uart(void);

mp3_decoder__msg_t mp3_command_message = {};

void mp3_decoder__init(void) {
  const uint32_t clear_funtion_bits = ~(0x7 << 0);
  // const uint32_t set_pin_function_uart_tx = (1 << 1);
  // const uint32_t set_pin_function_uart_rx = (1 << 1);

  const uint32_t mp3_decoder_baudrate = 9600;

  // LPC_IOCON->P2_8 &= clear_funtion_bits;
  // LPC_IOCON->P2_9 &= clear_funtion_bits;

  gpio__construct_with_function(GPIO__PORT_2, 8, GPIO__FUNCTION_2);
  gpio__construct_with_function(GPIO__PORT_2, 9, GPIO__FUNCTION_2);
  printf("MP3 init\n");

  uart__init(UART__2, clock__get_peripheral_clock_hz(), mp3_decoder_baudrate);

  intialize_command_message(&mp3_command_message);

  select_storage_device();
}

void mp3_decoder__play_song_at_next_index(void) {
  set_command_and_data(mp3_decoder__next_song, no_data, no_data);
  send_command_via_uart();
}

void mp3_decoder__play_song_at_prev_index(void) {
  set_command_and_data(mp3_decoder__prev_song, no_data, no_data);
  send_command_via_uart();
}

void mp3_decoder__play_song_at_index(uint8_t index) {
  set_command_and_data(mp3_decoder__play_at_index, no_data, index);
  send_command_via_uart();
}

void mp3_decoder__volume_set_level(uint8_t volume_level) {
  if (volume_level <= 30 && volume_level >= 0) {
    set_command_and_data(mp3_decoder__set_volume, no_data, volume_level);
    send_command_via_uart();
  } else {
    // printf("Volume must be from 0 to 30.\n");
  }
}

void mp3_decoder__reset_decoder(void) {
  set_command_and_data(mp3_decoder__reset, no_data, no_data);
  send_command_via_uart();
}

static void intialize_command_message(mp3_decoder__msg_t *mp3_decoder_message) {
  mp3_decoder_message->mp3_decoder_command_byte.start_byte = cmd_start;
  mp3_decoder_message->mp3_decoder_command_byte.version_byte = version;
  mp3_decoder_message->mp3_decoder_command_byte.data_length = cmd_length;
  mp3_decoder_message->mp3_decoder_command_byte.feedback_byte = no_feedback;
  mp3_decoder_message->mp3_decoder_command_byte.end_byte = cmd_end;
}

static void set_command_and_data(uint8_t command, uint8_t data_0, uint8_t data_1) {
  mp3_command_message.mp3_decoder_command_byte.command_byte = command;
  mp3_command_message.mp3_decoder_command_byte.data_byte0 = data_0;
  mp3_command_message.mp3_decoder_command_byte.data_byte1 = data_1;
}

static void select_storage_device(void) {
  static const uint8_t device = 0x02;
  set_command_and_data(mp3_decoder__select_device, no_data, device);
  send_command_via_uart();
}

static void send_command_via_uart(void) {
  uint8_t i = 0;
  while (i < 8) {
    if (uart__polled_put(uart, mp3_command_message.mp3_decoder_command.bytes[i])) {
      i++;
    }
  }
}

void mp3_decoder__play_song_by_index(uint8_t song_index) {
  // Ensure index is within a valid range (depends on the total number of songs)
  if (song_index > 0) {
    // Command for playing a song by index
    set_command_and_data(from_index, 0x00, song_index);
    send_command_via_uart();
  } else {
    printf("Invalid song index. Must be greater than 0.\n");
  }
}
