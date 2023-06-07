#include <iostream>
#include <fstream>

#include "WavFile.h"

class WavFile{
	private:
	WAV_FILE_HEADER* _file_header;
	std::ofstream _wav_file;

	static constexpr char* _default_file_name = (char* const)"result.wav";
	static constexpr uint32_t _default_sample_rate = (uint32_t)48000;

	void FillFileHeader(uint32_t sample_rate = _default_sample_rate){
		return;
	}

	public:
	WavFile(std::string file_name = _default_file_name,
	uint32_t sample_rate = _default_sample_rate)
	{
		_file_header = new WAV_FILE_HEADER;
		if(!_file_header){
			throw "Can't allocate memory for WAV header\n";
			return;
		}

	}

};

int main(int argc, char* argv[]){
	std::cout << "Hello!" << std::endl;
	return 0;
}
