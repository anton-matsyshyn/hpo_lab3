#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

int isNeighbor(int i, int j, int R, int gridSize) {
    int dx = abs(i % gridSize - j % gridSize);
    int dy = abs((i / gridSize) % gridSize - (j / gridSize) % gridSize);
    int dz = abs((i / (gridSize * gridSize)) % gridSize - (j / (gridSize * gridSize)) % gridSize);

    return (dx <= R / 2) && (dy <= R / 2) && (dz <= R / 2);
}

int** generateConnectivityMatrix(int N, int R) {
    int gridSize = N / 3; // Довжина сторони кубічної сітки
    int** matrix = (int**)malloc(N * sizeof(int*));

    #pragma omp parallel for
    for (int i = 0; i < N; i++) {
        matrix[i] = (int*)malloc(N * sizeof(int));
        for (int j = 0; j < N; j++) {
            if (isNeighbor(i, j, R, gridSize)) {
                matrix[i][j] = 1;
            } else {
                matrix[i][j] = 0;
            }
        }
    }

    return matrix;
}

void printMatrix(int** matrix, int N) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            printf("%d ", matrix[i][j]);
        }
        printf("\n");
    }
}

void freeMatrix(int** matrix, int N) {
    for (int i = 0; i < N; i++) {
        free(matrix[i]);
    }
    free(matrix);
}

int main() {
    int N, R, threads;

    printf("Введіть кількість нейронів (N): ");
    scanf("%d", &N);

    printf("Введіть відстань зв'язку (R): ");
    scanf("%d", &R);

    printf("Введіть кількість потоків: ");
    scanf("%d", &threads);

    omp_set_num_threads(threads);
    double before = omp_get_wtime();

    int** connectivityMatrix = generateConnectivityMatrix(N, R);

    double after = omp_get_wtime();

    printf("Час виконання: %f\n", after - before);

    printf("Матриця зв'язаності:\n");
    printMatrix(connectivityMatrix, N);

    freeMatrix(connectivityMatrix, N);

    return 0;
}