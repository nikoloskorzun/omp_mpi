#include <iostream>
#include <vector>
#include <algorithm>
#include <omp.h>
#include <cstdlib> // Для функции rand()

// Функция для сортировки столбцов матрицы
void sortColumns(std::vector<std::vector<unsigned int>>& matrix) {
    // Размер матрицы
    const int rows = matrix.size();
    const int cols = matrix[0].size();

    // Распараллеливаем сортировку столбцов
#pragma omp parallel for
    for (int j = 0; j < cols; ++j) {
        // Сортировка каждого столбца по убыванию
        std::vector<unsigned int> column(rows);
        for (int i = 0; i < rows; ++i) {
            column[i] = matrix[i][j];
        }
        std::sort(column.begin(), column.end(), std::greater<unsigned int>());
        for (int i = 0; i < rows; ++i) {
            matrix[i][j] = column[i];
        }
    }
}

// Функция для проверки, что все столбцы отсортированы в порядке убывания
bool isSorted(const std::vector<std::vector<unsigned int>>& matrix) {
    const int rows = matrix.size();
    const int cols = matrix[0].size();

    for (int j = 0; j < cols; ++j) {
        for (int i = 1; i < rows; ++i) {
            if (matrix[i - 1][j] < matrix[i][j]) {
                return false;
            }
        }
    }
    return true;
}

int main() {
    // Создаем матрицу размером   90x60
    std::vector<std::vector<unsigned int>> matrix(90, std::vector<unsigned int>(60));

    // Заполняем матрицу случайными числами
    for (auto& row : matrix) {
        for (auto& cell : row) {
            cell = rand() % 100; // Заполняем случайными числами от   0 до   99
        }
    }

    // Сортируем столбцы матрицы по убыванию
    sortColumns(matrix);

    // Проверяем, что все столбцы отсортированы в порядке убывания
    if (isSorted(matrix)) {
        std::cout << "success" << std::endl;
    }
    else {
        std::cout << "failure" << std::endl;
    }

    return   0;
}
