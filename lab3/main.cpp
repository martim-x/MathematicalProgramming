
#include <iostream>
#include <vector>
#include <climits>
#include <string>
#include "Combi.h" 

using namespace std;
using namespace combi;


const int N = 5;
const int INF = INT_MAX / 2;

int DIST[N][N] = {
    {INF,  6,  25,  9,  16},
    { 7, INF,  18, 34,  62},
    {13,  12, INF, 72,  43},
    {24,  49,  18, INF, 12},
    {79,  38,  45, 18, INF}
};


void printMatrix(int m[N][N], const string& title = "")
{
    if (!title.empty()) cout << "\n[" << title << "]\n";
    cout << "      ";
    for (int j = 0; j < N; j++) printf("  %2d ", j + 1);
    cout << "\n";
    for (int i = 0; i < N; i++) {
        printf("  %2d  ", i + 1);
        for (int j = 0; j < N; j++) {
            if (m[i][j] >= INF) printf("  INF");
            else                 printf("  %3d", m[i][j]);
        }
        cout << "\n";
    }
}

void copyMatrix(int src[N][N], int dst[N][N])
{
    for (int i = 0; i < N; i++)
        for (int j = 0; j < N; j++)
            dst[i][j] = src[i][j];
}

// Приведение матрицы по строкам и столбцам.
// Возвращает сумму констант приведения.
int reduceMatrix(int m[N][N])
{
    cout << "minVal + curTotal = total" << endl;
    int total = 0;
    // По строкам
    for (int i = 0; i < N; i++) {
        int minVal = INF;
        for (int j = 0; j < N; j++) if (m[i][j] < minVal) minVal = m[i][j];
        if (minVal > 0 && minVal < INF) {
            cout << minVal << " + " << total << " = " << minVal + total << endl;
            total += minVal;
            for (int j = 0; j < N; j++) if (m[i][j] < INF) m[i][j] -= minVal;
        }
    }
    // По столбцам
    for (int j = 0; j < N; j++) {
        int minVal = INF;
        for (int i = 0; i < N; i++) if (m[i][j] < minVal) minVal = m[i][j];
        if (minVal > 0 && minVal < INF) {
            cout << minVal << " + " << total << " = " << minVal + total << endl;
            total += minVal;
            for (int i = 0; i < N; i++) if (m[i][j] < INF) m[i][j] -= minVal;
        }
    }
    return total;
}

// Штраф за исключение дуги (row, col): min строки i (без j) + min столбца j (без i)
int calcPenalty(int m[N][N], int row, int col)
{
    int minRow = INF;
    for (int j = 0; j < N; j++)
        if (j != col && m[row][j] < minRow) minRow = m[row][j];
    int minCol = INF;
    for (int i = 0; i < N; i++)
        if (i != row && m[i][col] < minCol) minCol = m[i][col];
    if (minRow >= INF) minRow = 0;
    if (minCol >= INF) minCol = 0;
    cout << "calc penalty " << minRow << " " << minCol << endl;
    return minRow + minCol;

}

// Вычеркнуть строку row и столбец col; запретить обратную дугу (col->row)
void eliminateRowCol(int m[N][N], int row, int col)
{
    for (int k = 0; k < N; k++) { m[row][k] = INF; m[k][col] = INF; }
    m[col][row] = INF;
}

// ─────────────────────────────────────────────────────────
// Узел дерева ветвлений
// ─────────────────────────────────────────────────────────
struct Node {
    int matrix[N][N];           // текущая матрица (после вычёркиваний)
    int bound;                  // нижняя граница этого узла
    vector<pair<int, int>> path; // какие дуги уже включили
    int level;                  // глубина в дереве
};

int bestCost = INF;
vector<pair<int, int>> bestPath;

// ─────────────────────────────────────────────────────────
// Рекурсивный алгоритм ветвей и границ
// ─────────────────────────────────────────────────────────
void branchAndBound(Node& node)
{
    // Маршрут полностью сформирован
    if ((int)node.path.size() == N) {
        if (node.bound < bestCost) {
            bestCost = node.bound;
            bestPath = node.path;
        }
        return;
    }

    // Обрезка ветви
    if (node.bound >= bestCost) return;

    // Выбираем нулевой элемент с максимальным штрафом
    int bestPenalty = -1, bestRow = -1, bestCol = -1;
    for (int i = 0; i < N; i++)
        for (int j = 0; j < N; j++)
            if (node.matrix[i][j] == 0) {
                int pen = calcPenalty(node.matrix, i, j);
                if (pen > bestPenalty) {
                    bestPenalty = pen; bestRow = i; bestCol = j;
                }
            }

    if (bestRow == -1) return;

    printf("  Ветвление по дуге (%d->%d), штраф=%d, граница=%d\n",
        bestRow + 1, bestCol + 1, bestPenalty, node.bound);

    // ── ВКЛЮЧИТЬ дугу (bestRow -> bestCol) ───────────────
    {
        Node child;
        copyMatrix(node.matrix, child.matrix);
        child.path = node.path;
        child.path.push_back({ bestRow, bestCol });
        child.level = node.level + 1;
        eliminateRowCol(child.matrix, bestRow, bestCol);
        child.bound = node.bound + reduceMatrix(child.matrix);
        printf("    [ДА] (%d->%d) включена, граница=%d\n",
            bestRow + 1, bestCol + 1, child.bound);
        branchAndBound(child);
    }

    // ── ИСКЛЮЧИТЬ дугу (bestRow -> bestCol) ──────────────
    {
        Node child;
        copyMatrix(node.matrix, child.matrix);
        child.path = node.path;
        child.level = node.level + 1;
        child.matrix[bestRow][bestCol] = INF;
        child.bound = node.bound + bestPenalty + reduceMatrix(child.matrix);
        printf("    [НЕТ] (%d->%d) исключена, граница=%d\n",
            bestRow + 1, bestCol + 1, child.bound);
        branchAndBound(child);
    }
}

