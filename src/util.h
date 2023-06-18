#pragma once

#if defined(__cplusplus)
extern "C" {
#endif

typedef void* (*FPluginCreate)(const char* name);
typedef void  (*FPluginDestroy)(void* plugin);
typedef void  (*FPluginUpdate)(void* plugin);

struct Pod
{
    int test;
};

struct Plugin
{
    // public
    const char*     name;
    FPluginCreate   fn_create;
    FPluginDestroy  fn_destroy;
    FPluginUpdate   fn_update;

    // private
    void* plugin;
    struct Plugin* next;
};

void util_registerPlugin(struct Plugin* plugin_definition);
void util_createPlugins();
void util_destroyPlugins();
void util_updatePlugins();


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
