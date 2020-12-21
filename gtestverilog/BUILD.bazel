load("@rules_verilator//verilator:defs.bzl", "verilator_cc_library")
load(":defs.bzl", "gtest_verilog_testbench")

py_binary(
    name='generate_testbench',
    srcs=['tools/generate_testbench.py'],
    visibility=['//visibility:public'],
)

cc_library(
    name = "lib",
    srcs = glob([
        "lib/*.cpp",
        "lib/*.h"
    ]),
    hdrs = glob([
        "*.h"
    ]),
    deps = [
        "@com_google_googletest//:gtest",
    ],
    visibility = ["//visibility:public"]
)

cc_test(
    name = "test",
    srcs = glob(
        include =[
            "test/*.cpp",
            "test/*.h"
        ]
    ),
    deps = [
        "@com_google_googletest//:gtest",
        ":lib"
    ],
)

verilator_cc_library(
    name = "Counter",
    srcs = ["example/Counter.v"],
)

gtest_verilog_testbench(
    name = "CounterTestBench",
    deps = [":Counter"],
)

cc_test(
    name = "example",
    srcs = [
            ":CounterTestBench",
    ] + glob(
        include = [
            "example/*.cpp",
            "example/*.h",
        ]
    ),
    deps = [
        "@com_google_googletest//:gtest",
        ":lib",
        ":Counter",
    ]
)