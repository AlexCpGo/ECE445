from scipy.fftpack import fft
from scipy.signal import welch
import numpy as np
from scipy.signal import find_peaks
import matplotlib.pyplot as plt
from collections import Counter
import scipy
import pywt
import glob
import joblib
import json
import re
import numpy as np
from matplotlib import pyplot as plt
from scipy import interpolate

def calculate_entropy(list_values):
    counter_values = Counter(list_values).most_common()
    probabilities = [elem[1]/len(list_values) for elem in counter_values]
    entropy=scipy.stats.entropy(probabilities)
    return entropy
 
def calculate_statistics(list_values):
    n5 = np.nanpercentile(list_values, 5)
    n25 = np.nanpercentile(list_values, 25)
    n75 = np.nanpercentile(list_values, 75)
    n95 = np.nanpercentile(list_values, 95)
    median = np.nanpercentile(list_values, 50)
    mean = np.nanmean(list_values)
    std = np.nanstd(list_values)
    var = np.nanvar(list_values)
    rms = np.nanmean(np.sqrt(list_values**2))
    return [n5, n25, n75, n95, median, mean, std, var, rms]
 
def calculate_crossings(list_values):
    zero_crossing_indices = np.where(np.diff(np.sign(list_values) >= 0))[0]
    no_zero_crossings = len(zero_crossing_indices)
    mean_crossing_indices = np.where(np.diff(np.sign(list_values - np.nanmean(list_values)) >= 0))[0]
    no_mean_crossings = len(mean_crossing_indices)
    return [no_zero_crossings, no_mean_crossings]

def get_features(list_values):
    entropy = calculate_entropy(list_values)
    crossings = calculate_crossings(list_values)
    statistics = calculate_statistics(list_values)
    return [entropy] + crossings + statistics

def DWT_series(y_values, T, N, f_s):
    for i in range(3):
        x = np.linspace(0,t_n,N)
        data = y_values[i,:]
        waveletname = 'sym5'
        for ii in range(3):
            (data, coeff_d) = pywt.dwt(data, waveletname)
            if ii == 0:
                feature_temp = np.array(get_features(data)).reshape(1,-1)
            else:
                temp = np.array(get_features(data)).reshape(1,-1)
                feature_temp = np.concatenate((feature_temp, temp), axis = 1)
                if ii == 2:
                    temp = np.array(get_features(coeff_d)).reshape(1,-1)
                    feature_temp = np.concatenate((feature_temp, temp), axis = 1)
        if i == 0:
            feature = feature_temp
        else:
            feature = np.concatenate((feature, feature_temp), axis = 1)
    return feature


def preprocess():
    try:
        for filename in glob.glob('exp/log*.txt'):
            f = open(filename, 'r')
            readline = f.readlines()

            freq = 50
            l = []
            pos = []
            x = []
            y = []
            z = []
            T = []
            # ready = 0
            # print(readline)
            for i in range(len(readline)):
                # print(i,readline[i][0:-1],len(readline[i]))
                if len(readline[i]) >= 42:
                    if readline[i][9] == ',':
                        l = readline[i + 1:]

                        new_len = int((len(l) - 1) / 3) * 3
                        l = l[0:new_len + 1]
                        break
            # print(l)
            for i in range(len(l)):
                if i % 3 == 1:
                    T.append(float(l[i][0:2]))
                elif i % 3 == 2:

                    acc = l[i][-19:-1]
                    pos.append((float(l[i][0:9]), float(l[i][10:20])))
                    xyz = re.split("a+", acc, maxsplit=3)
                    # print(xyz)
                    x.append(int(xyz[0]))
                    y.append(int(xyz[1]))
                    z.append(int(xyz[2]))

            minute = 0

            ##complete time coordinate
            T = np.array(T)
            ini = T[0]
            times = 0

            x_inter = []
            y_inter = []
            z_inter = []

            # print(len(pos)，len(x),len(y),len(z))


            for j in range(len(T)):
                T[j] += minute * 60

                if ini == T[j]:
                    times += 1
                else:
                    T[j - times:j] += np.array(range(times)) / times

                    ini = T[j]
                    if T[j] != T[0] + 1:
                        # print(T[j-times:j])
                        funx = interpolate.interp1d(T[j - times:j + 1], x[j - times:j + 1])
                        funy = interpolate.interp1d(T[j - times:j + 1], y[j - times:j + 1])
                        funz = interpolate.interp1d(T[j - times:j + 1], z[j - times:j + 1])
                        T_insu = np.arange(T[j - times], T[j], 1 / freq)
                        # print(T_insu)
                        x_inter.append(funx(T_insu))
                        y_inter.append(funy(T_insu))
                        z_inter.append(funz(T_insu))
                    times = 0

                if (T[j] % 60 == 59) and (T[j + 1] == 0):
                    minute += 1

            x_inter = np.array(x_inter).reshape(1, -1)  # x complete
            y_inter = np.array(y_inter).reshape(1, -1)  # y complete
            z_inter = np.array(z_inter).reshape(1, -1)  # z complete
            T_INSU = np.arange(T[0] + 1, T[-1], 1 / freq)  # T complete

            # print(T[start],T[end],start,end)
            # print(len(y))

            f.close()

            save = np.concatenate((x_inter, y_inter, z_inter), axis = 0)
            for i in range((save.shape[1] - 22 * freq)//500):
                location = pos[int((i * 500 + (i + 1) * 500)/2 + 22 * freq)]
                np.save('signal/acc{}-{}-{}.npy'.format(location[0], location[1], i), save[:, i * 500 + 22 * freq : (i + 1) * 500 + 22 * freq])

            return True # 表明有log文件，进行了处理，需要后续infer

    except Exception as e:
        print(e)
        return False # 表明没有log文件，不执行infer

if __name__ == '__main__':
    gps = []
    for i, path in enumerate(glob.glob('signal/acc*.npy')):
        path_arg = path.split('-')
        lat = path_arg[0].split('acc')[1]
        lng = path_arg[1]
        acc = np.load(path)
        t_n = 10
        N = 500
        T = t_n / N
        f_s = 1/T
        feature = DWT_series(acc, T, N, f_s)
        if i == 0:
            X = feature
        else:
            X = np.concatenate((X, feature), axis = 0)
        gps.append((lat, lng))
    clf = joblib.load('model/lr.model')
    result = clf.predict(X)
    for i in range(len(result)):
        json_dict = {}
        json_dict['label'] = int(result[i])
        json_dict['gps'] = gps[i]
        with open('result/result{}.json'.format(i), 'w') as file_obj:
            json.dump(json_dict, file_obj)
        