"""Bounded DATA accounting for disjoint official test universes.

A CTest wrapper, pytest node and inner gtest/JUnit testcase are different
universes. Never add their counts to claim more tested behavior. Discovery,
actual execution, disabled/skipped/deselected and missing observations remain
separate even when the upstream command returned zero.
"""
import json
import xml.etree.ElementTree as ET


def need(value, reason):
    if not value: raise ValueError(reason)


def pairs(items):
    value = {}
    for key, item in items:
        need(key not in value, 'duplicate test metadata member'); value[key] = item
    return value


def decode(raw):
    need(len(raw) <= 64 * 1024**2, 'bounded test metadata required')
    return json.loads(raw, object_pairs_hook=pairs,
        parse_constant=lambda _: need(False, 'nonfinite test metadata'))


def ctest(raw):
    """Use the actual complete show-only JSON, including disabled wrappers."""
    value = decode(raw)
    need(value.get('kind') == 'ctestInfo' and value.get('version', {}).get('major') == 1 and
         type(value.get('tests')) is list and len(value['tests']) <= 200000, 'actual CTest JSON-v1 discovery required')
    rows = []
    for item in value['tests']:
        need(type(item.get('name')) is str and item['name'] and type(item.get('command', [])) is list,
             'actual named CTest wrapper/command required')
        disabled = [row['value'] for row in item.get('properties', []) if row.get('name') == 'DISABLED']
        need(len(disabled) <= 1 and all(type(v) is bool for v in disabled), 'ambiguous CTest disabled property')
        rows.append({'id': item['name'], 'disabled': disabled == [True], 'command': item.get('command', []),
                     'properties': item.get('properties', []), 'backtrace': item.get('backtrace')})
    need(len({row['id'] for row in rows}) == len(rows), 'duplicate CTest wrapper identity')
    return {'universe': 'ctest-registered-wrapper', 'cases': rows, 'raw': value}


def junit(raw):
    """Keep raw classname/name and declared counters; forbid XML entity input."""
    need(len(raw) <= 64 * 1024**2 and b'<!DOCTYPE' not in raw.upper() and b'<!ENTITY' not in raw.upper(),
         'bounded entity-free actual JUnit required')
    root = ET.fromstring(raw)
    need(root.tag in ('testsuite', 'testsuites'), 'actual JUnit root required')
    rows = []
    for index, case in enumerate(root.iter('testcase')):
        need(index < 200000 and type(case.get('name')) is str and case.get('name'), 'bounded named JUnit case required')
        failed, error, skipped = bool(case.findall('failure')), bool(case.findall('error')), bool(case.findall('skipped'))
        disabled = case.get('status') in ('notrun', 'disabled')
        rows.append({'ordinal': index, 'classname': case.get('classname', ''), 'name': case.get('name'),
            'file': case.get('file'), 'line': case.get('line'), 'attributes': dict(case.attrib),
            'status': 'ERROR' if error else 'FAIL' if failed else 'DISABLED' if disabled else 'SKIP' if skipped else 'PASS',
            'failure': failed, 'error': error, 'skipped': skipped, 'disabled': disabled})
    counters, mismatches = [], []
    # Reconcile EACH declared suite/aggregate with its own descendant cases.
    # Nested suite counters describe overlapping sets and are never summed.
    nodes = [value for value in root.iter() if value.tag in ('testsuite','testsuites')]
    for ordinal, node in enumerate(nodes):
        selected = {key: node.get(key) for key in ('tests', 'failures', 'errors', 'skipped', 'disabled') if node.get(key) is not None}
        need(all(value.isdecimal() for value in selected.values()), 'invalid declared JUnit count')
        descendants = list(node.iter('testcase'))
        actual = {'tests': len(descendants),
            'failures': sum(bool(value.findall('failure')) for value in descendants),
            'errors': sum(bool(value.findall('error')) for value in descendants),
            'skipped': sum(bool(value.findall('skipped')) for value in descendants),
            'disabled': sum(value.get('status') in ('notrun', 'disabled') for value in descendants)}
        difference = {key: {'declared': int(value), 'observed': actual[key]} for key, value in selected.items()
                      if int(value) != actual[key]}
        row = {'ordinal': ordinal, 'name': node.get('name'), 'declared': selected,
               'actual_descendant_cases': actual, 'counter_mismatches': difference}
        counters.append(row)
        if difference: mismatches.append({'ordinal': ordinal, 'name': node.get('name'), 'fields': difference})
    return {'universe': 'junit-reported-testcase', 'cases': rows, 'declared_counters': counters,
            'accounting_gaps': [{'JUNIT_DECLARED_COUNTERS_DIFFER_FROM_ACTUAL_CASES': mismatches}] if mismatches else [],
            'scope': 'Actual emitted cases only; absent files/cases and inner discovery remain separate gaps'}


