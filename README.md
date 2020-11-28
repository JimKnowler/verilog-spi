# Verilog-SPI

Test Driven Implementation of Serial Peripheral Interface (SPI) protocol in Verilog for FPGA.

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

- add option for generating traces from tests
  - command line option?
  - TestBench generates a trace for each test + writes after every tick?
  - see https://zipcpu.com/blog/2017/06/21/looking-at-verilator.html for examples

- write SPIMaster with tests
- write SPISlave with tests
- write integration tests for SPIMaster + SPISlave
- augment makefile to be sensitive to header files
- refactor makefile - generic rules for Verilation of Verilog modules
- refactor makefile - compile to object files in build directory separately so final build step is just linker
- convert from makefile to CMake or Bazel (python?)
- support testing all 4 SPI modes by using verilator arg at verilation time, such as '-GSPI_MODE=3' 
   - generate 4 versions of SPIMaster and SPISlave
   - run unit tests on each version
- support testing different clock speeds on SPIMaster by using verilator arg at verilation time, such as '-GCLKS_PER_HALF_BIT=3'
   - generate multiple versions of SPIMaster
   - run unit tests on each version