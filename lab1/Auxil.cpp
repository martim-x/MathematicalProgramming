#include "Auxil.h"

// Инициализация генератора случайных чисел
void start() {
    srand((time(0)));
}

// Возвращает случайное число double в диапазоне [0,1]
double dget() {
    return (double)(rand()) / RAND_MAX;
}

// Возвращает случайное целое число
int iget() {
    return rand();
}
