"""UNEXECUTED fixture entry; counted C doubles are not Native ROS proof."""
import _roswasm_owned_service as native


def main():
    for index in (4,8):
        for code in (0,11,90,110,0xffffffff):
            assert native.fixture_run_boundary(index,code) is None
    print('OWNED_SERVICE_ACTUAL_C_ALLOCATOR_BOUNDARY')
    return 0
