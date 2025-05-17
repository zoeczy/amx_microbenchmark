#include "utils.h"
#include <stdio.h>
#include <sys/syscall.h>
#include <immintrin.h>

/* Initialize tile config */
void init_tile_config (__tilecfg *tileinfo)
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
bool set_tiledata_use()
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

void amx_b_layout_transform(uint8_t *src, uint8_t *dst, size_t rows_src, size_t cols_src)
{
    for (int k = 0; k < rows_src/4; k++)
    {
        for (int i = 0; i < cols_src; i ++)
        {
            for (int j = 0; j < 4; j++)
            {
                dst[k*cols_src*4 + i*4 + j] = src[k*cols_src*4 + j*cols_src + i];
            }
        }
    }
}