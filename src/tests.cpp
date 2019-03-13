#include "../tests/ConfigurationTests.h"
#include "../tests/AuthorizationServiceTests.h"
#include "../tests/ExceptionsTests.h"

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

    //Test for exceptions
    /*
     * Commented out because it prints the exceptions
    testError1();
    testError2();
    */
}
