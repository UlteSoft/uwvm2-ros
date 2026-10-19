/**
 * @file authority.cc
 * @brief Direct native API tests for WASI Preview1 non-FD authority.
 *
 * Compile this source twice in the verified Linux 64 GiB cgroup, with
 * WASIP1_DEFAULT_DENY_WIDTH=32 and =64. The 64-bit row additionally requires
 * UWVM_ENABLE_LOCAL_IMPORTED_WASIP1_WASM64. Both rows call the low-level host
 * APIs directly, so an interpreter-only permission check cannot satisfy them.
 * All denied calls use valid memory and check complete output sentinels.
 * Callback counters cover the shared implementations without sending signals
 * to the test process. A successful clock or random call uses the real native
 * facility after the corresponding explicit grant has been installed.
 */
#include <array>
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <initializer_list>
#include <memory>
#include <type_traits>
#include <utility>
#include <fast_io.h>

#ifndef WASIP1_DEFAULT_DENY_WIDTH
#define WASIP1_DEFAULT_DENY_WIDTH 32
#endif

#if WASIP1_DEFAULT_DENY_WIDTH == 64
#ifndef UWVM_ENABLE_LOCAL_IMPORTED_WASIP1_WASM64
#error "The 64-bit test must enable the actual WASI Preview1 wasm64 implementation."
#endif
#include <uwvm2/imported/wasi/wasip1/func/clock_res_get_wasm64.h>
#include <uwvm2/imported/wasi/wasip1/func/clock_time_get_wasm64.h>
#include <uwvm2/imported/wasi/wasip1/func/poll_oneoff_wasm64.h>
#include <uwvm2/imported/wasi/wasip1/func/random_get_wasm64.h>
#include <uwvm2/imported/wasi/wasip1/func/proc_raise_wasm64.h>
#include <uwvm2/imported/wasi/wasip1/func/sched_yield_wasm64.h>
#elif WASIP1_DEFAULT_DENY_WIDTH == 32
#include <uwvm2/imported/wasi/wasip1/func/clock_res_get.h>
#include <uwvm2/imported/wasi/wasip1/func/clock_time_get.h>
#include <uwvm2/imported/wasi/wasip1/func/poll_oneoff.h>
#include <uwvm2/imported/wasi/wasip1/func/random_get.h>
#include <uwvm2/imported/wasi/wasip1/func/proc_raise.h>
#include <uwvm2/imported/wasi/wasip1/func/sched_yield.h>
#else
#error "WASIP1_DEFAULT_DENY_WIDTH must be 32 or 64."
#endif

namespace
{
    namespace wasi = ::uwvm2::imported::wasi::wasip1;
    using wasi_clock_id = wasi::abi::clockid_t;
    using wasi::abi::errno_t;
    using wasi::abi::eventtype_t;
    using wasi::abi::rights_t;
    using wasi::environment::permission_t;
    using memory_t = ::uwvm2::object::memory::linear::native_memory_t;
    using environment_t = wasi::environment::wasip1_environment<memory_t>;
#if WASIP1_DEFAULT_DENY_WIDTH == 64
    using pointer_t = wasi::abi::wasi_void_ptr_wasm64_t;
    using wasi_size = wasi::abi::wasi_size_wasm64_t;
    using subscription_t = wasi::func::wasi_subscription_wasm64_t;
    using event_t = wasi::func::wasi_event_wasm64_t;
    inline constexpr auto resolution_call = &wasi::func::clock_res_get_wasm64;
    inline constexpr auto time_call = &wasi::func::clock_time_get_wasm64;
    inline constexpr auto random_call = &wasi::func::random_get_wasm64;
    inline constexpr auto poll_call = &wasi::func::poll_oneoff_wasm64;
    inline constexpr auto raise_call = &wasi::func::proc_raise_wasm64;
    inline constexpr auto yield_call = &wasi::func::sched_yield_wasm64;
#else
    using pointer_t = wasi::abi::wasi_void_ptr_t;
    using wasi_size = wasi::abi::wasi_size_t;
    using subscription_t = wasi::func::wasi_subscription_t;
    using event_t = wasi::func::wasi_event_t;
    inline constexpr auto resolution_call = &wasi::func::clock_res_get;
    inline constexpr auto time_call = &wasi::func::clock_time_get;
    inline constexpr auto random_call = &wasi::func::random_get;
    inline constexpr auto poll_call = &wasi::func::poll_oneoff;
    inline constexpr auto raise_call = &wasi::func::proc_raise;
    inline constexpr auto yield_call = &wasi::func::sched_yield;
#endif
    constexpr pointer_t input{1024u}, output{4096u}, event_count{8192u};
    unsigned checks{}, raise_count{}, yield_count{};

