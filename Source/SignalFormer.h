#include <inttypes.h>

class SignalFormer{
	private:
	uint32_t	m_signal_freq{};
	uint32_t 	m_smaples_per_second{};
	uint32_t	m_signal_amplitude{};
	uint8_t		m_number_of_harmonics{};
	uint8_t		m_bytes_per_sample{};
	uint8_t 	m_data_bits_per_sample{};

	public:

	enum class HARMONICS_SET{
		ONLY_EVEN = 0,
		ONLY_ODD,
		ALL
	};

	struct SIGNAL_PARAMETERS{
		uint32_t		signal_frequency;
		uint32_t		samples_per_second;
		double			amplitude;
		uint8_t			number_of_harmonics;
		HARMONICS_SET	harmonics_set;
	};

	explicit SignalFormer(SIGNAL_PARAMETERS &signal_params);

	explicit SignalFormer(const SignalFormer&) = delete;
	SignalFormer& operator=(const SignalFormer&) = delete;

	SignalFormer(SignalFormer &&former) noexcept;
	SignalFormer& operator=(SignalFormer &&former) noexcept;

};
