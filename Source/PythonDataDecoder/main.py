import argparse as a
import numpy as np
import scipy.fftpack as sf
from math import ceil
import wave as w
import struct as s
from datetime import datetime
from functools import lru_cache
start_time = datetime.now()

# Константы
FREQ = 1000
PERIOD = 1/FREQ
SPS = 48000
SPP = ceil(PERIOD*SPS)
ADS = SPP * 1
AMPLITUDE = 2**8

argument_parser = a.ArgumentParser(description="Hellow!")
argument_parser.add_argument('-m', choices=["ed", "enc", "dec"], required=True, default='both', type=str,
                             help='Mode. enc - encrypt data in .wav-file, dec - decrypt '
                                  'data from .wav-file')
argument_parser.add_argument('-d', default='datafiles/text.txt', type=str, help='Data file name')
argument_parser.add_argument('-o', default='outputfiles/output.txt', type=str, help='Output file name')
argument_parser.add_argument('-w', default='wavfiles/result.wav', type=str, help='.wav-file name')

args = argument_parser.parse_args()


def get_data(filename):
    wave_file = w.open(filename, "rb")
    return list(np.frombuffer(wave_file.readframes(wave_file.getnframes()), dtype=np.int16))
    #return list(np.frombuffer(wave_file.readframes(wave_file.getnframes()), dtype=np.int8))


# Чтение данных из файла
def read_inform(filename="input.txt"):
    try:
        inp = open(filename, "rb")
    except FileNotFoundError:
        print(filename, "was not found! Please check name of data file.")
        exit()
    inform = []
    c = inp.read(1)
    while c:
        c = s.unpack('B', c)
        c = bin(c[0])[2:].zfill(8)
        inform.append(c)
        c = inp.read(1)
    inp.close()
    return inform


def write_inform(inform, filename="output.txt"):
    outp = open(filename, "wb")
    for i in inform:
        outp.write(s.pack("B", int(i, 2)))
    outp.close()

def decrypt():
    print("Starting decryption")
    output_data = []
    # Считываем значения из файла
    data_for_analysis = get_data(args.w)

    for b in range(ceil(len(data_for_analysis) / SPP)):
        cur_data = data_for_analysis[b * SPP:(b + 1) * SPP]
        # Фурье-анализ
        t = range(len(cur_data))
        temp = sf.fft(cur_data)
        Y = sf.fftshift(sf.fft(cur_data))
        f = sf.fftshift(sf.fftfreq(len(t)))
        p = np.angle(Y)
        # Не берём в учёт те гармоники, амплитуда которых меньше 10
        p[np.abs(Y) < 10] = 0
        # Смотрим вторую половину полученных данных
        p = p[len(p) // 2:]
        f = f[len(f) // 2:]



        # Восстанавливаем полученные данные
        returned_data = ''
        for i in range(1, len(p), 2):
            if 1 < p[i]:
                returned_data += '1'
            if p[i] < -1:
                returned_data += '0'
            if len(returned_data) >= 8:
                break
        #There is reverse bit order
        returned_data = returned_data[::-1]
        output_data.append(returned_data)

    write_inform(output_data, args.o)
    print("Decryption is done!")


if __name__ == "__main__":
    if args.m == "ed":
        encrypt()
        decrypt()
    elif args.m == "enc":
        encrypt()
    elif args.m == "dec":
        decrypt()
    print("Done!")
    print("Time:", datetime.now() - start_time)
