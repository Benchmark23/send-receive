# CC = g++
# CFLAGS = -Iinclude
# DEPS = include/parser.h include/receiver.h include/sender.h
# OBJ = main.o

# main: main.cc $(DEPS)
# 	$(CC) -o $@ $< $(CFLAGS)
CC = g++
CFLAGS = -Iinclude
DEPS = $(wildcard include/*.h)
OBJ = $(patsubst %.cc,%.o,$(wildcard *.cc) $(wildcard include/*.cc))

%.o: %.cc $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

main: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)
	rm -f $(OBJ)


