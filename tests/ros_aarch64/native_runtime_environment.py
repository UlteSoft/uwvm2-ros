"""Qualify the optional native loader path of an already qualified Gateway.

This module reads metadata and regular file bytes only. It does not run a
compiler, load a shared library, activate a ROS context, or grant Guest rights.
Call ``gateway_library_environment`` only after strict Gateway qualification,
passing the same canonical SDK root and complete read-only SDK inventory. The
caller must preserve that complete inventory, root aliases and returned file
pins throughout the run. In particular, file pins do not replace the inventory's
symlink records or establish an ELF DT_NEEDED dependency graph.

The result contains at most one literal native ``LD_LIBRARY_PATH`` entry. A
caller must start from its clean native environment, apply these entries, and
activate the trusted distro setup with ``source ... && exec ...``. There is no
ambient loader value, arbitrary prefix, empty search entry or implicit ROS/WASI
permission. Base setup and any additional setup hooks remain caller-owned,
separately bound SDK inputs.
"""
import os
from pathlib import Path
import posixpath
import re
import stat

from gateway_qualification import read_file_binding
from rebuild_affected import rooted

CUSTOM_PREFIX = '/opt/uwvm-custom-types'
CUSTOM_RELATIVE = 'opt/uwvm-custom-types'
CUSTOM_LIBRARY = CUSTOM_PREFIX + '/lib'
CUSTOM_LIBRARY_RELATIVE = CUSTOM_RELATIVE + '/lib'
SHARED_LIBRARY = re.compile(r'\.so(?:\.[0-9]+)*$')


def _require(condition, message):
    if not condition:
        raise RuntimeError('native Gateway loader qualification: ' + message)


def _canonical_path(value, absolute):
    """Reject alternate spelling and shell/loader separators in path metadata."""
    return (isinstance(value, str) and bool(value) and
            value.startswith('/') == absolute and posixpath.normpath(value) == value and
            not any(char.isspace() or char in '\\:\x00' for char in value) and
            '..' not in value.split('/') and '.' not in value.split('/'))


def _elf_header(path, pin):
    """Read only the pinned regular file's header, with identity brackets.

    The shared binder already streamed and hashed the complete file. This
    bounded nonblocking read must refer to those same bytes and file identity;
    it cannot block on a FIFO or silently inspect a replacement. No loader or
    executable is invoked, even for the synthetic model files.
    """
    fd = os.open(path, os.O_RDONLY | os.O_NONBLOCK)
    try:
        first = os.fstat(fd)
        keys = (('st_dev', 'device'), ('st_ino', 'inode'), ('st_size', 'bytes'),
                ('st_mtime_ns', 'mtime_ns'), ('st_ctime_ns', 'ctime_ns'))
        _require(stat.S_ISREG(first.st_mode) and
                 all(getattr(first, key) == pin[field] for key, field in keys),
                 'library identity changed before its ELF header read')
        header = os.read(fd, 64)
        last = os.fstat(fd)
        _require(all(getattr(first, key) == getattr(last, key) for key, _ in keys),
                 'library changed during its ELF header read')
        return header
    finally:
        os.close(fd)


def _arm_shared(header):
    """Require ELF64, little-endian AArch64 ET_DYN shared-library bytes."""
    return (len(header) == 64 and header[:4] == b'\x7fELF' and header[4:7] == b'\x02\x01\x01' and
            int.from_bytes(header[16:18], 'little') == 3 and
            int.from_bytes(header[18:20], 'little') == 183)


