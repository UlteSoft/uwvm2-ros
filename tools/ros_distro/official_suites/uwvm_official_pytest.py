"""Actual pytest collection/phase ledger; Python2.7-compatible plugin syntax.

Only the selected native upstream pytest interpreter imports this module. Its
owner supplies two fresh owned output paths and bounds logs/files externally.
This is observation of real pytest, not a fabricated test-result producer.
"""
from __future__ import absolute_import
import json
import os

_DESELECTED = []


def pytest_deselected(items):
    _DESELECTED.extend(item.nodeid for item in items)


def pytest_collection_finish(session):
    path = os.environ['UWVM_OFFICIAL_COLLECTION_FILE']
    value = {'format_version': 1, 'kind': 'actual-upstream-pytest-collection',
        'nodes': [{'id': item.nodeid, 'location': list(item.location)} for item in session.items],
        'deselected': list(_DESELECTED)}
    raw = (json.dumps(value, sort_keys=True) + '\n').encode('utf-8')
    descriptor = os.open(path, os.O_WRONLY | os.O_CREAT | os.O_EXCL, 0o600)
    try:
        while raw:
            count = os.write(descriptor, raw)
            if count <= 0: raise RuntimeError('short actual collection ledger write')
            raw = raw[count:]
        os.fsync(descriptor)
    finally: os.close(descriptor)


def pytest_runtest_logreport(report):
    value = {'nodeid': report.nodeid, 'when': report.when, 'outcome': report.outcome,
             'wasxfail': str(getattr(report, 'wasxfail', ''))}
    raw = (json.dumps(value, sort_keys=True) + '\n').encode('utf-8')
    descriptor = os.open(os.environ['UWVM_OFFICIAL_RUN_FILE'], os.O_WRONLY | os.O_APPEND | os.O_CREAT, 0o600)
    try:
        while raw:
            count = os.write(descriptor, raw)
            if count <= 0: raise RuntimeError('short actual pytest phase ledger write')
            raw = raw[count:]
    finally: os.close(descriptor)
