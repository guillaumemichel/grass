#include <assert.h>
#include "../include/FileReader.h"
#include "../include/Configuration.h"
#include "../include/AuthenticationService.h"
#include "AuthenticationServiceTests.h"

void testShouldAcceptToRegisterOnNewSocket() {
    FileReader fr("grass.conf");
    Configuration conf(fr);
    AuthenticationService s(conf);
    assert(s.registerUser(1, "Xavier"));
    assert(s.registerUser(2, "Guissou"));
    assert(s.getAuthenticatedUsers().size() == 0);
}

void testShouldRejectToRegisterOnExistingSocket() {
    FileReader fr("grass.conf");
    Configuration conf(fr);
    AuthenticationService s(conf);
    assert(s.registerUser(1, "Xavier"));
    assert(!s.registerUser(1, "Xavier"));
    assert(!s.registerUser(1, "Guissou"));
    assert(s.getAuthenticatedUsers().size() == 0);
}

void testShouldConnectUserWithCorrectCredentials() {
    FileReader fr("grass.conf");
    Configuration conf(fr);
    AuthenticationService s(conf);
    assert(s.registerUser(1, "Alex"));
    assert(s.login(1, "Acidburn", "CrashOverride"));
    assert(s.getAuthenticatedUsers().size() == 1);
}

void testShouldNotConnectUserWithIncorrectCredentials() {
    FileReader fr("grass.conf");
    Configuration conf(fr);
    AuthenticationService s(conf);
    assert(s.registerUser(1, "Alex"));
    assert(!s.login(1, "Alex", "ElFuego"));
    assert(s.getAuthenticatedUsers().size() == 0);
}

void testShouldRemoveUserOnLogout() {
    FileReader fr("grass.conf");
    Configuration conf(fr);
    AuthenticationService s(conf);
    assert(s.registerUser(1, "Alex"));
    s.logout(1);
    assert(s.getAuthenticatedUsers().size() == 0);
    assert(s.registerUser(1, "Acidburn"));
    assert(s.login(1, "Acidburn", "CrashOverride"));
    assert(s.getAuthenticatedUsers().size() == 1);
    s.logout(1);
    assert(s.getAuthenticatedUsers().size() == 0);
}

void testShouldReturnCorrectListOfAuthenticatedUsers() {
    FileReader fr("grass.conf");
    Configuration conf(fr);
    AuthenticationService s(conf);
    s.registerUser(1, "Acidburn");
    s.login(1, "Acidburn", "CrashOverride");
    assert(s.getAuthenticatedUsers().size() == 1);
    assert(s.getAuthenticatedUsers()[0].getName() == "Acidburn");
    assert(s.getAuthenticatedUsers()[0].isAuthenticated());
}
