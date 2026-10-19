#!/usr/bin/env python3
"""Create source-only profile selection for an exactly prepared Clearpath bundle.

This associates original source bytes and generated products. It neither grants
authority nor creates native middleware identity, signature or test evidence.
"""
import argparse,hashlib,json
from pathlib import Path
def main():
 p=argparse.ArgumentParser(description=__doc__)
 p.add_argument('--kit',type=Path,required=True);p.add_argument('--bundle',type=Path,required=True);p.add_argument('--profile',choices=['noetic','humble','jazzy','main'],required=True)
 a=p.parse_args();kit=a.kit.resolve(strict=True);bundle=a.bundle.resolve(strict=True);root=kit/'catalogs'/a.profile
 lock=json.loads((root/'source-lock.json').read_text());gen=json.loads((bundle/'generation.vendor.json').read_text());prep=json.loads((bundle/'preparation.vendor.json').read_text())
 assert lock['profile']==a.profile and lock['default_authority']==[] and gen['default_authority']==[] 
 sha=lambda path:hashlib.sha256(path.read_bytes()).hexdigest()
 assert prep['original_catalog']['sha256']==lock['catalog_sha256']==sha(root/'catalog.json')
 assert set(prep['original_definitions'])=={x['generation_file'] for x in lock['definitions']}
 for row in lock['definitions']:assert prep['original_definitions'][row['generation_file']]['sha256']==row['sha256']==sha(root/row['generation_file'])
 expected=16 if gen['native_registry_emitted'] else 14
 assert len(gen['outputs'])==expected and prep['mode']==('bind' if gen['native_registry_emitted'] else 'prepare')
 if gen['native_registry_emitted']:
  assert {'manifest.hpp','native_ros'+str(lock['ros_version'])+'.hpp'} <= set(gen['outputs'])
 for name,pin in gen['outputs'].items():
  assert len(Path(name).parts)==1 and not (bundle/name).is_symlink()
  assert (bundle/name).stat().st_size==pin['bytes'] and sha(bundle/name)==pin['sha256']
 repos={r['id']:r for r in lock['repositories']}
 selection=dict(kind='clearpath-explicit-profile-v1',profile=a.profile,ros_version=lock['ros_version'],source_lock_sha256=sha(root/'source-lock.json'),catalog_sha256=sha(root/'catalog.json'),message_commit=repos['msgs_'+a.profile]['commit'],driver_commits={key:repos[key]['commit'] for key in (['husky_noetic','jackal_noetic'] if a.profile=='noetic' else ['driver_'+a.profile])},products=gen['outputs'],default_authority=[])
 with (bundle/'profile-selection.json').open('x') as out:out.write(json.dumps(selection,indent=2)+'\n')
if __name__=='__main__':main()
