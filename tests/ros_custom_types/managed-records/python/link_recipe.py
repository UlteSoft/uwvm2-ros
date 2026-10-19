"""Read the actual CPython target Makefile link closure without invoking make.

This is a bounded data reader, not a compiler or process supervisor. Execute
it only inside the selected Linux build owner. No shell, make functions,
environment substitution, package installation or native application runs.
"""
import argparse
import hashlib
import json
from pathlib import Path
import re
import shlex
import sys

KEYS = ('CC', 'LINKCC', 'PY_CORE_LDFLAGS', 'LIBRARY', 'LIBS', 'MODLIBS',
        'SYSLIBS', 'LINKFORSHARED')


def read_recipe(path):
    raw = path.read_bytes()
    if len(raw) > 1 << 20:
        raise ValueError('bounded generated Makefile required')
    definitions = {}
    for line in raw.decode('utf-8').replace('\\\n', '').splitlines():
        match = re.match(r'^([A-Za-z_][A-Za-z_0-9]*)\s*=\s*(.*)$', line)
        if match:
            definitions[match[1]] = match[2].strip()
    variable = re.compile(r'\$\(([A-Za-z_][A-Za-z_0-9]*)\)|\$\{([A-Za-z_][A-Za-z_0-9]*)\}')

    undefined = set()

    def resolve(name, stack=()):
        if name in stack:
            raise ValueError('recursive generated Makefile variable: ' + name)
        # Ordinary make references to undefined variables expand to empty.
        # Record every such reference, and never inherit Host environment
        # values or evaluate make functions/commands.
        if name not in definitions:
            undefined.add(name)
            return ''
        value = variable.sub(lambda match: resolve(match[1] or match[2], (*stack, name)), definitions[name])
        if '$' in value:
            raise ValueError('unsupported generated Makefile expression: ' + name)
        return value

    values = {name: resolve(name) for name in KEYS}
    if values['LIBRARY'] != 'libpython3.14.a':
        raise ValueError('actual matching CPython target library required')
    return {'kind': 'ACTUAL_GENERATED_MAKEFILE_LINK_RECIPE_DATA_ONLY',
            'Makefile': str(path), 'bytes': len(raw), 'sha256': hashlib.sha256(raw).hexdigest(),
            'variables': values,
            'library_arguments': [argument for key in ('LIBS', 'MODLIBS', 'SYSLIBS', 'LINKFORSHARED')
                                  for argument in shlex.split(values[key])],
            'explicit_undefined_empty_defaults': sorted(undefined),
            'Host_environment_substitution': False,
            'executed_compiler_or_guest': False}


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--makefile', type=Path, required=True)
    args = parser.parse_args()
    if sys.platform != 'linux' or not args.makefile.is_absolute() or args.makefile.is_symlink():
        raise ValueError('actual Linux owner and ordinary absolute target Makefile required')
    print(json.dumps(read_recipe(args.makefile), indent=2))


if __name__ == '__main__':
    main()
