#include "../tests/ConfigurationTests.h"
#include "../tests/AuthorizationServiceTests.h"

int main(void) {
    // Tests for Configuration
    testGetBaseShouldReturnCorrectBaseOnStandardConfig();
    testGetBaseShouldReturnErrorOnMissingEntry();
    testGetBaseShouldReturnErrorOnMissingValue();
    testGetPortShouldReturn8888OnStandardConfig();
    testGetPortShouldReturnErrorOnMissingEntry();
    testGetPortShouldReturnErrorOnMissingValue();
    testGetUsersShouldReturnCorrectMapOnStandardConfig();
    testGetUsersShouldReturnEmptyMapOnMissingEntries();
    testGetUsersShouldIgnoreMalformedEntries();

    // Tests for AuthorizationService
    testsForAuthenticatedUser();
    testsForUnauthenticatedUser();
}