#include <iostream>
#include <chrono>
#include <random>
#include <fstream>

using namespace std;
using namespace chrono;

void comb_sort_with_stats(unsigned arr[], unsigned n, unsigned& swaps_count, unsigned& comparisons_count) {
    unsigned step = n;
    bool swapped = true;
    swaps_count = 0;
    comparisons_count = 0;
    
    while (step > 1 || swapped) {
        if (step > 1) {
            step = step * 10 / 13;
            if (step == 0) step = 1;
        }
        
        swapped = false;
        for (unsigned i = 0; i + step < n; ++i) {
            comparisons_count++;
            if (arr[i] > arr[i + step]) {
                unsigned tmp = arr[i];
                arr[i] = arr[i + step];
                arr[i + step] = tmp;
                swapped = true;
                swaps_count++;
            }
        }
    }
}

void copy_array(unsigned src[], unsigned dst[], unsigned n) {
    for (unsigned i = 0; i < n; ++i)
        dst[i] = src[i];
}

int main() {
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<unsigned> dist(0, 1000000);
    
    ofstream comb_file("comb_stats.csv");
    comb_file << "n,time,swaps,comparisons\n";
    
    unsigned sizes[] = {1000, 5000, 10000, 25000, 50000, 100000, 250000, 500000, 750000, 1000000};
    
    cout << "Анализ сортировки расчёской..." << endl;
    
    for (int s = 0; s < 10; ++s) {
        unsigned n = sizes[s];
        
        unsigned* original = new unsigned[n];
        for (unsigned i = 0; i < n; ++i)
            original[i] = dist(gen);
        
        unsigned* arr_copy = new unsigned[n];
        copy_array(original, arr_copy, n);
        
        unsigned swaps = 0, comparisons = 0;
        
        auto start = high_resolution_clock::now();
        comb_sort_with_stats(arr_copy, n, swaps, comparisons);
        auto end = high_resolution_clock::now();
        
        double time_sec = duration<double>(end - start).count();
        
        comb_file << n << "," << time_sec << "," << swaps << "," << comparisons << "\n";
        
        cout << "n=" << n << " | время=" << time_sec << "с | перестановок=" << swaps << " | сравнений=" << comparisons << endl;
        
        delete[] original;
        delete[] arr_copy;
    }
    
    comb_file.close();
    cout << "\nРезультаты сохранены в comb_stats.csv" << endl;
    
    return 0;
}