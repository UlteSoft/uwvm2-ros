"""UNEXECUTED real-Wasm bridge smoke: no model/default grant substitution.

Freeze as roswasm_app with the PRODUCTION optional builtin. Run in the real
Runtime under an explicitly EMPTY ROS grant list. Separate granted apps prove
actual native delivery; ABI/version and denied invalid owners do not do so.
"""
import _roswasm_owned_service as native


def main():
    token = native.prepare(0,0,0,0,0,b'',0)
    assert native.attempted(token) is False
    assert native.execute(token) is None
    assert native.returned(token) is True and native.version_one(token) is True
    assert native.version_bytes(token) == b'\1\0\0\0'
    try:
        native.execute(token)
    except RuntimeError:
        pass
    else:
        raise AssertionError('opaque attempt replay admitted')
    refused = ((True,0,0,0,0,b'',0), ((1<<32)+4,0,0,0,0,b'',8),
               (4,0,0,0,0,b'',True), (4,-1,0,0,0,b'',8),
               (4,1<<64,0,0,0,b'',8), (4,0,0,0,0,bytearray(),8),
               (3,0,0,0,0,b'x',256), (7,0,0,1<<32,0,b'',352),
               (7,0,0,16,0,b'',351), (10,0,0,0,0,b'',2),
               (0,1,0,0,0,b'',0), (4,0,0,0,1,b'',8),
               (5,0,0,1,1,b'',0), (6,0,0,0,0,b'',1))
    for form in refused:
        try:
            native.prepare(*form)
        except (ValueError,TypeError,OverflowError):
            pass
        else:
            raise AssertionError('pre-effect shape/numeric refusal missing')
    # The actual Host checks current caller/grants/handles; use no fabricated
    # full handle, origin registry, fake receipt or invented Native ticket.
    for index,capacity,a,b in ((1,256,0,0),(2,256,0,0),(3,256,0,0),(4,8,0,0),
            (5,0,1,0),(6,0,0,1),(7,352,16,0),(8,0,1,1),(9,0,1,1),(10,1,0,0),(11,0,0,0)):
        context=native.prepare(index,0,0,a,b,b'',capacity)
        native.execute(context)
        assert native.attempted(context) and native.returned(context) and not native.success(context)
        assert int.from_bytes(native.status_bytes(context),'little') != 0
        assert native.output(context) == bytes(capacity)
    print('OWNED_SERVICE_REAL_BRIDGE_EMPTY_GRANT_SMOKE')
    return 0
