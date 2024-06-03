#include <iostream>
#include <fstream>
#include <memory>

#include "DataEncoder.h"

void SetDefaultSystemParams(SYSTEM_PARAMS& params) {
	params.samples_per_second =		48000;
	params.number_of_harmonics =	8;
	params.max_shift =				M_PI;
	params.harmonics_set =			HARMONICS_SET::ONLY_ODD;
	params.data_bits_per_harmony =	DATA_BITS_PER_HARMONY::ONE;
	params.bytes_per_sample =		BYTES_PER_SAMPLE::TWO;
}

void SetDefaultSignalParams(SIGNAL_PARAMS& params) {
	params.amplitude =		256;
	params.signal_freq =	1000;
}

int main(int argc, char* argv[])
{	
	SYSTEM_PARAMS system_params;
	SetDefaultSystemParams(system_params);

	SIGNAL_PARAMS signal_params;
	SetDefaultSignalParams(signal_params);

	WavFile output_file(system_params.samples_per_second, static_cast<uint8_t>(system_params.bytes_per_sample));

	std::string output_file_path = "Result.wav";

	output_file.Initialize(output_file_path, WavFile::OPEN_MODE::WRITE);

	DataEncoder	encoder(signal_params, system_params, &output_file);

	std::string input_file_name = "input.txt";
	uint64_t input_data_size = 0;
	std::ifstream input_file(input_file_name, std::ios_base::binary);
	input_file.seekg(0, input_file.end);
	input_data_size = input_file.tellg();
	input_file.seekg(0, input_file.beg);

	uint8_t* input_data = new uint8_t[input_data_size];
	input_file.read((char*)input_data, input_data_size);
	//memset(input_data, 0, input_data_size);
	input_file.close();

	encoder.EncodeDataAndSave(input_data, input_data_size);

	delete[] input_data;

	output_file.Complete();

	return 0;
}