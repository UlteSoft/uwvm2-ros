//! Authored local canonical contracts, never Native qualification.
//! Stage actual generated records in the reviewed private SDK layout first.
#[path="../../../sdk/custom-types/generated-control-managed/bounded_records.rs"]
mod bounded_records;
use bounded_records::*;
use roswasm::{codes,typed::Canonical};
fn source(n:usize)->Ros2Type_description_interfacesMsgTypeSource {
 let mut s=Ros2Type_description_interfacesMsgTypeSource::default();
 s.type_name="rcl_interfaces/msg/Log".into();s.encoding="msg".into();s.raw_file_contents=vec![0xff;n];
 if n>2{s.raw_file_contents[0]=0;s.raw_file_contents[1]=0xc0;s.raw_file_contents[n-1]=0;}s
}
#[test]fn declared_default_and_constants(){assert!(Ros2Type_description_interfacesSrvGetTypeDescription_Request::default().include_type_sources);assert_eq!(Ros2Rcl_interfacesMsgParameterType::PARAMETER_STRING,4);assert_ne!(Ros2Type_description_interfacesSrvGetTypeDescriptionIdentity.codec_identity,Ros2Type_description_interfacesSrvGetTypeDescription_Request::CODEC_IDENTITY);}
#[test]fn raw_counted_bytes_preserve_nul_and_invalid_utf8(){let v=source(6162);let b=v.encode().unwrap();let d=Ros2Type_description_interfacesMsgTypeSource::decode(&b).unwrap();assert_eq!(v,d);}
#[test]fn full_sources_response_and_owned_decode(){let mut v=Ros2Type_description_interfacesSrvGetTypeDescription_Response::default();v.successful=true;v.type_sources=(0..12).map(|_|source(16)).collect();let mut b=v.encode().unwrap();let d=Ros2Type_description_interfacesSrvGetTypeDescription_Response::decode(&b).unwrap();b.fill(0);assert_eq!(d,v);}
#[test]fn aggregate_is_independent_of_cartesian_capacity(){assert!(Ros2Type_description_interfacesSrvGetTypeDescription_Response::CARTESIAN_WIRE_BYTES>32768);let mut v=Ros2Type_description_interfacesSrvGetTypeDescription_Response::default();v.type_sources=(0..12).map(|_|source(3000)).collect();assert_eq!(v.encode().unwrap_err().code,codes::TOO_LARGE);}
#[test]fn caller_buffer_unchanged_on_aggregate_refusal(){let mut v=Ros2Type_description_interfacesSrvGetTypeDescription_Response::default();v.type_sources=(0..12).map(|_|source(3000)).collect();let mut out=[0xa5;32768];assert_eq!(v.encode_into(&mut out).unwrap_err().code,90);assert!(out.iter().all(|x|*x==0xa5));}
#[test]fn strict_text_nul_refusal(){let mut v=source(1);v.type_name="bad\0text".into();assert_eq!(v.encode().unwrap_err().code,22);}
#[test]fn all_parameter_storage_members_roundtrip(){let mut v=Ros2Rcl_interfacesMsgParameterValue::default();v.r#type=4;v.bool_value=true;v.integer_value=i64::MIN;v.double_value=-1.25;v.string_value="text".into();v.byte_array_value=vec![0,255];v.bool_array_value=vec![true,false];v.integer_array_value=vec![i64::MIN,i64::MAX];v.double_array_value=vec![2.5,-3.25];v.string_array_value=vec!["a".into(),"b".into()];assert_eq!(Ros2Rcl_interfacesMsgParameterValue::decode(&v.encode().unwrap()).unwrap(),v);}
#[test]fn invalid_bool_trailing_and_truncation_refused(){let v=Ros2Type_description_interfacesSrvGetTypeDescription_Response::default();let b=v.encode().unwrap();let mut bad=b.clone();bad[0]=2;assert_eq!(Ros2Type_description_interfacesSrvGetTypeDescription_Response::decode(&bad).unwrap_err().code,22);let mut trailing=b.clone();trailing.push(0);assert!(Ros2Type_description_interfacesSrvGetTypeDescription_Response::decode(&trailing).is_err());assert!(Ros2Type_description_interfacesSrvGetTypeDescription_Response::decode(&b[..b.len()-1]).is_err());}
#[test]fn exact_fixed_array_and_full_u64(){let mut e=Ros2Service_msgsMsgServiceEventInfo::default();e.client_gid[15]=255;e.sequence_number=i64::MIN;assert_eq!(Ros2Service_msgsMsgServiceEventInfo::decode(&e.encode().unwrap()).unwrap(),e);let mut r=Ros2Rcl_interfacesMsgIntegerRange::default();r.from_value=i64::MIN;r.to_value=i64::MAX;r.step=u64::MAX;assert_eq!(Ros2Rcl_interfacesMsgIntegerRange::decode(&r.encode().unwrap()).unwrap(),r);}
#[test]fn nonfinite_and_overlong_raw_refused(){let mut v=Ros2Rcl_interfacesMsgFloatingPointRange::default();v.step=f64::NAN;assert_eq!(v.encode().unwrap_err().code,22);assert_eq!(source(6163).encode().unwrap_err().code,22);}
