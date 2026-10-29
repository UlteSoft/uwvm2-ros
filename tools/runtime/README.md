# Install a pinned Linux runtime

`install.py` copies an explicitly identified VM ELF into a new directory named
by its complete SHA-256 digest. It requires Linux and Python 3.11 or newer.
The caller supplies the image, its production receipt, their complete digests,
and the source identity recorded in that receipt.

The production receipt is a JSON object with at least these fields:

```json
{
  "source_identity": "sha256:<64 lowercase hexadecimal characters>",
  "artifact": {
    "sha256": "<64 lowercase hexadecimal characters>",
    "bytes": 90866056
  }
}
```

The byte count above is illustrative. Use the actual producer's receipt and
verify its provenance. The installer checks the receipt's digest and its image,
size and source bindings; it does not certify the producer or transfer any
execution qualification.

Run the installation in the deployment's admitted Linux environment. Select a
writable absolute prefix whose parent already exists, without symlink components:

```sh
python3 tools/runtime/install.py \
  --image /producer/uwvm2-ros \
  --sha256 <image-sha256> \
  --source-identity sha256:<source-sha256> \
  --producer-receipt /producer/receipt.json \
  --producer-receipt-sha256 <receipt-sha256> \
  --prefix /home/user/.local/opt/uwvm2-ros
```

Substitute the real paths and digests before executing the command. The result is:

```text
<prefix>/versions/<image-sha256>/
  bin/uwvm2-ros
  producer-receipt.json
  installation.json
```

Existing versions are preserved and cause refusal. An interrupted installation
is also preserved; it is never automatically removed or treated as complete.
The installer requires at least 512 MiB of free space after the copy. It does not
start the VM. Apply the deployment's process, memory, CPU and lifetime controls
when subsequently running the installed executable.

## Generate the SDK command configuration

Prepare a JSON array containing the exact runtime arguments for the application.
For example, this requests the AOT route and an explicit cache policy:

```json
["-Raot", "-Rct", "0", "-Rllvm-cache-path", "disable"]
```

Append the following options to the installation command to generate
`command-run.json` alongside `installation.json`:

```sh
--target wasm32-wasip1 --backend ros2 --distribution jazzy \
--runtime-arguments-json /deployment/runtime-arguments.json
```

The generated `uwvm-ros-command-run-v1` object contains:

| Field | Meaning |
| --- | --- |
| `runtime` | Absolute path to the installed ELF in this version directory. |
| `target` | Exact SDK compiler triple, matching its selected architecture and WASI profile. |
| `backend`, `distribution` | Exact selected ROS backend and distribution. |
| `runtime_arguments` | The supplied array, preserving every string and its order. |
| `application_arguments` | An empty array, or the explicit array from `--application-arguments-json`. |

Target, backend, distribution and runtime arguments must be supplied together.
The SDK launcher appends `--run <built-Wasm>` and then the application arguments.
Runtime arguments therefore cannot already contain `--run` or `-r`.

Set `UWVM_ROS_RUN_CONFIG` to the resulting absolute `command-run.json` path,
or select that file with the SDK's `--ros-run-config` option. The SDK checks that
the deployment matches its selected target, backend and distribution before
executing the installed VM. This configuration alone does not qualify any WASI,
thread, component, ROS or SDK target route.

Supply all required VM feature, preload, policy and ROS options explicitly in
the argument array. Paths must exist in the environment where the installed VM
runs. The installer adds no grants or native gateway, and does not install ROS
providers. A native ROS deployment and its policies need their own compatible
producer, installation and execution evidence.
