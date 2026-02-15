#include <iostream>
#include <chrono>
#include <cstdlib>
#include <ctime>
#include <iomanip>
#include <vector>
#include "BoatCenter.h"
#include "Combi.h"

using namespace std;
using namespace chrono;
using namespace combi;

void test_subset();
void test_combination();
void test_permutation();
void test_arrangement();

int main()
{
    test_subset();
    test_combination();
    test_permutation();
    test_arrangement();
    srand(time(0));

    // ЗАДАНИЕ 5: Решение задачи с заданными параметрами

    const short n = 8;   // общее количество контейнеров
    const short m = 5;   // количество мест на судне

    int v[n];            // веса контейнеров
    int c[n];            // доходы от перевозки

    cout << "=== ЗАДАНИЕ 5 ===" << endl << endl;

    for (int i = 0; i < n; i++)
    {
        v[i] = 100 + rand() % 101;   // вес: 100-200 кг
        c[i] = 10 + rand() % 91;     // доход: 10-100 у.е.
    }

    cout << "Контейнеры (вес кг / доход у.е.):" << endl;
    for (int i = 0; i < n; i++)
    {
        cout << "  Контейнер " << i << ": "
            << setw(3) << v[i] << " кг / "
            << setw(3) << c[i] << " у.е." << endl;
    }
    cout << endl;

    // Генерация ограничений по весу для каждого места
    int min_w[m];
    int max_w[m];

    cout << "Ограничения по весу для каждого места:" << endl;
    for (int i = 0; i < m; i++)
    {
        min_w[i] = 50 + rand() % 71;      // минимум: 50-120 кг
        max_w[i] = 150 + rand() % 701;    // максимум: 150-850 кг
        cout << "  Место " << i << ": "
            << setw(3) << min_w[i] << " кг - "
            << setw(3) << max_w[i] << " кг" << endl;
    }
    cout << endl;

    // Решение задачи
    short result[m];

    cout << "Решение задачи..." << endl;
    int profit = boat_center(m, n, v, c, min_w, max_w, result);

    // Вывод результата
    cout << endl;
    cout << "РЕЗУЛЬТАТ" << endl;
    cout << "Количество мест на судне:  " << m << endl;
    cout << "Количество контейнеров:    " << n << endl;
    cout << "Максимальная прибыль:      " << profit << " у.е." << endl;
    cout << "Выбранные контейнеры:      ";

    for (int i = 0; i < m; i++)
    {
        cout << result[i];
        if (i < m - 1) cout << ", ";
    }
    cout << endl;

    // Вывод деталей выбранных контейнеров
    cout << endl;
    cout << "Детали размещения:" << endl;
    int totalWeight = 0;
    int totalProfit = 0;

    for (int i = 0; i < m; i++)
    {
        int idx = result[i];
        cout << "  Место " << i << ": контейнер " << idx
            << " (" << v[idx] << " кг, " << c[idx] << " у.е.)"
            << " - диапазон [" << min_w[i] << "-" << max_w[i] << " кг]";

        // Проверка корректности
        if (v[idx] >= min_w[i] && v[idx] <= max_w[i])
            cout << " ✓" << endl;
        else
            cout << " ✗ ОШИБКА!" << endl;

        totalWeight += v[idx];
        totalProfit += c[idx];
    }

    cout << endl;
    cout << "Общий вес:                 " << totalWeight << " кг" << endl;
    cout << "Общая прибыль:             " << totalProfit << " у.е." << endl << endl;

    // ЗАДАНИЕ 6: Исследование зависимости времени от размерности задачи

    cout << " === ЗАДАНИЕ 6 === " << endl << endl;
    cout << "Фиксированные параметры:" << endl;
    cout << "  - Количество контейнеров: " << n << endl;
    cout << "  - Варьируем количество мест: 4-8" << endl << endl;

    // Количество итераций для усреднения
    const int ITERATIONS = 10000;

    cout << "Каждый тест выполняется " << ITERATIONS << " раз для точности измерений." << endl;
    cout << endl;

    // Проводим замеры для m от 4 до 8
    for (short places = 4; places <= 8; places++)
    {
        // Подготовка данных для текущего количества мест
        vector<int> minv(places);
        vector<int> maxv(places);
        vector<short> r(places);

        for (int i = 0; i < places; i++)
        {
            minv[i] = 50 + rand() % 71;      // минимум: 50-120 кг
            maxv[i] = 150 + rand() % 701;    // максимум: 150-850 кг
        }

        // Замер времени выполнения (усредненный)
        auto start = high_resolution_clock::now();

        // Запускаем алгоритм много раз
        for (int iter = 0; iter < ITERATIONS; iter++)
        {
            boat_center(
                places,
                n,
                v,
                c,
                minv.data(),
                maxv.data(),
                r.data()
            );
        }

        auto stop = high_resolution_clock::now();

        auto total_micro = duration_cast<microseconds>(stop - start);

        double avg_micro = static_cast<double>(total_micro.count()) / ITERATIONS;

        cout << avg_micro << " микросекунд" << endl;

    }

    return 0;
}


