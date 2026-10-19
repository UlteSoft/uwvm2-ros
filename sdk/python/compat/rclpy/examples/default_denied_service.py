"""Fresh real-target empty-Host-policy entry; not a local model result."""
import rclpy
from rclpy.context import AliasBinding
from example_interfaces.srv import AddTwoInts
from roswasm import RosError


def main():
    context = rclpy.get_default_context()
    context.configure({
        ('service_advertise', '/add_two_ints', AddTwoInts._ROSWASM_TYPE): AliasBinding('denied-advertise', 10, 24),
        ('service_respond', '/add_two_ints', AddTwoInts._ROSWASM_TYPE): AliasBinding('denied-respond', 10, 8),
    }, prepare_callback_logging=False)
    primary = None
    try:
        rclpy.init()
        node = rclpy.create_node('denied_service')
        try:
            node.create_service(AddTwoInts, '/add_two_ints', lambda request, response: response)
        except RosError as error:
            if error.error != 13:
                raise
            frame = context.session.last_factory_cleanup
            if frame is None or frame.primary is not error or frame.attempted != 0 or node.services or node._owners:
                raise RuntimeError('unexpected failed-open ownership state')
        else:
            raise RuntimeError('actual Host unexpectedly granted service')
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
    print('PASS RCLPY_REAL_SERVICE_IMPORTS_HOST_DENIED13_REQUIRES_ACTUAL_RUNTIME_RECEIPT')


if __name__ == '__main__':
    main()
