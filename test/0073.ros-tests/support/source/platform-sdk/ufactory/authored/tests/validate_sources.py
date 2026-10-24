#!/usr/bin/env python3
"""Read-only original trees, complete transitive source and six-language API audit.

Source correspondence establishes neither native type identity nor authority,
a controller effect, an original official-suite result, or hardware performance.
"""
import argparse,hashlib,json,re,xml.etree.ElementTree as ET
from pathlib import Path
PROFILES=['ros1_kinetic', 'ros1_legacy', 'ros1_master', 'ros2_foxy', 'ros2_galactic', 'ros2_humble_gz', 'ros2_humble', 'ros2_jazzy', 'ros2_master', 'ros2_rolling']
CUSTOM_COUNTS=[41,14,41,55,55,55,55,55,50,55]

def sections(raw,v,kind):
 text=raw.decode()
 if v==1 and kind=='srv':
  before=[];after=[];target=before
  for line in text.split('\n'):
   if line.startswith('---'):target=after
   else:target.append(line+'\n')
  return [''.join(before),''.join(after)]
 return re.split(r'^\s*---\s*(?:#[^\n]*)?$',text,flags=re.MULTILINE)

def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()
def verify(source):
 s=Path(source).resolve(strict=True);kit=s/'sdk/origin/robots/arms/ufactory/xarm';results=[]
 for profile,expected in zip(PROFILES,CUSTOM_COUNTS):
  root=kit/'catalogs'/profile;lock=json.loads((root/'source-lock.json').read_text());catalog=json.loads((root/'catalog.json').read_text());v=lock['ros_version']
  assert v==(1 if profile.startswith('ros1_') else 2) and lock['profile']==profile and lock['default_authority']==[]
  upstream={};trees={}
  for repo in lock['repositories']:
   path=kit/'upstream'/('drivers' if repo['id']==profile else 'core')/repo['id']
   commit=json.loads((path/'commit.json').read_text());tree=json.loads((path/'tree.json').read_text())
   assert repo['commit']==commit['sha'] and repo['tree_git_sha1']==(commit['tree']['sha'] if 'tree' in commit else commit['commit']['tree']['sha'])==tree['sha'] and not tree.get('truncated')
   upstream[repo['id']]=path;trees[repo['id']]={r['path']:r for r in tree['tree'] if r['type']=='blob'}
  original={n for n in trees[profile] if n.endswith(('.msg','.srv','.action'))}
  custom={r['original_file'] for r in lock['definitions'] if r['repository']==profile}
  assert len(original)==expected and original==custom
  definitions={r['generation_file']:r for r in lock['definitions']};assert len(definitions)==len(catalog['types'])
  fields={};constants={};expected_roots=set();native_times=set()
  for row in catalog['types']:
   pin=definitions[row['file']];p=root/row['file'];raw=p.read_bytes();path=upstream[pin['repository']]
   blob=trees[pin['repository']][pin['original_file']]
   assert raw==(path/'files'/pin['original_file']).read_bytes()
   assert len(raw)==pin['bytes']==blob['size'] and sha(p)==pin['sha256']
   assert hashlib.sha1(b'blob '+str(len(raw)).encode()+b'\0'+raw).hexdigest()==pin['git_blob_sha1']==blob['sha']
   original_path=Path(pin['original_file']);kind=original_path.parent.name
   pkg=ET.fromstring((path/'files'/original_path.parent.parent/'package.xml').read_bytes()).findtext('name');assert pkg
   name=pkg+('/'+kind+'/' if v==2 else '/')+original_path.stem
   assert row['type']==pin['type']==name and row['ros_version']==v and row['registered'] is (pin['repository']==profile)
   if pin['repository']==profile:expected_roots.add((kind,name))
   roles={'msg':['message'],'srv':['request','response'],'action':['goal','result','feedback']}[kind];parts=sections(raw,v,kind);assert len(parts)==len(roles)
   for role,part in zip(roles,parts):
    record=name if role=='message' else name+('_' if v==2 else '')+role.title();assert record not in fields
    members=set();consts=set()
    for line in part.splitlines():
     line=line.split('#',1)[0].strip()
     if not line:continue
     constant=re.match(r'^[A-Za-z0-9_]+\s+([A-Za-z0-9_]+)\s*=',line)
     if constant:consts.add(constant.group(1));continue
     typ,member=line.split()[:2];members.add(member)
     if typ in ('time','duration'):native_times.add(typ)
    fields[record]=members;constants[record]=consts
  if v==1:
   for typ in native_times:fields[typ]={'sec','nsec'};constants[typ]=set()
  assert expected_roots=={(r['kind'],r['type']) for r in lock['original_custom_roots']}
  gen=s/'sdk/origin/custom-types'/('ufactory-xarm-'+profile.replace('_','-')+'-complete');receipt=json.loads((gen/'generation.vendor.json').read_text());prep=json.loads((gen/'preparation.vendor.json').read_text());selection=json.loads((gen/'profile-selection.json').read_text())
  assert selection['profile']==profile and selection['ros_version']==v and selection['default_authority']==[]
  assert selection['source_lock_sha256']==sha(root/'source-lock.json')
  assert selection['products']==receipt['outputs'] and prep['original_catalog']['sha256']==lock['catalog_sha256']==sha(root/'catalog.json')
  assert set(prep['original_definitions'])==set(definitions)
  for name,pin in definitions.items():assert all(prep['original_definitions'][name][k]==pin[k] for k in ['bytes','sha256'])
  assert {r['type'] for r in receipt['records']}==set(fields)
  assert receipt['default_authority']==receipt['implicit_permissions']==[] and receipt['native_registry_emitted'] is False and len(receipt['outputs'])==14
  for name,pin in receipt['outputs'].items():
   p=gen/name;assert not p.is_symlink() and len(Path(name).parts)==1 and p.stat().st_size==pin['bytes'] and sha(p)==pin['sha256']
  bodies={ext:(gen/('bounded_records.'+ext)).read_text() for ext in ['h','hpp','rs','mjs','ts','go']}
  for name,members in fields.items():
   assert set(receipt['member_map'][name])=={'c','cpp','rust','javascript','assemblyscript','tinygo'}
   assert all(set(m)==members for m in receipt['member_map'][name].values())
   assert {r['name'] for r in receipt['declared_constants'].get(name,[])}==constants[name]
   symbol='Ros'+str(v)+''.join(p[:1].upper()+p[1:] for p in name.split('/'));c='roswasmc_bounded_'+str(v)+'_'+name.replace('/','_')
   markers={'h':['} '+c+';',c+'_encode(',c+'_decode('],'hpp':['using '+symbol+'='+c+';','struct Codec<uwvm_custom::'+symbol+'>'],'rs':['pub struct '+symbol+' {','impl Canonical for '+symbol+' {'],'mjs':['export const '+symbol+'Codec=Object.freeze('],'ts':['export class '+symbol+' {','export class '+symbol+'Codec implements VendorCodec<'+symbol+'>'],'go':['type '+symbol+' struct {','func Decode'+symbol+'(']}
   for ext,terms in markers.items():
    for term in terms:assert term in bodies[ext],(profile,name,ext,term)
  for src,dst in [('bounded_records.go','records.go'),('bounded_interfaces.go','interfaces.go')]:assert (kit/'examples/go'/('records_'+profile)/dst).read_bytes()==(gen/src).read_bytes()
  results.append(dict(profile=profile,original_custom_definitions=expected,all_original_definitions=len(definitions),complete_records=len(fields),products=14))
 models=json.loads((kit/'models.json').read_text());assert len(models)==55 and len({r['selection'] for r in models})==55
 for r in models:
  assert r['profile'] in PROFILES and r['degrees_of_freedom'] in (5,6,7) and r['original_model_paths']
  for suffix in ('.c','.cc'):assert (kit/'tests'/('contract_'+r['selection']+suffix)).is_file()
  for name in ['telemetry_'+r['selection']+'.mjs','telemetry_'+r['selection']+'.ts','main_'+r['selection']+'.ts','guest_'+r['selection']+'.mjs','javascript-'+r['selection']+'.json']:assert (kit/'examples'/name).is_file()
 return dict(status='PASS',profiles=results,explicit_model_selections=55,default_authority=[],scope='Original source and product correspondence only; no execution/native/controller/official-suite/performance claim.')
if __name__=='__main__':
 p=argparse.ArgumentParser(description=__doc__);p.add_argument('--source',type=Path,required=True);print(json.dumps(verify(p.parse_args().source),indent=2))
