CXX=clang++
CFLAGS=-I.
BUILD_DIR = build

.PHONY: directories clean rebuild

SOURCE_FILES_VERILOG = Counter.v
SOURCE_FILES_TEST = test.cpp

all: directories ${BUILD_DIR}/test

${BUILD_DIR}/test: ${SOURCE_FILES_TEST} ${BUILD_DIR}/VCounter/VCounter__ALL.a
	${CXX} ${SOURCE_FILES_TEST} ${BUILD_DIR}/VCounter/VCounter__ALL.a \
	/usr/local/share/verilator/include/verilated.cpp \
	-std=c++11 \
	-I ${BUILD_DIR}/VCounter \
	-I /usr/local/share/verilator/include \
	-lgtest \
	-o ${BUILD_DIR}/test

build/VCounter/VCounter.cpp: ${SOURCE_FILES_VERILOG}
	verilator -Wall -cc -Mdir ${BUILD_DIR}/VCounter Counter.v

build/VCounter/VCounter__ALL.a: ${BUILD_DIR}/VCounter/VCounter.cpp
	cd ${BUILD_DIR}/VCounter && make -f VCounter.mk

clean:
	rm -fr ${BUILD_DIR}

directories: ${BUILD_DIR}

${BUILD_DIR}:
	mkdir -p ${BUILD_DIR}

rebuild:
	make clean && make all