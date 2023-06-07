#pragma once
#include <stdint.h>

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

