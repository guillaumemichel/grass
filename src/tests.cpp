#include "../tests/ConfigurationTests.h"

int main(void) {
    // Tests for configuration
    testGetBaseShouldReturnCorrectBaseOnStandardConfig();
    testGetBaseShouldReturnErrorOnMissingEntry();
    testGetBaseShouldReturnErrorOnMissingValue();
    testGetPortShouldReturn8888OnStandardConfig();
    testGetPortShouldReturnErrorOnMissingEntry();
    testGetPortShouldReturnErrorOnMissingValue();
    testGetUsersShouldReturnCorrectMapOnStandardConfig();
    testGetUsersShouldReturnEmptyMapOnMissingEntries();
    testGetUsersShouldIgnoreMalformedEntries();
}