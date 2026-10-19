"""Shared bounded service factories/retirement; no native identity fabrication."""
from roswasm import Result, RosError
from roswasm.client import _factory_failure
from ._dispatch import DispatchLease


def codecs_for(srv_type):
    from example_interfaces.srv import AddTwoInts
    from std_srvs.srv import Trigger, SetBool, Empty
    if srv_type is Empty:
        raise NotImplementedError('Empty fields/codec exist; actual Runtime/Gateway Empty service registration is still required')
    if srv_type not in (AddTwoInts, Trigger, SetBool):
        raise NotImplementedError('complete registered service type required')
    return srv_type.Request, srv_type.Response


def distinct(*bindings):
    aliases = tuple(binding.alias for binding in bindings)
    if (any(not isinstance(alias, str) or not alias or '\0' in alias for alias in aliases)
            or len(set(aliases)) != len(aliases)):
        raise ValueError('nonempty distinct explicit service role aliases required')
    if any(len(alias.encode('utf-8', 'strict')) > 255 for alias in aliases):
        raise ValueError('bounded UTF8 service role alias required')


class RecordCodec:
    def __init__(self, record):
        self.record = record

    def encode(self, value):
        return self.record._encode(value)

    def decode(self, data):
        return self.record._decode(data)


def close_resource(resource):
    node = resource.node
    node._factory()
    owners = resource._dispatch_owners
    if not any(owner.handle for owner in owners):
        # Explicit core-owner retirement outside dispatch is permitted. The
        # facade may still own admitted local Futures: finish them under the
        # same whole-Context lease before Node removes this scheduling source.
        # No native close is repeated and no native cancellation is invented.
        with DispatchLease(node.context):
            resource._retired = True
            resource._finish_closed()
        return
    if any(owner.busy for owner in owners):
        raise RosError(Result(17))
    frame, owns = node.context.session._begin_factory(len(owners))
    try:
        frame.seed(owners)
        error = RuntimeError('service resource cleanup failed')
        error.roswasm_cleanup = frame.outcomes
        error.roswasm_failures = frame.failures
        lease = DispatchLease(node.context)
        resource.last_cleanup = frame
        resource._retired = True
        # Preexisting Future storage is terminally marked under a whole-context
        # lease BEFORE retirement. This is local125, not native cancellation.
        try:
            with lease:
                resource._finish_closed()
        except BaseException as primary:
            _factory_failure(frame, primary)
            raise
        frame.rollback(None)
        if frame.failed() or not frame.completed:
            raise error
    finally:
        node.context.session._end_factory(frame, owns)
