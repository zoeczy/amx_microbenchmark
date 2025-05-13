# CFLAG = -O2 -march=native -fno-strict-aliasing
CFLAG = -march=native -fno-strict-aliasing -g
CC = gcc
TEST = test_amx_tmm
BIN = $(TEST).out
ASS = $(TEST).s
CFILES = $(TEST).c 

all:
	$(CC) $(CFLAG) $(CFILES) -o $(BIN) $(LIBS)

ass:
	$(CC) $(CFLAG) $(CFILES) -o $(ASS) $(LIBS) -S

clean:
	rm -rf $(TEST) $(ASS)

.PHONY: clean

