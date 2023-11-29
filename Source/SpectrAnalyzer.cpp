#include "SpectrAnalyzer.h"

SpectrAnalyzer::SpectrAnalyzer(uint32_t samples_per_second, uint32_t signal_frequency, uint8_t bytes_per_sample){
	m_samples_per_period = samples_per_second / signal_frequency;
	m_bytes_per_period = m_samples_per_period * bytes_per_sample;
}



void SpectrAnalyzer::CalculatePhaseSpectr(const uint8_t* period, double phase_spectr){
	
}

void SpectrAnalyzer::AnalizePeriod(const uint8_t* signal_period, uint8_t* output_data){
	double* phase_spectr = new double[m_samples_per_period];


}