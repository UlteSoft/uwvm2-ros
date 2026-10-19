"""Real CPython/WASI lesson: resolve ROS imports, then observe no ROS authority.

Build a backend-specific interpreter with _roswasm_native. Freeze this module
and the complete SDK/stdlib import closure into it, or grant only explicit
read-only SDK/stdlib paths. Run without ROS grants. Missing imports, missing
Python modules and exceptions are failures, not substitutes for errno13.
"""
from roswasm import Backend
import _roswasm_native


def main():
    backend = Backend(_roswasm_native.backend)
    if backend.abi_version() != 1:
        raise RuntimeError('ROS ABI version mismatch')
    result = backend.open('ros-sdk-denied')
    if result.error != 13 or result.value is not None:
        raise RuntimeError(f'expected permission denied13 without a handle: {result}')
    print('PASS ROSWASM_PYTHON_DEFAULT_DENIED13')


if __name__ == '__main__':
    main()
