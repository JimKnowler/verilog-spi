# Verilog-SPI

Test Driven Implementation of Serial Peripheral Interface (SPI) protocol in Verilog for FPGA.

# References 

- [SparkFun -  SPI Tutorial](https://learn.sparkfun.com/tutorials/serial-peripheral-interface-spi/all)
- [NandLand - SPI in Verilog](https://www.youtube.com/watch?v=TR0Pw89EuGk&list=PLnAoag7Ew-vq5kOyfyNN50xL718AtLoCQ&index=3)
- [fpga4fun - SPI](https://www.fpga4fun.com/SPI1.html)
- [Wikipedia - SPI](https://en.wikipedia.org/wiki/Serial_Peripheral_Interface)

# Dependencies

- [GoogleTest](https://github.com/google/googletest/)

While standing in googletest's root directory.
```
$ mkdir build
$ cd build
$ cmake ..
$ make
$ sudo make install
```

- [Verilator](https://github.com/verilator/verilator)

```
$ brew install verilator
```

# Build & Run

While standing in project's root directory.

```
$ make
$ ./test
```

To do a full rebuild

```
$ make rebuild
$ ./test
```

To clean out the build directory
```
$ make clean
```


# TODO

- note use of CONTROLLER and PERIPHERAL https://www.sparkfun.com/spi_signal_names?_ga=2.39707619.1760252179.1606575342-655091437.1606575342

- add option for generating traces from tests
  - command line option?
  - TestBench generates a trace for each test + writes after every tick?
  - see https://zipcpu.com/blog/2017/06/21/looking-at-verilator.html for examples

- write SPIController with tests
- write SPIPeripheral with tests
- write integration tests for SPIController + SPIPeripheral
- improve build system
  - improvements to makefile
    - augment makefile to be sensitive to header files
    - refactor makefile - generic rules for Verilation of Verilog modules
    - refactor makefile - compile to object files in build directory separately so final build step is just linker
  - convert from makefile to CMake or Bazel (python?)
- support testing all 4 SPI modes by using verilator arg at verilation time, such as '-GSPI_MODE=3' 
   - generate 4 versions of SPIController and SPIPeripheral
   - run unit tests on each version
- support testing different clock speeds on SPIController by using verilator arg at verilation time, such as '-GCLKS_PER_HALF_BIT=3'
   - generate multiple versions of SPIController
   - run unit tests on each version
- support generation of C++ test framework code from verilog/verilator output