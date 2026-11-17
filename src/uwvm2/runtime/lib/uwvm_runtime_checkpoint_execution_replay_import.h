// Private selected checkpoint import bridge. Never used by ordinary JIT code.
class checkpoint_replay_import
{
    using session_type=runtime_checkpoint_replay_session;
    using object=::uwvm2::uwvm::debugger::checkpoint::object;
    using value=::uwvm2::uwvm::debugger::checkpoint::value;
    using error=::uwvm2::uwvm::debugger::checkpoint::error;
    using view=::uwvm2::uwvm::debugger::checkpoint::replay_memory_view;
    using image=::uwvm2::uwvm::debugger::checkpoint::replay_memory_image;
    ::std::shared_ptr<session_type> session_{};
    object event_{};
    cached_import_target const* target_{};
    ::std::uintptr_t results_{};
    ::std::size_t result_bytes_{};
    struct memory_owner
    {
        view actual{};
        ::uwvm2::extension::memtag::shadow_storage const* tags{};
        ::uwvm2::utils::container::std_vector<::std::byte> image{};
    };
    ::uwvm2::utils::container::std_vector<memory_owner> owners_{};
    ::uwvm2::utils::container::std_vector<view> memories_{};
    ::uwvm2::utils::container::std_vector<::uwvm2::utils::container::std_vector<::std::byte>> before_{};
    bool completed_{}, replayed_{}, phased_{}, accessing_{};
    ::std::uint64_t participant_{},logical_{},begin_sequence_{};
    object access_event_{};
    ::uwvm2::runtime::checkpoint::host_memory_phase::consumer wasi_consumer_{};
    bool wasi_phased_{};
    bool retained_host_{};
    ::uwvm2::runtime::memtag::report::observer report_observer_{};
    inline static thread_local checkpoint_replay_import* active_{};
    [[nodiscard]] static ::std::size_t width(auto kind) noexcept
    {
        switch(static_cast<unsigned>(kind))
        {case 0x7fu:case 0x7du:return 4u;case 0x7eu:case 0x7cu:return 8u;case 0x7bu:return 16u;default:return 0u;}
    }
    [[nodiscard]] static bool read_values(auto const& types,::std::uintptr_t address,::std::size_t bytes,::uwvm2::utils::container::std_vector<value>& values)
    {
        if(bytes && !address)return false;
        ::std::size_t offset{};
        for(::std::size_t type_index{};type_index<types.size;++type_index)
        {
            auto type=types.at(type_index);
            auto n=width(type);if(!n || offset>bytes || n>bytes-offset)return false;
            value item{};item.type.kind=static_cast<::uwvm2::uwvm::debugger::checkpoint::value_kind>(0x80u-static_cast<unsigned>(type));
            // Native raw ABI fields are dense, potentially unaligned. Copy into
            // integer carriers; never serialize a native reference/pointer.
            auto const source=reinterpret_cast<::std::byte const*>(address)+offset;
            if(n==4u){::std::uint32_t bits{};::std::memcpy(&bits,source,4u);item.low_bits=bits;}
            else {::std::memcpy(&item.low_bits,source,8u);if(n==16u)::std::memcpy(&item.high_bits,source+8u,8u);}
            values.push_back(item);offset+=n;
        }
        return offset==bytes;
    }
    [[nodiscard]] bool collect_memories()
    {
        memories_.clear();owners_.clear();
        auto const& graph=session_->origin;
        for(::std::size_t owner{};owner<session_->actual_modules.size();++owner)
        {
            auto actual=session_->actual_modules[owner];
            if(actual>=g_runtime.modules.size() || !checkpoint_effect_actual_full_owner(actual))return false;
            auto const& module=*g_runtime.modules.index_unchecked(actual).runtime_module;
            auto const& instance=graph.objects[graph.root_instances[owner]-1u];
            auto const begin=1u+instance.words[0]+instance.words[1]+module.imported_memory_vec_storage.size();
            if(instance.words[2]!=module.imported_memory_vec_storage.size()+module.local_defined_memory_vec_storage.size())return false;
            for(::std::size_t local{};local<module.local_defined_memory_vec_storage.size();++local)
            {
                auto const id=instance.links[begin+local];
                auto const& actual_memory=module.local_defined_memory_vec_storage.index_unchecked(local);
                auto const& memory=actual_memory.memory;
                auto extent=[]<typename M>(M const& current)->::std::span<::std::byte>
                {
                    ::std::size_t length{};
                    if constexpr(M::can_mmap)
                    {if(!current.memory_length_p)return {};length=current.memory_length_p->load(::std::memory_order_acquire);}
                    else length=current.memory_length;
                    return {reinterpret_cast<::std::byte*>(current.memory_begin),length};
                }(memory);
                if(!extent.empty() && !extent.data())return false;
                owners_.push_back({{id,extent},::std::addressof(memory.memtag_shadow),{}});
            }
        }
        ::std::sort(owners_.begin(),owners_.end(),[](auto const& a,auto const& b){return a.actual.memory<b.actual.memory;});
        // The replay transaction reads detached bytes, including on a hardware
        // MTE worker. Never clear TCO/change the caller's checking policy.
        for(auto& owner:owners_)
        {
            owner.image.resize(owner.actual.bytes.size());
            if(!copy_memory(owner,false))return false;
            memories_.push_back({owner.actual.memory,owner.image});
        }
        return ::uwvm2::uwvm::debugger::checkpoint::replay_memory_detail::ordered(::std::span<view const>{memories_}) &&
            ::uwvm2::uwvm::debugger::checkpoint::replay_memory_detail::disjoint(memories_);
    }
    [[nodiscard]] static bool copy_memory(memory_owner& owner,bool write) noexcept
    {
        namespace mt=::uwvm2::extension::memtag;
        auto memory=owner.actual.bytes;
        if(owner.image.size()!=memory.size() || !owner.tags)return false;
        auto transfer=[&](::std::byte* physical,::std::size_t offset,::std::size_t length) noexcept
        {
            if(write)::std::memcpy(physical,owner.image.data()+offset,length);
            else ::std::memcpy(owner.image.data()+offset,physical,length);
        };
        if(memory.empty())return true;
        if(!owner.tags->hardware_active()){transfer(memory.data(),0u,memory.size());return true;}
        if(!owner.tags->enabled() || owner.tags->hardware_base()!=memory.data() ||
           memory.size()>owner.tags->prepared_memory_bytes())return false;
        auto copy=[&]<typename Address,bool Shared>() noexcept
        {
            auto const tags=owner.tags->template view<Address,Shared>(memory.size());
            for(::std::size_t offset{};offset<memory.size();)
            {
                auto tag=tags.load(static_cast<Address>(offset));if(!tag)return false;
                auto end=offset+(::std::min)(::std::size_t{16u},memory.size()-offset);
                // This actual instruction turn excludes guest/tag writers.
                // A single tagged native copy may cover adjacent granules with
                // the same allocation tag. Never cross an unchecked tag or the
                // actual memory extent; do not disable hardware checking.
                while(end<memory.size())
                {
                    auto next=tags.load(static_cast<Address>(end));if(!next)return false;
                    if(next.allocation_tag!=tag.allocation_tag)break;
                    end+=(::std::min)(::std::size_t{16u},memory.size()-end);
                }
                transfer(mt::hardware_mte::tagged_pointer(memory.data()+offset,tag.allocation_tag),offset,end-offset);
                offset=end;
            }
            return true;
        };
        return owner.tags->width()==mt::address_width::i64 ?
            (owner.tags->shared()?copy.template operator()<::std::uint64_t,true>():copy.template operator()<::std::uint64_t,false>()) :
            (owner.tags->shared()?copy.template operator()<::std::uint32_t,true>():copy.template operator()<::std::uint32_t,false>());
    }
    void tag_guard(bool replay)
    {
        namespace cp=::uwvm2::uwvm::debugger::checkpoint;
        bool active{};for(auto const& owner:owners_)active|=owner.tags->enabled();
        if(!active)return;
        object guard{};guard.kind=cp::object_kind::event;guard.flags=cp::execution_replay::tags_kind;
        guard.words={session_->events.size()+1u,session_->instructions,session_->logical_thread(g_debug_pause_participant->identifier()),event_.words[3],0u,0u,cp::execution_replay::schema,0u};
        for(auto const& owner:owners_)
        {
            guard.links.push_back(owner.actual.memory);
            auto const bytes=owner.actual.bytes.size();
            ::std::span<::std::byte const> tags{};
            if(owner.tags->enabled())tags={static_cast<::std::byte const*>(owner.tags->tag_data()),bytes/16u+(bytes%16u!=0u)};
            auto hash=cp::replay_memory_detail::hash(tags);
            guard.bytes.insert(guard.bytes.end(),hash.begin(),hash.end());
        }
        if(replay)
        {
            session_->consume_ready();
            if(session_->cursor>=session_->events.size())reject(error::replay_diverged);
            auto const& saved=session_->events[session_->cursor];
            if(saved.flags!=guard.flags || saved.words[1]!=guard.words[1] || saved.words[2]!=guard.words[2] ||
               saved.words[3]!=guard.words[3] || saved.links!=guard.links || saved.bytes!=guard.bytes)reject(error::replay_diverged);
            session_->consume();
        }
        else session_->append(::std::move(guard));
    }

