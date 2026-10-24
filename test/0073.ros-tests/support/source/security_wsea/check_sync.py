#!/usr/bin/env python3
"""Compare the security core and its regression fixtures in three supplied roots."""
import argparse
import hashlib
import json
from pathlib import Path
import sys

parser = argparse.ArgumentParser(description=__doc__)
parser.add_argument('roots', type=Path, nargs=3)
args = parser.parse_args()
files = ['src/uwvm2/utils/security/wsea.h',
         'src/uwvm2/utils/mutex/rw_spin_lock.h',
         'documents/security/wsea.md',
         'tools/ci/run_wsea_embedded_targets.py',
         'tools/ci/run_wsea_usage_targets.py',
         'tools/ci/run_wsea_backend_disabled.py',
         'test/0073.ros-tests/support/source/security_wsea/backend_disabled.py',
         'tools/ci/run_wsea_thread_local.py',
         'test/0073.ros-tests/support/source/security_wsea/concurrent_verifier.cpp',
         'test/0073.ros-tests/support/source/security_wsea/native_sqrt.cpp',
         'test/0073.ros-tests/support/source/security_wsea/multisig_usage.py',
         'test/0073.ros-tests/support/source/security_wsea/multisig_receipt.cpp',
         'test/0073.ros-tests/support/source/security_wsea/concurrent_verifier.py',
         'test/0073.ros-tests/support/source/security_wsea/embedded_usage.py',
         'test/0073.ros-tests/support/source/security_wsea/embedded_receipt.py',
         'test/0073.ros-tests/support/source/security_wsea/embedded_receipt_runner.cpp',
         'test/0073.ros-tests/support/source/security_wsea/warm_key_matrix.py',
         'test/0073.ros-tests/support/source/security_wsea/multi_module_program.py',
         'test/0073.ros-tests/support/source/security_wsea/startup_algorithms.py',
         'test/0073.ros-tests/support/source/security_wsea/startup_cosign.py',
         'test/0073.ros-tests/support/source/security_wsea/startup_sizes.py',
         'test/0073.ros-tests/support/source/security_wsea/execution_policy.py',
         'test/0073.ros-tests/support/source/security_wsea/openssl_kat.cpp',
         'test/0073.ros-tests/support/source/security_wsea/arm_empty_concurrent_map.cpp',
         'third-parties/boost_unordered/include/boost/unordered/detail/foa/core.hpp',
         'tools/ci/collect_wsea_embedded_qualification.py',
         'test/0073.ros-tests/support/source/security_wsea/elf_audit_checks.py',
         'test/0073.ros-tests/support/source/security_wsea/verifier_runner.cpp',
         'test/0073.ros-tests/support/source/security_wsea/run.py',
         'test/0073.ros-tests/support/source/security_wsea/run_bounded.py',
         'test/0073.ros-tests/support/source/security_wsea/cli_loader_runner.cpp',
         'test/0073.ros-tests/support/source/security_wsea/cli_initializer_runner.cpp',
         'test/0073.ros-tests/support/source/security_wsea/cli_host_api_stub.cpp',
         'test/0073.ros-tests/support/source/security_wsea/README.md',
         'test/0073.ros-tests/support/source/security_wsea/check_sync.py',
         'test/0073.ros-tests/support/source/security_wsea/run_bounded_v2.py',
         'test/0073.ros-tests/support/source/security_wsea/openssl_dependency_smoke.cpp',
         'tools/dependencies/wsea_linux_profiles.json',
         'tools/ci/build_wsea_security_cross.py',
         'test/0073.ros-tests/support/source/security_wsea/algorithm_matrix.py',
         'test/0073.ros-tests/support/source/security_wsea/algorithm_matrix_runner.cpp',
         'test/0073.ros-tests/support/source/security_wsea/algorithm_fixture_signer.cpp',
         'test/0073.ros-tests/support/source/security_wsea/ALGORITHM_MATRIX.md',
         'tools/dependencies/build_openssl.py',
         'tools/dependencies/prepare_bootlin_sdk.py',
         'tools/dependencies/prepare_debian_sdk.py',
         'xmake/openssl.lua',
         'xmake/utility/bundled_openssl.lua',
         'third-parties/openssl/sources.sha256',
         'third-parties/openssl/provenance.json',
         'third-parties/openssl/README.md',
         'documents/toolchain/openssl-source.md']
