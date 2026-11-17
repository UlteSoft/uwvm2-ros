/** @file
 * @brief Bind a frozen bounded CDR schema to an approved generated ROS 2 type.
 * @details Only native worker code supplies type-support pointers. Preparation
 * compares the exact top-level ROS identity, ordered field names, primitive
 * kinds, nested records and array/string bounds with the generated C++
 * introspection metadata. A matching name alone never admits a representation.
 * Keyed and unsupported representations reject. No offset, callback or library
 * name is taken from a guest description, and no introspection function is used
 * to read or write a native object. The selected native code and its dependency
 * closure must be independently pinned and confined by deployment.
 *
 * Prepared codecs validate every input before native deserialization. Native
 * serialization writes only into caller-owned fast_io storage: allocator
 * callbacks refuse growth instead of replacing that buffer. The complete
 * bounded validator then checks the resulting message before publication to a
 * guest queue. Generated ROS object storage and middleware-internal allocation
 * remain native ABI storage covered by process limits. These codecs grant no
 * endpoint, type authority, transport, filesystem or device access.
 */
#pragma once
#include "external_process_only.h"
#include "../communication.h"
#include <rosidl_typesupport_introspection_cpp/message_introspection.hpp>
#include <rosidl_typesupport_introspection_cpp/message_type_support_decl.hpp>
#include <rosidl_typesupport_introspection_cpp/identifier.hpp>
#include <rosidl_typesupport_introspection_cpp/field_types.hpp>
#include <rosidl_typesupport_cpp/message_type_support.hpp>
#include <rosidl_runtime_cpp/traits.hpp>
#include <rmw/rmw.h>
#include <rmw/error_handling.h>

