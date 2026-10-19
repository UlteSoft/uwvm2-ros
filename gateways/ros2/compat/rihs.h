#pragma once
#include "features.h"
#include <uwvm2/imported/ros/ipc/custom_type.h>
#include "../../common/native_io.h"
#include <algorithm>
#include <array>
#include <charconv>
#include <string>
#include <string_view>
#include <utility>
#ifndef UWVM_ROS2_HAVE_OPENSSL
#define UWVM_ROS2_HAVE_OPENSSL 0
#endif
#if UWVM_ROS2_HAVE_OPENSSL
#include <openssl/sha.h>
#endif

namespace uwvm2::ros_gateway::ros2_compat
{
    /* Reproduce the public RIHS01 canonical representation, not a native hash
     * that a pre-Iron SDK never had. Algorithm and field IDs are from official
     * ros2/rosidl jazzy rosidl_generator_type_description/__init__.py:200-478.
     * A caller MUST first attest every actual native component with the full
     * introspection schema. A Service/Action needs the complete official root
     * and protocol/event dependency schemas; only hashing its 2/3 payloads is
     * insufficient. The trusted generator supplies those extra declarations.
     * This startup-only representation is bounded to 4 MiB; no per-message
     * hashing, crypto implementation, Guest-selected type loading or new grant.
     */
    inline unsigned rihs_scalar(std::string_view name)
    {
        if(name=="int8") { return 2; }if(name=="uint8") { return 3; }
        if(name=="int16") { return 4; }if(name=="uint16") { return 5; }
        if(name=="int32") { return 6; }if(name=="uint32") { return 7; }
        if(name=="int64") { return 8; }if(name=="uint64") { return 9; }
        if(name=="float32") { return 10; }if(name=="float64") { return 11; }
        if(name=="char") { return 13; }if(name=="bool") { return 15; }
        if(name=="byte") { return 16; }if(name=="string") { return 17; }return 0;
    }
    class rihs_json
    {
        std::string body_;bool valid_{true};
    public:
        void add(std::string_view value)
        { if(value.size()>4*1024*1024-body_.size()) { valid_=false;return; }body_.append(value); }
        void number(std::uint64_t value)
        { char buffer[24];auto result{std::to_chars(buffer,buffer+sizeof buffer,value)};if(result.ec!=std::errc{}) {valid_=false;return;}add({buffer,static_cast<std::size_t>(result.ptr-buffer)}); }
        void name(std::string_view value)
        {
            add("\"");
            // ROS field/type names have only this ASCII vocabulary. Refuse an
            // unrepresentable declaration instead of escaping a second grammar.
            for(char c:value)
            { if(!((c>='a'&&c<='z')||(c>='A'&&c<='Z')||(c>='0'&&c<='9')||c=='_'||c=='/')) { valid_=false;return; } }
            add(value);add("\"");
        }
        bool valid() const noexcept { return valid_; }
        std::string const& body() const noexcept { return body_; }
        // Transfer a completed bounded export without another whole-body copy.
        std::string release() && noexcept { return std::move(body_); }
        void record(::uwvm2::imported::ros::ipc::custom_native_record const& record)
        {
            add("{\"type_name\": ");name(record.type);add(", \"fields\": [");
            for(std::size_t index{};index!=record.field_count;++index)
            {
                auto const& field{record.fields[index]};if(index) { add(", "); }
                unsigned type{rihs_scalar(field.type)};bool nested{type==0};if(nested) { type=1; }
                if(field.type=="string"&&field.string_bound) { type=21; }
                if(field.is_array&&field.is_sequence) { valid_=false;return; }
                if(field.is_array) { type+=48; }else if(field.is_sequence) { type+=field.sequence_bound?96:144; }
                add("{\"name\": ");name(field.name);add(", \"type\": {\"type_id\": ");number(type);
                add(", \"capacity\": ");number(field.is_array?field.array_size:(field.is_sequence?field.sequence_bound:0));
                add(", \"string_capacity\": ");number(field.string_bound);add(", \"nested_type_name\": ");name(nested?field.type:std::string_view{});add("}}");
            }
            add("]}");
        }
    };
    /** Hash a complete declaration only after its caller has independently
     * checked the actual installed native schema. This is a RIHS01 derivation,
     * never evidence that an old type-support struct contains a baked hash.
     * On failure the caller's digest remains unchanged. No per-message use. */
    inline bool rihs_digest(::uwvm2::imported::ros::ipc::custom_native_schema schema,std::string_view root,
        std::array<unsigned char,32>& output)
    {
#if UWVM_ROS2_HAVE_OPENSSL
        if(!schema.records||!schema.record_count||schema.record_count>256) { return false; }
        std::array<std::size_t,256> references{};std::array<unsigned char,256> state{};std::size_t count{},selected{schema.record_count};
        for(std::size_t i{};i!=schema.record_count;++i)
        {
            if(schema.records[i].type==root) { selected=i; }
            for(std::size_t j{};j!=i;++j) { if(schema.records[i].type==schema.records[j].type) { return false; } }
        }
        if(selected==schema.record_count) { return false; }
        auto visit=[&](auto&& self,std::size_t index,unsigned depth)->bool {
            if(depth>16||state[index]==1) { return false; }if(state[index]==2) { return true; }
            auto const& record{schema.records[index]};if(record.field_count>4096||(!record.fields&&record.field_count)) { return false; }state[index]=1;
            for(std::size_t i{};i!=record.field_count;++i)
            {
                auto const& field{record.fields[i]};if(rihs_scalar(field.type)) { continue; }
                std::size_t next{};while(next<schema.record_count&&schema.records[next].type!=field.type) { ++next; }
                if(next==schema.record_count||!self(self,next,depth+1)) { return false; }
            }
            state[index]=2;if(index!=selected) { references[count++]=index; }return true;
        };
        if(!visit(visit,selected,1)) { return false; }
        for(std::size_t i{};i!=schema.record_count;++i) { if(state[i]!=2) { return false; } }
        std::sort(references.begin(),references.begin()+count,[&](auto a,auto b) { return schema.records[a].type<schema.records[b].type; });
        rihs_json json;json.add("{\"type_description\": ");json.record(schema.records[selected]);json.add(", \"referenced_type_descriptions\": [");
        for(std::size_t i{};i!=count;++i) { if(i) { json.add(", "); }json.record(schema.records[references[i]]); }json.add("]}");
        if(!json.valid()) { return false; }unsigned char digest[SHA256_DIGEST_LENGTH];
        if(!::SHA256(reinterpret_cast<unsigned char const*>(json.body().data()),json.body().size(),digest)) { return false; }
        static_assert(SHA256_DIGEST_LENGTH==32,"RIHS01 requires a SHA256 digest");
        std::copy(digest,digest+sizeof digest,output.begin());return true;
#else
        (void)schema;(void)root;(void)output;return false;
#endif
    }
    /** Construct a derived identity for a previously attested native schema.
     * Empty means unavailable/refused, including a build without OpenSSL. */
    inline std::string rihs_identity(::uwvm2::imported::ros::ipc::custom_native_schema schema,std::string_view root)
    {
        std::array<unsigned char,32> digest{};
        if(!rihs_digest(schema,root,digest)) { return {}; }
        return native_io::hexadecimal(digest.data(),digest.size(),"RIHS01_");
    }
    inline bool matches_rihs(::uwvm2::imported::ros::ipc::custom_native_schema schema,std::string_view root,std::string_view expected)
    {
        if(expected.size()!=71||expected.substr(0,7)!="RIHS01_") { return false; }
        std::array<unsigned char,32> digest{};
        return rihs_digest(schema,root,digest)&&
            native_io::matches_hexadecimal(digest.data(),digest.size(),expected,"RIHS01_");
    }
}
