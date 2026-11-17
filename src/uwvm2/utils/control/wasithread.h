/** @file
 * @brief Independent WASI Threads (I1T) authority and atomic admission accounting.
 * @details The guest ABI remains wasi.thread-spawn. This policy is independent
 * of WASI Preview 1. Unassigned modules select Global; explicit Single/Group membership
 * replaces that authority without inheritance. Global count limits additionally cover
 * every admitted main and child, including Single/Group members. Group limits
 * are shared, never copied per member. Limits are optional attenuation of an
 * explicit grant: absent means unlimited, zero means no admission.
 *
 * Configuration freezes at the first main admission. Reservation ownership
 * passes from the requesting host through compilation to the concrete child
 * work, and ends only after physical join and resource cleanup (or failed
 * preparation with no native child). No allocation, callback, guest execution,
 * compilation or wait occurs under the admission mutex. Configuration uses
 * recoverable fast_io allocation, including builds without C++ exceptions.
 */
#pragma once
#include <uwvm2/utils/container/unique.h>
#include <cstdint>
#include <cstring>
#include <mutex>
#include <string_view>
#include <span>
#include <utility>

namespace uwvm2::utils::control::wasithread
{
    enum class scope : unsigned char { global, single, group };
    enum class exposure : unsigned char { hidden, deny, allow };
    enum class limit : unsigned char { extra_threads, threads, spawns };
    enum class failure : unsigned char { none, unbound, disabled, hidden, denied, execution_profile, extra_threads, threads, spawns, exhausted };
    inline constexpr ::std::uint64_t unlimited=UINT64_MAX;
    [[nodiscard]] inline constexpr ::std::string_view name(scope value) noexcept
    { return value==scope::global?"global":value==scope::single?"single":value==scope::group?"group":"invalid"; }
    [[nodiscard]] inline constexpr ::std::string_view name(exposure value) noexcept
    { return value==exposure::hidden?"hidden":value==exposure::deny?"deny":value==exposure::allow?"allow":"invalid"; }
    /** @brief Decimal limits are exact; the reserved maximum uses 'unlimited'. */
    [[nodiscard]] inline bool parse_limit(::std::u8string_view text,::std::uint64_t& out) noexcept
    {
        if(text==u8"unlimited") { out=unlimited;return true; }
        if(text.empty())return false;
        ::std::uint64_t value{};
        for(auto c:text)
        {
            if(c<u8'0'||c>u8'9')return false;
            auto digit=static_cast<unsigned>(c-u8'0');
            if(value>(unlimited-digit)/10u)return false;
            value=value*10u+digit;
        }
        if(value==unlimited)return false;
        out=value;return true;
    }
    struct limits { ::std::uint64_t extra_threads{unlimited},threads{unlimited},spawns{unlimited}; };
    struct counters { ::std::uint64_t mains{},children{},spawn_admissions{}; };
    struct observation
    {
        bool exists{},enabled{},sealed{};
        scope kind{scope::global};
        ::std::u8string_view target;
        exposure interface{exposure::hidden};
        limits bound;
        counters occupied;
    };

    struct membership { ::std::u8string_view module; scope kind{}; ::std::u8string_view target; };
    struct page { ::std::size_t total{},copied{};bool sealed{}; };

