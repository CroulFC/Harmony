#include "SignalFormer.h"

#include <cstddef>

	SignalFormer::SignalFormer(SIGNAL_PARAMETERS &signal_params){
		m_signal_freq = signal_params.signal_frequency;
		m_signal_amplitude = signal_params.amplitude;
	}
