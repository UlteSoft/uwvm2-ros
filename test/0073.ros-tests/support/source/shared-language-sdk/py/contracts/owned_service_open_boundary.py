"""UNEXECUTED actual C bridge/allocator contract entry, counted ABI doubles only.

Build with owned_service_open_boundary_fixture.c INSTEAD of _native.c. Actual
other imports still need Runtime resolution; this is not Native ROS success.
"""
from _roswasm_native import fixture_open_boundary


def main():
    fixture_open_boundary()
    print('PYOWN_OPEN_C_PRIMARY_ONCE_CLOSE_RETAINED')
    return 0
