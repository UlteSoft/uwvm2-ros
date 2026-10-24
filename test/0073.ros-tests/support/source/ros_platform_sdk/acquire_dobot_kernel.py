#!/usr/bin/env python3
"""Restore the exact missing cross-SDK Linux UAPI dependency as private data.

The operator supplies independently admitted Debian Release/signature/keyring/
Packages.xz inputs. This rechecks the signature and index SHA before downloading
the one exact linux-libc-dev version required by the existing cross packages.
dpkg-deb extracts data into a fresh owned root; no package install, maintainer
script, upstream executable, kernel setting, SDK mutation or ROS process runs.
An independent cgroup owner must bound every descendant and output.
"""
import argparse
import hashlib
import json
import lzma
from pathlib import Path
import re
import subprocess
import urllib.request


def sha(path):
    with path.open('rb') as stream:
        return hashlib.file_digest(stream, 'sha256').hexdigest()


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--metadata', type=Path, required=True)
    parser.add_argument('--out', type=Path, required=True)
    args = parser.parse_args()
    metadata, out = args.metadata.resolve(strict=True), args.out
    assert out.is_absolute() and not out.exists() and out.parent.resolve(strict=True) == out.parent
    out.mkdir(mode=0o700)
    report = dict(status='RUNNING', inputs=[], commands=[], downloads=[])
    def save():
        (out / 'acquisition.json').write_text(json.dumps(report, indent=2) + '\n')
    def run(label, argv, accepted=(0,)):
        with (out / (label + '.stdout')).open('xb') as stdout, (out / (label + '.stderr')).open('xb') as stderr:
            result = subprocess.run(list(map(str, argv)), stdout=stdout, stderr=stderr, timeout=60, check=False)
        report['commands'].append(dict(label=label, argv=list(map(str, argv)), exit=result.returncode,
            stdout_sha256=sha(out / (label + '.stdout')), stderr_sha256=sha(out / (label + '.stderr'))))
        save()
        assert result.returncode in accepted, label
    try:
        for name in ('Release', 'Release.gpg', 'debian-archive-key-13.gpg', 'Packages.xz'):
            p = metadata / name
            assert p.is_file() and not p.is_symlink()
            report['inputs'].append(dict(path=str(p), bytes=p.stat().st_size, sha256=sha(p)))
        # Debian's multi-signature Release also has a rollover signer absent
        # from this explicitly admitted one-key keyring. Exit2 is accepted only
        # with the exact trusted signature below and solely unknown OTHER keys;
        # bad/expired/revoked signatures or missing trusted validation refuse.
        run('release-signature', ['/usr/bin/gpgv', '--status-fd', '1', '--keyring', metadata / 'debian-archive-key-13.gpg',
                                  metadata / 'Release.gpg', metadata / 'Release'], accepted=(0, 2))
        statuses = [line.split() for line in (out / 'release-signature.stdout').read_text().splitlines()
                    if line.startswith('[GNUPG:] ')]
        trusted = [row for row in statuses if row[1] == 'VALIDSIG' and len(row) == 12 and
                   row[2] == 'B8E5F13176D2A7A75220028078DBA3BC47EF2265' and
                   row[-1] == '04B54C3CDCA79751B16BC6B5225629DF75B188BD' and row[9] in ('8','9','10') and row[10] == '00']
        assert len(trusted) == 1
        assert not any(row[1] in ('BADSIG','EXPSIG','EXPKEYSIG','REVKEYSIG','NODATA','FAILURE','ERROR') for row in statuses)
        for row in statuses:
            if row[1] == 'ERRSIG':
                assert len(row) == 9 and row[7] == '9' and row[2] != '78DBA3BC47EF2265'
        report['required_trusted_signature'] = trusted[0]
        release = (metadata / 'Release').read_text()
        section = release.split('\nSHA256:\n', 1)[1].split('\nSHA512:', 1)[0]
        index = metadata / 'Packages.xz'
        candidates = [line.split() for line in section.splitlines() if len(line.split()) == 3]
        expected = [(h, int(n), name) for h, n, name in candidates if h == sha(index) and int(n) == index.stat().st_size]
        assert len(expected) == 1 and expected[0][2].endswith('/binary-amd64/Packages.xz')
        report['signed_index'] = dict(sha256=expected[0][0], bytes=expected[0][1], path=expected[0][2])
        package = None
        with lzma.open(index, 'rt', encoding='utf-8') as stream:
            fields, last = {}, None
            for line in stream:
                if line == '\n':
                    if fields.get('Package') == 'linux-libc-dev' and fields.get('Version') == '7.2.10-1':
                        assert package is None
                        package = fields
                    fields, last = {}, None
                elif line[0] in ' \t':
                    assert last is not None
                    fields[last] += '\n' + line.rstrip('\n')
                else:
                    last, value = line.rstrip('\n').split(': ', 1)
                    assert last not in fields
                    fields[last] = value
        assert package and package['Architecture'] == 'all'
        assert re.fullmatch(r'pool/main/l/linux/linux-libc-dev_7\.2\.10-1_all\.deb', package['Filename'])
        assert re.fullmatch('[0-9a-f]{64}', package['SHA256']) and 0 < int(package['Size']) <= 32 << 20
        report['package'] = package
        archive = out / 'linux-libc-dev_7.2.10-1_all.deb'
        for base in ('https://deb.debian.org/debian/', 'https://ftp.debian.org/debian/'):
            url = base + package['Filename']
            try:
                with urllib.request.urlopen(url, timeout=25) as response:
                    body = response.read(int(package['Size']) + 1)
                assert len(body) == int(package['Size']) and hashlib.sha256(body).hexdigest() == package['SHA256']
                archive.write_bytes(body)
                report['downloads'].append(dict(url=url, bytes=len(body), sha256=package['SHA256'], status='MATCHED_SIGNED_INDEX'))
                break
            except Exception as error:
                report['downloads'].append(dict(url=url, error=repr(error), status='FAILED_RETAINED'))
                save()
        assert archive.is_file(), 'matching original package unavailable; no host header fallback'
        run('extract-data-only', ['/usr/bin/dpkg-deb', '-x', archive, out / 'rootfs'])
        for rel in ('usr/include/linux/limits.h', 'usr/include/linux/types.h', 'usr/include/asm-generic/types.h',
                    'usr/lib/linux/uapi/x86/asm/types.h', 'usr/lib/linux/uapi/arm/asm/types.h'):
            p = out / 'rootfs' / rel
            assert p.is_file(), rel
        for row in report['inputs']:
            p = Path(row['path'])
            assert p.stat().st_size == row['bytes'] and sha(p) == row['sha256']
        report.update(status='MATCHING_SIGNED_CROSS_SDK_KERNEL_UAPI_DATA_RESTORED',
                      target_architectures=['x86', 'arm'], shared_sdk_modified=False,
                      host_libc_or_asm_fallback=False, default_authority=[])
        print(report['status'], flush=True)
    except BaseException as error:
        report.update(status='FAILED_RETAINED', error=repr(error))
        raise
    finally:
        save()


if __name__ == '__main__':
    main()
