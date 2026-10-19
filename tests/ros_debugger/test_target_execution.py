"""Authored parser/policy regressions; these fixtures are never JIT/ROS evidence.

Run only through the Root-owned current64 test supervisor after a reviewed
complete freeze. These tests have not been run by the patch author. Synthetic
ELF bytes exercise parsing/type rejection only; no target or QEMU is executed.
"""
import hashlib
import importlib.util
from pathlib import Path
import struct
import sys
import tempfile
import unittest
sys.dont_write_bytecode=True
# Root's enclosing supervisor binds the complete test/helper source manifest.
# Load the exact sibling explicitly so python -I does not need an ambient path.
_helper=Path(__file__).resolve().with_name('target_execution.py')
_spec=importlib.util.spec_from_file_location('debugger_target_execution_policy',_helper)
adapter=importlib.util.module_from_spec(_spec)
_spec.loader.exec_module(adapter)


def mathematical_pin(path='/selected/qemu'):
    return dict(path=path,bytes=64,sha256='a'*64,device=9,inode=7,mtime_ns=1,ctime_ns=2)


def fixture(architecture='loongarch64',host='x86_64'):
    machine,bits,endian=adapter.ARCHES[architecture]
    prefixes={'arm':'armv7','ppc64le':'powerpc64le','i386':'i686'}
    target=dict(machine=machine,bits=bits,endian=endian,host_machine=host,
        triple=prefixes.get(architecture,architecture)+'-linux-gnu')
    execution=dict(architecture=architecture,mode='qemu',qemu=mathematical_pin(),
        loader=mathematical_pin('/selected/root/lib/loader'),sysroot='/selected/root',environment={})
    return target,execution


