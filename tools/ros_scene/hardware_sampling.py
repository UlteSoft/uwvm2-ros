"""Audit collected hardware periods instead of trusting a profiler request.

VTune can clamp a requested sample-after value in sep.events while retaining
the requested value in context_values.cfg. Perf's event attributes and sample
records are the evidence for the programmed period. Timestamp deltas are wall
time, including scheduling and interrupt overhead; they are not call latency.
"""
import collections
from pathlib import Path
import re
import statistics
import xml.etree.ElementTree as ET


def reference_frequency(context, expected_host=None):
    root=ET.parse(context).getroot()
    if expected_host is not None:
        hosts={item.get('value') for item in root.iter('contextValue') if item.get('id') in ('computerName','hostName')}
        if hosts!={expected_host}:
            raise ValueError('Reference-frequency context is not from the current host')
    for item in root.iter('contextValue'):
        if item.get('id') == 'referenceFrequency':
            values = [v for k, v in item.attrib.items() if k.endswith('}value') or k == 'value']
            if len(values) == 1 and values[0].isdigit() and int(values[0]) > 0:
                return int(values[0])
    raise ValueError('No measured reference frequency in the supplied VTune context')


def vtune_periods(result):
    """Keep request clamping visible even when collection exits successfully."""
    result = Path(result)
    command = result / 'data.0/perfcmd'
    events = result / 'data.0/sep.events'
    text = events.read_text() if events.is_file() else ''
    return dict(actual_perf_periods=[int(v) for v in re.findall(r'period=(\d+)', command.read_text())] if command.is_file() else [],
                adjustments=[line for line in text.splitlines() if 'value reset' in line or 'too low' in line])


def audit_samples(evlist, samples, raw_events, expected, reference_hz, target_comm='uwvm2-ros'):
    """Reject absent, frequency-mode, clamped, lost or throttled sample evidence.

    Inputs are retained outputs of perf evlist -v and perf script, not counters
    inferred from the requested command. Only the named target's samples enter
    the interval distribution; each thread/event has an independent timeline.
    """
    attributes = {}
    for line in evlist.splitlines():
        match = re.match(r'([^:]+): .*\{ sample_period, sample_freq \}: (\d+),', line)
        if match:
            attributes[match[1]] = dict(period=int(match[2]), frequency_mode=bool(re.search(r'\bfreq: 1\b', line)),
                                        user_only='exclude_kernel: 1' in line)
    periods = collections.defaultdict(collections.Counter)
    times = collections.defaultdict(list)
    pattern = re.compile(r'^\s*(\S+)\s+(\d+)/(\d+)\s+(\d+)\.(\d+):\s+(\d+)\s+([^:]+):\s+[0-9a-f]+\s*$')
    for line in samples.splitlines():
        match = pattern.match(line)
        if not match:
            if line.strip():
                raise ValueError('Unrecognized perf sample: ' + line[:200])
            continue
        comm, pid, tid, seconds, fraction, period, event = match.groups()
        if comm != target_comm or event not in expected:
            continue
        periods[event][int(period)] += 1
        times[pid, tid, event].append(int(seconds)*10**9 + int(fraction.ljust(9, '0')))
    losses = {kind: len(re.findall(r': ' + kind + r'\b', raw_events)) for kind in
              ('PERF_RECORD_LOST', 'PERF_RECORD_LOST_SAMPLES', 'PERF_RECORD_THROTTLE', 'PERF_RECORD_UNTHROTTLE')}
    raw_sample_count=len(re.findall(r': PERF_RECORD_SAMPLE\(',raw_events))
    raw_complete=raw_sample_count>=sum(sum(values.values()) for values in periods.values())>0
    shift=re.search(r'Time Shift\s+(\d+)',raw_events)
    multiplier=re.search(r'Time Multiplier\s+(\d+)',raw_events)
    tsc_hz=(2**int(shift[1])*10**9/int(multiplier[1])) if shift and multiplier and int(multiplier[1]) else None
    frequency_consistent=bool(tsc_hz and reference_hz>0 and abs(tsc_hz/reference_hz-1)<.01)
    observations = {}
    for event, requested in expected.items():
        deltas = sorted(b-a for (pid, tid, name), values in times.items() if name == event for a, b in zip(values, values[1:]))
        attr = attributes.get(event, {})
        verified = (attr.get('period') == requested and attr.get('user_only') and not attr.get('frequency_mode')
                    and set(periods[event]) == {requested} and bool(deltas) and min(deltas)>0)
        observations[event] = dict(requested_period=requested, attribute=attr, sample_periods=dict(periods[event]),
                                   sample_count=sum(periods[event].values()), period_verified=bool(verified),
                                   wall_spacing_ns_p50=statistics.median(deltas) if deltas else None,
                                   wall_spacing_ns_p95=deltas[min(len(deltas)-1, int(.95*len(deltas)))] if deltas else None)
    reference = observations.get('CPU_CLK_UNHALTED.REF_TSC', {})
    nominal = reference.get('requested_period', 0)*10**9/reference_hz if reference_hz > 0 else None
    return dict(events=observations, loss_and_throttle_records=losses, reference_hz=reference_hz,
                raw_sample_count=raw_sample_count,raw_records_complete=raw_complete,
                trace_tsc_hz=tsc_hz,reference_matches_trace_tsc=frequency_consistent,
                nominal_reference_period_ns=nominal,
                periods_verified=bool(observations) and all(v['period_verified'] for v in observations.values()) and raw_complete and not any(losses.values()),
                ten_us_reference_period_verified=bool(reference.get('period_verified') and raw_complete and frequency_consistent and nominal and nominal <= 10100 and not any(losses.values())),
                fixed_wall_interval_guaranteed=False)
