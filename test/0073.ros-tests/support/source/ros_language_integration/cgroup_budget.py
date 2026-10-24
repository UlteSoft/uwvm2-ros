"""Conservative cache-aware admission data for new bounded Linux test owners.

This reads accounting only. It never changes a cgroup limit, writes reclaim or
drop_caches, removes files, or signals another process. Existing strict-current
owners keep their original admission profile. A caller opting into this new
profile must retain the sample, exact group identity and hard/swap limits, and
still enforce finite owned process-tree RSS/output/CPU/wall limits.
"""
from pathlib import Path
import time


def _statistics(group):
    rows = [line.split() for line in (group / 'memory.stat').read_text().splitlines()]
    if any(len(row) != 2 or not row[1].isdecimal() for row in rows):
        raise ValueError('invalid cgroup memory statistics')
    values = {key: int(value) for key, value in rows}
    if len(values) != len(rows):
        raise ValueError('duplicate cgroup memory statistic')
    names = ('file', 'shmem', 'inactive_file', 'file_dirty', 'file_writeback', 'file_mapped')
    if any(name not in values for name in names):
        raise ValueError('complete cache accounting required')
    return {name: values[name] for name in names}


def _credit(statistics):
    # memory.stat:file includes tmpfs/shared memory. Never credit those pages.
    # Also exclude all dirty, writeback and mapped file bytes; overlap between
    # exclusions deliberately reduces the allowance. Only inactive file pages
    # can contribute. Active cache, slab and all other charges remain included.
    ordinary = max(0, statistics['file'] - statistics['shmem']
        - statistics['file_dirty'] - statistics['file_writeback'] - statistics['file_mapped'])
    return min(statistics['inactive_file'], ordinary)


def sample(group):
    """Return a retained accounting sample, without claiming atomic peak data.

    Use the largest of three charged readings and smallest of two independent
    cache allowances. Kernel counters and other actors can change concurrently;
    an independent owner must sample throughout its bounded work and fail on
    OOM/kill changes. The kernel hard limit remains the final aggregate bound.
    """
    group = Path(group)
    if (group / 'memory.max').read_text().strip() != str(64 << 30):
        raise ValueError('original hard 64GiB group required')
    if (group / 'memory.swap.max').read_text().strip() != '0':
        raise ValueError('original zero-swap group required')
    current = [int((group / 'memory.current').read_text())]
    first = _statistics(group)
    current.append(int((group / 'memory.current').read_text()))
    second = _statistics(group)
    current.append(int((group / 'memory.current').read_text()))
    if any(value < 0 for value in current):
        raise ValueError('invalid charged memory')
    charged = max(current)
    credit = min(_credit(first), _credit(second), charged)
    return {'profile': 'conservative-inactive-nonshared-unmapped-clean-file-credit-v1',
            'monotonic_ns': time.monotonic_ns(), 'charged_samples': current,
            'first_statistics': first, 'second_statistics': second,
            'charged_bytes': charged, 'cache_credit_bytes': credit,
            'admission_bytes': charged - credit,
            'hard_limit_bytes': 64 << 30, 'swap_limit_bytes': 0,
            'limits_or_cache_modified': False}