namespace uwvm2::imported::ros::scene::communication::ros2::bounded_types
{
    namespace introspection=::rosidl_typesupport_introspection_cpp;
    namespace bounded=::uwvm2::imported::ros::scene::bounded_message;
    /** @brief Borrow bounded native metadata text from the pinned code closure. */
    inline std::string_view native_text(char const* text)noexcept
    {
        if(!text)return {};
        std::size_t size{};while(size<256&&text[size])++size;
        return size==256?std::string_view{}:std::string_view{text,size};
    }
    template<class T>bool keyed(T const& value)noexcept
    {
        if constexpr(requires{value.has_any_key_member_;})return value.has_any_key_member_;
        else if constexpr(requires{value.is_key_;})return value.is_key_;
        else return false;
    }
    /** @brief Compare generated package/category/name metadata without allocation. */
    inline bool identity(std::string_view expected,introspection::MessageMembers const& native)noexcept
    {
        auto space=native_text(native.message_namespace_),name=native_text(native.message_name_);
        if(space.empty()||name.empty()||expected.empty())return false;
        std::size_t cursor{};
        for(std::size_t i{};i<space.size();++i)
        {
            char next=space[i];
            if(next==':'){if(i+1>=space.size()||space[++i]!=':')return false;next='/';}
            if(cursor==expected.size()||expected[cursor++]!=next)return false;
        }
        return cursor<expected.size()&&expected[cursor]=='/'&&expected.substr(cursor+1)==name;
    }
    inline introspection::MessageMembers const* members(rosidl_message_type_support_t const* support)noexcept
    {
        if(!support||!support->data||native_text(support->typesupport_identifier)!=introspection::typesupport_identifier)return nullptr;
        return static_cast<introspection::MessageMembers const*>(support->data);
    }
    /** @brief Verify the deployment-pinned upstream ROS type hash in both supports.
     * @details This uses ROS's own hash version field. An absent hash, an
     * unsupported hash version or an all-zero value is never inferred from a
     * matching name/shape. The callback belongs to the pinned native closure.
     */
    inline bool hash_matches(rosidl_message_type_support_t const* support,rosidl_type_hash_t const& expected)noexcept
    {
        if(!support||!support->get_type_hash_func||expected.version!=1||
            std::all_of(std::begin(expected.value),std::end(expected.value),[](auto byte){return byte==0;}))return false;
        auto observed=support->get_type_hash_func(support);
        return observed&&observed->version==expected.version&&std::equal(std::begin(observed->value),std::end(observed->value),std::begin(expected.value));
    }
    struct node_view
    {bounded::kind kind{};std::uint64_t bound{},count{};wire::bytes fields;};
    /** @brief Bounded startup-only representation check; no native effects.
     * @details The source is an already validated immutable description. Node
     * views borrow it only during this call; temporary allocations use recoverable
     * fast_io. Explicit comparison work (1..65536) and depth (at most 64) prevent
     * repeated nested records or recursive native metadata from causing unbounded
     * work. Deployment may restrict an upstream unbounded sequence/string, or
     * select a smaller upstream bound, but cannot enlarge a fixed array or native
     * upper bound. Comparison failure never changes the caller's prepared codec.
     */
    class shape_check
    {
        ::uwvm2::utils::container::std_vector<node_view> nodes_;
        std::uint64_t remaining_{};
        bool spend()noexcept{if(!remaining_)return false;--remaining_;return true;}
        bool scalar(std::size_t index,introspection::MessageMember const& field,unsigned depth)noexcept
        {
            if(!spend()||index>=nodes_.size()||depth>64||keyed(field))return false;
            auto const& node=nodes_[index];using enum bounded::kind;
            switch(field.type_id_)
            {
                case introspection::ROS_TYPE_BOOLEAN:return node.kind==boolean;
                case introspection::ROS_TYPE_INT8:return node.kind==int8;
                case introspection::ROS_TYPE_CHAR:case introspection::ROS_TYPE_OCTET:case introspection::ROS_TYPE_UINT8:return node.kind==uint8;
                case introspection::ROS_TYPE_INT16:return node.kind==int16;
                case introspection::ROS_TYPE_UINT16:return node.kind==uint16;
                case introspection::ROS_TYPE_INT32:return node.kind==int32;
                case introspection::ROS_TYPE_UINT32:return node.kind==uint32;
                case introspection::ROS_TYPE_INT64:return node.kind==int64;
                case introspection::ROS_TYPE_UINT64:return node.kind==uint64;
                case introspection::ROS_TYPE_FLOAT:return node.kind==float32;
                case introspection::ROS_TYPE_DOUBLE:return node.kind==float64;
                case introspection::ROS_TYPE_STRING:case introspection::ROS_TYPE_WSTRING:
                    return node.kind==(field.type_id_==introspection::ROS_TYPE_STRING?string:wide_string)&&
                        (!field.string_upper_bound_||node.bound<=field.string_upper_bound_);
                case introspection::ROS_TYPE_MESSAGE:
                {auto nested=members(field.members_);return nested&&this->record(index,*nested,depth+1);}
                default:return false;
            }
        }
        bool field(std::size_t index,introspection::MessageMember const& native,unsigned depth)noexcept
        {
            if(!spend()||index>=nodes_.size()||depth>64||keyed(native))return false;
            if(!native.is_array_)return scalar(index,native,depth);
            auto const& node=nodes_[index];bool fixed=native.array_size_&&!native.is_upper_bound_;
            if(node.kind!=(fixed?bounded::kind::array:bounded::kind::sequence)||
               (fixed?node.bound!=native.array_size_:(native.is_upper_bound_&&node.bound>native.array_size_)))return false;
            wire::reader reader{node.fields};std::string_view name;std::uint64_t child{};
            return node.count==1&&reader.text(name)&&reader.u64(child)&&scalar(static_cast<std::size_t>(child),native,depth+1);
        }
        bool record(std::size_t index,introspection::MessageMembers const& native,unsigned depth)noexcept
        {
            if(!spend()||index>=nodes_.size()||depth>64||keyed(native)||!native.size_of_||
                (native.member_count_&&!native.members_))return false;
            auto const& node=nodes_[index];if(node.kind!=bounded::kind::record)return false;
            // ROS-generated empty structs have one explicit dummy octet. A
            // genuine user field with another name is not an empty record.
            if(!node.count&&native.member_count_==1)
            {auto const& dummy=native.members_[0];return native_text(dummy.name_)=="structure_needs_at_least_one_member"&&
                dummy.type_id_==introspection::ROS_TYPE_UINT8&&!dummy.is_array_&&!keyed(dummy);}
            if(node.count!=native.member_count_)return false;
            wire::reader reader{node.fields};
            for(std::uint64_t i{};i<node.count;++i)
            {
                std::string_view name;std::uint64_t child{};
                if(!reader.text(name)||!reader.u64(child)||name!=native_text(native.members_[i].name_)||
                    !field(static_cast<std::size_t>(child),native.members_[i],depth+1))return false;
            }
            return true;
        }
    public:
        error matches(bounded::schema const& schema,std::string_view expected,
                      rosidl_message_type_support_t const* support,std::uint64_t work)noexcept
        {
            if(!schema.ready()||schema.encoding()!=bounded::layout::cdr||!work||work>65536)return error::unsupported;
            auto native=members(support);if(!native||!identity(expected,*native))return error::unsupported;
            try
            {
                nodes_.clear();remaining_=work;auto description=schema.description();wire::reader reader{description};
                std::string_view marker,format;std::uint64_t bytes{},steps{},depth{},count{};
                if(!reader.text(marker)||!reader.text(format)||!reader.u64(bytes)||!reader.u64(steps)||!reader.u64(depth)||!reader.u64(count)||!count||count>4096)return error::invalid;
                nodes_.reserve(static_cast<std::size_t>(count));
                for(std::uint64_t i{};i<count;++i)
                {
                    node_view node;std::string_view name;std::uint64_t flags{};
                    if(!reader.text(name)||!reader.u64(node.bound)||!reader.u64(flags)||!reader.u64(node.count))return error::invalid;
                    auto found=std::find(bounded::kind_names.begin(),bounded::kind_names.end(),name);if(found==bounded::kind_names.end())return error::unsupported;
                    node.kind=static_cast<bounded::kind>(found-bounded::kind_names.begin());auto start=reader.position();
                    for(std::uint64_t j{};j<node.count;++j){std::uint64_t child{};if(!reader.text(name)||!reader.u64(child)||child>=i)return error::invalid;}
                    node.fields=description.subspan(start,reader.position()-start);nodes_.push_back(node);
                }
                return reader.done()&&record(nodes_.size()-1,*native,1)?error::ok:error::unsupported;
            }
            catch(std::bad_alloc const&){return error::no_memory;}
            catch(...){return error::invalid;}
        }
    };

