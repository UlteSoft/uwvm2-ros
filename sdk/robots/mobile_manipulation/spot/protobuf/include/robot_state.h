/* SPDX-License-Identifier: Apache-2.0 */
#ifndef UWVM_SPOT_PROTOBUF_ROBOT_STATE_H
#define UWVM_SPOT_PROTOBUF_ROBOT_STATE_H
#include "document.h"
#include "bosdyn/api/robot_state.pb.h"
#include "bosdyn/api/robot_command.pb.h"
#include <string.h>
static inline const uwvm_spot_proto_type *uwvm_spot_proto_type_named(const char *name) {
    if(!name) return NULL;
    for(size_t i=0;i<uwvm_spot_proto_type_count;++i)
        if(strcmp(name,uwvm_spot_proto_types[i].schema->full_name)==0)
            return &uwvm_spot_proto_types[i];
    return NULL;
}
static inline uint32_t uwvm_spot_robot_state_decode(uwvm_spot_proto_document *doc,
                                                   const void *bytes,size_t size) {
    return uwvm_spot_proto_decode(doc,uwvm_spot_proto_type_named("bosdyn.api.RobotState"),bytes,size);
}
static inline const bosdyn_api_RobotState *uwvm_spot_robot_state_value(
    const uwvm_spot_proto_document *doc) {
    if(!doc||doc->type!=uwvm_spot_proto_type_named("bosdyn.api.RobotState")) return NULL;
    return (const bosdyn_api_RobotState *)doc->value;
}
#endif
