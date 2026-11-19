"""Decode the fixture participants' successful loopback UDP transfers."""
from collections import Counter
import hashlib
import os
import re
import struct
from pathlib import Path

DOMAIN = 74
def user_ports(domain):
    base = 7400 + 250 * domain
    return range(base+11,base+32,2)


def submessages(data):
    i = 20
    while i + 4 <= len(data):
        kind, flags = data[i:i + 2]
        length = int.from_bytes(data[i + 2:i + 4], 'little' if flags & 1 else 'big')
        end = i + 4 + length if length else len(data)
        if end > len(data):
            return
        yield kind, data[i + 4:end]
        i = end


HEX = r'(?:\\x[0-9a-f]{2})*'
LOOPBACK = r'sin_addr=inet_addr\("(?:(?:127\.0\.0\.1)|(?:\\x31\\x32\\x37\\x2e\\x30\\x2e\\x30\\x2e\\x31))"\)'


BOUND_UDP = r'<UDP:\[(?:127\.0\.0\.1|0\.0\.0\.0):(\d+)\]>'


def datagrams(lines, *, receiving=False):
    """Yield UDP destination ports and complete RTPS bytes from strace."""
    message, buffer = ('recvmsg', 'recvfrom') if receiving else ('sendmsg', 'sendto')
    for line in lines:
        size = re.search(r' = (\d+)\s*$', line)
        port = re.search(BOUND_UDP if receiving else r'sin_port=htons\((\d+)\)', line)
        if not size or not port or 'AF_INET,' not in line or not re.search(LOOPBACK, line):
            continue
        if message + '(' in line:
            values = re.findall(r'iov_base="('+HEX+')"', line)
        elif buffer + '(' in line:
            match = re.search(buffer + r'\(\d+(?:<[^>]*>)?, "('+HEX+')"', line)
            values = [match[1]] if match else []
        else:
            continue
        payload = bytes.fromhex(''.join(values).replace('\\x', ''))
        if len(payload) == int(size[1]) and len(payload) >= 20 and payload.startswith(b'RTPS'):
            yield int(port[1]), payload


def trace_command(directory, role, argv, *, strace='/usr/bin/strace', receive=False):
    network = ['-yy', '-e', 'trace=sendto,sendmsg,recvfrom,recvmsg'] if receive else ['-e', 'trace=sendto,sendmsg']
    return [strace, '-ff', '-qq', '-xx', '-v', '-s', '65535', *network,
            '-o', directory/(role+'.udp'), *argv]


def app_pid(tracer):
    try:
        children = Path(f'/proc/{tracer.pid}/task/{tracer.pid}/children').read_text().split()
    except (FileNotFoundError, ProcessLookupError):
        return None
    return children[0] if len(children) == 1 else None


def receive_ports(pid, domain=DOMAIN):
    try:
        descriptors = list(Path(f'/proc/{pid}/fd').iterdir())
    except (FileNotFoundError, PermissionError, ProcessLookupError):
        return set()
    inodes = set()
    for fd in descriptors:
        try:
            name = os.readlink(fd)
        except (FileNotFoundError, PermissionError, ProcessLookupError):
            continue
        if name.startswith('socket:['):
            inodes.add(name[8:-1])
    ports = set()
    for line in Path('/proc/net/udp').read_text().splitlines()[1:]:
        fields = line.split()
        if fields[9] in inodes:
            address, port = fields[1].split(':')
            port = int(port,16)
            if address in ('0100007F','00000000') and port in user_ports(domain):
                ports.add(port)
    return ports


def protected_transforms(packet):
    """Report DDS Security AES-GCM submessage envelope identifiers."""
    messages = list(submessages(packet))
    for (kind,prefix), (body_id,body), (postfix,trailer) in zip(messages, messages[1:], messages[2:]):
        if (kind == 0x31 and len(prefix) >= 20 and body_id == 0x30 and len(body) > 4
                and postfix == 0x32 and len(trailer) >= 16 and prefix[:4] in (b'\0\0\0\2', b'\0\0\0\4')):
            yield {2: 'AES128-GCM', 4: 'AES256-GCM'}[prefix[3]]


def summarize(directory, roles, markers, domain=DOMAIN):
    clear, encrypted, total = Counter(), Counter(), Counter()
    identity = hashlib.sha256()
    for role in roles:
        for trace in sorted(directory.glob(role+'.udp.*')):
            with trace.open() as stream:
                for dest, packet in datagrams(stream):
                    identity.update(struct.pack('!HI', dest, len(packet)) + packet)
                    total[role] += 1
                    for label, value in markers.items():
                        if value.encode() in packet:
                            clear[(role,label)] += 1
                    if dest not in user_ports(domain):
                        continue
                    for transform in protected_transforms(packet):
                        encrypted[(role,dest,transform)] += 1
    return {'source':'successful participant UDP sendmsg/sendto syscalls', 'rtpsDatagrams':dict(total),
            'tracedDatagramsSha256':identity.hexdigest(),
            'clearMarkers':{r:{m:clear[(r,m)] for m in markers} for r in roles},
            'traceFiles':{p.name:hashlib.sha256(p.read_bytes()).hexdigest() for p in sorted(directory.glob('*.udp.*'))},
            'userPortEncryptedSubmessages':[{'sender':role, 'destination':port,
                'transform':transform, 'count':count}
                for (role,port,transform),count in sorted(encrypted.items())]}
