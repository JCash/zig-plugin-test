const std = @import("std");
const print = std.debug.print;


const c = @cImport({
    //@cInclude("lauxlib.h");
    @cInclude("./src/plugin.h");
});

const ZigPluginContext = struct {
    some_value: i32,
};


fn PluginCreate(name: [*c]const u8) callconv(.C) ?*anyopaque {
    print("    PluginZIG: PluginCreate {s}\n", .{name});
    const ctx : *ZigPluginContext = std.heap.c_allocator.create(ZigPluginContext) catch return null;
    ctx.some_value = 0;
    return @ptrCast(ctx);
}

fn PluginDestroy(_plugin: ?*anyopaque) callconv(.C) void {
    const plugin : *ZigPluginContext = @ptrCast(@alignCast(_plugin));
    print("    PluginZIG: PluginDestroy {*}\n", .{plugin});
    std.heap.c_allocator.destroy(plugin);
}

fn PluginUpdate(_plugin: ?*anyopaque) callconv(.C) void {
    const plugin : *ZigPluginContext = @ptrCast(@alignCast(_plugin));
    plugin.some_value += 1;
    print("    PluginZIG: PluginUpdate {}\n", .{plugin.some_value});
}

export fn PluginZIG() callconv(.C) void {
    const plugin : ?*c.Plugin = std.heap.c_allocator.create(c.Plugin) catch null;
    c.plugin_registerPlugin(plugin, "PluginZIG", PluginCreate, PluginDestroy, PluginUpdate);
}

export const mod_init_func_ptr linksection("__DATA,__mod_init_func") = &PluginZIG; // note the & op
