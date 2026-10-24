//! Small real SDK codec/source-debug probe, no ROS import or native delivery.
//! Trap failed assertions directly to avoid the full standard test runner.
const std = @import("std");
const ros = @import("client");
pub const panic = std.debug.no_panic;
var input: u32 = 3;
var observed: u32 = 0;
const Record = extern struct { code: u32, valid: bool, seed: u32 };
pub noinline fn codec_probe(seed: u32) u32 {
    var wire: [64]u8 = undefined;
    const text = "Zig\x00\xe4\xb8\xad";
    const encoded = ros.encodeString(text, &wire) catch @trap();
    const decoded = ros.decodeString(encoded) orelse @trap();
    const valid = std.mem.eql(u8, decoded, text);
    const code: u32 = if (valid and seed == 3) 13 else 22;
    const record = Record{ .code = code, .valid = valid, .seed = seed };
    const live: *volatile const Record = &record;
    const sink: *volatile u32 = &observed;
    sink.* = live.code; // ZIG_SDK_CODEC_READY
    if (!live.valid or live.seed != 3 or live.code != 13) @trap();
    return live.code;
}
pub export fn _start() callconv(.c) void {
    const source: *volatile u32 = &input;
    if (codec_probe(source.*) != 13) @trap();
}
