#include "levenshtein.h"
#include <algorithm>
#include <iostream>
#include <iomanip>
#include <vector>
#include <map>
#include <string>
using namespace std;


// ============================================================
// Вспомогательная функция
// ============================================================
static inline int min3(int a, int b, int c) {
    return min({ a, b, c });
}

// ============================================================
// Динамическое программирование
// d[i][j] = L(prefix(x,i), prefix(y,j))
// ============================================================
int levenshtein_dp(int lx, const char x[], int ly, const char y[]) {
    int* d = new int[(lx + 1) * (ly + 1)];
    // Индексирование: d[i*(ly+1)+j]
    auto D = [&](int i, int j) -> int& { return d[i * (ly + 1) + j]; };

    for (int i = 0; i <= lx; i++) D(i, 0) = i;
    for (int j = 0; j <= ly; j++) D(0, j) = j;

    for (int i = 1; i <= lx; i++)
        for (int j = 1; j <= ly; j++)
            D(i, j) = min3(
                D(i - 1, j) + 1,
                D(i, j - 1) + 1,
                D(i - 1, j - 1) + (x[i - 1] == y[j - 1] ? 0 : 1)
            );

    int result = D(lx, ly);
    delete[] d;
    return result;
}

// ============================================================
// Рекурсия (без мемоизации — экспоненциальная сложность)
// ============================================================
int levenshtein_r(int lx, const char x[], int ly, const char y[]) {
    if (lx == 0) return ly;
    if (ly == 0) return lx;
    if (lx == 1 && ly == 1) return (x[0] == y[0]) ? 0 : 1;
    return min3(
        levenshtein_r(lx - 1, x, ly, y) + 1,
        levenshtein_r(lx, x, ly - 1, y) + 1,
        levenshtein_r(lx - 1, x, ly - 1, y) + (x[lx - 1] == y[ly - 1] ? 0 : 1)
    );
}

// ============================================================
// Задание 4: Пошаговый вывод
// Работает с UTF-8 строками (кириллица = 2 байта на символ)
// ============================================================

// Разбить UTF-8 строку на вектор символов
static vector<string> split_utf8(const string& s) {
    vector<string> chars;
    for (size_t i = 0; i < s.size(); ) {
        unsigned char c = (unsigned char)s[i];
        int len = 1;
        if ((c & 0x80) == 0x00) len = 1;
        else if ((c & 0xE0) == 0xC0) len = 2;
        else if ((c & 0xF0) == 0xE0) len = 3;
        else if ((c & 0xF8) == 0xF0) len = 4;
        chars.push_back(s.substr(i, (size_t)len));
        i += (size_t)len;
    }
    return chars;
}

static string join_utf8(const vector<string>& v) {
    string s;
    for (const auto& c : v) s += c;
    return s;
}

using CharVec = vector<string>;

static int  g_step = 0;
static map<pair<string, string>, int> g_memo;

static int trace_impl(const CharVec& xv, const CharVec& yv) {
    string xs = join_utf8(xv);
    string ys = join_utf8(yv);
    auto key = make_pair(xs, ys);

    // Уже вычислено — берём из кэша (не выводим)
    auto it = g_memo.find(key);
    if (it != g_memo.end()) return it->second;

    // Базовые случаи
    if (xv.empty()) {
        int r = (int)yv.size();
        cout << "    L(\"\", \"" << ys << "\") = " << r
            << "  [базовый: X пустая]\n";
        return g_memo[key] = r;
    }
    if (yv.empty()) {
        int r = (int)xv.size();
        cout << "    L(\"" << xs << "\", \"\") = " << r
            << "  [базовый: Y пустая]\n";
        return g_memo[key] = r;
    }
    if (xv.size() == 1 && yv.size() == 1) {
        int r = (xv[0] == yv[0]) ? 0 : 1;
        cout << "    L(\"" << xs << "\", \"" << ys << "\") = " << r
            << "  [базовый: одиночные символы, "
            << xs << (r == 0 ? "==" : "!=") << ys << "]\n";
        return g_memo[key] = r;
    }

    // Общий случай
    CharVec cx(xv.begin(), xv.end() - 1); // cut(X)
    CharVec cy(yv.begin(), yv.end() - 1); // cut(Y)
    bool eq = (xv.back() == yv.back());
    int  cost = eq ? 0 : 1;
    string cxs = join_utf8(cx);
    string cys = join_utf8(cy);

    g_step++;
    int snum = g_step;

    cout << "\n" << snum << ". L(\"" << xs << "\", \"" << ys << "\") =\n";
    cout << "   = min {\n";
    cout << "       L(\"" << cxs << "\", \"" << ys << "\") + 1,    [удалить из X]\n";
    cout << "       L(\"" << xs << "\", \"" << cys << "\") + 1,    [удалить из Y]\n";
    cout << "       L(\"" << cxs << "\", \"" << cys << "\") + " << cost
        << "     [last(X)=" << xv.back() << (eq ? "==" : "!=")
        << yv.back() << "=last(Y)]\n";
    cout << "     }\n";

    int r1 = trace_impl(cx, yv);  // удаление из X
    int r2 = trace_impl(xv, cy);  // удаление из Y
    int r3 = trace_impl(cx, cy);  // замена / совпадение

    int result = min3(r1 + 1, r2 + 1, r3 + cost);

    cout << snum << ". L(\"" << xs << "\", \"" << ys << "\") = "
        << "min(" << r1 + 1 << ", " << r2 + 1 << ", " << r3 + cost
        << ") = " << result << "\n";

    return g_memo[key] = result;
}

void levenshtein_trace(const string& x, const string& y) {
    g_step = 0;
    g_memo.clear();

    CharVec xv = split_utf8(x);
    CharVec yv = split_utf8(y);

    cout << "\n=== ЗАДАНИЕ 4: Пошаговое вычисление L(\""
        << x << "\", \"" << y << "\") ===\n";
    cout << "len(\"" << x << "\") = " << xv.size()
        << ",  len(\"" << y << "\") = " << yv.size() << "\n";
    cout << "cut(X) = X без последнего символа,  last(X) = последний символ X\n";

    int result = trace_impl(xv, yv);

    cout << "\n>>> ИТОГ: L(\"" << x << "\", \"" << y << "\") = "
        << result << "\n";
    cout << "Всего шагов раскрытия (уникальных подзадач): " << g_step << "\n";
}