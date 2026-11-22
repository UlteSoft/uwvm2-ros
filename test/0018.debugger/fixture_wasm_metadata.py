"""Current metadata wire grammar for protocol-only fake modules.

The counts are explicit fixture declarations, not invented runtime discovery.
Native integration tests remain responsible for the real inventory and owners.
"""

def metadata(command, *, stop, epoch, functions, globals=0, tables=0, memories=0, thread=1, names=None):
    words = command.split()
    if not words or words[0] != "wasm-meta":
        return None
    assert len(words) == 7, command
    _, participant, selected_stop, module, kind, first, count = words
    assert int(participant) == thread and int(selected_stop) == stop, command
    family = {"summary": 1, "functions": 2, "globals": 3, "tables": 4, "memories": 5}[kind]
    first, count = int(first), int(count)
    total = {"summary": 0, "functions": functions, "globals": globals, "tables": tables, "memories": memories}[kind]
    assert first <= total and count > 0, command
    end = min(total, first + count)
    text = (f"wasm-meta thread={thread} stop={stop} module={module} epoch={epoch} kind={family} "
            f"functions={functions} globals={globals} tables={tables} memories={memories} bits=32 "
            f"first={first} count={end-first}\n")
    for index in range(first, end):
        name = (names or {}).get((kind, index), "").encode().hex() or "-"
        text += f"wasm-item index={index} imported=0 bits={32 if kind == 'memories' else 0} name={name}\n"
    return text


def code(command, *, stop, epoch, function, pc=4, module=0, thread=1, generation=1):
    """A finite nop/end function for formatter tests; no runtime authority."""
    if not command.startswith("wasm-code "): return None
    words = command.split()
    assert len(words) == 6 and int(words[1]) == thread and int(words[2]) == stop, command
    count, byte_offset, first = map(int, words[3:])
    assert 0 <= count <= 64, command
    base = pc + byte_offset
    assert 0 <= base <= pc, command
    size = pc + 1
    result = f"wasm-code thread={thread} stop={stop} module={module} function={function} epoch={epoch} generation={generation} pc={pc} size={size} first={first} count={count}\n"
    for index in range(base + first, base + first + count):
        result += (f"wasm-op offset={index} size=1 safe=1 text={'end' if index == pc else 'nop'}\n" if 0 <= index < size else "wasm-op unavailable\n")
    return result
