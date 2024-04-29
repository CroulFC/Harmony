import argparse
import numpy as np
import wave
import scipy
import struct
import math


argument_parser = argparse.ArgumentParser(description='Python script for decoding data from DataEncoder')
argument_parser.add_argument('-f', '--frequency',	type=int,	default=1000,
								help='Signal frequency in Hz (integer number)')

argument_parser.add_argument('--harms-number',		type=int,	default=8,
								help='Determines how many harmonics used in each period')

argument_parser.add_argument('--max-shift',			type=float,	default=np.pi,
								help='Max value of shift for one harmonic')

argument_parser.add_argument('--harm-set',			type=str,	default='odd',		choices=['odd', 'even', 'all'],
								help='Set of used harmonics')

argument_parser.add_argument('--bits-per-harm',		type=int,	default=1,
							 	help='Data bits per each harmonic')

argument_parser.add_argument('--input-file',		type=str,	default='./signal.wav',
							 	help='Path to wav file with encoded data')

argument_parser.add_argument('--output-file',		type=str,	default='./result.txt',
								help='Path for output')


def GetOnePeriodSpectr(one_period):
	Y = scipy.fftpack.fftshift(scipy.fftpack.fft(one_period))
	p = np.angle(Y)
	p[np.abs(Y) < 1**-10] = 0
	p = p[len(p) // 2:]
	return p


def DecodePhaseShift(shift, bits_per_harm, max_shift):
	# + PI/2 because shift from fft is [-PI/2; PI/2] but we need [0; PI]
	max_value = (2**bits_per_harm - 1)
	value = round((shift + np.pi / 2) / max_shift * max_value)
	if value > max_value:
		return max_value
	else:
		return value


def DecodeOnePeriod(one_period, args):
	phase_spectr = GetOnePeriodSpectr(one_period)

	harm_step = 2
	if args.harm_set == 'all':
		harm_step = 1

	harm_start_index = 1
	if args.harm_set == 'even':
		harm_start_index = 2
	
	harm_end_index = args.harms_number * 2 + 1
	if args.harm_set == 'all':
		harm_end_index = args.harms_number + 1
	
	data_to_write = []
	for i in range(harm_start_index, harm_end_index, harm_step):
		shift = phase_spectr[i]
		data_to_write.append(DecodePhaseShift(shift, args.bits_per_harm, args.max_shift))
	

	return data_to_write


def WriteDataToFile(data_to_write, args):
	raw_bytes = []
	number_of_bytes = int(len(data_to_write) / 8 * args.bits_per_harm)
	elements_per_one_byte = int(8 / args.bits_per_harm)
	
	for i in range(number_of_bytes):
		cur_byte = ''
		#Reverse order because higher bits codes by higher harms but they should be in start of string
		for e in reversed(range(elements_per_one_byte)):
			cur_value = data_to_write[e + i * elements_per_one_byte]
			cur_byte += bin(cur_value)[2:]

		raw_bytes.append(int(cur_byte, 2))

	with open(args.output_file, 'ab') as file:
		for b in raw_bytes:
			file.write(struct.pack('B', b))


def main(args):
	if args.frequency < 1:
		raise 'The signal frequency must be greater than zero'
	if args.harms_number < 1:
		raise 'The number of harmonics must be greater than zero'
	if args.max_shift > np.pi or args.max_shift <= 0:
		raise 'The maximum phase shift must be greater than 0 and less than or equal to Pi (0 < max_shift <= PI)'
	
	wave_file = wave.open(args.input_file, 'rb')
	frame_rate = wave_file.getframerate()
	bytes_per_sample = wave_file.getsampwidth()

	print(f'{args.input_file} has framerate {frame_rate} and {bytes_per_sample} bytes per sample')

	if bytes_per_sample == 1:
		data_type = np.int8
	elif bytes_per_sample == 2:
		data_type = np.int16
	elif bytes_per_sample == 4:
		data_type = np.int32
	elif bytes_per_sample == 8:
		data_type = np.int64
	else:
		raise 'Incorrect sample width'
	
	samples_per_period = int(frame_rate / args.frequency)

	#Clear file if it is already exist
	with open(args.output_file, 'wb') as file:
		pass

	while cur_period := list(np.frombuffer(wave_file.readframes(samples_per_period), dtype=data_type)):
		data_to_wrie = DecodeOnePeriod(cur_period, args)
		WriteDataToFile(data_to_wrie, args)


if __name__ == '__main__':
	args = argument_parser.parse_args()
	main(args)
	print(f'Encoded data saved to {args.output_file}')