    /** @brief Fixed caller-owned serialization buffer with explicitly denied growth.
     * @details RMW may request a larger buffer; every allocator callback refuses
     * replacement and records the violation. The caller retains ownership and
     * never passes this view to a native finalizer. This does not bound middleware
     * internals or native message objects; their isolated process remains required.
     */
    struct fixed_buffer
    {
        bool refused{};
        static void* allocate(std::size_t,void* state)noexcept{static_cast<fixed_buffer*>(state)->refused=true;return nullptr;}
        static void deallocate(void*,void* state)noexcept{static_cast<fixed_buffer*>(state)->refused=true;}
        static void* reallocate(void*,std::size_t,void* state)noexcept{static_cast<fixed_buffer*>(state)->refused=true;return nullptr;}
        static void* zero_allocate(std::size_t,std::size_t,void* state)noexcept{static_cast<fixed_buffer*>(state)->refused=true;return nullptr;}
        rcutils_allocator_t allocator()noexcept{return {allocate,deallocate,reallocate,zero_allocate,this};}
    };
    template<class Native>class codec
    {
        message_validator validator_;
        rosidl_message_type_support_t const* support_{};
    public:
        // The current RMW backend reports its four-byte rounded allocation
        // extent as the message length. Reserve its padding independently of
        // the canonical payload bound; never send padding to the guest queue.
        inline static constexpr std::size_t native_workspace_extra=3;
        /** @brief Strict guest-wire preflight; no trailing bytes or native calls. */
        bool canonical(wire::bytes input)const noexcept{return support_&&validator_(input);}
        /** @brief Validate a native CDR buffer and report its exact canonical extent.
         * @details Only an independently pinned native serializer or middleware
         * may supply this input. Its four-byte rounded extent can carry at most
         * three padding bytes. Guest input uses canonical() instead. No byte is
         * written and size is unchanged on failure; validation allocates nothing.
         */
        bool native_extent(wire::bytes input,std::size_t& size)const noexcept
        {
            if(!support_)return false;
            if(validator_(input)){size=input.size();return true;}
            if(input.size()%4)return false;
            for(std::size_t padding=1;padding<=native_workspace_extra&&padding<input.size();++padding)
                if(validator_(input.first(input.size()-padding))){size=input.size()-padding;return true;}
            return false;
        }
        /** @brief Prepare exact generated metadata before confinement; grant nothing.
         * @details The native worker closure fixes Native and both type-support
         * implementations. All failed preparations preserve the previous codec.
         * Only plain CDR bounded schemas are admitted; ROS1 conversion remains a
         * separately authorized bridge, never an implicit type-name adaptation.
         */
        static error prepare(type_plan const& plan,rosidl_type_hash_t const& expected,std::uint64_t work,codec& output)noexcept
        {
            if(!plan.validate.bounded()||plan.encoding!="cdr"||plan.type_identity!=rosidl_generator_traits::name<Native>()||
               !std::equal(plan.descriptor.begin(),plan.descriptor.end(),plan.validate.bounded()->description().begin(),plan.validate.bounded()->description().end()))return error::unsupported;
            try
            {
                auto metadata=introspection::get_message_type_support_handle<Native>();shape_check check;
                auto status=check.matches(*plan.validate.bounded(),plan.type_identity,metadata,work);if(status!=error::ok)return status;
                auto support=rosidl_typesupport_cpp::get_message_type_support_handle<Native>();
                if(!hash_matches(metadata,expected)||!hash_matches(support,expected)||native_text(rmw_get_serialization_format())!="cdr")return error::unsupported;
                codec candidate;candidate.validator_=plan.validate;candidate.support_=support;output=std::move(candidate);return error::ok;
            }
            catch(std::bad_alloc const&){return error::no_memory;}catch(...){return error::io;}
        }
        bool decode(wire::bytes input,Native& output)const noexcept
        {
            if(!canonical(input))return false;
            fixed_buffer ownership;rmw_serialized_message_t view{};
            view.buffer=const_cast<std::uint8_t*>(input.data());view.buffer_length=view.buffer_capacity=input.size();view.allocator=ownership.allocator();
            try
            {
                auto status=rmw_deserialize(&view,support_,&output);
                if(status!=RMW_RET_OK){rmw_reset_error();return false;}
                return !ownership.refused;
            }
            catch(...){return false;}
        }
        bool encode(Native const& input,std::span<std::uint8_t> output,std::size_t& size)const noexcept
        {
            if(!support_||output.empty())return false;
            fixed_buffer ownership;rmw_serialized_message_t view{};
            view.buffer=output.data();view.buffer_capacity=output.size();view.allocator=ownership.allocator();
            try
            {
                auto status=rmw_serialize(&input,support_,&view);
                if(status!=RMW_RET_OK){rmw_reset_error();return false;}
                if(ownership.refused||view.buffer!=output.data()||view.buffer_length>output.size())return false;
                return native_extent({output.data(),view.buffer_length},size);
            }
            catch(...){return false;}
        }
    };
}
