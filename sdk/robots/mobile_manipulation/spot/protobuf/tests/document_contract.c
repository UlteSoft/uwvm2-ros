/* SPDX-License-Identifier: Apache-2.0 */
#ifdef NDEBUG
#error Contract assertions must remain active
#endif
#include "robot_state.h"
#include "bosdyn/api/signals.pb.h"
#include "google/protobuf/struct.pb.h"
#include <assert.h>
#include <stdalign.h>
#include <stdint.h>
#include <string.h>
static alignas(max_align_t) unsigned char a[65536],b[65536];
int main(void) {
    uwvm_spot_proto_document doc;
    assert(uwvm_spot_proto_init(&doc,a,b,sizeof a)==0);
    /* The fixed production corpus is proto3: an empty message is valid for
     * every original descriptor. Exercise all generated C bindings, including
     * nested/map-entry types, rather than testing only the RobotState wrapper.
     * A reserved field-zero tag must refuse before replacing that entire value.
     * These are SDK decoder contracts, not official vendor/robot test results. */
    assert(uwvm_spot_proto_type_count==1319);
    for(size_t i=0;i<uwvm_spot_proto_type_count;++i) {
        const uwvm_spot_proto_type *type=&uwvm_spot_proto_types[i];
        assert(uwvm_spot_proto_decode(&doc,type,NULL,0)==0);
        assert(doc.type==type&&doc.value&&doc.wire_size==0);
        const void *complete=doc.value;
        const unsigned char reserved_field_zero[]={0};
        assert(uwvm_spot_proto_decode(&doc,type,reserved_field_zero,
                                     sizeof reserved_field_zero)==22);
        assert(doc.type==type&&doc.value==complete&&doc.wire_size==0);
    }
    assert(uwvm_spot_proto_clear(&doc)==0);
    /* Original keyword-named oneof members retain their wire tags. The actual
     * generator naming strategy changes only their C spelling, never payloads. */
    const uwvm_spot_proto_type *signal=uwvm_spot_proto_type_named("bosdyn.api.SignalData.Data");
    const unsigned char signal_double[]={0x09,0,0,0,0,0,0x40,0x48,0x40};
    assert(uwvm_spot_proto_decode(&doc,signal,signal_double,sizeof signal_double)==0);
    const bosdyn_api_SignalData_Data *data=(const bosdyn_api_SignalData_Data *)doc.value;
    assert(data->which_value==1&&data->value.double_&&*data->value.double_==48.5);
    const unsigned char signal_bool[]={0x20,1};
    assert(uwvm_spot_proto_decode(&doc,signal,signal_bool,sizeof signal_bool)==0);
    data=(const bosdyn_api_SignalData_Data *)doc.value;
    assert(data->which_value==4&&data->value.bool_&&*data->value.bool_);
    const unsigned char signal_int[]={0x10,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,1};
    assert(uwvm_spot_proto_decode(&doc,signal,signal_int,sizeof signal_int)==0);
    data=(const bosdyn_api_SignalData_Data *)doc.value;
    assert(data->which_value==2&&data->value.int_&&*data->value.int_==-1);
    /* Original RobotState.battery_states[0].identifier="a\0b". */
    const unsigned char nul[]={0x12,5,0x12,3,'a',0,'b'};
    assert(uwvm_spot_robot_state_decode(&doc,nul,sizeof nul)==0);
    const bosdyn_api_RobotState *state=uwvm_spot_robot_state_value(&doc);
    assert(state&&state->battery_states_count==1);
    const pb_bytes_array_t *id=state->battery_states[0].identifier;
    assert(id&&id->size==3&&!memcmp(id->bytes,"a\0b",3));
    const void *prior=doc.value;
    const unsigned char invalid_utf8[]={0x12,4,0x12,2,0xc0,0x80};
    assert(uwvm_spot_robot_state_decode(&doc,invalid_utf8,sizeof invalid_utf8)==22);
    assert(doc.value==prior&&!memcmp(doc.wire,nul,sizeof nul));
    /* Original proto3 unknown negative enum is representable as int32. */
    const unsigned char unknown_enum[]={0x12,11,0x40,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,1};
    assert(uwvm_spot_robot_state_decode(&doc,unknown_enum,sizeof unknown_enum)==0);
    state=uwvm_spot_robot_state_value(&doc);
    assert(state->battery_states[0].status&&*state->battery_states[0].status==-1);
    const unsigned char unknown_field[]={0xa0,6,7}; /* field100=7 */
    assert(uwvm_spot_robot_state_decode(&doc,unknown_field,sizeof unknown_field)==0);
    assert(doc.wire_size==sizeof unknown_field&&!memcmp(doc.wire,unknown_field,sizeof unknown_field));
    prior=doc.value;
    assert(uwvm_spot_robot_state_decode(&doc,a,UWVM_SPOT_PROTO_WIRE_MAX+1)==90);
    assert(doc.value==prior);
    uwvm_spot_proto_type forged=*uwvm_spot_proto_type_named("bosdyn.api.RobotState");
    assert(uwvm_spot_proto_decode(&doc,&forged,nul,sizeof nul)==22&&doc.value==prior);
    /* Exactly counted recursive Value.list_value/values wrappers exceed32.
     * Preflight rejects before calling nanopb or touching the active bank. */
    unsigned char deep[128]={0};size_t length=0;
    for(unsigned i=0;i<33;++i) {
        memmove(deep+2,deep,length);deep[0]=(i%2)?0x0a:0x32;
        deep[1]=(unsigned char)length;length+=2;
    }
    const uwvm_spot_proto_type *value=uwvm_spot_proto_type_named("google.protobuf.Value");
    assert(uwvm_spot_proto_decode(&doc,value,deep,length)==90&&doc.value==prior);
    /* Each repeated empty BatteryState still needs a full typed C object.
     * Arena exhaustion is90, not a partial decoded success or truncation. */
    unsigned char many[4096];
    for(size_t i=0;i<sizeof many;i+=2){many[i]=0x12;many[i+1]=0;}
    assert(uwvm_spot_robot_state_decode(&doc,many,sizeof many)==90&&doc.value==prior);

    /* Same singular message occurs twice: protobuf merges distinct children. */
    const unsigned char merged_power[]={0x0a,2,0x10,2,0x0a,2,0x18,2};
    assert(uwvm_spot_robot_state_decode(&doc,merged_power,sizeof merged_power)==0);
    state=uwvm_spot_robot_state_value(&doc);
    assert(state->power_state&&state->power_state->motor_power_state&&state->power_state->shore_power_state);
    assert(*state->power_state->motor_power_state==2&&*state->power_state->shore_power_state==2);
    /* Repeated occurrences of the SAME oneof message merge repeated children. */
    const unsigned char merged_oneof[]={0x32,4,0x0a,2,0x20,1,0x32,4,0x0a,2,0x20,0};
    assert(uwvm_spot_proto_decode(&doc,value,merged_oneof,sizeof merged_oneof)==0);
    const google_protobuf_Value *oneof=(const google_protobuf_Value *)doc.value;
    assert(oneof->which_kind==6&&oneof->kind.list_value&&oneof->kind.list_value->values_count==2);
    assert(oneof->kind.list_value->values[0].which_kind==4&&
           *oneof->kind.list_value->values[0].kind.bool_value);
    assert(oneof->kind.list_value->values[1].which_kind==4&&
           !*oneof->kind.list_value->values[1].kind.bool_value);
    /* Different oneof members still replace and release, without stale fields. */
    const unsigned char changed_oneof[]={0x32,4,0x0a,2,0x20,1,0x20,1};
    assert(uwvm_spot_proto_decode(&doc,value,changed_oneof,sizeof changed_oneof)==0);
    oneof=(const google_protobuf_Value *)doc.value;
    assert(oneof->which_kind==4&&oneof->kind.bool_value&&*oneof->kind.bool_value);
    prior=doc.value;
    const unsigned char bad_merge[]={0x0a,2,0x10,2,0x0a,1,0x18};
    assert(uwvm_spot_robot_state_decode(&doc,bad_merge,sizeof bad_merge)==22&&doc.value==prior);
    assert(uwvm_spot_proto_clear(&doc)==0&&doc.value==NULL);
    return 0;
}