rows = []
for name in files:
    hashes = {str(root.resolve()): hashlib.sha256((root / name).read_bytes()).hexdigest()
              if (root / name).is_file() else None for root in args.roots}
    rows.append({'path': name, 'sha256': hashes, 'check': 'identical common implementation',
                 'status': 'PASS' if None not in hashes.values() and len(set(hashes.values())) == 1 else 'FAIL'})
markers = {
    'xmake.lua': ['xmake/openssl.lua', 'openssl-provider', 'uwvm.bundled-openssl', 'uwvm.prebuilt-openssl', 'add_requires("openssl >=3.0.0"', '-fconstexpr-steps=8388608'],
    'xmake/option.lua': ['openssl-provider', 'openssl-provider-receipt', 'openssl-float-abi'],
    '.gitignore': ['!third-parties/openssl/**'],
    'src/uwvm2/utils/cmdline/handle.h': ['secure,'],
    'src/uwvm2/uwvm/cmdline/params/security.h': ['secure'],
    'src/uwvm2/uwvm/cmdline/params/security.cppm': ['#include "security.h"'],
    'src/uwvm2/uwvm/cmdline/params/impl.h': ['security.h'],
    'src/uwvm2/uwvm/cmdline/params/impl.cppm': ['security'],
    'src/uwvm2/uwvm/cmdline/params/help.h': ['secure'],
    'src/uwvm2/uwvm/cmdline/callback/help.h': ['<secure>', 'named-module'],
    'src/uwvm2/uwvm/cmdline/params.h': ['params::secure'],
    'src/uwvm2/uwvm/cmdline/parser.h': ['select_module(', 'named-module'],
    'src/uwvm2/uwvm/cmdline/parser.cppm': ['#include "parser.h"', 'security/wsea.h'],
    'src/uwvm2/uwvm/wasm/type/file.h': ['secure_required_', 'has_verified_execution_authorization'],
    'src/uwvm2/uwvm/wasm/type/file.cppm': ['#include "file.h"', 'security/wsea.h'],
    'src/uwvm2/uwvm/wasm/loader/wasm_file.h': ['verify_module(', 'owned_file_image::read'],
    'src/uwvm2/uwvm/wasm/loader/wasm_file.cppm': ['#include "wasm_file.h"', 'security/wsea.h'],
    'src/uwvm2/uwvm/wasm/loader/dl.h': ['secure_enabled()'],
    'src/uwvm2/uwvm/wasm/loader/dl.cppm': ['#include "dl.h"', 'security/wsea.h'],
    'src/uwvm2/uwvm/runtime/initializer/init.h': ['wsea_require_import_capabilities', 'has_verified_execution_authorization', '::std::_Exit(127)'],
    'src/uwvm2/runtime/lib/uwvm_runtime.default.cpp': ['wsea::secure_enabled()'],
    'src/uwvm2/runtime/lib/uwvm_runtime_threaded_wasip1_adapter.h': ['runtime_threaded_wasip1_adapter', 'resolve_wasip1_env_for_runtime_module_id', 'operation::yield'],
    'src/uwvm2/runtime/lib/uwvm_runtime.module.cpp': ['security/wsea.h'],
    'src/uwvm2/runtime/lib/uwvm_runtime_checkpoint_staged_llvm_full_engine.h': ['security_allows_original_code', 'secure_source'],
    'src/uwvm2/runtime/lib/uwvm_runtime_checkpoint_world_full_code_handoff.h': ['security_allows_original_code'],
    'src/uwvm2/runtime/lib/uwvm_runtime_checkpoint_world_source_initializer.h': ['secure_source', 'has_verified_execution_authorization'],
}
for name, expected in markers.items():
    copies = {}
    for root in args.roots:
        path = root / name
        content = path.read_bytes() if path.is_file() else None
        text = content.decode('utf-8-sig') if content is not None else ''
        copies[str(root.resolve())] = {'sha256': hashlib.sha256(content).hexdigest() if content is not None else None,
                                       'missing_markers': [marker for marker in expected if marker not in text]}
    rows.append({'path': name, 'check': 'security integration markers; base files may differ', 'copies': copies,
                 'status': 'PASS' if all(row['sha256'] is not None and not row['missing_markers'] for row in copies.values()) else 'FAIL'})
print(json.dumps({'status': 'PASS' if all(row['status'] == 'PASS' for row in rows) else 'FAIL',
                  'files': rows}, indent=2))
sys.exit(0 if all(row['status'] == 'PASS' for row in rows) else 1)
