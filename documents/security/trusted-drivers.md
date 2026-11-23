# Optional secondary verification of legacy native preload

This legacy feature is compiled out by default. Rebuild explicitly with
`--enable-preload-dl=y` to restore it. It is not a ROSv2 driver launcher and
cannot be combined with ROSv2 scene deployment. ROSv2 requires every native
driver to run outside the VM; see [native driver isolation](../runtime/ros-native-driver-isolation.md).

`--wasm-register-dl PATH [NAME]` is an explicit host registration. Secondary
signature verification is **off by default**, independently of WSEA's selected
Wasm-module verification. `--secure` no longer categorically disables native
drivers.

Opt in for the launch with:

```sh
uwvm2-ros --wasm-driver-verify trusted-signer.pem \
  --wasm-register-dl driver.signed.so driver --run app.wasm
```

All verification options are processed before any registered library's
constructor, even if written after its registration on the command line.
The supplied PEM certificate pins an already approved signer key.
No certificate embedded in an input library can extend trust.

The wire envelope matches Linux `scripts/sign-file`: the complete ELF payload,
detached CMS SignedData, the 12-byte Linux signature-information trailer and
the module-signature marker. The type and reserved trailer fields are checked,
as are signature bounds, DER consumption, detached content, digest algorithm,
signer key and cryptographic signature. SHA-2 and SHA-3 digests of at least
256 bits and signer keys of at least 112 security bits are required. Sign the
final ELF after stripping or other transformations:

```sh
/path/to/linux/scripts/sign-file sha256 signer.key signer.pem \
  driver.so driver.signed.so
```

The private key is not needed by the VM. Linux's existing signing tool can
produce the envelope; no runtime signing service or alternative crypto library
is introduced. A marker alone is not evidence of a valid signature.

The VM copies the selected input into a sealed Linux memory file, extracts a
sealed payload, verifies it with the repository's OpenSSL integration and
checks its host ELF architecture/ABI before `dlopen`. No constructor runs on
verification failure. The verified payload descriptor outlives the loader
handle, preventing descriptor-name reuse from aliasing a different library.
Memory allocation, copy, signature or loader failure rejects the load.
The signing format's lengths and the host/OpenSSL representable address range
are checked; there is no arbitrary library/path size quota.

A native driver is part of the host trust boundary. Signatures do not sandbox
its instructions. External deployment owns the complete certificate policy,
expiry/revocation, anti-rollback, dependencies, dynamic-loader search paths and
environment. The VM repeats pinned-key signature verification, not a new
online PKI or revocation service. Dependencies are resolved by the trusted host
loader; this option does not independently sign-check every transitive DSO.
When loading via the sealed `/proc/self/fd` inode, `$ORIGIN` does not refer to
the original input directory. Deployment must supply an approved resolvable
dependency closure.

ROS permanently disables weak-symbol provider discovery. The explicit legacy
native preload path is retained behind its build gate; implicit weak
registration is not an alternative admission route.

## Offline ELF verification

Linux builds with WSEA OpenSSL support provide a standalone offline command.
The command works with native preload compiled out and exits after verification.

```sh
uwvm2-ros --verify-elf-driver executable driver.signed signer.pem
uwvm2-ros --verify-elf-driver shared-library driver.signed.so signer.pem
```

The command uses the same sealed-image CMS implementation, requires the pinned
certificate to be current and limits the signed file to 256 MiB. Exit status
0 means verified. Status 2 means a matching ELF with no appended signature
marker. Status 3 means a recognized CMS signature or content verification
failure. Status 1 includes trust failures, malformed envelopes and I/O errors.
WSEA `--secure` and wasm-cosign retain their Wasm-only scope.

References: [Linux module signing](https://docs.kernel.org/admin-guide/module-signing.html),
[Linux signature envelope](https://github.com/torvalds/linux/blob/master/include/uapi/linux/module_signature.h),
[OpenSSL CMS verification](https://docs.openssl.org/3.1/man3/CMS_verify/).
