const std = @import("std");

const assert = std.debug.assert;

const Protocol = struct {
    inputFileName: []const u8,
    type: []const u8,
    outputFileName: []const u8,
};

pub fn build(b: *std.build.Builder) !void {
    const target = b.standardTargetOptions(.{});

    const mode = b.standardReleaseOptions();

    const xwayland = b.option(bool, "xwayland", "Enable XWayland support") orelse false;

    const exe = b.addExecutable("dwl", null);
    exe.setTarget(target);
    exe.setBuildMode(mode);
    exe.linkSystemLibrary("wlroots");
    exe.linkSystemLibrary("wayland-server");
    exe.linkSystemLibrary("xkbcommon");
    exe.linkSystemLibrary("libinput");

    if (xwayland) {
        exe.linkSystemLibrary("xcb");
        exe.linkSystemLibrary("xcb-icccm");
    }

    exe.linkLibC();
    exe.install();

    var ret: u8 = undefined;
    const scanner_path = std.mem.trimRight(u8, try b.execAllowFail(
        &[_][]const u8{ "pkg-config", "--variable=wayland_scanner", "wayland-scanner" },
        &ret,
        .Inherit,
    ), &std.ascii.spaces);

    const wayland_protocols_path = std.mem.trimRight(u8, try b.execAllowFail(
        &[_][]const u8{ "pkg-config", "--variable=pkgdatadir", "wayland-protocols" },
        &ret,
        .Inherit,
    ), &std.ascii.spaces)[1..];

    if (b.verbose) {
        std.log.info("Wayland-Scanner={s}", .{scanner_path});
        std.log.info("Wayland-Protocols={s}\n", .{wayland_protocols_path});
    }

    const protocol_xdg_shell = try std.fmt.allocPrint(
        b.allocator,
        "{s}/stable/xdg-shell/xdg-shell.xml",
        .{wayland_protocols_path},
    );

    const protocols = [_]Protocol{
        .{ .inputFileName = protocol_xdg_shell, .type = "server-header", .outputFileName = "xdg-shell-protocol.h" },
        .{ .inputFileName = "protocols/wlr-layer-shell-unstable-v1.xml", .type = "server-header", .outputFileName = "wlr-layer-shell-unstable-v1-protocol.h" },
        .{ .inputFileName = "protocols/dwl-ipc-unstable-v2.xml", .type = "server-header", .outputFileName = "dwl-ipc-unstable-v2-protocol.h" },
        .{ .inputFileName = "protocols/dwl-ipc-unstable-v2.xml", .type = "private-code", .outputFileName = "dwl-ipc-unstable-v2-protocol.c" },
    };

    for (protocols) |protocol| {
        if (b.verbose) {
            std.debug.print("{s} {s} {s} {s}\n", .{ scanner_path, protocol.type, protocol.inputFileName, protocol.outputFileName });
        }
        _ = try b.execAllowFail(
            &[_][]const u8{ scanner_path, protocol.type, protocol.inputFileName, protocol.outputFileName },
            &ret,
            .Inherit,
        );
    }

    // currently includes are scatter...
    exe.addIncludePath(".");

    var cflags = [_][]const u8{ "-std=c11", "-pedantic", "-Wall", "-Wextra", "-Wdeclaration-after-statement", "-Wno-unused-parameter", "-Wno-sign-compare", "-Wshadow", "-Wunused-macros", "-Werror=strict-prototypes", "-Werror=implicit", "-Werror=return-type", "-Werror=incompatible-pointer-types", "-DWLR_USE_UNSTABLE", "-D_POSIX_C_SOURCE=200809L", "-DVERSION=\"0.4\"", "-DXWAYLAND" };

    if (!xwayland) {
        exe.addCSourceFiles(&.{ "dwl.c", "util.c", "dwl-ipc-unstable-v2-protocol.c" }, cflags[0 .. cflags.len - 1]);
    } else {
        exe.addCSourceFiles(&.{ "dwl.c", "util.c", "dwl-ipc-unstable-v2-protocol.c" }, &cflags);
    }

    const run_cmd = exe.run();
    run_cmd.step.dependOn(b.getInstallStep());
    if (b.args) |args| {
        run_cmd.addArgs(args);
    }

    const run_step = b.step("run", "Run the app");
    run_step.dependOn(&run_cmd.step);
}
