"""Use the live finite owner's actual parent limits in maintained test drivers.

The original parent has the kernel hard64GiB/swap0 bound. Its child does not
have delegated memory controls. Child identity, sampled process/RSS bounds and
the outer per-process rlimits remain separate facts.
"""
import hashlib
import importlib.util
import json
import os
from pathlib import Path
from resource_contract import validate_resource_owner

LIMIT=64<<30
OWNER_SHA256='32045f800a38ada6dd9f66d32fe64138f4c9b4bff84f3b7bfd4b68c69af28144'
_OWNER=None


def require(condition,message):
    if not condition:raise ValueError(message)


def _read(path):return Path(path).read_text().strip()


def _owner(proof):
    global _OWNER
    report=json.loads(Path(proof['evidence']).read_bytes())
    pins=[p for p in report['source_pins']if Path(p['path']).name=='resource_owner.py']
    require(len(pins)==1 and pins[0]['sha256']==OWNER_SHA256,'exact approved finite owner source required')
    path=Path(pins[0]['path']);raw=path.read_bytes()
    require(hashlib.sha256(raw).hexdigest()==OWNER_SHA256,'approved finite owner source changed')
    if _OWNER is None:
        spec=importlib.util.spec_from_file_location('_approved_finite_owner_accounting',path)
        module=importlib.util.module_from_spec(spec);exec(compile(raw,str(path),'exec'),module.__dict__)
        _OWNER=module
    return _OWNER


def bind_arguments(args):
    """Derive omitted pins from an independently validated owner, never the env alone."""
    proof=validate_resource_owner();child=Path(proof['child']);identity=child.stat()
    expected=dict(expected_leaf=child,expected_device=identity.st_dev,expected_inode=identity.st_ino,
        expected_boot=_read('/proc/sys/kernel/random/boot_id'))
    for name,value in expected.items():
        previous=getattr(args,name,None)
        require(previous is None or previous==value,'explicit '+name+' differs from the actual owned child')
        setattr(args,name,value)
    require(args.expected_memory_max==LIMIT and args.expected_swap_max=='0','unchanged hard64GiB/swap0 required')
    require(args.expected_cpus==_read(Path(proof['parent'])/'cpuset.cpus.effective'),'original cpuset changed')
    require(Path(args.out).resolve().is_relative_to(Path(proof['run'])/'work'),'outputs must remain below the active owner work directory')
    _owner(proof)
    return proof


def accounted_memory(parent):
    """Use the exact approved v2 accounting function; retain its raw charge too."""
    require(_OWNER is not None,'live owner accounting has not been bound')
    result=_OWNER.memory_sample(Path(parent))
    require(result['admission_profile']=='conservative-inactive-nonshared-unmapped-clean-file-credit-v1'
        and result['limits_or_cache_modified']is False,'unexpected finite-owner accounting contract')
    return result


def snapshot(args):
    proof=bind_arguments(args);child=Path(proof['child']);parent=Path(proof['parent'])
    require(not(child/'memory.max').exists()and not(child/'memory.swap.max').exists(),
        'approved v2 child must expose inherited parent limits, without delegated memory controls')
    membership=Path('/proc/self/cgroup').read_text().splitlines()
    require(membership==['0::'+str(child).split('/sys/fs/cgroup',1)[1]],'controller left the owned child')
    require(_read(parent/'memory.max')==str(LIMIT)and _read(parent/'memory.swap.max')=='0',
        'original kernel hard64GiB/swap0 changed')
    parent_stat=parent.stat()
    return dict(probe=dict(status='PASS',kind='live-owner-inherited-original-parent-v1',
            effective_memory_max=LIMIT,expected_memory_max=LIMIT,membership=membership),
        expected_memory_max=LIMIT,leaf_memory_max=None,swap_max='0',cpus_effective=args.expected_cpus,
        leaf=str(child),device=args.expected_device,inode=args.expected_inode,boot_id=args.expected_boot,
        limit_owner=str(parent),limit_owner_identity=[parent_stat.st_dev,parent_stat.st_ino],
        resource_limit_kind='original-parent-kernel-limit-with-sampled-owned-child',
        events=_OWNER.numbers(parent/'memory.events'),events_scope='original-shared-parent',
        accounting=accounted_memory(parent),resource_owner=proof)
