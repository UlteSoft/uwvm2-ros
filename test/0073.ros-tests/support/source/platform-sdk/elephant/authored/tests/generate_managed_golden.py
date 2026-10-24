"""Create managed data tests from independent original-source C vectors.

This reads literal vectors/assignments already independently packed by the test
tool, not a tested codec's output. Original declarations determine field shape;
the published language map determines SDK spelling. No native identity or ROS
operation is created. Generated assertions inspect decoded named fields before
re-encoding, so inverse encoder/decoder errors cannot hide behind a round trip.
"""
from pathlib import Path
import argparse,json,re,struct,os
parser=argparse.ArgumentParser(description=__doc__)
parser.add_argument('--source',type=Path,required=True)
parser.add_argument('--vectors-root',type=Path,required=True)
parser.add_argument('--out',type=Path,required=True)
args=parser.parse_args();s=args.source.resolve(strict=True);vector_root=args.vectors_root.resolve(strict=True);root=args.out
assert root.is_absolute() and not root.exists()
root.mkdir(mode=0o700)
profiles=['kinetic', 'melodic', 'noetic', 'foxy', 'galactic', 'humble']
def symbol(v,n):return 'Ros'+str(v)+''.join(part[0].upper()+part[1:] for part in n.split('/'))
for profile in profiles:
 v=1 if profile in ('kinetic','melodic','noetic') else 2;catalog_root=s/'sdk/origin/robots/arms/elephant/mycobot/catalogs'/profile
 catalog=json.loads((catalog_root/'catalog.json').read_text());records={}
 for row in catalog['types']:
  suffix=Path(row['file']).suffix;roles={'.msg':['message'],'.srv':['request','response'],'.action':['goal','result','feedback']}[suffix]
  sections=re.split(r'^\s*---\s*(?:#[^\n]*)?$',(catalog_root/row['file']).read_text(),flags=re.MULTILINE)
  for role,section in zip(roles,sections):
   name=row['type'] if role=='message' else row['type']+('_' if v==2 else '')+role.title();fields={}
   for line in section.splitlines():
    line=line.split('#',1)[0].strip()
    if not line or re.match(r'^[A-Za-z0-9_]+\s+[A-Za-z0-9_]+\s*=',line):continue
    match=re.fullmatch(r'([A-Za-z0-9_/]+)(?:<=([0-9]+))?(?:\[([0-9]*|<=[0-9]+)\])?\s+([A-Za-z0-9_]+)(?:\s+.*)?',line);assert match
    typ,_,array,field=match.groups()
    if typ=='Header':typ='std_msgs/Header'
    elif typ not in {'bool','byte','char','int8','uint8','int16','uint16','int32','uint32','int64','uint64','float32','float64','string','time','duration'}:
     bits=typ.split('/')
     if len(bits)==1:typ=name.split('/')[0]+('/msg/' if v==2 else '/')+typ
     elif len(bits)==2 and v==2:typ=bits[0]+'/msg/'+bits[1]
    if typ=='byte':typ='int8' if v==1 else 'uint8'
    if typ=='char':typ='uint8'
    fields[field]=dict(type=typ,array=array)
   records[name]=fields
 if v==1:
  for n in ['time','duration']:records[n]={f:dict(type='uint32' if n=='time' else 'int32',array=None) for f in ['sec','nsec']}
 receipt=json.loads((s/'sdk/origin/custom-types'/('elephant-mycobot-'+profile+'-complete/generation.vendor.json')).read_text())
 vectors=vector_root/('golden-vectors-'+profile);description=json.loads((vectors/'vectors.json').read_text());case_sources={}
 for batch in description['products']:
  text=(vectors/batch['file']).read_text()
  for match in re.finditer(r'static void test_(\d+)\(void\)\{\n(.*?)\n\}\n',text,re.DOTALL):case_sources[int(match[1])]=match[2]
 assert len(case_sources)==description['records']
 def path_info(name,cpath,language):
  tokens=re.findall(r'\.([A-Za-z_][A-Za-z0-9_]*)|\[([0-9]+)\]',cpath[5:]);typ=name;array=None;expr='value';at=0
  while at<len(tokens):
   field,index=tokens[at];at+=1
   if array is not None:
    if not array or array.startswith('<='):
     if field=='size':assert at==len(tokens);return 'length',typ,expr
     assert field=='data';field,index=tokens[at];at+=1
    assert index;expr+='['+index+']';array=None;continue
   if typ=='string':
    assert at==len(tokens) and field in ('size','data');return ('text_length' if field=='size' else 'text'),typ,expr
   assert field and typ in records,(name,cpath,typ,field)
   mapped=receipt['member_map'][typ][language][field]
   expr+=('['+json.dumps(mapped)+']') if language=='javascript' else '.'+mapped
   spec=records[typ][field];typ,array=spec['type'],spec['array']
  return 'scalar',typ,expr
 def numeric(literal,typ):
  if literal in ('true','false'):return literal=='true'
  if typ.startswith('float'):return float(literal.removesuffix('f'))
  if literal=='(-INT64_C(9223372036854775807)-1)':return -(1<<63)
  match=re.fullmatch(r'(-?)U?INT64_C\(([0-9]+)\)',literal)
  if match:return int(match[2])*(-1 if match[1] else 1)
  return int(literal.removesuffix('u'))
 programs={lang:[] for lang in ['rust','assemblyscript','tinygo','javascript']};observations=0
 for row in description['cases']:
  index=row['index'];name=row['type'];sym=symbol(v,name);c=case_sources[index]
  values=re.search(r'expected\[\]=\{([^}]*)\}',c)[1];packet=bytes(int(x,16) if x.startswith('0x') else int(x) for x in values.split(','))[:row['wire_bytes']]
  assertions={lang:[] for lang in programs}
  for match in re.finditer(r'^(value[.A-Za-z0-9_\[\]]+)=([^;\n]+);$',c,re.MULTILINE):
   cpath,literal=match.groups()
   for lang in programs:
    kind,typ,expr=path_info(name,cpath,lang)
    if kind=='text_length':continue
    if kind=='length':
     expression={'rust':expr+'.len()','assemblyscript':expr+'.length','tinygo':'len('+expr+')','javascript':expr+'.length'}[lang]
     condition=expression+'=='+literal
     if lang=='rust':assertions[lang].append('assert!('+condition+');')
     elif lang=='assemblyscript':assertions[lang].append('assert('+condition+');')
     elif lang=='tinygo':assertions[lang].append('if !('+condition+'){panic("sequence length")}')
     else:assertions[lang].append('if(!('+condition+'))throw Error("sequence length");')
     continue
    value=numeric(literal,typ);actual=expr
    if typ.startswith('float'):
     fmt='<f' if typ=='float32' else '<d';bits=int.from_bytes(struct.pack(fmt,value),'little');width=32 if typ=='float32' else 64
     if lang=='rust':condition=expr+'.to_bits()=='+str(bits)+('u32' if width==32 else 'u64')
     elif lang=='assemblyscript':condition='reinterpret<u'+str(width)+'>('+expr+')==u'+str(width)+'(0x'+format(bits,'x')+')'
     elif lang=='tinygo':condition='math.Float'+str(width)+'bits('+expr+')==uint'+str(width)+'(0x'+format(bits,'x')+')'
     else:condition=expr+'==='+repr(value)
    elif typ=='bool':condition=expr+('===' if lang=='javascript' else '==')+('true' if value else 'false')
    elif lang=='assemblyscript':
     bits=int(typ[4:] if typ.startswith('uint') else typ[3:]);mask=(1<<bits)-1
     condition='(u64('+expr+')&u64(0x'+format(mask,'x')+'))==u64(0x'+format(value&mask,'x')+')'
    elif lang=='rust':condition=expr+'=='+str(value)+{'int8':'i8','uint8':'u8','int16':'i16','uint16':'u16','int32':'i32','uint32':'u32','int64':'i64','uint64':'u64'}[typ]
    elif lang=='tinygo':condition=expr+'=='+typ+'('+str(value)+')'
    else:condition=expr+'==='+str(value)+('n' if typ in ('int64','uint64') else '')
    if lang=='rust':assertions[lang].append('assert!('+condition+');')
    elif lang=='assemblyscript':assertions[lang].append('assert('+condition+');')
    elif lang=='tinygo':assertions[lang].append('if !('+condition+'){panic("original field")}')
    else:assertions[lang].append('if(!('+condition+'))throw Error("original field");')
   if path_info(name,cpath,'rust')[0]=='scalar':observations+=1
  for match in re.finditer(r'^memcpy\((value[.A-Za-z0-9_\[\]]+),"((?:\\x[0-9a-f]{2})+)",([0-9]+)\);$',c,re.MULTILINE):
   cpath,literal,length=match.groups();data=bytes.fromhex(literal.replace('\\x',''));assert len(data)==int(length)
   for lang in programs:
    kind,typ,expr=path_info(name,cpath,lang);assert kind=='text' and typ=='string'
    if lang=='rust':assertions[lang].append('assert_eq!('+expr+'.as_bytes(),&['+','.join(str(x) for x in data)+']);')
    elif lang=='assemblyscript':assertions[lang].append('assert('+expr+'=='+json.dumps(data.decode())+');')
    elif lang=='tinygo':assertions[lang].append('if '+expr+'!='+json.dumps(data.decode())+'{panic("original text")}')
    else:assertions[lang].append('if('+expr+'!=='+json.dumps(data.decode())+')throw Error("original text");')
   observations+=1
  octets=','.join(str(x) for x in packet)
  programs['rust'].append('fn test_'+str(index)+'(){let packet:&[u8]=&['+octets+'];let value=r::'+sym+'::decode(packet).unwrap();'+''.join(assertions['rust'])+'let out=value.encode().unwrap();assert_eq!(out.as_slice(),packet);}')
  programs['assemblyscript'].append('function test_'+str(index)+'():void {const original:StaticArray<u8>=['+octets+'];const packet=copy(original);const codec=new r.'+sym+'Codec();const decoded=codec.decodeChecked(packet);assert(decoded.status==0);const value=decoded.value as r.'+sym+';'+''.join(assertions['assemblyscript'])+'const out=codec.encodeChecked(value);assert(out.status==0&&same(out.bytes,packet));}')
  programs['tinygo'].append('func test_'+str(index)+'(){packet:=[]byte{'+octets+'};value,err:=r.Decode'+sym+'(packet);if err!=nil{panic(err)};'+';'.join(assertions['tinygo'])+';out,err:=value.Encode();if err!=nil||!bytes.Equal(out,packet){panic("canonical bytes")}}')
  programs['javascript'].append('function test_'+str(index)+'(){const packet=new Uint8Array(['+octets+']);const value=r.'+sym+'Codec.decode(packet);'+''.join(assertions['javascript'])+'const out=r.'+sym+'Codec.encode(value);if(out.length!==packet.length||out.some((x,i)=>x!==packet[i]))throw Error("canonical bytes");}')
 assert observations==description['scalar_or_string_field_instances'],(profile,observations)
 calls=''.join('test_'+str(row['index'])+'();' for row in description['cases'])
 dest=root/profile;dest.mkdir()
 rust='#[path="'+str(s/'sdk/origin/custom-types'/('elephant-mycobot-'+profile+'-complete/bounded_records.rs'))+'"] mod r;\nuse roswasm::typed::Canonical;\n'+'\n'.join(programs['rust'])+'\nfn main(){'+calls+'}\n'
 (dest/'rust.rs').write_text(rust)
 assembly='export {abortGuest} from "'+os.path.relpath(s/'sdk/origin/assemblyscript/api',dest)+'";\nimport * as r from "'+os.path.relpath(s/'sdk/origin/custom-types'/('elephant-mycobot-'+profile+'-complete/bounded_records'),dest)+'";\nfunction copy(a:StaticArray<u8>):Uint8Array{const b=new Uint8Array(a.length);for(let i=0;i<a.length;++i)b[i]=a[i];return b;}\nfunction same(a:Uint8Array,b:Uint8Array):bool{if(a.length!=b.length)return false;for(let i=0;i<a.length;++i)if(a[i]!=b[i])return false;return true;}\n'+'\n'.join(programs['assemblyscript'])+'\nexport function _start():void{'+calls+'}\n'
 (dest/'assemblyscript.ts').write_text(assembly)
 go='package main\nimport ("bytes";"math";r "uwvm2.local/elephant-mycobot-golden/records")\nvar _=math.Float32bits\n'+'\n'.join(programs['tinygo'])+'\nfunc main(){'+calls+'}\n'
 (dest/'tinygo.go').write_text(go)
 javascript='import * as r from "sdk/origin/custom-types/elephant-mycobot-'+profile+'-complete/bounded_records.mjs";\n'+'\n'.join(programs['javascript'])+'\nexport function main(){'+calls+'return 0;}\n'
 (dest/'javascript.mjs').write_text(javascript)
 (dest/'expectations.json').write_text(json.dumps(dict(records=len(description['cases']),original_field_assertions=observations,default_authority=[],scope='Managed tests of the independently packed original-source vectors; no execution or native identity inferred.'),indent=2)+'\n')
 print(json.dumps(dict(profile=profile,records=len(description['cases']),original_fields=observations)),flush=True)
