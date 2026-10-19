#!/usr/bin/env python3
"""Read-only verification of full original Clearpath graphs and six-language products.

Source hashes associate bytes with a selected original Git tree. They are not a
signature, native middleware identity, policy grant or physical robot test.
"""
import argparse,hashlib,json,re
from pathlib import Path
def verify(source):
 s=Path(source).resolve(strict=True);kit=s/'sdk/robots/mobile_bases/clearpath/platform';results=[]
 for profile,version,expected in [('noetic',1,(225,371,208,239)),('humble',2,(17,18,14,85)),('jazzy',2,(35,42,32,256)),('main',2,(13,14,10,76))]:
  root=kit/'catalogs'/profile;lock=json.loads((root/'source-lock.json').read_text());catalog=json.loads((root/'catalog.json').read_text())
  assert len(catalog['types'])==len(lock['definitions'])==expected[0] and lock['profile']==profile and lock['ros_version']==version
  assert lock['default_authority']==[] and hashlib.sha256((root/'catalog.json').read_bytes()).hexdigest()==lock['catalog_sha256']
  trees={}
  for repo in lock['repositories']:
   upstream=kit/'upstream'/repo['id'];commit=json.loads((upstream/'commit.json').read_text());tree=json.loads((upstream/'tree.json').read_text())
   assert repo['commit']==commit['sha'] and repo['tree_git_sha1']==commit['commit']['tree']['sha']==tree['sha'] and not tree.get('truncated')
   trees[repo['id']]={r['path']:r for r in tree['tree'] if r['type']=='blob'}
  definitions={r['generation_file']:r for r in lock['definitions']}
  for row in catalog['types']:
   pin=definitions[row['file']];body=(root/row['file']).read_bytes();blob=trees[pin['repository']][pin['original_file']]
   assert len(body)==pin['bytes']==blob['size'] and hashlib.sha256(body).hexdigest()==pin['sha256']
   assert hashlib.sha1(b'blob '+str(len(body)).encode()+b'\0'+body).hexdigest()==pin['git_blob_sha1']==blob['sha']
  vendor_keys=['msgs_'+profile,*(['husky_noetic','jackal_noetic'] if profile=='noetic' else ['driver_'+profile])]
  custom={(row['kind'],row['type']) for row in lock['definitions'] if row['repository'] in vendor_keys}
  assert custom=={(row['kind'],row['type']) for row in lock['original_custom_roots']} and len(custom)==expected[2]
  original=set()
  for key in vendor_keys:
   for path in trees[key]:
    if not path.endswith(('.msg','.srv','.action')):continue
    bits=path.split('/')
    logical=bits[-3:];assert len(logical)==3 and logical[1] in ('msg','srv','action')
    original.add((logical[1],logical[0]+('/'+logical[1]+'/' if version==2 else '/')+Path(logical[2]).stem))
  assert custom==original,'Missing or extra original custom interface'
  empty=set()
  for row in catalog['types']:
   if Path(row['file']).suffix=='.msg' and not any(line.split('#',1)[0].strip() for line in (root/row['file']).read_text().splitlines()):
    empty.add(('msg',row['type']))
  registered={(Path(row['file']).parent.name,row['type']) for row in catalog['types'] if row['registered']}
  assert registered==custom-empty,'Missing or extra operational original root'
  assert empty==({('msg','clearpath_configuration_msgs/PlatformConfig')} if profile=='noetic' else set())
  gen=s/'sdk/custom-types'/('clearpath-'+profile+'-complete')
  receipt=json.loads((gen/'generation.vendor.json').read_text());prep=json.loads((gen/'preparation.vendor.json').read_text());selection=json.loads((gen/'profile-selection.json').read_text())
  assert len(receipt['records'])==expected[1] and sum(map(len,receipt['declared_constants'].values()))==expected[3]
  assert len(receipt['outputs'])==14 and receipt['default_authority']==receipt['implicit_permissions']==[] and not receipt['native_registry_emitted']
  assert selection['profile']==profile and selection['ros_version']==version and selection['default_authority']==[]
  assert selection['source_lock_sha256']==hashlib.sha256((root/'source-lock.json').read_bytes()).hexdigest()
  assert selection['products']==receipt['outputs'] and prep['original_catalog']['sha256']==lock['catalog_sha256']
  assert set(prep['original_definitions'])==set(definitions)
  for name,pin in definitions.items():assert all(prep['original_definitions'][name][k]==pin[k] for k in ('bytes','sha256'))
  for name,pin in receipt['outputs'].items():
   p=gen/name;assert p.stat().st_size==pin['bytes'] and hashlib.sha256(p.read_bytes()).hexdigest()==pin['sha256']
  # Check real declarations as well as field maps and output hashes. A helper
  # rename must never silently alter an original native type's public symbol.
  bodies={ext:(gen/('bounded_records.'+ext)).read_text() for ext in ['h','hpp','rs','mjs','ts','go']}
  for record in receipt['records']:
   native=record['type'];v=record['ros_version']
   symbol='Ros'+str(v)+''.join(part[:1].upper()+part[1:] for part in native.split('/'))
   c_symbol='roswasmc_bounded_'+str(v)+'_'+native.replace('/','_')
   declarations={
    'h':['} '+c_symbol+';',c_symbol+'_encode(',c_symbol+'_decode('],
    'hpp':['using '+symbol+'='+c_symbol+';','struct Codec<uwvm_custom::'+symbol+'>'],
    'rs':['pub struct '+symbol+' {','impl Canonical for '+symbol+' {'],
    'mjs':['export const '+symbol+'Codec=Object.freeze('],
    'ts':['export class '+symbol+' {','export class '+symbol+'Codec implements VendorCodec<'+symbol+'>'],
    'go':['type '+symbol+' struct {','func Decode'+symbol+'('],
   }
   for ext,markers in declarations.items():
    for marker in markers:assert marker in bodies[ext],(profile,native,ext,marker)
  for row in catalog['types']:
   roles={'.msg':['message'],'.srv':['request','response'],'.action':['goal','result','feedback']}[Path(row['file']).suffix]
   sections=re.split(r'^\s*---\s*(?:#[^\n]*)?$',(root/row['file']).read_text(),flags=re.MULTILINE)
   assert len(sections)==len(roles)
   for role,section in zip(roles,sections):
    name=row['type'] if role=='message' else row['type']+('_' if version==2 else '')+role.title()
    fields=set();constants=set()
    for line in section.splitlines():
     line=line.split('#',1)[0].strip()
     if not line:continue
     constant=re.match(r'^[A-Za-z0-9_]+\s+([A-Za-z0-9_]+)\s*=',line)
     if constant:constants.add(constant.group(1));continue
     fields.add(line.split()[1])
    assert set(receipt['member_map'][name])=={'c','cpp','rust','javascript','assemblyscript','tinygo'}
    assert all(set(mapping)==fields for mapping in receipt['member_map'][name].values()),name
    assert {entry['name'] for entry in receipt['declared_constants'].get(name,[])}==constants,name
  for src,dest in [('bounded_records.go','records.go'),('bounded_interfaces.go','interfaces.go')]:
   assert (kit/'examples/go'/('records_'+profile)/dest).read_bytes()==(gen/src).read_bytes()
  results.append(dict(profile=profile,original_definitions=expected[0],complete_records=expected[1],custom_roots=expected[2],constants=expected[3],products=14))
 return dict(status='PASS',profiles=results,default_authority=[],scope='Complete original source and generated six-language products only; no native or controller acceptance.')
if __name__=='__main__':
 parser=argparse.ArgumentParser(description=__doc__);parser.add_argument('--source',type=Path,required=True)
 print(json.dumps(verify(parser.parse_args().source),indent=2))
