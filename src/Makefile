# CFLAGS = -march=native -fno-strict-aliasing -g
# CFLAGS = -march=native -fno-strict-aliasing -O2
CFLAGS = -march=native -fno-strict-aliasing -O3
CC = gcc
TILE_SHAPE ?= 256

CFLAGS += -DTILE_SHAPE=$(TILE_SHAPE)  # 16x16 or 8x8
# CFLAG += -DTILE_SHAPE=64  # 16x16 or 8x8

CFLAGS += -DENABLE_TRACE=$(ENABLE_TRACE)

# Directories
SRC_DIR = ./
KERNEL_DIR = $(SRC_DIR)/kernel
BUILD_DIR = ../build
BIN_DIR = ../bin
INCLUDE_DIR = ../include

BIN = $(BIN_DIR)/amx_microbenchmark.out
ASS = $(BUILD_DIR)/amx_microbenchmark.s
SRCS = $(wildcard $(SRC_DIR)/*.c) $(wildcard $(KERNEL_DIR)/*.c)
OBJS = $(patsubst %.c, $(BUILD_DIR)/%.o, $(notdir $(SRCS)))

all: build $(BIN)

build:
	mkdir -p $(BUILD_DIR)

$(BIN): $(OBJS)
	mkdir -p $(BIN_DIR)
	$(CC) $(CFLAGS) -I$(INCLUDE_DIR) -o $@ $^

vpath %.c $(SRC_DIR) $(KERNEL_DIR) ./
$(BUILD_DIR)/%.o: %.c
	$(CC) $(CFLAGS) -I$(INCLUDE_DIR) -c $<  -o $@

ass: build
	$(CC) $(CFLAGS) $(KERNEL_DIR)/l1.c -I$(INCLUDE_DIR) -o $(ASS) -S

clean:
	rm -rf $(BUILD_DIR) $(BIN_DIR)

.PHONY:  all build clean test ass
