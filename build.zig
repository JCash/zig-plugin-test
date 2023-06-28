const std = @import("std");

pub fn build(b: *std.build.Builder) void {
    const mode = b.standardReleaseOptions();

    const lib = b.addStaticLibrary("plugin_zig", "src/plugin.zig");
    lib.addIncludePath("include");
    lib.addIncludePath(".");
    lib.setBuildMode(mode);
    lib.install();
}
