#include <iostream>
#include <fstream>

#include "WavFile.h"

class WavFile{
	private:
	WAV_FILE_HEADER* _file_header;
	std::ofstream _wav_file;

	static constexpr char* _default_file_name = (char* const)"result.wav";
	static constexpr uint32_t _default_sample_rate = (uint32_t)48000;
	static constexpr uint16_t _default_num_of_channels = (uint16_t)1;
	static constexpr uint16_t _default_byte_per_sample = (uint16_t)1;

	void FillFileHeader(
		uint32_t sample_rate = _default_sample_rate,
		uint16_t num_of_ch = _default_num_of_channels,
		uint16_t bytes_per_sample = _default_byte_per_sample
	)
	{
		_file_header->riff_sign[0] = 'R';
		_file_header->riff_sign[1] = 'I';
		_file_header->riff_sign[2] = 'F';
		_file_header->riff_sign[3] = 'F';

		_file_header->wave_sign[0] = 'W';
		_file_header->wave_sign[1] = 'A';
		_file_header->wave_sign[2] = 'V';
		_file_header->wave_sign[3] = 'E';

		_file_header->fmt_sign[0] = 'f';
		_file_header->fmt_sign[1] = 'm';
		_file_header->fmt_sign[2] = 't';
		_file_header->fmt_sign[3] = ' ';

		_file_header->data_sign[0] = 'd';
		_file_header->data_sign[1] = 'a';
		_file_header->data_sign[2] = 't';
		_file_header->data_sign[3] = 'a';

		_file_header->fmt_chunk_size = FMT_CHUNK_SIZE;
		_file_header->audio_format = WAV_AUDIO_FORMAT_PCM;
		_file_header->num_of_channels = num_of_ch;
		_file_header->sample_rate = sample_rate;
		_file_header->byte_rate = sample_rate * bytes_per_sample;
		_file_header->sample_aligment = bytes_per_sample * num_of_ch;
		_file_header->bit_depth = bytes_per_sample * 8;

		return;
	}

	void FillHeaderPostInfo(size_t data_size){
		_file_header->wav_size = data_size + sizeof(WAV_FILE_HEADER) 
		- (sizeof(char)*4 + sizeof(uint32_t)); //Size of RIFF + size of wav_size field

		_file_header->data_bytes = data_size;

		return;
	}

	public:
	WavFile(std::string file_name = _default_file_name,
	uint32_t sample_rate = _default_sample_rate)
	{
		_file_header = new WAV_FILE_HEADER;
		if(!_file_header){
			throw "Can't allocate memory for WAV header\n";
		}
		try{
			_wav_file = std::ofstream(file_name);
		}
		catch(std::exception& e){
			throw e;
		}

		return;
	}

};

int main(int argc, char* argv[]){
	std::cout << "Hello!" << std::endl;
	return 0;
}
