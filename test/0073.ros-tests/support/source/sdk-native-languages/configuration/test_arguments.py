"""Configuration correctness only; real compiler/Guest qualification is separate."""

from pathlib import Path
import hashlib
import os
import sys
import tempfile
import unittest
selected = os.environ.get('UWVM_NATIVE_LANGUAGES_TOOLS')
if selected:
    production_tools = Path(selected).resolve(strict=True)
else:
    checkout = next((p for p in Path(__file__).resolve().parents
                     if (p / 'sdk/tools/native_languages').is_dir()), None)
    if checkout is None:
        raise ValueError('Set UWVM_NATIVE_LANGUAGES_TOOLS to the selected production helper directory')
    production_tools = checkout / 'sdk/tools/native_languages'
if not (production_tools / 'arguments.py').is_file():
    raise ValueError('Actual selected production argument helper required')
sys.path.insert(0, str(production_tools))
from arguments import validate,expand,zig_tokens

PROFILE=dict(target='wasm32-wasip1',address_bits=32,wasi='wasip1',exceptions=False,threads=False,startup='command',core_baseline='3.0')
OPTIONS=dict(version='1.85.0',backend='ros2',edition='2021',library_mode='core',panic='abort',swift_sdk='exact-sdk')


class Arguments(unittest.TestCase):
    def inspect(self,language,values,options=None,cwd=None):
        return validate(language,PROFILE,options or OPTIONS,values,cwd or Path.cwd())
    def test_verbatim_values(self):
        values=['build','-tags','ros2','-o','/output with spaces/程序.wasm','','@literal package','-ldflags=-s -w']
        original=list(values);self.assertEqual(self.inspect('go',values),{});self.assertEqual(values,original)
    def test_native_target_spellings(self):
        cases={'rust':['--target=wasm32-wasip1','--edition','2021','-Cpanic=abort'],
          'go':['build','-compiler=gc','-buildmode','exe','-tags=ros2'],
          'tinygo':['build','-target=wasi','-scheduler=none','-panic=trap','-tags=ros2'],
          'zig':['build-exe','-target','wasm32-wasi','-mexec-model=command'],
          'swift':['build','--swift-sdk','exact-sdk','--triple','wasm32-unknown-wasi'],
          'haskell':['-O2','Main.hs','-optl-Wl,--gc-sections'],
          'nim':['c','--cpu:wasm32','--os:any','--threads:off','--mm:arc','--exceptions:goto'],
          'd':['-betterC','-mtriple=wasm32-unknown-wasi','-L=--gc-sections'],
          'odin':['build','-target:wasi_wasm32','-thread-count:1','-extra-linker-flags:-L/path with spaces']}
        for language,values in cases.items():
            with self.subTest(language=language):self.assertEqual(self.inspect(language,values),{})
    def test_conflicting_language_selectors(self):
        cases={'rust':['--target=wasm64-wasip1','--edition=2024','-Cpanic=unwind','--cfg=feature="ros1"'],
          'go':['-tags=ros1','-compiler=gccgo','-buildmode=c-shared','-toolexec=/ambient/tool'],
          'tinygo':['-target=wasm','-scheduler=tasks','-panic=print'],
          'zig':['-target=wasm64-wasi','-fno-single-threaded','-mcpu=baseline+atomics'],
          'swift':['--swift-sdk=other-sdk','--triple=x86_64-linux','-sdk=/ambient/sdk'],
          'haskell':['-threaded','-pgmc/ambient/cc','-optl--shared-memory'],
          'nim':['--cpu:amd64','--os:linux','--threads:on','--mm:orc','--passL:--shared-memory'],
          'd':['-mtriple=x86_64-linux','-m64','-L--shared-memory'],
          'odin':['-target:linux_amd64','-thread-count:8','-extra-linker-flags:--shared-memory']}
        for language,values in cases.items():
            for value in values:
                with self.subTest(language=language,value=value):
                    with self.assertRaises(ValueError):self.inspect(language,[value])
    def test_provider_paths_cannot_be_overridden(self):
        for language in ('rust','zig','go','tinygo','swift','haskell','nim','d','odin'):
            with self.subTest(language=language):
                with self.assertRaises(ValueError):self.inspect(language,['--sysroot=/host/lib'])
    def test_rust_response_lines_not_shell_words(self):
        with tempfile.TemporaryDirectory() as directory:
            path=Path(directory)/'space 机器人.rsp'
            raw=b'--target=wasm32-wasip1\r\n--edition=2021\n-C\npanic=abort\n\nargument with spaces\n"quotes are literal"\n@not-recursive\nlast\r'
            path.write_bytes(raw);values=['@'+path.name]
            flat,pins=expand('rust',OPTIONS,values,path.parent)
            self.assertEqual(flat[-5:],['','argument with spaces','"quotes are literal"','@not-recursive','last\r'])
            self.assertEqual(pins[str(path)],{'bytes':len(raw),'sha256':hashlib.sha256(raw).hexdigest()})
            self.assertEqual(self.inspect('rust',values,cwd=path.parent),pins)
            self.assertEqual(values,['@'+path.name])
            path.write_text('--target=wasm64-wasip1\n')
            with self.assertRaises(ValueError):self.inspect('rust',values,cwd=path.parent)
    def test_rust_unstable_grammar_is_not_enabled(self):
        with self.assertRaises(ValueError):self.inspect('rust',['-Zshell-argfiles','@shell:file'])
        with self.assertRaises(ValueError):self.inspect('rust',['--cfg=feature="alloc"'])
    def test_zig_reviewed_response_subset(self):
        self.assertEqual(zig_tokens('a "two words" \'三 四\' ""'),['a','two words','三 四',''])
        with tempfile.TemporaryDirectory() as directory:
            path=Path(directory)/'zig args.rsp';path.write_text('-target wasm32-wasi -mexec-model=command "output with spaces"')
            options=dict(OPTIONS,version='0.14.1')
            self.assertIn(str(path),self.inspect('zig',['@'+path.name],options,cwd=path.parent))
            path.write_text('-target wasm64-wasi')
            with self.assertRaises(ValueError):self.inspect('zig',['@'+path.name],options,cwd=path.parent)
        for text in ('@nested','"bad','a\\b','a #comment'):
            with self.assertRaises(ValueError):zig_tokens(text)
    def test_unqualified_response_grammars_fail(self):
        for language in ('tinygo','swift','haskell','nim','d','odin'):
            with self.subTest(language=language):
                with self.assertRaises(ValueError):self.inspect(language,['@arguments.rsp'])
    def test_missing_option_and_invalid_argv_fail(self):
        for language,value in (('rust','--target'),('go','-tags'),('zig','-target'),('nim','--cpu'),('d','-mtriple'),('odin','-target')):
            with self.subTest(language=language):
                with self.assertRaises(ValueError):self.inspect(language,[value])
        with self.assertRaises(ValueError):self.inspect('go',['NUL\0value'])
        with self.assertRaises(ValueError):self.inspect('go','joined shell string')


if __name__=='__main__':unittest.main(verbosity=2)
