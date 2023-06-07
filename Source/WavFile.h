#pragma once
#include <stdint.h>

static constexpr int WAV_FILE_HEADER_SIZE = 44;

typedef struct _WAV_FILE_HEADER{
    char riff_sign[4];
    uint32_t wav_size;

    char wave_sign[4];
    char fmt_sign[4];
    uint32_t fmt_chunk_size;
    uint16_t audio_format;
    uint16_t num_of_channels;
    uint32_t sample_rate;
    uint32_t byte_rate;
    uint16_t sample_aligment;
    uint16_t bit_depth;

    char data_sign[4];
    uint32_t data_bytes;
} WAV_FILE_HEADER;

static_assert(sizeof(WAV_FILE_HEADER) == WAV_FILE_HEADER_SIZE,"Invalid WAV_FILE_HEADER structure size");