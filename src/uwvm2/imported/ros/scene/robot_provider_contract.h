/** @file
 * @brief Bounded canonical requests for device, control, media, bridge and component providers.
 * @details The ABI boundary is uwvm_ros_v2. These semantic names contain no
 * independent version suffix. Descriptors alone do not register VM imports or
 * grant authority. The router first checks the exact operation and carrier
 * extent, owns an immutable request, then calls this allocation-free decoder.
 * Every source/controller/output capability is independently resolved by the
 * selected provider; guest numbers, profiles and timestamps are only claims.
 *
 * All decode budgets default to zero. Array capacity is a deployment reservation,
 * not a fixed global registry size. Nested arrays share one element budget.
 * Returned views borrow the complete owned carrier only; no guest address is
 * retained. Unknown selectors, truncated records, trailing data, zero resource
 * identities and noncanonical repeated slot/reference order fail closed and
 * leave the destination unchanged. Security deadlines use host steady time.
 *
 * Providers must reserve complete replies, replay records, pins and native work
 * before effects. They revalidate current grouped authority at each effect and
 * delivery boundary, without holding that group while waiting for IPC. Stop
 * resources are reserved independently from normal queues and optional rates.
 * A successful decode establishes neither provider support nor physical safety.
 */
#pragma once

#define UWVM_ROS_DEVICE_FUNCTIONS(X) \
 X(device_open,device_open,"device.open") \
 X(device_submit,device_submit,"device.submit") \
 X(device_result,device_result,"device.result") \
 X(device_stop,device_stop,"device.stop") \
 X(device_describe,device_describe,"device.describe")
#define UWVM_ROS_CONTROL_FUNCTIONS(X) \
 X(control_open,control_open,"control.open") \
 X(control_sample,control_sample,"control.sample") \
 X(control_submit,control_submit,"control.submit") \
 X(control_result,control_result,"control.result") \
 X(control_stop,control_stop,"control.stop")
#define UWVM_ROS_MEDIA_FUNCTIONS(X) \
 X(media_open,media_open,"media.open") \
 X(media_submit,media_submit,"media.submit") \
 X(media_take,media_take,"media.take") \
 X(media_close,media_close,"media.close")
#define UWVM_ROS_BRIDGE_FUNCTIONS(X) \
 X(bridge_create,bridge_create,"bridge.create") \
 X(bridge_start,bridge_start,"bridge.start") \
 X(bridge_stop,bridge_stop,"bridge.stop") \
 X(bridge_describe,bridge_describe,"bridge.describe")
#define UWVM_ROS_COMPONENT_FUNCTIONS(X) \
 X(component_open,component_open,"component.open") \
 X(component_start,component_start,"component.start") \
 X(component_stop,component_stop,"component.stop") \
 X(component_describe,component_describe,"component.describe")

