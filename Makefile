CXX=clang++
CFLAGS=-I. -O0 -g
BUILD_DIR = build

.PHONY: directories clean rebuild

SOURCE_FILES_VERILOG = source/Counter.v source/SPIController.v
SOURCE_FILES_TEST = test/main.cpp \
					test/Counter.test.cpp test/SPIController.test.cpp \
					test/Trace.cpp test/Trace.test.cpp \
					test/Step.cpp test/Step.test.cpp \
					test/PortDescription.cpp \
					test/TraceBuilder.cpp test/TraceBuilder.test.cpp \
					test/MatchesTrace.cpp test/MatchesTrace.test.cpp \
					test/TestBench.test.cpp \
					test/ConsoleColour.cpp \
					test/SPIControllerGenerated.cpp test/CounterGenerated.cpp

all: directories ${BUILD_DIR}/test

${BUILD_DIR}/test: ${SOURCE_FILES_TEST} ${BUILD_DIR}/VCounter/VCounter__ALL.a ${BUILD_DIR}/VSPIController/VSPIController__ALL.a
	${CXX} ${SOURCE_FILES_TEST} ${BUILD_DIR}/VCounter/VCounter__ALL.a ${BUILD_DIR}/VSPIController/VSPIController__ALL.a \
	/usr/local/share/verilator/include/verilated.cpp \
	-std=c++2a \
	${CFLAGS} \
	-I ${BUILD_DIR}/VCounter \
	-I ${BUILD_DIR}/VSPIController \
	-I /usr/local/share/verilator/include \
	-lgtest -lgmock \
	-Werror \
	-o ${BUILD_DIR}/test

${BUILD_DIR}/VCounter/VCounter.cpp: source/Counter.v
	verilator -Wall -cc -Mdir ${BUILD_DIR}/VCounter source/Counter.v

${BUILD_DIR}/VCounter/VCounter__ALL.a: ${BUILD_DIR}/VCounter/VCounter.cpp
	cd ${BUILD_DIR}/VCounter && make -f VCounter.mk

${BUILD_DIR}/VSPIController/VSPIController.cpp: source/SPIController.v
	verilator -Wall -cc -Mdir ${BUILD_DIR}/VSPIController source/SPIController.v

${BUILD_DIR}/VSPIController/VSPIController__ALL.a: ${BUILD_DIR}/VSPIController/VSPIController.cpp
	cd ${BUILD_DIR}/VSPIController && make -f VSPIController.mk

clean:
	rm -fr ${BUILD_DIR}

directories: ${BUILD_DIR}

${BUILD_DIR}:
	mkdir -p ${BUILD_DIR}

rebuild:
	make clean && make all