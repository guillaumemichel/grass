#include <assert.h>
#include "ConfigurationTests.h"

void testGetBaseShouldReturnDotOnStandardConfig() {
    FileReader fr("grass.conf");
    Configuration conf(fr);
    assert(conf.getBase() == ".");
}

void testGetPortShouldReturn1337OnStandardConfig() {
    FileReader fr("grass.conf");
    Configuration conf(fr);
    assert(conf.getPort() == (unsigned int) 1337);
}