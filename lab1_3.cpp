#include <iostream>
#include <vector>
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

bool quadraticTwoSum(const vector<int>& arr, int target) {
    int n = arr.size();
    for (int i = 0; i < n; ++i) {
        for (int j = i + 1; j < n; ++j) {
            if (arr[i] + arr[j] == target) {
                return true;
            }
        }
    }
    return false;
}

bool linearTwoSum(const vector<int>& arr, int target) {
    int left = 0;
    int right = arr.size() - 1;
    
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
        
        vector<int> N_values;
        vector<double> quadratic_times;
        vector<double> linear_times;
        
        random_device rd;
        mt19937 gen(rd());

        cout << "Диапазон N: от " << MIN_N << " до " << MAX_N << " с шагом " << STEP << endl;
        cout << "Количество повторений: " << NUM_ITERATIONS << endl;
        cout << "Всего точек: " << (MAX_N - MIN_N) / STEP + 1 << endl << endl;
        
        cout << setw(10) << "N" 
            << setw(15) << "Квадр.(мкс)" 
            << setw(15) << "Линейн.(мкс)" 
            << setw(15) << "Отношение" << endl;
        cout << string(55, '-') << endl;
        
        for (int N = MIN_N; N <= MAX_N; N += STEP) {
            N_values.push_back(N);
            
            double quad_total = 0;
            double lin_total = 0;
            
            for (int iter = 0; iter < NUM_ITERATIONS; iter++) {
                vector<int> unsorted_array(N);
                vector<int> sorted_array(N);
                
                uniform_int_distribution<> dist(1, 1000000);
                for (int i = 0; i < N; i++) {
                    unsorted_array[i] = dist(gen);
                    sorted_array[i] = unsorted_array[i];
                }
                
                sort(sorted_array.begin(), sorted_array.end());
                
                uniform_int_distribution<> target_dist(1, 2000000);
                int target = target_dist(gen);
                
                auto start = high_resolution_clock::now();
                quadraticTwoSum(unsorted_array, target);
                auto end = high_resolution_clock::now();
                quad_total += duration_cast<microseconds>(end - start).count();
                
                start = high_resolution_clock::now();
                linearTwoSum(sorted_array, target);
                end = high_resolution_clock::now();
                lin_total += duration_cast<microseconds>(end - start).count();
            }
            
            double quad_time = quad_total / NUM_ITERATIONS;
            double lin_time = lin_total / NUM_ITERATIONS;
            
            quadratic_times.push_back(quad_time);
            linear_times.push_back(lin_time);
            
            double ratio = (lin_time > 0) ? quad_time / lin_time : 0;
            
            cout << setw(10) << N 
                 << setw(15) << fixed << setprecision(2) << quad_time
                 << setw(15) << setprecision(2) << lin_time
                 << setw(15) << setprecision(2) << ratio << endl;
        }
        
        ofstream outfile("twosum_results.csv");
        if (outfile.is_open()) {
            outfile << "N,quadratic_time_us,linear_time_us\n";
            for (size_t i = 0; i < N_values.size(); i++) {
                outfile << N_values[i] << ","
                       << quadratic_times[i] << ","
                       << linear_times[i] << "\n";
            }
            outfile.close();
            cout << "\nРезультаты сохранены в twosum_results.csv" << endl;
        }
        
}