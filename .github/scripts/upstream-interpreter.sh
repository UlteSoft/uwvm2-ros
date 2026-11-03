#!/usr/bin/env bash
# This executes an immutable ordinary Uwvm2 checkout, never an uwvm2-ros product.
set -euo pipefail
test "$(uname -s)" = Linux
test -n "${UPSTREAM_SOURCE:?}"
test -n "${LLVM23_PREFIX:?}"
cd "$UPSTREAM_SOURCE"
test "$(git rev-parse HEAD)" = 999bb9439024b3c2adba54044b8374910efa411d
export PATH="$LLVM23_PREFIX/bin:$PATH"
export CXXFLAGS=-stdlib=libc++
export LDFLAGS=-stdlib=libc++
export UWVM_XMAKE_JOBS=1
export UWVM_STRICT_ABI_MODES=all
export UWVM_STRICT_MATRIX_LEVEL=coverage
bash test/0013.uwvm_int/strict/run_strict_basic_full.sh
