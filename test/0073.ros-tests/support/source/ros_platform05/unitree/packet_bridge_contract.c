/* Pure vendor packet/FFI contract. No ROS import, endpoint, clock or robot.
 * Build as C11 and link the genuine C++20 Go2/B2 formatter providers. */
#include <platform/unitree/c/roswasm_unitree/packet_bridge.h>
#include <assert.h>
#include <math.h>
#include <stdalign.h>
#include <stdio.h>
#include <string.h>
typedef roswasmc_bounded_2_unitree_api_msg_Request Request;
typedef roswasmc_bounded_2_unitree_api_msg_Response Response;
static unsigned char original_wire[6181], expected_wire[6181], output[6181];
static size_t original_size, expected_size;
static float flat[210];
static uwvm_unitree_b2_path_point path[30];
static Request original;

#if defined(UWVM_UNITREE_PACKET_JSON)
static void query(char const *text,int32_t native,uint32_t expected,uint32_t boolean) {
    Response response={0};
    unsigned char wire[6172];
    size_t n=0;
    uint32_t value=777;
    int32_t code=888;
    response.header.identity.id=1;
    response.header.identity.api_id=2055;
    response.header.status.code=native;
    response.data.size=(uint32_t)strlen(text);
    memcpy(response.data.data,text,response.data.size);
    assert(roswasmc_bounded_2_unitree_api_msg_Response_encode(&response,wire,sizeof wire,&n)==0);
    assert(uwvm_unitree_packet_recovery(wire,n,&value,&code)==expected);
    if(expected) assert(value==777 && code==888);
    else assert(value==boolean && code==native);
}
#endif

static void one(uint32_t family, char const *name, uint32_t id,
                uwvm_unitree_sport_arguments args, int trajectory) {
    Request expected = original, decoded;
    uint32_t found = 0, shape = UINT32_MAX;
    size_t size = SIZE_MAX;
    assert(uwvm_unitree_packet_lookup(family,name,strlen(name),&found,&shape)==0);
    assert(found==id && shape<=6);
    if (family==UWVM_UNITREE_PACKET_GO2)
        assert(uwvm_unitree_sport_prepare(&expected,id,args)==0);
    else
        assert(uwvm_unitree_b2_sport_prepare(&expected,id,args,
            trajectory ? path : NULL,trajectory ? 30 : 0)==0);
    assert(roswasmc_bounded_2_unitree_api_msg_Request_encode(
        &expected,expected_wire,sizeof expected_wire,&expected_size)==0);
    assert(uwvm_unitree_packet_prepare(family,id,original_wire,original_size,
        args.x,args.y,args.z,args.level,args.flag,trajectory ? flat : NULL,
        trajectory ? 30 : 0,output,sizeof output,&size)==0);
    assert(size==expected_size && memcmp(output,expected_wire,size)==0);
    assert(roswasmc_bounded_2_unitree_api_msg_Request_decode(output,size,&decoded)==0);
    assert(decoded.header.identity.id==original.header.identity.id);
    assert(decoded.header.lease.id==original.header.lease.id);
    assert(decoded.header.policy.priority==-101 && decoded.header.policy.noreply==0);
    assert(decoded.binary.size==4 && memcmp(decoded.binary.data,original.binary.data,4)==0);
}

