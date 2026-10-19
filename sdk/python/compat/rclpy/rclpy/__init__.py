"""Incremental rclpy source migration over the ROS Wasm capability SDK.

The package preserves implemented public spellings and callback shapes. It does
not claim full distro/API conformance. Native identity, implicit parameter
services, wait-set blocking, callback groups, remapping and remaining message
types require further implementation and actual upstream acceptance tests.
"""
from .context import Context, AliasBinding
from roswasm import Executor, Result, RosError

_default_context = None


def get_default_context(*, shutting_down=False):
    global _default_context
    if _default_context is None:
        _default_context = Context()
    context = _default_context
    if shutting_down:
        _default_context = None
    return context


class InitContextManager:
    """Support modern `with rclpy.init(...):` local lifecycle spelling."""
    def __init__(self, context):
        self.context = context

    def __enter__(self):
        return self

    def __exit__(self, kind, error, trace):
        try:
            try_shutdown(context=self.context)
        except BaseException as cleanup:
            if error is None:
                raise
            try:
                error.roswasm_cleanup = cleanup
            except BaseException:
                pass  # Context's reserved cleanup report retains the failure.
        return False


def init(*, args=None, context=None, domain_id=None, signal_handler_options=None):
    if signal_handler_options is not None:
        raise NotImplementedError('native signal handler installation is not available in this guest profile')
    context = get_default_context() if context is None else context
    manager = InitContextManager(context)
    context.init(args, domain_id=domain_id)
    return manager


def create_node(node_name, **kwargs):
    from .node import Node
    return Node(node_name, **kwargs)


def ok(*, context=None):
    return (get_default_context() if context is None else context).ok()


def shutdown(*, context=None, uninstall_handlers=None):
    if uninstall_handlers not in (None, False):
        raise NotImplementedError('guest native signal handlers are not installed')
    selected = get_default_context() if context is None else context
    try:
        selected.shutdown()
    finally:
        global _default_context
        if selected is _default_context and not selected.ok():
            _default_context = None


def try_shutdown(*, context=None, uninstall_handlers=None):
    if uninstall_handlers not in (None, False):
        raise NotImplementedError('guest native signal handlers are not installed')
    selected = get_default_context() if context is None else context
    try:
        selected.try_shutdown()
    finally:
        global _default_context
        if selected is _default_context and not selected.ok():
            _default_context = None


def spin_once(node, *, executor=None, timeout_sec=None):
    """One nonblocking pass; nonzero blocking time requires a future wait-set ABI.

    Infinite blocking semantics cannot be fabricated using ungranted clocks or
    a busy-loop. The explicit deployment idle driver is used by spin below.
    """
    if timeout_sec not in (0, 0.0):
        raise NotImplementedError('positive/negative timeout waits require native wait-set support')
    if executor is not None:
        raise NotImplementedError('rclpy executor object compatibility is not implemented yet')
    node._live()
    result = node._executor.spin_once()
    if result.error not in (0, 11):
        raise RosError(result)


def spin(node, executor=None):
    """Drive original callbacks with an explicitly configured idle driver.

    The driver must block or return False to end a supervised run. It is not a
    native ROS wait-set and its tests are recorded as a migration profile.
    """
    if executor is not None:
        raise NotImplementedError('rclpy executor object compatibility is not implemented yet')
    if node.context.idle_wait is None:
        raise NotImplementedError('configure an explicit idle driver; native blocking spin is still pending')
    while node.context.ok() and not node._destroyed:
        spin_once(node, timeout_sec=0)
        if not node.context.next_credit():
            break


def spin_until_future_complete(node, future, executor=None, timeout_sec=None):
    """Drive a real admitted Future with finite explicit credits, never a fake wait.

    Zero performs one nonblocking pass. None uses the finite selected Driver.
    Native positive-timeout, Executor/Task and availability semantics remain
    unavailable; no ungranted time, thread or graph-derived readiness is used.
    """
    from .task import Future
    if executor is not None or timeout_sec not in (None, 0, 0.0):
        raise NotImplementedError('native timed waits/Executor tasks are not implemented')
    if not isinstance(future, Future) or future._context is not node.context:
        raise TypeError('Future from this exact client Context required')
    node._live()
    if node.context._dispatching:
        raise RosError(Result(17))
    if timeout_sec in (0, 0.0):
        if future._pending():
            spin_once(node, timeout_sec=0)
        return
    if node.context.idle_wait is None:
        raise NotImplementedError('explicit finite Driver required before polling')
    while node.context.ok() and not node._destroyed and future._pending():
        if not node.context.next_credit():
            break
        spin_once(node, timeout_sec=0)
