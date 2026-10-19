"""UNEXECUTED real-Wasm default-off entry, for either original ROS backend."""
from roswasm import Backend


def main():
    try:
        import _roswasm_owned_service
    except ModuleNotFoundError:
        pass
    else:
        raise AssertionError('default-off interpreter registered optional bridge')
    # Actual original builtin/backend/version, no Native capability opened.
    import _roswasm_native
    assert not hasattr(_roswasm_native, 'prepare_open_witness')
    assert Backend(_roswasm_native.backend).abi_version() == 1
    print('OWNED_SERVICE_DEFAULT_OFF_ORIGINAL_ABI')
    return 0
