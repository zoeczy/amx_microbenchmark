#ifndef _AMX_UBENCH_TEST_H_
#define _AMX_UBENCH_TEST_H_

int amx_tmm(int niters, int8_t *A, int8_t *B, int32_t *C, size_t M, size_t N, size_t K);
int amx_l1(int niters, int8_t *A, int8_t *B, int32_t *C, size_t M, size_t N, size_t K);
int amx_l1_bw(int niters, int8_t *A, int8_t *B, int32_t *C, size_t M, size_t N, size_t K);

#endif