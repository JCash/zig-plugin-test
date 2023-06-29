const std = @import("std");

pub fn build(b: *std.build.Builder) void {
    const optimize = b.standardOptimizeOption(.{});
    const target = b.standardTargetOptions(.{});

    const lib = b.addStaticLibrary(.{
                        .name = "plugin_zig",
                        .root_source_file = .{ .path = "src/plugin.zig" },
                        .target = target,
                        .optimize = optimize,
                    });
    lib.addIncludePath("include");
    lib.addIncludePath(".");
    b.installArtifact(lib);
}
