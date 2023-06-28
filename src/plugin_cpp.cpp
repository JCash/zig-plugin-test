#include <assert.h>
#include <stdio.h>
#include "plugin.h"

extern "C" {
    #include "lua.h"
    #include "lualib.h"
    #include "lauxlib.h"
}

static int Encode(lua_State* L)
{
    // The number of expected items to be on the Lua stack
    // once this struct goes out of scope
    int top = lua_gettop(L);

    // Check and get parameter string from stack
    size_t len;
    char* str = (char*)luaL_checklstring(L, 1, &len);

    for(int i = 0; i < len; ++i) {
        str[i] = str[i]+1;
    }
    lua_pushstring(L, str);
    return 1; // Return 1 item
}

// Functions exposed to Lua
static const luaL_reg Module_methods[] =
{
    {"encode", Encode},
    {0, 0}
};

static void LuaInit(lua_State* L)
{
    int top = lua_gettop(L);
    luaL_register(L, "plugincpp", Module_methods);
    lua_pop(L, 1);
    assert(top == lua_gettop(L));
}

static void* PluginCreate(const char* name, struct lua_State* L)
{
    int* plugin = new int;
    *plugin = 0;
    printf("    PluginCPP: %s: %s %p %d\n", __FUNCTION__, name, plugin, *plugin);
    LuaInit(L);
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
