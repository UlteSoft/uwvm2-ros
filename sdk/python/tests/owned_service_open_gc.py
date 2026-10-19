"""UNEXECUTED actual C/Python traceback/GC regression, selected C ABI doubles.

Freeze into a fresh matching owned1/backend2 target with the actual separate
owned_service_open_boundary_fixture.c replacing _native.c. No fake transport,
exception, manually edited traceback, or model GC collector is used here.
This is a CPython/C lifecycle contract, not Native ROS qualification.
"""
import gc
import _roswasm_native as native
from roswasm.raw import Backend


def retained_count(witness_type):
    # Observation allocates only after the C allocator has been restored. Do not
    # retain get_objects()'s instance list across the explicit collection below.
    return sum(1 for value in gc.get_objects() if type(value) is witness_type)


def make_actual_cycle():
    backend = Backend(2)
    retirement = [False]
    capture = backend.prepare_owned_open('explicit-fixture-alias', retirement)
    try:
        native.fixture_open_traceback(capture.token)
    except MemoryError as primary:
        # Actual C already rooted this SAME object. The propagated traceback
        # naturally owns this frame and its capture -> token reference.
        capture.primary = primary
        assert primary.__traceback__ is not None
        assert retirement[0] is True
        assert native.open_witness_primary(capture.token) is primary
        referents = gc.get_referents(capture.token)
        assert any(value is capture.data for value in referents)
        assert any(value is primary for value in referents)
        assert any(value is retirement for value in referents)
        facts = capture.snapshot()
        assert facts['full_handle'] == 0xfedcba9800000007
        assert facts['entered'] and facts['returned'] and facts['code'] == 0
        assert facts['cleanup_attempted'] and facts['cleanup_returned']
        assert facts['cleanup_code'] == 90 and facts['C_primary'] is primary
        return capture, primary
    raise AssertionError('actual post-open MemoryError was not reached')


def main():
    if gc.get_debug() & gc.DEBUG_SAVEALL:
        raise ValueError('fresh target without DEBUG_SAVEALL is required')
    probe = native.prepare_open_witness(b'probe', [False])
    witness_type = type(probe)
    assert gc.is_tracked(probe)
    try:
        witness_type()
    except TypeError:
        pass
    else:
        raise AssertionError('opaque witness permits arbitrary construction')
    try:
        object.__new__(witness_type)
    except TypeError:
        pass
    else:
        raise AssertionError('object.__new__ bypasses opaque witness construction')
    try:
        type('ForgedWitness', (witness_type,), {})
    except TypeError:
        pass
    else:
        raise AssertionError('opaque witness permits subclass construction')
    del probe
    gc.collect()
    baseline = retained_count(witness_type)
    assert baseline == 0
    for _ in range(3):
        before = native.fixture_open_counts()
        capture, primary = make_actual_cycle()
        token = capture.token
        traceback = primary.__traceback__
        assert gc.is_tracked(token) and retained_count(witness_type) == baseline + 1
        gc.collect()
        # While externally reachable, GC must not discard/alter diagnostics.
        assert native.open_witness_primary(token) is primary
        assert primary.__traceback__ is traceback
        capture.cleanup()
        after = native.fixture_open_counts()
        assert after == (before[0] + 1, before[1] + 1, before[2])
        # Drop ALL test roots, including the explicit traceback observation.
        # No traceback.clear_frames(), exception mutation or manual tp_clear.
        del token, traceback, capture, primary
        gc.collect()
        assert retained_count(witness_type) == baseline
        assert native.fixture_open_counts() == after  # GC/destructor did no ROS close.
    print('PYOWN_ACTUAL_C_TRACEBACK_GC_COLLECTED_NO_REPLAY')
    return 0
