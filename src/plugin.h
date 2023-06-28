#pragma once

#if defined(__cplusplus)
extern "C" {
#endif

#include "lua.h"
// #include <lualib.h>
// #include <lauxlib.h>


// Public
typedef void* (*FPluginCreate)(const char* name, struct lua_State* L);
typedef void  (*FPluginDestroy)(void* plugin);
typedef void  (*FPluginUpdate)(void* plugin);


// Internal
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

#define PLUGIN_PASTE_SYMREG(x, y) x ## y
#define PLUGIN_PASTE_SYMREG2(x, y) PLUGIN_PASTE_SYMREG(x, y)

void plugin_registerPlugin(struct Plugin* plugin, const char*, FPluginCreate, FPluginDestroy, FPluginUpdate);

// Public
void plugin_createPlugins(struct lua_State* L);
void plugin_destroyPlugins();
void plugin_updatePlugins();

#ifdef __GNUC__
    // Workaround for dead-stripping on OSX/iOS. The symbol "name" is explicitly exported. See wscript "exported_symbols"
    // Otherwise it's dead-stripped even though -no_dead_strip_inits_and_terms is passed to the linker
    // The bug only happens when the symbol is in a static library though
    #define REGISTER_PLUGIN(symbol, desc, name, fn_create, fn_destroy, fn_update) extern "C" void __attribute__((constructor)) symbol () { \
        plugin_registerPlugin(&desc, name, fn_create, fn_destroy, fn_update); \
    }
#else
    #define REGISTER_PLUGIN(symbol, desc, name, fn_create, fn_destroy, fn_update) extern "C" void symbol () { \
        plugin_registerPlugin(&desc, name, fn_create, fn_destroy, fn_update); \
        }\
        int symbol ## Wrapper(void) { symbol(); return 0; } \
        __pragma(section(".CRT$XCU",read)) \
        __declspec(allocate(".CRT$XCU")) int (* _Fp ## symbol)(void) = symbol ## Wrapper;
#endif

#define DECLARE_PLUGIN(symbol, name, fn_create, fn_destroy, fn_update) \
    static Plugin PLUGIN_PASTE_SYMREG2(symbol, __LINE__); \
    REGISTER_PLUGIN(symbol, PLUGIN_PASTE_SYMREG2(symbol, __LINE__), name, fn_create, fn_destroy, fn_update);

#if defined(__cplusplus)
} // extern "C"
#endif
