#include "util.h"
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

static Plugin* g_Plugins = 0;

void util_registerPlugin(Plugin* plugin_definition)
{
    plugin_definition->next = g_Plugins;
    g_Plugins = plugin_definition;
    printf("Registered plugin '%s'\n", g_Plugins->name);
}

void util_createPlugins()
{
    Plugin* plugin = g_Plugins;

    printf("Creating plugins:\n");
    while(plugin)
    {
        plugin->plugin = plugin->fn_create(plugin->name);
        printf("Created plugin '%s'\n", plugin->name);
        plugin = plugin->next;
    }
}

void util_destroyPlugins()
{
    Plugin* plugin = g_Plugins;

    printf("Destroying plugins:\n");
    while(plugin)
    {
        plugin->fn_destroy(plugin->plugin);
        plugin = plugin->next;
    }
}

void util_updatePlugins()
{
    Plugin* plugin = g_Plugins;
    printf("Updating plugins:\n");
    while(plugin)
    {
        plugin->fn_update(plugin->plugin);
        plugin = plugin->next;
    }
}
