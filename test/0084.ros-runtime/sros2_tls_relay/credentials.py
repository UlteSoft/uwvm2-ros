"""Issue temporary test credentials using Jazzy SROS 2 and the system openssl CLI."""
from pathlib import Path
import shutil
import subprocess
import xml.etree.ElementTree as ET


def openssl(*args):
    return subprocess.check_output(['/usr/bin/openssl',*args],stderr=subprocess.PIPE)


def issue_dds(cross, rootfs, stage, output, domain):
    stage.mkdir(mode=0o700)
    output.mkdir()
    root = ET.Element('policy',version='0.2.0')
    enclaves = ET.SubElement(root,'enclaves')
    for role in ('app','bridge'):
        profiles = ET.SubElement(ET.SubElement(enclaves,'enclave',path='/'+role),'profiles')
        profile = ET.SubElement(profiles,'profile',ns='/uwvm_test',node='relay_'+role)
        for mode in ('publish','subscribe'):
            topics = ET.SubElement(profile,'topics',{mode:'ALLOW'})
            ET.SubElement(topics,'topic').text = '/uwvm_test/relay'
            if mode=='subscribe' and domain==74:
                ET.SubElement(topics,'topic').text='/parameter_events'
    ET.ElementTree(root).write(output/'policy.xml',encoding='utf-8',xml_declaration=True)
    cross.generate_artifacts(rootfs,stage,output,domain)
    receipt = {'domain':domain,'policySha256':cross.sha(output/'policy.xml'),'enclaves':{}}
    for role in ('app','bridge'):
        folder = stage/'keystore/enclaves'/role
        permitted = 'rt/uwvm_test/relay'
        def signed(name):
            return ET.fromstring(openssl('smime','-verify','-text','-purpose','any','-in',folder/(name+'.p7s'),
                                         '-CAfile',folder/'permissions_ca.cert.pem'))
        rule, = signed('governance').findall('.//domain_rule')
        topic, = rule.findall('topic_access_rules/topic_rule')
        cross.check(rule.findtext('domains/id') == str(domain) and len(rule.find('domains')) == 1 and
                    rule.findtext('allow_unauthenticated_participants') == 'false' and
                    rule.findtext('enable_join_access_control') == 'true','unexpected domain governance')
        for name,value in [('topic_expression','*'),('enable_read_access_control','true'),('enable_write_access_control','true'),
                           ('metadata_protection_kind','ENCRYPT'),('data_protection_kind','ENCRYPT')]:
            cross.check(topic.findtext(name)==value,'unexpected signed topic protection '+name)
        grant, = signed('permissions').findall('.//grant')
        cross.check(grant.findtext('default') == 'DENY','expected default deny')
        for rule in grant.findall('allow_rule'):
            cross.check(rule.findtext('domains/id')==str(domain) and len(rule.find('domains'))==1,'grant spans domains')
        application_acl={}
        for mode in ('publish','subscribe'):
            names=[t.text for t in grant.findall('allow_rule/'+mode+'/topics/topic')]
            application_acl[mode]=sorted(t for t in names if t.startswith('rt/'))
            expected={permitted}
            if mode=='subscribe' and domain==74: expected.add('rt/parameter_events')
            cross.check(set(application_acl[mode])==expected,'unexpected signed application grant')
        runtime=stage/'runtime/enclaves'/role
        runtime.mkdir(mode=0o700,parents=True)
        for n in ('cert.pem','key.pem','identity_ca.cert.pem','permissions_ca.cert.pem','permissions.p7s','governance.p7s'):
            shutil.copyfile(folder/n,runtime/n)
            (runtime/n).chmod(0o600)
        receipt['enclaves'][role]={n:cross.sha(folder/n) for n in ('cert.pem','identity_ca.cert.pem','permissions_ca.cert.pem',
                                                                   'permissions.p7s','governance.p7s')}
        receipt['enclaves'][role]['applicationTopicGrants']=application_acl
    return receipt


def issue_tls(cross, directory):
    directory.mkdir(mode=0o700)
    ca, key = directory/'ca.pem',directory/'issuer.key'
    def req(*args):
        return openssl('req','-new','-newkey','ec','-pkeyopt','ec_paramgen_curve:P-256','-nodes',*args)
    req('-x509','-days','1','-subj','/CN=Local relay fixture CA','-addext','basicConstraints=critical,CA:TRUE',
        '-addext','keyUsage=critical,keyCertSign,cRLSign','-keyout',key,'-out',ca)
    for name in ('fast','cyclone','untrusted-fast','untrusted-cyclone'):
        path=directory/name
        path.mkdir(mode=0o700)
        endpoint=name.removeprefix('untrusted-')
        host=endpoint+'-bridge.test'
        eku='serverAuth' if endpoint=='fast' else 'clientAuth'
        ext=path/'extensions'
        ext.write_text('basicConstraints=critical,CA:FALSE\nkeyUsage=critical,digitalSignature\n'
                       f'extendedKeyUsage={eku}\nsubjectAltName=DNS:{host}\n')
        if name.startswith('untrusted-'):
            req('-x509','-days','1','-subj','/CN='+host,'-addext','subjectAltName=DNS:'+host,
                '-addext','extendedKeyUsage='+eku,'-keyout',path/'key.pem','-out',path/'cert.pem')
        else:
            req('-subj','/CN='+host,'-keyout',path/'key.pem','-out',path/'leaf.csr')
            openssl('x509','-req','-in',path/'leaf.csr','-CA',ca,'-CAkey',key,'-CAcreateserial','-days','1',
                    '-extfile',ext,'-out',path/'cert.pem')
        shutil.copyfile(ca,path/'ca.pem')
        (path/'key.pem').chmod(0o600)
    return {'caSha256':cross.sha(ca),'leafSha256':{n:cross.sha(directory/n/'cert.pem') for n in ('fast','cyclone','untrusted-fast','untrusted-cyclone')}}
