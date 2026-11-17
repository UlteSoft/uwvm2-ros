/** @file
 * @brief Exact buffer authority, finite private recordings and non-actuating replay.
 * @details All configuration is trusted and frozen before enrollment. Each
 * recording alias owns its complete MCAP store and a fixed buffer alias/profile
 * for every channel. Input handles are resolved to actual sealed allocations;
 * numeric IDs, timestamps and SHA values supplied by a guest confer no rights.
 * Every source independently requires buffer.read and data.export. Replay has
 * its own grant and retains an explicit source capability, checked on every use.
 * No files, sockets, native ROS publisher or clock advancement are provided.
 *
 * Writer, object and replay ownership includes the runtime caller incarnation.
 * Finite storage is reserved independently of optional rates. Mutation retries
 * retain one exact complete carrier and receipt, so lost copies do not repeat
 * accepted effects or debit work twice. All new memory uses recoverable fast_io
 * allocation; hot append/checkpoint/stop/seek paths allocate no memory. Shutdown
 * fences delivery but retains accounting. Destruction requires joined calls.
 */
#pragma once
#include "recording_contract.h"
#include "communication_workspace.h"
#include <uwvm2/utils/container/function_ref.h>

namespace uwvm2::imported::ros::scene
{
    class recording_dispatch
    {
    public:
        using operation=recording_contract::operation;
        using bytes=wire::bytes;
        using sink=::uwvm2::utils::container::function_ref<bool(bytes)>;
        using cancellation=::uwvm2::utils::container::function_ref<bool()>;
        using workspace_lease=communication_workspace::lease;
        struct limits{std::uint64_t recordings{},replays{},storage_bytes{},workspace_bytes{},workspace_calls{};};
        /** @brief Exact approved byte-buffer source for one immutable MCAP channel. */
        struct channel_source{std::uint16_t channel{};buffers::text<255> alias;buffers::profile schema;};
        struct replay_plan{buffers::profile profile;std::uint64_t handles{};};
        /** @brief Detached alias metadata; no cursor, digest, key or native address. */
        struct inspection
        {recording::usage store;std::uint64_t open_handles{},reserved_bytes{};bool replay{},retired{};};
    private:
        struct object_state{recording::object_info info;caller owner;};
        struct recording_binding
        {
            buffers::text<255> alias;buffers::profile profile;recording::owner origin;
            recording::plan plan;recording::store store;
            std::array<channel_source,recording::mcap::maximum_channels> sources{};std::size_t sources_count{};
            ::uwvm2::utils::container::std_unique_ptr<object_state[]> objects;
            caller writer_owner;handle writer{};bool revoked{},active_segment{};
            std::array<std::uint8_t,recording_contract::maximum_request> last_input{};std::size_t last_size{};
            operation last_operation{};recording::receipt last;
        };
        struct replay_state{caller owner;handle id{},source_capability{};};
        struct replay_binding
        {
            buffers::text<255> alias;recording_binding* source{};replay_plan plan;bool revoked{};
            ::uwvm2::utils::container::std_unique_ptr<replay_state[]> handles;
        };
        std::shared_ptr<authority> permissions;
        buffers::registry& buffers_;
        handle environment{};limits capacity;std::uint64_t reserved{};
        mutable std::mutex mutex;bool ready{},retired{};
        communication_workspace workspace;
        ::uwvm2::utils::container::std_vector<::uwvm2::utils::container::std_unique_ptr<recording_binding>> recordings;
        ::uwvm2::utils::container::std_vector<::uwvm2::utils::container::std_unique_ptr<replay_binding>> replays;
#if defined(__linux__)
        ::pid_t process{::getpid()};
#endif
        bool same_process() const noexcept
        {
#if defined(__linux__)
            return process==::getpid();
#else
            return true;
#endif
        }
        static bool same(caller a,caller b) noexcept{return !(a<b)&&!(b<a);}
        static bool valid_profile(buffers::profile const& profile) noexcept
        {
            auto const& text=profile.identity;
            return text.size&&text.size<=text.bytes.size()&&text.view().find('\0')==std::string_view::npos&&
                wire::utf8({reinterpret_cast<std::uint8_t const*>(text.bytes.data()),text.size});
        }
        static std::string_view name(operation op) noexcept
        {auto i=static_cast<std::size_t>(op);return i<recording_contract::operations.size()?recording_contract::operations[i].name:std::string_view{};}
        bool valid_limits() const noexcept
        {return capacity.recordings<=256&&capacity.replays<=256&&capacity.storage_bytes<=INT64_MAX&&capacity.workspace_bytes<=INT64_MAX&&capacity.workspace_calls<=INT64_MAX;}
        recording_binding* find_recording(std::string_view alias) const noexcept
        {for(auto const& p:recordings)if(p->alias.view()==alias)return p.get();return nullptr;}
        replay_binding* find_replay(std::string_view alias) const noexcept
        {for(auto const& p:replays)if(p->alias.view()==alias)return p.get();return nullptr;}
        error resolve(grant const& g,recording_binding*& output)
        {
            std::lock_guard lock{mutex};if(!ready||retired||g.kind!=purpose::recording||g.binding!="private-recording")return error::denied;
            auto* p=find_recording(g.alias);if(!p||p->revoked)return error::denied;output=p;return error::success;
        }
        error resolve(grant const& g,replay_binding*& output)
        {
            std::lock_guard lock{mutex};if(!ready||retired||g.kind!=purpose::replay||g.binding!="private-replay")return error::denied;
            auto* p=find_replay(g.alias);if(!p||p->revoked||p->source->revoked)return error::denied;output=p;return error::success;
        }
        bool live(recording_binding const& b) const noexcept
        {std::lock_guard lock{mutex};return ready&&!retired&&!b.revoked;}
        bool live(replay_binding const& b) const noexcept
        {std::lock_guard lock{mutex};return ready&&!retired&&!b.revoked&&!b.source->revoked;}
        static bool matches(grant const& g,recording_binding const& b) noexcept
        {return g.alias==b.alias.view()&&g.kind==purpose::recording&&g.binding=="private-recording";}
        error owner(recording_binding& b,caller who,handle id)
        {std::lock_guard lock{mutex};return b.writer==id&&id&&same(b.writer_owner,who)?error::success:error::bad_handle;}
        error owner(replay_binding& b,caller who,handle id,replay_state& output)
        {
            std::lock_guard lock{mutex};for(std::size_t i{};i<b.plan.handles;++i)
                if(b.handles[i].id==id&&id&&same(b.handles[i].owner,who)){output=b.handles[i];return error::success;}
            return error::bad_handle;
        }
        /** @brief Revalidate all exact scopes after a potentially reentrant callback. */
        error current(caller who,std::span<admission_claim const> claims,group_lease& lease)
        {return permissions->admit_group(who,claims,lease,false);}
        template<typename Binding> bool deliver_checked(caller who,std::span<admission_claim const> claims,bytes data,sink emit,cancellation cancelled,
                             Binding const& b,error& status)
        {
            if(cancelled&&cancelled()){status=error::cancelled;return false;}
            group_lease check;status=current(who,claims,check);if(status!=error::success||!live(b)){if(status==error::success)status=error::cancelled;return false;}
            bool copied{};try{copied=emit(data);}catch(...){status=error::io;return false;}
            if(!copied){status=error::io;return false;}
            if(cancelled&&cancelled()){status=error::cancelled;return false;}
            group_lease after;status=current(who,claims,after);
            if(status==error::success&&!live(b))status=error::cancelled;
            return status==error::success;
        }
        /** @brief Visit all actual pins in nested scopes; no borrowed byte span escapes a visit. */
        template<typename Effect> static error visit(std::span<buffers::registry::sealed_lease> pins,
                std::span<recording::item> items,recording_contract::request const& request,recording_binding const& binding,
                std::size_t index,Effect& effect)
        {
            if(index==pins.size())return effect();
            error result=error::success;
            auto status=pins[index].visit([&](bytes data,buffers::layout const& layout,buffers::digest const& sha)
            {
                auto const& input=request.items[index];channel_source const* source{};
                for(std::size_t i{};i<binding.sources_count;++i)if(binding.sources[i].channel==input.channel)source=&binding.sources[i];
                if(!source||layout.schema!=source->schema||layout.element!=buffers::element_type::uint8||layout.rank!=1||
                   layout.strides[0]!=1||layout.shape[0]!=data.size()||layout.bytes!=data.size()||layout.color!=buffers::color_space::absent||
                   data.size()!=input.bytes||sha!=input.sha256){result=error::conflict;return false;}
                items[index]={{static_cast<std::uint16_t>(input.channel),static_cast<std::uint32_t>(input.sequence),input.log_time,input.publish_time,data},
                    input.capability,input.buffer,1};
                result=visit(pins,items,request,binding,index+1,effect);return result==error::success;
            });
            return result!=error::success?result:status;
        }
    public:
        /** @brief Pure complete logical store/ownership reservation for trusted preflight.
         * @details Counts array prefixes and fixed binding metadata. Container
         * allocator bookkeeping and the bounded dispatch index remain within
         * the independent environment/process budget, not guest-selected sizes.
         */
        static bool recording_storage(recording::plan plan,std::string_view ros_profile,
                                      std::span<recording::mcap::channel const> channels,std::uint64_t& output)noexcept
        {
            std::uint64_t bytes{},definitions{};
            if(!recording::store::required_storage(plan,ros_profile,channels,bytes,definitions))return false;
            auto metadata=sizeof(recording_binding)+plan.segments*sizeof(object_state)+::uwvm2::utils::allocator::fast_io_strict::array_prefix<object_state>;
            if(bytes>INT64_MAX-metadata)return false;
            output=bytes+metadata;return true;
        }
        /** @brief Pure finite replay-handle ownership reservation, without pinning any object. */
        static bool replay_storage(replay_plan const& plan,std::uint64_t& output)noexcept
        {
            if(!valid_profile(plan.profile)||!plan.handles||plan.handles>256)return false;
            output=sizeof(replay_binding)+plan.handles*sizeof(replay_state)+::uwvm2::utils::allocator::fast_io_strict::array_prefix<replay_state>;
            return true;
        }
        recording_dispatch(std::shared_ptr<authority> authority,buffers::registry& buffers,handle env,limits value):
            permissions{std::move(authority)},buffers_{buffers},environment{env},capacity{value}{}
        ~recording_dispatch()
        {
            if(!same_process())
            {std::construct_at(std::addressof(recordings));std::construct_at(std::addressof(replays));std::construct_at(std::addressof(permissions));return;}
            retire();
        }
        /** @brief Reserve a private store and all ownership/retry metadata before publication.
         * @details Definitions are copied by the store. Each channel needs an
         * exact nonempty source alias/profile, and all schema validators are
         * trusted deployment functions. No runtime symbol name is accepted.
         */
        error bind_recording(std::string_view alias,buffers::profile profile,recording::plan plan,std::string_view ros_profile,
                             std::span<recording::mcap::channel const> channels,std::span<channel_source const> sources)
        {
            if(!same_process())return error::denied;
            std::lock_guard lock{mutex};if(ready||retired||!permissions||!environment)return error::denied;
            if(!valid_limits()||!identifier(alias)||!valid_profile(profile)||sources.size()!=channels.size())return error::invalid;
            if(find_recording(alias)||find_replay(alias))return error::conflict;
            std::uint64_t bytes{};if(!recording_storage(plan,ros_profile,channels,bytes))return error::invalid;
            for(std::size_t i{};i<sources.size();++i)
                if(sources[i].channel!=channels[i].id||sources[i].alias.size>sources[i].alias.bytes.size()||
                   !identifier(sources[i].alias.view())||!valid_profile(sources[i].schema))return error::invalid;
            if(recordings.size()>=capacity.recordings||reserved>capacity.storage_bytes||bytes>capacity.storage_bytes-reserved)return error::too_large;
            auto id=resources::allocate_identity();if(!id)return error::too_large;
            try
            {
                namespace a=::uwvm2::utils::allocator::fast_io_strict;
                ::uwvm2::utils::container::std_unique_ptr<recording_binding> value{a::try_object<recording_binding>([](auto* p){return std::construct_at(p);})};
                if(!value)return error::no_memory;
                value->objects.reset(a::try_array<object_state>(static_cast<std::size_t>(plan.segments)));if(!value->objects)return error::no_memory;
                value->alias.assign(alias);value->profile=profile;value->origin={environment,id,1};value->plan=plan;
                std::copy(sources.begin(),sources.end(),value->sources.begin());value->sources_count=sources.size();
                auto result=value->store.configure(value->origin,plan,ros_profile,channels);if(result!=error::success)return result;
                recordings.push_back(std::move(value));reserved+=bytes;return error::success;
            }
            catch(std::bad_alloc const&){return error::no_memory;}
        }
        /** @brief Bind replay to one existing recording alias; never search arbitrary objects. */
        error bind_replay(std::string_view alias,std::string_view source,replay_plan plan)
        {
            if(!same_process())return error::denied;
            std::lock_guard lock{mutex};if(ready||retired||!permissions||!environment)return error::denied;
            if(!valid_limits()||!identifier(alias)||!valid_profile(plan.profile)||!plan.handles||plan.handles>256)return error::invalid;
            if(find_recording(alias)||find_replay(alias))return error::conflict;
            auto* origin=find_recording(source);if(!origin)return error::bad_handle;
            if(plan.handles>origin->plan.replays)return error::too_large;
            std::uint64_t bytes{};if(!replay_storage(plan,bytes))return error::invalid;
            if(replays.size()>=capacity.replays||reserved>capacity.storage_bytes||bytes>capacity.storage_bytes-reserved)return error::too_large;
            try
            {
                namespace a=::uwvm2::utils::allocator::fast_io_strict;
                ::uwvm2::utils::container::std_unique_ptr<replay_binding> value{a::try_object<replay_binding>([](auto* p){return std::construct_at(p);})};
                if(!value)return error::no_memory;
                value->handles.reset(a::try_array<replay_state>(static_cast<std::size_t>(plan.handles)));if(!value->handles)return error::no_memory;
                value->alias.assign(alias);value->source=origin;value->plan=plan;replays.push_back(std::move(value));reserved+=bytes;return error::success;
            }
            catch(std::bad_alloc const&){return error::no_memory;}
        }
        /** @brief Freeze bindings and independently reserve transient carrier capacity. */
        error prepare()
        {
            if(!same_process())return error::denied;
            std::lock_guard lock{mutex};if(ready||retired||!permissions||!environment)return error::denied;
            if(!valid_limits())return error::invalid;
            auto result=workspace.configure(capacity.workspace_bytes,capacity.workspace_calls);if(result==error::success)ready=true;return result;
        }
        /** @brief Permission/extent preflight without charging a possible exact receipt retry. */
        error preflight(caller who,operation action,handle cap,std::size_t size)
        {
            if(!same_process()||!permissions)return error::denied;
            auto action_name=name(action);if(action_name.empty())return error::unsupported;
            if(size>recording_contract::maximum_request)return error::too_large;
            lease admitted;auto result=permissions->admit(who,cap,action_name,0,admitted,false);if(result!=error::success)return result;
            if(action<=operation::recording_stop){recording_binding* b{};return resolve(admitted.specification(),b);}
            replay_binding* b{};return resolve(admitted.specification(),b);
        }
        error reserve_workspace(caller who,operation action,handle cap,std::size_t size,workspace_lease& output)
        {auto result=preflight(who,action,cap,size);return result==error::success?workspace.reserve(size,output):result;}
        /** @brief Invoke one canonical carrier under complete authority and exact caller ownership. */
        error invoke(caller who,operation action,handle cap,bytes input,std::size_t output_capacity,std::size_t& required,sink emit,cancellation cancelled={})
        {
            required=0;auto result=preflight(who,action,cap,input.size());if(result!=error::success)return result;
            if(!emit||output_capacity>buffers::maximum_transfer)return error::invalid;
            recording_contract::request request;if(!recording_contract::decode(action,input,request))return error::invalid;
            if(cancelled&&cancelled())return error::cancelled;
            std::array<admission_claim,maximum_admission_claims> storage{};std::size_t count=1;
            storage[0]={cap,name(action),0,false,false};
            if(action==operation::recording_append)
                for(std::size_t i{};i<request.count;++i)
                {auto const& item=request.items[i];storage[count++]={item.capability,"buffer.read",item.bytes,false,false};storage[count++]={item.capability,"data.export",item.bytes,true,false};}
            else if(action==operation::replay_open)
            {storage[count++]={request.source_capability,"resource.observe",request.count*64,true,false};storage[count++]={request.source_capability,"data.export",0,true,false};}
            auto claims=std::span{storage}.first(count);group_lease admitted;result=current(who,claims,admitted);if(result!=error::success)return result;
            if(action<=operation::recording_stop)
            {
                recording_binding* b{};result=resolve(*admitted.specification(0),b);if(result!=error::success)return result;
                required=action==operation::recording_open?8:recording_contract::receipt_bytes;if(output_capacity<required)return error::too_large;
                storage[0].bytes=input.size()+required;
                if(action==operation::recording_open)
                {
                    if(request.profile!=b->profile)return error::conflict;
                    {std::lock_guard lock{mutex};std::uint64_t held=b->writer?1:0;
                     for(std::size_t i{};i<b->plan.segments;++i)if(b->objects[i].info.id)++held;
                     if(held>=admitted.specification(0)->max_resources)return error::again;}
                    group_lease charged;result=permissions->admit_group(who,claims,charged);if(result!=error::success)return result;
                    handle id{};
                    {std::lock_guard lock{mutex};if(retired||b->revoked)return error::cancelled;
                     result=b->store.open(b->origin,id);if(result!=error::success)return result;
                     b->writer=id;b->writer_owner=who;b->active_segment=true;b->last={};b->last_size=0;}
                    std::array<std::uint8_t,8> output;wire::writer writer{output};writer.u64(id);
                    if(!deliver_checked(who,claims,output,emit,cancelled,*b,result))
                    {std::lock_guard lock{mutex};if(b->writer==id){b->store.close_writer(b->origin,id);b->writer=0;}return result;}
                    return owner(*b,who,id);
                }
                result=owner(*b,who,request.resource);if(result!=error::success)return result;
                bool duplicate{};recording::receipt receipt;
                {
                    std::lock_guard lock{mutex};
                    if(request.key==b->last.key)
                    {
                        if(action!=b->last_operation||input.size()!=b->last_size||!std::equal(input.begin(),input.end(),b->last_input.begin()))return error::conflict;
                        duplicate=true;receipt=b->last;
                    }
                    else if(b->last.key==UINT64_MAX||request.key!=b->last.key+1)return error::conflict;
                }
                if(!duplicate)
                {
                    if(action==operation::recording_checkpoint||(action==operation::recording_stop&&request.drain))
                    {std::lock_guard lock{mutex};if(b->active_segment){std::uint64_t held=b->writer?1:0;
                     for(std::size_t i{};i<b->plan.segments;++i)if(b->objects[i].info.id)++held;
                     if(held>=admitted.specification(0)->max_resources)return error::again;}}
                    std::array<buffers::registry::sealed_lease,recording_contract::maximum_items> pins;
                    std::array<recording::item,recording_contract::maximum_items> items{};
                    for(std::size_t i{};i<(action==operation::recording_append?request.count:0);++i)
                    {
                        channel_source const* source{};for(std::size_t j{};j<b->sources_count;++j)if(b->sources[j].channel==request.items[i].channel)source=&b->sources[j];
                        auto const& g=*admitted.specification(1+2*i);
                        if(!source||g.kind!=purpose::buffer||g.binding!="sealed-buffer"||g.alias!=source->alias.view())return error::denied;
                        result=buffers_.pin(g.alias,request.items[i].buffer,pins[i]);if(result!=error::success)return result;
                    }
                    if(cancelled&&cancelled())return error::cancelled;
                    auto effect=[&]() -> error
                    {
                        group_lease charged;auto status=permissions->admit_group(who,claims,charged);if(status!=error::success)return status;
                        if(!live(*b))return error::cancelled;
                        if(action==operation::recording_append)status=b->store.append(b->origin,request.resource,request.key,{items.data(),static_cast<std::size_t>(request.count)},receipt);
                        else if(action==operation::recording_checkpoint)status=b->store.checkpoint(b->origin,request.resource,request.key,receipt);
                        else status=b->store.stop(b->origin,request.resource,request.key,request.drain,receipt);
                        if(status!=error::success)return status;
                        std::lock_guard lock{mutex};b->active_segment=action==operation::recording_append;
                        b->last=receipt;b->last_operation=action;b->last_size=input.size();std::copy(input.begin(),input.end(),b->last_input.begin());
                        if(receipt.object.id)
                        {
                            std::size_t slot{};while(slot<b->plan.segments&&b->objects[slot].info.id)++slot;
                            if(slot==b->plan.segments){b->revoked=true;b->store.retire(b->origin);return error::io;}
                            b->objects[slot]={receipt.object,who};
                        }
                        return error::success;
                    };
                    result=action==operation::recording_append?visit(std::span{pins}.first(request.count),std::span{items}.first(request.count),request,*b,0,effect):effect();
                    if(result!=error::success)return result;
                }
                std::array<std::uint8_t,recording_contract::receipt_bytes> output;wire::writer writer{output};if(!recording_contract::encode(writer,receipt))return error::io;
                deliver_checked(who,claims,output,emit,cancelled,*b,result);return result;
            }
            replay_binding* b{};result=resolve(*admitted.specification(0),b);if(result!=error::success)return result;
            if(action==operation::replay_open)
            {
                required=8;if(output_capacity<required)return error::too_large;
                if(request.profile!=b->plan.profile||!matches(*admitted.specification(1),*b->source))return error::denied;
                std::size_t slot{},used{};
                {std::lock_guard lock{mutex};for(;slot<b->plan.handles&&b->handles[slot].id;++slot){}
                 for(std::size_t i{};i<b->plan.handles;++i)if(b->handles[i].id)++used;
                 if(slot==b->plan.handles||used>=admitted.specification(0)->max_resources)return error::again;
                 for(std::size_t i{};i<request.count;++i)
                 {bool found{};for(std::size_t j{};j<b->source->plan.segments;++j)if(b->source->objects[j].info==request.objects[i]&&same(b->source->objects[j].owner,who))found=true;
                  if(!found)return error::bad_handle;}}
                storage[0].bytes=input.size()+required;group_lease charged;result=permissions->admit_group(who,claims,charged);if(result!=error::success)return result;
                handle id{};
                {std::lock_guard lock{mutex};if(retired||b->revoked||b->source->revoked)return error::cancelled;
                 result=b->source->store.replay_open(b->source->origin,{request.objects.data(),static_cast<std::size_t>(request.count)},id);if(result!=error::success)return result;
                 b->handles[slot]={who,id,request.source_capability};}
                std::array<std::uint8_t,8> output;wire::writer writer{output};writer.u64(id);
                if(!deliver_checked(who,claims,output,emit,cancelled,*b,result)||!live(*b))
                {std::lock_guard lock{mutex};b->source->store.close_replay(b->source->origin,id);if(b->handles[slot].id==id)b->handles[slot]={};return result==error::success?error::cancelled:result;}
                replay_state state;return owner(*b,who,id,state);
            }
            replay_state state;result=owner(*b,who,request.resource,state);if(result!=error::success)return result;
            storage[1]={state.source_capability,"resource.observe",0,true,false};storage[2]={state.source_capability,"data.export",0,true,action==operation::replay_seek};
            claims=std::span{storage}.first(3);group_lease origin;result=current(who,claims,origin);if(result!=error::success)return result;
            if(!matches(*origin.specification(1),*b->source))return error::denied;
            if(action==operation::replay_seek)
            {
                required=8;if(output_capacity<required)return error::too_large;
                storage[0].bytes=input.size()+required;group_lease charged;result=permissions->admit_group(who,claims,charged);if(result!=error::success)return result;
                if(!live(*b))return error::cancelled;
                result=b->source->store.seek(b->source->origin,state.id,request.position);if(result!=error::success)return result;
                std::array<std::uint8_t,8> output;wire::writer writer{output};writer.u64(request.position);
                deliver_checked(who,claims,output,emit,cancelled,*b,result);return result;
            }
            /** The store reserves delivery but advances only after this whole callback succeeds. */
            error delivery=error::success;
            result=b->source->store.read(b->source->origin,state.id,request.count,[&](std::uint64_t ordinal,std::span<recording::mcap::message const> records)
            {
                auto measured=wire::writer::measuring(static_cast<std::size_t>(buffers::maximum_transfer));
                if(!recording_contract::encode(measured,ordinal,records)){delivery=error::too_large;return false;}
                required=measured.size();if(output_capacity<required){delivery=error::too_large;return false;}
                storage[0].bytes=input.size()+required;storage[1].bytes=records.size()*32;storage[2].bytes=required;
                if(cancelled&&cancelled()){delivery=error::cancelled;return false;}
                group_lease charged;delivery=permissions->admit_group(who,claims,charged);if(delivery!=error::success)return false;
                if(!live(*b)){delivery=error::cancelled;return false;}
                workspace_lease reservation;delivery=workspace.reserve(required,reservation);if(delivery!=error::success)return false;
                ::uwvm2::utils::container::std_unique_ptr<std::uint8_t[]> output{::uwvm2::utils::allocator::fast_io_strict::try_array<std::uint8_t>(required,false)};
                if(!output){delivery=error::no_memory;return false;}
                wire::writer writer{{output.get(),required}};if(!recording_contract::encode(writer,ordinal,records)){delivery=error::io;return false;}
                if(!deliver_checked(who,claims,writer.written(),emit,cancelled,*b,delivery))return false;
                if(!live(*b)){delivery=error::cancelled;return false;}return true;
            });
            return delivery!=error::success?delivery:result;
        }
        /** @brief Explicit resource.close for a writer, replay or unpinned checkpoint.
         * @details Closing a capability alone never destroys retained resources.
         * A writer close abandons only its current unsealed segment. Checkpoint
         * erase needs the exact caller-owned object and refuses outstanding pins.
         */
        error close(caller who,handle cap,handle resource)
        {
            if(!same_process()||!permissions)return error::denied;
            lease admitted;auto result=permissions->admit(who,cap,"resource.close",8,admitted);if(result!=error::success)return result;
            if(admitted.specification().kind==purpose::recording)
            {
                recording_binding* b{};result=resolve(admitted.specification(),b);if(result!=error::success)return result;
                std::lock_guard lock{mutex};
                if(resource&&b->writer==resource&&same(b->writer_owner,who))
                {result=b->store.close_writer(b->origin,resource);if(result==error::success){b->writer=0;b->last={};b->last_size=0;}return result;}
                for(std::size_t i{};i<b->plan.segments;++i)if(resource&&b->objects[i].info.id==resource&&same(b->objects[i].owner,who))
                {result=b->store.erase(b->origin,b->objects[i].info);if(result==error::success)b->objects[i]={};return result;}
                return error::bad_handle;
            }
            replay_binding* b{};result=resolve(admitted.specification(),b);if(result!=error::success)return result;
            std::lock_guard lock{mutex};for(std::size_t i{};i<b->plan.handles;++i)if(resource&&b->handles[i].id==resource&&same(b->handles[i].owner,who))
            {result=b->source->store.close_replay(b->source->origin,resource);if(result==error::success)b->handles[i]={};return result;}
            return error::bad_handle;
        }
        /** @brief Fence exact aliases and their replay dependants; no payload inspection. */
        void revoke(std::string_view alias) noexcept
        {
            if(!same_process())return;
            std::lock_guard lock{mutex};if(auto* b=find_recording(alias)){b->revoked=true;b->store.retire(b->origin);}
            if(auto* b=find_replay(alias))
            {b->revoked=true;for(std::size_t i{};i<b->plan.handles;++i)if(b->handles[i].id){b->source->store.close_replay(b->source->origin,b->handles[i].id);b->handles[i]={};}}
        }
        void retire() noexcept
        {
            if(!same_process())return;
            std::lock_guard lock{mutex};retired=true;workspace.retire();
            for(auto& b:recordings){b->revoked=true;b->store.retire(b->origin);}
            for(auto& b:replays)b->revoked=true;
        }
        /** @brief Cached nonconsuming diagnostic projection; no read, clock or grant side effect. */
        error inspect(std::string_view alias,recording::usage& output) const noexcept
        {
            if(!same_process())return error::denied;
            std::unique_lock lock{mutex,std::try_to_lock};if(!lock.owns_lock())return error::again;
            auto* b=find_recording(alias);if(!b)return error::bad_handle;return b->store.inspect(b->origin,output);
        }
        /** @brief Try-copy cached recording/replay ownership without consuming bytes or advancing a cursor.
         * @details Every lock is nonblocking. A contended inner store leaves
         * the caller's output unchanged, so debugger pages cannot mix partial
         * observations. Retired aliases retain their charged ownership counts.
         */
        error inspect(std::string_view alias,inspection& output) const noexcept
        {
            if(!same_process())return error::denied;
            std::unique_lock lock{mutex,std::try_to_lock};if(!lock.owns_lock())return error::again;
            inspection value;
            if(auto* b=find_recording(alias))
            {
                auto status=b->store.inspect(b->origin,value.store);if(status!=error::success)return status;
                value.retired=retired||b->revoked||value.store.retired;value.open_handles=b->writer?1:0;
                for(std::size_t i{};i<b->plan.segments;++i)if(b->objects[i].info.id)++value.open_handles;
                value.reserved_bytes=value.store.reserved_bytes;
            }
            else if(auto* b=find_replay(alias))
            {
                value.replay=true;value.retired=retired||b->revoked||b->source->revoked;
                for(std::size_t i{};i<b->plan.handles;++i)if(b->handles[i].id)++value.open_handles;
                if(!replay_storage(b->plan,value.reserved_bytes))return error::invalid;
            }
            else return error::bad_handle;
            output=value;return error::success;
        }
    };
}
