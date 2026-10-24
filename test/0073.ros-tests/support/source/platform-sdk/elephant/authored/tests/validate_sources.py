#!/usr/bin/env python3
"""Read-only full original Elephant graph, exact products and public-symbol audit.

Source hashes are correspondence evidence. They do not establish a signature,
native identity, policy grant, original official test or physical robot effect.
"""
import argparse,hashlib,json,re,xml.etree.ElementTree as ET
from pathlib import Path
PROFILES=['kinetic','melodic','noetic','foxy','galactic','humble']
ROOTS=[24,84,109,47,47,58]
def verify(source):
 s=Path(source).resolve(strict=True);kit=s/'sdk/origin/robots/arms/elephant/mycobot';results=[]
 for profile,expected in zip(PROFILES,ROOTS):
  root=kit/'catalogs'/profile;upstream=kit/'upstream'/profile;lock=json.loads((root/'source-lock.json').read_text());catalog=json.loads((root/'catalog.json').read_text());v=lock['ros_version']
  assert v==(1 if profile in PROFILES[:3] else 2) and lock['profile']==profile and lock['default_authority']==[]
  commit=json.loads((upstream/'commit.json').read_text());tree=json.loads((upstream/'tree.json').read_text());repo=lock['repositories'][0]
  assert repo['commit']==commit['sha'] and repo['tree_git_sha1']==commit['commit']['tree']['sha']==tree['sha'] and not tree.get('truncated')
  blobs={r['path']:r for r in tree['tree'] if r['type']=='blob'}
  originals={n for n in blobs if n.endswith(('.msg','.srv','.action'))}
  assert len(originals)==len(catalog['types'])==len(lock['definitions'])==expected
  assert {r['original_file'] for r in lock['definitions']}==originals
  definitions={r['generation_file']:r for r in lock['definitions']};custom=set();expected_records=set();record_fields={};record_constants={}
  for row in catalog['types']:
   pin=definitions[row['file']];raw=(root/row['file']).read_bytes();blob=blobs[pin['original_file']]
   assert raw==(upstream/'files'/pin['original_file']).read_bytes()
   assert len(raw)==pin['bytes']==blob['size'] and hashlib.sha256(raw).hexdigest()==pin['sha256']
   assert hashlib.sha1(b'blob '+str(len(raw)).encode()+b'\0'+raw).hexdigest()==pin['git_blob_sha1']==blob['sha']
   package=Path(pin['original_file']).parent.parent
   assert ET.fromstring((upstream/'files'/package/'package.xml').read_bytes()).findtext('name')==package.name
   kind=Path(row['file']).parent.name;name=package.name+('/'+kind+'/' if v==2 else '/')+Path(row['file']).stem
   assert row['type']==name and row['ros_version']==v and row['registered'] is True
   custom.add((kind,name));roles={'msg':['message'],'srv':['request','response'],'action':['goal','result','feedback']}[kind]
   sections=re.split(r'^\s*---\s*(?:#[^\n]*)?$',raw.decode(),flags=re.MULTILINE);assert len(sections)==len(roles)
   for role,section in zip(roles,sections):
    record=name if role=='message' else name+('_' if v==2 else '')+role.title();expected_records.add(record);fields=set();constants=set()
    for line in section.splitlines():
     line=line.split('#',1)[0].strip()
     if not line:continue
     constant=re.match(r'^[A-Za-z0-9_]+\s+([A-Za-z0-9_]+)\s*=',line)
     if constant:constants.add(constant.group(1))
     else:fields.add(line.split()[1])
    record_fields[record]=fields;record_constants[record]=constants
  assert custom=={(r['kind'],r['type']) for r in lock['original_custom_roots']}
  gen=s/'sdk/origin/custom-types'/('elephant-mycobot-'+profile+'-complete');receipt=json.loads((gen/'generation.vendor.json').read_text());prep=json.loads((gen/'preparation.vendor.json').read_text());selection=json.loads((gen/'profile-selection.json').read_text())
  sha=lambda path:hashlib.sha256(path.read_bytes()).hexdigest()
  assert selection['profile']==profile and selection['ros_version']==v and selection['default_authority']==[]
  assert selection['source_lock_sha256']==sha(root/'source-lock.json') and selection['source_commit']==repo['commit']
  assert selection['products']==receipt['outputs'] and prep['original_catalog']['sha256']==lock['catalog_sha256']==sha(root/'catalog.json')
  assert set(prep['original_definitions'])==set(definitions)
  for name,pin in definitions.items():assert all(prep['original_definitions'][name][k]==pin[k] for k in ['bytes','sha256'])
  assert {r['type'] for r in receipt['records']}==expected_records
  assert receipt['default_authority']==receipt['implicit_permissions']==[] and receipt['native_registry_emitted'] is False and len(receipt['outputs'])==14
  for name,pin in receipt['outputs'].items():
   p=gen/name;assert not p.is_symlink() and len(Path(name).parts)==1 and p.stat().st_size==pin['bytes'] and sha(p)==pin['sha256']
  bodies={ext:(gen/('bounded_records.'+ext)).read_text() for ext in ['h','hpp','rs','mjs','ts','go']}
  for record in expected_records:
   assert set(receipt['member_map'][record])=={'c','cpp','rust','javascript','assemblyscript','tinygo'}
   assert all(set(mapping)==record_fields[record] for mapping in receipt['member_map'][record].values())
   assert {r['name'] for r in receipt['declared_constants'].get(record,[])}==record_constants[record]
   symbol='Ros'+str(v)+''.join(p[:1].upper()+p[1:] for p in record.split('/'));c='roswasmc_bounded_'+str(v)+'_'+record.replace('/','_')
   markers={'h':['} '+c+';',c+'_encode(',c+'_decode('],'hpp':['using '+symbol+'='+c+';','struct Codec<uwvm_custom::'+symbol+'>'],'rs':['pub struct '+symbol+' {','impl Canonical for '+symbol+' {'],'mjs':['export const '+symbol+'Codec=Object.freeze('],'ts':['export class '+symbol+' {','export class '+symbol+'Codec implements VendorCodec<'+symbol+'>'],'go':['type '+symbol+' struct {','func Decode'+symbol+'(']}
   for ext,names in markers.items():
    for marker in names:assert marker in bodies[ext],(profile,record,ext,marker)
  for src,dst in [('bounded_records.go','records.go'),('bounded_interfaces.go','interfaces.go')]:assert (kit/'examples/go'/('records_'+profile)/dst).read_bytes()==(gen/src).read_bytes()
  results.append(dict(profile=profile,original_definitions=expected,complete_records=len(expected_records),custom_roots=len(custom),constants=sum(len(x) for x in record_constants.values()),products=14))
 return dict(status='PASS',profiles=results,default_authority=[],scope='Complete original source/product/model-domain correspondence only; no native/controller/official-suite acceptance.')
if __name__=='__main__':
 p=argparse.ArgumentParser(description=__doc__);p.add_argument('--source',type=Path,required=True);print(json.dumps(verify(p.parse_args().source),indent=2))
