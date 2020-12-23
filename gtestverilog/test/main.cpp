#include <stdlib.h>
#include <gmock/gmock.h>

int main(int argc, char **argv){

    // initialise GoogleMock + GoogleTest
    ::testing::InitGoogleMock(&argc, argv);

    // Run all of the tests
    return RUN_ALL_TESTS();
}