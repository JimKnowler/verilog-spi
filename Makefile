CXX=clang++
CFLAGS=-I.
BUILD_DIR = build

.PHONY: directories clean rebuild

verilog_source_files = Counter.v

${BUILD_DIR}/test: ${BUILD_DIR}/VCounter/VCounter__ALL.a
	${CXX} ${BUILD_DIR}/VCounter/VCounter__ALL.a -o ${BUILD_DIR}/test -lgtest -lgtest_main

build/VCounter/VCounter.cpp: ${verilog_source_files} directories
	verilator -Wall -cc -Mdir ${BUILD_DIR}/VCounter Counter.v

build/VCounter/VCounter__ALL.a: ${BUILD_DIR}/VCounter/VCounter.cpp
	cd ${BUILD_DIR}/VCounter && make -f VCounter.mk

clean:
	rm -fr ${BUILD_DIR}

directories: ${BUILD_DIR}

${BUILD_DIR}:
	mkdir -p ${BUILD_DIR}

rebuild:
	make clean && make ${BUILD_DIR}/test