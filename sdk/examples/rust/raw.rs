//! Stage1: inspect and call the exact memory-zero imports before using wrappers.
//! Build with exactly one of ros1/ros2 features and a wasm32 WASI target. The
//! host must explicitly grant alias `pub` for the canonical String topic type.
//! This example does not choose a native topic or obtain any default rights.
#[cfg(all(feature = "ros1", not(feature = "ros2")))]
use roswasm::raw::ros1 as imports;
#[cfg(all(feature = "ros2", not(feature = "ros1")))]
use roswasm::raw::ros2 as imports;
#[cfg(any(
    all(feature = "ros1", feature = "ros2"),
    not(any(feature = "ros1", feature = "ros2"))
))]
compile_error!("Choose exactly one example feature: ros1 or ros2");

fn main() {
    if !cfg!(all(target_arch = "wasm32", target_pointer_width = "32")) {
        eprintln!("This pointer example requires wasm32 memory zero");
        std::process::exit(1);
    }
    // Every offset/length/handle is an i64 even though this guest uses wasm32
    // pointers. Never pass a pointer-width usize directly as the import signature.
    let alias = b"pub";
    let mut handle_slot = [0u8; 8];
    // SAFETY: alias and slot are live memory-zero objects for the synchronous call.
    let version = unsafe { imports::abi_version() };
    if version != 1 {
        eprintln!("Unsupported ROS ABI version {version}");
        std::process::exit(1);
    }
    let error = unsafe {
        imports::capability_open(
            0,
            alias.as_ptr() as usize as u64,
            alias.len() as u64,
            handle_slot.as_mut_ptr() as usize as u64,
        )
    };
    if error != 0 {
        eprintln!("Open failed: unsigned host code {error}");
        std::process::exit(1);
    }
    let handle = u64::from_le_bytes(handle_slot);
    if handle == 0 { eprintln!("Malformed successful handle slot"); std::process::exit(1); }
    // String is length prefix5 followed by UTF-8 hello, not a C string or CDR.
    let payload = [5u8, 0, 0, 0, b'h', b'e', b'l', b'l', b'o'];
    let error = unsafe {
        imports::topic_publish(
            handle,
            0,
            payload.as_ptr() as usize as u64,
            payload.len() as u64,
        )
    };
    // Close is attempted even after a publish refusal. The opaque handle is never
    // synthesized or transferred to another backend or WebAssembly instance.
    let close_error = unsafe { imports::capability_close(handle) };
    if error != 0 {
        eprintln!("Publish failed: {error}");
    }
    if close_error != 0 {
        eprintln!("Close notification failed after retirement: {close_error}");
    }
    if error != 0 || close_error != 0 { std::process::exit(1); }
}
