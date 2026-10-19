"""Observe the selected SDK's real unittest loader and TestResult callbacks.

This executable deliberately uses Python 2.6-compatible syntax and only its
selected interpreter's standard library. It is not imported by the coordinator.
The caller owns the process, read-only providers, output directory and budgets.
It selects whole modules, not individual methods or a synthetic case list.
Collection imports upstream tests but never runs their TestCase bodies. A
second process reloads the same modules and executes the original suite.

This is not nose, rostest or a ROS Master launcher. Those original workflows
must remain separately selected when upstream requires them. Module/class
fixtures, skipped/expected/unexpected failures and subtests are retained; no
JUnit wrapper count substitutes for the actual loader universe.
"""
import inspect
import json
import optparse
import os
import re
import sys
import unittest

MAX_CASES = 200000
MAX_BYTES = 64 * 1024 * 1024
try:
    TEXT_TYPES = (basestring,)
except NameError:
    TEXT_TYPES = (str,)


def need(value, reason):
    if not value:
        raise ValueError(reason)


def identity(test):
    value = test.id()
    need(isinstance(value, TEXT_TYPES), 'actual unittest identity must be text')
    need(value and len(value) <= 16384, 'bounded actual unittest identity required')
    return value


def nodes(suite, top):
    """Flatten the actual loader output; never select or rename test methods."""
    rows = []
    def visit(value):
        if isinstance(value, unittest.TestSuite):
            for child in value:
                visit(child)
        else:
            need(isinstance(value, unittest.TestCase), 'custom non-TestCase suite member needs an explicit adapter')
            name = identity(value)
            # _FailedTest and module fixture placeholders are loader errors,
            # not newly discovered successful upstream methods.
            method = getattr(value, getattr(value, '_testMethodName', ''), None)
            filename = inspect.getsourcefile(type(value))
            need(filename and os.path.realpath(filename).startswith(top + os.sep),
                 'loaded test class is outside the selected original source root')
            try:
                line = inspect.getsourcelines(method)[1]
            except (IOError, OSError, TypeError):
                line = None
            rows.append({'id': name, 'file': os.path.relpath(os.path.realpath(filename), top), 'line': line})
            need(len(rows) <= MAX_CASES, 'bounded unittest discovery required')
    visit(suite)
    need(len(set(row['id'] for row in rows)) == len(rows), 'duplicate actual unittest identities')
    return rows


def write_document(path, value):
    raw = json.dumps(value, sort_keys=True, ensure_ascii=True, allow_nan=False).encode('ascii') + b'\n'
    need(len(raw) <= MAX_BYTES, 'bounded unittest inventory required')
    descriptor = os.open(path, os.O_WRONLY | os.O_CREAT | os.O_EXCL, 0o600)
    stream = os.fdopen(descriptor, 'wb')
    try:
        stream.write(raw)
        stream.flush()
        os.fsync(stream.fileno())
    finally:
        stream.close()


