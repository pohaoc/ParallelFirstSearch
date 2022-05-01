GDB?=

.PHONY: clean
cpp_files=$(shell find . -type f -name "*.cpp")
files=$(cpp_files:.cpp=)

all:$(files)

%: %.cpp ../serial/serial_bfs.cpp ../graph_gen/graph_generator.cpp ../pthread/pthread_bfs.cpp ../OpenMP/omp_bfs.cpp
	g++ $+ -o $@ -fopenmp -lpthread ${GDB}

clean:
	rm -f $(files) *.o