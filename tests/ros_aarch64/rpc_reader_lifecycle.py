"""Draft bounded ordering adapter for denial witnesses; enable after diagnosis.

The original denial witness checks actual GIDs for5seconds after baseline.
Runner.case originally stops the Gateway before RpcRunner waits for that
witness, although the Gateway owns an independently known framework reader.
This adapter delays only those two Gateway stop calls until the original
witness call returns. Guest exit/session disconnect and assertions are intact.
"""
import os
from pathlib import Path
import sys
import time


def identity(process):
    fields = (Path('/proc')/str(process.pid)/'stat').read_text().rsplit(')', 1)[1].split()
    return {'pid': process.pid, 'starttime': int(fields[19]),
            'membership': (Path('/proc')/str(process.pid)/'cgroup').read_text().splitlines()}


class DenialGatewayLifetime:
    """Patch exact original case finally calls, then restore every method."""

    def __init__(self, network, rpc):
        self.network, self.rpc = network, rpc
        self.original_case = network.Runner.case
        self.original_stop = network.Runner.stop
        self.original_event = rpc.RpcRunner.typed_event
        self.contexts, self.rows = {}, []
        self.installed = False

    def install(self):
        if self.installed:
            raise RuntimeError('denial Gateway lifetime adapter already installed')
        owner = self

        def stop(receiver, process):
            frame = sys._getframe(1)
            context = owner.contexts.get(receiver)
            # The exact original case finally calls stop once for the Worker,
            # each Gateway and each case-owned native witness. Defer a live
            # Gateway only; Worker and native teardown use their original path.
            if (context is not None and frame.f_code is owner.original_case.__code__ and
                    frame.f_locals.get('name') == context['name'] and
                    process in frame.f_locals.get('gateways', ()) and process.poll() is None):
                facts = identity(process)
                row = {'name': context['name'], 'gateway': facts,
                    'original_stop_site': {'file': frame.f_code.co_filename, 'line': frame.f_lineno},
                    'deferred_at_monotonic': time.monotonic(), 'status': 'DEFERRED',
                    'scope': 'Specific denied-case trusted Gateway retained through original native absence observation'}
                context['deferred'].append((process, row));owner.rows.append(row)
                return
            return owner.original_stop(receiver, process)

        def event(receiver, description):
            if description['scenario'] not in ('permission-denied', 'no-capability'):
                return owner.original_event(receiver, description)
            if receiver in owner.contexts:
                raise RuntimeError('nested denied witness lifetime operation')
            context = {'name': description['name'], 'deferred': []}
            owner.contexts[receiver] = context
            completed = False
            try:
                result = owner.original_event(receiver, description)
                completed = True
                return result
            finally:
                owner.contexts.pop(receiver)
                for process, row in context['deferred']:
                    row['original_witness_completed'] = completed
                    row['released_at_monotonic'] = time.monotonic()
                    row['deferral_seconds'] = row['released_at_monotonic']-row['deferred_at_monotonic']
                    row['before_original_stop_returncode'] = process.poll()
                    if process.poll() is None:
                        current = identity(process)
                        if current != row['gateway']:
                            raise RuntimeError('deferred trusted Gateway PID identity or cgroup changed')
                    owner.original_stop(receiver, process)
                    row['after_original_stop_returncode'] = process.poll()
                    row['status'] = 'RELEASED'

        self.network.Runner.stop = stop
        self.rpc.RpcRunner.typed_event = event
        self.installed = True

    def restore(self):
        if self.contexts:
            raise RuntimeError('denial Gateway lifetime adapter still has active contexts')
        if self.installed:
            self.network.Runner.stop = self.original_stop
            self.rpc.RpcRunner.typed_event = self.original_event
            self.installed = False

    def evidence(self):
        if self.contexts or any(row['status'] != 'RELEASED' for row in self.rows):
            raise RuntimeError('denial Gateway lifetime cleanup was not completed')
        return {'rows': self.rows,
            'original_case_code_file': self.original_case.__code__.co_filename,
            'scope': 'Original Guest and native assertions/budgets; trusted Gateway stop ordered after native denial witness only'}
