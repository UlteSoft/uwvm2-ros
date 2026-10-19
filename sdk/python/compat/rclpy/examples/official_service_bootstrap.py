"""Explicit source-only deployment bootstrap; original main bytes stay intact.

Every alias here is a local selection, not a Host grant. Actual native identity,
QoS, service graph and finite Host quotas must be independently qualified.
No import of this file is authorized by its source package.
"""
import sys
import rclpy
from rclpy.context import AliasBinding
from example_interfaces.srv import AddTwoInts


class Credits:
    def __init__(self, stream, maximum=64):
        self.stream, self.remaining = stream, maximum

    def __call__(self):
        if self.remaining == 0:
            return False
        self.remaining -= 1
        return self.stream.read(2) == b'R\n'


def main():
    context = rclpy.get_default_context()
    context.configure({
        ('service_advertise', '/add_two_ints', AddTwoInts._ROSWASM_TYPE): AliasBinding('service-in', 10, 24),
        ('service_respond', '/add_two_ints', AddTwoInts._ROSWASM_TYPE): AliasBinding('service-out', 10, 8),
        ('log_write', '/minimal_service', ''): AliasBinding('service-log'),
    }, idle_wait=Credits(sys.stdin.buffer), maximum_owners=32,
       maximum_credits=64, prepare_callback_logging=True)
    # These two package bodies are byte-exact copies of the admitted official
    # source. The real matching producer freezes them by their actual module
    # names; no target filesystem preopen/dynamic source loading is needed.
    from examples_rclpy_minimal_service import service_member_function as original
    primary = None
    try:
        original.main()
    except BaseException as error:
        primary = error
        raise
    finally:
        try:
            context.try_shutdown()
        except BaseException as cleanup:
            if primary is None:
                raise
            try:
                primary.roswasm_cleanup = cleanup
            except BaseException:
                pass


if __name__ == '__main__':
    main()
