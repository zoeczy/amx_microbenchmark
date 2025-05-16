# CFLAG = -O2 -march=native -fno-strict-aliasing
# CFLAG = -march=native -fno-strict-aliasing -g
CFLAG = -march=native -fno-strict-aliasing -O3
CC = gcc

# Directories
SRC_DIR = src
KERNEL_DIR = $(SRC_DIR)/kernel
BUILD_DIR = build
INCLUDE_DIR = include


TEST = amx_tmm
BIN = $(BUILD_DIR)/amx_microbenchmark.out
ASS = $(BUILD_DIR)/amx_microbenchmark.s
SRCS = $(wildcard $(SRC_DIR)/*.c) $(wildcard $(KERNEL_DIR)/*.c) main.c
OBJS = $(patsubst %.c, $(BUILD_DIR)/%.o, $(notdir $(SRCS)))

all: build $(BIN)

build:
	mkdir -p $(BUILD_DIR)

$(BIN): $(OBJS)
	$(CC) $(CFLAG) -I$(INCLUDE_DIR) -o $@ $^

vpath %.c $(SRC_DIR) $(KERNEL_DIR) ./
$(BUILD_DIR)/%.o: %.c
	$(CC) $(CFLAG) -I$(INCLUDE_DIR) -c $<  -o $@

ass: build
	$(CC) $(CFLAG) $(KERNEL_DIR)/amx_l1.c -I$(INCLUDE_DIR) -o $(ASS) -S

clean:
	rm -rf $(BUILD_DIR)

.PHONY:  all build clean test ass
