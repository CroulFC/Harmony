#ifndef SOURCE_SYSTEM_PARAMS_
#define SOURCE_SYSTEM_PARAMS_

#include <inttypes.h>

#define M_PI	3.14159265

enum class HARMONICS_SET {
	ONLY_EVEN = 0,
	ONLY_ODD,
	ALL
};

enum class BYTES_PER_SAMPLE {
	ONE = 1,
	TWO = 2,
	FOUR = 4,
	EIGHT = 8
};

enum class DATA_BITS_PER_HARMONY {
	ONE = 1,
	TWO = 2,
	FOUR = 4,
	EIGHT = 8
};

typedef struct _SYSTEM_PARAMS {
	uint32_t				samples_per_second;
	uint8_t					number_of_harmonics;
	HARMONICS_SET			harmonics_set;
	BYTES_PER_SAMPLE		bytes_per_sample;
	DATA_BITS_PER_HARMONY	data_bits_per_harmony;
	double					max_shift;
}SYSTEM_PARAMS;

typedef union {
	union {
		struct {
			uint8_t	bit_0 : 1;
			uint8_t	bit_1 : 1;
			uint8_t	bit_2 : 1;
			uint8_t	bit_3 : 1;
			uint8_t	bit_4 : 1;
			uint8_t	bit_5 : 1;
			uint8_t	bit_6 : 1;
			uint8_t	bit_7 : 1;
		}one_bit;

		struct {
			uint8_t	bits_0_1 : 2;
			uint8_t	bits_2_3 : 2;
			uint8_t	bits_4_5 : 2;
			uint8_t	bits_6_7 : 2;
		}two_bits;

		struct {
			uint8_t	bits_0_3 : 4;
			uint8_t	bits_4_7 : 4;
		}four_bits;
	}bits;
	uint8_t	as_uint8;
}ONE_DATA_BYTE;

#endif