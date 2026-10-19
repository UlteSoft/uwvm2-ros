"""Real Fibonacci client with separate goal/feedback/result/cancel grants.

Start an independently observed native Fibonacci server. Feed bounded stdin
credits; this example never waits using an ambient clock or repeats a goal.
The terminal full sequence for order5 is [0,1,1,2,3,5]. Cleanup closes every
alias separately and never upgrades cancel-own into cancel-any authority.
"""
from roswasm import Backend, Session, codecs
from roswasm.client import ActionClient
import _roswasm_native
import sys


def main():
    with Session(Backend(_roswasm_native.backend)) as session:
        send, feedback, result, cancel = [session.open(name) for name in ('goal', 'feedback', 'result', 'cancel')]
        action = ActionClient(send, feedback, result, cancel)
        identity = action.send_goal(codecs.fibonacci_goal(5))
        complete = False
        for _ in range(64):
            if sys.stdin.readline() != 'R\n':
                break
            outcome = action.receive(identity, 512, terminal=True)
            if outcome.error == 11:
                continue
            outcome.checked()
            actual_id, kind, sequence = codecs.action_sequence(outcome.data)
            if actual_id != identity or kind != 2 or sequence != (0, 1, 1, 2, 3, 5):
                raise RuntimeError('unexpected native terminal result')
            complete = True; break
        if not complete:
            raise RuntimeError('bounded native Action result was not delivered')
    print('PASS ROSWASM_PYTHON_FIBONACCI')


if __name__ == '__main__':
    main()
