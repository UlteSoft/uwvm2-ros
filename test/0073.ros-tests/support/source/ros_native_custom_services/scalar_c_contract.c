/* SPDX-License-Identifier: Apache-2.0 */
/** SOURCE ONLY: actual original rosidl C objects and actual generated canonical
 * C records. No substitute structs, guessed NativeID or ROS operation. The
 * complete selected bundle supplies bounded_records.h, including real bounds,
 * default initializer and unchanged actual codec identity. Native init/fini uses
 * the real xarm_msgs generated C library. No C object is passed to a C++ codec.
 */
#include "scalar_c_contract.h"
#include "scalar_contract_features.h"
#if UWVM_CUSTOM_SCALAR_C_METADATA_API
#include "bounded_records.h"
#include <xarm_msgs/srv/get_set_modbus_data.h>
#include <rosidl_runtime_c/string_functions.h>
#include <rosidl_runtime_c/primitives_sequence_functions.h>
#include <string.h>
#include <stdbool.h>
typedef roswasmc_bounded_2_xarm_msgs_srv_GetSetModbusData_Request BQ;
typedef roswasmc_bounded_2_xarm_msgs_srv_GetSetModbusData_Response BS;
typedef xarm_msgs__srv__GetSetModbusData_Request NQ;
typedef xarm_msgs__srv__GetSetModbusData_Response NS;
uint32_t uwvm_custom_modbus_c(unsigned char* defaults,size_t default_capacity,
 unsigned char* request,size_t request_capacity,unsigned char* response,size_t response_capacity,
 struct uwvm_custom_modbus_c_observation* output)
{
 BQ q={0},decoded_q={0};BS s={0},decoded_s={0};NQ native_q;NS native_s;
 bool q_live=false,s_live=false;uint32_t error=22;
 unsigned char a[ROSWASMC_BOUNDED_2_XARM_MSGS_SRV_GETSETMODBUSDATA_REQUEST_MAX_WIRE_BYTES];
 unsigned char b[ROSWASMC_BOUNDED_2_XARM_MSGS_SRV_GETSETMODBUSDATA_REQUEST_MAX_WIRE_BYTES];
 unsigned char c[ROSWASMC_BOUNDED_2_XARM_MSGS_SRV_GETSETMODBUSDATA_RESPONSE_MAX_WIRE_BYTES];
 size_t an=SIZE_MAX,bn=SIZE_MAX,cn=SIZE_MAX;
 struct uwvm_custom_modbus_c_observation result={0};
 static unsigned char const data[3]={0,128,255},ret[3]={127,0,255};
 static unsigned char const message[9]={'n','a','t','i','v','e','-',194,181};
 if(!defaults||!request||!response||!output)return 22;
 if(!xarm_msgs__srv__GetSetModbusData_Request__init(&native_q))return 12;q_live=true;
 if(!xarm_msgs__srv__GetSetModbusData_Response__init(&native_s)){error=12;goto done;}s_live=true;
 if(roswasmc_bounded_2_xarm_msgs_srv_GetSetModbusData_Request_default(&q))goto done;
 /* Both actual C generated constructor and actual canonical C_DEFAULT preserve
  * every declared and undeclared field. A zero aggregate alone is NOT host_id9. */
 if(native_q.modbus_data.size||native_q.modbus_length!=0||native_q.ret_length!=0||native_q.host_id!=9||
    native_q.is_transparent_transmission||native_q.use_503_port||q.modbus_data.size||q.modbus_length!=0||
    q.ret_length!=0||q.host_id!=9||q.is_transparent_transmission||q.use_503_port)goto done;
 if(roswasmc_bounded_2_xarm_msgs_srv_GetSetModbusData_Request_encode(&q,a,sizeof a,&an)||an!=11)goto done;
 if(roswasmc_bounded_2_xarm_msgs_srv_GetSetModbusData_Request_decode(a,an,&decoded_q)||decoded_q.host_id!=9||
    decoded_q.modbus_length||decoded_q.ret_length||decoded_q.modbus_data.size||decoded_q.is_transparent_transmission||decoded_q.use_503_port)goto done;
 rosidl_runtime_c__uint8__Sequence__fini(&native_q.modbus_data);
 if(!rosidl_runtime_c__uint8__Sequence__init(&native_q.modbus_data,3)){error=12;goto done;}
 memcpy(native_q.modbus_data.data,data,3);native_q.modbus_length=-3;native_q.ret_length=64;native_q.host_id=10;
 native_q.is_transparent_transmission=true;native_q.use_503_port=true;
 q.modbus_data.size=3;memcpy(q.modbus_data.data,native_q.modbus_data.data,3);
 q.modbus_length=native_q.modbus_length;q.ret_length=native_q.ret_length;q.host_id=native_q.host_id;
 q.is_transparent_transmission=native_q.is_transparent_transmission;q.use_503_port=native_q.use_503_port;
 if(roswasmc_bounded_2_xarm_msgs_srv_GetSetModbusData_Request_encode(&q,b,sizeof b,&bn)||bn!=14||
    roswasmc_bounded_2_xarm_msgs_srv_GetSetModbusData_Request_decode(b,bn,&decoded_q)||
    decoded_q.modbus_data.size!=3||memcmp(decoded_q.modbus_data.data,data,3)||decoded_q.modbus_length!=-3||
    decoded_q.ret_length!=64||decoded_q.host_id!=10||decoded_q.is_transparent_transmission!=1||decoded_q.use_503_port!=1)goto done;
 native_s.ret=-7;if(!rosidl_runtime_c__String__assignn(&native_s.message,(char const*)message,9)){error=12;goto done;}
 rosidl_runtime_c__uint8__Sequence__fini(&native_s.ret_data);
 if(!rosidl_runtime_c__uint8__Sequence__init(&native_s.ret_data,3)){error=12;goto done;}
 memcpy(native_s.ret_data.data,ret,3);s.ret=native_s.ret;s.message.size=(uint32_t)native_s.message.size;
 memcpy(s.message.data,native_s.message.data,s.message.size);s.ret_data.size=3;memcpy(s.ret_data.data,native_s.ret_data.data,3);
 if(roswasmc_bounded_2_xarm_msgs_srv_GetSetModbusData_Response_encode(&s,c,sizeof c,&cn)||cn!=22||
    roswasmc_bounded_2_xarm_msgs_srv_GetSetModbusData_Response_decode(c,cn,&decoded_s)||decoded_s.ret!=-7||
    decoded_s.message.size!=9||memcmp(decoded_s.message.data,message,9)||decoded_s.ret_data.size!=3||memcmp(decoded_s.ret_data.data,ret,3))goto done;
 /* All success products remain local until every complete value is validated. */
 if(an>default_capacity||bn>request_capacity||cn>response_capacity){error=90;goto done;}
 if(uwvm_bounded_overlap(defaults,an,request,bn)||uwvm_bounded_overlap(defaults,an,response,cn)||uwvm_bounded_overlap(request,bn,response,cn)||uwvm_bounded_overlap(output,sizeof *output,defaults,an)||uwvm_bounded_overlap(output,sizeof *output,request,bn)||uwvm_bounded_overlap(output,sizeof *output,response,cn))goto done;
 result.request=ROSIDL_GET_MSG_TYPE_SUPPORT(xarm_msgs,srv,GetSetModbusData_Request);
 result.response=ROSIDL_GET_MSG_TYPE_SUPPORT(xarm_msgs,srv,GetSetModbusData_Response);
 result.event=ROSIDL_GET_MSG_TYPE_SUPPORT(xarm_msgs,srv,GetSetModbusData_Event);
 result.service=ROSIDL_GET_SRV_TYPE_SUPPORT(xarm_msgs,srv,GetSetModbusData);
 if(!result.request||!result.response||!result.event||!result.service){error=95;goto done;}
 result.default_request_bytes=an;result.request_bytes=bn;result.response_bytes=cn;
 memcpy(defaults,a,an);memcpy(request,b,bn);memcpy(response,c,cn);*output=result;error=0;
 done:
 if(s_live)xarm_msgs__srv__GetSetModbusData_Response__fini(&native_s);
 if(q_live)xarm_msgs__srv__GetSetModbusData_Request__fini(&native_q);
 return error;
}

#else
uint32_t uwvm_custom_modbus_c(unsigned char*a,size_t an,unsigned char*b,size_t bn,unsigned char*c,size_t cn,struct uwvm_custom_modbus_c_observation*out){(void)a;(void)an;(void)b;(void)bn;(void)c;(void)cn;(void)out;return 95;}
#endif
