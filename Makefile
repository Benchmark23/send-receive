CC = g++
CFLAGS = -Iinclude
DEPS = $(wildcard include/*.h)

OBJ1 = $(patsubst %.cc,%.o,$(wildcard receiver/*.cc) )
OBJ2 = $(patsubst %.cc,%.o,$(wildcard sender/*.cc) )
OBJ3 = $(patsubst %.cc,%.o,$(wildcard utils/*.cc))
OBJ4 = $(patsubst %.cc,%.o,$(wildcard manager/*.cc))

%.o: %.cc $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

all: recv send

recv: $(OBJ1) $(OBJ3) manager/recv_manager.o
	$(CC) $(CFLAGS) -o $@ $^
send: $(OBJ2) $(OBJ3) manager/send_manager.o
	$(CC) $(CFLAGS) -o $@ $^
	rm -f $(OBJ1) $(OBJ2) $(OBJ3) $(OBJ4)

clean:
	rm -f sender/*.o receiver/*.o utils/*.o manager/*.o recv send

