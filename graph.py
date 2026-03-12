import matplotlib.pyplot as plt
import numpy as np
import pandas as pd

data = pd.read_csv('search_results.csv')

N = data['N'].values
av_times = data['av_times'].values * 1000
worst_times = data['worst_times'].values * 1000
av_bin_times = data['av_bin_times'].values * 1000
worst_bin_times = data['worst_bin_times'].values * 1000

logN = np.log2(N)

def linear_approx(x, y):
    coeffs = np.polyfit(x, y, 1)
    return coeffs, np.polyval(coeffs, x)

def linear_approx_ignore_first(x, y):
    coeffs = np.polyfit(x[1:], y[1:], 1)
    return coeffs, np.polyval(coeffs, x)

fig, ((ax1, ax2), (ax3, ax4)) = plt.subplots(2, 2, figsize=(16, 12))
fig.suptitle('Исследование асимптотической сложности алгоритмов поиска', fontsize=16, fontweight='bold')

coeffs_av_bin, approx_av_bin = linear_approx_ignore_first(logN, av_bin_times)
ax1.plot(logN, av_bin_times, 'o', label='Среднее время', marker='o', markersize=4)
ax1.plot(logN, approx_av_bin, 'b--', linewidth=2)
ax1.set_xlabel('log₂(N)', fontsize=12)
ax1.set_ylabel('Время (мкс)', fontsize=12)
ax1.set_title('Бинарный поиск: среднее время от log₂(N)', fontsize=14)
ax1.legend(fontsize=10)
ax1.grid(True, alpha=0.3, linestyle='--')

coeffs_worst_bin, approx_worst_bin = linear_approx_ignore_first(logN, worst_bin_times)
ax2.plot(logN, worst_bin_times, 'o', label='Худшее время', marker='s', markersize=4)
ax2.plot(logN, approx_worst_bin, 'b--', linewidth=2)
ax2.set_xlabel('log (N)', fontsize=12)
ax2.set_ylabel('Время (мкс)', fontsize=12)
ax2.set_title('Бинарный поиск: худшее время от log₂(N)', fontsize=14)
ax2.legend(fontsize=10)
ax2.grid(True, alpha=0.3, linestyle='--')

coeffs_worst_log, approx_worst_log = linear_approx_ignore_first(logN, np.log2(worst_times))
coeffs_worst_bin_log, approx_worst_bin_log = linear_approx_ignore_first(logN, np.log2(worst_times))

ax3.plot(logN, np.log2(worst_times), 'o', label='Линейный поиск', marker='s', markersize=3)
ax3.plot(logN, np.log2(worst_bin_times), 'o', label='Бинарный поиск', marker='o', markersize=3)
ax3.plot(logN, approx_worst_log, 'g--', linewidth=1.5)
ax3.plot(logN, approx_worst_bin_log, 'g--', linewidth=1.5)
ax3.set_xlabel('log (N)', fontsize=12)
ax3.set_ylabel('log Время (мкс)', fontsize=12)
ax3.set_title('Сравнение худшего времени: линейный vs бинарный поиск', fontsize=14)
ax3.legend(fontsize=10)
ax3.grid(True, alpha=0.3, linestyle='--')

coeffs_av_lin, approx_av_lin = linear_approx(N, av_times)
coeffs_worst_lin, approx_worst_lin = linear_approx(N, worst_times)
ax4.plot(N, av_times, 'o', label='Среднее время', marker='o', markersize=3)
ax4.plot(N, worst_times, 'o', label='Худшее время', marker='s', markersize=3)
ax4.plot(N, approx_av_lin, 'g--', linewidth=1.5)
ax4.plot(N, approx_worst_lin, 'm--', linewidth=1.5)
ax4.set_xlabel('N', fontsize=12)
ax4.set_ylabel('Время', fontsize=12)
ax4.set_title('Линейный поиск: сравнение среднего и худшего времени', fontsize=14)
ax4.legend(fontsize=9)
ax4.grid(True, alpha=0.3, linestyle='--')

plt.tight_layout()
plt.show()