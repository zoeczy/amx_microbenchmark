#include <stdint.h>
#include <immintrin.h>
#include "utils.h"

int amx_l1(int niters, int8_t *A, int8_t *B, int32_t *C, size_t M, size_t N, size_t K) {

  for(int i = 0; i < niters; ++i)
  {
    for(int mi=0; mi<(M/TILE_M); mi+=2){

      for(int ni=0; ni<(N/TILE_N); ni+=2)
      {
        _tile_zero(TMM4);
        _tile_zero(TMM5);
        _tile_zero(TMM6);
        _tile_zero(TMM7);
        for(int ki=0; ki<(K/TILE_K); ++ki)
        {
          _tile_loadd(TMM0, A+mi*TILE_M*K+ki*TILE_K, K); // Load A
          _tile_loadd(TMM2, B+ki*(TILE_K/4)*(N*4)+ni*TILE_N*4, N*4); // Load B
          _tile_dpbuud(TMM4, TMM0, TMM2); // C += A * B
          _tile_loadd(TMM1, A+(mi+1)*TILE_M*K+ki*TILE_K, K); // Load A+1
          _tile_dpbuud(TMM6, TMM1, TMM2); // C += A * B
          _tile_loadd(TMM3, B+ki*(TILE_K/4)*(N*4)+(ni+1)*TILE_N*4, N*4); // Load B+1
          _tile_dpbuud(TMM5, TMM0, TMM3); // C += A * B
          _tile_dpbuud(TMM7, TMM1, TMM3); // C += A * B
        }
        _tile_stored(TMM4, (int8_t *)C+mi*TILE_M*N*sizeof(int32_t)+ni*TILE_N*sizeof(int32_t), N*sizeof(int32_t)); // Store C00
        _tile_stored(TMM5, (int8_t *)C+mi*TILE_M*N*sizeof(int32_t)+(ni+1)*TILE_N*sizeof(int32_t), N*sizeof(int32_t)); // Store C01
        _tile_stored(TMM6, (int8_t *)C+(mi+1)*TILE_M*N*sizeof(int32_t)+ni*TILE_N*sizeof(int32_t), N*sizeof(int32_t)); // Store C10
        _tile_stored(TMM7, (int8_t *)C+(mi+1)*TILE_M*N*sizeof(int32_t)+(ni+1)*TILE_N*sizeof(int32_t), N*sizeof(int32_t)); // Store C11
      }
    }
  }
  return 0;
}


// load bw utilization: 50% (limited by store)
// store bw utilization: 100%
int amx_l1_bw_load_store(int niters, int8_t *A, int8_t *B, int32_t *C, size_t M, size_t N, size_t K) {

  for(int i = 0; i < niters; ++i)
  {
    for(int mi=0; mi<(M/TILE_M); mi+=2) {
      for(int ni=0; ni<(N/TILE_N); ni+=2) {
        for(int ki=0; ki<(K/TILE_K); ++ki) {
          _tile_loadd(TMM0, A+mi*TILE_M*K+ki*TILE_K, K); // Load A
          _tile_loadd(TMM2, B+ki*(TILE_K/4)*(N*4)+ni*TILE_N*4, N*4); // Load B
          _tile_loadd(TMM1, A+(mi+1)*TILE_M*K+ki*TILE_K, K); // Load A+1
          _tile_loadd(TMM3, B+ki*(TILE_K/4)*(N*4)+(ni+1)*TILE_N*4, N*4); // Load B+1
        }
        _tile_zero(TMM0);
        _tile_zero(TMM1);
        _tile_zero(TMM2);
        _tile_zero(TMM3);

        _tile_stored(TMM0, (int8_t *)C+mi*TILE_M*N*sizeof(int32_t)+ni*TILE_N*sizeof(int32_t), N*sizeof(int32_t)); // Store C00
        _tile_stored(TMM1, (int8_t *)C+mi*TILE_M*N*sizeof(int32_t)+(ni+1)*TILE_N*sizeof(int32_t), N*sizeof(int32_t)); // Store C01
        _tile_stored(TMM2, (int8_t *)C+(mi+1)*TILE_M*N*sizeof(int32_t)+ni*TILE_N*sizeof(int32_t), N*sizeof(int32_t)); // Store C10
        _tile_stored(TMM3, (int8_t *)C+(mi+1)*TILE_M*N*sizeof(int32_t)+(ni+1)*TILE_N*sizeof(int32_t), N*sizeof(int32_t)); // Store C11
      }
    }
  }
  return 0;
}

// load bw utilization: 100%
int amx_l1_bw_load(int niters, int8_t *A, int8_t *B, int32_t *C, size_t M, size_t N, size_t K) {

  for(int i = 0; i < niters; ++i)
  {
    for(int mi=0; mi<(M/TILE_M); mi+=2) {
      for(int ni=0; ni<(N/TILE_N); ni+=2) {
        for(int ki=0; ki<(K/TILE_K); ++ki) {
          _tile_loadd(TMM0, A+mi*TILE_M*K+ki*TILE_K, K); // Load A
          _tile_loadd(TMM2, B+ki*(TILE_K/4)*(N*4)+ni*TILE_N*4, N*4); // Load B
          _tile_loadd(TMM1, A+(mi+1)*TILE_M*K+ki*TILE_K, K); // Load A+1
          _tile_loadd(TMM3, B+ki*(TILE_K/4)*(N*4)+(ni+1)*TILE_N*4, N*4); // Load B+1
        }
      }
    }
  }
  return 0;
}

// store bw utilization: 100%
int amx_l1_bw_store(int niters, int8_t *A, int8_t *B, int32_t *C, size_t M, size_t N, size_t K) {

  for(int i = 0; i < niters; ++i)
  {
    for(int mi=0; mi<(M/TILE_M); mi+=2) {
      for(int ni=0; ni<(N/TILE_N); ni+=2) {
        _tile_zero(TMM0);
        _tile_zero(TMM1);
        _tile_zero(TMM2);
        _tile_zero(TMM3);

        _tile_stored(TMM0, (int8_t *)C+mi*TILE_M*N*sizeof(int32_t)+ni*TILE_N*sizeof(int32_t), N*sizeof(int32_t)); // Store C00
        _tile_stored(TMM1, (int8_t *)C+mi*TILE_M*N*sizeof(int32_t)+(ni+1)*TILE_N*sizeof(int32_t), N*sizeof(int32_t)); // Store C01
        _tile_stored(TMM2, (int8_t *)C+(mi+1)*TILE_M*N*sizeof(int32_t)+ni*TILE_N*sizeof(int32_t), N*sizeof(int32_t)); // Store C10
        _tile_stored(TMM3, (int8_t *)C+(mi+1)*TILE_M*N*sizeof(int32_t)+(ni+1)*TILE_N*sizeof(int32_t), N*sizeof(int32_t)); // Store C11
      }
    }
  }
  return 0;
}