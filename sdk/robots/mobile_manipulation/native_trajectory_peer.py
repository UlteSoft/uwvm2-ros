"""Real ROS2 native witness helpers; not a Guest permission implementation.

The operator creates an original Node/Executor with its ordinary default
endpoints. These helpers use the actual selected ActionClient and never create
a second discovery/probe client. Caller supplies an absolute monotonic deadline.
No method silently sends a stop/cancel, retries a goal, or destroys caller Node.
"""
import time
from rclpy.action import ActionClient
from control_msgs.action import FollowJointTrajectory
from std_srvs.srv import Trigger


def remaining(deadline_ns):
    if type(deadline_ns) is not int:
        raise ValueError('absolute monotonic deadline required')
    value = (deadline_ns - time.monotonic_ns()) / 1_000_000_000
    if value <= 0:
        raise TimeoutError('deadline reached; native outcome may be unknown')
    return value


def finish_future(node, executor, future, deadline_ns):
    while not future.done():
        if not node.context.ok():
            raise OSError(125, 'actual ROS context stopped')
        executor.spin_once(timeout_sec=min(0.1, remaining(deadline_ns)))
    return future.result()


class NativeTrajectoryPeer:
    def __init__(self, node, executor, action_name, maximum_feedback=256):
        if type(maximum_feedback) is not int or not 1 <= maximum_feedback <= 256:
            raise ValueError('bounded feedback storage required')
        # All local bookkeeping is reserved before the native ActionClient.
        self.feedback = [None] * maximum_feedback
        self.feedback_used, self.feedback_error = 0, 0
        self.goal_handle = self.send_future = self.result_future = None
        self.cancel_future = self.cancel_response = None
        self.native_destroy_error = None
        self.node, self.executor = node, executor
        self.client = ActionClient(node, FollowJointTrajectory, action_name)

    def _feedback(self, message):
        if self.feedback_used == len(self.feedback):
            self.feedback_error = 90
            return
        # This is the complete actual typed FeedbackMessage, including UUID
        # and all standard scalar/multi-DOF fields, not reconstructed metadata.
        self.feedback[self.feedback_used] = message
        self.feedback_used += 1

    def ready(self, deadline_ns):
        while not self.client.server_is_ready():
            if not self.node.context.ok():
                raise OSError(125, 'actual ROS context stopped')
            self.executor.spin_once(timeout_sec=min(0.1, remaining(deadline_ns)))
        remaining(deadline_ns)

    def submit(self, complete_goal, deadline_ns):
        if self.send_future is not None:
            raise OSError(17, 'this witness sends at most one goal')
        if not isinstance(complete_goal, FollowJointTrajectory.Goal):
            raise TypeError('actual original generated complete Goal required')
        self.ready(deadline_ns)
        remaining(deadline_ns)
        self.send_future = self.client.send_goal_async(complete_goal, feedback_callback=self._feedback)
        self.goal_handle = finish_future(self.node, self.executor, self.send_future, deadline_ns)
        if not self.goal_handle.accepted:
            raise OSError(22, 'real server rejected goal')
        remaining(deadline_ns)
        self.result_future = self.goal_handle.get_result_async()
        return self.goal_handle.goal_id

    def cancel(self, deadline_ns):
        if self.goal_handle is None or self.cancel_future is not None:
            raise OSError(17, 'one actual owned accepted goal and one cancel attempt required')
        remaining(deadline_ns)
        self.cancel_future = self.goal_handle.cancel_goal_async()
        self.cancel_response = finish_future(self.node, self.executor, self.cancel_future, deadline_ns)
        wanted = bytes(self.goal_handle.goal_id.uuid)
        accepted = any(bytes(item.goal_id.uuid) == wanted for item in self.cancel_response.goals_canceling)
        # Even accepted cancellation is not a physical-stop result. Continue
        # to the actual result with an independently selected finite deadline.
        return accepted

    def result(self, deadline_ns):
        if self.result_future is None:
            raise OSError(9, 'no owned result request')
        value = finish_future(self.node, self.executor, self.result_future, deadline_ns)
        if self.feedback_error:
            raise OSError(self.feedback_error, 'feedback history exhausted; no silent success')
        return value

    def close(self):
        client, self.client = self.client, None
        if client is not None:
            try:
                client.destroy()
            except BaseException as error:
                self.native_destroy_error = error
                raise


def trigger_once(node, executor, service_name, deadline_ns):
    """One independently approved real Trigger call (e.g. Stretch stop/mode).

    A true success/message is preserved as the actual driver response, not
    certified braking completion. Destruction does not cancel an unknown RPC.
    """
    client = node.create_client(Trigger, service_name)
    primary = cleanup = None
    try:
        while not client.service_is_ready():
            executor.spin_once(timeout_sec=min(0.1, remaining(deadline_ns)))
            if not node.context.ok():
                raise OSError(125, 'actual ROS context stopped')
        request = Trigger.Request()
        remaining(deadline_ns)
        future = client.call_async(request)
        return finish_future(node, executor, future, deadline_ns)
    except BaseException as error:
        primary = error
        raise
    finally:
        try:
            if not node.destroy_client(client):
                raise RuntimeError('original Node refused to destroy actual client')
        except BaseException as error:
            cleanup = error
            if primary is not None:
                primary.mobile_manipulation_cleanup = cleanup
            else:
                raise