    class policy final
    {
        struct text
        {
            ::uwvm2::utils::container::std_unique_ptr<char8_t[]> bytes;
            ::std::size_t size{};
            bool assign(::std::u8string_view value) noexcept
            {
                auto* next=::uwvm2::utils::allocator::fast_io_strict::try_array<char8_t>(value.size(),false);
                if(!next)return false;
                if(!value.empty())::std::memcpy(next,value.data(),value.size());
                bytes.reset(next);size=value.size();return true;
            }
            ::std::u8string_view view() const noexcept { return {bytes.get(),size}; }
        };
        struct target
        {
            target* next{};
            text key;
            scope kind{scope::global};
            bool enabled{};
            exposure interface{exposure::hidden};
            limits bound;
            counters occupied;
        };
        struct binding { binding* next{};text module;target* selected{}; };
        mutable ::std::mutex mutex_;
        target global_;
        target* targets_{};
        binding* bindings_{};
        bool sealed_{};
        [[nodiscard]] static bool valid_name(::std::u8string_view value) noexcept
        {
            if(value.empty())return false;
            for(auto c:value)if(c==u8'\0'||c<u8' '||c==u8'\x7f')return false;
            return true;
        }
        target* find(scope kind,::std::u8string_view key) const noexcept
        {
            if(kind==scope::global)return key.empty()?const_cast<target*>(&global_):nullptr;
            for(auto* item=targets_;item;item=item->next)if(item->kind==kind&&item->key.view()==key)return item;
            return nullptr;
        }
        target* explicitly_bound(::std::u8string_view module) const noexcept
        {
            for(auto* item=bindings_;item;item=item->next)if(item->module.view()==module)return item->selected;
            return nullptr;
        }
        target* resolve(::std::u8string_view module) const noexcept
        {
            if(!valid_name(module))return nullptr;
            if(auto* selected=explicitly_bound(module))return selected;
            return const_cast<target*>(&global_);
        }
        static observation observe(target const* item,bool sealed) noexcept
        {
            if(!item){observation empty{};empty.sealed=sealed;return empty;}
            return {true,item->enabled,sealed,item->kind,item->key.view(),item->interface,item->bound,item->occupied};
        }
        static failure capacity(target const& item,bool child) noexcept
        {
            auto const& count=item.occupied;
            if(count.mains>=unlimited-count.children)return failure::exhausted;
            if(count.mains+count.children>=item.bound.threads)return failure::threads;
            if(child)
            {
                if(count.children>=item.bound.extra_threads)return failure::extra_threads;
                if(count.spawn_admissions>=item.bound.spawns)return failure::spawns;
            }
            return failure::none;
        }
        static void increment(target& item,bool child) noexcept
        { if(child){++item.occupied.children;++item.occupied.spawn_admissions;}else ++item.occupied.mains; }
        void release(target* selected,bool child) noexcept
        {
            ::std::lock_guard lock{mutex_};
            auto decrement=[child](target& item) noexcept { if(child)--item.occupied.children;else --item.occupied.mains; };
            decrement(global_);if(selected&&selected!=&global_)decrement(*selected);
        }
    public:
        /** @brief Move-only concurrency charge; returned bodies remain charged until join.
         * @details The policy must outlive all reservations. Host runtime storage
         * has process lifetime. A failed native creation releases main/child
         * occupancy; a successful admission still consumes max-spawns, preventing
         * allocation/compilation failure loops from bypassing that optional cap.
         */
        class reservation final
        {
            friend class policy;
            policy* owner_{};target* selected_{};bool child_{};
            reservation(policy* owner,target* selected,bool child) noexcept:owner_(owner),selected_(selected),child_(child){}
        public:
            reservation() noexcept=default;
            reservation(reservation const&)=delete;
            reservation& operator=(reservation const&)=delete;
            reservation(reservation&& other) noexcept:owner_(::std::exchange(other.owner_,nullptr)),selected_(other.selected_),child_(other.child_){}
            reservation& operator=(reservation&& other) noexcept
            {if(this!=&other){reset();owner_=::std::exchange(other.owner_,nullptr);selected_=other.selected_;child_=other.child_;}return *this;}
            ~reservation(){reset();}
            void reset() noexcept {if(auto* owner=::std::exchange(owner_,nullptr))owner->release(selected_,child_);}
            explicit operator bool() const noexcept{return owner_!=nullptr;}
        };
        struct admission { reservation held;failure reason{failure::none};observation source; };
        policy() noexcept=default;
        policy(policy const&)=delete;
        policy& operator=(policy const&)=delete;
        ~policy()
        {
            // Local test/embedding owners must have drained their reservations.
            // Iterative destruction avoids configuration-size-dependent recursion.
            while(bindings_){auto* next=bindings_->next;::uwvm2::utils::allocator::fast_io_strict::destroy_object(bindings_);bindings_=next;}
            while(targets_){auto* next=targets_->next;::uwvm2::utils::allocator::fast_io_strict::destroy_object(targets_);targets_=next;}
        }
        /** @brief Create an initially denied scope, publishing only after all allocations succeed. */
        bool create(scope kind,::std::u8string_view key) noexcept
        {
            if((kind!=scope::single&&kind!=scope::group)||!valid_name(key))return false;
            using namespace ::uwvm2::utils::allocator::fast_io_strict;
            ::uwvm2::utils::container::std_unique_ptr<target> value{try_object<target>([](target* p) noexcept {return ::std::construct_at(p);})};
            ::uwvm2::utils::container::std_unique_ptr<binding> member;
            if(!value||!value->key.assign(key))return false;
            value->kind=kind;
            if(kind==scope::single)
            {
                member.reset(try_object<binding>([](binding* p) noexcept {return ::std::construct_at(p);}));
                if(!member||!member->module.assign(key))return false;
                member->selected=value.get();
            }
            ::std::lock_guard lock{mutex_};
            if(sealed_||find(kind,key)||(kind==scope::single&&explicitly_bound(key)))return false;
            value->next=targets_;targets_=value.release();
            if(member){member->next=bindings_;bindings_=member.release();}
            return true;
        }
        /** @brief Move a module from implicit Global into one Group; duplicates are forbidden. */
        bool bind(scope kind,::std::u8string_view key,::std::u8string_view module) noexcept
        {
            if(kind!=scope::group||!valid_name(module))return false;
            ::uwvm2::utils::container::std_unique_ptr<binding> value{
                ::uwvm2::utils::allocator::fast_io_strict::try_object<binding>([](binding* p) noexcept {return ::std::construct_at(p);})};
            if(!value||!value->module.assign(module))return false;
            ::std::lock_guard lock{mutex_};auto* selected=find(kind,key);
            if(sealed_||!selected||explicitly_bound(module))return false;
            value->selected=selected;value->next=bindings_;bindings_=value.release();return true;
        }
        bool enable(scope kind,::std::u8string_view key,bool value) noexcept
        {::std::lock_guard lock{mutex_};auto* item=find(kind,key);if(sealed_||!item)return false;item->enabled=value;return true;}
        bool expose(scope kind,::std::u8string_view key,exposure value) noexcept
        {if(value!=exposure::hidden&&value!=exposure::deny&&value!=exposure::allow)return false;
            ::std::lock_guard lock{mutex_};auto* item=find(kind,key);if(sealed_||!item)return false;item->interface=value;return true;}
        bool constrain(scope kind,::std::u8string_view key,limit field,::std::uint64_t value) noexcept
        {
            ::std::lock_guard lock{mutex_};auto* item=find(kind,key);if(sealed_||!item)return false;
            switch(field){case limit::extra_threads:item->bound.extra_threads=value;break;case limit::threads:item->bound.threads=value;break;
                case limit::spawns:item->bound.spawns=value;break;default:return false;}return true;
        }
        [[nodiscard]] observation inspect(scope kind,::std::u8string_view key={}) const noexcept
        {::std::lock_guard lock{mutex_};return observe(find(kind,key),sealed_);}
        [[nodiscard]] observation effective(::std::u8string_view module) const noexcept
        {::std::lock_guard lock{mutex_};return observe(resolve(module),sealed_);}
        /** @brief Copy cold diagnostic data without allocations, callbacks or guest/native resources.
         * Names refer to immutable configuration owned by this policy. The caller
         * must retain the owner; runtime_policy() has process lifetime. Pagination
         * is stable once sealed and never authorizes execution or count rewind.
         */
        [[nodiscard]] page inspect_targets(::std::size_t offset,::std::span<observation> out) const noexcept
        {
            ::std::lock_guard lock{mutex_};page result{0,0,sealed_};
            auto copy=[&](target const* item) noexcept
            {if(result.total++>=offset&&result.copied<out.size())out[result.copied++]=observe(item,sealed_);};
            copy(&global_);for(auto* item=targets_;item;item=item->next)copy(item);return result;
        }
        [[nodiscard]] page inspect_memberships(::std::size_t offset,::std::span<membership> out) const noexcept
        {
            ::std::lock_guard lock{mutex_};page result{0,0,sealed_};
            for(auto* item=bindings_;item;item=item->next)
                if(result.total++>=offset&&result.copied<out.size())
                    out[result.copied++]={item->module.view(),item->selected->kind,item->selected->key.view()};
            return result;
        }
        [[nodiscard]] bool visible(::std::u8string_view module) const noexcept
        {auto value=effective(module);return value.exists&&value.enabled&&value.interface!=exposure::hidden;}
        /** @brief Atomically reserve both aggregate and selected-scope occupancy.
         * @details Main admission does not grant spawn authority. All main
         * threads count, even without an I1T binding. Child authority is resolved
         * from the authenticated executing module, never the root/provider name.
         * No failed capacity/permission check changes any counter. Admission is
         * immediate; it never waits for capacity or attenuates existing threads.
         */
        [[nodiscard]] admission reserve(::std::u8string_view module,bool child) noexcept
        {
            ::std::lock_guard lock{mutex_};auto* selected=resolve(module);
            auto reject=[&](failure reason,target* target) noexcept -> admission {return {{},reason,observe(target,sealed_)};};
            if(!selected)return reject(failure::unbound,nullptr);
            if(child)
            {
                if(!sealed_)return reject(failure::disabled,selected);
                if(!selected->enabled)return reject(failure::disabled,selected);
                if(selected->interface==exposure::hidden)return reject(failure::hidden,selected);
                if(selected->interface!=exposure::allow)return reject(failure::denied,selected);
            }
            auto reason=capacity(global_,child);if(reason!=failure::none)return reject(reason,&global_);
            if(selected&&selected!=&global_){reason=capacity(*selected,child);if(reason!=failure::none)return reject(reason,selected);}
            sealed_=true;increment(global_,child);if(selected&&selected!=&global_)increment(*selected,child);
            return {reservation{this,selected,child},failure::none,observe(selected,sealed_)};
        }
    };
    /** @brief Process-lifetime policy; creation alone grants nothing and allocates nothing. */
    inline policy& runtime_policy() noexcept
    {
        alignas(policy) static ::std::byte storage[sizeof(policy)];
        static policy* const value{::new(static_cast<void*>(storage)) policy{}};
        return *value;
    }
}
