"""Read the finite TinyGo backend constraint without evaluating source code.

The SDK supports both the Go ``wasm`` architecture tag and TinyGo's actual
``tinygo.wasm`` target tag.  A substring check cannot establish their Boolean
meaning: an added ``|| ros2`` would bypass backend isolation.  This parser
accepts only the five tags used by the raw bindings and checks all 32 possible
assignments, including neither/both backends and non-TinyGo hosts.  It neither
selects a compiler nor grants a ROS capability.
"""

from itertools import product
import re


TAGS = ('tinygo', 'wasm', 'tinygo.wasm', 'ros1', 'ros2')
MAXIMUM_SOURCE_BYTES = 1024 * 1024
MAXIMUM_EXPRESSION_BYTES = 4096
MAXIMUM_TOKENS = 128
MAXIMUM_NESTING = 32


def parse_expression(expression):
    """Parse a complete bounded Go constraint with its real operator precedence.

    Only identifiers, ``!``, ``&&``, ``||`` and parentheses are accepted.  An
    unknown tag or trailing token is an error; Python ``eval`` is never used.
    The returned tuple tree contains data only, with no executable callbacks.
    """
    if type(expression) is not str or not expression or not expression.isascii() or len(expression) > MAXIMUM_EXPRESSION_BYTES:
        raise ValueError('missing or oversized Go build constraint')
    tokens = []
    position = 0
    pattern = re.compile(r'[ \t]*(\&\&|\|\||!|\(|\)|[A-Za-z0-9_.]+)')
    while position < len(expression):
        if not expression[position:].strip(' \t'):
            break
        match = pattern.match(expression, position)
        if match is None:
            raise ValueError('unsupported token in Go build constraint')
        token = match.group(1)
        if token not in ('&&', '||', '!', '(', ')') and token not in TAGS:
            raise ValueError('unknown tag in Go build constraint: ' + token)
        tokens.append(token)
        if len(tokens) > MAXIMUM_TOKENS:
            raise ValueError('too many Go build constraint tokens')
        position = match.end()
    cursor = 0

    def unary(depth):
        nonlocal cursor
        if depth > MAXIMUM_NESTING or cursor >= len(tokens):
            raise ValueError('incomplete or deeply nested Go build constraint')
        token = tokens[cursor]
        cursor += 1
        if token == '!':
            return ('not', unary(depth + 1))
        if token == '(':
            result = either(depth + 1)
            if cursor >= len(tokens) or tokens[cursor] != ')':
                raise ValueError('unclosed Go build constraint parenthesis')
            cursor += 1
            return result
        if token in TAGS:
            return ('tag', token)
        raise ValueError('Go build constraint operand required')

    def both(depth):
        nonlocal cursor
        result = unary(depth)
        while cursor < len(tokens) and tokens[cursor] == '&&':
            cursor += 1
            result = ('and', result, unary(depth))
        return result

    def either(depth):
        nonlocal cursor
        result = both(depth)
        while cursor < len(tokens) and tokens[cursor] == '||':
            cursor += 1
            result = ('or', result, both(depth))
        return result

    result = either(0)
    if cursor != len(tokens):
        raise ValueError('trailing Go build constraint token')
    return result


def evaluate(tree, values):
    """Evaluate a parsed data tree for one explicit assignment of all five tags."""
    if tree[0] == 'tag':
        return values[tree[1]]
    if tree[0] == 'not':
        return not evaluate(tree[1], values)
    if tree[0] == 'and':
        return evaluate(tree[1], values) and evaluate(tree[2], values)
    if tree[0] == 'or':
        return evaluate(tree[1], values) or evaluate(tree[2], values)
    raise ValueError('unknown Go build constraint data node')


def validate_backend_source(source, backend):
    """Require the complete exact TinyGo/Wasm/exclusive-backend truth table.

    Raw SDK files use one first-line ``//go:build`` directive followed by a
    blank separator.  Additional directives, including legacy ``// +build``
    lines, are rejected rather than trying to reconcile two constraint forms.
    This deliberate SDK convention is narrower than general Go source syntax.
    Equivalent parenthesization or operand order remains valid.  The older
    ``tinygo && wasm`` constraint fails because it excludes the real supported
    TinyGo pure target.  The successful result describes source evidence only.
    """
    if type(backend) is not int or backend not in (1, 2):
        raise ValueError('exact ROS backend 1 or 2 required')
    if type(source) is not str or len(source.encode('utf-8')) > MAXIMUM_SOURCE_BYTES:
        raise ValueError('ordinary bounded Go source text required')
    lines = source.splitlines()
    if len(lines) < 3 or not lines[0].startswith('//go:build ') or lines[1].strip():
        raise ValueError('one first-line Go build constraint and blank separator required')
    if any(line.lstrip().startswith(('//go:build', '// +build')) for line in lines[1:]):
        raise ValueError('duplicate or legacy Go build constraints are not selected')
    tree = parse_expression(lines[0][len('//go:build '):])
    selected = []
    for assignment in product((False, True), repeat=len(TAGS)):
        values = dict(zip(TAGS, assignment))
        expected = (values['tinygo'] and (values['wasm'] or values['tinygo.wasm'])
                    and values['ros' + str(backend)] and not values['ros' + str(3 - backend)])
        actual = evaluate(tree, values)
        if actual != expected:
            raise ValueError('Go backend constraint differs for tags: ' + repr(values))
        if actual:
            selected.append(values)
    return {'backend': backend, 'assignments_checked': 32,
            'selected_assignments': selected,
            'scope': 'SOURCE_BUILD_CONSTRAINT_ONLY_NO_COMPILER_OR_ROS_EXECUTION'}