// Восстановить последовательный маршрут из списка дуг
vector<int> buildTour(const vector<pair<int, int>>& edges)
{
    vector<int> tour;
    if ((int)edges.size() != N) return tour;
    int cur = 0;
    tour.push_back(0);
    for (int step = 0; step < N - 1; step++)
        for (auto& e : edges)
            if (e.first == cur) { cur = e.second; tour.push_back(cur); break; }
    return tour;
}

// ─────────────────────────────────────────────────────────
// Задание 3: Проверка через генератор перестановок (Combi.h)
// ─────────────────────────────────────────────────────────
void checkWithPermutations()
{
    cout << "\n=========================================\n";
    cout << " ЗАДАНИЕ 3: ПРОВЕРКА ГЕНЕРАТОРОМ ПЕРЕСТАНОВОК\n";
    cout << "=========================================\n";
    cout << "(combi::permutation, город 1 фиксирован)\n\n";

    // Перебираем все перестановки городов с индексами 1..4
    // (город 0 = город 1 фиксирован как начальный)
    permutation perm(N - 1);
    perm.getfirst();

    int minDist = INF;
    vector<int> bestTour;
    long long checked = 0;

    do {
        // Маршрут: 0 -> perm[0]+1 -> perm[1]+1 -> ... -> 0
        vector<int> tour;
        tour.push_back(0);
        for (int i = 0; i < N - 1; i++)
            tour.push_back(perm.ntx(i) + 1);

        int dist = 0; bool valid = true;
        for (int i = 0; i < N; i++) {
            int from = tour[i], to = tour[(i + 1) % N];
            if (DIST[from][to] >= INF) { valid = false; break; }
            dist += DIST[from][to];
        }

        if (valid && dist < minDist) {
            minDist = dist;
            bestTour = tour;
        }
        checked++;
    } while (perm.getnext() >= 0);

    printf("Проверено маршрутов: %lld\n", checked);
    cout << "Минимальное расстояние: " << minDist << "\n";
    cout << "Оптимальный маршрут:    ";
    for (int c : bestTour) cout << c + 1 << " -> ";
    cout << bestTour[0] + 1 << "\n";
}

// ─────────────────────────────────────────────────────────
// MAIN
// ─────────────────────────────────────────────────────────
int main()
{
    cout << "=========================================\n";
    cout << " ЗАДАЧА КОММИВОЯЖЕРА, n = 6 (вариант 6)\n";
    cout << "=========================================\n";

    printMatrix(DIST, "Исходная матрица расстояний");

    // ── Корневой узел ─────────────────────────────────────
    Node root;
    copyMatrix(DIST, root.matrix);
    root.level = 0;
    root.bound = reduceMatrix(root.matrix);

    printMatrix(root.matrix, "Матрица после приведения (корень)");
    cout << "\nНачальная нижняя граница (γ) = " << root.bound << "\n";

    cout << "\n--- Процесс ветвления ---\n";
    branchAndBound(root);

    // ── Результат B&B ─────────────────────────────────────
    cout << "\n=========================================\n";
    cout << " РЕЗУЛЬТАТ: МЕТОД ВЕТВЕЙ И ГРАНИЦ\n";
    cout << "=========================================\n";
    cout << "Минимальное расстояние: " << bestCost << "\n";
    cout << "Включённые дуги:        ";
    for (auto& e : bestPath) printf("(%d->%d) ", e.first + 1, e.second + 1);
    cout << "\n";

    auto tour = buildTour(bestPath);
    cout << "Маршрут:                ";
    for (int c : tour) cout << c + 1 << " -> ";
    if (!tour.empty()) cout << tour[0] + 1;
    cout << "\n";

    // ── Задание 3: проверка перестановками ───────────────
    checkWithPermutations();

    return 0;
}