class Policy(unittest.TestCase):
    def test_all_eight_architectures_have_distinct_exact_ELF_selection(self):
        self.assertEqual(len(adapter.ARCHES),8)
        for architecture in adapter.ARCHES:
            host='aarch64' if architecture=='x86_64' else 'x86_64'
            target,value=fixture(architecture,host)
            self.assertEqual(adapter.validate_selection(target,value,host),adapter.host_target(host))
    def test_wrong_machine_is_not_an_architecture_label(self):
        target,value=fixture();target['machine']=62
        with self.assertRaises(ValueError):adapter.validate_selection(target,value,'x86_64')
    def test_wrong_endianness_is_refused(self):
        target,value=fixture('s390x');target['endian']='little'
        with self.assertRaises(ValueError):adapter.validate_selection(target,value,'x86_64')
    def test_wrong_word_size_is_refused(self):
        target,value=fixture('arm');target['bits']=64
        with self.assertRaises(ValueError):adapter.validate_selection(target,value,'x86_64')
    def test_wrong_triple_is_refused(self):
        target,value=fixture();target['triple']='riscv64-linux-gnu'
        with self.assertRaises(ValueError):adapter.validate_selection(target,value,'x86_64')
    def test_same_host_route_cannot_be_relabelled_foreign(self):
        target,value=fixture('x86_64')
        with self.assertRaises(ValueError):adapter.validate_selection(target,value,'x86_64')
    def test_missing_QEMU_or_loader_never_falls_back_to_binfmt(self):
        for role in ('qemu','loader'):
            target,value=fixture();value[role]=None
            with self.assertRaises(ValueError):adapter.validate_selection(target,value,'x86_64')
    def test_extra_or_native_execution_mode_is_refused(self):
        for mutation in ('extra','native'):
            target,value=fixture()
            if mutation=='extra':value['extra']=True
            else:value['mode']='native'
            with self.assertRaises(ValueError):adapter.validate_selection(target,value,'x86_64')
    def test_guest_preload_or_ambient_env_is_refused(self):
        for key in ('LD_PRELOAD','PATH','QEMU_CPU'):
            target,value=fixture();value['environment']={key:'/selected/root/lib'}
            with self.assertRaises(ValueError):adapter.validate_selection(target,value,'x86_64')
    def test_empty_guest_library_component_is_refused(self):
        target,value=fixture();value['environment']={'LD_LIBRARY_PATH':'/selected/root/lib:'}
        with self.assertRaises(ValueError):adapter.validate_selection(target,value,'x86_64')
    def test_QEMU_comma_environment_assignment_injection_is_refused(self):
        target,value=fixture();value['environment']={'LD_LIBRARY_PATH':'/selected/lib,LD_PRELOAD=/selected/evil'}
        with self.assertRaises(ValueError):adapter.validate_selection(target,value,'x86_64')
    def test_argv_keeps_target_tail_and_guest_environment_separate(self):
        _,value=fixture();value['environment']={'LD_LIBRARY_PATH':'/selected/root/lib'}
        self.assertEqual(adapter.launch_prefix(value),['/selected/qemu','-L','/selected/root','-U','LD_PRELOAD',
            '-U','LD_LIBRARY_PATH','-E','LD_LIBRARY_PATH=/selected/root/lib'])
    def test_full7_refuses_partial_or_boolean_physical_fields(self):
        for key,value in [('inode',True),('bytes',0),('sha256','x')]:
            pin=mathematical_pin();pin[key]=value
            with self.assertRaises(ValueError):adapter.full_pin(pin)
    def test_guest_absolute_symlink_restarts_at_selected_root(self):
        with tempfile.TemporaryDirectory() as folder:
            root=Path(folder);(root/'lib').mkdir();(root/'lib/loader').write_bytes(b'parser-only')
            (root/'lib64').symlink_to('/lib',target_is_directory=True)
            self.assertEqual(adapter.rooted(root,'/lib64/loader'),root/'lib/loader')
    def test_guest_alias_escape_is_refused(self):
        with tempfile.TemporaryDirectory() as folder:
            root=Path(folder);(root/'escape').symlink_to('../../outside')
            with self.assertRaises(ValueError):adapter.rooted(root,'/escape')
    def test_big_endian_32_and_64_bit_ELF_headers_are_decoded(self):
        def opened(path):
            path=Path(path);info=path.stat();raw=path.read_bytes()
            return dict(path=str(path),bytes=len(raw),sha256=hashlib.sha256(raw).hexdigest(),
                device=info.st_dev,inode=info.st_ino,mtime_ns=info.st_mtime_ns,ctime_ns=info.st_ctime_ns)
        with tempfile.TemporaryDirectory() as folder:
            for bits,machine in ((32,40),(64,22)):
                path=Path(folder)/str(bits);header=bytearray(64);header[:7]=b'\x7fELF'+bytes((1 if bits==32 else 2,2,1))
                struct.pack_into('>HH',header,16,2,machine);path.write_bytes(header)
                value=adapter.elf_image(path,opened)
                self.assertEqual((value['bits'],value['endian'],value['machine']),(bits,'big',machine))
    def test_malformed_program_table_is_refused(self):
        def opened(path):
            info=path.stat();raw=path.read_bytes()
            return dict(path=str(path),bytes=len(raw),sha256=hashlib.sha256(raw).hexdigest(),
                device=info.st_dev,inode=info.st_ino,mtime_ns=info.st_mtime_ns,ctime_ns=info.st_ctime_ns)
        with tempfile.TemporaryDirectory() as folder:
            path=Path(folder)/'bad';header=bytearray(64);header[:7]=b'\x7fELF\x02\x01\x01'
            struct.pack_into('<HH',header,16,2,258);struct.pack_into('<Q',header,32,63);struct.pack_into('<HH',header,54,56,1)
            path.write_bytes(header)
            with self.assertRaises(ValueError):adapter.elf_image(path,opened)
    def test_decoded_descriptor_must_be_the_fully_hashed_inode(self):
        # A declared complete pin for another inode cannot authorize decoding
        # the currently named ELF, even when every subsequent open is stable.
        with tempfile.TemporaryDirectory() as folder:
            root=Path(folder);header=bytearray(64);header[:7]=b'\x7fELF\x02\x01\x01'
            struct.pack_into('<HH',header,16,2,258)
            first=root/'first';second=root/'second';first.write_bytes(header);second.write_bytes(header)
            info=second.stat();pin=dict(path=str(first),bytes=len(header),sha256=hashlib.sha256(header).hexdigest(),
                device=info.st_dev,inode=info.st_ino,mtime_ns=info.st_mtime_ns,ctime_ns=info.st_ctime_ns)
            with self.assertRaisesRegex(ValueError,'descriptor differs'):
                adapter.elf_image(first,lambda _:pin)


if __name__=='__main__':unittest.main()
