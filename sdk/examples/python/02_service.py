"""Real Trigger client lesson using one explicitly granted `trigger` alias.

Run an independent native Trigger server returning (True, 'native-trigger').
The configured host deadline is authoritative. A timeout110 is propagated and
never replayed. Session cleanup is checked even when decoding/calling fails.
"""
from roswasm import Backend, Session, Node, codecs
import _roswasm_native


def main():
    with Session(Backend(_roswasm_native.backend)) as session:
        client = Node(session).create_client(codecs.Trigger, codecs.ServiceResponse, 'trigger', 4096)
        if client.call(None) != (True, 'native-trigger'):
            raise RuntimeError('unexpected independent native response')
    print('PASS ROSWASM_PYTHON_TRIGGER')


if __name__ == '__main__':
    main()
