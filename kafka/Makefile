CXX = g++
CXXFLAGS = -std=c++11 -Wall -Wextra -I/usr/local/include/librdkafka
LDFLAGS = -L/usr/local/lib -lrdkafka++ -lrdkafka

all: kafka

kafka: kafka.cc
	$(CXX) $(CXXFLAGS) $^ -o $@ $(LDFLAGS)

clean:
	rm -f kafka
