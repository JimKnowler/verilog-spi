load("@rules_verilator//verilator:defs.bzl", "verilator_cc_library")
load("@gtestverilog//gtestverilog:defs.bzl", "gtest_verilog_testbench")

verilator_cc_library(
    name = "SPIController",
    srcs = ["SPIController.v"],
)

gtest_verilog_testbench(
    name = "SPIControllerTestBench",
    deps = [":SPIController"]
)

verilator_cc_library(
    name = "SPIPeripheral",
    srcs = ["SPIPeripheral.v"],
)

gtest_verilog_testbench(
    name = "SPIPeripheralTestBench",
    deps = [":SPIPeripheral"]
)

cc_test(
    name = "test",
    srcs = glob(
        include =[
            "**/*.cpp",
            "**/*.h"
        ]
    ) + [
        ":SPIControllerTestBench",
        ":SPIPeripheralTestBench"
    ],
    deps = [
        "@com_google_googletest//:gtest",
        "@gtestverilog//gtestverilog:lib",
        ":SPIController",
        ":SPIPeripheral"
    ],
)
