// Include INSIDE uwvm2::runtime::lib after its real canonical WASI environment
// resolver. This is not an ordinary WASI function or an exported Host API.
#if defined(__linux__) && defined(UWVM_UTILS_HAS_FAST_IO_NATIVE_THREAD) && defined(UWVM_CPP_EXCEPTIONS) && defined(UWVM_IMPORT_WASI_WASIP1) && !defined(UWVM_DISABLE_LOCAL_IMPORTED_WASIP1)
extern "C++"
{
    class runtime_threaded_wasip1_adapter final
    {
        friend class runtime_threaded_profile_bridge;
        using group=::uwvm2::runtime::wasm_threads::ThreadedGroup;
        using lease=group::ChildLease;
        using world=::uwvm2::uwvm::runtime::initializer::ThreadedChildWorld;
        using source=::uwvm2::uwvm::runtime::full::full_source_instance;
        using provider=::uwvm2::uwvm::wasm::type::local_imported_t;
        using environment=::uwvm2::uwvm::imported::wasi::wasip1::storage::wasip1_env_type;
        using memory=::uwvm2::object::memory::linear::native_memory_t;
        using error=::uwvm2::imported::wasi::wasip1::abi::errno_t;
        using right=::uwvm2::imported::wasi::wasip1::abi::rights_t;
        using clock=::uwvm2::imported::wasi::wasip1::abi::clockid_t;
        using permission=::uwvm2::imported::wasi::wasip1::environment::permission_t;
        using dispatch_status=::uwvm2::runtime::wasm_threads::host_dispatch_status;
        using admission_status=::uwvm2::runtime::wasm_threads::import_admission;
        using native_fd=::uwvm2::imported::wasi::wasip1::fd_manager::wasi_fd_t;
        using fd_ref=::uwvm2::imported::wasi::wasip1::fd_manager::wasi_fd_ref_t;
        enum class operation {args_sizes,args_get,env_sizes,env_get,clock_time,clock_res,yield,random,read,write,poll};
        struct binding {operation code{};environment* env{};::std::size_t parameter_bytes{};::std::size_t interface_id{};};
        static constexpr ::std::size_t transfer_bytes{PIPE_BUF}; // Native packet capacity; legal partial I/O, never a request limit.
        template<typename T> [[nodiscard]] static ::std::unique_ptr<T[]> allocate_array(::std::size_t count) noexcept
        {
            if(count > (SIZE_MAX / sizeof(T))) { return {}; }
            try { return ::std::make_unique<T[]>(count); }
            catch(...) { return {}; } // Allocation only: no Guest/native work is in this catch scope.
        }
        static constexpr ::std::uint32_t trap_memory{0x57415301u},trap_native{0x57415302u};
        struct context final
        {
            lease& actual;world const& instance;environment& env;memory& shared;
            ::std::size_t module{},function{};
            [[nodiscard]] bool stopped() const noexcept {
#if defined(UWVM_ROS_ZERO_HOST_DEFAULTS)
                if(::uwvm2::imported::wasi::wasip1::environment::process_budget_ledger.cancelled.load(::std::memory_order_acquire)) return true;
                auto* scope{::uwvm2::imported::wasi::wasip1::environment::host_wait_scope::current};
                if(scope && scope->exit_owner.cancelled()) return true;
#endif
                return actual.cancellation().stop_requested();}
            [[nodiscard]] bool native_failure() noexcept
            {actual.request_trap(trap_native,module,function,UINT64_MAX);return false;}
            [[nodiscard]] bool bounds(::std::uint64_t offset,::std::size_t bytes,::std::byte*& pointer) noexcept
            {
                ::std::size_t length{};
                if(::uwvm2::runtime::wasm_threads::details::access(shared,offset,bytes,length,[&](::std::byte* p) noexcept{pointer=p;}))return true;
                actual.request_trap(trap_memory,module,function,UINT64_MAX);return false;
            }
            [[nodiscard]] bool aligned(::std::uint32_t offset,unsigned alignment) noexcept
            {if((offset&(alignment-1u))==0u)return true;actual.request_trap(trap_memory,module,function,UINT64_MAX);return false;}
            using byte_type=typename ::uwvm2::runtime::compiler::shared::wasm_threads::atomic_access_traits<1>::type;
            [[nodiscard]] bool copy_in(::std::uint64_t offset,::std::span<::std::byte> out) noexcept
            {
                ::std::byte* pointer{};if(!bounds(offset,out.size(),pointer))return false;
                for(::std::size_t i{};i<out.size();++i)
                {if((i&1023uz)==0u&&stopped())return false;out[i]=static_cast<::std::byte>(__atomic_load_n(reinterpret_cast<byte_type*>(pointer+i),__ATOMIC_RELAXED));}
                return !stopped();
            }
            [[nodiscard]] bool copy_out(::std::uint64_t offset,::std::span<::std::byte const> input) noexcept
            {
                ::std::byte* pointer{};if(!bounds(offset,input.size(),pointer))return false;
                for(::std::size_t i{};i<input.size();++i)
                {if((i&1023uz)==0u&&stopped())return false;__atomic_store_n(reinterpret_cast<byte_type*>(pointer+i),static_cast<byte_type>(::std::to_integer<unsigned char>(input[i])),__ATOMIC_RELAXED);}
                return !stopped();
            }
            template<unsigned Bytes> [[nodiscard]] bool store(::std::uint32_t offset,::std::uint64_t value) noexcept
            {
                ::std::array<::std::byte,Bytes> bytes{};
                for(unsigned i{};i<Bytes;++i)bytes[i]=static_cast<::std::byte>(value>>(i*8u));
                return copy_out(offset,bytes);
            }
        };
        [[nodiscard]] static ::std::uint64_t little(::std::byte const* p,unsigned width) noexcept
        {::std::uint64_t result{};for(unsigned i{};i<width;++i)result|=static_cast<::std::uint64_t>(::std::to_integer<unsigned char>(p[i]))<<(i*8u);return result;}
        static void little_put(::std::byte* p,::std::uint64_t value,unsigned width) noexcept
        {for(unsigned i{};i<width;++i)p[i]=static_cast<::std::byte>(value>>(i*8u));}
        [[nodiscard]] static error native_error(int value) noexcept
        {
            switch(value)
            {
                case EAGAIN:return error::eagain;case EBADF:return error::ebadf;case EACCES:return error::eacces;
                case EINTR:return error::eintr;case EINVAL:return error::einval;case EPIPE:return error::epipe;
                case ENOMEM:return error::enomem;case EMFILE:return error::emfile;case ENFILE:return error::enfile;
                case ENOSPC:return error::enospc;case EOVERFLOW:return error::eoverflow;case ECONNRESET:return error::econnreset;
                case ENOTCONN:return error::enotconn;case ETIMEDOUT:return error::etimedout;case EISDIR:return error::eisdir;
                case ENOTSOCK:return error::enotsock;case ENOSYS:case EOPNOTSUPP:return error::enotsup;
                default:return error::eio;
            }
        }
        [[nodiscard]] static bool authenticate(world const& instance,::std::size_t module_index,::std::size_t imported,
            provider const& actual_provider,binding& out) noexcept
        {
            if(module_index!=0u||!instance.admission().owns_actual_generation()||
               !source::has_canonical_owner(instance.source_owner()))return false;
            auto validated=instance.validated_code();auto const* child=instance.module(module_index);
            auto const& original=instance.admission().source_module();
            if(!validated||!validated->covers(instance)||!child||imported>=child->imported_function_vec_storage.size()||
               imported>=original.imported_function_vec_storage.size())return false;
            auto const& a=child->imported_function_vec_storage.index_unchecked(imported);
            auto const& b=original.imported_function_vec_storage.index_unchecked(imported);
            using kind=::uwvm2::uwvm::runtime::storage::imported_function_link_kind;
            if(a.link_kind!=kind::local_imported||b.link_kind!=kind::local_imported||a.import_type_ptr==nullptr||
               a.import_type_ptr!=b.import_type_ptr||a.target.local_imported.module_ptr!=&actual_provider||
               b.target.local_imported.module_ptr!=&actual_provider||a.target.local_imported.index!=b.target.local_imported.index)return false;
            ::uwvm2::uwvm::runtime::full::builtin_wasip1_function_data concrete{};
            if(!instance.source_owner()->actual_builtin_wasip1_function(instance.source_module_id(),instance.admission().source_validation_epoch(),
                &original,imported,concrete)||concrete.function_index!=a.target.local_imported.index||concrete.exits_guest)return false;
            // No provider dereference occurs before actual canonical provenance.
            auto const information=actual_provider.get_function_information_from_index(a.target.local_imported.index);
            if(!information.successed||concrete.result_count!=1u||concrete.results[0]!=0x7fu)return false;
            auto const name=information.function_name;operation op{};unsigned count{};int wide{-1};
            if(name==u8"args_sizes_get"){op=operation::args_sizes;count=2;}
            else if(name==u8"args_get"){op=operation::args_get;count=2;}
            else if(name==u8"environ_sizes_get"){op=operation::env_sizes;count=2;}
            else if(name==u8"environ_get"){op=operation::env_get;count=2;}
            else if(name==u8"clock_time_get"){op=operation::clock_time;count=3;wide=1;}
            else if(name==u8"clock_res_get"){op=operation::clock_res;count=2;}
            else if(name==u8"sched_yield"){op=operation::yield;count=0;}
            else if(name==u8"random_get"){op=operation::random;count=2;}
            else if(name==u8"fd_read"){op=operation::read;count=4;}
            else if(name==u8"fd_write"){op=operation::write;count=4;}
            else if(name==u8"poll_oneoff"){op=operation::poll;count=4;}
            else return false; // wasm64/filesystem/other imports are not widened.
            if(concrete.parameter_count!=count)return false;
            auto const* signature=a.import_type_ptr->imports.storage.function;if(!signature)return false;
            if((count==0u ? signature->parameter.begin!=signature->parameter.end :
                (signature->parameter.begin==nullptr||signature->parameter.end==nullptr||signature->parameter.end-signature->parameter.begin!=static_cast<::std::ptrdiff_t>(count)))||
               signature->result.begin==nullptr||signature->result.end==nullptr||signature->result.end-signature->result.begin!=1)return false;
            ::std::size_t bytes{};
            for(unsigned i{};i<count;++i)
            {
                unsigned const expected{static_cast<int>(i)==wide?0x7eu:0x7fu};
                if(concrete.parameters[i]!=expected||static_cast<unsigned>(signature->parameter.begin[i])!=expected)return false;
                bytes+=expected==0x7eu?8uz:4uz;
            }
            if(static_cast<unsigned>(signature->result.begin[0])!=0x7fu)return false;
            auto& env=resolve_wasip1_env_for_runtime_module_id(instance.source_module_id());
#if defined(UWVM_ROS_ZERO_HOST_DEFAULTS)
            if(!env.interfaces.exposed(::uwvm2::utils::control::host_interfaces::find(::uwvm2::utils::control::host_interfaces::family::wasip1, name))) return false;
#endif
            // These are actual pre-existing overrides, not readiness/authority.
            // No bypass into a possibly blocking debugger/trace callback.
            if(env.trace_wasip1_call||env.debug_console_read!=nullptr||env.debug_console_write!=nullptr||
               (op==operation::yield&&env.wasip1_sched_yield_func_ptr!=nullptr))return false;
            out={op,&env,bytes,::uwvm2::utils::control::host_interfaces::find(::uwvm2::utils::control::host_interfaces::family::wasip1,name)};return true;
        }
        [[nodiscard]] static admission_status admit(world const& instance,::std::size_t module,::std::size_t imported,provider const& actual) noexcept
        {binding found{};return authenticate(instance,module,imported,actual,found)?admission_status::allowed:admission_status::unsupported;}

        template<typename Strings>
        [[nodiscard]] static error strings(context& call,Strings const& strings,::std::uint32_t first,::std::uint32_t second,bool sizes) noexcept
        {
            if(strings.size()>UINT32_MAX||strings.size()>SIZE_MAX/4u)return error::eoverflow;
            ::std::uint64_t total{};
            for(auto const& string:strings)
            {if(call.stopped())return error::ecanceled;if(string.size()>=UINT32_MAX-total)return error::eoverflow;total+=string.size()+1u;}
#if defined(UWVM_ROS_ZERO_HOST_DEFAULTS)
            if(total > call.env.limits.max_args_env_bytes)
            { ::uwvm2::utils::control::host_diagnostics::limited("args-env-bytes",false); return error::e2big; }
#endif
            ::std::byte* unused{};
            if(!call.aligned(first,4u)||!call.bounds(first,sizes?4uz:strings.size()*4uz,unused)||
               !call.bounds(second,sizes?4uz:static_cast<::std::size_t>(total),unused)||(sizes&&!call.aligned(second,4u)))return error::efault;
            if(sizes)
            {return call.store<4>(first,strings.size())&&call.store<4>(second,total)?error::esuccess:error::efault;}
            ::std::uint32_t cursor=second;::std::uint32_t index{};
            for(auto const& string:strings)
            {
                if(!call.store<4>(first+index*4u,cursor)||!call.copy_out(cursor,{reinterpret_cast<::std::byte const*>(string.data()),string.size()}))return error::efault;
                cursor+=static_cast<::std::uint32_t>(string.size());
                if(!call.store<1>(cursor++,0u))return error::efault;++index;
            }
            // Exactly count pointers, each string independently NUL-terminated.
            // There is NO trailing pointer-table NULL outside the validated span.
            return error::esuccess;
        }
        [[nodiscard]] static bool clock_native(context& call,::std::uint32_t id,::clockid_t& native,bool timer=false,bool resolution=false) noexcept
        {
            switch(id){case 0:native=CLOCK_REALTIME;break;case 1:native=CLOCK_MONOTONIC;break;
                case 2:native=CLOCK_PROCESS_CPUTIME_ID;break;case 3:native=CLOCK_THREAD_CPUTIME_ID;break;default:return false;}
            namespace policy=::uwvm2::imported::wasi::wasip1::environment;
            return call.env.permits(timer ? policy::clock_timer_permission(static_cast<clock>(id)) :
                resolution ? policy::clock_resolution_permission(static_cast<clock>(id)) : policy::clock_permission(static_cast<clock>(id)));
        }
        [[nodiscard]] static error get_clock(context& call,::std::uint32_t id,::std::uint32_t pointer,bool resolution) noexcept
        {
            ::clockid_t native{};if(id>3u)return error::einval;
            if(!clock_native(call,id,native,false,resolution))return error::enotcapable;
            ::std::byte* unused{};if(!call.aligned(pointer,8u)||!call.bounds(pointer,8uz,unused))return error::efault;
#if defined(UWVM_ROS_ZERO_HOST_DEFAULTS)
            ::uwvm2::imported::wasi::wasip1::environment::budget_reservation reservation{
                static_cast<::uwvm2::imported::wasi::wasip1::environment::budget_kind>(static_cast<unsigned>(::uwvm2::imported::wasi::wasip1::environment::budget_kind::realtime) + id), call.env.limits, 0u};
            if(!reservation) return reservation.error<error>();
#endif
            ::fast_io::posix_clock_id sample_clock;
            switch(id)
            {
                case 0: sample_clock=::fast_io::posix_clock_id::realtime; break;
                case 1: sample_clock=::fast_io::posix_clock_id::monotonic; break;
                case 2: sample_clock=::fast_io::posix_clock_id::process_cputime_id; break;
                default: sample_clock=::fast_io::posix_clock_id::thread_cputime_id; break;
            }
            ::fast_io::unix_timestamp ts{};
            try { ts=resolution ? ::fast_io::posix_clock_getres(sample_clock) : ::fast_io::posix_clock_gettime(sample_clock); }
            catch(::fast_io::error const&) { return error::eio; }
            constexpr ::std::uint64_t billion{1'000'000'000u};
            constexpr auto fraction{::fast_io::uint_least64_subseconds_per_second/billion};
            auto const nanos{static_cast<::std::uint64_t>(ts.subseconds/fraction)};
            if(ts.seconds<0 || nanos>=billion ||
               static_cast<::std::uint64_t>(ts.seconds)>(UINT64_MAX-nanos)/billion) return error::eoverflow;
            auto value{static_cast<::std::uint64_t>(ts.seconds)*billion+nanos};
#if defined(UWVM_ROS_ZERO_HOST_DEFAULTS)
            auto const granularity{call.env.limits.clock_granularity_ns[id]};
            if(granularity == 0u) return error::enotcapable;
            if(resolution) { if(value < granularity) value = granularity; }
            else value -= value % granularity;
#endif
            return call.store<8>(pointer,value)?error::esuccess:error::efault;
        }
        [[nodiscard]] static error random_bytes(context& call,::std::uint32_t destination,::std::uint32_t bytes) noexcept
        {
            if(!call.env.permits(permission::random))return error::enotcapable;
            ::std::byte* unused{};
            if(!call.bounds(destination,bytes,unused))return error::efault;
#if defined(UWVM_ROS_ZERO_HOST_DEFAULTS)
            // Reserve the full request exactly once, before producing any bytes.
            // A quota wait returns to the outer provider boundary without effects.
            ::uwvm2::imported::wasi::wasip1::environment::budget_reservation reservation{
                ::uwvm2::imported::wasi::wasip1::environment::budget_kind::random,call.env.limits,bytes};
            if(!reservation)return reservation.error<error>();
#endif
            ::std::array<::std::byte,transfer_bytes> scratch{};
            ::std::uint64_t done{};
            unsigned interrupted{};
            while(done<bytes)
            {
                if(call.stopped())return error::ecanceled;
                auto const wanted{static_cast<::std::size_t>((::std::min)(static_cast<::std::uint64_t>(scratch.size()),bytes-done))};
#if defined(UWVM_ROS_ZERO_HOST_DEFAULTS)
                ::uwvm2::imported::wasi::wasip1::environment::process_budget_ledger.native_random_calls.fetch_add(1u,::std::memory_order_relaxed);
#endif
                // Nonblocking kernel entropy. EAGAIN is a real WASI error, not
                // permission to sleep-poll or treat /dev/urandom readiness as
                // completion. Chunk size bounds private storage, never requests.
                auto const generated{::getrandom(scratch.data(),wanted,GRND_NONBLOCK)};
                if(generated<0 && errno==EINTR && ++interrupted<8u)continue;
                if(generated<0)return errno==EINTR?error::eagain:native_error(errno);
                if(generated==0 || static_cast<::std::size_t>(generated)>wanted)return error::eio;
                if(!call.copy_out(static_cast<::std::uint64_t>(destination)+done,
                    {scratch.data(),static_cast<::std::size_t>(generated)}))return call.stopped()?error::ecanceled:error::efault;
                done+=static_cast<::std::uint64_t>(generated);interrupted=0u;
            }
            return error::esuccess;
        }
        struct fd_pin;
        [[nodiscard]] static error wait_ready(context&,int,short,short&) noexcept;
        [[nodiscard]] static error fd_io(context&,bool,::std::uint32_t,::std::uint32_t,::std::uint32_t,::std::uint32_t) noexcept;
        [[nodiscard]] static error poll_oneoff(context&,::std::uint32_t,::std::uint32_t,::std::uint32_t,::std::uint32_t) noexcept;
        [[nodiscard]] static dispatch_status dispatch(lease& actual,world const& instance,::std::size_t module,::std::size_t caller,
            ::std::size_t imported,provider const& native,::std::span<::std::byte> results,::std::span<::std::byte const> params) noexcept
        {
            if(&actual.world()!=&instance||&actual.admission()!=&instance.admission()||!actual.owns_current_native_thread())return dispatch_status::rejected;
            if(actual.cancellation().stop_requested())return dispatch_status::group_stopped;
            binding found{};if(!authenticate(instance,module,imported,native,found)||results.size()!=4u||params.size()!=found.parameter_bytes)return dispatch_status::rejected;
            namespace diagnostic=::uwvm2::utils::control::host_diagnostics;
            namespace wasi_environment=::uwvm2::imported::wasi::wasip1::environment;
            diagnostic::call_scope quota{found.interface_id,wasi_environment::diagnostic_scope(*found.env),wasi_environment::diagnostic_target(*found.env)};
#if defined(UWVM_ROS_ZERO_HOST_DEFAULTS)
            if(!found.env->interfaces.allowed(found.interface_id))
            {
                wasi_environment::diagnostic_denied(*found.env,found.interface_id,true);
                ::std::uint32_t result{static_cast<::std::uint32_t>(error::enotcapable)};
                ::std::memcpy(results.data(),&result,4u);return dispatch_status::returned;
            }
#endif
            auto const* unit=instance.module(module);
            if(!unit||caller<unit->imported_function_vec_storage.size()||caller-unit->imported_function_vec_storage.size()>=unit->local_defined_function_vec_storage.size())return dispatch_status::rejected;
            auto const* shared=instance.shared_memory(0u);if(!shared||!::uwvm2::runtime::wasm_threads::is_shared(shared->memory)||shared->memory.custom_page_size_log2!=16u)return dispatch_status::rejected;
            context call{actual,instance,*found.env,const_cast<memory&>(shared->memory),module,caller};
            ::std::array<::std::uint64_t,4> a{};::std::size_t offset{};
            for(::std::size_t i{};offset<params.size();++i)
            {
                if(i==a.size())return dispatch_status::rejected;
                auto const width=found.code==operation::clock_time&&i==1u?8uz:4uz;
                ::std::memcpy(&a[i],params.data()+offset,width);offset+=width;
            }
            error code{error::enotsup};
            auto const invoke_provider{[&]() noexcept
            {
            switch(found.code)
            {
                case operation::args_sizes:
#if defined(UWVM_ROS_ZERO_HOST_DEFAULTS)
                    if(!call.env.permits(permission::args)) { code=error::enotcapable; break; }
#endif
                    code=strings(call,call.env.argv,a[0],a[1],true);break;
                case operation::args_get:
#if defined(UWVM_ROS_ZERO_HOST_DEFAULTS)
                    if(!call.env.permits(permission::args)) { code=error::enotcapable; break; }
#endif
                    code=strings(call,call.env.argv,a[0],a[1],false);break;
                case operation::env_sizes:
#if defined(UWVM_ROS_ZERO_HOST_DEFAULTS)
                    if(!call.env.permits(permission::environ)) { code=error::enotcapable; break; }
#endif
                    code=strings(call,call.env.envs,a[0],a[1],true);break;
                case operation::env_get:
#if defined(UWVM_ROS_ZERO_HOST_DEFAULTS)
                    if(!call.env.permits(permission::environ)) { code=error::enotcapable; break; }
#endif
                    code=strings(call,call.env.envs,a[0],a[1],false);break;
                case operation::clock_time:code=get_clock(call,a[0],a[2],false);break; // precision is a hint.
                case operation::clock_res:code=get_clock(call,a[0],a[1],true);break;
                case operation::yield:
                {
                    if(!call.env.permits(permission::sched_yield)) { code=error::enotcapable; break; }
#if defined(UWVM_ROS_ZERO_HOST_DEFAULTS)
                    ::uwvm2::imported::wasi::wasip1::environment::budget_reservation reservation{
                        ::uwvm2::imported::wasi::wasip1::environment::budget_kind::yield, call.env.limits, 0u};
                    if(!reservation) { code=reservation.error<error>(); break; }
#endif
                    // Preserve the real environment grant BEFORE the native
                    // effect, exactly as the ordinary canonical WASI leaf.
                    code=call.env.permits(permission::sched_yield)?
                        (::sched_yield()==0?error::esuccess:native_error(errno)):error::enotcapable;
                    break;
                }
                case operation::random:code=random_bytes(call,a[0],a[1]);break;
                case operation::read:case operation::write:code=fd_io(call,found.code==operation::write,a[0],a[1],a[2],a[3]);break;
                case operation::poll:code=poll_oneoff(call,a[0],a[1],a[2],a[3]);break;
            }
            }};
#if defined(UWVM_ROS_ZERO_HOST_DEFAULTS)
            using quota_result=::uwvm2::imported::wasi::wasip1::environment::quota_call_result;
            ::uwvm2::utils::thread::execution_domain::host_wait_registration wake_on_close{
                g_runtime.execution_domain, [](void*) noexcept
                { ::uwvm2::imported::wasi::wasip1::environment::process_budget_ledger.notify_waiters(); }, nullptr};
            auto const completion{::uwvm2::imported::wasi::wasip1::environment::invoke_with_quota_wait(invoke_provider,
                [](void* opaque) noexcept
                {
                    auto& current{*static_cast<context*>(opaque)};
                    // The ordinary public domain is intentionally closed for the
                    // entire admitted threaded profile. Only its actual ChildLease
                    // stop/cancellation and process-exit owner cancel this call.
                    return current.stopped();
                }, &call, actual.cancellation_fd())};
            if(completion != quota_result::completed)
            { code=completion == quota_result::cancelled ? error::ecanceled : error::eio; }
#else
            invoke_provider();
#endif
            if(call.stopped())return dispatch_status::group_stopped;
            if(code==error::enotcapable)
            {
                ::std::uint32_t first{UINT32_MAX};if(params.size()>=4u)::std::memcpy(&first,params.data(),4);
                wasi_environment::diagnostic_denied(*found.env,found.interface_id,false,first);
            }
            ::std::uint32_t result{static_cast<::std::uint32_t>(code)};::std::memcpy(results.data(),&result,4u);return dispatch_status::returned;
        }
    };
}
#include "uwvm_runtime_threaded_wasip1_io.h"
#include "uwvm_runtime_threaded_wasip1_poll.h"
#endif
