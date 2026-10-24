"""Actual frozen Python codec computation against independent C++ Host bytes."""
import sys
from roswasm import TransformCodec
from roswasm._transform_vectors import CASES


def main():
    if sys.argv[1:] not in ([], ['corrupt-golden']):
        return 2
    corrupt = bool(sys.argv[1:])
    accepted = refused = 0
    for index, (backend, wire_hex, code, output_hex) in enumerate(CASES):
        wire = bytes.fromhex(wire_hex)
        try:
            codec = TransformCodec(backend)
            value = codec.decode(wire)
        except ValueError:
            if code != 22:
                raise RuntimeError('C++ accepted input refused by Python: ' + str(index))
            refused += 1
        else:
            if code != 0:
                raise RuntimeError('C++ refused input accepted by Python: ' + str(index))
            encoded = codec.encode(value)
            expected = bytes.fromhex(output_hex)
            if corrupt and accepted == 0:
                expected = bytes((expected[0] ^ 1,)) + expected[1:]
            if encoded != expected or encoded != wire:
                raise RuntimeError('complete independent golden mismatch: ' + str(index))
            accepted += 1
    if not accepted or not refused:
        raise RuntimeError('both corpus decisions required')
    print('PASS_PYTHON_TRANSFORM_CORPUS', len(CASES), accepted, refused, flush=True)
    return 0
