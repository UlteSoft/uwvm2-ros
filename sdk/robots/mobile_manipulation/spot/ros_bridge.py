"""Fixed native ROS2 service bridge. No SDK/ROS entity exists for empty config.

Run natively as `python3 -m uwvm_mobile_manipulation.spot.ros_bridge` only after
the actual ament/rosidl producer is built. This is not a Wasm Python backend.
Credentials/hostname/config are native operator arguments, never request fields.
"""
import argparse
import array
import math
import time
from ..common.native_gate import NativeGate, SERVICES, read_native_json


def _rpc_seconds(deadline):
    value = (deadline - time.monotonic_ns()) / 1_000_000_000
    if value <= 0:
        raise TimeoutError('native SDK preparation deadline elapsed')
    return value


def prepare_native(gate, hostname, token_file):
    """Selected 5.0.1 setup without token-refresh/time-sync/keepalive threads.

    Use a pre-obtained native operator token. It is assigned to the actual
    SDK Robot.user_token field used by its secure-channel credentials callback;
    do not call update_user_token/authenticate, which start a refresh thread.
    Expiration fails honestly; there is no automatic refresh or retry here.
    """
    if not gate.enabled:
        raise PermissionError(13, 'empty native allowlist; no SDK construction')
    from importlib.metadata import version
    if version('bosdyn-client') != '5.0.1' or version('bosdyn-api') != '5.0.1':
        raise ValueError('actual SDK 5.0.1 required')
    from bosdyn.client import create_standard_sdk
    from bosdyn.client.common import BaseClient
    from bosdyn.client.directory import DirectoryClient
    from bosdyn.client.lease import LeaseClient, add_lease_wallet_processors
    from bosdyn.client.robot_command import RobotCommandClient
    from bosdyn.client.robot_state import RobotStateClient
    from bosdyn.client.time_sync import TimeSyncClient, TimeSyncEndpoint
    from .native_provider import SpotProvider
    secret = read_native_json(token_file)
    if (type(secret) is not dict or set(secret) != {'user_token'} or
            type(secret['user_token']) is not str or not 0 < len(secret['user_token']) <= 8192 or
            '\0' in secret['user_token'] or not hostname or '\0' in hostname):
        raise ValueError('explicit native operator hostname/token required')
    channel_outcomes = [None] * 8  # fixed selected setup creates at most five authorities
    deadline = time.monotonic_ns() + 10_000_000_000
    sdk = create_standard_sdk('uwvm-spot-v5-native-bridge')
    robot = sdk.create_robot(hostname)
    try:
        robot.user_token = secret['user_token']
        robot.max_send_message_length = robot.max_receive_message_length = 4 * 1024 * 1024
        # Exactly one genuine bounded directory RPC. Supplying its actual
        # result prevents ensure_channel from taking an unbounded fallback.
        directory = robot.ensure_client(DirectoryClient.default_service_name, options=[('grpc.enable_retries', 0)])
        services = directory.list(timeout=_rpc_seconds(deadline))
        robot.sync_with_services_list(services)
        state = lease = command = None
        command_needed = any(gate.permits_operation(name) for name in SERVICES if name.startswith('command.'))
        needed = []
        if gate.permits_operation('state'):
            needed.append(RobotStateClient.default_service_name)
        if command_needed or any(gate.permits_operation(name) for name in SERVICES if name.startswith('lease.')):
            needed.append(LeaseClient.default_service_name)
        if command_needed:
            needed.extend((RobotCommandClient.default_service_name, TimeSyncClient.default_service_name))
        if any(name not in robot.authorities_by_name for name in needed):
            raise OSError(95, 'actual native directory lacks a required selected service')
        if RobotStateClient.default_service_name in needed:
            state = robot.ensure_client(RobotStateClient.default_service_name, options=[('grpc.enable_retries', 0)])
        if LeaseClient.default_service_name in needed:
            lease = robot.ensure_client(LeaseClient.default_service_name, options=[('grpc.enable_retries', 0)])
        if command_needed:
            class DeadlineTimeSyncClient(TimeSyncClient):
                def get_time_sync_update(self, previous_round_trip, clock_identifier, **kwargs):
                    kwargs['timeout'] = _rpc_seconds(deadline)
                    return super().get_time_sync_update(previous_round_trip, clock_identifier, **kwargs)
            clock = DeadlineTimeSyncClient()
            BaseClient.update_from(clock, robot)
            clock.channel = robot.ensure_channel(TimeSyncClient.default_service_name, options=[('grpc.enable_retries', 0)])
            endpoint = TimeSyncEndpoint(clock)
            if not endpoint.establish_timesync(max_samples=25, break_on_success=True):
                raise OSError(95, 'actual native time sync not established')
            _rpc_seconds(deadline)
            command = RobotCommandClient()
            # Fixed 5.0.1 adapter: reproduce its real update_from Base+wallet
            # steps while avoiding Robot.time_sync's automatic worker start.
            BaseClient.update_from(command, robot)
            add_lease_wallet_processors(command, robot.lease_wallet)
            command._timesync_endpoint = endpoint
            command.channel = robot.ensure_channel(RobotCommandClient.default_service_name, options=[('grpc.enable_retries', 0)])
        provider = SpotProvider(state, lease, command, gate=gate,
                                maximum_history=len(gate._rows))
        return robot, provider, channel_outcomes
    except BaseException as primary:
        for index, channel in enumerate(robot.channels_by_authority.values()):
            try:
                channel.close()
            except BaseException as error:
                channel_outcomes[index] = error
        primary.mobile_manipulation_cleanup = channel_outcomes
        raise


