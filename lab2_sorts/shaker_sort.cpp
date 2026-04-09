#include <iostream>
#include <cassert>

// Прямой проход: от begin_idx до end_idx-1
void forward_step(unsigned arr[], unsigned const begin_idx, unsigned const end_idx) {
    for (unsigned i = begin_idx; i < end_idx; ++i) {
        if (arr[i] > arr[i + 1]) {
            unsigned tmp = arr[i];
            arr[i] = arr[i + 1];
            arr[i + 1] = tmp;
        }
    }
}

// Обратный проход: от end_idx до begin_idx+1
void backward_step(unsigned arr[], unsigned const begin_idx, unsigned const end_idx) {
    for (unsigned i = end_idx; i > begin_idx; --i) {
        if (arr[i - 1] > arr[i]) {
            unsigned tmp = arr[i];
            arr[i] = arr[i - 1];
            arr[i - 1] = tmp;
        }
    }
}

// Шейкерная сортировка с использованием forward_step и backward_step
void shaker_sort(unsigned arr[], unsigned const begin_idx, unsigned const end_idx) {
    unsigned left = begin_idx;
    unsigned right = end_idx;
    bool swapped = true;
    
    while (swapped && left < right) {
        swapped = false;
        
        // прямой проход
        forward_step(arr, left, right);
        if (right > left) {
            right--;
            swapped = true;
        }
        
        // обратный проход
        backward_step(arr, left, right);
        if (right > left) {
            left++;
            swapped = true;
        }
    }
}

// ========== ТЕСТИРОВАНИЕ ФУНКЦИЙ ПО ОТДЕЛЬНОСТИ ==========

// вспомогательная функция для вывода массива
void print_array(unsigned arr[], unsigned n, const char* msg) {
    std::cout << msg << ": ";
    for (unsigned i = 0; i < n; ++i)
        std::cout << arr[i] << " ";
    std::cout << std::endl;
}

// проверка, что массив отсортирован
bool is_sorted(unsigned arr[], unsigned n) {
    for (unsigned i = 0; i < n - 1; ++i)
        if (arr[i] > arr[i + 1]) return false;
    return true;
}

// Тест 1: тестирование forward_step
void test_forward_step() {
    std::cout << "\n=== ТЕСТ forward_step ===\n";
    
    unsigned arr[] = {5, 1, 4, 2, 8};
    unsigned n = 5;
    print_array(arr, n, "До forward_step (0, 3)");
    forward_step(arr, 0, 3);
    print_array(arr, n, "После forward_step (0, 3)");
    assert(arr[0] == 1 && arr[1] == 4 && arr[2] == 2 && arr[3] == 5 && arr[4] == 8);
    std::cout << "✓ forward_step работает корректно\n";
    
    unsigned arr2[] = {3, 2, 1};
    print_array(arr2, 3, "До forward_step (0, 1)");
    forward_step(arr2, 0, 1);
    print_array(arr2, 3, "После forward_step (0, 1)");
    assert(arr2[0] == 2 && arr2[1] == 3 && arr2[2] == 1);
    std::cout << "✓ forward_step на малом массиве корректен\n";
}

// Тест 2: тестирование backward_step
void test_backward_step() {
    std::cout << "\n=== ТЕСТ backward_step ===\n";
    
    unsigned arr[] = {5, 1, 4, 2, 8};
    unsigned n = 5;
    print_array(arr, n, "До backward_step (1, 4)");
    backward_step(arr, 1, 4);
    print_array(arr, n, "После backward_step (1, 4)");
    assert(arr[0] == 5 && arr[1] == 1 && arr[2] == 2 && arr[3] == 4 && arr[4] == 8);
    std::cout << "✓ backward_step работает корректно\n";
    
    unsigned arr2[] = {1, 3, 2, 4};
    print_array(arr2, 4, "До backward_step (0, 3)");
    backward_step(arr2, 0, 3);
    print_array(arr2, 4, "После backward_step (0, 3)");
    assert(arr2[0] == 1 && arr2[1] == 2 && arr2[2] == 3 && arr2[3] == 4);
    std::cout << "✓ backward_step на малом массиве корректен\n";
    
    unsigned arr3[] = {2, 1};
    print_array(arr3, 2, "До backward_step (0, 1)");
    backward_step(arr3, 0, 1);
    print_array(arr3, 2, "После backward_step (0, 1)");
    assert(arr3[0] == 1 && arr3[1] == 2);
    std::cout << "✓ backward_step на двух элементах корректен\n";
}

