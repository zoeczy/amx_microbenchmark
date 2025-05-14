#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

void print_matrix_to_file(void* matrix, size_t rows, size_t cols, size_t element_size, const char* filename) {
    FILE* file = fopen(filename, "w");
    if (file == NULL) {
        perror("Failed to open file");
        return;
    }

    if (element_size == sizeof(int8_t)) {
        int8_t* mat = (int8_t*)matrix;
        fprintf(file, "Matrix (int8_t):\n");
        for (size_t i = 0; i < rows; ++i) {
            for (size_t j = 0; j < cols; ++j) {
                fprintf(file, "%4d ", mat[i * cols + j]); // 打印 int8_t 类型的值
            }
            fprintf(file, "\n");
        }
    } else if (element_size == sizeof(int32_t)) {
        int32_t* mat = (int32_t*)matrix;
        fprintf(file, "Matrix (int32_t):\n");
        for (size_t i = 0; i < rows; ++i) {
            for (size_t j = 0; j < cols; ++j) {
                fprintf(file, "%8d ", mat[i * cols + j]); // 打印 int32_t 类型的值
            }
            fprintf(file, "\n");
        }
    } else {
        fprintf(file, "Unsupported element size: %zu\n", element_size);
    }

    fclose(file);
}

void init_matrices(int8_t *A, int8_t *B, int32_t *C, size_t M, size_t N, size_t K) 
{
    for (int i = 0; i < M*K; i++) 
        A[i] = (int8_t)(i % 10 + 1);

    for (int i = 0; i < K*N; i++) 
        B[i] = (int8_t)(i % 10 + 1);

    for (int i = 0; i < M*N; i++) 
            C[i] = 0;
}

// 矩阵乘法验证函数
bool verify_matrix_multiplication(int8_t* A, int8_t* B, int32_t* C, size_t M, size_t N, size_t K) {
    // 手动计算矩阵乘法结果
    int32_t* expected_C = (int32_t*)malloc(M * N * sizeof(int32_t));
    memset(expected_C, 0, M * N * sizeof(int32_t));

    for (size_t i = 0; i < M; ++i) {
        for (size_t j = 0; j < N; ++j) {
            for (size_t k = 0; k < K; ++k) {
                expected_C[i * N + j] += A[i * K + k] * B[k * N + j];
            }
        }
    }

    // 比较计算结果与程序结果
    // print_matrix_to_file(expected_C, M, N, 4, "expected_C.txt");
    bool is_correct = true;
    for (size_t i = 0; i < M; ++i) {
        for (size_t j = 0; j < N; ++j) {
            if (expected_C[i * N + j] != C[i * N + j]) {
                printf("Mismatch at C[%zu][%zu]: expected %d, got %d\n",
                       i, j, expected_C[i * N + j], C[i * N + j]);
                is_correct = false;
                break;
            }
        }
    }

    free(expected_C);
    return is_correct;
}