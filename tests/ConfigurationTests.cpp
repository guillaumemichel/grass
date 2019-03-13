#include <assert.h>
#include "ConfigurationTests.h"

void testTest() {
    FileReader fr("grass.conf");
    Configuration conf(fr);
    conf.getPort();
    assert(false);
}
