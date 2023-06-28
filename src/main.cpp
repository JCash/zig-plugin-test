#include <stdio.h>
#include "util.h"
#include "plugin.h"

extern "C"
{
    #include <lua.h>
    #include <lauxlib.h>
    #include <lualib.h>
}

extern "C" void PluginCPP();
extern "C" void PluginZIG();

// By referencing the symbols here, the linker cannot optimize them away
extern "C" void theLinkerTrick()
{
    PluginCPP();
    PluginZIG();
}

int main(int argc, char** argv)
{
    printf("main.cpp: Entering\n");

    lua_State* L = luaL_newstate();
    luaL_openlibs(L);

    plugin_createPlugins(L);

    for (int i = 0; i < 2; ++i)
    {
        plugin_updatePlugins();
    }

    plugin_destroyPlugins();

    printf("main.cpp: Exiting\n");
    return 0;
}
