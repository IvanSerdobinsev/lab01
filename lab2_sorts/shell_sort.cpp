#include <iostream>
#include <chrono>
#include <random>
#include <fstream>

using namespace std;
using namespace chrono;

// последовательность 1: деление пополам
unsigned* generate_gaps_half(unsigned n, unsigned& gap_count) {
    unsigned* gaps = new unsigned[32];
    gap_count = 0;
    for (unsigned gap = n / 2; gap > 0; gap /= 2)
        gaps[gap_count++] = gap;
    return gaps;
}

// последовательность 2: Хиббард (2^i - 1)
unsigned* generate_gaps_hibbard(unsigned n, unsigned& gap_count) {
    unsigned* gaps = new unsigned[32];
    gap_count = 0;
    unsigned i = 1;
    while ((1u << i) - 1 <= n) ++i;
    while (--i > 0)
        gaps[gap_count++] = (1u << i) - 1;
    return gaps;
}

// последовательность 3: числа Фибоначчи
unsigned* generate_gaps_fibonacci(unsigned n, unsigned& gap_count) {
    unsigned* gaps = new unsigned[64];
    gap_count = 0;
    unsigned a = 1, b = 1;
    while (b <= n) {
        unsigned next = a + b;
        a = b;
        b = next;
    }
    while (a >= 1) {
        gaps[gap_count++] = a;
        unsigned prev = b - a;
        b = a;
        a = prev;
    }
    return gaps;
}

unsigned shell_sort(unsigned arr[], unsigned n, unsigned gaps[], unsigned gap_count) {
    unsigned swaps = 0;
    for (unsigned g = 0; g < gap_count; ++g) {
        unsigned gap = gaps[g];
        for (unsigned i = gap; i < n; ++i) {
            unsigned temp = arr[i];
            unsigned j = i;
            while (j >= gap && arr[j - gap] > temp) {
                arr[j] = arr[j - gap];
                j -= gap;
                ++swaps;
            }
            arr[j] = temp;
        }
    }
    return swaps;
}

void copy_array(unsigned src[], unsigned dst[], unsigned n) {
    for (unsigned i = 0; i < n; ++i)
        dst[i] = src[i];
}

int main() {
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<unsigned> dist(0, 1000000);
    
    ofstream time_file("shell_times.csv");
    ofstream swaps_file("shell_swaps.csv");
    
    time_file << "n,half,hibbard,fibonacci\n";
    swaps_file << "n,half,hibbard,fibonacci\n";
    
    unsigned sizes[] = {1000, 5000, 10000, 50000, 100000, 200000, 500000, 1000000};
    
    for (int s = 0; s < 8; ++s) {
        unsigned n = sizes[s];
        
        unsigned* original = new unsigned[n];
        for (unsigned i = 0; i < n; ++i)
            original[i] = dist(gen);
        
        unsigned gap_count_half, gap_count_hibbard, gap_count_fib;
        unsigned* gaps_half = generate_gaps_half(n, gap_count_half);
        unsigned* gaps_hibbard = generate_gaps_hibbard(n, gap_count_hibbard);
        unsigned* gaps_fib = generate_gaps_fibonacci(n, gap_count_fib);
        
        unsigned* arr_copy = new unsigned[n];
        
        // замер времени и перестановок для half
        copy_array(original, arr_copy, n);
        auto start = high_resolution_clock::now();
        unsigned swaps_half = shell_sort(arr_copy, n, gaps_half, gap_count_half);
        auto end = high_resolution_clock::now();
        double time_half = duration<double>(end - start).count();
        
        // hibbard
        copy_array(original, arr_copy, n);
        start = high_resolution_clock::now();
        unsigned swaps_hibbard = shell_sort(arr_copy, n, gaps_hibbard, gap_count_hibbard);
        end = high_resolution_clock::now();
        double time_hibbard = duration<double>(end - start).count();
        
        // fibonacci
        copy_array(original, arr_copy, n);
        start = high_resolution_clock::now();
        unsigned swaps_fib = shell_sort(arr_copy, n, gaps_fib, gap_count_fib);
        end = high_resolution_clock::now();
        double time_fib = duration<double>(end - start).count();
        
        time_file << n << "," << time_half << "," << time_hibbard << "," << time_fib << "\n";
        swaps_file << n << "," << swaps_half << "," << swaps_hibbard << "," << swaps_fib << "\n";
        
        cout << "n=" << n << " done" << endl;
        
        delete[] original;
        delete[] arr_copy;
        delete[] gaps_half;
        delete[] gaps_hibbard;
        delete[] gaps_fib;
    }
    
    time_file.close();
    swaps_file.close();
    
    return 0;
}