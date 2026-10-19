#!/usr/bin/env python3
"""Verify unchanged original UR declarations, whole members and exact products.

This read-only source check invokes no generator/compiler/ROS operation and
grants no authority. Git source association is not a signature, Native identity
or model/controller compatibility claim.
"""
import argparse,hashlib,json,re
from pathlib import Path

def verify(source):
    s=Path(source).resolve(strict=True)
    kit=s/'sdk/robots/industrial/universal_robots'
    results=[]
    for variant,expected in [('ros1',(36,56,28,65)),('ros2',(67,108,53,108)),('polyscopex',(43,50,35,64))]:
     root=kit/'catalogs'/variant;catalog=json.loads((root/'catalog.json').read_text());lock=json.loads((root/'source-lock.json').read_text())
     assert len(catalog['types'])==len(lock['definitions'])==expected[0]
     assert hashlib.sha256((root/'catalog.json').read_bytes()).hexdigest()==lock['catalog_sha256'] and lock['default_authority']==[]
     repos={r['id']:r for r in lock['repositories']};trees={}
     for key,row in repos.items():
      source=kit/'upstream'/key;commit=json.loads((source/'commit.json').read_text());tree=json.loads((source/'tree.json').read_text())
      assert row['commit']==commit['sha'] and row['tree_git_sha1']==commit['commit']['tree']['sha']==tree['sha']
      assert not tree.get('truncated');trees[key]={r['path']:r for r in tree['tree'] if r['type']=='blob'}
     declared={r['generation_file']:r for r in lock['definitions']}
     for row in catalog['types']:
      definition=declared[row['file']];body=(root/row['file']).read_bytes();blob=trees[definition['repository']][definition['original_file']]
      assert len(body)==blob['size']==definition['bytes'] and hashlib.sha256(body).hexdigest()==definition['sha256']
      assert hashlib.sha1(b'blob '+str(len(body)).encode()+b'\0'+body).hexdigest()==definition['git_blob_sha1']==blob['sha']
     custom={r['type'] for r in catalog['types'] if r['registered']};assert custom==set(lock['original_custom_roots']) and len(custom)==expected[2]
     vendor_keys={'ros1':['ur_msgs_ros1','driver_ros1'],'ros2':['ur_msgs_inventory','driver_ros2'],'polyscopex':['polyscopex_interfaces']}[variant]
     all_original=set()
     for key in vendor_keys:
      for path in trees[key]:
       if not path.endswith(('.msg','.srv','.action')):continue
       logical=('ur_msgs/'+path) if key.startswith('ur_msgs') else ('urinterfaces/'+path) if key=='polyscopex_interfaces' else path
       bits=logical.split('/');assert len(bits)==3 and bits[1] in ('msg','srv','action')
       name=bits[0]+('/'+bits[1]+'/' if lock['ros_version']==2 else '/')+Path(bits[2]).stem
       all_original.add(name)
     assert custom==all_original, 'Missing or extra complete original custom declaration'
     gen=s/'sdk/custom-types'/('universal-robots-'+variant+'-complete')
     receipt=json.loads((gen/'generation.vendor.json').read_text());prep=json.loads((gen/'preparation.vendor.json').read_text())
     assert len(receipt['records'])==expected[1] and sum(len(v) for v in receipt['declared_constants'].values())==expected[3]
     assert len(receipt['outputs'])==14 and receipt['default_authority']==receipt['implicit_permissions']==[] and not receipt['native_registry_emitted']
     assert prep['original_catalog']['sha256']==lock['catalog_sha256']
     for name,pin in receipt['outputs'].items():
      p=gen/name;assert p.stat().st_size==pin['bytes'] and hashlib.sha256(p.read_bytes()).hexdigest()==pin['sha256']
     # Independently compare every original component's member/constant names
     # to every emitted language map, rather than trusting only a record count.
     for row in catalog['types']:
      roles={'.msg':['message'],'.srv':['request','response'],'.action':['goal','result','feedback']}[Path(row['file']).suffix]
      sections=re.split(r'^\s*---\s*(?:#[^\n]*)?$',(root/row['file']).read_text(),flags=re.MULTILINE)
      assert len(sections)==len(roles)
      for role,section in zip(roles,sections):
       name=row['type'] if role=='message' else row['type']+('_' if lock['ros_version']==2 else '')+role.title()
       fields=set();constants=set()
       for line in section.splitlines():
        line=line.split('#',1)[0].strip()
        if not line:continue
        constant=re.match(r'^[A-Za-z0-9_]+\s+([A-Za-z0-9_]+)\s*=',line)
        if constant:constants.add(constant.group(1));continue
        fields.add(line.split()[1])
       assert all(set(mapping)==fields for mapping in receipt['member_map'][name].values()),name
       assert set(receipt['member_map'][name])=={'c','cpp','rust','javascript','assemblyscript','tinygo'}
       assert {entry['name'] for entry in receipt['declared_constants'].get(name,[])}==constants,name
     assert (kit/'examples/go'/('records_'+variant)/'records.go').read_bytes()==(gen/'bounded_records.go').read_bytes()
     results.append(dict(variant=variant,original_definitions=expected[0],complete_records=expected[1],custom_roots=expected[2],constants=expected[3],products=14))
    
    return dict(status='PASS',variants=results,default_authority=[],
                scope='Whole original interface source and six-language product integrity only; no native or controller acceptance.')

if __name__=='__main__':
    parser=argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--source',type=Path,required=True)
    print(json.dumps(verify(parser.parse_args().source),indent=2))
