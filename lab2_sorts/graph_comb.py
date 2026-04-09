import matplotlib.pyplot as plt
import pandas as pd
import numpy as np
from scipy import stats

print("Загрузка данных расчёски...")
comb_data = pd.read_csv('comb_stats.csv')
n_comb = comb_data['n'].values
time_comb = comb_data['time'].values
swaps_comb = comb_data['swaps'].values
comparisons_comb = comb_data['comparisons'].values

# аппроксимация для расчёски
log_n = np.log(n_comb[2:])
log_time = np.log(time_comb[2:])
slope, intercept, r_value, p_value, std_err = stats.linregress(log_n, log_time)
time_fit = np.exp(intercept) * n_comb ** slope

log_swaps = np.log(swaps_comb[2:])
slope_s, intercept_s, _, _, _ = stats.linregress(log_n, log_swaps)
swaps_fit = np.exp(intercept_s) * n_comb ** slope_s

plt.figure(figsize=(15, 10))

# время расчёски (обычный)
plt.subplot(2, 2, 1)
plt.plot(n_comb, time_comb, 'o', color='green', markersize=4, label='эксперимент')
plt.plot(n_comb, time_fit, '--', color='red', linewidth=2, label=f'аппроксимация O(n^{slope:.2f})')
plt.xlabel('n (размер массива)', fontsize=12)
plt.ylabel('время (сек)', fontsize=12)
plt.title('Сортировка расчёской: время от n', fontsize=14)
plt.legend()
plt.grid(True, alpha=0.3)

# логарифмический график времени
plt.subplot(2, 2, 2)
plt.loglog(n_comb, time_comb, 'o', color='green', markersize=4, label='эксперимент')
plt.loglog(n_comb, time_fit, '--', color='red', linewidth=2, label=f'аппроксимация O(n^{slope:.2f})')
plt.xlabel('n (log scale)', fontsize=12)
plt.ylabel('время (log scale)', fontsize=12)
plt.title('Расчёска: логарифмический график времени', fontsize=14)
plt.legend()
plt.grid(True, alpha=0.3)

# перестановки
plt.subplot(2, 2, 3)
plt.plot(n_comb, swaps_comb, 's', color='blue', markersize=4, label='эксперимент')
plt.plot(n_comb, swaps_fit, '--', color='red', linewidth=2, label=f'аппроксимация O(n^{slope_s:.2f})')
plt.xlabel('n (размер массива)', fontsize=12)
plt.ylabel('количество перестановок', fontsize=12)
plt.title('Расчёска: перестановки от n', fontsize=14)
plt.legend()
plt.grid(True, alpha=0.3)

# сравнения
plt.subplot(2, 2, 4)
plt.plot(n_comb, comparisons_comb, '^', color='purple', markersize=4)
plt.xlabel('n (размер массива)', fontsize=12)
plt.ylabel('количество сравнений', fontsize=12)
plt.title('Расчёска: сравнения от n', fontsize=14)
plt.grid(True, alpha=0.3)

plt.tight_layout()
plt.savefig('comb_sort_plots.png', dpi=150)
plt.show()

print(f"\n=== Сортировка расчёской ===")
print(f"Время: O(n^{slope:.2f})")
print(f"Перестановки: O(n^{slope_s:.2f})")

print("\nЗагрузка данных Шелла...")
times = pd.read_csv('shell_times.csv')
swaps = pd.read_csv('shell_swaps.csv')

n_shell = times['n'].values
half_time = times['half'].values
hibbard_time = times['hibbard'].values
fib_time = times['fibonacci'].values

half_swaps = swaps['half'].values
hibbard_swaps = swaps['hibbard'].values
fib_swaps = swaps['fibonacci'].values

# аппроксимация для каждой последовательности
def fit_power_law(x, y):
    log_x = np.log(x[3:])
    log_y = np.log(y[3:])
    slope, intercept, _, _, _ = stats.linregress(log_x, log_y)
    fit = np.exp(intercept) * x ** slope
    return slope, fit

slope_half, fit_half_time = fit_power_law(n_shell, half_time)
slope_hibbard, fit_hibbard_time = fit_power_law(n_shell, hibbard_time)
slope_fib, fit_fib_time = fit_power_law(n_shell, fib_time)

slope_half_s, fit_half_swaps = fit_power_law(n_shell, half_swaps)
slope_hibbard_s, fit_hibbard_swaps = fit_power_law(n_shell, hibbard_swaps)
slope_fib_s, fit_fib_swaps = fit_power_law(n_shell, fib_swaps)

plt.figure(figsize=(15, 10))

# время Шелла (обычный)
plt.subplot(2, 2, 1)
plt.plot(n_shell, half_time, 'o', label='half', markersize=4)
plt.plot(n_shell, hibbard_time, 's', label='Hibbard', markersize=4)
plt.plot(n_shell, fib_time, '^', label='Fibonacci', markersize=4)
plt.plot(n_shell, fit_half_time, '--', linewidth=2, alpha=0.7)
plt.plot(n_shell, fit_hibbard_time, '--', linewidth=2, alpha=0.7)
plt.plot(n_shell, fit_fib_time, '--', linewidth=2, alpha=0.7)
plt.xlabel('n (размер массива)', fontsize=12)
plt.ylabel('время (сек)', fontsize=12)
plt.title('Сортировка Шелла: время от n', fontsize=14)
plt.legend()
plt.grid(True, alpha=0.3)

