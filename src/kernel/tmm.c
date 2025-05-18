
#include <immintrin.h>
#include <stdint.h>

#include "utils.h"

// 100% MAC utilization by load->tdpb pipeline parallelism
int tmm_4ld4tdpb_with_dep(int niters, int8_t *A, int8_t *B, int32_t *C, size_t M, size_t N, size_t K) {
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

// 50% MAC utilization due to:
// 1. in-order schedule of tdpb and store
// 2. shared schedule queue between tdpb and store
int tmm_4tdpb4st_with_dep_1(int niters, int8_t *A, int8_t *B, int32_t *C, size_t M, size_t N, size_t K) {
	int a = 0;
	for (int i = 0; i < niters; ++i) {

      _tile_dpbssd(2, 1, 0);
      _tile_stored(2, C, 64);

      _tile_dpbssd(3, 1, 0);
      _tile_stored(3, C, 64);

      _tile_dpbssd(4, 1, 0);
      _tile_stored(4, C, 64);

      _tile_dpbssd(5, 1, 0);
      _tile_stored(5, C, 64);
	}

	return a;
}

// 50% MAC utilization due to:
// 1. in-order schedule of tdpb and store
// 2. shared schedule queue between tdpb and store
int tmm_4tdpb4st_with_dep_2(int niters, int8_t *A, int8_t *B, int32_t *C, size_t M, size_t N, size_t K) {
	int a = 0;
	for (int i = 0; i < niters; ++i) {

      _tile_dpbssd(2, 1, 0);
      _tile_dpbssd(3, 1, 0);
      _tile_dpbssd(4, 1, 0);
      _tile_dpbssd(5, 1, 0);

      _tile_stored(2, C, 64);
      _tile_stored(3, C, 64);
      _tile_stored(4, C, 64);
      _tile_stored(5, C, 64);
	}

	return a;
}

// 100% MAC utilization by tdpb/store parallelism
int tmm_4tdpb4st_without_dep_1(int niters, int8_t *A, int8_t *B, int32_t *C, size_t M, size_t N, size_t K) {
	int a = 0;
	for (int i = 0; i < niters; ++i) {
      _tile_dpbssd(2, 1, 0);
      _tile_stored(6, C, 64);

      _tile_dpbssd(3, 1, 0);
      _tile_stored(6, C, 64);

      _tile_dpbssd(4, 1, 0);
      _tile_stored(6, C, 64);

      _tile_dpbssd(5, 1, 0);
      _tile_stored(6, C, 64);
	}

	return a;
}

// 100% MAC utilization by tdpb/store parallelism
int tmm_4tdpb4st_without_dep_2(int niters, int8_t *A, int8_t *B, int32_t *C, size_t M, size_t N, size_t K) {
	int a = 0;
	for (int i = 0; i < niters; ++i) {
      _tile_dpbssd(2, 1, 0);
      _tile_dpbssd(3, 1, 0);
      _tile_dpbssd(4, 1, 0);
      _tile_dpbssd(5, 1, 0);

      _tile_stored(6, C, 64);
      _tile_stored(6, C, 64);
      _tile_stored(6, C, 64);
      _tile_stored(6, C, 64);
	}

	return a;
}

// near 100% MAC utilization by high tdbp/store ratio
int tmm_4ldtdpb4st_with_dep_100(int niters, int8_t *A, int8_t *B, int32_t *C, size_t M, size_t N, size_t K) {
      int a = 0;
      for (int i = 0; i < niters; ++i) {

            for (int k = 0; k < 100; ++k) {
            _tile_loadd(0, A, 64);
            _tile_loadd(3, B, 64);
            _tile_dpbssd(4, 3, 0);
            _tile_loadd(1, A, 64);
            _tile_dpbssd(5, 3, 1);
            _tile_loadd(2, B, 64);
            _tile_dpbssd(6, 2, 0);
            _tile_dpbssd(7, 2, 1);
            }

            _tile_stored(4, C, 64);
            _tile_stored(5, C, 64);
            _tile_stored(6, C, 64);
            _tile_stored(7, C, 64);
      }
      return a;
}
