"""Native-only callable sequence for a genuinely Root-admitted Spot provider.

This does not authenticate, create clients, grant permissions, power on, reset
estop/faults, start LeaseKeepAlive or create a time-sync thread. Root prepares
these prerequisites independently. All cleanup roles are explicit in aliases.
"""
from ..native_provider import SpotProvider


def run(provider, aliases, start_correlation, start_nonce, velocity):
    if not isinstance(provider, SpotProvider):
        raise TypeError('actual selected native provider required')
    required = ('state', 'acquire', 'retain', 'stand', 'velocity', 'feedback', 'stop', 'return')
    if (set(aliases) != set(required) or len(set(aliases.values())) != len(required) or
            type(start_correlation) is not int or not 0 < start_correlation <= (1 << 64) - 8 or
            type(start_nonce) is not int or not 0 < start_nonce <= (1 << 64) - 8):
        raise ValueError('all independent aliases and eight non-reused correlations required')
    report = {'primary': None, 'telemetry': None, 'lease': None, 'retain': None,
              'stand': None, 'velocity': None, 'feedback': None,
              'stop': None, 'return': None}
    acquired = command_attempted = False
    try:
        report['telemetry'] = provider.invoke(aliases['state'], 'state', start_correlation, nonce=start_nonce, budget_ns=2_000_000_000)
        report['lease'] = provider.invoke(aliases['acquire'], 'lease.acquire', start_correlation + 1, nonce=start_nonce + 1, budget_ns=2_000_000_000)
        acquired = True
        report['retain'] = provider.invoke(aliases['retain'], 'lease.retain', start_correlation + 2, nonce=start_nonce + 2, budget_ns=2_000_000_000)
        command_attempted = True
        report['stand'] = provider.invoke(aliases['stand'], 'command.stand', start_correlation + 3, nonce=start_nonce + 3, budget_ns=2_000_000_000)
        report['velocity'] = provider.invoke(aliases['velocity'], 'command.velocity', start_correlation + 4, nonce=start_nonce + 4,
                                             budget_ns=2_000_000_000, arguments=velocity)
        report['feedback'] = provider.invoke(aliases['feedback'], 'command.feedback', start_correlation + 5, nonce=start_nonce + 5,
                                             budget_ns=2_000_000_000, arguments=(start_correlation + 4,))
    except BaseException as error:
        report['primary'] = error
    finally:
        # These are independently admitted commands under the expressly
        # selected stop/return roles. They are NOT inverse rollback, a native
        # cancel RPC, or proof the physical robot is stationary.
        if acquired and command_attempted:
            try:
                report['stop'] = provider.invoke(aliases['stop'], 'command.stop', start_correlation + 6, nonce=start_nonce + 6,
                                                 budget_ns=2_000_000_000)
            except BaseException as error:
                report['stop'] = error
        if acquired:
            try:
                report['return'] = provider.invoke(aliases['return'], 'lease.return', start_correlation + 7, nonce=start_nonce + 7,
                                                   budget_ns=2_000_000_000)
            except BaseException as error:
                report['return'] = error
    return report
