#!/usr/bin/env bash
set -euo pipefail
source "$(dirname -- "${BASH_SOURCE[0]}")/require_wasm3_test_cgroup.sh"
cd -- "$(dirname -- "${BASH_SOURCE[0]}")/../.."
out=${1:?output directory};mkdir -p "$out";out=$(cd "$out" && pwd)
compiler=${CXX:-clang++}
before=$(python3 tools/ci/wasm3_source_fingerprint.py . "$out/source-before.json")
includes=(-I src -I third-parties/fast_io/include -I third-parties/bizwen/include -I third-parties/boost_unordered/include)
command=("$compiler" -std=c++26 -stdlib=libc++ -fuse-ld=lld -rtlib=compiler-rt -unwindlib=libunwind -O1 -g0
  -DUWVM=2 -DUWVM_FORCE_USE_MMAP -DUWVM_DISABLE_INT -DUWVM_DISABLE_JIT -DUWVM_RUNTIME_LLVM_JIT_CACHE_USE_OPENSSL_ED25519
  "${includes[@]}" -I /work/deps/usr/include -I /work/deps/usr/include/x86_64-linux-gnu
  test/0014.llvm_jit/cache_product_isolation.cc -L /work/deps/usr/lib/x86_64-linux-gnu -lcrypto -ldl -pthread -o "$out/mmap")
printf '%q ' "${command[@]}" > "$out/mmap.command"
"${command[@]}" > "$out/mmap.build.log" 2>&1
python3 test/0014.llvm_jit/check_cache_memory_backends.py --mmap "$out/mmap" --out "$out/results"

# The product header diagnoses each removed guest storage model even without a Wasm module.
for flag in UWVM_FORCE_DISABLE_MMAP UWVM_USE_MULTITHREAD_ALLOCATOR; do
    if "$compiler" -std=c++26 -stdlib=libc++ -DUWVM=2 "-D$flag" "${includes[@]}" -fsyntax-only -x c++ - \
        >"$out/$flag.log" 2>&1 <<< '#include <uwvm2/object/memory/linear/native.h>'; then
        echo "obsolete guest memory option accepted: $flag" >&2
        exit 1
    fi
    grep -Fq 'remove allocator memory model flags' "$out/$flag.log"
done
after=$(python3 tools/ci/wasm3_source_fingerprint.py . "$out/source-after.json")
[[ $before == "$after" ]]
