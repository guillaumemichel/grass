#include "../tests/ConfigurationTests.h"

int main(void) {
    // Tests for configuration
    testGetBaseShouldReturnDotOnStandardConfig();
    testGetPortShouldReturn1337OnStandardConfig();
}