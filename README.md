# Verilog-SPI

Test Driven Implementation of Serial Peripheral Interface (SPI) protocol in Verilog for FPGA.

# References 

- [SparkFun -  SPI Tutorial](https://learn.sparkfun.com/tutorials/serial-peripheral-interface-spi/all)
- [NandLand - SPI in Verilog](https://www.youtube.com/watch?v=TR0Pw89EuGk&list=PLnAoag7Ew-vq5kOyfyNN50xL718AtLoCQ&index=3)
- [fpga4fun - SPI](https://www.fpga4fun.com/SPI1.html)
- [Wikipedia - SPI](https://en.wikipedia.org/wiki/Serial_Peripheral_Interface)

# Dependencies

- [Bazel](https://docs.bazel.build/versions/3.7.0/install.html)


# Build & Run Unit Tests for SPI Verilog

While standing in project's root directory, use Bazel to build the test target:

```
$ bazel build //spi:test
```

The test executable can be run from the 'bazel-bin' output directory with:

```
$ ./bazel-bin/spi/test
```

# Build & Run Unit Tests for gtestverilog

While standing in project's root directory, use Bazel to build the test target:

```
$ bazel build //gtestverilog:test
```

The test executable can be run from the 'bazel-bin' output directory with:

```
$ ./bazel-bin/gtestverilog/test
```
# Build & Run 'counter' Example for gtestverilog

While standing in project's root directory, use Bazel to build the test target:

```
$ bazel build //gtestverilog:example
```

The test executable can be run from the 'bazel-bin' output directory with:

```
$ ./bazel-bin/gtestverilog/example
```

# NOTES

## SPI Terminology

Preferring use of [CONTROLLER and PERIPHERAL](https://www.sparkfun.com/spi_signal_names?_ga=2.39707619.1760252179.1606575342-655091437.1606575342) terminology:

# TODO

- support all 4 SPI modes by using verilator arg at verilation time, such as '-GSPI_MODE=3' 
   - generate 4 versions of SPIController and SPIPeripheral
   - run unit tests on each version
     - can we parameterise the same set of sets?

- support different SPI clock speeds on SPIController by using verilator arg at verilation time, such as '-GCLKS_PER_HALF_BIT=3'
   - generate multiple versions of SPIController
   - run unit tests on each version
     - can we parameterise the same set of sets?
