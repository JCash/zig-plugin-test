const std = @import("std");
const print = std.debug.print;


const c = @cImport({
    @cInclude("lua.h");
    // @cInclude("lualib.h");
    // @cInclude("lauxlib.h");
    @cInclude("./src/plugin.h");
    @cInclude("./src/util.h");
});

const ZigPluginContext = struct {
    some_value: i32,
};


fn zigPluginCreate(name: [*c]const u8, L: ?*c.lua_State) callconv(.C) ?*anyopaque {
    print("    PluginZIG: zigPluginCreate {s} {*}\n", .{name, L});
    const ctx : *ZigPluginContext = std.heap.c_allocator.create(ZigPluginContext) catch return null;
    ctx.some_value = 0;
    return @ptrCast(*anyopaque, ctx);
}

fn zigPluginDestroy(_plugin: ?*anyopaque) callconv(.C) void {
    const plugin : *ZigPluginContext = @ptrCast(*ZigPluginContext, @alignCast(4, _plugin));
    print("    PluginZIG: zigPluginDestroy {*}\n", .{plugin});
    std.heap.c_allocator.destroy(plugin);
}

fn zigPluginUpdate(_plugin: ?*anyopaque) callconv(.C) void {
    const plugin : *ZigPluginContext = @ptrCast(*ZigPluginContext, @alignCast(4, _plugin));
    plugin.some_value += 1;
    print("    PluginZIG: zigPluginUpdate {}\n", .{plugin.some_value});
}


fn allocatePlugin() ?*c.Plugin {
    var plugin : *c.Plugin = std.heap.c_allocator.create(c.Plugin) catch return null;
    return plugin;
}

export fn PluginZIG() callconv(.C) void {
    const plugin : ?*c.Plugin = allocatePlugin();
    c.plugin_registerPlugin(plugin, "PluginZIG", zigPluginCreate, zigPluginDestroy, zigPluginUpdate);
}

export const mod_init_func_ptr linksection("__DATA,__mod_init_func") = &PluginZIG; // note the & op
