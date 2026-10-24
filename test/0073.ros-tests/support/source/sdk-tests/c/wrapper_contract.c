/** @file
 * @brief Prepared pure C11 model tests for ownership, output-slot errors and bounded codecs.
 * Tests require no actual ROS imports. Their execution is not native ROS/ARM evidence.
 */
#include <assert.h>
#include <platform/ros/c/roswasm/node.h>
#include "mock_backend.h"
/** @brief Count guest-driven callbacks; no host callback enters the guest. */
static void on_event(void* user,unsigned char const* bytes,size_t length) {
    (void)user;(void)bytes;(void)length;++model_callback_calls;
}
/** @brief Check nonzero-return output preservation, typed wires and one-pass dispatch. */
int main(void) {
    roswasmc_context context={model_backend()};roswasmc_capability cap;
    unsigned char buffer[128];size_t length=99,delivered=99;roswasmc_result result;
    roswasmc_string string={"unchanged",9};int boolean=77;roswasmc_twist twist={{1,2,3},{4,5,6}},decoded;
    roswasmc_parameter_value parameter={0},decoded_parameter={0};
    roswasmc_capability_init(&cap,&context);
    assert(roswasmc_open(&cap,"absent",6)==UWVM_ROS_DENIED&&cap.handle==0);
    assert(roswasmc_open(&cap,"allowed",7)==0&&cap.handle==UINT64_C(0x0102030405060708));
    assert(roswasmc_poll(&cap,0,1).error==22);
    assert(roswasmc_publish(&cap,0,1)==22);
    {roswasmc_capability empty={&context,0};assert(roswasmc_publish(&empty,buffer,0)==9);}
    assert(roswasmc_encode_string((roswasmc_string){"hello",5},buffer,sizeof buffer,&length)==0&&length==9);
    assert(buffer[0]==5&&buffer[1]==0&&memcmp(buffer+4,"hello",5)==0);
    assert(roswasmc_decode_string(buffer,length,&string)==0&&string.size==5);
    {roswasmc_string nul_text={"a\0b",3},nul_decoded;
     assert(roswasmc_encode_string(nul_text,buffer,sizeof buffer,&length)==0&&length==7&&buffer[5]==0);
     assert(roswasmc_decode_string(buffer,length,&nul_decoded)==0&&nul_decoded.size==3&&nul_decoded.data[1]==0);}
    assert(roswasmc_encode_string((roswasmc_string){"hello",5},buffer,sizeof buffer,&length)==0);
    string=(roswasmc_string){"unchanged",9};assert(roswasmc_decode_string(buffer,length-1,&string)==22&&string.size==9);
    buffer[0]=2;assert(roswasmc_decode_bool(buffer,1,&boolean)==22&&boolean==77);
    assert(roswasmc_encode_twist(&twist,buffer,sizeof buffer,&length)==0&&length==48);
    assert(roswasmc_decode_twist(buffer,length,&decoded)==0&&decoded.angular[2]==6);
    parameter.tag=ROSWASMC_PARAM_INT64;parameter.integer_bits=UINT64_MAX;
    assert(roswasmc_encode_parameter(&parameter,buffer,sizeof buffer,&length)==0&&length==8);
    assert(roswasmc_decode_parameter(ROSWASMC_PARAM_INT64,buffer,length,&decoded_parameter)==0&&decoded_parameter.integer_bits==UINT64_MAX);
    model_error=90;model_write_length=0;model_payload_size=23;
    result=roswasmc_poll(&cap,buffer,sizeof buffer);assert(result.error==90&&!result.has_length);
    result=roswasmc_service_call(&cap,0,0,buffer,sizeof buffer);assert(result.error==90&&!result.has_length);
    model_write_length=1;result=roswasmc_poll(&cap,buffer,sizeof buffer);assert(result.error==90&&result.has_length&&result.length==23);
    model_error=11;model_write_length=0;
    {roswasmc_poll_task task={&cap,buffer,sizeof buffer,on_event,0};assert(roswasmc_spin_some(&task,1,1,&delivered)==0&&delivered==0&&model_callback_calls==0);}
    model_error=0;model_payload_size=1;model_payload[0]=1;
    {roswasmc_poll_task task={&cap,buffer,sizeof buffer,on_event,0};assert(roswasmc_spin_some(&task,1,1,&delivered)==0&&delivered==1&&model_callback_calls==1);}
    model_malformed_success=1;
    result=roswasmc_poll(&cap,buffer,sizeof buffer);assert(result.error==22&&!result.has_length);
    result=roswasmc_service_call(&cap,0,0,buffer,sizeof buffer);assert(result.error==22&&!result.has_length);
    model_malformed_success=2;
    result=roswasmc_poll(&cap,buffer,sizeof buffer);assert(result.error==22&&!result.has_length);
    result=roswasmc_service_call(&cap,0,0,buffer,sizeof buffer);assert(result.error==22&&!result.has_length);
    model_malformed_success=0;
    {roswasmc_context empty_context={0};roswasmc_capability empty={&empty_context,1};
     assert(roswasmc_poll(&empty,buffer,sizeof buffer).error==9);assert(roswasmc_service_call(&empty,0,0,buffer,sizeof buffer).error==9);}
    model_error=107;assert(roswasmc_close(&cap)==107&&cap.handle==0&&model_close_calls==1);
    assert(roswasmc_close(&cap)==9&&model_close_calls==1);
    return 0;
}