def _proto_bytes(message):
    if message.ByteSize() > 16384:
        raise OSError(90, 'complete native protobuf exceeds selected 16384-byte bound')
    return array.array('B', message.SerializeToString(deterministic=True))


def _state_values(value):
    if not value.HasField('kinematic_state') or not value.HasField('power_state'):
        raise OSError(95, 'actual native state lacks required typed projection')
    state = value.kinematic_state
    if not state.HasField('acquisition_timestamp') or not 0 < len(state.joint_states) <= 32:
        raise OSError(95, 'actual native timestamp/joint extent absent or incompatible')
    timestamp = state.acquisition_timestamp
    if not -(1 << 63) <= timestamp.seconds < 1 << 63 or not 0 <= timestamp.nanos < 1_000_000_000:
        raise OSError(95, 'actual native timestamp extent')
    names, position, velocity, load = [], [], [], []
    for joint in state.joint_states:
        if (not joint.name or '\0' in joint.name or len(joint.name.encode('utf-8')) > 128 or
                not all(joint.HasField(field) for field in ('position', 'velocity', 'load'))):
            raise OSError(95, 'required actual native joint field missing')
        values = (joint.position.value, joint.velocity.value, joint.load.value)
        if not all(math.isfinite(item) for item in values):
            raise OSError(22, 'nonfinite native telemetry cannot enter canonical float fields')
        names.append(joint.name)
        position.append(values[0]); velocity.append(values[1]); load.append(values[2])
    return (timestamp.seconds, timestamp.nanos, names, position, velocity, load,
            value.power_state.motor_power_state, _proto_bytes(value))


