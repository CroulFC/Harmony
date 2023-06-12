#ifndef SOURCE_WAVFILE_HPP_
#define SOURCE_WAVFILE_HPP_

#include <string>
#include <fstream>

class WavFile{
	private:

	struct WAV_FILE_HEADER{
		char riff_sign[4];
		uint32_t wav_size;
		char wave_sign[4];
		char fmt_sign[4];
		uint32_t fmt_chunk_size;
		uint16_t audio_format;
		uint16_t num_of_channels;
		uint32_t sample_rate;
		uint32_t byte_rate;
		uint16_t sample_aligment;
		uint16_t bit_depth;
		char data_sign[4];
		uint32_t data_bytes;
	} m_file_header{};

	std::fstream m_wav_file{};

	size_t m_total_data_size = 0;

	void FillFileHeader(uint32_t sample_rate, uint16_t bytes_per_sample) noexcept;
	void CompleteFile();

	public:
	enum class OPEN_MODE{
		READ = 0,
		WRITE,
		NOT_OPEN
	};

	explicit WavFile(uint32_t sample_rate, uint8_t bytes_per_sample);

	explicit WavFile(const WavFile&) = delete;
	WavFile& operator=(const WavFile&) = delete;

	WavFile(WavFile &&wavFile) noexcept;
	WavFile& operator=(WavFile &&wavFile) noexcept;

	virtual ~WavFile();
	void Initialize(const std::string& file_name, OPEN_MODE open_mode);
	void Write(const uint8_t* data, size_t data_size);
	void Read(uint8_t* buffer, size_t data_size);
	void Complete();
	OPEN_MODE GetOpenMode() const noexcept;

	private:
	OPEN_MODE m_file_open_mode{};
};

#endif // !SOURCE_WAVFILE_HPP_