def pytest_events(collection, events):
    """Require actual node IDs from the admitted collector plugin, not -q text."""
    inventory = decode(collection)
    need(set(inventory) == {'format_version', 'kind', 'nodes', 'deselected'} and inventory['format_version'] == 1 and
         inventory['kind'] == 'actual-upstream-pytest-collection' and type(inventory['nodes']) is list and
         type(inventory['deselected']) is list and len(inventory['nodes']) <= 200000, 'actual complete pytest collection required')
    ids = [row['id'] for row in inventory['nodes']]
    need(len(set(ids)) == len(ids) and all(type(v) is str and v for v in ids), 'distinct exact pytest node IDs required')
    reports = {}
    for line in events.splitlines():
        row = decode(line)
        need(set(row) == {'nodeid', 'when', 'outcome', 'wasxfail'} and row['nodeid'] in ids and
             row['when'] in ('setup', 'call', 'teardown') and row['outcome'] in ('passed', 'failed', 'skipped') and
             (row['wasxfail'] is None or type(row['wasxfail']) is str),
             'actual pytest phase record outside discovered universe')
        phases = reports.setdefault(row['nodeid'], {})
        need(row['when'] not in phases, 'duplicate pytest phase report'); phases[row['when']] = row
    rows = []
    for node in inventory['nodes']:
        phases = reports.get(node['id'], {})
        if any(row['outcome'] == 'failed' for row in phases.values()): status = 'FAIL'
        elif any(row['outcome'] == 'passed' and row['wasxfail'] for row in phases.values()): status = 'XPASS'
        elif any(row['outcome'] == 'skipped' and row['wasxfail'] for row in phases.values()): status = 'XFAIL'
        elif any(row['outcome'] == 'skipped' for row in phases.values()): status = 'SKIP'
        elif set(phases) == {'setup', 'call', 'teardown'} and all(row['outcome'] == 'passed' for row in phases.values()): status = 'PASS'
        else: status = 'UNEXECUTED_OR_INCOMPLETE'
        rows.append({**node, 'status': status, 'phases': phases})
    return {'universe': 'pytest-collected-node', 'cases': rows, 'deselected': inventory['deselected']}


def reconcile_ctest(discovery, execution):
    """Associate outer JUnit names only; do not mistake them for gtest children."""
    grouped = {}
    for row in execution['cases']: grouped.setdefault(row['name'], []).append(row)
    known = {row['id'] for row in discovery['cases']}
    need(set(grouped) <= known and all(len(value) == 1 for value in grouped.values()),
         'CTest JUnit identities differ from complete actual discovery')
    return {'universe': discovery['universe'], 'accounting_gaps': execution.get('accounting_gaps', []), 'cases': [{**row,
        'status': grouped[row['id']][0]['status'] if row['id'] in grouped else
            'DISABLED_UNEXECUTED' if row['disabled'] else 'UNEXECUTED',
        'actual_junit': grouped.get(row['id'], [])} for row in discovery['cases']]}


