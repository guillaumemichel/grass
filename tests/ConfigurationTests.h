#include "../include/Configuration.h"

#ifndef ASS_ON_THE_GRASS_CONFIGURATIONTESTS_H
#define ASS_ON_THE_GRASS_CONFIGURATIONTESTS_H

void testGetBaseShouldReturnDotOnStandardConfig();
void testGetBaseShouldReturnErrorOnMissingEntry();
void testGetBaseShouldReturnErrorOnMissingValue();
void testGetPortShouldReturn8888OnStandardConfig();
void testGetPortShouldReturnErrorOnMissingEntry();
void testGetPortShouldReturnErrorOnMissingValue();

#endif //ASS_ON_THE_GRASS_CONFIGURATIONTESTS_H
