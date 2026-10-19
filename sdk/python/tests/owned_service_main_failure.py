"""UNEXECUTED genuine frozen driver: retain its actual failure1, not wrapper0.

Caller supplies real plans/commands on stdin and argv kind/expected-operation/
expected-errno. Keeper must require marker AND exact exit1 AND retained actual
operation/cleanup evidence. A generic pre-entry denial/exception cannot pass.
Use the real selected builtin+Native peer, never a model transport here.
"""
import sys
from roswasm.owned_service import drive


def main():
    if len(sys.argv) != 4 or sys.argv[1] not in ('type_query', 'parameters'):
        raise ValueError('kind expected-operation expected-errno required')
    expected = int(sys.argv[3])
    if not 1 <= expected <= 4294967295:
        raise ValueError('actual expected nonzero errno required')
    code = drive.main(sys.argv[1])
    application = drive.application
    if application is None:
        raise AssertionError('actual application was never retained')
    failure = application.command_failure
    if (code != 1 or not failure.failed or failure.operation != sys.argv[2]
            or failure.status != expected or failure.result is None):
        raise AssertionError('actual failed command/exit outcome absent')
    if not application.closed or application.session.active:
        raise AssertionError('actual independent cleanup/shutdown was not completed')
    print('PYOWN_ACTUAL_MAIN_FAILURE_RETAINED', failure.operation, failure.status,
          failure.cleanup_status, failure.cleanup_error is not None,
          failure.shutdown_error is not None)
    # Preserve the actual driver's nonzero process result. Only the external
    # keeper may accept it together with the exact complete evidence marker.
    return code
