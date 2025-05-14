#include <stdint.h>
#include <immintrin.h>
#include <mm_malloc.h>
#include "utils.h"


#define TMM0    0
#define TMM1    1
#define TMM2    2
#define TMM3    3
#define TMM4    4
#define TMM5    5
#define TMM6    6
#define TMM7    7

#define TILE_M 16
#define TILE_K 64
#define TILE_N 16

// #define M   (1024)
// #define K   (5120)
// #define N   (1280) 

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

// int main(){
//     int niters = 10;
//     int M=1024,N=2024,K=1024;
//     int8_t *A = (int8_t *)_mm_malloc(M * K * sizeof(int8_t), 64);
//     int8_t *B = (int8_t *)_mm_malloc(K * N * sizeof(int8_t), 64);
//     int32_t *C = (int32_t *)_mm_malloc(M * N * sizeof(int32_t), 64);
//     init_matrices(A, B, C, M, N, K);
//     if (!set_tiledata_use())
//        exit(-1);
//     __tilecfg tile_data = {0};
//     init_tile_config (&tile_data);
//     amx_l1(niters, A, B, C, M, N, K);
//     _mm_free(A);
//     _mm_free(B);
//     _mm_free(C);
//     return 0;

// }