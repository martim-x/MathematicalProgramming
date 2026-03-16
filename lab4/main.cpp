#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <ctime>
#include <cstring>
#include <chrono>
#include <string>
#include <vector>
#include <algorithm>
#include <cmath>

#include "levenshtein.h"
#include "matrix_chain.h"
using namespace std;

// ============================================================
// Глобальные строки (генерируются в задании 1)
// ============================================================
static const int LEN1 = 300;
static const int LEN2 = 200;
static char S1[LEN1 + 1];
static char S2[LEN2 + 1];

// ============================================================
// ЗАДАНИЕ 1: Генерация случайных строк
// ============================================================
void task_1() {
    cout << "\n";
    cout << "============================================================\n";
    cout << " ЗАДАНИЕ 1: Генерация случайных строк латинского алфавита\n";
    cout << "============================================================\n";

    srand(time(0));

    for (int i = 0; i < LEN1; i++) S1[i] = 'a' + rand() % 26;
    S1[LEN1] = '\0';

    for (int i = 0; i < LEN2; i++) S2[i] = 'a' + rand() % 26;
    S2[LEN2] = '\0';

    cout << "\nS1 (" << LEN1 << " символов):\n";
    for (int i = 0; i < LEN1; i++) {
        cout << S1[i];
        if ((i + 1) % 60 == 0) cout << "\n";
    }
    cout << "\n";

    cout << "\nS2 (" << LEN2 << " символов):\n";
    for (int i = 0; i < LEN2; i++) {
        cout << S2[i];
        if ((i + 1) % 60 == 0) cout << "\n";
    }
    cout << "\n";
}

// ============================================================
// ЗАДАНИЯ 2 & 3: Дистанция Левенштейна для префиксов
// k ∈ {1/25, 1/20, 1/15, 1/10, 1/5, 1/2, 1}
// ============================================================
void task_2_3() {
    cout << "\n";
    cout << "============================================================\n";
    cout << " ЗАДАНИЯ 2 & 3: Дистанция Левенштейна — рекурсия vs ДП\n";
    cout << "============================================================\n";

    // Значения k
    const double  ks[] = { 1.0 / 25, 1.0 / 20, 1.0 / 15, 1.0 / 10, 1.0 / 5, 1.0 / 2, 1.0 };
    const char* knames[] = { "1/25","1/20","1/15","1/10","1/5","1/2","1" };
    const int     NK = 7;
    // При суммарной длине > MAX_REC рекурсия нефинансово медленна
    const int     MAX_REC = 25;
    const int     REPEAT = 5;   // повторений для замера

    struct Row {
        const char* kname;
        int l1, l2;
        double time_r, time_dp;
        int    dist_r, dist_dp;
        bool   feasible; // ограничение для рекурсии
    };
    vector<Row> rows;

    for (int ki = 0; ki < NK; ki++) {
        Row row;
        row.kname = knames[ki];
        row.l1 = max(1, (int)(ks[ki] * LEN1));
        row.l2 = max(1, (int)(ks[ki] * LEN2));
        row.feasible = (row.l1 + row.l2 <= MAX_REC);

        // --- Замер ДП ---
        double acc_dp = 0;
        for (int r = 0; r < REPEAT; r++) {
            auto t1 = chrono::high_resolution_clock::now();
            row.dist_dp = levenshtein_dp(row.l1, S1, row.l2, S2);
            auto t2 = chrono::high_resolution_clock::now();
            acc_dp += chrono::duration<double, micro>(t2 - t1).count();
        }
        row.time_dp = acc_dp / REPEAT;

        // --- Замер рекурсии (только для небольших размеров) ---
        if (row.feasible) {
            double acc_r = 0;
            for (int r = 0; r < REPEAT; r++) {
                auto t1 = chrono::high_resolution_clock::now();
                row.dist_r = levenshtein_r(row.l1, S1, row.l2, S2);
                auto t2 = chrono::high_resolution_clock::now();
                acc_r += chrono::duration<double, micro>(t2 - t1).count();
            }
            row.time_r = acc_r / REPEAT;
        }
        else {
            row.dist_r = -1;
            row.time_r = -1.0;
        }
        rows.push_back(row);
    }

    // --- Таблица результатов ---
    cout << "\n";
    cout << left
        << setw(6) << "k"
        << setw(7) << "len1"
        << setw(7) << "len2"
        << setw(18) << "Рекурсия (мс)"
        << setw(14) << "ДП (мс)"
        << setw(9) << "dist_r"
        << setw(9) << "dist_dp"
        << "\n";
    cout << string(70, '-') << "\n";

    for (auto& row : rows) {
        cout << left
            << setw(6) << row.kname
            << setw(7) << row.l1
            << setw(7) << row.l2;
        if (row.feasible) {
            cout << fixed << setprecision(4)
                << setw(18) << row.time_r;
        }
        else {
            cout << setw(18) << "слишком медленно";
        }
        cout << fixed << setprecision(4)
            << setw(14) << row.time_dp
            << setw(9) << (row.feasible ? to_string(row.dist_r) : "-")
            << setw(9) << row.dist_dp
            << "\n";
    }
    cout << "\nПримечание: рекурсия не запускается при len1+len2 > " << MAX_REC
        << " из-за экспоненциального роста времени O(3^min(len1,len2)).\n";


}

// ============================================================
// ЗАДАНИЕ 4: Вычисление L("тень", "день") пошагово
// ============================================================
void task_4() {
    cout << "\n";
    cout << "============================================================\n";
    cout << " ЗАДАНИЕ 4: Рекурсивный алгоритм для слов 'тень' и 'день'\n";
    cout << "============================================================\n";

    // Строки в кодировке UTF-8
    levenshtein_trace("тень", "день");
}