int main(void) {
    alignas(16) unsigned char state[4096]={0}, copy[4096]={0}, before[4096];
    unsigned char request_wire[6181], response_wire[6172], output_before[6181];
    Request request;
    Response response={0};
    size_t n, request_size, response_size, size=123;
    uint32_t id=777, shape=888;
    unsigned i;
    assert(uwvm_unitree_packet_abi_version()==1);
    n=uwvm_unitree_packet_pending_bytes();
    assert(n<=sizeof state);
    original.header.identity.id=INT64_C(9007199254740993);
    original.header.identity.api_id=-77;
    original.header.lease.id=-INT64_C(9007199254740993);
    original.header.policy.priority=-101;
    original.parameter.size=4;
    memcpy(original.parameter.data,"keep",4);
    original.binary.size=4;
    memcpy(original.binary.data,"\0\xff\1\x80",4);
    assert(roswasmc_bounded_2_unitree_api_msg_Request_encode(
        &original,original_wire,sizeof original_wire,&original_size)==0);
    for(i=0;i<30;++i) {
        path[i]=(uwvm_unitree_b2_path_point){(float)i,0,-0.0f,0,0,0,0};
        flat[7*i]=(float)i; flat[7*i+2]=-0.0f;
    }
#define A_none ((uwvm_unitree_sport_arguments){0})
#define A_query A_none
#define A_xyz ((uwvm_unitree_sport_arguments){1.25f,-0.0f,0.5f,0,0})
#define A_position A_xyz
#define A_integer ((uwvm_unitree_sport_arguments){0,0,0,INT32_MIN,0})
#define A_boolean ((uwvm_unitree_sport_arguments){0,0,0,0,1})
#define A_height ((uwvm_unitree_sport_arguments){-0.125f,0,0,0,0})
#define A_trajectory A_none
#define UWVM_UNITREE_SPORT(name,value,kind) one(1,#name,value,A_##kind,0);
#include <platform/unitree/c/roswasm_unitree/sport_operations.inc>
#undef UWVM_UNITREE_SPORT
#define UWVM_UNITREE_B2(name,value,kind) one(2,#name,value,A_##kind,UWVM_UNITREE_B2_##kind==UWVM_UNITREE_B2_trajectory);
#include <platform/unitree/c/roswasm_unitree/b2_sport_operations.inc>
#undef UWVM_UNITREE_B2
    memset(output,0xa5,sizeof output); memcpy(output_before,output,sizeof output);
    assert(uwvm_unitree_packet_prepare(1,1001,original_wire,original_size,
        0,0,0,0,0,NULL,0,output,1,&size)==90);
    assert(size==123 && memcmp(output,output_before,sizeof output)==0);
    assert(uwvm_unitree_packet_prepare(1,1008,original_wire,original_size,
        NAN,0,0,0,0,NULL,0,output,sizeof output,&size)==22);
    assert(size==123 && memcmp(output,output_before,sizeof output)==0);
    assert(uwvm_unitree_packet_prepare(2,1018,original_wire,original_size,
        0,0,0,0,0,flat,29,output,sizeof output,&size)==22);
    assert(uwvm_unitree_packet_prepare(2,1001,original_wire,original_size,
        0,0,0,0,0,flat,30,output,sizeof output,&size)==22);
    assert(uwvm_unitree_packet_prepare(1,1001,original_wire,original_size,
        0,0,0,0,0,NULL,0,original_wire,sizeof original_wire,&size)==22);
    assert(uwvm_unitree_packet_lookup(3,"Damp",4,&id,&shape)==22);
    assert(id==777 && shape==888);
    assert(uwvm_unitree_packet_lookup(1,"Damp",4,&id,&id)==22 && id==777);
    assert(uwvm_unitree_packet_lookup(1,"AutoRecoveryGet",15,&id,&shape)==0 && id==2055 && shape==0);
    assert(uwvm_unitree_packet_lookup(2,"BodyHeight",10,&id,&shape)==0 && id==1013 && shape==4);
    assert(uwvm_unitree_packet_pending_init(state,n,1,0)==0);
    memcpy(before,state,n);
    assert(uwvm_unitree_packet_pending_init(state,n,1,0)==17);
    assert(memcmp(before,state,n)==0);
    request=original; request.header.identity.api_id=1001;
    for(i=1;i<=16;++i) {
        request.header.identity.id=i;
        assert(roswasmc_bounded_2_unitree_api_msg_Request_encode(
            &request,request_wire,sizeof request_wire,&request_size)==0);
        assert(uwvm_unitree_packet_pending_register(state,n,request_wire,request_size,10,100)==0);
    }
    request.header.identity.id=17;
    assert(roswasmc_bounded_2_unitree_api_msg_Request_encode(
        &request,request_wire,sizeof request_wire,&request_size)==0);
    memcpy(before,state,n);
    assert(uwvm_unitree_packet_pending_register(state,n,request_wire,request_size,10,100)==11);
    assert(memcmp(before,state,n)==0);
    response.header.identity.id=1; response.header.identity.api_id=1001;
    response.header.status.code=INT32_MIN;
    response.data.size=2; memcpy(response.data.data,"{}",2);
    response.binary.size=2; response.binary.data[0]=-128; response.binary.data[1]=127;
    assert(roswasmc_bounded_2_unitree_api_msg_Response_encode(
        &response,response_wire,sizeof response_wire,&response_size)==0);
    assert(uwvm_unitree_packet_pending_check(state,n,response_wire,response_size,9)==22);
    assert(uwvm_unitree_packet_pending_check(state,n,response_wire,response_size,99)==0);
    assert(memcmp(before,state,n)==0);
    memcpy(copy,state,n);
    assert(uwvm_unitree_packet_pending_check(copy,n,response_wire,response_size,99)==9);
    assert(uwvm_unitree_packet_pending_accept(state,n,response_wire,response_size,99)==0);
    assert(uwvm_unitree_packet_pending_accept(state,n,response_wire,response_size,99)==9);
    assert(uwvm_unitree_packet_pending_register(state,n,request_wire,request_size,98,200)==22);
    assert(uwvm_unitree_packet_pending_register(state,n,request_wire,request_size,99,200)==0);
    assert(uwvm_unitree_packet_pending_expire(state,n,2,1001,99)==11);
    assert(uwvm_unitree_packet_pending_expire(state,n,2,1001,100)==110);
    assert(uwvm_unitree_packet_pending_forget(state,n,17,1001)==0);
    assert(uwvm_unitree_packet_pending_register(state,n,request_wire,request_size,100,200)==17);
    assert(uwvm_unitree_packet_pending_close(state,n)==0);
    assert(uwvm_unitree_packet_pending_close(state,n)==0);
    assert(uwvm_unitree_packet_pending_register(state,n,request_wire,request_size,100,200)==9);
    assert(uwvm_unitree_packet_pending_close(copy,n)==9);
#if defined(UWVM_UNITREE_PACKET_JSON)
    query("{\"data\":true}",0,0,1);
    query("{\"data\":false,\"extra\":[1,2,3]}",0,0,0);
    query("{\"data\":1}",0,22,0);
    query("{\"data\":\"true\"}",0,22,0);
    query("{}",0,22,0);
    query("{\"data\":true}trailing",0,22,0);
    query("{\"data\":true,\"deep\":[[[[[[[[[[[[[[[[[]]]]]]]]]]]]]]]]]}",0,22,0);
    query("native failure need not contain Boolean JSON",INT32_MIN,0,0);
    puts("UNITREE_PACKET_JSON_LOCAL_OK");
#endif
    puts("UNITREE_PACKET_LOCAL_OK operations=60 fixed_slots=16");
    return 0;
}
