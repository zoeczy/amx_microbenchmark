#ifndef _AMX_UBENCH_UTILS_H_
#define _AMX_UBENCH_UTILS_H_

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

#define MAX 1024
#define MAX_ROWS 16
#define MAX_COLS 64
#define STRIDE 64
#define ARCH_GET_XCOMP_PERM     0x1022
#define ARCH_REQ_XCOMP_PERM     0x1023
#define XFEATURE_XTILECFG       17
#define XFEATURE_XTILEDATA      18

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

void init_tile_config (__tilecfg *tileinfo);
bool set_tiledata_use();
void amx_b_layout_transform(uint8_t *src, uint8_t *dst, size_t rows_src, size_t cols_src);

#endif