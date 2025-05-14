
#include <immintrin.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

#include "utils.h"
#include "test.h"

typedef int (*amx_test)(int64_t num_iter);

typedef struct {
    const char* name;
    amx_test func;
} amx_test_entry;

amx_test_entry amx_tests[] = {
    {"amx_tmm", amx_tmm},
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
    if (argc != 3) {
        printf("Usage: %s name niters\n", argv[0]);
        exit(1);
    }
 
    const char* name = argv[1];
    int num_iters = atoi(argv[2]);

    amx_test_entry* entry = find_test(name);
 
    struct timespec start, end;
 
    __tilecfg tile_data = {0};
 
    // Request permission to linux kernel to run AMX 
    if (!set_tiledata_use())
       exit(-1);
 
    // Load tile configuration 
    init_tile_config (&tile_data);

    // warmup
    int ret = entry->func(1000000);
 
    clock_gettime(CLOCK_MONOTONIC, &start);
 
    // run test
    ret = entry->func(num_iters);
 
    clock_gettime(CLOCK_MONOTONIC, &end);
 
    // Calculate elapsed time in seconds
    double elapsed = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;
 
    double ops = 2.0 * 16 * 16 * 64 * 4 * num_iters;
    double gflops = (ops / elapsed) / 1e9;
 
    printf("Performance:     [%.2f] GFLOPS\n", gflops);
    printf("MAC Utilization: [%.2f%%]\n", (gflops / 8600) * 100.0);
 
    printf("%d\n", ret);
 
    return 0;
 }