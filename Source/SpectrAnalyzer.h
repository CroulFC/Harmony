/*
Sh(f) = (1/N) * for(i = 0; i < N; ++i){s(i) * cos(2 * pi * f * i) + sin(2 * pi * f * i)}
*/
#include <inttypes.h>

class SpectrAnalyzer{
	public:

	explicit SpectrAnalyzer(uint32_t samples_per_second, uint32_t signal_frequency, uint8_t bytes_per_sample);

	explicit SpectrAnalyzer(const SpectrAnalyzer&) = delete;
	SpectrAnalyzer& operator=(const SpectrAnalyzer&) = delete;

	SpectrAnalyzer(SpectrAnalyzer &&former) noexcept;
	SpectrAnalyzer& operator=(SpectrAnalyzer &&former) noexcept;

	void AnalizePeriod(const uint8_t* signal_period, uint8_t* output_data);

	private:
	uint32_t	m_samples_per_period{};
	uint8_t		m_bytes_per_period{};

	void CalculatePhaseSpectr(const uint8_t* period, double phase_spectr);
	double HartleyOdd(double sample_value, uint32_t harm_index, uint32_t sample_index);
	double HartleyEven(double sample_value, uint32_t harm_index, uint32_t sample_index);
};