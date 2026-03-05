import matplotlib.pyplot as plt
import numpy as np
import pandas as pd

data = pd.read_csv('twosum_results.csv')

print("Названия колонок в файле:", list(data.columns))

N = data['N'].values


if 'quadratic_time_us' in data.columns:
    quad_time = data['quadratic_time_us'].values
elif 'quadratic_time' in data.columns:
    quad_time = data['quadratic_time'].values
elif 'quadratic_mean_us' in data.columns:
    quad_time = data['quadratic_mean_us'].values
else:
    quad_time = data.iloc[:, 1].values
    print(f"Используем колонку {data.columns[1]} для квадратичного времени")

if 'linear_time_us' in data.columns:
    lin_time = data['linear_time_us'].values
elif 'linear_time' in data.columns:
    lin_time = data['linear_time'].values
elif 'linear_mean_us' in data.columns:
    lin_time = data['linear_mean_us'].values
else:
    lin_time = data.iloc[:, 2].values
    print(f"Используем колонку {data.columns[2]} для линейного времени")

def linear_approx(x, y):
    coeffs = np.polyfit(x, y, 1)
    return coeffs, np.polyval(coeffs, x)

def quadratic_approx(x, y):
    coeffs = np.polyfit(x, y, 2)
    return coeffs, np.polyval(coeffs, x)



fig1, (ax1, ax2) = plt.subplots(1, 2, figsize=(16, 7))
fig1.suptitle('Алгоритм полного перебора для задачи "Сумма двух" (O(N²))', 
        fontsize=16, fontweight='bold')

# График 1: Зависимость времени от N
ax1.plot(N, quad_time, 'b-', linewidth=2, label='Экспериментальные данные', marker='o', markersize=4)

# Квадратичная аппроксимация
coeffs_quad, approx_quad = quadratic_approx(N, quad_time)
ax1.plot(N, approx_quad, 'r--', linewidth=2, 
        label=f'Аппроксимация: y = {coeffs_quad[0]:.2e}·N²')


ax1.set_xlim([N[0] - 500, N[-1] + 500])
ax1.set_ylim([0, max(quad_time) * 1.05])  

ax1.set_xlabel('Размер массива N', fontsize=12)
ax1.set_ylabel('Время (микросекунды)', fontsize=12)
ax1.set_title('Зависимость времени от N', fontsize=14)
ax1.legend(fontsize=10)
ax1.grid(True, alpha=0.3, linestyle='--')

# График 2: Зависимость времени от N²
N_squared = N * N
ax2.plot(N_squared, quad_time, 'b-', linewidth=2, label='Экспериментальные данные', marker='o', markersize=4)

# Линейная аппроксимация от N²
coeffs_quad_linear, approx_quad_linear = linear_approx(N_squared, quad_time)
ax2.plot(N_squared, approx_quad_linear, 'r--', linewidth=2,
        label=f'Аппроксимация: y = {coeffs_quad_linear[0]:.2e}·N²')


ax2.set_xlim([N_squared[0] * 0.95, N_squared[-1] * 1.05])
ax2.set_ylim([0, max(quad_time) * 1.05])

ax2.set_xlabel('N²', fontsize=12)
ax2.set_ylabel('Время (микросекунды)', fontsize=12)
ax2.set_title('Зависимость времени от N² (линейная проверка)', fontsize=14)
ax2.legend(fontsize=10)
ax2.grid(True, alpha=0.3, linestyle='--')



plt.tight_layout()

# второе окно
fig2, ax3 = plt.subplots(1, 1, figsize=(10, 7))
fig2.suptitle('Линейный алгоритм для упорядоченного массива (O(N))', 
              fontsize=16, fontweight='bold')

# График 3: Зависимость времени от N
ax3.plot(N, lin_time, 'g-', linewidth=2, label='Экспериментальные данные', marker='s', markersize=4)

coeffs_lin, approx_lin = linear_approx(N, lin_time)
ax3.plot(N, approx_lin, 'r--', linewidth=2,
        label=f'Аппроксимация: y = {coeffs_lin[0]:.4f}·N')

ax3.set_xlim([N[0] - 500, N[-1] + 500])
ax3.set_ylim([0, max(lin_time) * 1.05])

ax3.set_xlabel('Размер массива N', fontsize=12)
ax3.set_ylabel('Время (микросекунды)', fontsize=12)
ax3.set_title('Линейный алгоритм: зависимость от N', fontsize=14)
ax3.legend(fontsize=10)
ax3.grid(True, alpha=0.3, linestyle='--')

plt.tight_layout()


plt.show()