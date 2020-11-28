# Verilog-SPI

Test Driven Implementation of Serial Peripheral Interface (SPI) protocol in Verilog for FPGA.

# Dependencies

- [GoogleTest](https://github.com/google/googletest/)


When standing in googletest's root directory.
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