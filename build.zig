const std = @import("std");

pub fn build(b: *std.build.Builder) void {
    const mode = b.standardReleaseOptions();

    const lib = b.addStaticLibrary("zigtest", "src/zigtest.zig");
    lib.addIncludePath(".");
    lib.setBuildMode(mode);
    lib.install();
}
