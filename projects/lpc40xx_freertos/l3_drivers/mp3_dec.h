#pragma once

#include "gpio.h"
#include "uart.h"
#include <stdbool.h>
#include <stdint.h>

typedef enum {
  mp3_decoder__next_song = 0x01,
  mp3_decoder__prev_song = 0x02,
  mp3_decoder__play_at_index = 0x03,
  mp3_decoder__set_volume = 0x06,
  mp3_decoder__select_device = 0x09,
  mp3_decoder__reset = 0x0C,
} mp3_decoder__commands_e;

typedef struct {
  uint8_t bytes[8];
} mp3_decoder__command_t;

typedef union {
  mp3_decoder__command_t mp3_decoder_command;
  struct {
    uint64_t start_byte : 8;
    uint64_t version_byte : 8;
    uint64_t data_length : 8;
    uint64_t command_byte : 8;
    uint64_t feedback_byte : 8;
    uint64_t data_byte0 : 8;
    uint64_t data_byte1 : 8;
    uint64_t end_byte : 8;
  } mp3_decoder_command_byte;
} mp3_decoder__msg_t;

void mp3_decoder__init(void);
void mp3_decoder__play_song_at_next_index(void);
void mp3_decoder__play_song_at_prev_index(void);
void mp3_decoder__play_song_at_index(uint8_t index);
void mp3_decoder__volume_set_level(uint8_t volume_level);
void mp3_decoder__reset_decoder(void);