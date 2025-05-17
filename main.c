
#include <immintrin.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <string.h>

#include "utils.h"
#include "test.h"

typedef int (*amx_test)(int64_t num_iter, int8_t *A, int8_t *B, int32_t *C, size_t M, size_t N, size_t K);

typedef struct {
    const char* name;
    amx_test func;
} amx_test_entry;

amx_test_entry amx_tests[] = {
    {"amx_tmm_4ld4tdpb_with_dep", amx_tmm_4ld4tdpb_with_dep},
    {"amx_tmm_4tdpb4st_with_dep_1", amx_tmm_4tdpb4st_with_dep_1},
    {"amx_tmm_4tdpb4st_with_dep_2", amx_tmm_4tdpb4st_with_dep_2},
    {"amx_tmm_4tdpb4st_without_dep_1", amx_tmm_4tdpb4st_without_dep_1},
    {"amx_tmm_4tdpb4st_without_dep_2", amx_tmm_4tdpb4st_without_dep_2},
    {"amx_tmm_4ld4tdpb4st_with_dep_100", amx_tmm_4ldtdpb4st_with_dep_100},
    {"amx_l1",  amx_l1},
    {"amx_l1_bw_load_store",  amx_l1_bw_load_store},
    {"amx_l1_bw_load",  amx_l1_bw_load},
    {"amx_l1_bw_store",  amx_l1_bw_store},
    {NULL, NULL}
};

amx_test_entry* find_test(const char* name) {
    for (int i = 0; amx_tests[i].name != NULL; ++i) {
        if (strcmp(amx_tests[i].name, name) == 0) {
            return &amx_tests[i];
        }
    }

    return NULL;
}

int main(int argc, char** argv) {
    if (argc != 6) {
        printf("Usage: %s name niters M N K\n", argv[0]);
        exit(1);
    }

    const char* name = argv[1];
    int num_iters = atoi(argv[2]);
    size_t M = (size_t)atoi(argv[3]);
    size_t N = (size_t)atoi(argv[4]);
    size_t K = (size_t)atoi(argv[5]);

    int8_t *A = (int8_t *)_mm_malloc(M * K * sizeof(int8_t), 64);
    int8_t *B = (int8_t *)_mm_malloc(K * N * sizeof(int8_t), 64);
    int8_t *B_trans = (int8_t *)_mm_malloc(K * N * sizeof(int8_t), 64);
    int32_t *C = (int32_t *)_mm_malloc(M * N * sizeof(int32_t), 64);
    init_matrices(A, B, C, M, N, K);
    amx_b_layout_transform(B, B_trans, K, N);

    amx_test_entry* entry = find_test(name);
 
    // Request permission to linux kernel to run AMX 
    if (!set_tiledata_use())
       exit(-1);
    // Load tile configuration 
    __tilecfg tile_data = {0};
    init_tile_config (&tile_data);

    // correctness test
    entry->func(1, A, B_trans, C, M, N, K);
    // print_matrix_to_file(A, M, K, 1, "A.txt");
    // print_matrix_to_file(B, K, N, 1, "B.txt");
    // print_matrix_to_file(C, M, N, 4, "C.txt");
    if (verify_matrix_multiplication(A, B, C, M, N, K)) {
        printf("Matrix multiplication is correct.\n");
    } else {
        printf("Matrix multiplication is incorrect.\n");
    }

    // warmup
    int ret = entry->func(10000000, A, B_trans, C, M, N, K);
    // run test
    struct timespec start, end;
    clock_gettime(CLOCK_MONOTONIC, &start);
    ret = entry->func(num_iters, A, B_trans, C, M, N, K);
    clock_gettime(CLOCK_MONOTONIC, &end);
 
    // Calculate elapsed time in seconds
    double elapsed = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;
    double ops = 0.0;
    // if (strcmp("amx_tmm", name) == 0)
    if (strstr(name, "amx_tmm") != NULL) {
        ops = 2.0 * 16 * 16 * 64 * 4 * num_iters;
    }
    else {
        ops = M * N * K * 2 * num_iters;
    }

    double gflops = (ops / elapsed) / 1e9;
    printf("Performance:     [%.2f] GFLOPS\n", gflops);
    printf("MAC Utilization: [%.2f%%]\n", (gflops / (2048*3.126)) * 100.0); 
    // printf("%d\n", ret);

    double A_bytes = M*K*sizeof(int8_t);
    double B_bytes = K*N*sizeof(int8_t);
    double C_bytes = M*N*sizeof(int8_t);
    double total_kbytes = (A_bytes+B_bytes+C_bytes) / 1024;
    printf("A/B/C KB:     [%.2f] KB\n", total_kbytes);

    double load_gbytes = (M/(16*2)) * (N/(16*2)) * (K/64) * (16*2*64+16*2*64) * num_iters / 1e9;
    double store_gbytes = (M/(16*2)) * (N/(16*2)) * (4*16*64) * num_iters / 1e9;
    double load_bw = load_gbytes / elapsed;
    double store_bw = store_gbytes / elapsed;
    printf("Read GB:     [%.2f] GB\n", load_gbytes);
    printf("Store GB:    [%.2f] GB\n", store_gbytes);
    printf("Read BW:     [%.2f] GB/s\n", load_bw);
    printf("Store BW:    [%.2f] GB/s\n", store_bw);
    printf("Read BW Utilization: [%.2f%%]\n", (load_bw / (64*2*3.126)) * 100.0); 
    printf("Store BW Utilization: [%.2f%%]\n", (store_bw / (64*3.126)) * 100.0); 
 
    return 0;
 }