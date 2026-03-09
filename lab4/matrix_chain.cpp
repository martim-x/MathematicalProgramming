#include "matrix_chain.h"
#include <climits>
#include <cstring>
#include <iostream>
#include <iomanip>
#include <string>
using namespace std;


// ============================================================
// Рекурсивный метод (наивный, без мемоизации)
// ============================================================
int matrix_chain_r(int i, int j, int n, const int c[], int* s) {
    // s[(a-1)*n+(b-1)] — позиция разбиения для пары (a,b)
    auto S = [&](int a, int b) -> int& { return s[(a - 1) * n + (b - 1)]; };

    if (i >= j) return 0;

    int best = INT_MAX;
    for (int k = i; k < j; k++) {
        int cost = matrix_chain_r(i, k, n, c, s)
            + matrix_chain_r(k + 1, j, n, c, s)
            + c[i - 1] * c[k] * c[j];
        if (cost < best) {
            best = cost;
            S(i, j) = k;
        }
    }
    return best;
}

// ============================================================
// Динамическое программирование
// m[i][j] — минимальное кол-во умножений для A_i..A_j
// ============================================================
int matrix_chain_dp(int n, const int c[], int* s) {
    int* m = new int[n * n]();

    auto M = [&](int i, int j) -> int& { return m[(i - 1) * n + (j - 1)]; };
    auto S = [&](int i, int j) -> int& { return s[(i - 1) * n + (j - 1)]; };

    // Одиночная матрица — 0 умножений
    for (int i = 1; i <= n; i++) M(i, i) = 0;

    // l — длина цепочки (от 2 до n)
    for (int l = 2; l <= n; l++) {
        for (int i = 1; i <= n - l + 1; i++) {
            int j = i + l - 1;
            M(i, j) = INT_MAX;
            for (int k = i; k <= j - 1; k++) {
                int q = M(i, k) + M(k + 1, j) + c[i - 1] * c[k] * c[j];
                if (q < M(i, j)) {
                    M(i, j) = q;
                    S(i, j) = k;
                }
            }
        }
    }

    int result = M(1, n);
    delete[] m;
    return result;
}

// ============================================================
// Рекурсивный вывод расстановки скобок по матрице S
// ============================================================
void print_brackets(int i, int j, int n, const int* s) {
    if (i == j) {
        cout << "A" << i;
    }
    else {
        int k = s[(i - 1) * n + (j - 1)];
        cout << "(";
        print_brackets(i, k, n, s);
        cout << " x ";
        print_brackets(k + 1, j, n, s);
        cout << ")";
    }
}

// ============================================================
// Вывод матрицы S
// ============================================================
void print_s_matrix(int n, const int* s) {
    cout << "\nМатрица S (позиция разбиения k для пары (i,j)):\n";
    cout << "    ";
    for (int j = 1; j <= n; j++)
        cout << setw(4) << "A" + to_string(j);
    cout << "\n   +" << string(4 * n, '-') << "\n";

    for (int i = 1; i <= n; i++) {
        cout << "A" << i << " |";
        for (int j = 1; j <= n; j++) {
            if (j >= i)
                cout << setw(4) << s[(i - 1) * n + (j - 1)];
            else
                cout << "    ";
        }
        cout << "\n";
    }
}