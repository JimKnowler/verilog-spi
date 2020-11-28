CXX=clang++
CFLAGS=-I.
BUILD_DIR = build

.PHONY: directories clean rebuild

SOURCE_FILES_VERILOG = source/Counter.v source/SPIMaster.v
SOURCE_FILES_TEST = test/test.cpp test/TestCounter.cpp test/TestSPIMaster.cpp

all: directories ${BUILD_DIR}/test

${BUILD_DIR}/test: ${SOURCE_FILES_TEST} ${BUILD_DIR}/VCounter/VCounter__ALL.a ${BUILD_DIR}/VSPIMaster/VSPIMaster__ALL.a
	${CXX} ${SOURCE_FILES_TEST} ${BUILD_DIR}/VCounter/VCounter__ALL.a ${BUILD_DIR}/VSPIMaster/VSPIMaster__ALL.a \
	/usr/local/share/verilator/include/verilated.cpp \
	-std=c++14 \
	-I ${BUILD_DIR}/VCounter \
	-I ${BUILD_DIR}/VSPIMaster \
	-I /usr/local/share/verilator/include \
	-lgtest \
	-o ${BUILD_DIR}/test

${BUILD_DIR}/VCounter/VCounter.cpp: source/Counter.v
	verilator -Wall -cc -Mdir ${BUILD_DIR}/VCounter source/Counter.v

${BUILD_DIR}/VCounter/VCounter__ALL.a: ${BUILD_DIR}/VCounter/VCounter.cpp
	cd ${BUILD_DIR}/VCounter && make -f VCounter.mk

${BUILD_DIR}/VSPIMaster/VSPIMaster.cpp: source/SPIMaster.v
	verilator -Wall -cc -Mdir ${BUILD_DIR}/VSPIMaster source/SPIMaster.v

${BUILD_DIR}/VSPIMaster/VSPIMaster__ALL.a: ${BUILD_DIR}/VSPIMaster/VSPIMaster.cpp
	cd ${BUILD_DIR}/VSPIMaster && make -f VSPIMaster.mk

clean:
	rm -fr ${BUILD_DIR}

directories: ${BUILD_DIR}

${BUILD_DIR}:
	mkdir -p ${BUILD_DIR}

rebuild:
	make clean && make all