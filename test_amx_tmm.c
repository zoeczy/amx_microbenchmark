//==============================================================
// Copyright © 2022 Intel Corporation
//
// SPDX-License-Identifier: MIT
// =============================================================
#include <immintrin.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <sys/syscall.h>
#include <unistd.h>
#include <stdbool.h>
#include <time.h>

#define MAX 1024
#define MAX_ROWS 16
#define MAX_COLS 64
#define STRIDE 64
#define ARCH_GET_XCOMP_PERM     0x1022
#define ARCH_REQ_XCOMP_PERM     0x1023
#define XFEATURE_XTILECFG       17
#define XFEATURE_XTILEDATA      18

#define AMX_ALIGN(x) __attribute__((aligned(x)))

//Define tile config data structure 
typedef struct __tile_config
{
  uint8_t palette_id;
  uint8_t start_row;
  uint8_t reserved_0[14];
  uint16_t colsb[16]; 
  uint8_t rows[16]; 
} __tilecfg;

int8_t A[8*4096*16] AMX_ALIGN(64) = {0};
int8_t B[8*4096*16] AMX_ALIGN(64) = {0};

int64_t stride = 64;

/* Initialize tile config */
static void init_tile_config (__tilecfg *tileinfo)
{
  int i;
  tileinfo->palette_id = 1;
  tileinfo->start_row = 0;

//   for (i = 0; i < 1; ++i)
//   {
//     tileinfo->colsb[i] = MAX_ROWS;
//     tileinfo->rows[i] =  MAX_ROWS;
//   }

  for (i = 0; i < 8; ++i)
  {
    tileinfo->colsb[i] = MAX_COLS;
    tileinfo->rows[i] =  MAX_ROWS;
  }

  _tile_loadconfig (tileinfo);
}

/* Set_tiledata_use() - Invoke syscall to set ARCH_SET_STATE_USE */
static bool set_tiledata_use()
{
   if (syscall(SYS_arch_prctl, ARCH_REQ_XCOMP_PERM, XFEATURE_XTILEDATA)) 
   {
      printf("\n Fail to do XFEATURE_XTILEDATA \n\n");
      return false;
   }
   else
   {
      printf("\n TILE DATA USE SET - OK \n\n");
      return true;
   }

   return true;
}

// test
int test_mac_utilization(int niters) {
	int a = 0;
	for (int i = 0; i < niters; ++i) {
      _tile_loadd(0, A, 64);
      _tile_loadd(3, B, 64);
      _tile_dpbssd(4, 3, 0);
      _tile_loadd(1, A, 64);
      _tile_dpbssd(5, 3, 1);
      _tile_loadd(2, B, 64);
      _tile_dpbssd(6, 2, 0);
      _tile_dpbssd(7, 2, 1);
	}

	return a;
}

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
