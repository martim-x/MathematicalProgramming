#pragma once
#include <string>

// Дистанция Левенштейна — Динамическое программирование
int levenshtein_dp(int lx, const char x[], int ly, const char y[]);

// Дистанция Левенштейна — Рекурсия (без мемоизации, наивная)
int levenshtein_r(int lx, const char x[], int ly, const char y[]);

// Задание 4: пошаговый вывод рекурсивного алгоритма (UTF-8 строки)
void levenshtein_trace(const std::string& x, const std::string& y);