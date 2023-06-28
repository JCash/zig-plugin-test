#include "plugin.h"
#include <stdio.h>

static void* PluginCreate(const char* name, struct lua_State* L)
{
    int* plugin = new int;
    *plugin = 0;
    printf("    PluginCPP: %s: %s %p %d\n", __FUNCTION__, name, plugin, *plugin);
    return (void*)plugin;
}

static void PluginDestroy(void* _plugin)
{
    int* plugin = (int*)_plugin;
    printf("    PluginCPP: %s: %p %d\n", __FUNCTION__, plugin, *plugin);
    delete plugin;
}

static void PluginUpdate(void* _plugin)
{
    int* plugin = (int*)_plugin;
    (*plugin)++;
    printf("    PluginCPP: %s: %p %d\n", __FUNCTION__, plugin, *plugin);
}

DECLARE_PLUGIN(PluginCPP, "PluginCPP", PluginCreate, PluginDestroy, PluginUpdate);
