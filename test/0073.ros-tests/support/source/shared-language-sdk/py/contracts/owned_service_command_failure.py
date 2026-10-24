"""UNEXECUTED local carrier states only; not actual main/Native qualification."""
from types import SimpleNamespace
from roswasm.owned_service.applications import CommandFailure


class FalsyError(RuntimeError):
    def __bool__(self):
        return False


def main():
    failure = CommandFailure()
    first = SimpleNamespace(error=None, delivery=None)
    failure.record('submit', 0, 13, first)
    failure.record('available', 1, 95, SimpleNamespace(error=None, delivery=None))
    assert failure.failed and failure.status == 13 and failure.result is first
    idle = CommandFailure()
    idle.record('take', 0, 11, SimpleNamespace(error=None, delivery=None))
    assert not idle.failed
    idle.record('submit', 0, 11, first)
    assert idle.failed and idle.status == 11
    control = CommandFailure()
    item = SimpleNamespace(control=True, error=0)
    result = SimpleNamespace(error=None, delivery=item)
    control.record('take', 0, 0, result)
    assert control.failed and control.status == 125 and control.result is result
    raised = FalsyError('exact retained object')
    observed = CommandFailure()
    observed.record('take', 0, 11, SimpleNamespace(error=raised, delivery=None))
    assert observed.failed and observed.exception is raised
    return 0
