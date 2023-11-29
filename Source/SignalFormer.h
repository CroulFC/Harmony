#include <inttypes.h>

class SignalFormer{
	public:

	enum class HARMONICS_SET{
		ONLY_EVEN = 0,
		ONLY_ODD,
		ALL
	};

	enum class BYTES_PER_SAMPLE{
		ONE = 1,
		TWO = 2,
		FOUR = 4,
		EIGHT = 8
	};

	enum class DATA_BITS_PER_SAMPLE{
		ONE = 1,
		TWO = 2,
		FOUR = 4,
		EIGHT = 8
	};

	struct SIGNAL_PARAMETERS{
		uint32_t				signal_frequency;
		uint32_t				samples_per_second;
		double 					amplitude;
		uint8_t					number_of_harmonics;
		HARMONICS_SET			harmonics_set;
		BYTES_PER_SAMPLE		bytes_per_sample;
		DATA_BITS_PER_SAMPLE	data_bits_per_sample;
	};

	explicit SignalFormer(SIGNAL_PARAMETERS &signal_params);

	explicit SignalFormer(const SignalFormer&) = delete;
	SignalFormer& operator=(const SignalFormer&) = delete;

	SignalFormer(SignalFormer &&former) noexcept = default;
	SignalFormer& operator=(SignalFormer &&former) noexcept = default;

	uint32_t GetOnePeriodBufferSize() const noexcept;
	void FormOnePeriod(const uint8_t* in_data, uint8_t* out_signal);
	uint32_t GetDataBufferSize() const noexcept;


	private:
	struct SIGNAL_PARAMETERS	m_signal_params;
	uint32_t					m_samples_per_period{};
	uint32_t					m_data_buffer_size{};
	uint32_t					m_shifts_buffer_size{};
	double 						m_max_shift{};

	typedef union{
		union{
			struct{
				uint8_t	bit_0 : 1;
				uint8_t	bit_1 : 1;
				uint8_t	bit_2 : 1;
				uint8_t	bit_3 : 1;
				uint8_t	bit_4 : 1;
				uint8_t	bit_5 : 1;
				uint8_t	bit_6 : 1;
				uint8_t	bit_7 : 1;
			}one_bit;

			struct{
				uint8_t	bits_0_1 : 2;
				uint8_t	bits_2_3 : 2;
				uint8_t	bits_4_5 : 2;
				uint8_t	bits_6_7 : 2;
			}two_bits;

			struct{
				uint8_t	bits_0_3 : 4;
				uint8_t	bits_4_7 : 4;
			}four_bits;
		}bits;
		uint8_t	as_uint8;
	}ONE_DATA_BYTE;

	void FormShiftsBuffer(const uint8_t* in_data, double* shifts);
	double CalculateValueOfSampleForHarm(uint8_t harm_index, uint32_t sample_index, double phase_shift) const noexcept;
	double CalculateShift(uint8_t bits, uint8_t max_value) const noexcept;

};
