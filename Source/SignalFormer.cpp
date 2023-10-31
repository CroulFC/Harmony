#include "SignalFormer.h"

#include <cstddef>
#include <stdexcept>
#include <cmath>

#include <iostream>
#include <limits>
// #ifdef DEBUG
// 	#define LOG(msg)	do{ std::cout << __FUNCTION__ << "(" << __LINE__ << "):" << msg << std::endl;} while(false) 
// #else
// 	#define LOG()
// #endif //DEBUG

SignalFormer::SignalFormer(SIGNAL_PARAMETERS &signal_params){
	m_max_shift = M_PI;
	m_signal_params = signal_params;
	m_samples_per_period = m_signal_params.samples_per_second / m_signal_params.signal_frequency;
	m_data_buffer_size = static_cast<uint8_t>(m_signal_params.data_bits_per_sample) * m_signal_params.number_of_harmonics / 8;
	m_shifts_buffer_size = m_data_buffer_size * (8 / static_cast<uint8_t>(m_signal_params.data_bits_per_sample));
}

uint32_t SignalFormer::GetOnePeriodBufferSize(){
	return m_samples_per_period * static_cast<uint8_t>(m_signal_params.bytes_per_sample);
}

uint32_t SignalFormer::GetDataBufferSize(){
	return m_data_buffer_size;
}

double SignalFormer::CalculateShift(uint8_t bits, uint8_t max_value){
	return m_max_shift / max_value * bits;
}

void SignalFormer::FormShiftsBuffer(const uint8_t* in_data, double* shifts){
	ONE_DATA_BYTE cur_data_byte;
	uint32_t shifts_index = 0;
	
	for(uint32_t i = 0; i < m_data_buffer_size; ++i){
		cur_data_byte.as_uint8 = in_data[i];

		switch(m_signal_params.data_bits_per_sample){
			case DATA_BITS_PER_SAMPLE::ONE :{
				shifts[shifts_index++] = CalculateShift(cur_data_byte.bits.one_bit.bit_0, 1);
				shifts[shifts_index++] = CalculateShift(cur_data_byte.bits.one_bit.bit_1, 1);
				shifts[shifts_index++] = CalculateShift(cur_data_byte.bits.one_bit.bit_2, 1);
				shifts[shifts_index++] = CalculateShift(cur_data_byte.bits.one_bit.bit_3, 1);
				shifts[shifts_index++] = CalculateShift(cur_data_byte.bits.one_bit.bit_4, 1);
				shifts[shifts_index++] = CalculateShift(cur_data_byte.bits.one_bit.bit_5, 1);
				shifts[shifts_index++] = CalculateShift(cur_data_byte.bits.one_bit.bit_6, 1);
				shifts[shifts_index++] = CalculateShift(cur_data_byte.bits.one_bit.bit_7, 1);
				break;
			}
			case DATA_BITS_PER_SAMPLE::TWO :{
				shifts[shifts_index++] = CalculateShift(cur_data_byte.bits.two_bits.bits_0_1, 3);
				shifts[shifts_index++] = CalculateShift(cur_data_byte.bits.two_bits.bits_2_3, 3);
				shifts[shifts_index++] = CalculateShift(cur_data_byte.bits.two_bits.bits_4_5, 3);
				shifts[shifts_index++] = CalculateShift(cur_data_byte.bits.two_bits.bits_6_7, 3);
				break;
			}
			case DATA_BITS_PER_SAMPLE::FOUR :{
				shifts[shifts_index++] = CalculateShift(cur_data_byte.bits.four_bits.bits_0_3, 31);
				shifts[shifts_index++] = CalculateShift(cur_data_byte.bits.four_bits.bits_4_7, 31);
				break;
			}
			case DATA_BITS_PER_SAMPLE::EIGHT :{
				shifts[shifts_index++] = CalculateShift(cur_data_byte.as_uint8, 255);
				break;
			}
		}
	}

	return;
}

double SignalFormer::CalculateValueOfSampleForHarm(uint8_t harm_index, uint32_t sample_index, double phase_shift){
	double argument = static_cast<double>(harm_index) * 2 * M_PI * m_signal_params.signal_frequency;
	argument *= (static_cast<double>(sample_index) / m_signal_params.samples_per_second);
	argument += phase_shift;

	return sin(argument)/harm_index;
}

void SignalFormer::FormOnePeriod(const uint8_t* in_data, uint8_t* out_signal){

	uint32_t i = 0;
	uint32_t out_signal_index = 0;
	double* shifts = new double[m_shifts_buffer_size];
	
	uint32_t shift_index = 0;

	while(i < m_samples_per_period * static_cast<uint8_t>(m_signal_params.bytes_per_sample)){
		double cur_sample_value = 0.0;

		for(uint32_t h = 1; h < m_signal_params.number_of_harmonics + 1; ++h){
			if(		m_signal_params.harmonics_set == HARMONICS_SET::ONLY_EVEN
				&&	h % 2
			  )
			{
				continue;
			}

			if(		m_signal_params.harmonics_set == HARMONICS_SET::ONLY_ODD
				&&	!(h % 2)
			  )
			{
				continue;
			}

			cur_sample_value += CalculateValueOfSampleForHarm(h, i, shifts[shift_index++]);
		}

		cur_sample_value *= m_signal_params.amplitude * 4 / M_PI;

		switch(m_signal_params.bytes_per_sample){
			case BYTES_PER_SAMPLE::ONE :{
				out_signal[out_signal_index] = static_cast<int8_t>(cur_sample_value);
				//std::cout << "cur_sample_value: " << cur_sample_value << std::endl;
				//std::cout << "Converted value: " << int(static_cast<int8_t>(cur_sample_value)) << std::endl;
				out_signal_index += 1;
				break;
			}
			case BYTES_PER_SAMPLE::TWO :{
				break;
			}
			case BYTES_PER_SAMPLE::FOUR :{
				break;
			}
			case BYTES_PER_SAMPLE::EIGHT :{
				break;
			}
		}

		i += static_cast<uint32_t>(m_signal_params.bytes_per_sample);
	}

	delete[] shifts;

	return;
}
