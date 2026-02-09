#include <iostream>
#include <vector>
#include <fstream>
#include "Auxil.h"
#include <ctime>
using namespace std;

// Рекурсивная функция Фибоначчи
long long fib(int n) {
    if (n <= 1) return n;
    return fib(n - 1) + fib(n - 2);
}

int main() {
    start(); // инициализация генератора

    // === Задание 2: измерение времени работы цикла ===
    vector<int> cycles = { 10000, 50000, 100000, 500000 }; // количество итераций
    ofstream out("timing_results.txt");
    out << "Iterations,Duration_sec\n";

    for (int N : cycles) {
        clock_t t1 = clock();

        for (int i = 0; i < N; ++i) {
            double x = dget();
            int y = iget();
        }

        clock_t t2 = clock();
        double duration = double(t2 - t1) / CLOCKS_PER_SEC;
        cout << "Циклов: " << N << ", Время: " << duration << " сек\n";
        out << "Циклов: " << N << ", Время: " << duration << " сек\n";
    }

    out.close();

    // === Задание 3: рекурсивный алгоритм (Фибоначчи) ===
    vector<int> fib_indices = { 10, 15, 20,25 ,30 ,       35 ,40 ,45 }; // для больших чисел рекурсия сильно замедляет
    ofstream out2("fib_timing.txt");


    for (int idx : fib_indices) {
        clock_t t1 = clock();
        long long value = fib(idx);
        clock_t t2 = clock();
        double duration = double(t2 - t1) / CLOCKS_PER_SEC;

        cout << "Fib(" << idx << ") = " << value << ", время: " << duration << " сек\n";
        out2 << "Fib(" << idx << ") = " << value << ", время: " << duration << " сек\n";
    }

    out2.close();

    return 0;
}
