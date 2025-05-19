
# build configs
TILE_SHAPE ?= 64   # 256 (8x8 HMA) or 64 (8x8 AMX)
ENABLE_TRACE ?= 1  # enable (1) trace facilities or not (0)

# runtime configs
CASE ?= l1
NITERS ?= 10000
M ?= 64
N ?= 64
K ?= 64