// Тест 3: тестирование shaker_sort
void test_shaker_sort() {
    std::cout << "\n=== ТЕСТ shaker_sort ===\n";
    
    // тест 1: обычный массив
    unsigned arr1[] = {5, 1, 4, 2, 8, 0, 9, 3, 6, 7};
    unsigned n1 = 10;
    print_array(arr1, n1, "До сортировки");
    shaker_sort(arr1, 0, n1 - 1);
    print_array(arr1, n1, "После сортировки");
    assert(is_sorted(arr1, n1));
    std::cout << "✓ Массив 1 отсортирован\n";
    
    // тест 2: уже отсортированный
    unsigned arr2[] = {1, 2, 3, 4, 5};
    unsigned n2 = 5;
    print_array(arr2, n2, "До сортировки (уже отсорт)");
    shaker_sort(arr2, 0, n2 - 1);
    print_array(arr2, n2, "После сортировки");
    assert(is_sorted(arr2, n2));
    std::cout << "✓ Массив 2 (уже отсортирован) корректен\n";
    
    // тест 3: обратный порядок
    unsigned arr3[] = {9, 8, 7, 6, 5, 4, 3, 2, 1};
    unsigned n3 = 9;
    print_array(arr3, n3, "До сортировки (обратный)");
    shaker_sort(arr3, 0, n3 - 1);
    print_array(arr3, n3, "После сортировки");
    assert(is_sorted(arr3, n3));
    std::cout << "✓ Массив 3 (обратный порядок) отсортирован\n";
    
    // тест 4: массив из одного элемента
    unsigned arr4[] = {42};
    shaker_sort(arr4, 0, 0);
    assert(arr4[0] == 42);
    std::cout << "✓ Массив из 1 элемента корректен\n";
    
    // тест 5: массив из двух элементов
    unsigned arr5[] = {2, 1};
    shaker_sort(arr5, 0, 1);
    assert(arr5[0] == 1 && arr5[1] == 2);
    std::cout << "✓ Массив из 2 элементов корректен\n";
}

// Тест 4: проверка работы отдельных проходов внутри сортировки
void test_integration() {
    std::cout << "\n=== ТЕСТ ИНТЕГРАЦИИ ===\n";
    
    unsigned arr[] = {4, 3, 2, 1};
    unsigned n = 4;
    
    print_array(arr, n, "Начальный массив");
    
    // ручное применение проходов как в шейкерной сортировке
    forward_step(arr, 0, 3);
    print_array(arr, n, "После forward_step (0,3)");
    
    backward_step(arr, 0, 2);
    print_array(arr, n, "После backward_step (0,2)");
    
    forward_step(arr, 1, 2);
    print_array(arr, n, "После forward_step (1,2)");
    
    assert(arr[0] == 1 && arr[1] == 2 && arr[2] == 3 && arr[3] == 4);
    std::cout << "✓ Интеграционный тест пройден\n";
}

int main() {
    std::cout << "====== ШЕЙКЕРНАЯ СОРТИРОВКА ======";
    
    // запуск всех тестов
    test_forward_step();
    test_backward_step();
    test_shaker_sort();
    test_integration();
    
    std::cout << "\n=== ВСЕ ТЕСТЫ ПРОЙДЕНЫ УСПЕШНО ===\n";
    
    // демонстрация работы на примере
    std::cout << "\n=== ДЕМОНСТРАЦИЯ РАБОТЫ ===\n";
    unsigned demo[] = {64, 25, 12, 22, 11, 90, 33, 77, 44, 55};
    unsigned n_demo = 10;
    
    print_array(demo, n_demo, "Исходный массив");
    shaker_sort(demo, 0, n_demo - 1);
    print_array(demo, n_demo, "Отсортированный массив");
    
    return 0;
}