// ============================================================
// ЗАДАНИЕ 5 (чётный вариант 6):
// Расстановка скобок — матрицы 10x22, 22x40, 40x33, 33x14, 14x20, 20x30
// ============================================================
void task_5() {
    cout << "\n";
    cout << "============================================================\n";
    cout << " ЗАДАНИЕ 5: Оптимальная расстановка скобок (вариант 6)\n";
    cout << " Матрицы: 10x22, 22x40, 40x33, 33x14, 14x20, 20x30\n";
    cout << "============================================================\n";

    const int N = 6;
    int c[N + 1] = { 10, 22, 40, 33, 14, 20, 30 };
    int s_r[N * N] = {};
    int s_dp[N * N] = {};

    // --- Рекурсивный метод ---
    cout << "\n--- Рекурсивный метод ---\n";
    auto t1 = chrono::high_resolution_clock::now();
    int res_r = matrix_chain_r(1, N, N, c, s_r);
    auto t2 = chrono::high_resolution_clock::now();
    double tr = chrono::duration<double, milli>(t2 - t1).count();

    cout << "Минимальное кол-во умножений: " << res_r << "\n";
    cout << "Время: " << fixed << setprecision(4) << tr << " мс\n";
    print_s_matrix(N, s_r);
    cout << "Оптимальное выражение: ";
    print_brackets(1, N, N, s_r);
    cout << "\n";

    // --- ДП ---
    cout << "\n--- Динамическое программирование ---\n";
    auto t3 = chrono::high_resolution_clock::now();
    int res_dp = matrix_chain_dp(N, c, s_dp);
    auto t4 = chrono::high_resolution_clock::now();
    double tdp = chrono::duration<double, milli>(t4 - t3).count();

    cout << "Минимальное кол-во умножений: " << res_dp << "\n";
    cout << "Время: " << fixed << setprecision(4) << tdp << " мс\n";
    print_s_matrix(N, s_dp);
    cout << "Оптимальное выражение: ";
    print_brackets(1, N, N, s_dp);
    cout << "\n";

    // --- Сравнение времени при n = 2..6 матриц ---
    cout << "\n--- Сравнительный анализ по числу матриц n ---\n";
    const int REPEAT = 10000;

    cout << left
        << setw(5) << "n"
        << setw(20) << "Рекурсия (мкс)"
        << setw(20) << "ДП (мкс)"
        << setw(14) << "Мин. опер."
        << "\n" << string(59, '-') << "\n";

    vector<double> t_rec_all, t_dp_all;

    for (int n = 2; n <= N; n++) {
        int* sr = new int[n * n]();
        int* sd = new int[n * n]();

        // Рекурсия
        auto tr1 = chrono::high_resolution_clock::now();
        int  rr = 0;
        for (int rep = 0; rep < REPEAT; rep++) {
            memset(sr, 0, sizeof(int) * (size_t)(n * n));
            rr = matrix_chain_r(1, n, n, c, sr);
        }
        auto tr2 = chrono::high_resolution_clock::now();
        double t_r = chrono::duration<double, micro>(tr2 - tr1).count() / REPEAT;

        // ДП
        auto td1 = chrono::high_resolution_clock::now();
        for (int rep = 0; rep < REPEAT; rep++) {
            memset(sd, 0, sizeof(int) * (size_t)(n * n));
            matrix_chain_dp(n, c, sd);
        }
        auto td2 = chrono::high_resolution_clock::now();
        double t_d = chrono::duration<double, micro>(td2 - td1).count() / REPEAT;

        t_rec_all.push_back(t_r);
        t_dp_all.push_back(t_d);

        cout << left
            << setw(5) << n
            << fixed << setprecision(4)
            << setw(20) << t_r
            << setw(20) << t_d
            << setw(14) << rr
            << "\n";

        delete[] sr;
        delete[] sd;
    }

    // --- Объяснение расстановки скобок ---
    cout << "\n--- Объяснение расстановки скобок по матрице S ---\n";
    cout << "Матрицы: A1(10x22), A2(22x40), A3(40x33),\n";
    cout << "         A4(33x14), A5(14x20), A6(20x30)\n\n";
    cout << "Матрица S[i][j] = k означает:\n";
    cout << "  произведение A_i * ... * A_j оптимально разбивается\n";
    cout << "  на (A_i * ... * A_k) и (A_(k+1) * ... * A_j)\n\n";
    cout << "Восстановление скобок (рекурсивно по S):\n";

    // Распечатаем ключевые значения S
    for (int i = 1; i <= N; i++)
        for (int j = i + 1; j <= N; j++) {
            int k = s_dp[(i - 1) * N + (j - 1)];
            cout << "  S[" << i << "][" << j << "] = " << k
                << "  =>  (A" << i << "..A" << k
                << ") x (A" << k + 1 << "..A" << j << ")\n";
        }

    cout << "\nОптимальное выражение: ";
    print_brackets(1, N, N, s_dp);
    cout << "\nМинимальное число операций умножения: " << res_dp << "\n";
}

// ============================================================
// main
// ============================================================
int main() {
    task_1();
    task_2_3();
    task_4();
    task_5();

    cout << "\n============================================================\n";
    cout << " Выполнение завершено. Нажмите Enter для выхода.\n";
    cout << "============================================================\n";
    cin.get();
    return 0;
}