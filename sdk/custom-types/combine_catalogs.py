#!/usr/bin/env python3
"""Copy complete official native-bound catalogs into one fresh immutable generator input.

This preparation operation never computes native identities or executes a native
process. The caller retains the original official-export binding receipts; the
combined output pins every definition byte and every source catalog separately.
"""
from pathlib import Path
import argparse
import hashlib
import importlib.util
import json


def combine(inputs,output):
    helper=Path(__file__).resolve().parents[2]/'tests/ros_custom_types/bounded-v2/bind_catalog.py'
    # Only the repository-owned pure reader is selected, never a catalog module.
    # This module imports no launch/controller code or native library.
    spec=importlib.util.spec_from_file_location('_uwvm_bounded_catalog_reader',helper)
    reader=importlib.util.module_from_spec(spec);spec.loader.exec_module(reader)
    records=[];products={};pins=[];seen=set();standard=False
    for index,path in enumerate(inputs):
        raw=reader.read(path,2*1024*1024);catalog=reader.strict(raw)
        if not isinstance(catalog,dict) or set(catalog)!={'format','types'} or catalog['format'] not in ('uwvm-ros-custom-bounded-v2','uwvm-ros-official-standard-bounded-v1') or not isinstance(catalog['types'],list):
            raise ValueError('closed native-bound bounded catalog required')
        standard = standard or catalog['format']=='uwvm-ros-official-standard-bounded-v1'
        pins.append({'path':str(path.resolve()),'bytes':len(raw),'sha256':hashlib.sha256(raw).hexdigest()})
        for row in catalog['types']:
            if not isinstance(row,dict) or type(row.get('ros_version')) is not int or row['ros_version'] not in (1,2) or not isinstance(row.get('type'),str) or not row.get('native_identity'):
                raise ValueError('exact officially bound type/version required')
            key=(row['ros_version'],row['type'])
            if key in seen:raise ValueError('duplicate interface across catalogs')
            seen.add(key);original=Path(row['file'])
            if original.is_absolute() or '..' in original.parts or '.' in original.parts:raise ValueError('closed relative definition path required')
            source=(path.resolve().parent/original).resolve()
            if not source.is_relative_to(path.resolve().parent):raise ValueError('definition escapes catalog root')
            body=reader.read(source,1024*1024);name='inputs/'+str(index)+'/'+str(original)
            products[name]=body;copy=dict(row);copy['file']=name;records.append(copy)
    if not 1<=len(records)<=128:raise ValueError('combined interface ceiling exceeded')
    if output.exists():raise ValueError('select a fresh combined output directory')
    products['catalog.json']=(json.dumps({'format':'uwvm-ros-official-standard-bounded-v1' if standard else 'uwvm-ros-custom-bounded-v2','types':records},indent=2)+'\n').encode()
    receipt={'status':'COMBINED_NATIVE_CATALOG_SOURCE_ONLY','inputs':pins,'outputs':{name:{'bytes':len(value),'sha256':hashlib.sha256(value).hexdigest()} for name,value in products.items()},'native_registration':'NOT_ESTABLISHED','implicit_permissions':[]}
    output.mkdir(parents=True)
    for name,body in products.items():
        path=output/name;path.parent.mkdir(parents=True,exist_ok=True)
        with path.open('xb') as stream:stream.write(body)
    with (output/'combination.json').open('xb') as stream:stream.write((json.dumps(receipt,indent=2)+'\n').encode())
    return receipt


def main():
    parser=argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--input',type=Path,action='append',required=True)
    parser.add_argument('--output',type=Path,required=True)
    args=parser.parse_args();print(json.dumps(combine(args.input,args.output),indent=2))

if __name__=='__main__':main()
