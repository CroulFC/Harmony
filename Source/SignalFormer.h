#include <inttypes.h>
#include <vector>

class SignalFormer{
	private:
	uint8_t		m_number_of_harmonics{};
	uint8_t		m_bytes_per_sample{};
	uint8_t 	m_data_bits_per_sample{};
	uint32_t 	m_samples_per_period{};



	public:
	explicit SignalFormer(uint8_t butes_per_sample, uint8_t data_bits_per_sample);

	explicit SignalFormer(const SignalFormer&) = delete;
	SignalFormer& operator=(const SignalFormer&) = delete;

	SignalFormer(SignalFormer &&former) noexcept;
	SignalFormer& operator=(SignalFormer &&former) noexcept;

	//Размер элемента вектора должен определяться в соответсвтии с полем m_bytes_per_sample
	//Также стоит определиться, стоит ли возвращать вектор или функция должна принимать указатель
	//на буфер, который будет содержать результат
	std::vector<uint32_t> GenerateOnePeriod(std::vector<uint8_t> &data_slice);

	std::vector<uint32_t> GenerateFullSignal(std::vector<uint8_t> &full_data);
};