    void reject(error why) noexcept
    {
        session_->fail(why);
        // No provider may run on a failed replay transaction. This cold bridge
        // is before any guest/result write. 125 is a stable replay refusal.
        checkpoint_replay_refuse(*session_);
    }
#include "uwvm_runtime_checkpoint_execution_replay_import_phases.h"
public:
    checkpoint_replay_import(cached_import_target const* target,::std::uintptr_t parameters,::std::size_t bytes,
        ::std::uintptr_t results,::std::size_t result_bytes):session_{checkpoint_replay_current()},target_{target},results_{results},result_bytes_{result_bytes}
    {
        namespace cp=::uwvm2::uwvm::debugger::checkpoint;
        if(!session_)return;
        ::std::unique_lock lock{session_->mutex};
        session_->consume_ready();
        if(session_->mode==2u){session_.reset();return;}
        bool const replay=session_->mode==3u;
        if(session_->mode!=1u && !replay)reject(error::replay_diverged);
        ::uwvm2::uwvm::runtime::full::builtin_checkpoint_function_data builtin{};
        if(!target || checkpoint_effect_actual_cached_leaf(*target,true,::std::addressof(builtin))!=checkpoint_resolved_effect::checkpoint_builtin_host ||
           target->reference_params || target->reference_results || session_->epoch!=current_runtime_generation())
        {
            if(replay)reject(error::non_replayable_import);
            session_->fail(error::non_replayable_import);completed_=true;return;
        }
        try
        {
            if(!g_debug_pause_participant || active_)reject(error::replay_diverged);
            participant_=g_debug_pause_participant->identifier();logical_=session_->logical_thread(participant_);
            if(!logical_ || session_->running!=participant_)reject(error::replay_diverged);
            wasi_phased_=builtin.ros_family==0u && builtin.phased_memory;
            phased_=wasi_phased_ || (builtin.ros_family>=1u && builtin.ros_family<=4u);
            auto const module=session_->logical_module(target->origin_module_id);
            if(module>UINT32_MAX || target->frame.function_index>UINT32_MAX || !session_->instructions ||
               !read_values(target->signature().params,parameters,bytes,event_.values) ||
               (result_bytes && !results) || !collect_memories())reject(error::replay_diverged);
            event_.kind=cp::object_kind::event;
            event_.flags=phased_?cp::execution_replay::import_begin_kind:cp::execution_replay::import_kind;
            event_.words={session_->events.size()+1u,session_->instructions,session_->logical_thread(g_debug_pause_participant->identifier()),
                (::std::uint64_t{module}<<32u)|target->frame.function_index,event_.values.size(),0u,cp::execution_replay::schema,0u};
            for(auto const& memory:memories_)event_.links.push_back(memory.memory);
            tag_guard(replay);
            event_.words[0]=session_->events.size()+1u;
            if(phased_)
            {
                begin_phased(lock,replay);return;
            }
            if(replay)
            {
                session_->consume_ready();
                if(session_->cursor>=session_->events.size())reject(error::replay_diverged);
                auto const& saved=session_->events[session_->cursor];
                if(saved.flags!=event_.flags || saved.words[1]!=event_.words[1] || saved.words[2]!=event_.words[2] ||
                   saved.words[3]!=event_.words[3] || saved.words[4]!=event_.words[4] || saved.links!=event_.links ||
                   saved.values.size()<event_.values.size() ||
                   !::std::equal(event_.values.begin(),event_.values.end(),saved.values.begin()))reject(error::replay_diverged);
                // Validate the complete numeric output signature before writes.
                ::std::size_t length{},index=event_.values.size();
                for(::std::size_t type_index{};type_index<target->signature().results.size;++type_index)
                {
                    auto type=target->signature().results.at(type_index);
                    auto n=width(type);
                    if(!n || index>=saved.values.size() || saved.values[index].type.kind!=static_cast<cp::value_kind>(0x80u-static_cast<unsigned>(type)) ||
                       length>result_bytes || n>result_bytes-length)reject(error::incompatible_type);
                    length+=n;++index;
                }
                if(length!=result_bytes || index!=saved.values.size())reject(error::incompatible_type);
                ::uwvm2::utils::container::std_vector<cp::replay_memory_detail::write> writes{};
                auto checked=cp::replay_memory_detail::prepare(saved.bytes,memories_,writes);
                if(checked!=error::none)reject(checked);
                for(auto const& write:writes)::std::memcpy(write.target.data(),write.value.data(),write.value.size());
                for(auto& owner:owners_)if(!copy_memory(owner,true))reject(error::replay_diverged);
                auto output=reinterpret_cast<::std::byte*>(results);::std::size_t offset{};index=event_.values.size();
                for(::std::size_t type_index{};type_index<target->signature().results.size;++type_index)
                {
                    auto type=target->signature().results.at(type_index);
                    auto const& result=saved.values[index++];auto n=width(type);
                    if(n==4u){auto bits=static_cast<::std::uint32_t>(result.low_bits);::std::memcpy(output+offset,&bits,4u);}
                    else {::std::memcpy(output+offset,&result.low_bits,8u);if(n==16u)::std::memcpy(output+offset+8u,&result.high_bits,8u);}
                    offset+=n;
                }
                session_->consume();replayed_=completed_=true;return;
            }
            before_.reserve(memories_.size());
            for(auto const& memory:memories_)before_.emplace_back(memory.bytes.begin(),memory.bytes.end());
        }
        catch(...){reject(error::limit_exceeded);}
    }
    [[nodiscard]] bool replayed() const noexcept{return replayed_;}
    void finish()
    {
        namespace cp=::uwvm2::uwvm::debugger::checkpoint;
        if(!session_ || completed_)return;
        if(phased_){finish_phased();return;}
        ::std::lock_guard lock{session_->mutex};
        try
        {
            if(session_->mode!=1u || !read_values(target_->signature().results,results_,result_bytes_,event_.values))reject(error::replay_diverged);
            event_.words[5]=event_.values.size()-event_.words[4];
            for(auto& owner:owners_)if(!copy_memory(owner,false))reject(error::replay_diverged);
            ::uwvm2::utils::container::std_vector<image> before{},after{};
            for(::std::size_t i{};i<memories_.size();++i)
            {before.push_back({memories_[i].memory,before_[i]});after.push_back({memories_[i].memory,memories_[i].bytes});}
            auto checked=cp::encode_replay_memory_effect(before,after,event_.bytes);
            if(checked!=error::none)reject(checked);
            event_.words[0]=session_->events.size()+1u;
            session_->append(::std::move(event_));completed_=true;
        }
        catch(...){reject(error::limit_exceeded);}
    }
    ~checkpoint_replay_import()
    {
        if(retained_host_)
        {
            if(!g_debug_pause_participant)::fast_io::fast_terminate();
            static_cast<void>(g_debug_pause_participant->return_from_host_operation());
            retained_host_=false;
        }
        if(::uwvm2::runtime::checkpoint::host_memory_phase::current==&wasi_consumer_)
            ::uwvm2::runtime::checkpoint::host_memory_phase::current=nullptr;
        if(::uwvm2::runtime::memtag::report::current_observer==&report_observer_)
            ::uwvm2::runtime::memtag::report::current_observer=nullptr;
        if(active_==this)active_=nullptr;
        if(session_ && !completed_)
        {::std::lock_guard lock{session_->mutex};
         if(logical_ && logical_<=session_->retained_imports.size())session_->retained_imports[logical_-1u]=0u;
         session_->fail(error::non_replayable_import);}
    }
};