def unittest_events(collection, events):
    """Account the SDK's original loader and actual TestResult lifecycle.

    A successful subtest is not an extra discovered case. A fixture failure is
    not attached to an arbitrary case: it stays a separate accounting gap and
    leaves unstarted methods unexecuted. start/stop and final run-stop are
    required so a truncated process cannot be qualified by an earlier PASS.
    """
    inventory = decode(collection)
    need(type(inventory) is dict and set(inventory) == {'format_version','kind','nodes','modules','loader_errors'} and
         inventory['format_version'] == 1 and inventory['kind'] == 'actual-upstream-unittest-collection' and
         type(inventory['nodes']) is list and len(inventory['nodes']) <= 200000 and
         type(inventory['modules']) is list and inventory['modules'] and
         all(type(value) is str and value for value in inventory['modules']) and
         type(inventory['loader_errors']) is list, 'actual original unittest loader inventory required')
    ids = []
    for node in inventory['nodes']:
        need(type(node) is dict and set(node) == {'id','file','line'} and type(node['id']) is str and node['id'] and
             type(node['file']) is str and node['file'] and not node['file'].startswith('/') and
             '..' not in node['file'].split('/') and
             (node['line'] is None or type(node['line']) is int and node['line'] > 0), 'actual source-located unittest case required')
        ids.append(node['id'])
    need(len(set(ids)) == len(ids), 'duplicate unittest case identity')
    need(len(events) <= 64 * 1024**2, 'bounded unittest lifecycle required')
    known, state, fixtures, started, stopped = set(ids), {}, [], False, False
    for line in events.splitlines():
        row = decode(line)
        need(type(row) is dict and set(row) == {'event','id','status','detail'} and not stopped,
             'closed actual unittest lifecycle record required')
        event = row['event']
        if event in ('run-start','run-stop'):
            need(row['id'] is None and row['status'] is None and row['detail'] is None and
                 (not started if event == 'run-start' else started), 'invalid unittest run boundary')
            if event == 'run-start': started = True
            else: stopped = True
            continue
        need(started and event in ('start','stop','outcome','fixture','subtest'), 'unittest observation outside actual run')
        if event == 'fixture':
            need(type(row['id']) is str and row['id'] and row['id'] not in known and row['status'] in ('ERROR','SKIP') and
                 type(row['detail']) is str, 'actual non-case fixture error/skip required')
            fixtures.append(row); continue
        need(row['id'] in known, 'unittest lifecycle references an undiscovered case')
        value = state.setdefault(row['id'], {'started':False,'stopped':False,'outcomes':[],'subtests':[]})
        if event == 'start':
            need(not value['started'] and row['status'] is None and row['detail'] is None, 'duplicate unittest start')
            value['started'] = True
        elif event == 'stop':
            need(value['started'] and not value['stopped'] and row['status'] is None and row['detail'] is None,
                 'duplicate/out-of-order unittest stop'); value['stopped'] = True
        elif event == 'outcome':
            need(value['started'] and not value['stopped'] and
                 row['status'] in ('PASS','FAIL','ERROR','SKIP','XFAIL','XPASS') and
                 (row['detail'] is None or type(row['detail']) is str), 'invalid unittest outcome')
            # A body failure followed by a tearDown error is two legitimate
            # TestResult callbacks. Preserve both; never downgrade to PASS.
            need(row['status'] != 'PASS' or not value['outcomes'], 'duplicate/contradictory unittest success')
            need(not any(item['status'] == 'PASS' for item in value['outcomes']), 'outcome after unittest success')
            value['outcomes'].append(row)
        else:
            need(value['started'] and not value['stopped'] and row['status'] in ('PASS','FAIL','ERROR') and
                 type(row['detail']) is dict and set(row['detail']) == {'id','error'} and
                 type(row['detail']['id']) is str and row['detail']['id'] and
                 (row['detail']['error'] is None or type(row['detail']['error']) is str), 'actual bounded subtest observation required')
            value['subtests'].append(row)
    cases = []
    for node in inventory['nodes']:
        value = state.get(node['id'], {'started':False,'stopped':False,'outcomes':[],'subtests':[]})
        statuses = {row['status'] for row in value['subtests'] + value['outcomes']}
        status = next((name for name in ('ERROR','FAIL','XPASS','XFAIL','SKIP') if name in statuses),
                      'PASS' if len(value['outcomes']) == 1 and value['outcomes'][0]['status'] == 'PASS' else 'UNEXECUTED_OR_INCOMPLETE')
        if not value['started'] or not value['stopped'] or not stopped: status = 'UNEXECUTED_OR_INCOMPLETE'
        cases.append(dict(node, status=status, lifecycle=value))
    gaps = []
    if inventory['loader_errors']: gaps.append({'UNITTEST_LOADER_ERRORS':inventory['loader_errors']})
    if fixtures: gaps.append({'UNITTEST_FIXTURE_FAILURES_OR_SKIPS':fixtures})
    if not started or not stopped: gaps.append('UNITTEST_RUN_BOUNDARY_INCOMPLETE')
    return {'universe':'unittest-loaded-method','cases':cases,'modules':inventory['modules'],
            'fixture_observations':fixtures,'accounting_gaps':gaps}