    void require(bool value, char const* reason)
    {
        ++checks;
        if(!value)
        {
            ::fast_io::io::perrln(::fast_io::err(), "WASIp1 authority failure: ", ::fast_io::mnp::os_c_str(reason));
            ::fast_io::fast_terminate();
        }
    }

    errno_t raise_callback(::uwvm2::parser::wasm::standard::wasm1::type::wasm_i32) noexcept
    {
        ++raise_count;
        return errno_t::eagain;
    }
    errno_t yield_callback() noexcept
    {
        ++yield_count;
        return errno_t::eagain;
    }

    // Guest memory is already allocated and single-threaded in this fixture.
    // Direct byte access here deliberately avoids calling another WASI function.
    void reset_output(memory_t& memory)
    {
        ::std::memset(memory.memory_begin + output, 0xa5, 128uz);
        ::std::memset(memory.memory_begin + event_count, 0x5a, 16uz);
    }
    void require_untouched(memory_t& memory)
    {
        ::std::array<unsigned char, 128uz> expected{};
        expected.fill(0xa5);
        require(::std::memcmp(memory.memory_begin + output, expected.data(), expected.size()) == 0, "denial changed output bytes");
        ::std::array<unsigned char, 16uz> expected_count{};
        expected_count.fill(0x5a);
        require(::std::memcmp(memory.memory_begin + event_count, expected_count.data(), expected_count.size()) == 0,
                "denial changed event-count bytes");
    }
    void put_subscriptions(memory_t& memory, subscription_t const* subscriptions, ::std::size_t count)
    {
        ::std::memcpy(memory.memory_begin + input, subscriptions, sizeof(subscription_t) * count);
    }
    subscription_t clock_subscription(wasi_clock_id id, bool absolute = false)
    {
        subscription_t sub{};
        sub.userdata = static_cast<wasi::abi::userdata_t>(0x1122334455667788ull);
        sub.u.tag = eventtype_t::eventtype_clock;
        sub.u.u.clock.id = id;
        sub.u.u.clock.timeout = static_cast<wasi::abi::timestamp_t>(0u);
        sub.u.u.clock.flags = absolute ? wasi::abi::subclockflags_t::subscription_clock_abstime : wasi::abi::subclockflags_t{};
        return sub;
    }
    void require_poll_denied(environment_t& env, memory_t& memory, subscription_t const* subscriptions,
                             ::std::size_t count, errno_t expected = errno_t::enotcapable)
    {
        put_subscriptions(memory, subscriptions, count);
        reset_output(memory);
        require(poll_call(env, input, output, static_cast<wasi_size>(count), event_count) == expected, "unexpected poll denial errno");
        require_untouched(memory);
    }
}