# время Шелла (логарифмический)
plt.subplot(2, 2, 2)
plt.loglog(n_shell, half_time, 'o', label='half', markersize=4)
plt.loglog(n_shell, hibbard_time, 's', label='Hibbard', markersize=4)
plt.loglog(n_shell, fib_time, '^', label='Fibonacci', markersize=4)
plt.loglog(n_shell, fit_half_time, '--', linewidth=2, alpha=0.7)
plt.loglog(n_shell, fit_hibbard_time, '--', linewidth=2, alpha=0.7)
plt.loglog(n_shell, fit_fib_time, '--', linewidth=2, alpha=0.7)
plt.xlabel('n (log scale)', fontsize=12)
plt.ylabel('время (log scale)', fontsize=12)
plt.title('Шелл: логарифмический график времени', fontsize=14)
plt.legend()
plt.grid(True, alpha=0.3)

# перестановки Шелла
plt.subplot(2, 2, 3)
plt.plot(n_shell, half_swaps, 'o', label='half', markersize=4)
plt.plot(n_shell, hibbard_swaps, 's', label='Hibbard', markersize=4)
plt.plot(n_shell, fib_swaps, '^', label='Fibonacci', markersize=4)
plt.plot(n_shell, fit_half_swaps, '--', linewidth=2, alpha=0.7)
plt.plot(n_shell, fit_hibbard_swaps, '--', linewidth=2, alpha=0.7)
plt.plot(n_shell, fit_fib_swaps, '--', linewidth=2, alpha=0.7)
plt.xlabel('n (размер массива)', fontsize=12)
plt.ylabel('количество перестановок', fontsize=12)
plt.title('Шелл: перестановки от n', fontsize=14)
plt.legend()
plt.grid(True, alpha=0.3)

# перестановки Шелла (логарифмический)
plt.subplot(2, 2, 4)
plt.loglog(n_shell, half_swaps, 'o', label='half', markersize=4)
plt.loglog(n_shell, hibbard_swaps, 's', label='Hibbard', markersize=4)
plt.loglog(n_shell, fib_swaps, '^', label='Fibonacci', markersize=4)
plt.loglog(n_shell, fit_half_swaps, '--', linewidth=2, alpha=0.7)
plt.loglog(n_shell, fit_hibbard_swaps, '--', linewidth=2, alpha=0.7)
plt.loglog(n_shell, fit_fib_swaps, '--', linewidth=2, alpha=0.7)
plt.xlabel('n (log scale)', fontsize=12)
plt.ylabel('перестановки (log scale)', fontsize=12)
plt.title('Шелл: логарифмический график перестановок', fontsize=14)
plt.legend()
plt.grid(True, alpha=0.3)

plt.tight_layout()
plt.savefig('shell_sort_plots.png', dpi=150)
plt.show()

print("\n Сортировка Шелла ")
print(f"half        : время O(n^{slope_half:.2f}), перестановки O(n^{slope_half_s:.2f})")
print(f"Hibbard     : время O(n^{slope_hibbard:.2f}), перестановки O(n^{slope_hibbard_s:.2f})")
print(f"Fibonacci   : время O(n^{slope_fib:.2f}), перестановки O(n^{slope_fib_s:.2f})")

# пузырёк теоретический O(n²)
bubble_fit = (n_comb / 1000)**2 * time_comb[0] * (1000**2 / n_comb[0]**2)

# Шелл лучший (Hibbard)
best_shell_fit = fit_hibbard_time

plt.figure(figsize=(12, 8))

# только аппроксимирующие прямые на логарифмическом графике
plt.loglog(n_comb, time_comb, 'o', color='green', markersize=5, label='Расчёска (эксп)')
plt.loglog(n_comb, time_fit, '-', color='green', linewidth=2, label=f'Расчёска O(n^{slope:.2f})')

plt.loglog(n_shell, half_time, 'o', color='blue', markersize=4, label='Шелл half (эксп)')
plt.loglog(n_shell, fit_half_time, '-', color='blue', linewidth=1.5, alpha=0.7)

plt.loglog(n_shell, hibbard_time, 's', color='red', markersize=4, label='Шелл Хиббард (эксп)')
plt.loglog(n_shell, fit_hibbard_time, '-', color='red', linewidth=2, label=f'Шелл Хиббард O(n^{slope_hibbard:.2f})')

plt.loglog(n_shell, fib_time, '^', color='purple', markersize=4, label='Шелл Фибоначчи (эксп)')
plt.loglog(n_shell, fit_fib_time, '-', color='purple', linewidth=1.5, alpha=0.7)

# теоретический пузырёк
plt.loglog(n_comb, bubble_fit, '--', color='black', linewidth=2, label='Пузырёк O(n²) (теория)')

plt.xlabel('n (размер массива, log scale)', fontsize=12)
plt.ylabel('время (сек, log scale)', fontsize=12)
plt.title('Сравнение эффективности сортировок (аппроксимации)', fontsize=14)
plt.legend()
plt.grid(True, alpha=0.3)
plt.savefig('all_sorts_comparison.png', dpi=150)
plt.show()

print("\n Вывод ")
print(f"Лучшая по времени: {'Хиббард' if slope_hibbard == min(slope_half, slope_hibbard, slope_fib) else ('Фибоначчи' if slope_fib == min(slope_half, slope_hibbard, slope_fib) else 'half')}")
print(f"Расчёска значительно быстрее пузырька (O(n²) vs O(n^{slope:.2f}))")