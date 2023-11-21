CC = g++
CFLAGS = -Wall -std=c++11
LDFLAGS = -lrdkafka

# 设置librdkafka的头文件路径
LIBRDKAFKA_INCLUDE = -I/path/to/librdkafka/include
CFLAGS += $(LIBRDKAFKA_INCLUDE)

# 设置librdkafka的库文件路径
LIBRDKAFKA_LIB = -L/path/to/librdkafka/lib
LDFLAGS += $(LIBRDKAFKA_LIB)

# 编译规则
all: your_program

your_program: your_program.cpp
	$(CC) $(CFLAGS) -o $@ $< $(LDFLAGS)

clean:
	rm -f your_program
