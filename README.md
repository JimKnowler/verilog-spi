# Verilog-SPI

Test Driven Implementation of Serial Peripheral Interface (SPI) protocol in Verilog for FPGA.

# References 

- [SparkFun -  SPI Tutorial](https://learn.sparkfun.com/tutorials/serial-peripheral-interface-spi/all)
- [NandLand - SPI in Verilog](https://www.youtube.com/watch?v=TR0Pw89EuGk&list=PLnAoag7Ew-vq5kOyfyNN50xL718AtLoCQ&index=3)
- [fpga4fun - SPI](https://www.fpga4fun.com/SPI1.html)
- [Wikipedia - SPI](https://en.wikipedia.org/wiki/Serial_Peripheral_Interface)

# Dependencies

- [Bazel](https://docs.bazel.build/versions/3.7.0/install.html)


# Features

- Verilog SPI Controller

- Verilog SPI Peripheral

- SPI MODE 1
  - CPOL = 0 (clock polarity - IDLE at 0)
  - CPHA = 1 (clock phase - signal on COPI and CIPO change on rising edge, and are sampled on falling edge)

# Build & Run Unit Tests for SPI Verilog

While standing in project's root directory, use Bazel to build the test target:

```
$ bazel build //spi:test --incompatible_require_linker_input_cc_api=false
```

The test executable can be run from the 'bazel-bin' output directory with:

```
$ ./bazel-bin/spi/test
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
