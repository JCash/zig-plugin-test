#include <stdio.h>
#include "util.h"

extern "C" void mod_init_func();

extern "C" void theLinkerTrick()
{
    mod_init_func();
}

static void* cpp_pluginCreate(const char* name)
{
    int* plugin = new int;
    *plugin = 0;
    printf("  %s: %s %p %d\n", __FUNCTION__, name, plugin, *plugin);
    return (void*)plugin;
}

static void cpp_pluginDestroy(void* _plugin)
{
    int* plugin = (int*)_plugin;
    printf("  %s: %p %d\n", __FUNCTION__, plugin, *plugin);
    delete plugin;
}

static void cpp_pluginUpdate(void* _plugin)
{
    int* plugin = (int*)_plugin;
    (*plugin)++;
    printf("  %s: %p %d\n", __FUNCTION__, plugin, *plugin);
}

struct CPP_PluginRegistrator
{
    Plugin plugin_definition;
    CPP_PluginRegistrator()
    {
        printf("GLOBAL: CPP\n");
        plugin_definition.name = "CPP Plugin";
        plugin_definition.fn_create = cpp_pluginCreate;
        plugin_definition.fn_destroy = cpp_pluginDestroy;
        plugin_definition.fn_update = cpp_pluginUpdate;
        util_registerPlugin(&plugin_definition);
    }
} g_CppPluginAdder;



int main(int argc, char** argv)
{
    printf("main.cpp: Entering\n");

    util_createPlugins();

    for (int i = 0; i < 2; ++i)
    {
        util_updatePlugins();
    }

    util_destroyPlugins();

    printf("main.cpp: Exiting\n");
    return 0;
}
