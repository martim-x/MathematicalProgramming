#pragma once
#include <ctime>   // для clock
#include <cstdlib> // для rand()

// Инициализация генератора случайных чисел
void start();

// Возвращает случайное число double от 0 до 1
double dget();

// Возвращает случайное число int
int iget();
