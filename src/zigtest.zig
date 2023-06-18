const std = @import("std");
const print = std.debug.print;

const c = @cImport({
    @cInclude("./src/util.h");
});

const ZigPluginContext = struct {
    some_value: i32,
};

fn zigPluginCreate(name: [*c]const u8) callconv(.C) ?*anyopaque {
    print("  zigPluginCreate {s}\n", .{name});
    const ctx : *ZigPluginContext = std.heap.c_allocator.create(ZigPluginContext) catch return null;
    ctx.some_value = 0;
    return @ptrCast(*anyopaque, ctx);
}

fn zigPluginDestroy(_plugin: ?*anyopaque) callconv(.C) void {
    const plugin : *ZigPluginContext = @ptrCast(*ZigPluginContext, @alignCast(4, _plugin));
    print("zigPluginDestroy {*}\n", .{plugin});
    std.heap.c_allocator.destroy(plugin);
}

fn zigPluginUpdate(_plugin: ?*anyopaque) callconv(.C) void {
    const plugin : *ZigPluginContext = @ptrCast(*ZigPluginContext, @alignCast(4, _plugin));
    plugin.some_value += 1;
    print("  zigPluginUpdate {}\n", .{plugin.some_value});
}


fn createZigPlugin() ?*c.Plugin {
    var plugin : *c.Plugin = std.heap.c_allocator.create(c.Plugin) catch return null;
    plugin.name = "Zig Plugin";
    plugin.fn_create = zigPluginCreate;
    plugin.fn_destroy = zigPluginDestroy;
    plugin.fn_update = zigPluginUpdate;
    return plugin;
}

export fn mod_init_func() callconv(.C) void {
    const plugin : ?*c.Plugin = createZigPlugin();
    c.util_registerPlugin(plugin);
}

export const mod_init_func_ptr linksection("__DATA,__mod_init_func") = &mod_init_func; // note the & op
