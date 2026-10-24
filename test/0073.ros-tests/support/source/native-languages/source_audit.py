#!/usr/bin/env python3
"""Finite-owner source/configuration checks; no native-language/Guest credit."""
from pathlib import Path
import argparse
import hashlib
import importlib
import json
import os
import shutil
import subprocess
import sys
import tarfile


def main():
    p=argparse.ArgumentParser(description=__doc__)
    p.add_argument('--source',type=Path,required=True);p.add_argument('--integration',type=Path,required=True)
    p.add_argument('--xmake',type=Path)
    p.add_argument('--test-artifact-root',type=Path,
                   help='Canonical separate test/0073.ros-tests artifact; no SDK test restoration')
    a=p.parse_args();root=Path.cwd();source=root/'source';source.mkdir();integration=root/'integration';integration.mkdir();rows=[];excluded=[]
    with tarfile.open(a.source) as archive:archive.extractall(source,filter='data')
    with tarfile.open(a.integration) as archive:archive.extractall(integration,filter='data')
    helper=source/'sdk/origin/tools/native_languages'
    if a.test_artifact_root:
        tests = a.test_artifact_root.resolve(strict=True) / 'support/source/sdk-native-languages'
        vector_generator = tests / 'rust/tests/generate_vectors.py'
        configuration = tests / 'configuration'
        if not (tests.parent.parent / 'sdk-test-source-map.json').is_file():
            raise ValueError('Canonical test source map required')
    else:
        # Explicit frozen-archive reproduction remains separate from current
        # canonical-artifact checks; historical bytes are never rewritten.
        vector_generator = source / 'sdk/origin/rust/tests/generate_vectors.py'
        configuration = helper
        if not vector_generator.is_file() or not (configuration / 'test_profile.py').is_file():
            raise ValueError('Current tests require --test-artifact-root')
    handoff=integration/'sdk/origin/coordination/20261021-director/handoffs/03/review02'
    def save():
        (root/'source-audit.json').write_text(json.dumps({'rows':rows,'source_sha256':hashlib.sha256(a.source.read_bytes()).hexdigest(),
            'integration_sha256':hashlib.sha256(a.integration.read_bytes()).hexdigest(),'native_language_compiler_passes':0,
            'Guest_runs':0,'native_ROS_runs':0,'Gateway_runs':0,'excluded_metadata_files':excluded,'default_authority':[]},indent=2)+'\n')
    files=[*helper.glob('*.py'),vector_generator,
        *(source/'test/0073.ros-tests/support/source/native-languages').glob('*.py'),*(handoff/'candidates').glob('*.py')]
    for path in files:
        if path.name.startswith('._'):
            excluded.append(str(path.relative_to(root)));continue
        row={'name':str(path),'layer':'host_Python_source_syntax'}
        try:compile(path.read_bytes(),str(path),'exec');row['status']='PASS'
        except Exception as error:row.update(status='FAIL',error=str(error))
        rows.append(row);save()
    sys.path.insert(0,str(source/'sdk/origin/tools'))
    for name in ('profile','arguments','examples','distribution'):
        row={'name':name,'layer':'actual_production_Python_module_import'}
        try:importlib.import_module('native_languages.'+name);row['status']='PASS'
        except Exception as error:row.update(status='FAIL',error=str(error))
        rows.append(row);save()
    def run(name,argv,cwd=root,layer='configuration_or_patch_check'):
        log=root/(name+'.log')
        environment = dict(os.environ, UWVM_NATIVE_LANGUAGES_TOOLS=str(helper))
        with log.open('wb') as stream:r=subprocess.run([str(x) for x in argv],cwd=cwd,env=environment,stdout=stream,stderr=subprocess.STDOUT,timeout=120)
        rows.append({'name':name,'layer':layer,'argv':[str(x) for x in argv],
            'exit':r.returncode,'status':'PASS' if r.returncode==0 else 'FAIL','log':str(log.relative_to(root))});save();return r.returncode==0
    for name in ('test_profile.py','test_arguments.py'):run(name,[sys.executable,'-I','-S','-B',configuration/name])
    if a.xmake:
        lua_files=[path for path in (source/'sdk/origin').rglob('*.lua') if not path.name.startswith('._')]
        script=root/'lua-syntax.lua'
        script.write_text('local files={'+','.join(json.dumps(str(path),ensure_ascii=False) for path in lua_files)+'}\n'+
            'for _,file in ipairs(files) do local loaded=import(path.basename(file),{rootdir=path.directory(file),anonymous=true});assert(type(loaded)=="table","Actual module import failed") end\n'+
            'print("PASS_NATIVE_LANGUAGES_LUA_MODULE_IMPORT "..#files)\n')
        if run('actual-xmake-lua-import',[a.xmake,'lua',script],layer='actual_xmake_Lua_module_import'):
            marker='PASS_NATIVE_LANGUAGES_LUA_MODULE_IMPORT '+str(len(lua_files))
            if marker not in (root/'actual-xmake-lua-import.log').read_text(errors='replace'):
                rows[-1].update(status='FAIL',reason='actual xmake module-import completion marker missing');save()
    private=root/'patch-source'
    for name,relative in (('wasm_profiles.py','sdk/origin/tools/wasm_profiles.py'),('prepare_distribution.py','sdk/tools/prepare_distribution.py')):
        target=private/relative;target.parent.mkdir(parents=True,exist_ok=True);shutil.copy2(handoff/'before'/name,target)
    for name in ('native-example-action.patch','native-example-exports.patch'):
        patch=handoff/'patches'/name
        if run(name+'-check',['/usr/bin/git','apply','--check',patch],private):run(name+'-apply',['/usr/bin/git','apply',patch],private)
    for name,relative in (('wasm_profiles.py','sdk/origin/tools/wasm_profiles.py'),('prepare_distribution.py','sdk/tools/prepare_distribution.py')):
        same=(private/relative).read_bytes()==(handoff/'candidates'/name).read_bytes()
        rows.append({'name':name+'-exact-postimage','layer':'patch_postimage','status':'PASS' if same else 'FAIL'});save()
    return 1 if any(row['status']=='FAIL' for row in rows) else 0


if __name__=='__main__':sys.exit(main())