def gateway_library_environment(qualification, sdk, inventory):
    """Return a closed native environment and preserved custom library pins.

    ``qualification`` is the result of ``gateway_qualification.qualify_gateway``;
    this helper never accepts a raw build report as an equivalent qualification.
    Both ROS versions are supported. A disabled custom selection refuses custom
    scalar links, including links whose resolved SDK path reveals an alias.
    An enabled ROS1 message package with no custom scalar links is explicitly
    header-only and receives no loader path. Enabled ROS2 requires its recorded
    custom shared-library links.

    Every direct custom link must name the exact logical library directory and
    resolve to its recorded SDK-relative regular bytes. The complete directory
    inventory is checked as well, and every ELF shared object there is pinned
    and checked for ARM architecture. This includes installed type-support
    libraries which are dependencies rather than scalar link-argv operands.
    """
    _require(isinstance(qualification, dict) and isinstance(inventory, dict) and
             all(isinstance(name, str) for name in inventory),
             'strict qualification and complete SDK inventory mappings required')
    sdk = Path(sdk).resolve(strict=True)
    _require(sdk.is_dir() and qualification.get('sdk_root') == str(sdk),
             'qualification and canonical SDK roots differ')
    binding = qualification.get('custom_selection_binding')
    selection = binding.get('selection') if isinstance(binding, dict) else None
    _require(isinstance(selection, dict) and type(selection.get('enabled')) is bool and
             type(selection.get('version')) is int and selection['version'] in (1, 2) and
             type(qualification.get('version')) is int and qualification['version'] == selection['version'] and
             qualification.get('target') == 'uwvm-ros' + str(selection['version']) + '-gateway',
             'explicit qualified matching ROS1/ROS2 selection required')
    linked = qualification.get('sdk_link_inputs')
    _require(isinstance(linked, dict), 'qualified scalar SDK link inputs required')
    custom = {}
    for logical, fact in linked.items():
        _require(_canonical_path(logical, True) and isinstance(fact, dict) and
                 _canonical_path(fact.get('resolved_sdk_path'), False),
                 'malformed logical or resolved SDK link path')
        if (logical == CUSTOM_PREFIX or logical.startswith(CUSTOM_PREFIX + '/') or
                fact['resolved_sdk_path'] == CUSTOM_RELATIVE or
                fact['resolved_sdk_path'].startswith(CUSTOM_RELATIVE + '/')):
            custom[logical] = fact
    empty = {'environment': {}, 'input_pins': [], 'libraries': {}, 'enabled': False,
             'scope': 'CUSTOM_DISABLED; trusted native loader selection only; no Guest permission'}
    if not selection['enabled']:
        _require(not custom, 'default Gateway unexpectedly links custom SDK inputs')
        return empty
    prefix = selection.get('prefix')
    _require(isinstance(prefix, dict) and prefix.get('guest_path') == CUSTOM_PREFIX and
             prefix.get('sdk_relative') == CUSTOM_RELATIVE, 'custom guest/SDK-relative prefix differs')
    selected_prefix = rooted(sdk, CUSTOM_PREFIX).resolve(strict=True)
    _require(selected_prefix.is_dir() and selected_prefix == sdk / CUSTOM_RELATIVE,
             'custom prefix aliases another SDK directory')
    if not custom:
        _require(selection['version'] == 1, 'enabled ROS2 Gateway has no qualified custom native links')
        return {**empty, 'enabled': True, 'prefix': CUSTOM_PREFIX,
                'scope': 'ROS1_HEADER_ONLY; no custom scalar native links or loader path; no Guest permission'}
    directory = rooted(sdk, CUSTOM_LIBRARY).resolve(strict=True)
    _require(directory.is_dir() and directory == sdk / CUSTOM_LIBRARY_RELATIVE,
             'custom native library directory differs from the exact immutable SDK path')
    expected = {name for name in inventory if name.startswith(CUSTOM_LIBRARY_RELATIVE + '/')}
    actual = {}
    for path in directory.rglob('*'):
        relative = str(path.relative_to(sdk))
        if path.is_symlink():
            _require(not rooted(sdk, '/' + relative).is_dir(),
                     'nested library directory symlink makes closure ambiguous')
            actual[relative] = path
        elif path.is_dir():
            continue
        else:
            _require(stat.S_ISREG(path.lstat().st_mode), 'custom library directory contains a special file')
            actual[relative] = path
    _require(set(actual) == expected, 'custom native library directory inventory exactset differs')
    pins, libraries, files = {}, {}, {}
    for relative, logical_file in sorted(actual.items()):
        recorded = inventory[relative]
        _require(isinstance(recorded, dict), 'malformed custom SDK inventory entry')
        if logical_file.is_symlink():
            _require(recorded == {'symlink': os.readlink(logical_file)}, 'custom native symlink differs from SDK inventory')
        else:
            _require('symlink' not in recorded, 'custom SDK regular file was replaced by a symlink')
        path = rooted(sdk, '/' + relative).resolve(strict=True)
        _require(path.is_relative_to(directory) and path.is_file(), 'custom native library alias escapes its directory')
        resolved = str(path.relative_to(sdk))
        pin = pins.get(resolved)
        if pin is None:
            pin = read_file_binding(path)
            source = inventory.get(resolved, {})
            _require(isinstance(source, dict) and source.get('sha256') == pin['sha256'] and type(source.get('bytes')) is int and
                     source['bytes'] == pin['bytes'], 'custom native file differs from full SDK hash/size')
            pins[resolved] = pin
        files[relative] = (resolved, pin)
        header = _elf_header(path, pin)
        if header[:4] == b'\x7fELF':
            _require(SHARED_LIBRARY.search(relative) is not None and _arm_shared(header),
                     'custom native ELF is not an ARM64 little-endian shared object')
            libraries['/' + relative] = pin
    for logical, fact in custom.items():
        _require(posixpath.dirname(logical) == CUSTOM_LIBRARY and SHARED_LIBRARY.search(logical) is not None,
                 'custom scalar link is outside the closed shared-library directory')
        item = files.get(logical[1:])
        _require(item is not None and logical in libraries, 'linked custom shared object is absent or not ARM ELF')
        resolved, pin = item
        _require(fact.get('resolved_sdk_path') == resolved and fact.get('sha256') == pin['sha256'] and
                 ('bytes' not in fact or type(fact['bytes']) is int and fact['bytes'] == pin['bytes']),
                 'custom native link differs from its qualified resolved hash/size')
    _require(libraries, 'custom native shared-library closure is empty')
    return {'environment': {'LD_LIBRARY_PATH': CUSTOM_LIBRARY},
            'input_pins': [pins[name] for name in sorted(pins)], 'libraries': libraries,
            'enabled': True, 'prefix': CUSTOM_PREFIX,
            'scope': 'QUALIFIED_CUSTOM_ARM_LIBRARY_DIRECTORY; full SDK inventory still required; '
                     'no DT_NEEDED graph or Guest permission claim'}
