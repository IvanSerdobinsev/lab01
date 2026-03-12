#include <iostream>
#include <algorithm>
#include <chrono>
#include <random>
#include <fstream>
#include <iomanip>

using namespace std;

const int MIN_N = 100;
const int MAX_N = 100000;
const int STEP = 2500;
const int NUM_REPEATS = 500;
const int MAX_ARRAY_SIZE = 100000; // Максимальный размер массива

bool lin_search(const int* arr, int size, int target) {
    for (int i = 0; i < size; ++i) {
        if (arr[i] == target) {
            return true;
        }
    }
    return false;
}

bool bin_search(const int* arr, int size, int target) {
    int left = 0;
    int right = size - 1;
    
    while (left <= right) {
        int mid = left + (right - left) / 2;
        
        if (arr[mid] == target) {
            return true;
        }
        else if (arr[mid] < target) {
            left = mid + 1;
        }
        else {
            right = mid - 1;
        }
    }
    return false;
}

int main() {
    int N_values[(MAX_N - MIN_N) / STEP + 1];
    double av_times[(MAX_N - MIN_N) / STEP + 1];
    double worst_times[(MAX_N - MIN_N) / STEP + 1];
    double av_bin_times[(MAX_N - MIN_N) / STEP + 1];
    double worst_bin_times[(MAX_N - MIN_N) / STEP + 1];
    
    int* arr = new int[MAX_ARRAY_SIZE];
    int* sorted_arr = new int[MAX_ARRAY_SIZE];
    
    random_device rd;
    mt19937 gen(rd());
    
    cout << setw(10) << "N " 
        << setw(15) << "Линейный(ср) " 
        << setw(15) << "Линейный(худ) "
        << setw(15) << "Бинарный(ср) "
        << setw(15) << "Бинарный(худ) " << endl;
    cout << string(70, '-') << endl;
    
    int index = 0;
    for (int N = MIN_N; N <= MAX_N; N += STEP) {
        N_values[index] = N;
        
        uniform_int_distribution<> dis(1, N * 10);
        for (int i = 0; i < N; ++i) {
            arr[i] = dis(gen);
            sorted_arr[i] = arr[i];
        }
        
        sort(sorted_arr, sorted_arr + N);
        
        int worst_value = N * 10 + 1;
        
        chrono::steady_clock::time_point start_time;
        double total_av_time = 0;
        uniform_int_distribution<> value_dis(1, N * 10);
        
        for (int repeat = 0; repeat < NUM_REPEATS; ++repeat) {
            int target = value_dis(gen);
            start_time = chrono::steady_clock::now();
            lin_search(arr, N, target);
            auto end_time = chrono::steady_clock::now();
            auto duration = chrono::duration_cast<chrono::microseconds>(end_time - start_time);
            total_av_time += duration.count() / 1000.0;
        }
        
        av_times[index] = total_av_time / NUM_REPEATS;
        
        start_time = chrono::steady_clock::now();
        for (int repeat = 0; repeat < NUM_REPEATS; ++repeat) {
            lin_search(arr, N, worst_value);
        }
        auto end_time = chrono::steady_clock::now();
        auto duration = chrono::duration_cast<chrono::microseconds>(end_time - start_time);
        worst_times[index] = duration.count() / 1000.0 / NUM_REPEATS;
        
        double total_av_bin_time = 0;
        
        for (int repeat = 0; repeat < NUM_REPEATS; ++repeat) {
            int target = value_dis(gen);
            start_time = chrono::steady_clock::now();
            bin_search(sorted_arr, N, target);
            end_time = chrono::steady_clock::now();
            duration = chrono::duration_cast<chrono::microseconds>(end_time - start_time);
            total_av_bin_time += duration.count() / 1000.0;
        }
        av_bin_times[index] = total_av_bin_time / NUM_REPEATS;
        
        start_time = chrono::steady_clock::now();
        for (int repeat = 0; repeat < NUM_REPEATS; ++repeat) {
            bin_search(sorted_arr, N, worst_value);
        }
        end_time = chrono::steady_clock::now();
        duration = chrono::duration_cast<chrono::microseconds>(end_time - start_time);
        worst_bin_times[index] = duration.count() / 1000.0 / NUM_REPEATS;
        
        cout << setw(10) << N 
            << setw(15) << fixed << setprecision(5) << av_times[index]
            << setw(15) << worst_times[index]
            << setw(15) << av_bin_times[index]
            << setw(15) << worst_bin_times[index] << endl;
        
        ++index;
    }
    
    ofstream file("search_results.csv");
    file << "N,av_times,worst_times,av_bin_times,worst_bin_times\n";
    
    for (int i = 0; i < index; ++i) {
        file << N_values[i] << ","
            << av_times[i] << ","
            << worst_times[i] << ","
            << av_bin_times[i] << ","
            << worst_bin_times[i] << "\n";
    }
    file.close();
    
    delete[] arr;
    delete[] sorted_arr;
    
    cout << string(70, '-') << endl;
    cout << "\nИзмерения завершены! Данные сохранены в файл search_results.csv" << endl;
    cout << "Всего измерено точек: " << index << endl;
    
    return 0;
}