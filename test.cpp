#include <stdlib.h>
#include <verilated.h>
#include <gtest/gtest.h>

int main(int argc, char **argv){
    // initialise Verilator
    Verilated::commandArgs(argc, argv);

    // initialise GoogleTest
    ::testing::InitGoogleTest(&argc, argv);

    // Run all of the tests
    return RUN_ALL_TESTS();
}