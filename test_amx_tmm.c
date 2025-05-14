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

#include "utils.h"


int8_t A[8*4096*16] AMX_ALIGN(64) = {0};
int8_t B[8*4096*16] AMX_ALIGN(64) = {0};

int64_t stride = 64;


// test
int amx_tmm(int niters) {
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