def summary(universe):
    counts = {}
    for row in universe['cases']: counts[row['status']] = counts.get(row['status'], 0) + 1
    return {'universe': universe['universe'], 'observations': len(universe['cases']), 'by_status': counts,
            'deselected': len(universe.get('deselected', [])),
            'complete_success': bool(universe['cases']) and counts == {'PASS': len(universe['cases'])} and
                not any(row.get('disabled') or row.get('originally_disabled') for row in universe['cases']) and
                not universe.get('deselected') and not universe.get('accounting_gaps'),
            'accounting_gaps': universe.get('accounting_gaps', [])}


def gtest(raw):
    """Observe the real inner --gtest_list_tests universe, including DISABLED_."""
    need(len(raw) <= 64 * 1024**2, 'bounded actual gtest listing required')
    suite, rows = None, []
    for line in raw.decode('utf-8', 'strict').splitlines():
        if not line.strip() or line.startswith('Running main() from '): continue
        text = line.split('#', 1)[0].strip()
        if not text: continue
        if line[:1].isspace():
            need(suite is not None and not any(value.isspace() for value in text), 'unrecognized inner gtest testcase listing')
            name = suite + '.' + text
            rows.append({'id': name, 'suite': suite, 'name': text,
                'originally_disabled': any(part.startswith('DISABLED_') for part in suite.split('/') + text.split('/'))})
        else:
            need(text.endswith('.') and not any(value.isspace() for value in text), 'unrecognized actual gtest suite listing')
            suite = text[:-1]
    need(len(rows) <= 200000 and len({row['id'] for row in rows}) == len(rows), 'bounded distinct inner gtest identities required')
    return {'universe': 'gtest-listed-inner-case', 'cases': rows}


def reconcile_gtest(discovery, execution, *, include_disabled):
    need(type(include_disabled) is bool, 'explicit original disabled-test execution selection required')
    observed = {}
    for row in execution['cases']:
        identity = row['classname'] + '.' + row['name']
        need(identity not in observed, 'duplicate actual inner gtest result identity'); observed[identity] = row
    known = {row['id'] for row in discovery['cases']}
    need(set(observed) <= known, 'actual inner gtest result outside complete discovery')
    return {'universe': discovery['universe'], 'accounting_gaps': execution.get('accounting_gaps', []), 'cases': [{**row,
        'status': observed[row['id']]['status'] if row['id'] in observed else
            'DISABLED_UNEXECUTED' if row['originally_disabled'] and not include_disabled else 'UNEXECUTED',
        'actual_junit': observed.get(row['id'])} for row in discovery['cases']]}
