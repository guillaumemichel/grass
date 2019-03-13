#include <assert.h>
#include <map>
#include <string>
#include <iostream>
#include "ConfigurationTests.h"

void testGetBaseShouldReturnCorrectBaseOnStandardConfig() {
    FileReader fr("tests/testables/grass.conf");
    Configuration conf(fr);
    assert(conf.getBase() == "my_super_base");
}

void testGetBaseShouldReturnErrorOnMissingEntry() {
    FileReader fr("tests/testables/no_base_grass.conf");
    Configuration conf(fr);
    assert(conf.getBase() == "NOT FOUND");

}

void testGetBaseShouldReturnErrorOnMissingValue() {
    FileReader fr1("tests/testables/malformed_base_grass_1.conf");
    Configuration conf1(fr1);
    assert(conf1.getBase() == "NOT FOUND");

    FileReader fr2("tests/testables/malformed_base_grass_2.conf");
    Configuration conf2(fr2);
    assert(conf2.getBase() == "NOT FOUND");
}

void testGetPortShouldReturn8888OnStandardConfig() {
    FileReader fr("tests/testables/grass.conf");
    Configuration conf(fr);
    assert(conf.getPort() == (unsigned int) 8888);
}

void testGetPortShouldReturnErrorOnMissingEntry() {
    FileReader fr("tests/testables/no_port_grass.conf");
    Configuration conf(fr);
    assert(conf.getPort() == (unsigned int) -1);

}

void testGetPortShouldReturnErrorOnMissingValue() {
    FileReader fr1("tests/testables/malformed_port_grass_1.conf");
    Configuration conf1(fr1);
    assert(conf1.getPort() == (unsigned int) -1);

    FileReader fr2("tests/testables/malformed_port_grass_2.conf");
    Configuration conf2(fr2);
    assert(conf2.getPort() == (unsigned int) -1);

    FileReader fr3("tests/testables/malformed_port_grass_3.conf");
    Configuration conf3(fr3);
    try { conf3.getPort(); }
    catch(...) { return; }
    assert(false);
}

void testGetUsersShouldReturnCorrectMapOnStandardConfig() {
    FileReader fr("tests/testables/grass.conf");
    Configuration conf(fr);
    map<string, string> users = conf.getUsers();
    map<string, string> usersTest = {
            {"XavierP", "TopSecretPasswd"},
            {"GuillaumeMichel", "FreeGuissou"},
            {"Alex", "ElFuego"}
    };

    assert(users.size() == usersTest.size());
    assert(equal(users.begin(), users.end(), usersTest.begin()));
}

void testGetUsersShouldReturnEmptyMapOnMissingEntries() {
    FileReader fr("tests/testables/no_users_grass.conf");
    Configuration conf(fr);
    assert(conf.getUsers().size() == 0);
}

void testGetUsersShouldIgnoreMalformedEntries() {
    FileReader fr("tests/testables/malformed_users_grass.conf");
    Configuration conf(fr);
    map<string, string> users = conf.getUsers();
    map<string, string> usersTest = {
            {"GuillaumeMichel", "FreeGuissou"},
            {"Alex", "ElFuego"}
    };

    assert(users.size() == usersTest.size());
    assert(equal(users.begin(), users.end(), usersTest.begin()));
}