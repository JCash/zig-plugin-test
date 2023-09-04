#include "plugin.h"
#include <stdio.h>

static Plugin* g_Plugins = 0;

void plugin_registerPlugin(struct Plugin* plugin, const char* name, FPluginCreate fn_create, FPluginDestroy fn_destroy, FPluginUpdate fn_update)
{
    plugin->next = g_Plugins;
    plugin->name = name;
    plugin->fn_create = fn_create;
    plugin->fn_destroy = fn_destroy;
    plugin->fn_update = fn_update;
    g_Plugins = plugin;
    printf("Registered plugin '%s'\n", g_Plugins->name);
}

void plugin_createPlugins()
{
    Plugin* plugin = g_Plugins;

    printf("  Creating plugins:\n");
    while(plugin)
    {
        plugin->plugin = plugin->fn_create(plugin->name);
        plugin = plugin->next;
    }
}

void plugin_destroyPlugins()
{
    Plugin* plugin = g_Plugins;

    printf("  Destroying plugins:\n");
    while(plugin)
    {
        plugin->fn_destroy(plugin->plugin);
        plugin = plugin->next;
    }
}

void plugin_updatePlugins()
{
    Plugin* plugin = g_Plugins;
    printf("  Updating plugins:\n");
    while(plugin)
    {
        plugin->fn_update(plugin->plugin);
        plugin = plugin->next;
    }
}
