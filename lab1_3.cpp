#include <iostream>
#include <algorithm>
#include <chrono>
#include <random>
#include <fstream>
#include <iomanip>
#include <string>
#include <cmath>

using namespace std;
using namespace std::chrono;

const int MIN_N = 100;
const int MAX_N = 20000;
const int STEP = 500;
const int NUM_ITERATIONS = 100;
const int MAX_ARRAY_SIZE = 20000;

bool quad_sum(const int* arr, int size, int target) {
    for (int i = 0; i < size; ++i) {
        for (int j = i + 1; j < size; ++j) {
            if (arr[i] + arr[j] == target) {
                return true;
            }
        }
    }
    return false;
}

bool lin_sum(const int* arr, int size, int target) {
    int left = 0;
    int right = size - 1;
    
    while (left < right) {
        int sum = arr[left] + arr[right];
        if (sum == target) {
            return true;
        }
        else if (sum < target) {
            left++;
        }
        else {
            right--;
        }
    }
    return false;
}

int main() {
    int num_points = (MAX_N - MIN_N) / STEP + 1;
    
    int* N_values = new int[num_points];
    double* quadratic_times = new double[num_points];
    double* linear_times = new double[num_points];
    
    int* unsorted_array = new int[MAX_ARRAY_SIZE];
    int* sorted_array = new int[MAX_ARRAY_SIZE];
    
    random_device rd;
    mt19937 gen(rd());

    cout << "Диапазон N: от " << MIN_N << " до " << MAX_N << " с шагом " << STEP << endl;
    cout << "Количество повторений: " << NUM_ITERATIONS << endl;
    cout << "Всего точек: " << num_points << endl << endl;
    
    cout << setw(10) << "N" 
        << setw(15) << "Квадр.(мкс)" 
        << setw(15) << "Линейн.(мкс)" 
        << setw(15) << "Отношение" << endl;
    cout << string(55, '-') << endl;
    
    int index = 0;
    for (int N = MIN_N; N <= MAX_N; N += STEP) {
        N_values[index] = N;
        
        double quad_total = 0;
        double lin_total = 0;
        
        for (int iter = 0; iter < NUM_ITERATIONS; iter++) {
            uniform_int_distribution<> dist(1, 1000000);
            for (int i = 0; i < N; i++) {
                unsorted_array[i] = dist(gen);
                sorted_array[i] = unsorted_array[i];
            }
            
            sort(sorted_array, sorted_array + N);
            
            uniform_int_distribution<> target_dist(1, 2000000);
            int target = target_dist(gen);
            
            auto start = high_resolution_clock::now();
            quad_sum(unsorted_array, N, target);
            auto end = high_resolution_clock::now();
            quad_total += duration_cast<microseconds>(end - start).count();
            
            start = high_resolution_clock::now();
            lin_sum(sorted_array, N, target);
            end = high_resolution_clock::now();
            lin_total += duration_cast<microseconds>(end - start).count();
        }
        
        double quad_time = quad_total / NUM_ITERATIONS;
        double lin_time = lin_total / NUM_ITERATIONS;
        
        quadratic_times[index] = quad_time;
        linear_times[index] = lin_time;
        
        double ratio = (lin_time > 0) ? quad_time / lin_time : 0;
        
        cout << setw(10) << N 
            << setw(15) << fixed << setprecision(2) << quad_time
            << setw(15) << setprecision(2) << lin_time
            << setw(15) << setprecision(2) << ratio << endl;
        
        ++index;
    }
    
    ofstream outfile("twosum_results.csv");
    if (outfile.is_open()) {
        outfile << "N,quadratic_time_us,linear_time_us\n";
        for (int i = 0; i < index; i++) {
            outfile << N_values[i] << ","
                << quadratic_times[i] << ","
                << linear_times[i] << "\n";
        }
        outfile.close();
        cout << "\nРезультаты сохранены в twosum_results.csv" << endl;
    }
    delete[] N_values;
    delete[] quadratic_times;
    delete[] linear_times;
    delete[] unsorted_array;
    delete[] sorted_array;
    
    return 0;
}