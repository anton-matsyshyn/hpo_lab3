#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <omp.h>

// Функція для обчислення Евклідової відстані між двома точками у тривимірному просторі
double calculateDistance(int x1, int y1, int z1, int x2, int y2, int z2) {
    int dx = abs(x1 - x2);
    int dy = abs(y1 - y2);
    int dz = abs(z1 - z2);
    return sqrt(dx*dx + dy*dy + dz*dz);
}

// Функція для генерації матриці зв'язаності
int** generateConnectivityMatrix(int N, int R, int randomSeed) {
    int gridSize = cbrt(N); // Довжина сторони кубічної сітки
    int** matrix = (int**)malloc(N * sizeof(int*));

    // Генерація випадкових координат нейронів
    srand(randomSeed);
    int* neuronX = (int*)malloc(N * sizeof(int));
    int* neuronY = (int*)malloc(N * sizeof(int));
    int* neuronZ = (int*)malloc(N * sizeof(int));

    #pragma omp parallel for
    for (int i = 0; i < N; i++) {
        neuronX[i] = rand() % gridSize;
        neuronY[i] = rand() % gridSize;
        neuronZ[i] = rand() % gridSize;
    }

    // Генерація матриці зв'язаності
    #pragma omp parallel for
    for (int i = 0; i < N; i++) {
        matrix[i] = (int*)malloc(N * sizeof(int));
        for (int j = 0; j < N; j++) {
            if (calculateDistance(neuronX[i], neuronY[i], neuronZ[i], neuronX[j], neuronY[j], neuronZ[j]) <= R) {
                matrix[i][j] = 1;
            } else {
                matrix[i][j] = 0;
            }
        }
    }

    free(neuronX);
    free(neuronY);
    free(neuronZ);

    return matrix;
}

// Функція для виведення матриці на екран
void printMatrix(int** matrix, int N) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            printf("%d ", matrix[i][j]);
        }
        printf("\n");
    }
}

// Функція для звільнення пам'яті, виділеної під матрицю
void freeMatrix(int** matrix, int N) {
    for (int i = 0; i < N; i++) {
        free(matrix[i]);
    }
    free(matrix);
}

int main() {
    int N, R, randomSeed, threads;

    printf("Введіть кількість нейронів (N): ");
    scanf("%d", &N);

    printf("Введіть радіус зв'язності (R): ");
    scanf("%d", &R);

    printf("Введіть значення randomSeed (0 для випадкових чисел, інакше для однакових чисел): ");
    scanf("%d", &randomSeed);

    printf("Введіть кількість потоків, що буде використана: ");
    scanf("%d", &threads);

    omp_set_num_threads(threads);

    double before = omp_get_wtime();

    int** connectivityMatrix = generateConnectivityMatrix(N, R, randomSeed);

    double after = omp_get_wtime();
    
    printf("Час виконання %f\n", after - before);

    freeMatrix(connectivityMatrix, N);

    return 0;
}