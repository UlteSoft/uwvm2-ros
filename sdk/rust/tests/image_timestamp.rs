//! Backend timestamp-range fixtures prepared independently of the common golden set.
//! No ROS import is invoked. Compilation/execution remains a separate cgroup verdict.
#![cfg(feature = "alloc")]
use roswasm::typed::{Canonical, Header, Image, Transform};
#[test]
fn ros1_image_preserves_all_u32_second_bits() {
    let image = Image {
        header: Header {
            sequence: 0,
            seconds: u32::MAX,
            nanoseconds: 2,
            frame: "camera".into(),
        },
        height: 1,
        width: 1,
        encoding: "rgb8".into(),
        big_endian: false,
        step: 3,
        data: vec![1, 2, 3],
    };
    let wire = image.encode().unwrap();
    assert_eq!(&wire[4..8], &[255, 255, 255, 255]);
    assert_eq!(Image::decode(&wire).unwrap(), image);
    // The same structural Image wire is refused by the selected ROS2 host
    // adapter. A backend-neutral codec cannot silently truncate it to i32.
}
#[test]
fn transforms_keep_their_bounded_seconds_contract() {
    let transform = Transform {
        header: Header {
            sequence: 0,
            seconds: u32::MAX,
            nanoseconds: 2,
            frame: "map".into(),
        },
        child_frame: "base".into(),
        values: [0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0],
    };
    assert!(transform.encode().is_err());
    let mut valid = transform;
    valid.header.seconds = i32::MAX as u32;
    let mut wire = valid.encode().unwrap();
    wire[4..8].copy_from_slice(&u32::MAX.to_le_bytes());
    assert!(Transform::decode(&wire).is_err());
}
