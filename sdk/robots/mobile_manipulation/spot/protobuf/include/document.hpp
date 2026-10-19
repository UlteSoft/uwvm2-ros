/* SPDX-License-Identifier: Apache-2.0 */
#ifndef UWVM_SPOT_PROTOBUF_DOCUMENT_HPP
#define UWVM_SPOT_PROTOBUF_DOCUMENT_HPP
#include "schema.h"
#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/io/zero_copy_stream_impl_lite.h>
#include <array>
#include <cstdint>
#include <cstring>
#include <memory>
#include <new>
#include <string>
#include <type_traits>
#include "bosdyn/api/robot_state.pb.h"
#include "bosdyn/api/robot_command.pb.h"

namespace uwvm::spot::protobuf {
/* Literal original names resolve ONLY against generated original descriptors.
 * This is schema selection, not native RIHS proof or ROS authorization. */
inline const uwvm_spot_proto_schema *schema(char const *name) noexcept {
    if(!name) return nullptr;
    for(std::size_t i=0;i<uwvm_spot_proto_schema_count;++i)
        if(std::strcmp(name,uwvm_spot_proto_schemas[i].full_name)==0)
            return &uwvm_spot_proto_schemas[i];
    return nullptr;
}
/* The official Lite parser owns the full original generated object graph.
 * Wire/depth/work are bounded before allocation. Unlike C's bank allocator,
 * Arena blocks DO NOT bound std::string allocations. This API does not claim
 * a per-document C++ heap cap. Actual Guest linear memory/stack limits remain
 * mandatory; allocator abort/trap is a terminal Guest outcome, not errno0.
 * Ordinary std::bad_alloc is returned90 without replacing the old document.
 *
 * The object is exposed const: use a separate actual generated message for
 * edits. Do not cast away const or share one document between threads. */
template<class Message> class Document {
    static_assert(std::is_base_of_v<::google::protobuf::MessageLite,Message>);
    std::unique_ptr<Message> object_;
    std::array<std::uint8_t,UWVM_SPOT_PROTO_WIRE_MAX> wire_{};
    std::size_t size_{};
public:
    Document()=default;
    Document(Document const&)=delete;
    Document& operator=(Document const&)=delete;
    Document(Document&&)=delete;
    Document& operator=(Document&&)=delete;
    std::uint32_t decode(void const *input,std::size_t length) {
        if((!input&&length)||length>wire_.size()) return length>wire_.size()?90:22;
        try {
            auto next=std::make_unique<Message>();
            auto const name=next->GetTypeName();
            auto const *row=schema(name.c_str());
            if(!row) return 95;
            auto status=uwvm_spot_proto_preflight(row,input,length);
            if(status) return status;
            std::array<std::uint8_t,UWVM_SPOT_PROTO_WIRE_MAX> captured{};
            if(length) std::memcpy(captured.data(),input,length);
            ::google::protobuf::io::ArrayInputStream raw(captured.data(),static_cast<int>(length));
            ::google::protobuf::io::CodedInputStream stream(&raw);
            stream.SetTotalBytesLimit(UWVM_SPOT_PROTO_WIRE_MAX);
            stream.SetRecursionLimit(UWVM_SPOT_PROTO_DEPTH_MAX);
            if(!next->ParseFromCodedStream(&stream)||!stream.ConsumedEntireMessage()) return 22;
            /* All fallible work precedes replacement. Unique ownership swap
             * and fixed byte copies cannot invoke ROS or user callbacks. */
            object_.swap(next);
            if(length) std::memcpy(wire_.data(),captured.data(),length);
            size_=length;
            return 0;
        } catch(std::bad_alloc const&) {return 90;}
    }
    Message const *get() const noexcept {return object_.get();}
    std::uint8_t const *original_wire() const noexcept {return object_?wire_.data():nullptr;}
    std::size_t original_size() const noexcept {return size_;}
    void clear() noexcept {object_.reset();size_=0;}
};
using RobotState=Document<::bosdyn::api::RobotState>;
using CommandFeedback=Document<::bosdyn::api::RobotCommandFeedbackResponse>;
} // namespace uwvm::spot::protobuf
#endif
