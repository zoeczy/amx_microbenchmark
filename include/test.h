#ifndef _AMX_UBENCH_TEST_H_
#define _AMX_UBENCH_TEST_H_

int tmm_4ld4tdpb_with_dep(int niters, int8_t *A, int8_t *B, int32_t *C, size_t M, size_t N, size_t K);
int tmm_4tdpb4st_with_dep_1(int niters, int8_t *A, int8_t *B, int32_t *C, size_t M, size_t N, size_t K);
int tmm_4tdpb4st_with_dep_2(int niters, int8_t *A, int8_t *B, int32_t *C, size_t M, size_t N, size_t K);
int tmm_4tdpb4st_without_dep_1(int niters, int8_t *A, int8_t *B, int32_t *C, size_t M, size_t N, size_t K);
int tmm_4tdpb4st_without_dep_2(int niters, int8_t *A, int8_t *B, int32_t *C, size_t M, size_t N, size_t K);
int tmm_4ldtdpb4st_with_dep_100(int niters, int8_t *A, int8_t *B, int32_t *C, size_t M, size_t N, size_t K);

int l1(int niters, int8_t *A, int8_t *B, int32_t *C, size_t M, size_t N, size_t K);
int l1_bw_load_store(int niters, int8_t *A, int8_t *B, int32_t *C, size_t M, size_t N, size_t K);
int l1_bw_load(int niters, int8_t *A, int8_t *B, int32_t *C, size_t M, size_t N, size_t K);
int l1_bw_store(int niters, int8_t *A, int8_t *B, int32_t *C, size_t M, size_t N, size_t K);

#endif