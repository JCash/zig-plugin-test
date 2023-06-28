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

static bool RunString(lua_State* L, const char* script)
{
    if (luaL_dostring(L, script) != 0)
    {
        printf("Error: %s", lua_tolstring(L, -1, 0));
        return false;
    }
    return true;
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

    // Call some Lua script
    printf("\nmain.cpp: Run lua script\n\n");
    const char* script = "\n"
    "local s = 'Hello World!'\n" \
    "print('String: ', s)\n" \
    "local e = plugincpp.encode(s)\n" \
    "print('Encoded (C++):', e)\n" \
    "local d = pluginzig.decode(e)\n" \
    "print('Decoded (Zig):', d)\n" \
    ;
    RunString(L, script);

    printf("\nmain.cpp: End script\n\n");

    plugin_destroyPlugins();

    printf("main.cpp: Exiting\n");
    return 0;
}
