GDB?=

.PHONY: clean
cpp_files=$(shell find . -type f \( -name "*.cpp" -and ! -name "omp_bfs.cpp" \))
files=$(cpp_files:.cpp=)

all:$(files)

%: %.cpp omp_bfs.cpp ../serial/serial_bfs.cpp ../graph_gen/graph_generator.cpp
	g++ $+ -o $@ -fopenmp ${GDB}

clean:
	rm -f $(files) *.o