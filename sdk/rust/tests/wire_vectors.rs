//! Generated from language-neutral wire fixtures; compilation/execution is a separate verdict.
#![cfg(feature = "alloc")]
use roswasm::{codec, typed::*};
use std::string::String;
fn bytes(h: &str) -> Vec<u8> {
    (0..h.len())
        .step_by(2)
        .map(|i| u8::from_str_radix(&h[i..i + 2], 16).unwrap())
        .collect()
}
#[test]
fn shared_positive_vectors() {
    {
        // string-empty
        let expected = bytes("00000000");
        let value = String::from("");
        assert_eq!(value.encode().unwrap(), expected);
        assert_eq!(String::decode(&expected).unwrap(), value);
    }
    {
        // string-ascii
        let expected = bytes("0500000068656c6c6f");
        let value = String::from("hello");
        assert_eq!(value.encode().unwrap(), expected);
        assert_eq!(String::decode(&expected).unwrap(), value);
    }
    {
        // string-utf8
        let expected = bytes("0a000000726f626f7420f09f8c8d");
        let value = String::from("robot 🌍");
        assert_eq!(value.encode().unwrap(), expected);
        assert_eq!(String::decode(&expected).unwrap(), value);
    }
    {
        // bool-false
        let expected = bytes("00");
        let mut b = [0u8; 1];
        codec::encode_bool(false, &mut b).unwrap();
        assert_eq!(b.as_slice(), expected);
        assert_eq!(codec::decode_bool(&expected).unwrap(), false);
    }
    {
        // bool-true
        let expected = bytes("01");
        let mut b = [0u8; 1];
        codec::encode_bool(true, &mut b).unwrap();
        assert_eq!(b.as_slice(), expected);
        assert_eq!(codec::decode_bool(&expected).unwrap(), true);
    }
    {
        // int32--2147483648
        let expected = bytes("00000080");
        let value = ParameterValue::Int32(-2147483648i32);
        assert_eq!(value.encode().unwrap(), expected);
        assert_eq!(
            ParameterValue::decode(ParameterKind::Int32, &expected)
                .unwrap()
                .encode()
                .unwrap(),
            expected
        );
    }
    {
        // int32--1
        let expected = bytes("ffffffff");
        let value = ParameterValue::Int32(-1i32);
        assert_eq!(value.encode().unwrap(), expected);
        assert_eq!(
            ParameterValue::decode(ParameterKind::Int32, &expected)
                .unwrap()
                .encode()
                .unwrap(),
            expected
        );
    }
    {
        // int32-2147483647
        let expected = bytes("ffffff7f");
        let value = ParameterValue::Int32(2147483647i32);
        assert_eq!(value.encode().unwrap(), expected);
        assert_eq!(
            ParameterValue::decode(ParameterKind::Int32, &expected)
                .unwrap()
                .encode()
                .unwrap(),
            expected
        );
    }
    {
        // int64--9223372036854775808
        let expected = bytes("0000000000000080");
        let value = ParameterValue::Int64(-9223372036854775808i64);
        assert_eq!(value.encode().unwrap(), expected);
        assert_eq!(
            ParameterValue::decode(ParameterKind::Int64, &expected)
                .unwrap()
                .encode()
                .unwrap(),
            expected
        );
    }
    {
        // int64--1
        let expected = bytes("ffffffffffffffff");
        let value = ParameterValue::Int64(-1i64);
        assert_eq!(value.encode().unwrap(), expected);
        assert_eq!(
            ParameterValue::decode(ParameterKind::Int64, &expected)
                .unwrap()
                .encode()
                .unwrap(),
            expected
        );
    }
    {
        // int64-9223372036854775807
        let expected = bytes("ffffffffffffff7f");
        let value = ParameterValue::Int64(9223372036854775807i64);
        assert_eq!(value.encode().unwrap(), expected);
        assert_eq!(
            ParameterValue::decode(ParameterKind::Int64, &expected)
                .unwrap()
                .encode()
                .unwrap(),
            expected
        );
    }
    {
        // float64-zero
        let expected = bytes("0000000000000000");
        let value = ParameterValue::Float64(0.0f64);
        assert_eq!(value.encode().unwrap(), expected);
        assert_eq!(
            ParameterValue::decode(ParameterKind::Float64, &expected)
                .unwrap()
                .encode()
                .unwrap(),
            expected
        );
    }
    {
        // float64-negative-zero
        let expected = bytes("0000000000000080");
        let value = ParameterValue::Float64(-0.0f64);
        assert_eq!(value.encode().unwrap(), expected);
        assert_eq!(
            ParameterValue::decode(ParameterKind::Float64, &expected)
                .unwrap()
                .encode()
                .unwrap(),
            expected
        );
    }
    {
        // float64-fraction
        let expected = bytes("000000000000f43f");
        let value = ParameterValue::Float64(1.25f64);
        assert_eq!(value.encode().unwrap(), expected);
        assert_eq!(
            ParameterValue::decode(ParameterKind::Float64, &expected)
                .unwrap()
                .encode()
                .unwrap(),
            expected
        );
    }
    {
        // twist
        let expected = bytes("000000000000f03f00000000000000c0000000000000084000000000000000000000000000000080000000000000e03f");
        let value = [1.0f64, -2.0f64, 3.0f64, 0.0f64, -0.0f64, 0.5f64];
        assert_eq!(value.encode().unwrap(), expected);
        assert_eq!(
            <[f64; 6] as Canonical>::decode(&expected)
                .unwrap()
                .encode()
                .unwrap(),
            expected
        );
    }
    {
        // array-bool
        let expected = bytes("03000000000100");
        let value = ParameterValue::BoolArray(vec![false, true, false]);
        assert_eq!(value.encode().unwrap(), expected);
        assert_eq!(
            ParameterValue::decode(ParameterKind::BoolArray, &expected)
                .unwrap()
                .encode()
                .unwrap(),
            expected
        );
    }
    {
        // array-byte
        let expected = bytes("03000000007fff");
        let value = ParameterValue::ByteArray(vec![0u8, 127u8, 255u8]);
        assert_eq!(value.encode().unwrap(), expected);
        assert_eq!(
            ParameterValue::decode(ParameterKind::ByteArray, &expected)
                .unwrap()
                .encode()
                .unwrap(),
            expected
        );
    }
    {
        // array-int32
        let expected = bytes("02000000ffffffffffffff7f");
        let value = ParameterValue::Int32Array(vec![-1i32, 2147483647i32]);
        assert_eq!(value.encode().unwrap(), expected);
        assert_eq!(
            ParameterValue::decode(ParameterKind::Int32Array, &expected)
                .unwrap()
                .encode()
                .unwrap(),
            expected
        );
    }
    {
        // array-int64
        let expected = bytes("020000000000000000000080ffffffffffffff7f");
        let value =
            ParameterValue::Int64Array(vec![-9223372036854775808i64, 9223372036854775807i64]);
        assert_eq!(value.encode().unwrap(), expected);
        assert_eq!(
            ParameterValue::decode(ParameterKind::Int64Array, &expected)
                .unwrap()
                .encode()
                .unwrap(),
            expected
        );
    }
    {
        // array-float64
        let expected = bytes("02000000000000000000f43f0000000000000080");
        let value = ParameterValue::Float64Array(vec![1.25f64, -0.0f64]);
        assert_eq!(value.encode().unwrap(), expected);
        assert_eq!(
            ParameterValue::decode(ParameterKind::Float64Array, &expected)
                .unwrap()
                .encode()
                .unwrap(),
            expected
        );
    }
    {
        // array-string
        let expected = bytes("03000000000000000500000068656c6c6f02000000cebb");
        let value = ParameterValue::StringArray(vec![
            String::from(""),
            String::from("hello"),
            String::from("λ"),
        ]);
        assert_eq!(value.encode().unwrap(), expected);
        assert_eq!(
            ParameterValue::decode(ParameterKind::StringArray, &expected)
                .unwrap()
                .encode()
                .unwrap(),
            expected
        );
    }
    {
        // array-empty
        let expected = bytes("00000000");
        let value = ParameterValue::StringArray(vec![]);
        assert_eq!(value.encode().unwrap(), expected);
        assert_eq!(
            ParameterValue::decode(ParameterKind::StringArray, &expected)
                .unwrap()
                .encode()
                .unwrap(),
            expected
        );
    }
    {
        // image-rgb8
        let expected = bytes("0000000001000000020000000600000063616d65726101000000010000000400000072676238000300000003000000ff0007");
        let value = Image {
            header: Header {
                sequence: 0,
                seconds: 1,
                nanoseconds: 2,
                frame: String::from("camera"),
            },
            height: 1,
            width: 1,
            encoding: String::from("rgb8"),
            big_endian: false,
            step: 3,
            data: vec![255, 0, 7],
        };
        assert_eq!(value.encode().unwrap(), expected);
        assert_eq!(
            Image::decode(&expected).unwrap().encode().unwrap(),
            expected
        );
    }
    {
        // transform
        let expected = bytes("000000000100000002000000030000006d61700400000062617365000000000000f03f00000000000000400000000000000840000000000000000000000000000000000000000000000000000000000000f03f");
        let value = Transform {
            header: Header {
                sequence: 0,
                seconds: 1,
                nanoseconds: 2,
                frame: String::from("map"),
            },
            child_frame: String::from("base"),
            values: [1.0f64, 2.0f64, 3.0f64, 0.0f64, 0.0f64, 0.0f64, 1.0f64],
        };
        assert_eq!(value.encode().unwrap(), expected);
        assert_eq!(
            Transform::decode(&expected).unwrap().encode().unwrap(),
            expected
        );
    }
    {
        // trigger-request
        let expected = bytes("");
        assert_eq!(codec::encode_trigger_request().as_slice(), expected);
        codec::decode_trigger_request(&expected).unwrap();
    }
    {
        // set-bool-request
        let expected = bytes("01");
        let mut b = [0u8; 1];
        codec::encode_bool(true, &mut b).unwrap();
        assert_eq!(b.as_slice(), expected);
        assert_eq!(codec::decode_bool(&expected).unwrap(), true);
    }
    {
        // service-response
        let expected = bytes("01020000006f6b");
        let value = codec::ServiceResponse {
            success: true,
            message: "ok",
        };
        let mut b = [0u8; 64];
        let n = codec::encode_service_response(&value, &mut b).unwrap();
        assert_eq!(&b[..n], expected);
        assert_eq!(codec::decode_service_response(&expected).unwrap(), value);
    }
    {
        // service-request-event
        let expected = bytes("050000000000008001");
        let (id, value) = codec::decode_set_bool_event(&expected).unwrap();
        assert_eq!(id.raw(), 9223372036854775813u64);
        assert!(value);
    }
    {
        // service-response-envelope
        let expected = bytes("050000000000008001020000006f6b");
        let id = codec::decode_trigger_event(&expected[..8]).unwrap();
        let response = codec::decode_service_response(&expected[8..]).unwrap();
        let mut b = [0u8; 64];
        let n = codec::encode_service_reply(id, &response, &mut b).unwrap();
        assert_eq!(&b[..n], expected);
        assert_eq!(id.raw(), 9223372036854775813u64);
    }
    {
        // fibonacci-order
        let expected = bytes("05000000");
        let mut b = [0u8; 4];
        codec::encode_fibonacci_goal(5, &mut b).unwrap();
        assert_eq!(b.as_slice(), expected);
    }
    {
        // goal-id
        let expected = bytes("0500000000000080");
        let id = codec::decode_goal_id(&expected).unwrap();
        assert_eq!(id.raw(), 9223372036854775813u64);
        assert_eq!(id.bytes().as_slice(), expected);
    }
    {
        // incoming-goal
        let expected = bytes("050000000000008005000000");
        let (id, order) = codec::decode_fibonacci_goal_event(&expected).unwrap();
        assert_eq!(id.raw(), 9223372036854775813u64);
        assert_eq!(order, 5);
    }
    {
        // feedback
        let expected =
            bytes("050000000000008006000000000000000100000001000000020000000300000005000000");
        let value = codec::decode_fibonacci_feedback(&expected).unwrap();
        assert_eq!(value.goal.raw(), 9223372036854775813u64);
        assert_eq!(value.values().collect::<Vec<_>>(), vec![0, 1, 1, 2, 3, 5]);
        let mut b = [0u8; 256];
        let n = codec::encode_fibonacci_feedback(
            value.goal,
            &value.values().collect::<Vec<_>>(),
            &mut b,
        )
        .unwrap();
        assert_eq!(&b[..n], expected);
    }
    {
        // success
        let expected =
            bytes("05000000000000800206000000000000000100000001000000020000000300000005000000");
        let value = codec::decode_fibonacci_result(&expected).unwrap();
        assert_eq!(value.goal.raw(), 9223372036854775813u64);
        assert_eq!(value.values().collect::<Vec<_>>(), vec![0, 1, 1, 2, 3, 5]);
        let mut b = [0u8; 256];
        let n = codec::encode_fibonacci_reply(
            value.goal,
            value.kind,
            &value.values().collect::<Vec<_>>(),
            &mut b,
        )
        .unwrap();
        assert_eq!(&b[..n], expected);
    }
    {
        // server-feedback
        let expected =
            bytes("05000000000000800106000000000000000100000001000000020000000300000005000000");
        let value = codec::decode_fibonacci_reply(&expected).unwrap();
        assert_eq!(value.goal.raw(), 9223372036854775813u64);
        assert_eq!(value.values().collect::<Vec<_>>(), vec![0, 1, 1, 2, 3, 5]);
        let mut b = [0u8; 256];
        let n = codec::encode_fibonacci_reply(
            value.goal,
            value.kind,
            &value.values().collect::<Vec<_>>(),
            &mut b,
        )
        .unwrap();
        assert_eq!(&b[..n], expected);
    }
    {
        // lifecycle-state
        let expected = bytes("0306000000616374697665");
        let value = LifecycleState {
            id: 3,
            label: String::from("active"),
        };
        assert_eq!(value.encode().unwrap(), expected);
        assert_eq!(LifecycleState::decode(&expected).unwrap(), value);
    }
    {
        // time-nanoseconds
        let expected = bytes("0500000000000080");
        assert_eq!(
            codec::decode_time(&expected).unwrap(),
            9223372036854775813u64
        );
    }
    {
        // legacy-parameter-event
        let expected =
            bytes("010000000200000002050000002f64656d6f0500000076616c75650400000074727565");
        let value = LegacyParameterEvent {
            seconds: 1,
            nanoseconds: 2,
            kind: 2,
            node: String::from("/demo"),
            name: String::from("value"),
            value: String::from("true"),
        };
        assert_eq!(value.encode().unwrap(), expected);
        assert_eq!(LegacyParameterEvent::decode(&expected).unwrap(), value);
    }
    {
        // typed-event-bool
        let expected =
            bytes("0100000002000000020201050000002f64656d6f0500000076616c75650100000001");
        let value = TypedParameterEvent {
            seconds: 1,
            nanoseconds: 2,
            kind: 2,
            tag: EventTag::Bool,
            node: String::from("/demo"),
            name: String::from("value"),
            value: Some(ParameterValue::Bool(true)),
        };
        assert_eq!(value.encode().unwrap(), expected);
        assert_eq!(
            TypedParameterEvent::decode(&expected)
                .unwrap()
                .encode()
                .unwrap(),
            expected
        );
    }
    {
        // typed-event-int64
        let expected = bytes(
            "0100000002000000020202050000002f64656d6f0500000076616c756508000000ffffffffffffff7f",
        );
        let value = TypedParameterEvent {
            seconds: 1,
            nanoseconds: 2,
            kind: 2,
            tag: EventTag::Int64,
            node: String::from("/demo"),
            name: String::from("value"),
            value: Some(ParameterValue::Int64(9223372036854775807i64)),
        };
        assert_eq!(value.encode().unwrap(), expected);
        assert_eq!(
            TypedParameterEvent::decode(&expected)
                .unwrap()
                .encode()
                .unwrap(),
            expected
        );
    }
    {
        // typed-event-float64
        let expected = bytes(
            "0100000002000000020203050000002f64656d6f0500000076616c756508000000000000000000f43f",
        );
        let value = TypedParameterEvent {
            seconds: 1,
            nanoseconds: 2,
            kind: 2,
            tag: EventTag::Float64,
            node: String::from("/demo"),
            name: String::from("value"),
            value: Some(ParameterValue::Float64(1.25f64)),
        };
        assert_eq!(value.encode().unwrap(), expected);
        assert_eq!(
            TypedParameterEvent::decode(&expected)
                .unwrap()
                .encode()
                .unwrap(),
            expected
        );
    }
    {
        // typed-event-string
        let expected = bytes(
            "0100000002000000020204050000002f64656d6f0500000076616c7565090000000500000068656c6c6f",
        );
        let value = TypedParameterEvent {
            seconds: 1,
            nanoseconds: 2,
            kind: 2,
            tag: EventTag::String,
            node: String::from("/demo"),
            name: String::from("value"),
            value: Some(ParameterValue::String(String::from("hello"))),
        };
        assert_eq!(value.encode().unwrap(), expected);
        assert_eq!(
            TypedParameterEvent::decode(&expected)
                .unwrap()
                .encode()
                .unwrap(),
            expected
        );
    }
    {
        // typed-event-byte-array
        let expected =
            bytes("0100000002000000020205050000002f64656d6f0500000076616c7565060000000200000000ff");
        let value = TypedParameterEvent {
            seconds: 1,
            nanoseconds: 2,
            kind: 2,
            tag: EventTag::ByteArray,
            node: String::from("/demo"),
            name: String::from("value"),
            value: Some(ParameterValue::ByteArray(vec![0u8, 255u8])),
        };
        assert_eq!(value.encode().unwrap(), expected);
        assert_eq!(
            TypedParameterEvent::decode(&expected)
                .unwrap()
                .encode()
                .unwrap(),
            expected
        );
    }
    {
        // typed-event-bool-array
        let expected =
            bytes("0100000002000000020206050000002f64656d6f0500000076616c756506000000020000000001");
        let value = TypedParameterEvent {
            seconds: 1,
            nanoseconds: 2,
            kind: 2,
            tag: EventTag::BoolArray,
            node: String::from("/demo"),
            name: String::from("value"),
            value: Some(ParameterValue::BoolArray(vec![false, true])),
        };
        assert_eq!(value.encode().unwrap(), expected);
        assert_eq!(
            TypedParameterEvent::decode(&expected)
                .unwrap()
                .encode()
                .unwrap(),
            expected
        );
    }
    {
        // typed-event-int64-array
        let expected = bytes("0100000002000000020207050000002f64656d6f0500000076616c75651400000002000000ffffffffffffffffffffffffffffff7f");
        let value = TypedParameterEvent {
            seconds: 1,
            nanoseconds: 2,
            kind: 2,
            tag: EventTag::Int64Array,
            node: String::from("/demo"),
            name: String::from("value"),
            value: Some(ParameterValue::Int64Array(vec![
                -1i64,
                9223372036854775807i64,
            ])),
        };
        assert_eq!(value.encode().unwrap(), expected);
        assert_eq!(
            TypedParameterEvent::decode(&expected)
                .unwrap()
                .encode()
                .unwrap(),
            expected
        );
    }
    {
        // typed-event-float64-array
        let expected = bytes("0100000002000000020208050000002f64656d6f0500000076616c75650c00000001000000000000000000f43f");
        let value = TypedParameterEvent {
            seconds: 1,
            nanoseconds: 2,
            kind: 2,
            tag: EventTag::Float64Array,
            node: String::from("/demo"),
            name: String::from("value"),
            value: Some(ParameterValue::Float64Array(vec![1.25f64])),
        };
        assert_eq!(value.encode().unwrap(), expected);
        assert_eq!(
            TypedParameterEvent::decode(&expected)
                .unwrap()
                .encode()
                .unwrap(),
            expected
        );
    }
    {
        // typed-event-string-array
        let expected = bytes("0100000002000000020209050000002f64656d6f0500000076616c756511000000020000000500000068656c6c6f00000000");
        let value = TypedParameterEvent {
            seconds: 1,
            nanoseconds: 2,
            kind: 2,
            tag: EventTag::StringArray,
            node: String::from("/demo"),
            name: String::from("value"),
            value: Some(ParameterValue::StringArray(vec![
                String::from("hello"),
                String::from(""),
            ])),
        };
        assert_eq!(value.encode().unwrap(), expected);
        assert_eq!(
            TypedParameterEvent::decode(&expected)
                .unwrap()
                .encode()
                .unwrap(),
            expected
        );
    }
    {
        // typed-deletion-tag-0
        let expected = bytes("0100000002000000030200050000002f64656d6f0500000076616c756500000000");
        let value = TypedParameterEvent {
            seconds: 1,
            nanoseconds: 2,
            kind: 3,
            tag: EventTag::Deletion,
            node: String::from("/demo"),
            name: String::from("value"),
            value: None,
        };
        assert_eq!(value.encode().unwrap(), expected);
        assert_eq!(
            TypedParameterEvent::decode(&expected)
                .unwrap()
                .encode()
                .unwrap(),
            expected
        );
    }
    {
        // typed-deletion-tag-1
        let expected = bytes("0100000002000000030201050000002f64656d6f0500000076616c756500000000");
        let value = TypedParameterEvent {
            seconds: 1,
            nanoseconds: 2,
            kind: 3,
            tag: EventTag::Bool,
            node: String::from("/demo"),
            name: String::from("value"),
            value: None,
        };
        assert_eq!(value.encode().unwrap(), expected);
        assert_eq!(
            TypedParameterEvent::decode(&expected)
                .unwrap()
                .encode()
                .unwrap(),
            expected
        );
    }
    {
        // typed-deletion-tag-2
        let expected = bytes("0100000002000000030202050000002f64656d6f0500000076616c756500000000");
        let value = TypedParameterEvent {
            seconds: 1,
            nanoseconds: 2,
            kind: 3,
            tag: EventTag::Int64,
            node: String::from("/demo"),
            name: String::from("value"),
            value: None,
        };
        assert_eq!(value.encode().unwrap(), expected);
        assert_eq!(
            TypedParameterEvent::decode(&expected)
                .unwrap()
                .encode()
                .unwrap(),
            expected
        );
    }
    {
        // typed-deletion-tag-3
        let expected = bytes("0100000002000000030203050000002f64656d6f0500000076616c756500000000");
        let value = TypedParameterEvent {
            seconds: 1,
            nanoseconds: 2,
            kind: 3,
            tag: EventTag::Float64,
            node: String::from("/demo"),
            name: String::from("value"),
            value: None,
        };
        assert_eq!(value.encode().unwrap(), expected);
        assert_eq!(
            TypedParameterEvent::decode(&expected)
                .unwrap()
                .encode()
                .unwrap(),
            expected
        );
    }
    {
        // typed-deletion-tag-4
        let expected = bytes("0100000002000000030204050000002f64656d6f0500000076616c756500000000");
        let value = TypedParameterEvent {
            seconds: 1,
            nanoseconds: 2,
            kind: 3,
            tag: EventTag::String,
            node: String::from("/demo"),
            name: String::from("value"),
            value: None,
        };
        assert_eq!(value.encode().unwrap(), expected);
        assert_eq!(
            TypedParameterEvent::decode(&expected)
                .unwrap()
                .encode()
                .unwrap(),
            expected
        );
    }
    {
        // typed-deletion-tag-5
        let expected = bytes("0100000002000000030205050000002f64656d6f0500000076616c756500000000");
        let value = TypedParameterEvent {
            seconds: 1,
            nanoseconds: 2,
            kind: 3,
            tag: EventTag::ByteArray,
            node: String::from("/demo"),
            name: String::from("value"),
            value: None,
        };
        assert_eq!(value.encode().unwrap(), expected);
        assert_eq!(
            TypedParameterEvent::decode(&expected)
                .unwrap()
                .encode()
                .unwrap(),
            expected
        );
    }
    {
        // typed-deletion-tag-6
        let expected = bytes("0100000002000000030206050000002f64656d6f0500000076616c756500000000");
        let value = TypedParameterEvent {
            seconds: 1,
            nanoseconds: 2,
            kind: 3,
            tag: EventTag::BoolArray,
            node: String::from("/demo"),
            name: String::from("value"),
            value: None,
        };
        assert_eq!(value.encode().unwrap(), expected);
        assert_eq!(
            TypedParameterEvent::decode(&expected)
                .unwrap()
                .encode()
                .unwrap(),
            expected
        );
    }
    {
        // typed-deletion-tag-7
        let expected = bytes("0100000002000000030207050000002f64656d6f0500000076616c756500000000");
        let value = TypedParameterEvent {
            seconds: 1,
            nanoseconds: 2,
            kind: 3,
            tag: EventTag::Int64Array,
            node: String::from("/demo"),
            name: String::from("value"),
            value: None,
        };
        assert_eq!(value.encode().unwrap(), expected);
        assert_eq!(
            TypedParameterEvent::decode(&expected)
                .unwrap()
                .encode()
                .unwrap(),
            expected
        );
    }
    {
        // typed-deletion-tag-8
        let expected = bytes("0100000002000000030208050000002f64656d6f0500000076616c756500000000");
        let value = TypedParameterEvent {
            seconds: 1,
            nanoseconds: 2,
            kind: 3,
            tag: EventTag::Float64Array,
            node: String::from("/demo"),
            name: String::from("value"),
            value: None,
        };
        assert_eq!(value.encode().unwrap(), expected);
        assert_eq!(
            TypedParameterEvent::decode(&expected)
                .unwrap()
                .encode()
                .unwrap(),
            expected
        );
    }
    {
        // typed-deletion-tag-9
        let expected = bytes("0100000002000000030209050000002f64656d6f0500000076616c756500000000");
        let value = TypedParameterEvent {
            seconds: 1,
            nanoseconds: 2,
            kind: 3,
            tag: EventTag::StringArray,
            node: String::from("/demo"),
            name: String::from("value"),
            value: None,
        };
        assert_eq!(value.encode().unwrap(), expected);
        assert_eq!(
            TypedParameterEvent::decode(&expected)
                .unwrap()
                .encode()
                .unwrap(),
            expected
        );
    }
}
#[test]
fn shared_negative_vectors() {
    {
        let b = bytes("02");
        assert!(codec::decode_bool(&b).is_err());
    }
    {
        let b = bytes("0100");
        assert!(codec::decode_bool(&b).is_err());
    }
    {
        let b = bytes("050000006869");
        assert!(codec::decode_string(&b).is_err());
    }
    {
        let b = bytes("02000000c0af");
        assert!(codec::decode_string(&b).is_err());
    }
    {
        let b = bytes("000000000000f07f");
        assert!(ParameterValue::decode(ParameterKind::Float64, &b).is_err());
    }
    {
        let b = bytes("01000000000000000000f87f");
        assert!(ParameterValue::decode(ParameterKind::Float64Array, &b).is_err());
    }
    {
        let b = bytes("0100000002");
        assert!(ParameterValue::decode(ParameterKind::BoolArray, &b).is_err());
    }
    {
        let b = bytes("0100000003000000610062");
        assert!(ParameterValue::decode(ParameterKind::StringArray, &b).is_err());
    }
    {
        let b = bytes("00000000000000000200000000");
        assert!(codec::decode_fibonacci_result(&b).is_err());
    }
    {
        let b = bytes("0100000002000000010200050000002f64656d6f0500000076616c756500000000");
        assert!(TypedParameterEvent::decode(&b).is_err());
    }
    {
        let b = bytes("0100000002000000030201050000002f64656d6f0500000076616c75650100000001");
        assert!(TypedParameterEvent::decode(&b).is_err());
    }
}
