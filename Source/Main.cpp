#include "WavFile.h"
#include "ArgsParser.h"

#include <iostream>
#include <cmath>

static void doTest(const std::string &file_path)
{
	//Try to create WAV-file with 10 secs of 1 KHz sin
	static constexpr uint32_t TEST_SOUND_LENGTH = 5u;
	static constexpr uint32_t TEST_SOUND_FREQUENCY = 300u;
	static constexpr uint32_t TEST_SAMPLE_RATE = 48000u;
	static constexpr uint32_t TEST_AMPLITUDE = 100u;
	static constexpr uint8_t TEST_BYTES_PER_SAMPLE = 1;

	WavFile test_wav_file(TEST_SAMPLE_RATE, TEST_BYTES_PER_SAMPLE);

	test_wav_file.Initialize(file_path, WavFile::OPEN_MODE::WRITE);

	uint8_t *one_second = new uint8_t[TEST_SAMPLE_RATE];

	for(uint32_t s = 0; s < TEST_SOUND_LENGTH; ++s){
		for(uint32_t i = 0; i < TEST_SAMPLE_RATE; ++i){
			double argument = TEST_SOUND_FREQUENCY * i * 2 * M_PI / TEST_SAMPLE_RATE;
			one_second[i] = TEST_AMPLITUDE * sin(argument);
		}

		test_wav_file.Write(one_second, TEST_SAMPLE_RATE);
	}

	test_wav_file.Complete();

	delete[] one_second;
}

static void showUsage(const std::string &app_path)
{
	std::cout << "Usage: " << app_path << " <wav file path>" << std::endl;
}

static void ShowVerboseHelp(){

}

static void ShowHelp(){
	std::cout << "Usage: Harmony [-o/--output]";
}

int main(int argc, char* argv[]){

	// Check args
	if (argc < 2)
	{
		std::cerr << "Invalid args" << std::endl;
		showUsage(argv[0]);
		return EXIT_SUCCESS;
	}

	ArgumentsParser parser(argc, argv);
	ArgumentsParser::Parameter help_param('h',
	std::string("help"),
	ArgumentsParser::Parameter::PARAM_PRESENCE::OPTIONAL,
	ArgumentsParser::Parameter::VALUE_PRESENCE::NOT_REQUIRED
	);

	auto result = parser.GetValueOfParameter(help_param);
	if(result.first){
		ShowHelp();
		return 0;
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
