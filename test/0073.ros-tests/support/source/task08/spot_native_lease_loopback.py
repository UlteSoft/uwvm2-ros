"""Actual Spot 5.0.1 lease/session transport on a bounded loopback peer.

Real generated Lease/RobotCommand/TimeSync services and genuine SDK clients,
wallet processors and time-sync endpoint are used. No client method is mocked.
The peer is a test service, not a robot. No stand/velocity/power/estop operation
is sent. A complete Stop request is accepted only by this 127.0.0.1 peer.
"""

import argparse
from concurrent.futures import ThreadPoolExecutor
import hashlib
import json
from pathlib import Path
import sys
import threading
import time


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--dependencies", type=Path, required=True)
    parser.add_argument("--source", type=Path, required=True)
    parser.add_argument("--output", type=Path, required=True)
    args = parser.parse_args()
    sys.path[:0] = [str(args.dependencies), str(args.source / "sdk/origin/robots")]
    import grpc
    from bosdyn.api import header_pb2, lease_pb2, lease_service_pb2_grpc
    from bosdyn.api import robot_command_pb2, robot_command_service_pb2_grpc
    from bosdyn.api import time_sync_pb2, time_sync_service_pb2_grpc
    from bosdyn.client.exceptions import TimedOutError
    from bosdyn.client.lease import (
        Lease,
        LeaseClient,
        LeaseWallet,
        NoSuchLease,
        add_lease_wallet_processors,
        test_active_lease,
    )
    from bosdyn.client.processors import AddRequestHeader
    from bosdyn.client.robot_command import RobotCommandClient
    from bosdyn.client.time_sync import TimeSyncClient, TimeSyncEndpoint
    from mobile_manipulation.common.native_gate import NativeGate, SERVICES
    from mobile_manipulation.spot.native_provider import SpotProvider
    from mobile_manipulation.spot.native_sessions import SpotNativeSessions

    rows, captures, exchanges = [], [], []
    frame_root = args.output.parent / "lease-frames"
    frame_root.mkdir()
    lock = threading.Lock()

    class State:
        active = None
        generation = 0
        delay_acquire = False
        delay_return = False
        take_calls = 0

    state = State()

    def finish(name, request, response):
        response.header.request_header.CopyFrom(request.header)
        response.header.error.code = header_pb2.CommonError.CODE_OK
        now = time.time_ns()
        for stamp in (
            response.header.request_received_timestamp,
            response.header.response_timestamp,
        ):
            stamp.seconds = now // 1_000_000_000
            stamp.nanos = now % 1_000_000_000
        request_body = request.SerializeToString(deterministic=True)
        response_body = response.SerializeToString(deterministic=True)
        with lock:
            index = len(exchanges) + 1
            for direction, body in (
                ("request", request_body),
                ("response", response_body),
            ):
                path = frame_root / ("%04d.%s.%s.pb" % (index, name, direction))
                with path.open("xb") as output:
                    output.write(body)
                captures.append(
                    dict(
                        path=str(path),
                        method=name,
                        direction=direction,
                        bytes=len(body),
                        sha256=hashlib.sha256(body).hexdigest(),
                    )
                )
            exchanges.append((name, request_body, response_body))
        return response

    def withhold_until_deadline(context):
        # The peer commits first. The real transport deadline expires before
        # it can deliver the response; a native caller cannot infer rollback.
        end = time.monotonic() + 0.5
        while context.is_active() and time.monotonic() < end:
            time.sleep(0.002)

    def check_lease(requested):
        assert state.active is not None, "peer requires an actually acquired lease"
        result, incoming = test_active_lease(requested, state.active)
        assert result.status == lease_pb2.LeaseUseResult.STATUS_OK
        state.active = Lease(result.latest_known_lease)
        return result

    class Leases(lease_service_pb2_grpc.LeaseServiceServicer):
        def AcquireLease(self, request, context):
            assert request.resource == "body"
            result = lease_pb2.AcquireLeaseResponse()
            with lock:
                assert (
                    state.active is None
                ), "unexpected duplicate acquisition reached peer"
                state.generation += 1
                result.lease.resource = "body"
                result.lease.epoch = "test-only-loopback-epoch"
                result.lease.sequence.append(state.generation)
                result.lease.client_names.append(request.header.client_name)
                result.status = lease_pb2.AcquireLeaseResponse.STATUS_OK
                state.active = Lease(result.lease)
                delay = state.delay_acquire
                state.delay_acquire = False
            result = finish("AcquireLease", request, result)
            if delay:
                withhold_until_deadline(context)
            return result

        def RetainLease(self, request, context):
            result = lease_pb2.RetainLeaseResponse()
            with lock:
                result.lease_use_result.CopyFrom(check_lease(request.lease))
            return finish("RetainLease", request, result)

        def ReturnLease(self, request, context):
            result = lease_pb2.ReturnLeaseResponse()
            with lock:
                check_lease(request.lease)
                state.active = None
                result.status = lease_pb2.ReturnLeaseResponse.STATUS_OK
                delay = state.delay_return
                state.delay_return = False
            result = finish("ReturnLease", request, result)
            if delay:
                withhold_until_deadline(context)
            return result

        def TakeLease(self, request, context):
            state.take_calls += 1
            context.abort(
                grpc.StatusCode.PERMISSION_DENIED,
                "force acquisition is never authorized",
            )

    class Commands(robot_command_service_pb2_grpc.RobotCommandServiceServicer):
        def RobotCommand(self, request, context):
            assert request.clock_identifier == "test-only-loopback-clock"
            assert request.command.full_body_command.HasField("stop_request")
            assert not request.command.HasField("synchronized_command")
            result = robot_command_pb2.RobotCommandResponse()
            with lock:
                result.lease_use_result.CopyFrom(check_lease(request.lease))
            result.status = robot_command_pb2.RobotCommandResponse.STATUS_OK
            result.robot_command_id = 0xF1234567
            return finish("RobotCommand.Stop", request, result)

    class Clock(time_sync_service_pb2_grpc.TimeSyncServiceServicer):
        def TimeSyncUpdate(self, request, context):
            result = time_sync_pb2.TimeSyncUpdateResponse()
            result.clock_identifier = "test-only-loopback-clock"
            result.state.status = time_sync_pb2.TimeSyncState.STATUS_OK
            result.state.best_estimate.round_trip_time.nanos = 1
            return finish("TimeSyncUpdate", request, result)

    pool = ThreadPoolExecutor(max_workers=2)
    server = grpc.server(
        pool,
        options=(
            ("grpc.max_receive_message_length", 65536),
            ("grpc.max_send_message_length", 65536),
        ),
    )
    lease_service_pb2_grpc.add_LeaseServiceServicer_to_server(Leases(), server)
    robot_command_service_pb2_grpc.add_RobotCommandServiceServicer_to_server(
        Commands(), server
    )
    time_sync_service_pb2_grpc.add_TimeSyncServiceServicer_to_server(Clock(), server)
    port = server.add_insecure_port("127.0.0.1:0")
    assert port > 0
    server.start()
    channel = grpc.insecure_channel(
        "127.0.0.1:" + str(port),
        options=(
            ("grpc.max_receive_message_length", 65536),
            ("grpc.max_send_message_length", 65536),
            ("grpc.enable_retries", 0),
        ),
    )
    try:

        class BoundedClock(TimeSyncClient):
            def get_time_sync_update(
                self, previous_round_trip, clock_identifier, **kwargs
            ):
                kwargs["timeout"] = 1.0
                return super().get_time_sync_update(
                    previous_round_trip, clock_identifier, **kwargs
                )

        clock = BoundedClock()
        clock.channel = channel
        clock.request_processors.append(AddRequestHeader(lambda: "test-clock"))
        endpoint = TimeSyncEndpoint(clock)
        assert endpoint.establish_timesync(max_samples=2, break_on_success=True)
        assert endpoint.has_established_time_sync
        rows.append(
            dict(
                name="genuine finite TimeSync endpoint without background worker",
                status="PASS",
            )
        )

        def provider(name):
            wallet = LeaseWallet()
            wallet.client_name = name
            lease = LeaseClient(wallet)
            lease.channel = channel
            command = RobotCommandClient()
            command.channel = channel
            command.lease_wallet = wallet
            command._timesync_endpoint = endpoint
            add_lease_wallet_processors(command, wallet)
            for client in (lease, command):
                client.request_processors.append(AddRequestHeader(lambda: name))
            gate = NativeGate(
                {
                    "operations": {
                        operation: {"maximum_rpc_ns": 2_000_000_000}
                        for operation in (
                            "lease.acquire",
                            "lease.retain",
                            "lease.return",
                            "command.stop",
                        )
                    }
                }
            )
            return SpotProvider(
                None, lease, command, gate=gate, maximum_rpc_ns=2_000_000_000
            )

        def refused(name, kind, call):
            before = len(exchanges)
            try:
                call()
            except kind:
                pass
            else:
                raise AssertionError(name + " unexpectedly returned")
            assert len(exchanges) == before
            rows.append(dict(name=name, status="PASS", native_sdk_rpcs=0))

        def missing_wallet(native):
            try:
                native._wallet.get_lease("body")
            except NoSuchLease:
                return
            raise AssertionError("actual SDK wallet still has body lease")

        def attach_pair(lifetime=30_000_000_000):
            operator, owner, other, robot = object(), object(), object(), object()
            sessions = SpotNativeSessions(
                cleanup_operator=operator,
                cleanup_deadline_ns=time.monotonic_ns() + 30_000_000_000,
            )
            native, foreign = provider("one"), provider("two")
            binding = sessions.attach_native_only(
                owner, robot, native, expires_at_ns=time.monotonic_ns() + lifetime
            )
            other_binding = sessions.attach_native_only(
                other,
                robot,
                foreign,
                expires_at_ns=time.monotonic_ns() + 30_000_000_000,
            )
            return sessions, operator, owner, other, native, binding, other_binding

        def invoke(
            sessions, binding, owner, operation, correlation, budget=1_000_000_000
        ):
            return sessions.invoke_from_authenticated_host(
                binding,
                owner,
                SERVICES[operation],
                operation,
                correlation,
                nonce=correlation + 1000,
                budget_ns=budget,
            )

        sessions, operator, owner, other, native, binding, other_binding = attach_pair(
            1_500_000_000
        )
        acquired = invoke(sessions, binding, owner, "lease.acquire", 1)
        assert (
            acquired.state == "returned"
            and binding.body_exclusion
            and native._body_state == "active"
        )
        message = lease_pb2.AcquireLeaseResponse.FromString(exchanges[-1][2])
        assert acquired.value.lease_proto.SerializeToString(
            deterministic=True
        ) == message.lease.SerializeToString(deterministic=True)
        rows.append(
            dict(
                name="actual acquire retains original Lease and SDK wallet",
                status="PASS",
                native_sdk_rpcs=1,
            )
        )
        refused(
            "other owner cannot borrow actual active body lease",
            PermissionError,
            lambda: invoke(sessions, other_binding, other, "lease.acquire", 2),
        )
        expected = native._wallet.get_lease("body").lease_proto.SerializeToString(
            deterministic=True
        )
        retained = invoke(sessions, binding, owner, "lease.retain", 3)
        assert retained.value.SerializeToString(deterministic=True) == exchanges[-1][2]
        assert (
            lease_pb2.RetainLeaseRequest.FromString(
                exchanges[-1][1]
            ).lease.SerializeToString(deterministic=True)
            == expected
        )
        rows.append(
            dict(
                name="actual retain reads current genuine wallet lease and full response",
                status="PASS",
                native_sdk_rpcs=1,
            )
        )
        time.sleep(
            max(0, (binding.expires_at_ns - time.monotonic_ns()) / 1_000_000_000) + 0.02
        )
        refused(
            "expired business binding retains body exclusion",
            PermissionError,
            lambda: invoke(sessions, binding, owner, "lease.retain", 4),
        )
        assert binding.body_exclusion
        refused(
            "expired owner is not the independent native cleanup operator",
            PermissionError,
            lambda: sessions.cleanup_from_native_operator(binding, owner),
        )
        before = len(exchanges)
        cleanup = sessions.cleanup_from_native_operator(binding, operator)
        assert len(exchanges) == before + 2 and all(
            row.state == "returned" for row in cleanup
        )
        assert cleanup[0].native_command_id == 0xF1234567
        assert (
            native._body_state == "returned"
            and not binding.body_exclusion
            and binding.retired
        )
        assert state.active is None
        missing_wallet(native)
        rows.append(
            dict(
                name="independent native cleanup sends actual Stop then ReturnLease after expiry",
                status="PASS",
                native_sdk_rpcs=2,
            )
        )
        refused(
            "native cleanup slots are never replayed",
            PermissionError,
            lambda: sessions.cleanup_from_native_operator(binding, operator),
        )
        invoke(sessions, other_binding, other, "lease.acquire", 5)
        returned = invoke(sessions, other_binding, other, "lease.return", 6)
        assert returned.value.SerializeToString(deterministic=True) == exchanges[-1][2]
        assert not other_binding.body_exclusion and state.active is None
        rows.append(
            dict(
                name="next owner acquires only after actual returned lease",
                status="PASS",
                native_sdk_rpcs=2,
            )
        )

        sessions, operator, owner, other, native, binding, other_binding = attach_pair()
        invoke(sessions, binding, owner, "lease.acquire", 11)
        state.delay_return = True
        try:
            invoke(sessions, binding, owner, "lease.return", 12, 100_000_000)
        except TimedOutError:
            pass
        else:
            raise AssertionError(
                "withheld original ReturnLease response did not time out"
            )
        assert (
            state.active is None
            and native._body_state == "return-unknown"
            and binding.body_exclusion
        )
        assert native.diagnostics()[-1].state == "unknown"
        missing_wallet(native)  # Original LeaseClient removes it before the RPC.
        rows.append(
            dict(
                name="real ReturnLease timeout retains exclusion despite peer commit and empty wallet",
                status="PASS",
                native_sdk_rpcs=2,
            )
        )
        refused(
            "unknown return cannot be replayed",
            ValueError,
            lambda: invoke(sessions, binding, owner, "lease.return", 12),
        )
        refused(
            "second owner remains excluded after unknown return",
            PermissionError,
            lambda: invoke(sessions, other_binding, other, "lease.acquire", 13),
        )
        before = len(exchanges)
        cleanup = sessions.cleanup_from_native_operator(binding, operator)
        assert len(exchanges) == before and binding.body_exclusion
        assert all(row.state == "no-known-active-body-lease" for row in cleanup)
        rows.append(
            dict(
                name="native cleanup does not replay uncertain ReturnLease",
                status="PASS",
                native_sdk_rpcs=0,
            )
        )

        sessions, operator, owner, other, native, binding, other_binding = attach_pair()
        state.delay_acquire = True
        try:
            invoke(sessions, binding, owner, "lease.acquire", 21, 100_000_000)
        except TimedOutError:
            pass
        else:
            raise AssertionError(
                "withheld original AcquireLease response did not time out"
            )
        assert (
            state.active is not None
            and native._body_state == "acquire-unknown"
            and binding.body_exclusion
        )
        assert native.diagnostics()[-1].state == "unknown"
        missing_wallet(native)
        rows.append(
            dict(
                name="real AcquireLease timeout retains unknown committed acquisition",
                status="PASS",
                native_sdk_rpcs=1,
            )
        )
        refused(
            "unknown acquisition is never reacquired",
            OSError,
            lambda: invoke(sessions, binding, owner, "lease.acquire", 22),
        )
        refused(
            "second owner cannot bypass unknown acquisition",
            PermissionError,
            lambda: invoke(sessions, other_binding, other, "lease.acquire", 23),
        )
        before = len(exchanges)
        cleanup = sessions.cleanup_from_native_operator(binding, operator)
        assert len(exchanges) == before and binding.body_exclusion and binding.retired
        assert all(row.state == "no-known-active-body-lease" for row in cleanup)
        assert state.take_calls == 0
        rows.append(
            dict(
                name="retirement/cleanup retains unknown ownership without TakeLease",
                status="PASS",
                native_sdk_rpcs=0,
            )
        )
        report = dict(
            schema="uwvm-task08-spot-native-lease-loopback-v1",
            cases=rows,
            passed=len(rows),
            denominator=len(rows),
            native_sdk_rpcs=len(exchanges),
            methods=[entry[0] for entry in exchanges],
            frames=captures,
            default_authority=[],
            actual_sdk_wallet_processors=True,
            actual_sdk_time_sync_endpoint=True,
            robot_cases=0,
            ros_cases=0,
            gateway_cases=0,
            wasm_guest_cases=0,
            physical_motion_commands=0,
            take_lease_calls=state.take_calls,
            retained_test_peer_lease=state.active.lease_proto.SerializeToString(
                deterministic=True
            ).hex(),
        )
        args.output.write_text(json.dumps(report, indent=2, sort_keys=True) + "\n")
        print(
            json.dumps(
                dict(
                    passed=len(rows),
                    denominator=len(rows),
                    native_sdk_rpcs=len(exchanges),
                )
            )
        )
    finally:
        channel.close()
        assert server.stop(0).wait(timeout=2), "actual gRPC server did not stop"
        pool.shutdown(wait=True, cancel_futures=True)


if __name__ == "__main__":
    main()
