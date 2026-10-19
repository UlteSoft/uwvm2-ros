"""Real bounded topic lesson: two independent aliases and a guest callback.

The trusted policy must grant `publish` and `subscribe` for the same String
topic/type/QoS. The example itself cannot install those grants. Feed one `R`
line per scheduling pass through stdin (at most64); EOF ends the run. A native
observer must independently verify the exact published value. No busy wait,
ambient timer, network library or host-Python bridge is used.
"""
from roswasm import Backend, Session, Node, Executor, codecs
import _roswasm_native
import sys


def main():
    seen = []
    with Session(Backend(_roswasm_native.backend)) as session:
        node = Node(session)
        subscriber = node.create_subscription(codecs.String, 'subscribe', seen.append, 4096)
        publisher = node.create_publisher(codecs.String, 'publish')
        publisher.publish('roswasm-python:机器人').checked()
        executor = Executor(1); executor.add(subscriber)
        for _ in range(64):
            if sys.stdin.readline() != 'R\n':
                break
            result = executor.spin_once()
            if result.error not in (0, 11):
                result.checked()
            if seen:
                break
        if seen != ['roswasm-python:机器人']:
            raise RuntimeError('exact native-delivered String was not observed')
    print('PASS ROSWASM_PYTHON_TOPIC_ROUNDTRIP')


if __name__ == '__main__':
    main()
