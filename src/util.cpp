#include "util.h"
#include <assert.h>
#include <stdio.h>

// C first
int util_calcValue(int value)
{
    return value*3 + value/2;
}

// ... then add C++ wrappers
int util::calcValue(int value)
{
    return util_calcValue(value);
}


void util::printMsg(const char* msg)
{
    printf("C++ message: %s\n", msg);
}
