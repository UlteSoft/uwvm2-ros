//! Actual WASI checks. Synthetic handle states exercise local lifecycle and
//! actual Host rejection only; they are not positive native ROS qualification.
const std = @import("std");
const ros = @import("client");
const raw = @import("ros");
fn expect(value: bool) void {
    if (!value) std.process.exit(1);
}
fn group(comptime name: []const u8) void {
    std.io.getStdOut().writer().writeAll("PASS_ZIG_SDK_" ++ name ++ "\n") catch std.process.exit(1);
}
var too_big: [ros.max_output + 1]u8 = undefined;
pub fn main() void {
    expect(raw.abi_version() == 1);
    var bytes: [128]u8 = undefined;
    const text = "\xef\xbb\xbfZig\x00\xe4\xb8\xad";
    const wire = ros.encodeString(text, &bytes) catch {
        std.process.exit(1);
    };
    expect(std.mem.eql(u8, ros.decodeString(wire).?, text));
    expect(ros.decodeString(&.{ 0, 0, 0, 0, 1 }) == null);
    expect(ros.decodeString(&.{ 1, 0, 0, 0, 255 }) == null);
    if (ros.encodeString("\xff", &bytes)) |_| {
        std.process.exit(1);
    } else |e| expect(e == error.Invalid);
    if (ros.encodeString("x", bytes[0..4])) |_| {
        std.process.exit(1);
    } else |e| expect(e == error.NoSpace);
    group("STRING_CODEC");

    const id: u64 = 0xfedcba9876543210;
    const response = ros.encodeTriggerResponse(id, true, text, &bytes) catch {
        std.process.exit(1);
    };
    expect(ros.decodeTriggerRequest(response[0..8]).? == id);
    const decoded = ros.decodeServiceResponse(response[8..]).?;
    expect(decoded.success and std.mem.eql(u8, decoded.message, text));
    expect(ros.decodeTriggerRequest(&([_]u8{0} ** 8)) == null);
    bytes[8] = 2;
    expect(ros.decodeServiceResponse(bytes[8..response.len]) == null);
    group("SERVICE_ID_AND_CODEC");

    var state: ros.State = .{};
    for ([_][]const u8{ "", "a\x00b", "\xff" }) |alias| expect(ros.open(&state, alias).code == ros.invalid);
    const denied = ros.open(&state, "ungranted-telemetry");
    expect(denied.code == ros.denied and denied.owner == null and !state.spent and state.handle == 0);
    group("ACTUAL_DEFAULT_DENIAL");

    // These deliberately invalid handle bits have no Host authority.
    state = .{ .spent = true, .handle = 0xfedcba9876543210 };
    const owner = ros.Owner{ .state = &state };
    const copy = owner;
    state.leased = true;
    expect(copy.close() == ros.busy and state.handle == id);
    state.leased = false;
    const close_code = owner.close();
    expect(close_code == ros.bad_handle or close_code == ros.denied);
    expect(state.handle == 0 and copy.close() == ros.bad_handle and copy.revoke() == ros.bad_handle);
    expect(ros.open(&state, "sub").code == ros.bad_handle);
    group("COPY_RETIREMENT");

    state = .{ .spent = true, .handle = id };
    expect(owner.watch(copy, 0) == ros.invalid);
    expect(owner.submit(copy, &.{}).code == ros.invalid);
    var second: ros.State = .{ .spent = true, .handle = 7, .leased = true };
    const other = ros.Owner{ .state = &second };
    expect(owner.watch(other, id) == ros.busy and !state.leased);
    expect(owner.submit(other, &.{}).code == ros.busy and !state.leased);
    second.leased = false;
    const watching = owner.watch(other, id);
    expect((watching == ros.bad_handle or watching == ros.denied) and !state.leased and !second.leased);
    const submission = owner.submit(other, &.{});
    expect((submission.code == ros.bad_handle or submission.code == ros.denied) and submission.id == 0);
    group("PAIR_LEASE_ROLLBACK");

    inline for (comptime std.meta.tags(ros.Operation)) |operation| {
        @memset(&bytes, 42);
        const result = owner.request(operation, &.{}, &bytes);
        expect(result.data.len == 0 and result.required == null);
        if (comptime ros.supports(operation)) {
            expect(result.code == ros.bad_handle or result.code == ros.denied);
            for (bytes) |b| expect(b == 0);
        } else expect(result.code == ros.unsupported);
        expect(!state.leased);
    }
    group("ALL_33_RPC_SELECTIONS");

    expect(owner.request(.service_call, &.{}, &too_big).code == ros.too_large);
    expect(owner.publish(&too_big) == ros.too_large);
    expect(owner.poll(&too_big).code == ros.too_large);
    expect(owner.request(.service_call, bytes[0..8], bytes[4..12]).code == ros.invalid);
    group("BOUNDS_AND_OVERLAP");

    for ([_]u32{ owner.subscribe(), owner.advertiseService(), owner.subscribeParameterEvents(), owner.publish(&.{}), owner.log(&.{}), owner.poll(&bytes).code, owner.pollEvent(&bytes).code, owner.ready().code }) |code| expect(code == ros.bad_handle or code == ros.denied or code == ros.unsupported);
    expect(!state.leased);
    group("ACTUAL_NON_RPC_REFUSALS");

    var session: ros.Session(2) = .{};
    session.cells[0] = .{ .spent = true, .handle = id };
    session.cells[1] = .{ .spent = true, .handle = 7, .leased = true };
    session.used = 2;
    expect(session.open("call").code == ros.too_large);
    expect(session.close().code == ros.busy and !session.closed and session.cells[0].handle == id);
    session.cells[1].leased = false;
    const cleanup = session.close();
    expect(cleanup.code == ros.ok and session.closed);
    for (cleanup.rows) |row| expect(row.attempted and (row.code == ros.bad_handle or row.code == ros.denied));
    expect(session.cells[0].handle == 0 and session.cells[1].handle == 0);
    expect(session.close().code == ros.bad_handle and session.open("call").code == ros.bad_handle);
    group("SESSION_CLEANUP");
}
