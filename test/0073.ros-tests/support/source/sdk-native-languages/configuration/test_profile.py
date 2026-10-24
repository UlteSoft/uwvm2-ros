"""Pure native-adapter correctness tests; no Guest/compiler acceptance credit."""

import copy
import os
from pathlib import Path
import sys
import tempfile
import unittest
# The canonical test artifact is independent of the production SDK. An explicit
# selected helper root also works after install/relocation, without a checkout.
selected = os.environ.get('UWVM_NATIVE_LANGUAGES_TOOLS')
if selected:
    production_tools = Path(selected).resolve(strict=True)
else:
    checkout = next((p for p in Path(__file__).resolve().parents
                     if (p / 'sdk/tools/native_languages').is_dir()), None)
    if checkout is None:
        raise ValueError('Set UWVM_NATIVE_LANGUAGES_TOOLS to the selected production helper directory')
    production_tools = checkout / 'sdk/tools/native_languages'
if not (production_tools / 'profile.py').is_file():
    raise ValueError('Actual selected production profile helper required')
sys.path.insert(0, str(production_tools))
import profile as native


class Profiles(unittest.TestCase):
    def setUp(self):
        self.resolved={'format':'uwvm-sdk-resolved-profile-v1',
          'profile':{'target':'wasm32-wasip1','address_bits':32,'wasi':'wasip1','exceptions':False,'threads':False,'startup':'command','core_baseline':'3.0'},
          'compiler':'/pinned toolchain/机器人/bin/go','linker':None,'sysroot':'/sdk/runtime',
          'resource_dir':'','compiler_argv':[],'linker_argv':[],
          'library_abi':{'linker_kind':'compiler-internal'},'default_authority':[]}
        self.rust={'version':'1.85.0','edition':'2021','library_mode':'core','panic':'abort','backend':'ros2'}
    def test_rust_editions_are_independent(self):
        for edition in ('2015','2018','2021','2024'):
            for mode in ('core','alloc','std'):
                with self.subTest(edition=edition,mode=mode):
                    options=dict(self.rust,edition=edition,library_mode=mode)
                    native.validate(self.resolved,'rust',options)
    def test_dimensions_refuse_without_downgrade(self):
        for key,value in [('target','wasm64-wasip1'),('address_bits',64),('wasi','wasip2'),('exceptions',True),('threads',True),('exceptions','false'),('core_baseline','2.0')]:
            with self.subTest(key=key,value=value):
                selected=copy.deepcopy(self.resolved);selected['profile'][key]=value
                with self.assertRaises(ValueError):native.validate(selected,'rust',self.rust)
    def test_no_silent_panic_or_edition_change(self):
        for change in ({'panic':'unwind'},{'edition':'2013'},{'library_mode':'everything'},{'backend':'implicit'}):
            with self.subTest(change=change):
                with self.assertRaises(ValueError):native.validate(self.resolved,'rust',dict(self.rust,**change))
    def test_argv_plan_preserves_values(self):
        options={'version':'1.24.2','backend':'ros2'}
        extra=['','argument with spaces','Unicode 机器人','@literal response.rsp','-Wl,--gc-sections']
        command=native.argv(self.resolved,'go',options,'/project with spaces/main.go','/output/程序.wasm',extra)
        self.assertEqual(command[0],self.resolved['compiler'])
        self.assertEqual(command[-len(extra):],extra)
        self.assertIn('/project with spaces/main.go',command)
        self.assertEqual(native.environment('go',self.resolved)['GOROOT'],'/pinned toolchain/机器人')
    def test_non_rust_backend_and_reactor_refusal(self):
        for backend in ('none','both'):
            with self.assertRaises(ValueError):native.validate(self.resolved,'go',{'version':'1.24.2','backend':backend})
        selected=copy.deepcopy(self.resolved);selected['profile']['startup']='reactor'
        with self.assertRaises(ValueError):native.argv(selected,'go',{'version':'1.24.2','backend':'ros2'},'main.go','guest.wasm')
    def test_cargo_preserves_frozen_manifest_edition(self):
        selected=copy.deepcopy(self.resolved);selected['language_options']=self.rust
        selected['build_tools']={'cargo':{'path':'/pinned/cargo'}}
        with tempfile.TemporaryDirectory() as directory:
            p=Path(directory)/'Cargo.toml';p.write_text('[package]\nname="probe"\nversion="0.1.0"\nedition="2021"\n')
            with self.assertRaises(ValueError):native.cargo_arguments(selected,p,str(Path(directory)/'out'))
            (p.parent/'Cargo.lock').write_text('version=3\n')
            command=native.cargo_arguments(selected,p,str(Path(directory)/'output with spaces'))
            self.assertIn('--frozen',command);self.assertIn('--offline',command)
            self.assertEqual(command[command.index('--target')+1],'wasm32-wasip1')
            self.assertNotIn('--edition',command)
            selected['language_options']=dict(self.rust,edition='2015')
            with self.assertRaises(ValueError):native.cargo_arguments(selected,p,str(Path(directory)/'out'))
    def test_cargo_flag_array_keeps_spaces_and_rejects_conflicts(self):
        selected=copy.deepcopy(self.resolved);selected['language_options']=self.rust
        selected['compiler_argv']=['--target=wasm32-wasip1','--edition=2021','-C','linker=/path with spaces/wasm-ld']
        env=native.cargo_environment(selected)
        self.assertEqual(env['CARGO_ENCODED_RUSTFLAGS'].split('\x1f'),['-C','linker=/path with spaces/wasm-ld','-C','panic=abort'])
        selected['compiler_argv']=['--target=wasm64-wasip1']
        with self.assertRaises(ValueError):native.cargo_environment(selected)


if __name__=='__main__':unittest.main(verbosity=2)