int main()
{
    memory_t memory{};
    memory.init_by_page_count(1uz);
    environment_t env{};
    env.wasip1_memory = ::std::addressof(memory);
    env.wasip1_proc_raise_func_ptr = &raise_callback;
    env.wasip1_sched_yield_func_ptr = &yield_callback;
    require(env.non_fd_permissions == permission_t{}, "default non-FD permissions are nonzero");
    require(!env.permits(permission_t{}), "zero permission was authorized");

    constexpr wasi_clock_id clocks[]{wasi_clock_id::clock_realtime, wasi_clock_id::clock_monotonic,
                                  wasi_clock_id::clock_process_cputime_id, wasi_clock_id::clock_thread_cputime_id};
    // No clock may leak even its resolution under the default policy. The four
    // clocks are independent: granting monotonic below must not grant another ID.
    for(auto id : clocks)
    {
        reset_output(memory);
        require(resolution_call(env, id, output) == errno_t::enotcapable, "default clock resolution was allowed");
        require_untouched(memory);
        require(time_call(env, id, wasi::abi::timestamp_t{}, output) == errno_t::enotcapable, "default clock time was allowed");
        require_untouched(memory);
        for(bool absolute : {false, true})
        {
            auto sub{clock_subscription(id, absolute)};
            require_poll_denied(env, memory, ::std::addressof(sub), 1uz);
        }
    }
    // An invalid ID remains EINVAL rather than being treated as a grant request.
    reset_output(memory);
    require(resolution_call(env, static_cast<wasi_clock_id>(99u), output) == errno_t::einval, "invalid resolution clock ID changed errno");
    require(time_call(env, static_cast<wasi_clock_id>(99u), wasi::abi::timestamp_t{}, output) == errno_t::einval,
            "invalid time clock ID changed errno");
    require_untouched(memory);
    auto invalid_clock{clock_subscription(static_cast<wasi_clock_id>(99u))};
    require_poll_denied(env, memory, ::std::addressof(invalid_clock), 1uz, errno_t::einval);

    // Exercise every independent clock grant against every clock ID. A native
    // platform may lack a granted CPU clock, but only ENOTSUP is accepted for
    // that case; cross-clock calls must always refuse with ENOTCAPABLE.
    constexpr permission_t clock_grants[]{permission_t::clock_realtime, permission_t::clock_monotonic,
                                          permission_t::clock_process_cputime, permission_t::clock_thread_cputime};
    for(::std::size_t grant_index{}; grant_index != 4uz; ++grant_index)
    {
        env.non_fd_permissions = clock_grants[grant_index];
        for(::std::size_t clock_index{}; clock_index != 4uz; ++clock_index)
        {
            reset_output(memory);
            auto const resolution_status{resolution_call(env, clocks[clock_index], output)};
            if(grant_index == clock_index)
            {
                require(resolution_status == errno_t::esuccess || resolution_status == errno_t::enotsup,
                        "matching clock resolution grant was denied");
            }
            else
            {
                require(resolution_status == errno_t::enotcapable, "clock grant crossed into another resolution");
                require_untouched(memory);
            }
            reset_output(memory);
            auto const time_status{time_call(env, clocks[clock_index], wasi::abi::timestamp_t{}, output)};
            if(grant_index == clock_index)
            {
                require(time_status == errno_t::esuccess || time_status == errno_t::enotsup, "matching clock time grant was denied");
            }
            else
            {
                require(time_status == errno_t::enotcapable, "clock grant crossed into another time source");
                require_untouched(memory);
            }
        }
    }
    env.non_fd_permissions = permission_t{};

    for(wasi_size length : {wasi_size{}, static_cast<wasi_size>(64u)})
    {
        reset_output(memory);
        require(random_call(env, output, length) == errno_t::enotcapable, "default random request was allowed");
        require_untouched(memory);
    }
    require(raise_call(env, wasi::abi::signal_t::sigterm) == errno_t::enotcapable, "default raise wrapper was allowed");
    require(wasi::func::proc_raise_impl(env, wasi::abi::signal_t::sigterm) == errno_t::enotcapable, "raise implementation bypassed policy");
    require(yield_call(env) == errno_t::enotcapable, "default yield wrapper was allowed");
    require(wasi::func::sched_yield_impl(env) == errno_t::enotcapable, "yield implementation bypassed policy");
    require(raise_count == 0u && yield_count == 0u, "denied callback was invoked");

    env.non_fd_permissions = permission_t::clock_monotonic;
    require(resolution_call(env, wasi_clock_id::clock_monotonic, output) == errno_t::esuccess, "explicit monotonic resolution grant failed");
    require(time_call(env, wasi_clock_id::clock_monotonic, wasi::abi::timestamp_t{}, output) == errno_t::esuccess,
            "explicit monotonic time grant failed");
    for(auto id : clocks)
    {
        if(id == wasi_clock_id::clock_monotonic) { continue; }
        reset_output(memory);
        require(resolution_call(env, id, output) == errno_t::enotcapable, "monotonic grant leaked another resolution");
        require(time_call(env, id, wasi::abi::timestamp_t{}, output) == errno_t::enotcapable, "monotonic grant leaked another clock");
        require_untouched(memory);
    }
    reset_output(memory);
    require(random_call(env, output, static_cast<wasi_size>(64u)) == errno_t::enotcapable, "clock grant leaked randomness");
    require_untouched(memory);

    // Preflight must cover later entries too, including after an otherwise
    // authorized clock or an FD that would produce an immediate EBADF event.
    ::std::array<subscription_t, 2uz> mixed{clock_subscription(wasi_clock_id::clock_monotonic), clock_subscription(wasi_clock_id::clock_realtime)};
    require_poll_denied(env, memory, mixed.data(), mixed.size());
    ::std::swap(mixed[0uz], mixed[1uz]);
    require_poll_denied(env, memory, mixed.data(), mixed.size());
    mixed[0uz] = subscription_t{};
    mixed[0uz].u.tag = eventtype_t::eventtype_fd_read;
    mixed[0uz].u.u.fd_readwrite.file_descriptor = static_cast<wasi::abi::fd_t>(12345u);
    mixed[1uz] = clock_subscription(wasi_clock_id::clock_realtime);
    require_poll_denied(env, memory, mixed.data(), mixed.size());
    mixed[0uz] = clock_subscription(wasi_clock_id::clock_monotonic);
    mixed[1uz] = invalid_clock;
    require_poll_denied(env, memory, mixed.data(), mixed.size(), errno_t::einval);

    auto authorized_clock{clock_subscription(wasi_clock_id::clock_monotonic)};
    put_subscriptions(memory, ::std::addressof(authorized_clock), 1uz);
    require(poll_call(env, input, output, static_cast<wasi_size>(1u), event_count) == errno_t::esuccess, "explicit poll clock grant failed");
    wasi_size observed_count{};
    ::std::memcpy(::std::addressof(observed_count), memory.memory_begin + event_count, sizeof(observed_count));
    require(observed_count == static_cast<wasi_size>(1u), "authorized zero-duration clock did not produce one event");

    // These descriptors deliberately have no native backing. Capability refusal
    // must happen before platform code tries to inspect or register that backing.
    // POLL alone and POLL plus the opposite direction are both insufficient.
    env.fd_storage.opens.resize(4uz);
    auto& fd{*env.fd_storage.opens.index_unchecked(3uz).fd_p};
    for(auto tag : {eventtype_t::eventtype_fd_read, eventtype_t::eventtype_fd_write})
    {
        for(rights_t rights : {rights_t::right_poll_fd_readwrite,
                              rights_t::right_poll_fd_readwrite | (tag == eventtype_t::eventtype_fd_read ? rights_t::right_fd_write : rights_t::right_fd_read),
                              tag == eventtype_t::eventtype_fd_read ? rights_t::right_fd_read : rights_t::right_fd_write})
        {
            fd.rights_base = rights;
            subscription_t sub{};
            sub.u.tag = tag;
            sub.u.u.fd_readwrite.file_descriptor = static_cast<wasi::abi::fd_t>(3u);
            put_subscriptions(memory, ::std::addressof(sub), 1uz);
            require(poll_call(env, input, output, static_cast<wasi_size>(1u), event_count) == errno_t::esuccess, "FD denial should be an event");
            event_t event{};
            ::std::memcpy(::std::addressof(event), memory.memory_begin + output, sizeof(event));
            require(event.type == tag && event.error == errno_t::enotcapable, "FD readiness exposed an unauthorized direction");
        }
    }

    env.non_fd_permissions = permission_t::proc_raise;
    require(raise_call(env, wasi::abi::signal_t::sigterm) == errno_t::eagain && raise_count == 1u, "explicit raise grant did not reach callback");
    require(yield_call(env) == errno_t::enotcapable && yield_count == 0u, "raise grant leaked yield authority");
    env.non_fd_permissions = permission_t::sched_yield;
    require(yield_call(env) == errno_t::eagain && yield_count == 1u, "explicit yield grant did not reach callback");
    require(raise_call(env, wasi::abi::signal_t::sigterm) == errno_t::enotcapable && raise_count == 1u, "yield grant leaked raise authority");
    env.non_fd_permissions = permission_t::random;
    require(random_call(env, output, static_cast<wasi_size>(64u)) == errno_t::esuccess, "explicit random grant failed");
    require(time_call(env, wasi_clock_id::clock_monotonic, wasi::abi::timestamp_t{}, output) == errno_t::enotcapable,
            "random grant leaked clock authority");

    environment_t isolated{};
    isolated.wasip1_memory = ::std::addressof(memory);
    isolated.wasip1_proc_raise_func_ptr = &raise_callback;
    isolated.wasip1_sched_yield_func_ptr = &yield_callback;
    reset_output(memory);
    require(random_call(isolated, output, static_cast<wasi_size>(64u)) == errno_t::enotcapable, "grant leaked to another environment");
    require(time_call(isolated, wasi_clock_id::clock_monotonic, wasi::abi::timestamp_t{}, output) == errno_t::enotcapable,
            "clock grant leaked to another environment");
    require_untouched(memory);
    ::fast_io::io::println(::fast_io::out(), "WASIp1 authority width=", WASIP1_DEFAULT_DENY_WIDTH, " checks=", checks, " PASS");
}
