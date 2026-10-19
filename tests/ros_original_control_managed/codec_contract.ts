/** Authored canonical contracts. Export does not mean it has been invoked.
 * Stage the actual complete generated directory in the private SDK tree. */
import * as r from '../../sdk/custom-types/generated-control-managed/bounded_records';
export {abortGuest} from '../../sdk/assemblyscript/api';
function need(value:bool):void{if(!value)unreachable();}
function source(n:i32):r.Ros2Type_description_interfacesMsgTypeSource{const s=new r.Ros2Type_description_interfacesMsgTypeSource();s.type_name='rcl_interfaces/msg/Log';s.encoding='msg';s.raw_file_contents=new Uint8Array(n);s.raw_file_contents.fill(255);if(n)s.raw_file_contents[n-1]=0;return s;}
/** Returns0 only after every local assertion; no ROS import is called. */
export function runContracts():u32{
 need(new r.Ros2Type_description_interfacesSrvGetTypeDescription_Request().include_type_sources);
 need(r.Ros2Rcl_interfacesMsgParameterType.PARAMETER_STRING==4);
 need(r.Ros2Type_description_interfacesSrvGetTypeDescriptionIdentity.codecIdentity!=r.Ros2Type_description_interfacesSrvGetTypeDescription_RequestCodec.codecIdentity);
 const sc=new r.Ros2Type_description_interfacesMsgTypeSourceCodec(),s=source(6162),wire=sc.encodeChecked(s);need(wire.status==0);const decoded=sc.decodeChecked(wire.bytes);need(decoded.status==0&&decoded.value!==null);const d=changetype<r.Ros2Type_description_interfacesMsgTypeSource>(decoded.value);need(d.raw_file_contents.length==6162&&d.raw_file_contents[0]==255&&d.raw_file_contents[6161]==0);
 need(sc.encodeChecked(source(6163)).status==22);
 const rc=new r.Ros2Type_description_interfacesSrvGetTypeDescription_ResponseCodec(),v=new r.Ros2Type_description_interfacesSrvGetTypeDescription_Response();v.successful=true;for(let i=0;i<12;++i)v.type_sources.push(source(16));const good=rc.encodeChecked(v);need(!good.status);const rd=rc.decodeChecked(good.bytes);need(!rd.status&&rd.value!==null);good.bytes.fill(0);need(changetype<r.Ros2Type_description_interfacesSrvGetTypeDescription_Response>(rd.value).type_sources[0].raw_file_contents[0]==255);
 v.type_sources=[];for(let i=0;i<12;++i)v.type_sources.push(source(3000));const too=rc.encodeChecked(v);need(too.status==90&&too.bytes.length==0);const large=rc.decodeChecked(new Uint8Array(32769));need(large.status==90&&large.value===null);
 const text=source(0);text.type_name='bad\0text';need(sc.encodeChecked(text).status==22);text.type_name=String.fromCharCode(0xd800);need(sc.encodeChecked(text).status==22);text.type_name='\ufeffname';const bom=sc.decodeChecked(sc.encode(text));need(!bom.status&&changetype<r.Ros2Type_description_interfacesMsgTypeSource>(bom.value).type_name==text.type_name);
 const pv=new r.Ros2Rcl_interfacesMsgParameterValue();pv.ros_type=4;pv.bool_value=true;pv.integer_value=i64.MIN_VALUE;pv.double_value=-1.25;pv.string_value='text';pv.byte_array_value=[0,255];pv.bool_array_value=[true,false];pv.integer_array_value=[i64.MIN_VALUE,i64.MAX_VALUE];pv.double_array_value=[2.5,-3.25];pv.string_array_value=['a','b'];const pc=new r.Ros2Rcl_interfacesMsgParameterValueCodec(),pd=pc.decodeChecked(pc.encode(pv));need(!pd.status&&pd.value!==null);const p=changetype<r.Ros2Rcl_interfacesMsgParameterValue>(pd.value);need(p.ros_type==4&&p.bool_value&&p.integer_value==i64.MIN_VALUE&&p.double_array_value[1]==-3.25&&p.string_array_value[1]=='b');
 const ec=new r.Ros2Service_msgsMsgServiceEventInfoCodec(),e=new r.Ros2Service_msgsMsgServiceEventInfo();e.client_gid.pop();need(ec.encodeChecked(e).status==22);
 const valid=sc.encode(source(1));for(let n=0;n<valid.length;++n)need(!sc.valid(valid.slice(0,n)));const trailing=new Uint8Array(valid.length+1);trailing.set(valid);need(!sc.valid(trailing));
 return 0;
}
