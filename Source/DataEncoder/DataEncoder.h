#ifndef SOURCE_DATA_ENCODER_
#define SOURCE_DATA_ENCODER_

#include <inttypes.h>
#include <vector>

#include "../SystemHeaders/SystemParams.h"
#include "../SystemHeaders/SignalParams.h"
#include "../WavFile/WavFile.h"

class DataEncoder {
public:

	explicit DataEncoder(SIGNAL_PARAMS& signal_params, SYSTEM_PARAMS& system_params, WavFile *wav_file);

	explicit DataEncoder(const DataEncoder&) = delete;
	DataEncoder& operator=(const DataEncoder&) = delete;

	DataEncoder(DataEncoder&& encoder) noexcept;
	DataEncoder& operator=(DataEncoder&& encoder) noexcept;

	uint32_t GetOnePeriodBufferSize();
	void FormOnePeriod(const uint8_t* in_data, uint8_t* out_signal);
	uint32_t GetDataBufferSize();

	double CalculateShift(uint8_t bits, uint8_t max_value);
	double CalculateValueOfSampleForHarm(uint8_t harm_index, uint32_t sample_index, double phase_shift);

	void FormShiftsBuffer(const uint8_t* in_data, std::vector<double>& shifts);
	void EncodeDataAndSave(const uint8_t* in_data, uint64_t data_size);

private:
	typedef struct {
		uint32_t samples_per_period;
		uint32_t data_buffer_size;
		uint32_t shifts_buffer_size;
	}ENCODER_PARAMS;

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

	SIGNAL_PARAMS	m_signal_params{};
	SYSTEM_PARAMS	m_system_params{};
	ENCODER_PARAMS	m_encoder_params{};
	WavFile			*m_output_file;
};


#endif //SOURCE_DATA_ENCODER_H_
