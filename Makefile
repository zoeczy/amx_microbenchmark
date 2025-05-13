# CFLAG = -O2 -march=native -fno-strict-aliasing
CFLAG = -march=native -fno-strict-aliasing -g
CC = gcc
TEST = test_amx_tmm
BIN = $(TEST).out
ASS = $(TEST).s
SRCS = $(TEST).c run.c utils.c
OBJS = $(SRCS:.c=.o)

all: $(BIN)

$(BIN): $(OBJS)
	$(CC) $(CFLAG) -o $@ $^

%.o: %.c
	$(CC) $(CFLAG) -c $< -o $@

ass:
	$(CC) $(CFLAG) $(SRCS) -o $(ASS) -S

clean:
	rm -rf $(BIN) $(ASS) *.o

.PHONY: clean

