GDB?=

.PHONY: clean
cpp_files=$(shell find . -type f \( -name "*.cpp" -and ! -name "pthread_bfs.cpp" \))
files=$(cpp_files:.cpp=)

all:$(files)

%: %.cpp pthread_bfs.cpp ../serial/serial_bfs.cpp ../graph_gen/graph_generator.cpp
	g++ $+ -o $@ -lpthread ${GDB}

clean:
	rm -f $(files) *.o