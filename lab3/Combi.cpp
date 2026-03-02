#include "Combi.h"
#include <algorithm>

using namespace combi;

// МЕТОДЫ ГЕНЕРАТОРА ПОДМНОЖЕСТВ

subset::subset(short n)
{
    this->n = n;
    this->sset = new short[n];
    reset();
}

void subset::reset()
{
    this->sn = 0;
    this->mask = 0;
}

// Сформировать массив индексов по текущей битовой маске
short subset::getfirst()
{
    unsigned long long buf = this->mask;
    this->sn = 0;
    // Проходим по всем битам маски
    for (short i = 0; i < n; i++)
    {
        if (buf & 1) // если бит установлен
            this->sset[this->sn++] = i; // добавляем индекс в подмножество
        buf >>= 1; // сдвиг вправо
    }
    return this->sn;
}

// Перейти к следующему подмножеству
short subset::getnext()
{
    this->sn = 0;
    if (++this->mask >= count())
        return -1; // все подмножества перебраны
    return getfirst();
}

short subset::ntx(short i)
{
    return this->sset[i];
}

// Количество подмножеств = 2^n
unsigned long long subset::count()
{
    return 1ULL << n;
}

// МЕТОДЫ ГЕНЕРАТОР СОЧЕТАНИЙ (эвристический алгоритм)

xcombination::xcombination(short n, short m)
{
    this->n = n;
    this->m = m;
    this->sset = new short[m + 2]; // +2 для граничных значений
    reset();
}

void xcombination::reset()
{
    this->nc = 0;
    // Первое сочетание: 0, 1, 2, ..., m-1
    for (int i = 0; i < this->m; i++)
        this->sset[i] = i;
    // Граничные значения для алгоритма
    this->sset[m] = this->n;
    this->sset[m + 1] = 0;
}

short xcombination::getfirst()
{
    return (this->n >= this->m) ? this->m : -1;
}

// Алгоритм генерации следующего сочетания
short xcombination::getnext()
{
    short rc = getfirst();
    if (rc > 0)
    {
        short j;
        // Ищем правый элемент, который можно увеличить
        for (j = 0; this->sset[j] + 1 == this->sset[j + 1]; ++j)
            this->sset[j] = j; // Сбрасываем элементы слева

        if (j >= this->m)
            rc = -1; // Все сочетания перебраны
        else
        {
            this->sset[j]++; // Увеличиваем элемент
            this->nc++;
        }
    }
    return rc;
}

short xcombination::ntx(short i)
{
    return this->sset[i];
}

// Вспомогательная функция: факториал
unsigned long long fact(unsigned long long x)
{
    return (x == 0) ? 1 : (x * fact(x - 1));
}

// Количество сочетаний: C(n,m) = n! / (m! * (n-m)!)
unsigned long long xcombination::count() const
{
    return (this->n >= this->m) ?
        fact(this->n) / (fact(this->n - this->m) * fact(this->m)) : 0;
}

// МЕТОДЫ ГЕНЕРАТОРА ПЕРЕСТАНОВОК (алгоритм std::next_permutation)

permutation::permutation(short n)
{
    this->n = n;
    sset = new short[n];
    dart = new bool[n];
    reset();
}

void permutation::reset()
{
    getfirst();
}

// Первая перестановка: 0, 1, 2, ..., n-1
long long permutation::getfirst()
{
    np = 0;
    for (int i = 0; i < n; i++)
    {
        sset[i] = i;
        dart[i] = L; // все стрелки влево
    }
    return 0;
}

// Генерация следующей перестановки (используем STL)
long long permutation::getnext()
{
    if (!std::next_permutation(sset, sset + n))
        return -1; // все перестановки перебраны
    return ++np;
}

short permutation::ntx(short i)
{
    return sset[i];
}

// Количество перестановок = n!
unsigned long long permutation::count() const
{
    unsigned long long f = 1;
    for (int i = 2; i <= n; i++)
        f *= i;
    return f;
}

// МЕТОДЫ ГЕНЕРАТОРА РАЗМЕЩЕНИЙ

arrangement::arrangement(short n, short m)
{
    this->n = n;
    this->m = m;
    p = new permutation(n); // создаем генератор перестановок
}

short arrangement::getfirst()
{
    p->getfirst();
    return m;
}

short arrangement::getnext()
{
    if (p->getnext() < 0)
        return -1;
    return m;
}

short arrangement::ntx(short i)
{
    return p->ntx(i); // берем первые m элементов перестановки
}