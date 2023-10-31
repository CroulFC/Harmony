#include "WavFile.h"

#include <fstream>
#include <cstddef>
#include <limits>

void WavFile::FillFileHeader(uint32_t sample_rate, uint16_t bytes_per_sample) noexcept {
	static constexpr uint32_t  FMT_CHUNK_SIZE = 16; 
	static constexpr uint16_t  WAV_AUDIO_FORMAT_PCM = 1; 
	static constexpr uint16_t  WAV_NUM_OF_CH_MONO = 1;

	m_file_header.riff_sign[0] = 'R';
	m_file_header.riff_sign[1] = 'I';
	m_file_header.riff_sign[2] = 'F';
	m_file_header.riff_sign[3] = 'F';

	m_file_header.wave_sign[0] = 'W';
	m_file_header.wave_sign[1] = 'A';
	m_file_header.wave_sign[2] = 'V';
	m_file_header.wave_sign[3] = 'E';

	m_file_header.fmt_sign[0] = 'f';
	m_file_header.fmt_sign[1] = 'm';
	m_file_header.fmt_sign[2] = 't';
	m_file_header.fmt_sign[3] = ' ';

	m_file_header.data_sign[0] = 'd';
	m_file_header.data_sign[1] = 'a';
	m_file_header.data_sign[2] = 't';
	m_file_header.data_sign[3] = 'a';

	m_file_header.fmt_chunk_size = FMT_CHUNK_SIZE;
	m_file_header.audio_format = WAV_AUDIO_FORMAT_PCM;
	m_file_header.num_of_channels = WAV_NUM_OF_CH_MONO;
	m_file_header.sample_rate = sample_rate;
	m_file_header.byte_rate = sample_rate * bytes_per_sample;
	m_file_header.sample_aligment = bytes_per_sample * m_file_header.num_of_channels;
	m_file_header.bit_depth = bytes_per_sample * 8;

	m_file_header.wav_size = 0;
	m_file_header.data_bytes = 0;
}

void WavFile::Complete(){
	if(!m_wav_file.is_open()){
		throw std::runtime_error("The file is not open");
	}

	CompleteFile();
}

WavFile::WavFile(uint32_t sample_rate, uint8_t bytes_per_sample):
m_file_header{}, m_file_open_mode(OPEN_MODE::NOT_OPEN) {

	if(bytes_per_sample > 2) {
		if(bytes_per_sample > 8){
			throw std::domain_error("More than 8 bytes per sample is unsupported");
		}
		//TODO
	}

	FillFileHeader(sample_rate, bytes_per_sample);
}

WavFile::WavFile(WavFile &&wavFile)  noexcept {
	*this = std::move(wavFile);
}

WavFile &WavFile::operator=(WavFile &&wavFile)  noexcept {
	this->m_file_header = wavFile.m_file_header;
	this->m_wav_file = std::move(wavFile.m_wav_file);
	this->m_total_data_size = wavFile.m_total_data_size;
	wavFile.m_total_data_size = 0;
	this->m_file_open_mode = wavFile.m_file_open_mode;
	wavFile.m_file_open_mode = OPEN_MODE::NOT_OPEN;
	return *this;
}

WavFile::~WavFile(){
	if(!m_wav_file.is_open()){
		return;
	}

	CompleteFile();
}

void WavFile::Write(const uint8_t* data, size_t data_size){
	// if(data_size % m_file_header.byte_rate){
	// 	throw std::invalid_argument("Data size is not a multiple of byte rate of this wav file");
	// }

	if(!m_wav_file.is_open()){
		throw std::runtime_error("The file is not open");
	}

	if(m_file_open_mode == OPEN_MODE::READ){
		throw std::runtime_error("The file is open for reading");
	}

	if(data_size > static_cast<size_t>(std::numeric_limits<std::streamsize>::max())){
		throw std::length_error("Data size is too large");
	}

	m_wav_file.write(reinterpret_cast<const char*>(data), static_cast<std::streamsize>(data_size));
	m_total_data_size += data_size;
}

void WavFile::Read(uint8_t* buffer, size_t data_size){
	if(data_size % m_file_header.byte_rate){
		throw std::domain_error("Data size is not a multiple of byte rate of this wav file");
	}

	if(!m_wav_file.is_open()){
		throw std::runtime_error("The file is not open");
	}

	if(m_file_open_mode == OPEN_MODE::WRITE){
		throw std::runtime_error("The file is open for writing");
	}

	if(data_size > static_cast<size_t>(std::numeric_limits<std::streamsize>::max())){
		throw std::length_error("Data size is too large");
	}

	m_wav_file.read(reinterpret_cast<char*>(buffer), static_cast<std::streamsize>(data_size));
}

void WavFile::Initialize(const std::string& file_name, OPEN_MODE open_mode){
	switch(open_mode){
		case OPEN_MODE::READ:
			m_wav_file = std::fstream(file_name, std::ios_base::binary | std::ios_base::in);
			break;
		case OPEN_MODE::WRITE:
			m_wav_file = std::fstream(file_name, std::ios_base::binary | std::ios_base::out);
			break;
		case OPEN_MODE::NOT_OPEN:
			return;
	}

	if(!m_wav_file.is_open()){
		throw std::runtime_error(std::string("Can not open file ") + file_name);
	}

	m_file_open_mode = open_mode;

	if(m_file_open_mode == OPEN_MODE::READ){
		m_wav_file.read(reinterpret_cast<char*>(&m_file_header), sizeof(m_file_header));
	}
	else{
		m_wav_file.write(reinterpret_cast<const char*>(&m_file_header), sizeof(m_file_header));
	}
}

void WavFile::CompleteFile() {
	if(m_file_open_mode == OPEN_MODE::WRITE) {
		m_file_header.wav_size = m_total_data_size
								 - (sizeof(m_file_header.riff_sign) + sizeof(m_file_header.wav_size))
								 + sizeof(m_file_header);

		m_wav_file.seekp(offsetof(WavFile::WAV_FILE_HEADER, wav_size));
		m_wav_file.write(reinterpret_cast<const char*>(&m_file_header.wav_size), sizeof(m_file_header.wav_size));

		m_file_header.data_bytes = m_total_data_size;
		m_wav_file.seekp(offsetof(WavFile::WAV_FILE_HEADER, data_bytes));
		m_wav_file.write(reinterpret_cast<const char*>(&m_file_header.data_bytes), sizeof(m_file_header.data_bytes));

	}

	m_wav_file.close();
	m_file_open_mode = OPEN_MODE::NOT_OPEN;
}

WavFile::OPEN_MODE WavFile::GetOpenMode() const noexcept {
	return m_file_open_mode;
}
