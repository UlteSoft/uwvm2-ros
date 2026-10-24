#!/usr/bin/env python3
"""Emit authored whole-record/truncation contracts; no native ROS acceptance.

The independent nonzero vector tool reads original declarations separately.
These complementary contracts check transactional errors and exact source API.
Synthetic values never assert a valid physical command or controller limit.
"""
import argparse
import json
from pathlib import Path


def emit(source, out):
    source, out = Path(source).resolve(strict=True), Path(out)
    assert out.is_absolute() and not out.exists()
    out.mkdir(mode=0o700)
    for profile in [r['id'] for r in json.loads((source / 'sdk/origin/robots/arms/realman/profiles.json').read_bytes())['profiles']]:
        version = 1 if profile.startswith('ros1_') else 2
        family = 'realman-' + profile.replace('_', '-') + '-complete'
        generation = json.loads((source / 'sdk/origin/custom-types' / family / 'generation.vendor.json').read_bytes())
        text = '''/* Authored complete records and transactional malformed/capacity cases.
 * These are SDK contracts, not original upstream or hardware tests. */
#include <assert.h>
#include <stdint.h>
#include <string.h>
#include "state.h"
#define INIT_0(P,v) ((void)0)
#define INIT_1(P,v) assert(P##_default(&(v))==0)
#define CHECK(P,D) do{P a={0},out,before;unsigned char wire[32769];size_t n=0,i;INIT_##D(P,a);assert(P##_encode(&a,wire,32768,&n)==0);assert(P##_decode(wire,n,&out)==0);memset(&out,0x55,sizeof out);memcpy(&before,&out,sizeof out);for(i=0;i<n;++i){assert(P##_decode(wire,i,&out)!=0);assert(!memcmp(&out,&before,sizeof out));}wire[n]=0xff;assert(P##_decode(wire,n+1,&out)!=0);assert(!memcmp(&out,&before,sizeof out));if(n){size_t used=99;memset(wire,0x66,n);assert(P##_encode(&a,wire,n-1,&used)!=0);for(i=0;i<n;++i)assert(wire[i]==0x66);}}while(0)
#if defined(__GNUC__)
#define UWVM_REALMAN_TEST_NOINLINE __attribute__((noinline))
#else
#define UWVM_REALMAN_TEST_NOINLINE
#endif
/* One finite frame per record also works in unoptimized native CMake builds. */
#define DECL(P,D) static UWVM_REALMAN_TEST_NOINLINE void check_##P(void){CHECK(P,D);}
'''
        calls = []
        for record in generation['records']:
            name = record['type']
            prefix = 'roswasmc_bounded_' + str(version) + '_' + name.replace('/', '_')
            default = int(bool(generation['declared_defaults'].get(name)))
            text += 'DECL(' + prefix + ',' + str(default) + ');\n'
            calls.append('check_' + prefix + '();')
        text += 'int main(void){\n' + '\n'.join(calls) + '\n'
        text += 'return 0;}\n'
        (out / ('contract_' + profile + '.c')).write_text(text)
        (out / ('contract_' + profile + '.cc')).write_text('#include "state.hpp"\n' + text)


if __name__ == '__main__':
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--source', type=Path, required=True)
    parser.add_argument('--out', type=Path, required=True)
    args = parser.parse_args()
    emit(args.source, args.out)