#ifndef UWVM_SCENE_ROBOT_CONTRACT_MACROS_ONLY
#include "wire.h"
#include <array>
namespace uwvm2::imported::ros::scene::robot_contract
{
    enum class operation : std::uint8_t
    {
#define UWVM_ROBOT_OPERATION(symbol,id,name) id,
        UWVM_ROS_DEVICE_FUNCTIONS(UWVM_ROBOT_OPERATION)
        UWVM_ROS_CONTROL_FUNCTIONS(UWVM_ROBOT_OPERATION)
        UWVM_ROS_MEDIA_FUNCTIONS(UWVM_ROBOT_OPERATION)
        UWVM_ROS_BRIDGE_FUNCTIONS(UWVM_ROBOT_OPERATION)
        UWVM_ROS_COMPONENT_FUNCTIONS(UWVM_ROBOT_OPERATION)
#undef UWVM_ROBOT_OPERATION
    };
    struct descriptor {operation id;std::string_view name;};
    inline constexpr std::array operations{
#define UWVM_ROBOT_DESCRIPTOR(symbol,id,name) descriptor{operation::id,name},
        UWVM_ROS_DEVICE_FUNCTIONS(UWVM_ROBOT_DESCRIPTOR)
        UWVM_ROS_CONTROL_FUNCTIONS(UWVM_ROBOT_DESCRIPTOR)
        UWVM_ROS_MEDIA_FUNCTIONS(UWVM_ROBOT_DESCRIPTOR)
        UWVM_ROS_BRIDGE_FUNCTIONS(UWVM_ROBOT_DESCRIPTOR)
        UWVM_ROS_COMPONENT_FUNCTIONS(UWVM_ROBOT_DESCRIPTOR)
#undef UWVM_ROBOT_DESCRIPTOR
    };
    static_assert(operations.size()==22);
    /** @brief Explicit per-carrier byte and aggregate element budgets; no implicit defaults. */
    struct limits {std::size_t request_bytes{},blob_bytes{},text_bytes{},elements{};};
    /** @brief Immutable deployment-selected semantic identity and exact 32-byte content hash. */
    struct profile {std::string_view identity;wire::bytes digest;};
    /** @brief Caller-bound capability and owned resource; neither integer is transferable authority. */
    struct reference {std::uint64_t capability{},resource{};friend bool operator==(reference const&,reference const&)=default;};
    /** @brief Next-or-exact-retry sequence inside one owned resource's recovery ledger. */
    struct request_key {std::uint64_t session{},sequence{};};
    /** @brief Bounded typed bytes; complete value semantics require the selected content validator. */
    struct message {std::string_view type,encoding;wire::bytes body;};
    /** @brief Validated borrowed array payload excluding its count; lifetime equals the owned carrier. */
    struct repeated {wire::bytes encoded;std::uint64_t count{};};
    /** @brief Guest source claim that must match independently authorized host provenance and pins. */
    struct observation
    {
        reference input;std::uint64_t source{},generation{},clock{},epoch{};
        std::int64_t nanoseconds{};std::string_view frame;bool stamped{},framed{};
    };
    /** @brief Exact active dependency revision; storage publication alone cannot activate it. */
    struct dependency {reference source;std::uint64_t revision{};profile selected;};
    /** @brief Explicit source and independent buffer authority retained by an opened control session. */
    struct control_source {std::uint64_t slot{};reference sensor;std::uint64_t buffer_capability{};};
    /** @brief Raw plane with checked strided extent; no accessible padding is assumed. */
    struct plane {std::uint64_t offset{},row_bytes{},stride{},rows{};};
    enum class frame_kind:std::uint8_t {image,audio,encoded_packet};
    /** @brief Profile-bound image/audio/packet shape; encoded expansion needs an isolated codec check. */
    struct frame
    {
        profile layout;frame_kind kind{};std::uint64_t sequence{},width{},height{},channels{},samples{};repeated planes;
    };
    /** @brief Closed borrowed request union; unused fields are zero initialized. */
    struct request
    {
        operation action{};request_key key;
        std::uint64_t resource{},request_id{},sequence{},epoch{},deadline{},output_capability{},configuration_capability{},configuration_generation{};
        profile selected,mode,schema;reference controller,source,buffer,egress;
        std::string_view configuration_identity;wire::bytes configuration_digest;
        message command;repeated observations,dependencies,bindings;frame media;
        bool has_egress{};
    };
    inline constexpr bool ordered(reference left,reference right) noexcept
    {return left.capability<right.capability||(left.capability==right.capability&&left.resource<right.resource);}
    /** @brief One bounded decoder; array visitors consume the same element budget. */
    class decoder
    {
        wire::bytes owned_;wire::reader input_;limits capacity_;std::size_t remaining_;
    public:
        decoder(wire::bytes input,limits capacity) noexcept:owned_(input),input_(input),capacity_(capacity),remaining_(capacity.elements){}
        wire::reader& input() noexcept{return input_;}
        bool number(std::uint64_t& value) noexcept{return input_.u64(value);}
        bool identity(std::uint64_t& value) noexcept{return number(value)&&value;}
        bool deadline(std::uint64_t& value) noexcept{return identity(value)&&value<=INT64_MAX;}
        bool text(std::string_view& value,std::size_t maximum=96) noexcept
        {return input_.text(value)&&!value.empty()&&value.size()<=maximum&&value.size()<=capacity_.text_bytes&&value.find('\0')==value.npos;}
        bool digest(wire::bytes& value) noexcept{return input_.blob(value)&&value.size()==32;}
        bool selected(profile& value) noexcept{return text(value.identity)&&digest(value.digest);}
        bool resource(reference& value) noexcept{return identity(value.capability)&&identity(value.resource);}
        bool key(request_key& value) noexcept{return identity(value.session)&&identity(value.sequence);}
        bool body(message& value) noexcept
        {return text(value.type)&&text(value.encoding,32)&&input_.blob(value.body)&&value.body.size()<=capacity_.blob_bytes;}
        bool source(observation& value) noexcept
        {
            if(!resource(value.input)||!identity(value.source)||!identity(value.generation)||!input_.boolean(value.stamped))return false;
            if(value.stamped&&(!identity(value.clock)||!identity(value.epoch)||!input_.i64(value.nanoseconds)))return false;
            return input_.boolean(value.framed)&&(!value.framed||text(value.frame));
        }
        bool version(dependency& value) noexcept{return resource(value.source)&&identity(value.revision)&&selected(value.selected);}
        bool control_input(control_source& value) noexcept
        {return identity(value.slot)&&resource(value.sensor)&&identity(value.buffer_capability);}
        bool image_plane(plane& value) noexcept
        {
            if(!number(value.offset)||!identity(value.row_bytes)||!number(value.stride)||!identity(value.rows)||value.stride<value.row_bytes)return false;
            if(value.offset>UINT64_MAX-value.row_bytes)return false;
            return value.rows-1<=(UINT64_MAX-value.offset-value.row_bytes)/value.stride;
        }
        /** @brief Borrow a fully validated canonical sequence without allocating.
         * @details Nonempty arrays consume a shared whole-request element budget.
         * Each visitor must consume at least one byte; otherwise decode refuses
         * the shape instead of permitting unbounded work on a zero-width record.
         */
        template<typename Visitor>bool array(repeated& value,Visitor&& visit) noexcept
        {
            std::uint64_t count{};if(!number(count)||count>remaining_)return false;
            remaining_-=static_cast<std::size_t>(count);auto start=input_.position();
            for(std::uint64_t n{};n<count;++n)
            {auto before=input_.position();if(!visit(*this,n)||input_.position()<=before)return false;}
            value={owned_.subspan(start,input_.position()-start),count};return true;
        }
        bool observations(repeated& output) noexcept
        {
            reference previous{};
            return array(output,[&](decoder& reader,std::uint64_t n) noexcept
            {observation value;if(!reader.source(value)||(n&&!ordered(previous,value.input)))return false;previous=value.input;return true;});
        }
        bool dependencies(repeated& output) noexcept
        {
            reference previous{};
            return array(output,[&](decoder& reader,std::uint64_t n) noexcept
            {dependency value;if(!reader.version(value)||(n&&!ordered(previous,value.source)))return false;previous=value.source;return true;});
        }
        bool bindings(repeated& output,bool bridge) noexcept
        {
            std::uint64_t previous{};
            return array(output,[&](decoder& reader,std::uint64_t) noexcept
            {std::uint64_t slot{};reference from,to;if(!reader.identity(slot)||slot<=previous||!reader.resource(from)||(bridge&&!reader.resource(to)))return false;previous=slot;return true;});
        }
        bool media(frame& output) noexcept
        {
            std::string_view kind;
            if(!selected(output.layout)||!text(kind,32)||!identity(output.sequence)||!number(output.width)||!number(output.height)||
                !number(output.channels)||!number(output.samples))return false;
            if(kind=="image")output.kind=frame_kind::image;
            else if(kind=="audio")output.kind=frame_kind::audio;
            else if(kind=="encoded_packet")output.kind=frame_kind::encoded_packet;
            else return false;
            if(output.kind==frame_kind::image&&(!output.width||!output.height||!output.channels||output.samples))return false;
            if(output.kind==frame_kind::audio&&(output.width||output.height||!output.channels||!output.samples))return false;
            std::uint64_t prior_end{};
            if(!array(output.planes,[&](decoder& reader,std::uint64_t n) noexcept
            {plane value;if(!reader.image_plane(value)||(n&&value.offset<prior_end))return false;
                prior_end=value.offset+(value.rows-1)*value.stride+value.row_bytes;return true;}))return false;
            return output.kind==frame_kind::encoded_packet?output.planes.count==0:output.planes.count!=0;
        }
        bool done() const noexcept{return input_.done();}
    };
    /** @brief Decode all 22 request shapes and publish only a complete result.
     * @details Expired exact replays remain decodable; the provider ledger decides
     * whether this is historical recovery before applying current-time admission.
     * Missing/zero byte budgets deny decoding. No allocation, syscall, native
     * resource creation, credential use or implicit configuration occurs here.
     */
    inline bool decode(operation action,wire::bytes data,limits capacity,request& output) noexcept
    {
        if(static_cast<std::size_t>(action)>=operations.size()||data.empty()||!capacity.request_bytes||data.size()>capacity.request_bytes)return false;
        decoder in{data,capacity};request value;value.action=action;bool keyed{};
        switch(action)
        {
        case operation::device_open:
            if(!in.selected(value.selected))return false;
            break;
        case operation::device_submit:
            if(!in.key(value.key)||!in.identity(value.resource)||!in.resource(value.controller)||!in.identity(value.epoch)||
               !in.selected(value.mode)||!in.selected(value.schema)||!in.body(value.command)||!in.observations(value.observations)||
               !in.dependencies(value.dependencies)||!in.deadline(value.deadline))return false;
            keyed=true;break;
        case operation::device_result:
            if(!in.identity(value.resource)||!in.identity(value.request_id))return false;
            break;
        case operation::device_stop:
        case operation::control_stop:
            if(!in.key(value.key)||!in.identity(value.resource)||!in.selected(value.selected))return false;
            keyed=true;break;
        case operation::device_describe:
        case operation::control_sample:
        case operation::media_take:
        case operation::bridge_describe:
        case operation::component_describe:
            if(!in.identity(value.resource))return false;
            break;
        case operation::control_open:
        {
            std::uint64_t previous{};
            if(!in.selected(value.selected)||!in.resource(value.source)||!in.array(value.bindings,[&](decoder& source,std::uint64_t)
                {control_source row;if(!source.control_input(row)||row.slot<=previous)return false;previous=row.slot;return true;}))return false;
            break;
        }
        case operation::control_submit:
            if(!in.key(value.key)||!in.identity(value.resource)||!in.identity(value.sequence)||!in.identity(value.epoch)||
                !in.selected(value.schema)||!in.body(value.command))return false;
            keyed=true;break;
        case operation::control_result:
            if(!in.identity(value.resource)||!in.identity(value.sequence))return false;
            break;
        case operation::media_open:
            if(!in.selected(value.selected)||!in.resource(value.source)||!in.identity(value.output_capability)||!in.input().boolean(value.has_egress)||
                (value.has_egress&&!in.resource(value.egress)))return false;
            break;
        case operation::media_submit:
            if(!in.key(value.key)||!in.identity(value.resource)||!in.resource(value.buffer)||!in.media(value.media)||!in.deadline(value.deadline))return false;
            keyed=true;break;
        case operation::media_close:
        case operation::bridge_start:
        case operation::component_start:
            if(!in.key(value.key)||!in.identity(value.resource)||!in.deadline(value.deadline))return false;
            keyed=true;break;
        case operation::bridge_create:
            if(!in.selected(value.selected)||!in.bindings(value.bindings,true)||!value.bindings.count)return false;
            break;
        case operation::bridge_stop:
        case operation::component_stop:
            if(!in.key(value.key)||!in.identity(value.resource)||!in.selected(value.selected)||!in.deadline(value.deadline))return false;
            keyed=true;break;
        case operation::component_open:
            if(!in.selected(value.selected)||!in.identity(value.configuration_capability)||!in.text(value.configuration_identity,255)||
                !in.identity(value.configuration_generation)||!in.digest(value.configuration_digest)||!in.bindings(value.bindings,false))return false;
            break;
        }
        if(!in.done()||(keyed&&value.key.session!=value.resource))return false;
        output=value;return true;
    }
    /** @brief Check raw-plane extents against the actual immutable buffer length.
     * @details The frame profile still validates color/endianness, width, height,
     * channel/sample relations and codec-specific expansion. This helper cannot
     * validate an encoded packet and deliberately refuses that kind. No padded
     * readable bytes are assumed beyond the real pinned allocation.
     */
    inline bool raw_extent(frame const& value,std::uint64_t buffer_bytes) noexcept
    {
        if((value.kind!=frame_kind::image&&value.kind!=frame_kind::audio)||!value.planes.count||!buffer_bytes)return false;
        decoder in{value.planes.encoded,{value.planes.encoded.size(),0,0,0}};
        std::uint64_t previous{};
        for(std::uint64_t n{};n<value.planes.count;++n)
        {plane current;if(!in.image_plane(current)||(n&&current.offset<previous))return false;
            previous=current.offset+(current.rows-1)*current.stride+current.row_bytes;if(previous>buffer_bytes)return false;}
        return in.done();
    }
}
#endif
