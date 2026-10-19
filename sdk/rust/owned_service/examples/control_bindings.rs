//! Actual future generated-record binding points for all seven roots.
//! These constants come from the separate exact source operator, never this file.
#![allow(dead_code)]
#[path="../../../custom-types/generated-control-managed/bounded_records.rs"]
pub mod records;
use roswasm_owned_service::{Codec,Definition};
pub fn describe_parameters()->Definition<records::Ros2Rcl_interfacesSrvDescribeParameters_Request,records::Ros2Rcl_interfacesSrvDescribeParameters_Response>{
Definition{native_type:records::Ros2Rcl_interfacesSrvDescribeParametersIdentity.native_type,canonical_identity:records::Ros2Rcl_interfacesSrvDescribeParametersIdentity.codec_identity,request:Codec::canonical(records::Ros2Rcl_interfacesSrvDescribeParameters_Request::MAX_WIRE_BYTES),response:Codec::canonical(records::Ros2Rcl_interfacesSrvDescribeParameters_Response::MAX_WIRE_BYTES)}
}
pub fn get_parameter_types()->Definition<records::Ros2Rcl_interfacesSrvGetParameterTypes_Request,records::Ros2Rcl_interfacesSrvGetParameterTypes_Response>{
Definition{native_type:records::Ros2Rcl_interfacesSrvGetParameterTypesIdentity.native_type,canonical_identity:records::Ros2Rcl_interfacesSrvGetParameterTypesIdentity.codec_identity,request:Codec::canonical(records::Ros2Rcl_interfacesSrvGetParameterTypes_Request::MAX_WIRE_BYTES),response:Codec::canonical(records::Ros2Rcl_interfacesSrvGetParameterTypes_Response::MAX_WIRE_BYTES)}
}
pub fn get_parameters()->Definition<records::Ros2Rcl_interfacesSrvGetParameters_Request,records::Ros2Rcl_interfacesSrvGetParameters_Response>{
Definition{native_type:records::Ros2Rcl_interfacesSrvGetParametersIdentity.native_type,canonical_identity:records::Ros2Rcl_interfacesSrvGetParametersIdentity.codec_identity,request:Codec::canonical(records::Ros2Rcl_interfacesSrvGetParameters_Request::MAX_WIRE_BYTES),response:Codec::canonical(records::Ros2Rcl_interfacesSrvGetParameters_Response::MAX_WIRE_BYTES)}
}
pub fn list_parameters()->Definition<records::Ros2Rcl_interfacesSrvListParameters_Request,records::Ros2Rcl_interfacesSrvListParameters_Response>{
Definition{native_type:records::Ros2Rcl_interfacesSrvListParametersIdentity.native_type,canonical_identity:records::Ros2Rcl_interfacesSrvListParametersIdentity.codec_identity,request:Codec::canonical(records::Ros2Rcl_interfacesSrvListParameters_Request::MAX_WIRE_BYTES),response:Codec::canonical(records::Ros2Rcl_interfacesSrvListParameters_Response::MAX_WIRE_BYTES)}
}
pub fn set_parameters()->Definition<records::Ros2Rcl_interfacesSrvSetParameters_Request,records::Ros2Rcl_interfacesSrvSetParameters_Response>{
Definition{native_type:records::Ros2Rcl_interfacesSrvSetParametersIdentity.native_type,canonical_identity:records::Ros2Rcl_interfacesSrvSetParametersIdentity.codec_identity,request:Codec::canonical(records::Ros2Rcl_interfacesSrvSetParameters_Request::MAX_WIRE_BYTES),response:Codec::canonical(records::Ros2Rcl_interfacesSrvSetParameters_Response::MAX_WIRE_BYTES)}
}
pub fn set_parameters_atomically()->Definition<records::Ros2Rcl_interfacesSrvSetParametersAtomically_Request,records::Ros2Rcl_interfacesSrvSetParametersAtomically_Response>{
Definition{native_type:records::Ros2Rcl_interfacesSrvSetParametersAtomicallyIdentity.native_type,canonical_identity:records::Ros2Rcl_interfacesSrvSetParametersAtomicallyIdentity.codec_identity,request:Codec::canonical(records::Ros2Rcl_interfacesSrvSetParametersAtomically_Request::MAX_WIRE_BYTES),response:Codec::canonical(records::Ros2Rcl_interfacesSrvSetParametersAtomically_Response::MAX_WIRE_BYTES)}
}
pub fn get_type_description()->Definition<records::Ros2Type_description_interfacesSrvGetTypeDescription_Request,records::Ros2Type_description_interfacesSrvGetTypeDescription_Response>{
Definition{native_type:records::Ros2Type_description_interfacesSrvGetTypeDescriptionIdentity.native_type,canonical_identity:records::Ros2Type_description_interfacesSrvGetTypeDescriptionIdentity.codec_identity,request:Codec::canonical(records::Ros2Type_description_interfacesSrvGetTypeDescription_Request::MAX_WIRE_BYTES),response:Codec::canonical(records::Ros2Type_description_interfacesSrvGetTypeDescription_Response::MAX_WIRE_BYTES)}
}
