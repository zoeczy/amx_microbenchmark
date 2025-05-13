
#include <immintrin.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

#include "utils.h"
#include "test.h"

int main(int argc, char** argv) {
    if (argc != 2) {
        printf("Usage: %s niters\n", argv[0]);
        exit(1);
    }
 
    int num_iters = atoi(argv[1]);
 
    struct timespec start, end;
 
    __tilecfg tile_data = {0};
 
    // Request permission to linux kernel to run AMX 
    if (!set_tiledata_use())
       exit(-1);
 
    // Load tile configuration 
    init_tile_config (&tile_data);
 
    clock_gettime(CLOCK_MONOTONIC, &start);
 
    // run test
    int ret = test_mac_utilization(num_iters);
 
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