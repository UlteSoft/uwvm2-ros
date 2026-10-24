#!/usr/bin/env python3
"""@file wasm_archive.py
@brief Inspect real archive members before a Wasm static-library cohort runs.
@details This is artifact admission, not a compiler, linker or proof of symbol
extraction. A final module without linking metadata cannot qualify as an object.
Qualification additionally needs source-pinned producer commands, actual linker
extraction observations and execution of the resulting consumer application.
"""
from pathlib import Path
import hashlib


def _uleb(data, at, end):
    """@brief Read one bounded uint64 LEB128 field without consuming another section."""
    value = 0
    for shift in range(0, 70, 7):
        if at >= end:
            raise ValueError('Truncated unsigned LEB128')
        byte = data[at]
        at += 1
        if shift == 63 and byte > 1:
            raise ValueError('Unsigned LEB128 exceeds uint64')
        value |= (byte & 127) << shift
        if byte < 128:
            return value, at
    raise ValueError('Overlong unsigned LEB128')


def object_metadata(data):
    """@brief Require actual Wasm v1 bytes with recognized relocatable linking metadata.
    @param data One complete immutable archive member, owned by the caller.
    @return SHA-256, byte length, linking version and relocation-section names.
    @exception ValueError Truncated modules or absent/unsupported linking metadata.
    @details Linking metadata is necessary admission evidence, never sufficient
    proof of a valid relocatable producer. No final-Wasm archive is accepted on
    its filename or archive wrapper alone. The shared language owner must pair
    these bytes with its genuine compiler/assembler and successful object link.
    """
    if data[:8] != b'\0asm\x01\0\0\0':
        raise ValueError('Archive member is not a Wasm v1 object')
    at, linking, relocations = 8, [], []
    while at < len(data):
        section = data[at]
        size, begin = _uleb(data, at + 1, len(data))
        end = begin + size
        if end > len(data):
            raise ValueError('Wasm section exceeds member bytes')
        if section == 0:
            length, cursor = _uleb(data, begin, end)
            if cursor + length > end:
                raise ValueError('Truncated custom-section name')
            name = data[cursor:cursor + length].decode('utf-8')
            cursor += length
            if name == 'linking':
                version, cursor = _uleb(data, cursor, end)
                linking.append(version)
            elif name.startswith('reloc.'):
                relocations.append(name)
        at = end
    if linking != [2]:
        raise ValueError('Require exactly one version-two Wasm linking section')
    return {'bytes': len(data), 'sha256': hashlib.sha256(data).hexdigest(),
            'linking_version': 2, 'relocation_sections': relocations,
            'linked_consumer_execution_qualified': False}


def inspect_archive(path, *, maximum_bytes=268435456, maximum_members=4096):
    """@brief Inspect GNU/BSD ar object members with bounded extents and unique names.
    @param path Complete ordinary ar archive; thin/native archives are refused.
    @param maximum_bytes Finite inclusive input-size budget, default256MiB.
    @param maximum_members Finite member-count ceiling, default4096.
    @return Immutable archive identity, index presence and each actual Wasm member.
    @exception ValueError Bad archive framing, duplicate names or a non-object input.
    @details Index/long-name members are metadata, never compiled objects. Names
    identify members only: this reader extracts no filesystem paths or executables.
    An archive index is required so the ordinary linker can select symbols; the
    actual selected and unselected sentinel members remain execution obligations.
    """
    path = Path(path)
    if not 8 <= path.stat().st_size <= maximum_bytes:
        raise ValueError('Archive exceeds its finite input budget')
    data = path.read_bytes()
    if data[:8] != b'!<arch>\n':
        raise ValueError('Require an ordinary indexed ar archive, not thin/native substitution')
    at, names, index, members, seen = 8, b'', False, [], set()
    count = 0
    while at < len(data):
        count += 1
        if count > maximum_members or at + 60 > len(data):
            raise ValueError('Truncated or excessive archive members')
        header = data[at:at + 60]
        if header[58:60] != b'`\n':
            raise ValueError('Invalid archive member header')
        raw_name = header[:16].decode('ascii').rstrip()
        size_text = header[48:58].decode('ascii').strip()
        if not size_text.isdecimal():
            raise ValueError('Invalid archive member extent')
        size = int(size_text)
        begin = at + 60
        end = begin + size
        if end > len(data):
            raise ValueError('Archive member exceeds the complete input')
        payload = data[begin:end]
        at = end + (size % 2)
        if at > len(data):
            raise ValueError('Missing odd-member archive padding')
        if raw_name in ['/', '/SYM64/', '__.SYMDEF', '__.SYMDEF SORTED']:
            index = True
            continue
        if raw_name == '//':
            if names:
                raise ValueError('Repeated archive long-name table')
            names = payload
            continue
        if raw_name.startswith('#1/'):
            length = int(raw_name[3:])
            if not 0 < length <= len(payload):
                raise ValueError('Truncated BSD member name')
            name = payload[:length].rstrip(b'\0').decode('utf-8')
            payload = payload[length:]
            if name in ['__.SYMDEF', '__.SYMDEF SORTED']:
                index = True
                continue
        elif raw_name.startswith('/'):
            offset = int(raw_name[1:])
            if not 0 <= offset < len(names):
                raise ValueError('Invalid long-name table offset')
            finish = names.find(b'\n', offset)
            if finish < 0:
                raise ValueError('Unterminated long archive name')
            name = names[offset:finish].removesuffix(b'/').decode('utf-8')
        else:
            name = raw_name.removesuffix('/')
        if not name or name in seen:
            raise ValueError('Empty or ambiguous archive member name')
        seen.add(name)
        members.append({'name': name, **object_metadata(payload)})
    if not index or not members:
        raise ValueError('Require an archive index and actual Wasm object members')
    return {'bytes': len(data), 'sha256': hashlib.sha256(data).hexdigest(),
            'indexed': index, 'members': members, 'member_names_are_not_paths': True,
            'compiler_or_assembler_qualified': False, 'symbol_extraction_qualified': False}
