from scipy.fftpack import fft
from scipy.signal import welch
import numpy as np
from scipy.signal import find_peaks
import matplotlib.pyplot as plt

def get_psd_values(y_values, T, N, f_s):
    f_values, psd_values = welch(y_values, fs=f_s)
    return f_values, psd_values

def get_fft_values(y_values, T, N, f_s):
    f_values = np.linspace(0.0, 1.0/(2.0*T), N//2)
    fft_values_ = fft(y_values)
    fft_values = 2.0/N * np.abs(fft_values_[0:N//2])
    return f_values, fft_values

def autocorr(x):
    result = np.correlate(x, x, mode='full')
    return result[len(result)//2:]

def get_autocorr_values(y_values, T, N, f_s):
    autocorr_values = autocorr(y_values)
    x_values = np.array([T * jj for jj in range(0, N)])
    return x_values, autocorr_values

def takeSecond(elem):
    return elem[1]

def get_first_n_peaks(x,y,no_peaks=5):
    indices_peaks = find_peaks(y)[0]
    if len(indices_peaks) >= no_peaks:
        peaks_x = [x[peak] for peak in indices_peaks]
        peaks_y = [y[peak] for peak in indices_peaks]
        x_y = list(zip(peaks_x, peaks_y))
        x_y.sort(key=takeSecond, reverse=True)
        peaks_x, peaks_y = zip(*x_y)
        
        return peaks_x[:no_peaks], peaks_y[:no_peaks]
    else:
        missing_no_peaks = no_peaks-len(indices_peaks)
        return [x[peak] for peak in indices_peaks] + [0]*missing_no_peaks, [y[peak] for peak in indices_peaks] + [0]*missing_no_peaks

if __name__ == '__main__':
    t_n = 10
    N = 1000
    T = t_n / N
    f_s = 1/T

    x_value = np.linspace(0,t_n,N)
    amplitudes = [4, 6, 8, 10, 14]
    frequencies = [6.5, 5, 3, 1.5, 1]
    y_values = [amplitudes[ii]*np.sin(2*np.pi*frequencies[ii]*x_value) for ii in range(0,len(amplitudes))]
    composite_y_value = np.sum(y_values, axis=0)

    f_values_fft, fft_values = get_fft_values(composite_y_value, T, N, f_s)
    f_values_psd, psd_values = get_psd_values(composite_y_value, T, N, f_s)
    t_values, autocorr_values = get_autocorr_values(composite_y_value, T, N, f_s)

    # plt.plot(f_values, fft_values, linestyle='-', color='blue')
    # plt.xlabel('Frequency [Hz]', fontsize=16)
    # plt.ylabel('Amplitude', fontsize=16)
    # plt.title("Frequency domain of the signal", fontsize=16)
    # plt.show()