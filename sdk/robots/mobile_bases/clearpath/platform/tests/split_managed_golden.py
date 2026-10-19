"""Split managed tests into bounded compilation units without dropping vectors."""
from pathlib import Path
import argparse,re,json,os
parser=argparse.ArgumentParser(description=__doc__)
parser.add_argument('--inputs',type=Path,required=True)
parser.add_argument('--out',type=Path,required=True)
args=parser.parse_args();inputs=args.inputs.resolve(strict=True);out=args.out
assert out.is_absolute() and not out.exists()
out.mkdir(mode=0o700)
rows=[]
for profile in ['noetic','humble','jazzy','main']:
 base=inputs/profile
 sources={name:(base/name).read_text() for name in ['rust.rs','assemblyscript.ts','tinygo.go','javascript.mjs']}
 functions={}
 patterns={'rust.rs':r'^fn test_(\d+)\(\)\{[^\n]*\}$','assemblyscript.ts':r'^function test_(\d+)\(\):void \{[^\n]*\}$','tinygo.go':r'^func test_(\d+)\(\)\{[^\n]*\}$','javascript.mjs':r'^function test_(\d+)\(\)\{[^\n]*\}$'}
 for name,text in sources.items():functions[name]={int(m[1]):m[0] for m in re.finditer(patterns[name],text,re.MULTILINE)}
 indices=sorted(functions['rust.rs']);assert all(sorted(f)==indices for f in functions.values())
 fields=0
 for first in range(0,len(indices),32):
  chosen=indices[first:first+32];folder=out/(profile+'-%03d'%(first//32));folder.mkdir()
  for name,text in sources.items():
   prefix=text[:re.search(patterns[name],text,re.MULTILINE).start()]
   if name=='assemblyscript.ts':
    def relocate(match):
     original=match[1]
     if not original.startswith('.'):return match[0]
     absolute=(base/original).resolve()
     return 'from "'+os.path.relpath(absolute,folder)+'"'
    prefix=re.sub(r'from "([^"]+)"',relocate,prefix)
   calls=''.join('test_'+str(i)+'();' for i in chosen)
   suffix={'rust.rs':'fn main(){'+calls+'}\n','assemblyscript.ts':'export function _start():void{'+calls+'}\n','tinygo.go':'func main(){'+calls+'}\n','javascript.mjs':'export function main(){'+calls+'return 0;}\n'}[name]
   (folder/name).write_text(prefix+'\n'.join(functions[name][i] for i in chosen)+'\n'+suffix)
  count=sum(sum(1 for expr in re.findall(r'assert!\((.*?)\);',functions['rust.rs'][i]) if '.len()' not in expr)+functions['rust.rs'][i].count('.as_bytes(),&[') for i in chosen)
  fields+=count
  row=dict(id=folder.name,profile=profile,records=len(chosen),original_field_assertions=count,indices=chosen)
  rows.append(row);(folder/'expectations.json').write_text(json.dumps(row,indent=2)+'\n')
 assert fields==json.loads((base/'expectations.json').read_text())['original_field_assertions'],(profile,fields)
(out/'batches.json').write_text(json.dumps(rows,indent=2)+'\n')
print(json.dumps(dict(batches=len(rows),records=sum(r['records'] for r in rows),original_fields=sum(r['original_field_assertions'] for r in rows))))
