/* SPDX-License-Identifier: Apache-2.0 */
#pragma once
#include "custom_owned_services.h"
#include "native_rcl_service_registry.h"
#include <array>
#include <cstring>
#include <type_traits>
#include <new>
#if __has_include("custom_owned_service_features.h")
#include "custom_owned_service_features.h"
#endif
#ifndef UWVM_NATIVE_CUSTOM_SERVICE_METADATA_API
#define UWVM_NATIVE_CUSTOM_SERVICE_METADATA_API 0
#endif
#if UWVM_NATIVE_CUSTOM_SERVICE_METADATA_API && UWVM_ROS2_HAVE_TYPE_DESCRIPTION && UWVM_ROS2_HAVE_OPENSSL
#include <rosidl_typesupport_introspection_cpp/identifier.hpp>
#include <rosidl_typesupport_introspection_cpp/service_introspection.hpp>
#include <service_msgs/msg/service_event_info.hpp>
#include <builtin_interfaces/msg/time.hpp>
#endif
namespace uwvm2::ros_gateway::custom_owned_services::detail
{
    inline bool hexadecimal(std::string_view s,std::size_t n)noexcept
    {if(s.size()!=n)return false;for(char c:s)if(!((c>='0'&&c<='9')||(c>='a'&&c<='f')))return false;return true;}
    inline bool native_identity(std::string_view s)noexcept
    {return s.size()==71&&s.substr(0,7)=="RIHS01_"&&hexadecimal(s.substr(7),64);}
    template<class T,class=void>struct empty_native
    {static constexpr bool present=false;static bool zero(T const&)noexcept{return false;}};
    template<class T>struct empty_native<T,std::void_t<decltype(std::declval<T const&>().structure_needs_at_least_one_member)>>
    {using field=std::remove_cv_t<std::remove_reference_t<decltype(std::declval<T const&>().structure_needs_at_least_one_member)>>;static constexpr bool present=std::is_same<field,std::uint8_t>::value;
     static bool zero(T const& v)noexcept{return std::is_same<field,std::uint8_t>::value&&v.structure_needs_at_least_one_member==0;}};
    /** A private immutable typed proof. It cannot be made by a supplied bool,
     * packet or a native hash read back and called its own expected identity.
     * Every actual root/protocol/nested hash and source digest has an independent
     * immutable exported row. This object owns the descriptor address/lifetime.
     * All native metadata is warmed before Native init; no lazy code loading
     * or canonical-identity shortcut happens in business request callbacks. */
    template<class Service,class RequestCodec,class ResponseCodec>class CompiledCanonicalIdentity
    {
        static_assert(std::is_same<typename Service::Request,typename RequestCodec::Native>::value&&
            std::is_same<typename Service::Response,typename ResponseCodec::Native>::value,"actual complete native component types");
        static_assert(ros2_custom_topics::bounded_generated_codec<RequestCodec>::value&&ros2_custom_topics::bounded_generated_codec<ResponseCodec>::value,"complete bounded generated component codecs");
        std::array<uwvm_rmw_codec,3> codecs_{};ipc::custom_service_descriptor const* descriptor_{};
        row_identity row_{};export_view exports_{};
        explicit CompiledCanonicalIdentity(row_identity row,ipc::custom_service_descriptor const* d,export_view e):descriptor_{d},row_{row},exports_{e}{}
        template<class Codec>static std::uint32_t encode(void const* object,void* bytes,std::size_t capacity,std::size_t* length)noexcept
        {
            if(!object||!length||(!bytes&&capacity))return 22;
            try{auto const& actual=*static_cast<typename Codec::Native const*>(object);
                // Logical Empty0 does not authorize dropping its ORIGINAL IDL
                // dummy field. Older/no-dummy variants are unsupported here.
                if constexpr(Codec::wire_bytes==0){if(!empty_native<typename Codec::Native>::zero(actual))return 22;}
                std::size_t active{};if(!Codec::encoded_size(actual,active))return 22;
                if(active>Codec::wire_bytes||active>capacity)return 90;
                if(!Codec::encode(actual,static_cast<std::uint8_t*>(bytes),active))return 22;
                *length=active;return 0;
            }catch(std::bad_alloc const&){return 12;}catch(...){return 22;}
        }
        template<class Codec>static std::uint32_t decode(void const* bytes,std::size_t length,void* object)noexcept
        {
            if(!object||(!bytes&&length)||length>Codec::wire_bytes)return 22;
            try{return Codec::decode(static_cast<std::uint8_t const*>(bytes),length,*static_cast<typename Codec::Native*>(object))?0u:22u;}
            catch(std::bad_alloc const&){return 12;}catch(...){return 22;}
        }
        static std::uint32_t validate(uwvm_rmw_codec const* c,void const* support)noexcept
        {
            if(!c||!c->validation_state)return 22;auto s=static_cast<CompiledCanonicalIdentity const*>(c->validation_state);
            bool own{};for(auto const& actual:s->codecs_)if(&actual==c)own=support==actual.generated_support;
            if(!own)return 22;
#if UWVM_NATIVE_CUSTOM_SERVICE_METADATA_API && UWVM_ROS2_HAVE_TYPE_DESCRIPTION && UWVM_ROS2_HAVE_OPENSSL
            // Recheck the ORIGINAL complete graph at descriptor admission, not
            // merely the address of a formerly qualified metadata object.
            try{rosidl_service_type_support_t const* root{};
                return full_native(s->row_,s->exports_,root)&&root==s->codecs_[2].generated_support?0u:22u;
            }catch(std::bad_alloc const&){return 12;}catch(...){return 22;}
#else
            return 95;
#endif
        }
        static bool hash_bytes(std::string_view text,std::uint8_t* out)noexcept
        {
            if(!native_identity(text))return false;auto nibble=[](char c){return unsigned(c<='9'?c-'0':c-'a'+10);};
            for(std::size_t i{};i<32;++i)out[i]=static_cast<std::uint8_t>((nibble(text[7+2*i])<<4)|nibble(text[8+2*i]));return true;
        }
#if UWVM_NATIVE_CUSTOM_SERVICE_METADATA_API && UWVM_ROS2_HAVE_TYPE_DESCRIPTION && UWVM_ROS2_HAVE_OPENSSL
        using Message=rosidl_message_type_support_t;using Support=rosidl_service_type_support_t;
        using members=rosidl_typesupport_introspection_cpp::MessageMembers;
        static exported_identity const* expected(export_view const& e,std::string_view type)noexcept
        {for(std::size_t i{};i<e.count;++i)if(e.records[i].type==type)return e.records+i;return nullptr;}
        static bool counted(rosidl_runtime_c__String const& s,std::string_view v)noexcept
        {return s.size==v.size()&&s.capacity>=s.size&&(!s.size||(s.data&&!std::memcmp(s.data,v.data(),s.size)));}
        static bool subset(ipc::custom_native_schema all,std::string_view root,std::array<ipc::custom_native_record,256>& storage,std::size_t& count)
        {
            auto index=ros2_custom_topics::native_schema_index(all,root);if(index==all.record_count)return false;
            std::array<std::uint8_t,256> colors{},height{};if(!ros2_custom_topics::native_schema_visit(all,index,colors.data(),height.data(),1))return false;
            count=0;for(std::size_t i{};i<all.record_count;++i)if(colors[i])storage[count++]=all.records[i];return count!=0;
        }
        template<class H>static bool metadata(H const* h,std::string_view name,ipc::custom_native_schema schema,export_view const& exports)
        {
            auto id=expected(exports,name);if(!h||!id||!h->get_type_hash_func||!h->get_type_description_func||!h->get_type_description_sources_func||
                ros2_compat::baked_hash(h)!=id->native_identity)return false;
            std::array<ipc::custom_native_record,256> selected{};std::size_t count{};
            if(!subset(schema,name,selected,count))return false;ipc::custom_native_schema slice{selected.data(),count};
            auto d=h->get_type_description_func(h);if(!d||d->referenced_type_descriptions.size+1!=count||
                d->referenced_type_descriptions.capacity<d->referenced_type_descriptions.size||
                (d->referenced_type_descriptions.size&&!d->referenced_type_descriptions.data))return false;
            for(std::size_t i{};i<count;++i){auto const& want=selected[i];unsigned found{};
                if(counted(d->type_description.type_name,want.type)){++found;if(!ros2_custom_topics::native_record_matches(d->type_description,want))return false;}
                for(std::size_t j{};j<d->referenced_type_descriptions.size;++j)if(counted(d->referenced_type_descriptions.data[j].type_name,want.type))
                    {++found;if(!ros2_custom_topics::native_record_matches(d->referenced_type_descriptions.data[j],want))return false;}
                if(found!=1)return false;
            }
            // Compare the ACTUAL independently exported hash as well as the
            // complete declarations. No nonzero/hash-width criterion is proof.
            if(!ros2_compat::matches_rihs(slice,name,id->native_identity))return false;
            auto sources=h->get_type_description_sources_func(h);if(!sources||sources->size!=count||sources->capacity<sources->size||!sources->data)return false;
            std::array<bool,256> seen{};std::size_t total{};
            for(std::size_t i{};i<sources->size;++i){auto const& source=sources->data[i];std::size_t at{};
                for(;at<count;++at)if(counted(source.type_name,selected[at].type))break;
                if(at==count||seen[at])return false;seen[at]=true;auto original=expected(exports,selected[at].type);
                if(!original||!counted(source.encoding,original->encoding)||source.raw_file_contents.size>4194304||
                    source.raw_file_contents.capacity<source.raw_file_contents.size||(!source.raw_file_contents.data&&source.raw_file_contents.size)||
                    source.raw_file_contents.size>16777216-total)return false;
                total+=source.raw_file_contents.size;unsigned char actual[SHA256_DIGEST_LENGTH];
                auto p=reinterpret_cast<unsigned char const*>(source.raw_file_contents.data);
                static unsigned char const empty{};if(!::SHA256(p?p:&empty,source.raw_file_contents.size,actual)||
                    !native_io::matches_hexadecimal(actual,sizeof actual,original->source_sha256,""))return false;
            }
            return true;
        }
        static Message const* introspection(Message const* h)
        {
            if(!h||!h->typesupport_identifier)return nullptr;auto out=std::strcmp(h->typesupport_identifier,rosidl_typesupport_introspection_cpp::typesupport_identifier)==0?h:
                h->func?h->func(h,rosidl_typesupport_introspection_cpp::typesupport_identifier):nullptr;
            return out&&out->data&&out->typesupport_identifier&&std::strcmp(out->typesupport_identifier,rosidl_typesupport_introspection_cpp::typesupport_identifier)==0?out:nullptr;
        }
        static bool messages(Message const* h,std::string_view name,ipc::custom_native_schema schema,export_view const& exports,
            std::array<std::string_view,256>& seen,std::array<Message const*,256>& handles,std::size_t& count,unsigned depth)
        {
            if(depth>16||count==256)return false;auto actual=introspection(h);if(!actual||!metadata(h,name,schema,exports)||!metadata(actual,name,schema,exports))return false;
            for(std::size_t i{};i<count;++i)if(seen[i]==name)return handles[i]==actual;
            auto root=ros2_custom_topics::native_schema_index(schema,name);if(root==schema.record_count)return false;
            auto m=static_cast<members const*>(actual->data);
            if(!exports.typed_layout||!exports.typed_layout(name,m))return false;
            std::array<ipc::custom_native_record,256> selected{};std::size_t n{};if(!subset(schema,name,selected,n)||
                !ros2_compat::matches_schema(m,{selected.data(),n},name)||!m->init_function||!m->fini_function)return false;
            seen[count]=name;handles[count++]=actual;
            for(std::size_t i{};i<schema.records[root].field_count;++i){auto const& f=schema.records[root].fields[i];
                if(ros2_custom_topics::native_scalar_type(f.type))continue;
                if(!m->members_[i].members_||!messages(m->members_[i].members_,f.type,schema,exports,seen,handles,count,depth+1))return false;
            }
            return true;
        }
        /** Private layout comparison for an ALREADY fully proved modern graph.
         * It is called only after both actual generic/introspection complete
         * TypeDescriptions, exact default text, independently exported hashes
         * and whole source SHA, typed sizeof/offsets, all dependencies and real
         * Service root/Event callbacks have passed. Introspection default_value_
         * is deliberately NEVER read or dereferenced here: null and nonnull are
         * both UNKNOWN for declared default semantics, not alternative proofs.
         * The original public legacy matcher and no-default path stay strict.
         */
        static bool modern_layout_record(members const* actual,ipc::custom_native_record const& expected)
        {
            if(!actual||!ros2_compat::metadata_name(*actual,expected.type)||!actual->size_of_||
                actual->member_count_>4096||(expected.field_count&&!expected.fields))return false;
            bool canonical_empty=expected.field_count==1&&expected.fields&&
                expected.fields[0].name=="structure_needs_at_least_one_member"&&expected.fields[0].type=="uint8"&&
                !expected.fields[0].array_size&&!expected.fields[0].is_array&&!expected.fields[0].is_sequence&&
                !expected.fields[0].sequence_bound&&!expected.fields[0].string_bound;
            if(!expected.field_count||canonical_empty)
            {
                if(!actual->member_count_)return true;
                if(actual->member_count_!=1||!actual->members_)return false;
                auto const& dummy=actual->members_[0];
                return ros2_compat::native_string(dummy.name_,"structure_needs_at_least_one_member")&&dummy.offset_==0&&
                    dummy.type_id_==ros2_compat::scalar_type("uint8")&&!dummy.is_array_&&!dummy.is_upper_bound_&&
                    !dummy.array_size_&&!dummy.string_upper_bound_&&!dummy.members_;
            }
            if(actual->member_count_!=expected.field_count||!actual->members_)return false;
            for(std::size_t i{};i<expected.field_count;++i)
            {
                auto const& a=actual->members_[i];auto const& e=expected.fields[i];
                if(!ros2_compat::native_string(a.name_,e.name)||a.offset_>=actual->size_of_||
                    a.string_upper_bound_!=e.string_bound||a.is_array_!=(e.is_array||e.is_sequence))return false;
                if(e.is_sequence)
                {if(a.array_size_!=e.sequence_bound||a.is_upper_bound_!=(e.sequence_bound!=0))return false;}
                else if(a.array_size_!=(e.is_array?e.array_size:0)||a.is_upper_bound_)return false;
                auto type=ros2_compat::scalar_type(e.type);
                if(type){if(a.type_id_!=type||a.members_)return false;}
                else if(a.type_id_!=rosidl_typesupport_introspection_cpp::ROS_TYPE_MESSAGE||!a.members_||!a.members_->data)return false;
            }
            return true;
        }
        /** First phase: collect complete real metadata/layout attestations.
         * Do not compare/default-infer any introspection pointer in this phase.
         * Every indexed nested slot is bounded before it is read. The later
         * phase compares every field only after the ENTIRE graph is proved.
         */
        static bool proven_messages(Message const* h,std::string_view name,ipc::custom_native_schema schema,export_view const& exports,
            std::array<std::string_view,256>& seen,std::array<Message const*,256>& handles,std::size_t& count,unsigned depth)
        {
            if(depth>16||count==256)return false;
            auto actual=introspection(h);
            if(!actual||!metadata(h,name,schema,exports)||!metadata(actual,name,schema,exports))return false;
            for(std::size_t i{};i<count;++i)if(seen[i]==name)return handles[i]==actual;
            auto index=ros2_custom_topics::native_schema_index(schema,name);if(index==schema.record_count)return false;
            auto const& record=schema.records[index];auto m=static_cast<members const*>(actual->data);
            if(!exports.typed_layout||!exports.typed_layout(name,m)||!m->init_function||!m->fini_function)return false;
            bool canonical_empty=record.field_count==1&&record.fields&&
                record.fields[0].name=="structure_needs_at_least_one_member"&&record.fields[0].type=="uint8"&&
                !record.fields[0].array_size&&!record.fields[0].is_array&&!record.fields[0].is_sequence&&
                !record.fields[0].sequence_bound&&!record.fields[0].string_bound;
            if(!record.field_count||canonical_empty)
            {if(m->member_count_>1||(m->member_count_&&!m->members_))return false;}
            else if(m->member_count_!=record.field_count||!m->members_)return false;
            seen[count]=name;handles[count++]=actual;
            for(std::size_t i{};i<record.field_count;++i)
            {
                auto const& field=record.fields[i];if(ros2_custom_topics::native_scalar_type(field.type))continue;
                if(!m->members_||i>=m->member_count_||!m->members_[i].members_||
                    !proven_messages(m->members_[i].members_,field.type,schema,exports,seen,handles,count,depth+1))return false;
            }
            return true;
        }
        static bool full_native_defaults(row_identity row,export_view exports,ipc::custom_native_schema schema,Support const*& support)
        {
            // Defaults are not in RIHS01. Never substitute the original legacy
            // matcher or hash equality for the complete modern default proof.
            auto root_id=expected(exports,row.type);auto req_id=expected(exports,RequestCodec::type),resp_id=expected(exports,ResponseCodec::type);
            if(!root_id||!req_id||!resp_id||root_id->native_identity!=row.native||req_id->native_identity!=RequestCodec::native_identity||resp_id->native_identity!=ResponseCodec::native_identity)return false;
            support=rosidl_typesupport_cpp::get_service_type_support_handle<Service>();
            if(!support||!metadata(support,row.type,schema,exports)||!support->func)return false;
            auto native=support->func(support,rosidl_typesupport_introspection_cpp::typesupport_identifier);
            if(!native||!native->data||!native->typesupport_identifier||
                std::strcmp(native->typesupport_identifier,rosidl_typesupport_introspection_cpp::typesupport_identifier)||
                !metadata(native,row.type,schema,exports))return false;
            auto req=rosidl_typesupport_cpp::get_message_type_support_handle<typename Service::Request>();
            auto resp=rosidl_typesupport_cpp::get_message_type_support_handle<typename Service::Response>();
            auto event=rosidl_typesupport_cpp::get_message_type_support_handle<typename Service::Event>();
            auto a=introspection(req),b=introspection(resp),e=introspection(event);
            if(!a||!b||!e||static_cast<members const*>(a->data)->size_of_!=sizeof(typename Service::Request)||
                static_cast<members const*>(b->data)->size_of_!=sizeof(typename Service::Response)||
                static_cast<members const*>(e->data)->size_of_!=sizeof(typename Service::Event))return false;
            auto m=static_cast<rosidl_typesupport_introspection_cpp::ServiceMembers const*>(native->data);
            auto pair=[](Message const* p,Message const* g,Message const* n){return p==g||p==n;};
            // Establish actual root/component/Event ownership and both genuine
            // constructor/destructor callbacks BEFORE the private comparison
            // may disregard an ambiguous introspection default pointer.
            if(m->request_members_!=a->data||m->response_members_!=b->data||m->event_members_!=e->data||
                !pair(support->request_typesupport,req,a)||!pair(support->response_typesupport,resp,b)||!pair(support->event_typesupport,event,e)||
                native->request_typesupport!=a||native->response_typesupport!=b||native->event_typesupport!=e||
                !support->event_message_create_handle_function||!support->event_message_destroy_handle_function||
                support->event_message_create_handle_function!=native->event_message_create_handle_function||
                support->event_message_destroy_handle_function!=native->event_message_destroy_handle_function)return false;
            std::string event_name=std::string(row.type)+"_Event";
            std::array<std::string_view,256> seen{};std::array<Message const*,256> handles{};std::size_t count{};
            if(!proven_messages(req,RequestCodec::type,schema,exports,seen,handles,count,1)||
                !proven_messages(resp,ResponseCodec::type,schema,exports,seen,handles,count,1)||
                !proven_messages(event,event_name,schema,exports,seen,handles,count,1)||count+1!=schema.record_count)return false;
            // Complete connected coverage, original handle agreement and ALL
            // generic/introspection default/source/hash/layout checks are now
            // established. Only here does nullable/non-nullable become UNKNOWN.
            for(std::size_t i{};i<count;++i)
            {
                auto index=ros2_custom_topics::native_schema_index(schema,seen[i]);
                if(index==schema.record_count||!modern_layout_record(static_cast<members const*>(handles[i]->data),schema.records[index]))return false;
            }
            return true;
        }
        static bool full_native(row_identity row,export_view exports,Support const*& support)
        {
            auto schema=ipc::custom_native_interface_schema<Service>::schema();if(!schema.records||!schema.record_count||schema.record_count>256||
                !exports.records||!exports.typed_layout||exports.count!=schema.record_count||!hexadecimal(exports.actual_export_sha256,64)||!hexadecimal(exports.actual_sdk_receipt_sha256,64))return false;
            std::size_t members_total{};bool declared_defaults{};
            for(std::size_t i{};i<schema.record_count;++i){auto const& r=schema.records[i];if(r.type.empty()||r.field_count>4096||(!r.fields&&r.field_count)||
                r.field_count>16384-members_total)return false;members_total+=r.field_count;
                auto id=expected(exports,r.type);if(!id||!native_identity(id->native_identity)||!hexadecimal(id->source_sha256,64)||
                    (id->encoding!="msg"&&id->encoding!="srv"&&id->encoding!="implicit"))return false;
                for(std::size_t j{};j<i;++j)if(schema.records[j].type==r.type)return false;
                bool record_defaults{};for(std::size_t f{};f<r.field_count;++f)record_defaults=record_defaults||!r.fields[f].native_default.empty();
                if(record_defaults&&!ros2_custom_topics::native_default_variant(r))return false;
                declared_defaults=declared_defaults||record_defaults;
                for(std::size_t j{};j<i;++j)if(exports.records[j].type==exports.records[i].type)return false;
            }
            // Every export row belongs to the exact graph, including Event,
            // Time/Info and nested dependencies. No disconnected extra hash.
            for(std::size_t i{};i<exports.count;++i)if(ros2_custom_topics::native_schema_index(schema,exports.records[i].type)==schema.record_count)return false;
            if(declared_defaults)return full_native_defaults(row,exports,schema,support);
            // Original no-default proof continues below, byte for byte.
            auto root_id=expected(exports,row.type);auto req_id=expected(exports,RequestCodec::type),resp_id=expected(exports,ResponseCodec::type);
            if(!root_id||!req_id||!resp_id||root_id->native_identity!=row.native||req_id->native_identity!=RequestCodec::native_identity||resp_id->native_identity!=ResponseCodec::native_identity)return false;
            support=rosidl_typesupport_cpp::get_service_type_support_handle<Service>();if(!support||!metadata(support,row.type,schema,exports)||!support->func)return false;
            auto native=support->func(support,rosidl_typesupport_introspection_cpp::typesupport_identifier);
            if(!native||!native->data||!native->typesupport_identifier||std::strcmp(native->typesupport_identifier,rosidl_typesupport_introspection_cpp::typesupport_identifier)||!metadata(native,row.type,schema,exports))return false;
            auto req=rosidl_typesupport_cpp::get_message_type_support_handle<typename Service::Request>();
            auto resp=rosidl_typesupport_cpp::get_message_type_support_handle<typename Service::Response>();
            auto event=rosidl_typesupport_cpp::get_message_type_support_handle<typename Service::Event>();
            std::string event_name=std::string(row.type)+"_Event";
            std::array<std::string_view,256> seen{};std::array<Message const*,256> handles{};std::size_t count{};
            if(!messages(req,RequestCodec::type,schema,exports,seen,handles,count,1)||!messages(resp,ResponseCodec::type,schema,exports,seen,handles,count,1)||
                !messages(event,event_name,schema,exports,seen,handles,count,1)||count+1!=schema.record_count)return false;
            auto a=introspection(req),b=introspection(resp),e=introspection(event);
            if(!a||!b||!e||static_cast<members const*>(a->data)->size_of_!=sizeof(typename Service::Request)||
                static_cast<members const*>(b->data)->size_of_!=sizeof(typename Service::Response)||
                static_cast<members const*>(e->data)->size_of_!=sizeof(typename Service::Event))return false;
            auto m=static_cast<rosidl_typesupport_introspection_cpp::ServiceMembers const*>(native->data);
            auto pair=[](Message const* p,Message const* g,Message const* n){return p==g||p==n;};
            return a&&b&&e&&m->request_members_==a->data&&m->response_members_==b->data&&m->event_members_==e->data&&
                pair(support->request_typesupport,req,a)&&pair(support->response_typesupport,resp,b)&&pair(support->event_typesupport,event,e)&&
                native->request_typesupport==a&&native->response_typesupport==b&&native->event_typesupport==e&&
                support->event_message_create_handle_function&&support->event_message_destroy_handle_function&&
                support->event_message_create_handle_function==native->event_message_create_handle_function&&support->event_message_destroy_handle_function==native->event_message_destroy_handle_function;
        }
#endif
    public:
        CompiledCanonicalIdentity(CompiledCanonicalIdentity const&)=delete;CompiledCanonicalIdentity& operator=(CompiledCanonicalIdentity const&)=delete;
        uwvm_rmw_codec const* root()const noexcept{return codecs_.data()+2;}
        ipc::custom_service_descriptor const& descriptor()const noexcept{return *descriptor_;}
        row_identity row()const noexcept{return row_;}
        bool selected(ros::capability_spec const& cap)const
        {
            auto p=cap.limits.find("adapter-codec-identity");
            return p!=cap.limits.end()&&p->second==row_.canonical&&cap.type==row_.type&&cap.type_identity==row_.native&&
                native::explicit_qos(cap)&&field(cap,"validator")=="none"&&!cap.alias.empty()&&!cap.resource.empty()&&cap.resource.front()=='/'&&
                (cap.operation=="service.call"||cap.operation=="service.advertise"||cap.operation=="service.respond")&&
                limit(cap,"max-request-bytes")>=RequestCodec::wire_bytes&&limit(cap,"max-response-bytes")>=ResponseCodec::wire_bytes&&
                limit(cap,"max-inflight")&&limit(cap,"max-inflight")<=256&&limit(cap,"timeout-ms")&&limit(cap,"timeout-ms")<=3600000;
        }
        static std::uint32_t qualify(row_identity row,std::shared_ptr<CompiledCanonicalIdentity const>& output)noexcept
        {
#if !UWVM_NATIVE_CUSTOM_SERVICE_METADATA_API || !UWVM_ROS2_HAVE_TYPE_DESCRIPTION || !UWVM_ROS2_HAVE_OPENSSL
            (void)row;(void)output;return 95;
#else
            try{auto d=ipc::find_custom_service(2,row.type);auto exports=independent_native_exports<Service>::get();
                if(!exports.records||!exports.count||!exports.typed_layout)return 95;
                if constexpr(RequestCodec::wire_bytes==0){if(!empty_native<typename Service::Request>::present)return 95;}
                if constexpr(ResponseCodec::wire_bytes==0){if(!empty_native<typename Service::Response>::present)return 95;}
                if(!d||d->native_identity!=row.native||d->codec_identity!=row.canonical||!d->request||!d->response||
                    d->request->type!=RequestCodec::type||d->response->type!=ResponseCodec::type||
                    d->request->native_identity!=RequestCodec::native_identity||d->response->native_identity!=ResponseCodec::native_identity||
                    d->request->codec_identity!=RequestCodec::codec_identity||d->response->codec_identity!=ResponseCodec::codec_identity||
                    d->request->wire_bytes!=RequestCodec::wire_bytes||d->response->wire_bytes!=ResponseCodec::wire_bytes||
                    RequestCodec::wire_bytes>32768||ResponseCodec::wire_bytes>32768)return 13;
                Support const* support{};if(!full_native(row,exports,support))return 13;
                auto value=std::shared_ptr<CompiledCanonicalIdentity>{new CompiledCanonicalIdentity{row,d,exports}};
                auto req=rosidl_typesupport_cpp::get_message_type_support_handle<typename Service::Request>();
                auto resp=rosidl_typesupport_cpp::get_message_type_support_handle<typename Service::Response>();
                auto& q=value->codecs_[0];q.version=1;q.kind=UWVM_RMW_MESSAGE_CODEC;q.native_type=RequestCodec::type;q.generated_support=req;
                hash_bytes(RequestCodec::native_identity,q.native_rihs01);q.maximum_wire_bytes=RequestCodec::wire_bytes;q.validate=validate;q.validation_state=value.get();q.encode=encode<RequestCodec>;q.decode=decode<RequestCodec>;
                auto& s=value->codecs_[1];s.version=1;s.kind=UWVM_RMW_MESSAGE_CODEC;s.native_type=ResponseCodec::type;s.generated_support=resp;
                hash_bytes(ResponseCodec::native_identity,s.native_rihs01);s.maximum_wire_bytes=ResponseCodec::wire_bytes;s.validate=validate;s.validation_state=value.get();s.encode=encode<ResponseCodec>;s.decode=decode<ResponseCodec>;
                auto& r=value->codecs_[2];r.version=1;r.kind=UWVM_RMW_SERVICE_CODEC;r.native_type=d->type.data();r.generated_support=support;
                hash_bytes(row.native,r.native_rihs01);r.maximum_wire_bytes=std::max(q.maximum_wire_bytes,s.maximum_wire_bytes);r.request=&q;r.response=&s;r.validate=validate;r.validation_state=value.get();
                output=std::move(value);return 0;
            }catch(std::bad_alloc const&){return 12;}catch(...){return 13;}
#endif
        }
    };
}