def callback(provider, operation, prepared_ns):
    """Bind the actual operation statically. No request dispatch string exists."""
    def call(request, response):
        response.token, response.nonce = request.token, request.nonce
        response.error, response.unknown = 5, True
        try:
            arguments = ()
            if operation == 'command.velocity':
                if time.monotonic_ns() - prepared_ns > 60_000_000_000:
                    raise OSError(95, 'native velocity time-sync profile expired; operator restart required')
                arguments = (request.x_mps, request.y_mps, request.yaw_rps, request.duration_ns)
            elif operation == 'command.feedback':
                arguments = (request.command_token,)
            outcome = provider.invoke(SERVICES[operation], operation, request.token,
                                      nonce=request.nonce, budget_ns=request.budget_ns, arguments=arguments)
            if operation == 'state':
                values = _state_values(outcome.value)
                (response.acquisition_sec, response.acquisition_nsec, response.joint_names,
                 response.joint_position, response.joint_velocity, response.joint_load,
                 response.motor_power_state, response.native_proto) = values
            elif operation.startswith('lease.'):
                response.provider_holds_body_lease = provider._body_state == 'active'
            elif operation == 'command.feedback':
                prior = provider._command_row(request.command_token)
                value = outcome.value
                raw = _proto_bytes(value)
                feedback = value.feedback
                has_full = feedback.HasField('full_body_feedback')
                has_sync = feedback.HasField('synchronized_feedback')
                has_mobility = has_sync and feedback.synchronized_feedback.HasField('mobility_command_feedback')
                full_status = feedback.full_body_feedback.status if has_full else 0
                mobility_status = kind = substatus = 0
                has_substatus = False
                if has_mobility:
                    mobility = feedback.synchronized_feedback.mobility_command_feedback
                    mobility_status = mobility.status
                    field = mobility.WhichOneof('feedback')
                    kind = {'stand_feedback': 1, 'sit_feedback': 2, 'se2_velocity_feedback': 3,
                            'stop_feedback': 4}.get(field, 5 if field else 0)
                    if field:
                        submessage = getattr(mobility, field)
                        has_substatus = 'status' in submessage.DESCRIPTOR.fields_by_name
                        if has_substatus:
                            substatus = submessage.status
                response.native_command_id = prior.native_command_id
                response.has_full_body_feedback, response.full_body_status = has_full, full_status
                response.has_mobility_feedback, response.mobility_status = has_mobility, mobility_status
                response.feedback_kind = kind
                response.has_subcommand_status, response.subcommand_status = has_substatus, substatus
                response.native_proto = raw
            else:
                response.native_accepted = True
                response.native_command_id = outcome.native_command_id
            response.error, response.unknown = 0, False
        except Exception as error:
            code = 110 if isinstance(error, TimeoutError) else 22 if isinstance(error, ValueError) else getattr(error, 'errno', 5)
            response.error = code if type(code) is int and 0 < code < 1 << 32 else 5
            # Conservative unknown if an SDK call OR subsequent output handling
            # may have happened. Never use this error number as a retry permit.
            response.unknown = True
        return response
    return call


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--native-allowlist')
    parser.add_argument('--robot-hostname')
    parser.add_argument('--native-token-file')
    args = parser.parse_args()
    configuration = read_native_json(args.native_allowlist) if args.native_allowlist else {}
    gate = NativeGate(configuration)
    # This branch precedes importing bosdyn/rclpy, creating SDK/Robot/clients,
    # loading credentials, opening a ROS entity or making any SDK RPC.
    if not gate.enabled:
        raise PermissionError(13, 'default native allowlist is empty')
    if not args.robot_hostname or not args.native_token_file:
        raise ValueError('native operator hostname/token file required')
    robot = provider = node = None
    services, outcomes = [None] * len(SERVICES), [None] * (len(SERVICES) + 3)
    channel_outcomes = None
    primary = None
    import rclpy
    from rclpy.node import Node
    from uwvm_spot_bridge.srv import State, Lease, Command, Velocity, Feedback
    try:
        robot, provider, channel_outcomes = prepare_native(gate, args.robot_hostname, args.native_token_file)
        prepared_ns = time.monotonic_ns()
        rclpy.init(args=[])
        node = Node('uwvm_spot_v5_native_bridge')
        for index, (operation, name) in enumerate(SERVICES.items()):
            if gate.permits_operation(operation):
                kind = (State if operation == 'state' else Lease if operation.startswith('lease.') else
                        Velocity if operation == 'command.velocity' else Feedback if operation == 'command.feedback' else Command)
                services[index] = node.create_service(kind, name, callback(provider, operation, prepared_ns))
        rclpy.spin(node)  # original default Node/executor behavior, no quiet options
    except BaseException as error:
        primary = error
        raise
    finally:
        if provider is not None:
            try:
                outcomes[-3] = provider.cleanup_native()
            except BaseException as error:
                outcomes[-3] = error
        if node is not None:
            for index in range(len(services) - 1, -1, -1):
                if services[index] is not None:
                    try:
                        outcomes[index] = node.destroy_service(services[index])
                    except BaseException as error:
                        outcomes[index] = error
            try:
                node.destroy_node()
            except BaseException as error:
                outcomes[-2] = error
        try:
            if rclpy.ok():
                rclpy.shutdown()
        except BaseException as error:
            outcomes[-1] = error
        if robot is not None:
            # No native refresh/time-sync/keepalive worker was started. Retire
            # every truly owned channel independently despite earlier errors.
            for index, channel in enumerate(robot.channels_by_authority.values()):
                try:
                    channel.close()
                except BaseException as error:
                    channel_outcomes[index] = error
        channel_errors = channel_outcomes if channel_outcomes is not None else ()
        if primary is not None:
            primary.mobile_manipulation_cleanup = (outcomes, channel_errors)
        elif (any(isinstance(item, BaseException) or item is False for item in (*outcomes, *channel_errors)) or
              (provider is not None and any(row.error is not None for row in provider._cleanup_rows))):
            raise RuntimeError('native cleanup failed', outcomes, channel_errors)


if __name__ == '__main__':
    main()
