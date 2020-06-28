SRC = $(wildcard *.c)
OBJ = $(SRC:.c=.o)
CC = gcc
EXEC = start

all:$(EXEC)

$(EXEC) : $(OBJ)
	$(CC) $^ -o $@

%.o : %.c def.h
	$(CC) -c $<

#.PHONY: clean
clean:
	rm -f $(OBJ) $(EXEC)
