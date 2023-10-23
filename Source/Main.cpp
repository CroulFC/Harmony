#include "WavFile.h"
#include "SignalFormer.h"

#include <iostream>

static void doTest(const std::string &file_path)
{
	//Try to create WAV-file with 10 secs of 1 KHz sin
	static constexpr uint32_t 	TEST_SOUND_LENGTH = 5u;
	static constexpr uint32_t 	TEST_SOUND_FREQUENCY = 300u;
	static constexpr uint32_t 	TEST_NUMBER_OF_HARMONICS = 32u;
	static constexpr uint32_t 	TEST_SAMPLE_RATE = 48000u;
	static constexpr double 	TEST_AMPLITUDE = 1.0f;
	static constexpr uint8_t 	TEST_BYTES_PER_SAMPLE = 1;

	WavFile test_wav_file(TEST_SAMPLE_RATE, TEST_BYTES_PER_SAMPLE);

	test_wav_file.Initialize(file_path, WavFile::OPEN_MODE::WRITE);

	SignalFormer::SIGNAL_PARAMETERS test_params;
	test_params.amplitude = 			TEST_AMPLITUDE;
	test_params.bytes_per_sample = 		SignalFormer::BYTES_PER_SAMPLE::ONE;
	test_params.data_bits_per_sample = 	SignalFormer::DATA_BITS_PER_SAMPLE::ONE;
	test_params.harmonics_set = 		SignalFormer::HARMONICS_SET::ALL;
	test_params.number_of_harmonics = 	TEST_NUMBER_OF_HARMONICS;
	test_params.samples_per_second = 	TEST_SAMPLE_RATE;
	test_params.signal_frequency = 		TEST_SOUND_FREQUENCY;

	SignalFormer test_signal_former(test_params);

	// uint8_t *one_second = new uint8_t[TEST_SAMPLE_RATE];

	// for(uint32_t s = 0; s < TEST_SOUND_LENGTH; ++s){
	// 	for(uint32_t i = 0; i < TEST_SAMPLE_RATE; ++i){
	// 		double argument = TEST_SOUND_FREQUENCY * i * 2 * M_PI / TEST_SAMPLE_RATE;
	// 		one_second[i] = TEST_AMPLITUDE * sin(argument);
	// 	}

	// 	test_wav_file.Write(one_second, TEST_SAMPLE_RATE);
	// }

	uint32_t full_data_size = 0;
	uint8_t* test_input_data = new uint8_t[test_signal_former.GetDataBufferSize()];
	uint8_t* test_one_period = new uint8_t[test_signal_former.GetOnePeriodBufferSize()];

	while(full_data_size < TEST_SOUND_LENGTH * TEST_SAMPLE_RATE){
		test_signal_former.FormOnePeriod(test_input_data, test_one_period);
		test_wav_file.Write(test_one_period, test_signal_former.GetOnePeriodBufferSize());
		full_data_size += test_signal_former.GetOnePeriodBufferSize();
	}

	test_wav_file.Complete();
	delete[] test_input_data;
	delete[] test_one_period;

	//delete[] one_second;
}

static void showUsage(const std::string &app_path)
{
	std::cout << "Usage: " << app_path << " <wav file path>" << std::endl;
}

int main(int argc, char* argv[]){

	// Check args
	if (argc < 2)
	{
		std::cerr << "Invalid args" << std::endl;
		showUsage(argv[0]);
		return EXIT_SUCCESS;
	}

	// Run tests
	try
	{
		doTest(argv[1]);
	}
	catch (const std::exception &ex)
	{
		std::cerr << "Test failure: " << ex.what() << std::endl;
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}
