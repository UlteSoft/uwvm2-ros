#!/usr/bin/env python3
"""Check actual ARM soft-float and big-endian canonical C data under user QEMU.

These freestanding Linux ELF programs use the shipped C codec and unchanged
independent field vectors. A tiny test-only memory/assert/exit support file
avoids substituting little-endian libc archives for unavailable big-endian
ones. This does not qualify libc, C++ standard libraries, native ROS, a Runtime,
vendor drivers or an officially supported ROS distribution on these targets.
Every compiler/emulator child is bounded by the independent Linux owner.
"""
import argparse
import hashlib
import json
from pathlib import Path
import subprocess


SUPPORT = r'''/* Test-only Linux process support; no production SDK or ROS implementation. */
#include <stddef.h>
__attribute__((noreturn)) static void terminate(int code) {
#if defined(__aarch64__)
  register long result __asm__("x0")=code;
  register long syscall __asm__("x8")=93;
  __asm__ volatile("svc 0"::"r"(result),"r"(syscall):"memory");
#elif defined(__arm__)
  register long result __asm__("r0")=code;
  register long syscall __asm__("r7")=1;
  __asm__ volatile("svc 0"::"r"(result),"r"(syscall):"memory");
#else
#error Explicit ARM test target required
#endif
  __builtin_unreachable();
}
void test_assert_fail(void) { terminate(97); }
void *memcpy(void *out, void const *in, size_t n) {
  unsigned char *d=out;unsigned char const *s=in;
  for(size_t i=0;i<n;++i)d[i]=s[i];return out;
}
void *memset(void *out,int value,size_t n) {
  unsigned char *d=out;for(size_t i=0;i<n;++i)d[i]=(unsigned char)value;return out;
}
int memcmp(void const *a,void const *b,size_t n) {
  unsigned char const *x=a,*y=b;
  for(size_t i=0;i<n;++i)if(x[i]!=y[i])return x[i]<y[i]?-1:1;return 0;
}
#if defined(__arm__)
void __aeabi_memcpy(void *a,void const *b,size_t n){memcpy(a,b,n);}
void __aeabi_memcpy4(void *a,void const *b,size_t n){memcpy(a,b,n);}
void __aeabi_memcpy8(void *a,void const *b,size_t n){memcpy(a,b,n);}
void __aeabi_memset(void *a,size_t n,int value){memset(a,value,n);}
void __aeabi_memset4(void *a,size_t n,int value){memset(a,value,n);}
void __aeabi_memset8(void *a,size_t n,int value){memset(a,value,n);}
void __aeabi_memclr(void *a,size_t n){memset(a,0,n);}
void __aeabi_memclr4(void *a,size_t n){memset(a,0,n);}
void __aeabi_memclr8(void *a,size_t n){memset(a,0,n);}
#endif
extern int main(void);
__attribute__((noreturn)) void _start(void) { terminate(main()); }
'''


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--root', type=Path, required=True)
    parser.add_argument('--out', type=Path, required=True)
    args = parser.parse_args()
    root, out = args.root.resolve(strict=True), args.out
    inputs = json.loads((root / 'inputs.json').read_bytes())
    source = Path(inputs['source_root'])
    vendor = inputs['selected_vendor']
    assert vendor in ('agibot', 'deep_robotics', 'fourier', 'pal', 'trossen', 'ubtech')
    kit = source / 'sdk/origin/robots/platforms' / vendor
    direct = Path(inputs['selected_direct_result'])
    qualification = json.loads((direct / 'qualification.json').read_bytes())
    assert qualification['vendor'] == vendor
    assert qualification['status'] == 'SELECTED_VENDOR_C_CPP_COMPLETE_DATA_AND_DEFAULT_REFUSAL_PASS'
    assert out.parent == root / 'out' and not out.exists()
    out.mkdir(mode=0o700)
    (out / 'commands').mkdir()
    headers = out / 'test-support'
    headers.mkdir()
    (headers / 'support.c').write_text(SUPPORT)
    (headers / 'assert.h').write_text('#ifndef TEST_ASSERT_H\n#define TEST_ASSERT_H\n'
        'void test_assert_fail(void);\n#define assert(x) ((x)?(void)0:test_assert_fail())\n#endif\n')
    (headers / 'string.h').write_text('#ifndef TEST_STRING_H\n#define TEST_STRING_H\n#include <stddef.h>\n'
        'void *memcpy(void *,void const *,size_t);\nvoid *memset(void *,int,size_t);\n'
        'int memcmp(void const *,void const *,size_t);\n#endif\n')
    report = dict(status='RUNNING', vendor=vendor, commands=[], targets=[],
                  default_authority=[], original_official_cases_run=0,
                  scope='Freestanding shipped C codec and independent vector ELF programs only; no target libc/C++/Runtime/native ROS claim')

    def save():
        (out / 'endian.json').write_text(json.dumps(report, indent=2) + '\n')

    def pin(path):
        with path.open('rb') as stream:
            digest = hashlib.file_digest(stream, 'sha256').hexdigest()
        return dict(path=str(path), bytes=path.stat().st_size, sha256=digest)

    def run(label, argv):
        directory = out / 'commands' / label
        directory.mkdir()
        with (directory / 'stdout').open('xb') as stdout, (directory / 'stderr').open('xb') as stderr:
            result = subprocess.run(list(map(str, argv)), cwd=source, stdout=stdout,
                                    stderr=stderr, timeout=180, check=False)
        report['commands'].append(dict(label=label, argv=list(map(str, argv)), exit=result.returncode,
            stdout=pin(directory / 'stdout'), stderr=pin(directory / 'stderr')))
        save()
        assert result.returncode == 0, (label, (directory / 'stderr').read_text()[-2600:])

    matrix = [('armel', 'arm-linux-gnueabi', 'qemu-arm', ['-march=armv7-a', '-mfloat-abi=soft'], 1, 1),
              ('armeb', 'armeb-linux-gnueabi', 'qemu-armeb', ['-march=armv7-a', '-mfloat-abi=soft'], 2, 1),
              ('armebhf', 'armeb-linux-gnueabihf', 'qemu-armeb', ['-march=armv7-a', '-mfpu=vfpv3-d16', '-mfloat-abi=hard'], 2, 1),
              ('aarch64be', 'aarch64_be-linux-gnu', 'qemu-aarch64_be', [], 2, 2)]
    try:
        for arch, triple, qemu, extra, order, elf_class in matrix:
            emulator = Path('/home/macromodel/.local/opt/qemu-10.2.1/usr/bin') / qemu
            assert emulator.is_file()
            row = dict(architecture=arch, target=triple, emulator=pin(emulator),
                       contracts=0, independent_vector_programs=0, artifacts=[])
            report['targets'].append(row)
            flags = ['--target=' + triple, *extra, '-fuse-ld=/usr/bin/ld.lld-21', '-std=c11',
                     '-ffreestanding', '-fno-builtin', '-fno-stack-protector', '-fno-unwind-tables',
                     '-fno-asynchronous-unwind-tables', '-nostdlib', '-static', '-O1',
                     '-ffunction-sections', '-fdata-sections', '-Wl,--threads=1',
                     '-Wl,--gc-sections', '-Wl,-e,_start', '-I' + str(headers)]
            for index, profile in enumerate(json.loads((kit / 'profiles.json').read_bytes())['profiles'], 1):
                key = profile['id']
                family = source / 'sdk/origin/custom-types' / profile['family']
                vectors = direct / ('golden-vectors-' + key)
                description = json.loads((vectors / 'vectors.json').read_bytes())
                products = [('contract', kit / ('tests/contract_' + key + '.c'))]
                products += [('golden-' + str(n), vectors / product['file'])
                             for n, product in enumerate(description['products'])]
                for kind, program in products:
                    label = arch + '-' + key + '-' + kind
                    artifact = out / label
                    run(label + '-build', ['/usr/bin/clang-21', *flags,
                        '-I' + str(source / 'sdk/origin/include'), '-I' + str(family), '-I' + str(kit / 'include'),
                        '-DUWVM_' + vendor.upper() + '_PROFILE=' + str(index),
                        program, headers / 'support.c', '-o', artifact])
                    with artifact.open('rb') as stream:
                        identity = stream.read(64)
                    assert identity[:4] == b'\x7fELF' and identity[4] == elf_class and identity[5] == order
                    byte_order = 'little' if order == 1 else 'big'
                    machine = int.from_bytes(identity[18:20], byte_order)
                    flags_offset = 36 if elf_class == 1 else 48
                    elf_flags = int.from_bytes(identity[flags_offset:flags_offset + 4], byte_order)
                    assert machine == (40 if elf_class == 1 else 183)
                    if elf_class == 1:
                        assert bool(elf_flags & 0x400) == (arch == 'armebhf'), 'Actual ARM floating-point ABI differs'
                    run(label + '-qemu', [emulator, artifact])
                    row['contracts' if kind == 'contract' else 'independent_vector_programs'] += 1
                    row['artifacts'].append(dict(pin(artifact), elf_class=elf_class,
                        byte_order=byte_order, machine=machine, elf_flags=elf_flags))
                    save()
        report['status'] = 'SELECTED_VENDOR_ARM_SOFT_AND_BIG_ENDIAN_FREESTANDING_C_DATA_PASS'
        print(report['status'], flush=True)
    except BaseException as error:
        report.update(status='FAILED_RETAINED', error=repr(error))
        raise
    finally:
        save()


if __name__ == '__main__':
    main()
