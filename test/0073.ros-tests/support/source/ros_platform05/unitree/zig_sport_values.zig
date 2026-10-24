//! Actual complete-record Zig/C++ values and local lifecycle, with no grants.
const std = @import("std");
const unitree = @import("unitree");
const c = @cImport({
    @cInclude("platform/unitree/c/roswasm_unitree/packet_bridge.h");
});

fn require(value: bool) void {
    if (!value) @panic("Unitree Zig value contract failed");
}
fn original() unitree.Request {
    var value = std.mem.zeroes(unitree.Request);
    value.header.identity.id = 9007199254740993;
    value.header.identity.api_id = -77;
    value.header.lease.id = -9007199254740993;
    value.header.policy.priority = -101;
    value.parameter.size = 4;
    @memcpy(value.parameter.data[0..4], "keep");
    value.binary.size = 4;
    @memcpy(value.binary.data[0..4], &[_]u8{ 0, 255, 1, 128 });
    return value;
}
fn one(family: unitree.Family, name: []const u8) void {
    const operation = unitree.lookupOperation(family, name) orelse @panic("missing real method");
    var found: u32 = 0;
    var shape: u32 = 99;
    require(c.uwvm_unitree_packet_lookup(if (family == .go2) 1 else 2, name.ptr, name.len, &found, &shape) == 0);
    require(found == operation.id() and shape == @intFromEnum(operation.shape()));
    var scalar = std.mem.zeroes(unitree.Arguments);
    var points = std.mem.zeroes([30]unitree.PathPoint);
    var flat = std.mem.zeroes([210]f32);
    var count: usize = 0;
    switch (operation.shape()) {
        .none => {},
        .xyz, .position => {
            scalar.x = 1.25;
            scalar.y = -0.0;
            scalar.z = 0.5;
        },
        .integer => scalar.level = std.math.minInt(i32),
        .boolean => scalar.flag = 1,
        .height => scalar.x = -0.125,
        .trajectory => {
            count = 30;
            for (&points, 0..) |*point, i| {
                point.timeFromStart = @floatFromInt(i);
                point.y = -0.0;
                flat[7 * i] = point.timeFromStart;
                flat[7 * i + 2] = point.y;
            }
        },
    }
    var source = original();
    var actual: unitree.Request = undefined;
    const code = switch (operation) {
        .go2 => |method| unitree.prepareGo2(&source, method, scalar, &actual),
        .b2 => |method| unitree.prepareB2(&source, method, scalar, points[0..count], &actual),
    };
    require(code == 0);
    var input: [6181]u8 = undefined;
    var expected: [6181]u8 = undefined;
    var output: [6181]u8 = undefined;
    var n: usize = 0;
    var expected_n: usize = 0;
    var actual_n: usize = 0;
    require(unitree.encodeRequest(&source, &input, &n) == 0);
    require(c.uwvm_unitree_packet_prepare(if (family == .go2) 1 else 2, found, &input, n, scalar.x, scalar.y, scalar.z, scalar.level, scalar.flag, if (count == 0) null else &flat, count, &expected, expected.len, &expected_n) == 0);
    require(unitree.encodeRequest(&actual, &output, &actual_n) == 0);
    require(std.mem.eql(u8, expected[0..expected_n], output[0..actual_n]));
    require(source.header.identity.api_id == -77 and source.binary.data[0] == 0);
    require(unitree.prepareGo2(&source, .Move, scalar, &source) == 22);
}

pub fn main() !void {
    var methods: usize = 0;
    inline for (@typeInfo(unitree.Go2).@"enum".fields) |field| {
        one(.go2, field.name);
        methods += 1;
    }
    inline for (@typeInfo(unitree.B2).@"enum".fields) |field| {
        one(.b2, field.name);
        methods += 1;
    }
    require(methods == 60);
    var pending: unitree.Pending = .{};
    require(pending.init(.go2, 0) == 0);
    var copied = pending;
    var next: i64 = 999;
    require(copied.nextIdentity(&next) == 9 and next == 999);
    var request = original();
    request.header.identity.api_id = 1008;
    var id: i64 = 1;
    while (id <= 16) : (id += 1) {
        request.header.identity.id = id;
        require(pending.register(&request, 10, 100) == 0);
    }
    request.header.identity.id = 17;
    require(pending.register(&request, 10, 100) == 11);
    require(pending.nextIdentity(&next) == 0 and next == 17);
    require(pending.forget(1, 1008) == 0);
    request.header.identity.id = 1;
    require(pending.register(&request, 10, 100) == 17);
    request.header.identity.id = 17;
    require(pending.register(&request, 9, 100) == 22);
    require(pending.register(&request, 11, 100) == 0);
    var response = std.mem.zeroes(unitree.Response);
    response.header.identity.id = 17;
    response.header.identity.api_id = 1008;
    response.header.status.code = std.math.minInt(i32);
    response.binary.size = 4;
    @memcpy(response.binary.data[0..4], &[_]i8{ -128, -1, 0, 127 });
    require(pending.checkResponse(&response, 12) == 0);
    response.header.identity.api_id = 2055;
    require(pending.accept(&response, 12) == 9);
    response.header.identity.api_id = 1008;
    require(pending.accept(&response, 13) == 0);
    require(response.header.status.code == std.math.minInt(i32) and response.binary.data[0] == -128);
    require(pending.accept(&response, 13) == 9);
    request.header.identity.id = 18;
    require(pending.register(&request, 12, 100) == 22);
    require(pending.register(&request, 13, 100) == 0);
    require(pending.expire(18, 1008, 14) == 11);
    require(pending.expire(18, 1008, 13) == 22);
    require(pending.expire(18, 1008, 100) == 110);
    require(pending.register(&request, 100, 200) == 17);
    const alias = &pending;
    require(pending.close() == 0 and alias.close() == 0);
    require(alias.nextIdentity(&next) == 9);
    require(alias.init(.go2, 0) == 9);
    var exhausted: unitree.Pending = .{};
    require(exhausted.init(.b2, std.math.maxInt(i64)) == 0);
    require(exhausted.nextIdentity(&next) == 75);
    require(exhausted.close() == 0);
    response.header.identity.api_id = 2055;
    response.header.status.code = 0;
    var buffer: [32768]u8 = undefined;
    for ([_][]const u8{ "{\"data\":true}", "{\"data\":false}", "{}", "{\"data\":1}", "{\"data\":true} trailing" }, 0..) |text, i| {
        response.data.size = @intCast(text.len);
        @memcpy(response.data.data[0..text.len], text);
        var memory = std.heap.FixedBufferAllocator.init(&buffer);
        const parsed = unitree.recoveryValue(&response, memory.allocator());
        if (i < 2) require(parsed.code == 0 and parsed.has_value and parsed.value == (i == 0)) else require(parsed.code == 22 and !parsed.has_value);
    }
    response.header.status.code = std.math.minInt(i32);
    var memory = std.heap.FixedBufferAllocator.init(&buffer);
    const native = unitree.recoveryValue(&response, memory.allocator());
    require(native.code == 0 and !native.has_value and native.native_code == std.math.minInt(i32));
    var conversation: unitree.Conversation = .{};
    require(conversation.open(.go2, "same", "same") == 22);
    try std.io.getStdOut().writeAll("UNITREE_ZIG_VALUES_OK operations=60 fixed_slots=16\n");
}
