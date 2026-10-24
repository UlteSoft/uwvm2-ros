#!/usr/bin/env python3
"""Admit immutable official ROS package XML schemas for offline native tests.

Run under owner_flexiv. Downloaded XML is data, never executable code. Verify
the original commit/tree/blob correspondence before creating a fresh private
catalog. The original package.xml, test registration and network isolation stay
unchanged. This operator resource creates no Guest right or native endpoint.
"""
import argparse
import hashlib
import json
from pathlib import Path
import xml.etree.ElementTree as ET

COMMIT = '11ca24a41f31480dfb9562ba99f2a5b93d3ebda5'
TREE = 'eae87fb0a3cebe51ade75010a419c0f63052829b'
NAMES = ('package_format3.xsd','package_common.xsd')


def main():
    p = argparse.ArgumentParser(description=__doc__)
    p.add_argument('--root',type=Path,required=True)
    p.add_argument('--out',type=Path,required=True)
    a = p.parse_args()
    root,out = a.root.resolve(strict=True),a.out
    mirror = root/'mirror/schema'
    commit = json.loads((mirror/'commit.json').read_bytes())
    tree = json.loads((mirror/'tree.json').read_bytes())
    assert commit['sha']==COMMIT and commit['commit']['tree']['sha']==tree['sha']==TREE
    assert not tree.get('truncated')
    entries = {r['path']:r for r in tree['tree']if r['type']=='blob'}
    dest = root/'tools/ros-schema-xsd'
    assert not dest.exists() and out.is_absolute() and out.parent==root/'out' and not out.exists()
    bodies,rows = {},[]
    for name in NAMES:
        original = 'xsd/'+name
        body = (mirror/name).read_bytes()
        row = entries[original]
        assert row['mode']=='100644' and len(body)==row['size'] and len(body)<65536
        assert hashlib.sha1(b'blob '+str(len(body)).encode()+b'\0'+body).hexdigest()==row['sha']
        parsed = ET.fromstring(body)
        assert parsed.tag=='{http://www.w3.org/2001/XMLSchema}schema'
        for child in parsed.findall('{http://www.w3.org/2001/XMLSchema}include'):
            assert child.attrib['schemaLocation']in NAMES
        assert not parsed.findall('{http://www.w3.org/2001/XMLSchema}import')
        bodies[name] = body
        rows.append(dict(name=name,bytes=len(body),sha256=hashlib.sha256(body).hexdigest(),
            git_blob_sha1=row['sha'],url='https://raw.githubusercontent.com/ros-infrastructure/rep/'+COMMIT+'/'+original))
    dest.mkdir(mode=0o700)
    for name,body in bodies.items():
        with (dest/name).open('xb')as stream:
            stream.write(body)
    ns = 'urn:oasis:names:tc:entity:xmlns:xml:catalog'
    ET.register_namespace('',ns)
    catalog = ET.Element('{'+ns+'}catalog',{'prefer':'system'})
    for name in NAMES:
        for protocol in ('http','https'):
            url = protocol+'://download.ros.org/schema/'+name
            local = 'file:///tmp/tools/ros-schema-xsd/'+name
            ET.SubElement(catalog,'{'+ns+'}system',{'systemId':url,'uri':local})
            ET.SubElement(catalog,'{'+ns+'}uri',{'name':url,'uri':local})
    body = ET.tostring(catalog,encoding='utf-8',xml_declaration=True)+b'\n'
    with (dest/'catalog.xml').open('xb')as stream:
        stream.write(body)
    report = dict(status='PINNED_OFFICIAL_ROS_XML_SCHEMAS_ADMITTED',repository='https://github.com/ros-infrastructure/rep',
        commit=COMMIT,tree_git_sha1=TREE,schemas=rows,catalog_sha256=hashlib.sha256(body).hexdigest(),
        default_authority=[],scope='Offline data resource for unchanged original native package tests only')
    out.mkdir(mode=0o700)
    (out/'schema.json').write_text(json.dumps(report,indent=2)+'\n')
    print(report['status'],flush=True)


if __name__=='__main__':
    main()
