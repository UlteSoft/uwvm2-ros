# Persistent Linux cache origin

This optional administrator helper preserves the cache signing root across
reboots. The VM still reads only its authenticated Linux UID-keyring origin.
It never opens this helper's disk store and never accepts a signature based
only on public metadata or an executable's SHA-256.

Build on the deployment's Linux ABI with the selected native compiler:

```sh
xmake -P tools/cache-origin f --toolchain=clang -m release
xmake -P tools/cache-origin
```

The result is an ordinary administrator program, **not a setuid executable**.
Install it as root at `/usr/libexec/uwvm2-ros-cache-origin`, owned by `root:root`
with mode `0755`. Install `uwvm2-ros-cache-origin@.service` in the system unit
directory. For a dedicated VM service account with UID `1000`:

```sh
sudo systemctl daemon-reload
sudo systemctl enable uwvm2-ros-cache-origin@1000.service
```

The VM service must start after successful provisioning. Add this dependency
to that service's administrator-owned unit:

```ini
[Unit]
Requires=uwvm2-ros-cache-origin@1000.service
After=uwvm2-ros-cache-origin@1000.service
```

Provision before the UID first runs the VM on that boot. Starting the helper
after the VM has already created a different ephemeral origin fails explicitly;
it does not replace that live key. Enable the service, stop the affected VM
service, and activate the configured ordering on the next system boot.
Do not revoke another running process's key to make provisioning succeed.

For a supervisor without systemd, run the following once before starting that
UID's VM processes, and require a successful exit status:

```sh
sudo /usr/libexec/uwvm2-ros-cache-origin --uid 1000
```

The store is fixed at `/var/lib/uwvm2-ros/cache-origin`. Every ancestor must be
root-owned and not group/other writable; the final directory must be `0700`.
Each `uid-N.secret` is a root-owned, single-link, regular `0600` file containing
32 random bytes. A private root-only lock serializes provisioning. Creation is
exclusive, the file and directory are synchronized before kernel publication,
and malformed existing files are never regenerated silently.

The child drops supplementary groups and all root UID/GID credentials before
selecting the target's actual kernel UID ring. Its immutable key has exactly
the runtime's owner, permissions and direct-membership contract. Publication
uses an exclusive kernel move, then verifies the winning key. An existing
different origin, a symlink, a hard link, a loose mode, a wrong owner or a
malformed file causes refusal. No secret is placed in arguments, environment,
logs or VM-accessible descriptors. The helper disables core dumps and erases
its temporary secret buffer before exit.

This is a root-controlled deployment trust store, not TPM sealing or disk
encryption. Root, a compromised kernel, physical access to an unencrypted
store, and trusted native processes acting as the VM UID remain outside the
guest/cache-file trust boundary. Keep backups private and never grant the
store through a guest filesystem, stdin, container bind mount or ROS service.
Use a dedicated non-root VM account. Do not copy a signing root to unrelated
machines or tenants merely to share caches.

Retaining this origin does not retain guest authority. Actual executable SHA,
IR, ISA, ABI, provider admission, signature verification, quotas and current
ROS/WASI grants remain mandatory. Source or compatible-library path spelling
alone does not rotate the origin. Losing or deliberately replacing the root
requires rebuilding objects; it is never grounds to skip authentication.

The regression in
`test/0073.ros-tests/support/source/runtime_repair/run_cache_origin_reboot.py`
uses two full-system Linux boots with different boot IDs and the same retained
disk. It checks a signed hit after reboot, unchanged object bytes, live unwind,
and refusal cases without replacing the original working key. Raw test images
containing test signing roots are private test artifacts and must not be
published or included in result archives.