class Observation(unittest.TestResult):
    """Retain real lifecycle callbacks including non-case fixture failures."""
    def __init__(self, stream, known):
        unittest.TestResult.__init__(self)
        self.stream, self.known, self.written = stream, set(known), 0

    def emit(self, event, test=None, status=None, detail=None):
        row = {'event': event, 'id': identity(test) if test is not None else None,
               'status': status, 'detail': detail}
        raw = json.dumps(row, sort_keys=True, ensure_ascii=True, allow_nan=False).encode('ascii') + b'\n'
        self.written += len(raw)
        need(self.written <= MAX_BYTES, 'bounded unittest lifecycle required')
        self.stream.write(raw)
        self.stream.flush()

    def startTest(self, test):
        unittest.TestResult.startTest(self, test)
        self.emit('start', test)

    def stopTest(self, test):
        self.emit('stop', test)
        unittest.TestResult.stopTest(self, test)

    def addSuccess(self, test):
        unittest.TestResult.addSuccess(self, test)
        self.emit('outcome', test, 'PASS')

    def addError(self, test, error):
        unittest.TestResult.addError(self, test, error)
        self.emit('outcome' if identity(test) in self.known else 'fixture', test, 'ERROR',
                  self._exc_info_to_string(error, test))

    def addFailure(self, test, error):
        unittest.TestResult.addFailure(self, test, error)
        self.emit('outcome', test, 'FAIL', self._exc_info_to_string(error, test))

    def addSkip(self, test, reason):
        callback = getattr(unittest.TestResult, 'addSkip', None)
        if callback is not None:
            callback(self, test, reason)
        self.emit('outcome' if identity(test) in self.known else 'fixture', test, 'SKIP', reason)

    def addExpectedFailure(self, test, error):
        callback = getattr(unittest.TestResult, 'addExpectedFailure', None)
        if callback is not None:
            callback(self, test, error)
        self.emit('outcome', test, 'XFAIL', self._exc_info_to_string(error, test))

    def addUnexpectedSuccess(self, test):
        callback = getattr(unittest.TestResult, 'addUnexpectedSuccess', None)
        if callback is not None:
            callback(self, test)
        self.emit('outcome', test, 'XPASS')

    def addSubTest(self, test, subtest, error):
        # Python 2 has no subTest API; this override is used only by SDKs which
        # actually implement it. Keep its original parameterized identity.
        callback = getattr(unittest.TestResult, 'addSubTest', None)
        if callback is not None:
            callback(self, test, subtest, error)
        status = 'PASS' if error is None else 'FAIL' if issubclass(error[0], test.failureException) else 'ERROR'
        self.emit('subtest', test, status, {'id': identity(subtest),
                  'error': None if error is None else self._exc_info_to_string(error, test)})


def main():
    parser = optparse.OptionParser()
    parser.add_option('--top')
    parser.add_option('--collection')
    parser.add_option('--events')
    parser.add_option('--collect-only', action='store_true', default=False)
    options, modules = parser.parse_args()
    need(options.top and options.collection and modules and len(modules) <= 4096,
         'explicit original module universe and fresh output paths required')
    top = os.path.realpath(options.top)
    need(os.path.isabs(options.top) and top == options.top and os.path.isdir(top), 'canonical owned original source top required')
    need(all(re.match(r'^[A-Za-z_][A-Za-z0-9_]*(\.[A-Za-z_][A-Za-z0-9_]*)*$', name) for name in modules) and
         len(set(modules)) == len(modules), 'whole distinct unittest modules required, not method filters')
    sys.dont_write_bytecode = True
    sys.path.insert(0, top)
    loader = unittest.TestLoader()
    selected = []
    for name in modules:
        module = __import__(name, globals(), locals(), ['*'])
        filename = inspect.getsourcefile(module)
        need(filename and os.path.realpath(filename).startswith(top + os.sep),
             'selected whole unittest module resolved outside original source')
        selected.append(loader.loadTestsFromModule(module))
    suite = unittest.TestSuite(selected)
    errors = list(getattr(loader, 'errors', []))
    discovered = nodes(suite, top)
    inventory = {'format_version': 1, 'kind': 'actual-upstream-unittest-collection',
                 'nodes': discovered, 'modules': modules, 'loader_errors': errors}
    write_document(options.collection, inventory)
    if options.collect_only:
        return 1 if errors else 0
    need(options.events and not errors, 'fresh unittest event path and successful original loader required')
    descriptor = os.open(options.events, os.O_WRONLY | os.O_CREAT | os.O_EXCL, 0o600)
    stream = os.fdopen(descriptor, 'wb')
    try:
        result = Observation(stream, [row['id'] for row in discovered])
        result.emit('run-start')
        suite.run(result)
        result.emit('run-stop')
        return 0 if result.wasSuccessful() else 1
    finally:
        stream.flush()
        os.fsync(stream.fileno())
        stream.close()


if __name__ == '__main__':
    sys.exit(main())
