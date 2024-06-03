#include "DataEncoder.h"

#include <cstddef>
#include <stdexcept>
#include <cmath>

#include <iostream>
#include <limits>
#include <vector>

#include <memory>

DataEncoder::DataEncoder(SIGNAL_PARAMS& signal_params, SYSTEM_PARAMS& system_params, WavFile *wav_file) {
	m_system_params = system_params;
	m_signal_params = signal_params;
	m_output_file = wav_file;

	m_encoder_params.samples_per_period = static_cast<uint32_t>(m_system_params.samples_per_second
		/ m_signal_params.signal_freq);

	m_encoder_params.data_buffer_size = static_cast<uint8_t>(m_system_params.data_bits_per_harmony) *
		m_system_params.number_of_harmonics / 8;

	m_encoder_params.shifts_buffer_size = m_encoder_params.data_buffer_size /
		static_cast<uint8_t>(m_system_params.data_bits_per_harmony) * 8;

	//Check number of harms and sample rate
}

double DataEncoder::CalculateShift(uint8_t bits, uint8_t max_value) {
	return m_system_params.max_shift / max_value * bits;
}

void DataEncoder::FormShiftsBuffer(const uint8_t* in_data, std::vector<double>& shifts) {
	ONE_DATA_BYTE cur_data_byte;

	for (uint32_t i = 0; i < m_encoder_params.data_buffer_size; ++i) {
		cur_data_byte.as_uint8 = in_data[i];

		//Calculate max value 

		switch (m_system_params.data_bits_per_harmony) {
		case DATA_BITS_PER_HARMONY::ONE: {
			shifts.push_back(CalculateShift(cur_data_byte.bits.one_bit.bit_0, 1));
			shifts.push_back(CalculateShift(cur_data_byte.bits.one_bit.bit_1, 1));
			shifts.push_back(CalculateShift(cur_data_byte.bits.one_bit.bit_2, 1));
			shifts.push_back(CalculateShift(cur_data_byte.bits.one_bit.bit_3, 1));
			shifts.push_back(CalculateShift(cur_data_byte.bits.one_bit.bit_4, 1));
			shifts.push_back(CalculateShift(cur_data_byte.bits.one_bit.bit_5, 1));
			shifts.push_back(CalculateShift(cur_data_byte.bits.one_bit.bit_6, 1));
			shifts.push_back(CalculateShift(cur_data_byte.bits.one_bit.bit_7, 1));
			break;
		}
		case DATA_BITS_PER_HARMONY::TWO: {
			shifts.push_back(CalculateShift(cur_data_byte.bits.two_bits.bits_0_1, 3));
			shifts.push_back(CalculateShift(cur_data_byte.bits.two_bits.bits_2_3, 3));
			shifts.push_back(CalculateShift(cur_data_byte.bits.two_bits.bits_4_5, 3));
			shifts.push_back(CalculateShift(cur_data_byte.bits.two_bits.bits_6_7, 3));
			break;
		}
		case DATA_BITS_PER_HARMONY::FOUR: {
			shifts.push_back(CalculateShift(cur_data_byte.bits.four_bits.bits_0_3, 31));
			shifts.push_back(CalculateShift(cur_data_byte.bits.four_bits.bits_4_7, 31));
			break;
		}
		case DATA_BITS_PER_HARMONY::EIGHT: {
			shifts.push_back(CalculateShift(cur_data_byte.as_uint8, 255));
			break;
		}
		}
	}

	return;
}

double DataEncoder::CalculateValueOfSampleForHarm(uint8_t harm_index, uint32_t sample_index, double phase_shift) {
	double argument = static_cast<double>(harm_index) * 2 * M_PI * m_signal_params.signal_freq;
	argument *= (static_cast<double>(sample_index) / m_system_params.samples_per_second);
	argument += phase_shift;

	double return_value = sin(argument);
	return_value *= m_signal_params.amplitude * static_cast<double>(4) / M_PI;
	return_value /= harm_index;


	return return_value;
}

void DataEncoder::FormOnePeriod(const uint8_t* in_data, uint8_t* out_signal) {

	uint32_t i = 0;
	uint32_t out_signal_index = 0;
	std::vector<double> shifts;
	shifts.reserve(m_encoder_params.shifts_buffer_size);
	FormShiftsBuffer(in_data, shifts);

	uint32_t shift_index = 0;

	uint32_t start_harm_index = m_system_params.harmonics_set == HARMONICS_SET::ONLY_EVEN ? 2 : 1;
	uint32_t end_harm_index = static_cast<uint32_t>(m_system_params.number_of_harmonics);
	if (m_system_params.harmonics_set != HARMONICS_SET::ALL) {
		end_harm_index *= 2;
	}
	uint32_t harm_step = m_system_params.harmonics_set == HARMONICS_SET::ALL ? 1 : 2;

	while (i < m_encoder_params.samples_per_period) {
		double cur_sample_value = 0.0;

		for (uint32_t h = start_harm_index; h < end_harm_index; h += harm_step) {
			cur_sample_value += CalculateValueOfSampleForHarm(h, i, shifts[shift_index++]);
		}

		switch (m_system_params.bytes_per_sample) {
		case BYTES_PER_SAMPLE::ONE: {
			out_signal[out_signal_index] = static_cast<int8_t>(cur_sample_value);
			break;
		}
		case BYTES_PER_SAMPLE::TWO: {
			((uint16_t*)out_signal)[out_signal_index] = static_cast<int16_t>(cur_sample_value);
			break;
		}
		case BYTES_PER_SAMPLE::FOUR: {
			((uint32_t*)out_signal)[out_signal_index] = static_cast<int32_t>(cur_sample_value);
			break;
		}
		case BYTES_PER_SAMPLE::EIGHT: {
			((uint64_t*)out_signal)[out_signal_index] = static_cast<int64_t>(cur_sample_value);
			break;
		}
		}

		out_signal_index += 1;
		shift_index = 0;
		i += 1;
	}

	return;
}

void DataEncoder::EncodeDataAndSave(const uint8_t* in_data, uint64_t data_size) {
	//Calculate how much periods we need
	uint32_t bits_per_period = static_cast<uint8_t>(m_system_params.data_bits_per_harmony) *
		m_system_params.number_of_harmonics;
	uint64_t number_of_periods = data_size * 8 / bits_per_period;

	uint32_t bytes_per_period = m_encoder_params.samples_per_period *
		static_cast<uint8_t>(m_system_params.bytes_per_sample);
	uint8_t* one_period = new uint8_t[bytes_per_period];

	//Calculate each period
	for (uint64_t i = 0; i < number_of_periods; ++i) {
		FormOnePeriod(in_data, one_period);

		m_output_file->Write(one_period, bytes_per_period);
		in_data += m_encoder_params.data_buffer_size;
	}

	delete[] one_period;

	return;
}