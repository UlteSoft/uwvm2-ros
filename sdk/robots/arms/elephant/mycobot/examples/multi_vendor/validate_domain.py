"""Refuse a partial, substituted or conflicting complete multi-vendor domain.
Check exact source/product bytes. Export and bind Native identities separately;
a source catalog, generated codec or matching hash never grants Guest rights.
"""
from pathlib import Path
import argparse,hashlib,json
p=argparse.ArgumentParser(description=__doc__)
p.add_argument('--source',type=Path,required=True);p.add_argument('--generated',type=Path,required=True)
p.add_argument('--catalog',type=Path,required=True)
args=p.parse_args();s=args.source.resolve(strict=True);g=args.generated.resolve(strict=True)
receipt=json.loads((g/'generation.vendor.json').read_text())
assert receipt['default_authority']==[] and receipt['implicit_permissions']==[]
assert len(receipt['records'])==534 and len(receipt['outputs'])==14
for rel,pin in receipt['outputs'].items():
 path=g/rel;assert not path.is_symlink() and path.resolve(strict=True).is_relative_to(g)
 body=path.read_bytes();assert len(body)==pin['bytes'] and hashlib.sha256(body).hexdigest()==pin['sha256']
def read_catalog(path,check=None):
 body=path.read_bytes()
 if check:assert len(body)==check['bytes'] and hashlib.sha256(body).hexdigest()==check['sha256']
 document=json.loads(body);assert document['format']=='uwvm-ros-vendor-complete-bounded-v1'
 result={}
 for row in document['types']:
  relative=Path(row['file']);assert not relative.is_absolute() and '..' not in relative.parts
  physical=path.parent/relative;assert not physical.is_symlink() and physical.resolve(strict=True).is_relative_to(path.parent)
  data=physical.read_bytes()
  key=(row['ros_version'],row['type'],relative.suffix)
  assert key not in result
  result[key]=({k:v for k,v in row.items() if k!='file'},hashlib.sha256(data).hexdigest())
 return result
expected={}
for name in ['sdk/robots/mobile_bases/clearpath/platform/catalogs/noetic',
             'sdk/robots/arms/elephant/mycobot/catalogs/noetic']:
 for key,value in read_catalog(s/name/'catalog.json').items():
  assert key not in expected or expected[key]==value
  expected[key]=value
catalog=args.catalog.resolve(strict=True)
actual=read_catalog(catalog,receipt['source_inputs']['catalog'])
assert len(actual)==334 and actual==expected
for row in receipt['source_inputs']['definitions']:
 relative=Path(row['file']);assert not relative.is_absolute() and '..' not in relative.parts
 body=(catalog.parent/relative).read_bytes()
 assert len(body)==row['bytes'] and hashlib.sha256(body).hexdigest()==row['sha256']
print('Exact complete Clearpath + Elephant Noetic source/product domain matches')