void test_subset()
{
    cout << " === ЗАДАНИЕ 1 === " << endl << endl;
    cout << "Тест генератора подмножеств" << endl;
    cout << endl;

    char elements[][2] = { "A", "B", "C", "D" };
    const int n = 4;

    cout << "Исходное множество: { ";
    for (int i = 0; i < n; i++)
        cout << elements[i] << (i < n - 1 ? ", " : " ");
    cout << "}" << endl;
    cout << endl;

    cout << "Генерация всех подмножеств:" << endl;

    subset s(n);
    int k = s.getfirst();
    int count = 0;

    while (k >= 0)
    {
        cout << setw(3) << count++ << ": { ";
        for (int i = 0; i < k; i++)
            cout << elements[s.ntx(i)] << (i < k - 1 ? ", " : " ");
        cout << "}" << endl;

        k = s.getnext();
    }

    cout << endl << "Всего подмножеств: " << s.count() << endl;
    cout << "Ожидаемое: 2^" << n << " = " << (1 << n) << endl << endl;

}

void test_combination()
{
    cout << " === ЗАДАНИЕ 2 === " << endl << endl;
    cout << "Тест генератора сочетаний" << endl << endl;

    char elements[][2] = { "A", "B", "C", "D", "E" };
    const int n = 5;
    const int m = 3;

    cout << "Исходное множество: { ";
    for (int i = 0; i < n; i++)
        cout << elements[i] << (i < n - 1 ? ", " : " ");
    cout << "}" << endl << endl;
    cout << "Генерация сочетаний из " << n << " по " << m << ":" << endl;

    xcombination xc(n, m);
    int k = xc.getfirst();

    while (k >= 0)
    {
        cout << setw(3) << xc.nc << ": { ";
        for (int i = 0; i < k; i++)
            cout << elements[xc.ntx(i)] << (i < k - 1 ? ", " : " ");
        cout << "}" << endl;

        k = xc.getnext();
    }

    cout << endl << "Всего сочетаний: " << xc.count() << endl;
    cout << "Ожидаемое: C(" << n << "," << m << ") = " << xc.count() << endl << endl;
}

void test_permutation()
{
    cout << " === ЗАДАНИЕ 3 === " << endl << endl;
    cout << "Тест генератора перестановок" << endl << endl;

    char elements[][2] = { "A", "B", "C", "D" };
    const int n = 4;

    cout << "Исходное множество: { ";
    for (int i = 0; i < n; i++)
        cout << elements[i] << (i < n - 1 ? ", " : " ");
    cout << "}" << endl;
    cout << endl;

    cout << "Генерация перестановок:" << endl;

    permutation p(n);
    long long k = p.getfirst();

    while (k >= 0)
    {
        cout << setw(3) << p.np << ": { ";
        for (int i = 0; i < n; i++)
            cout << elements[p.ntx(i)] << (i < n - 1 ? ", " : " ");
        cout << "}" << endl;

        k = p.getnext();
    }


    cout << endl << "Всего перестановок: " << p.count() << endl;
    cout << "Ожидаемое: " << n << "! = " << p.count() << endl << endl;

}

void test_arrangement()
{
    cout << " === ЗАДАНИЕ 4 === " << endl << endl;
    cout << "Тест генератора размещений" << endl << endl;

    char elements[][2] = { "A", "B", "C", "D" };
    const int n = 4;
    const int m = 3;

    cout << "Исходное множество: { ";
    for (int i = 0; i < n; i++)
        cout << elements[i] << (i < n - 1 ? ", " : " ");
    cout << "}" << endl;
    cout << endl;

    cout << "Генерация размещений из " << n << " по " << m << ":" << endl;

    arrangement a(n, m);
    int k = a.getfirst();
    int count = 0;

    while (k >= 0)
    {
        cout << setw(3) << count++ << ": { ";
        for (int i = 0; i < m; i++)
            cout << elements[a.ntx(i)] << (i < m - 1 ? ", " : " ");
        cout << "}" << endl;

        k = a.getnext();
    }


    cout << endl << "Всего размещений: " << count << endl;

    // Вычисление A(n,m) = n!/(n-m)!
    unsigned long long expected = 1;
    for (int i = n; i > n - m; i--)
        expected *= i;

    cout << "Ожидаемое: A(" << n << "," << m << ") = " << expected << endl << endl;

}