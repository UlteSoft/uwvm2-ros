"""Fetch just the pinned public data files of one MuJoCo qualification profile."""
import argparse
import hashlib
import json
from pathlib import Path, PurePosixPath
import re
import tempfile
from urllib import request


class DirectHTTPS(request.HTTPRedirectHandler):
    def redirect_request(self, *args):
        raise ValueError('unexpected redirect from the selected repository')


def relative(value):
    p = PurePosixPath(value)
    if (str(p) != value or not p.parts or
            not all(re.fullmatch(r'[A-Za-z0-9_.-]+', s) and not s.startswith('.') for s in p.parts)):
        raise ValueError('invalid upstream asset path')
    return value


def main():
    args = argparse.ArgumentParser(description=__doc__)
    args.add_argument('--profile', required=True, type=Path)
    args.add_argument('--output', required=True, type=Path)
    opts = args.parse_args()
    profile = json.loads(opts.profile.read_bytes())
    repo = re.fullmatch(r'https://github[.]com/([A-Za-z0-9_-]+)/([A-Za-z0-9_-]+)',profile['repository'])
    commit = profile['commit']
    files = profile['files']
    if not repo or not re.fullmatch(r'[a-f0-9]{40}',commit) or not 1<=len(files)<=1024:
        raise ValueError('expected a pinned public GitHub model profile')
    root = 'https://raw.githubusercontent.com/'+ '/'.join((*repo.groups(),commit))
    if profile['sourceDirectory'] != '.':
        root += '/' + relative(profile['sourceDirectory'])
    output = opts.output.absolute()
    if output.exists() or output.is_symlink() or '.tmp' not in output.resolve().parts:
        args.error('select a new asset directory under the project .tmp')
    output.parent.mkdir(parents=True,exist_ok=True)
    opener = request.build_opener(DirectHTTPS())
    total = 0
    with tempfile.TemporaryDirectory(dir=output.parent) as scratch:
        tree = Path(scratch)/'model'
        tree.mkdir()
        for name,expected in files.items():
            if not re.fullmatch('[a-f0-9]{64}',expected):
                raise ValueError('invalid SHA-256 for '+name)
            path = tree/relative(name)
            path.parent.mkdir(parents=True,exist_ok=True)
            h = hashlib.sha256()
            with opener.open(root+'/'+name, timeout=45) as response, path.open('xb') as writer:
                size = 0
                while chunk := response.read(1<<18):
                    size += len(chunk)
                    total += len(chunk)
                    if size>64<<20 or total>256<<20:
                        raise ValueError('model transfer limit exceeded')
                    h.update(chunk)
                    writer.write(chunk)
            if h.hexdigest()!=expected:
                raise ValueError('model asset digest differs: '+name)
        tree.rename(output)
    print(f'{len(files)} pinned files, {total} bytes, {output}')


if __name__=='__main__':
    main()
