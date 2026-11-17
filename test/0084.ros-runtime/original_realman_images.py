"""Pin an unchanged RealMan ELF and ABI headers for the isolated metadata fixture.

This is a closed test-package resolver, not a general driver installer. Nothing
from the upstream archive is executed during inspection. Native code subsequently
executes only in the production component confinement domain.
"""
import hashlib
import json
from pathlib import Path
import re
import subprocess
import sys
import tarfile
from run_scene_vm import ROOT, sha, verify_gate


def prepare(receipt, output, images):
    verify_gate()
    receipt = Path(receipt).resolve()
    source = json.loads(receipt.read_text())
    identity = source['source']
    assert source['status'] == 'ACQUIRED_AND_SCANNED'
    assert identity['url'] == 'https://github.com/RealManRobot/ros2_rm_robot'
    revision = identity['ref']
    assert re.fullmatch('[0-9a-f]{40}', revision)
    assert re.fullmatch('[0-9a-f]{24}', identity['id'])
    archive = receipt.parent.parent/'archives'/(identity['id']+'.tar.gz')
    assert archive.stat().st_size == source['archive']['bytes'] <= 128*1024*1024
    assert sha(archive) == source['archive']['sha256']
    root = Path(source['source_directory']).resolve()/('ros2_rm_robot-'+revision)
    package = output/'original-driver'
    package.mkdir()
    include = package/'include'
    include.mkdir()
    library = 'rm_driver/lib/linux_x86_c++_v1.1.5/libapi_cpp.so'
    headers = ['rm_driver/include/rm_driver/'+name for name in
               ('rm_interface.h', 'rm_interface_global.h', 'rm_define.h', 'rm_version.h')]
    pins = {str(receipt): sha(receipt), str(archive): sha(archive), str(Path(__file__).resolve()): sha(__file__)}
    extracted = []
    with tarfile.open(archive, 'r:gz') as packed:
        members = packed.getmembers()
        assert len(members) <= 50000
        for relative in [library, *headers]:
            name = root.name+'/'+relative
            matching = [m for m in members if m.name == name]
            assert len(matching) == 1
            member = matching[0]
            assert member.isfile() and 0 < member.size <= 32*1024*1024
            stream = packed.extractfile(member)
            assert stream is not None
            with stream:
                content = stream.read(member.size+1)
            assert len(content) == member.size
            digest = hashlib.sha256(content).hexdigest()
            assert sha(root/relative) == digest
            target = package/'libapi_cpp.so' if relative == library else include/Path(relative).name
            target.write_bytes(content)
            target.chmod(0o500 if relative == library else 0o400)
            pins[str(root/relative)] = pins[str(target)] = digest
            extracted.append(dict(member=name, bytes=len(content), sha256=digest, copied=str(target)))
    sys.path.insert(0, str(ROOT/'tools/ros_scene'))
    from pack_communication_worker import inspect_elf
    abi = inspect_elf((package/'libapi_cpp.so').read_bytes(), 'x86_64')
    images['/worker/libapi_cpp.so'] = package/'libapi_cpp.so'
    allowed = {'libc.so.6', 'libstdc++.so.6', 'libgcc_s.so.1', 'libm.so.6',
               'libdl.so.2', 'libpthread.so.0', 'librt.so.1', 'ld-linux-x86-64.so.2'}
    queue = [package/'libapi_cpp.so']
    checked = set()
    dependencies = []
    while queue:
        path = queue.pop()
        if path in checked:
            continue
        checked.add(path)
        result = subprocess.run(['/usr/bin/readelf', '-W', '-d', '--', str(path)],
                                capture_output=True, text=True, timeout=30, check=True)
        needed = re.findall(r'\(NEEDED\).*Shared library: \[([^\]]+)\]', result.stdout)
        assert set(needed) <= allowed, ('Unapproved vendor dependency', needed)
        dependencies.append(dict(image=str(path), needed=needed, readelf=result.stdout))
        for name in needed:
            selected = next((v for k, v in images.items() if Path(k).name == name), None)
            if selected is None:
                original = Path('/lib/x86_64-linux-gnu')/name
                target = package/name
                content = original.read_bytes()
                assert len(content) <= 32*1024*1024
                inspect_elf(content, 'x86_64')
                target.write_bytes(content)
                target.chmod(0o500)
                pins[str(original)] = pins[str(target)] = sha(target)
                images['/lib/x86_64-linux-gnu/'+name] = target
                selected = target
            queue.append(selected)
    pins[str(ROOT/'tools/ros_scene/pack_communication_worker.py')] = sha(ROOT/'tools/ros_scene/pack_communication_worker.py')
    return include, pins, dict(scope=__doc__, upstream=identity['url'], revision=revision,
        archive_sha256=source['archive']['sha256'], images=extracted, abi=abi, dependencies=dependencies,
        called_api='rm_api_version', motion_or_transport_initialized=False,
        injected_fault_owner='external qualification adapter; original vendor ELF is unchanged')
