#pragma once

#if defined(__cplusplus)
extern "C" {
#endif

    int util_calcValue(int value);

#if defined(__cplusplus)
} // extern "C"
#endif


#ifdef __cplusplus

namespace util
{
    void printMsg(const char* msg);

    int calcValue(int value);
}

#endif // __cplusplus
