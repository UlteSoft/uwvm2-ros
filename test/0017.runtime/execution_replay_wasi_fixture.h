// Actual single-module policy setup for native concurrent WASI replay tests.
#pragma once
#include <cerrno>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <fcntl.h>
#include <unistd.h>
static bool replay_wasi_fixture{};
static bool replay_wasi_io_fixture{};
static bool replay_wasi_scalar_fixture{};
static ::std::shared_ptr<::uwvm2::imported::wasi::wasip1::environment::budget_ledger> replay_wasi_scalar_budget{};
static auto replay_wasi_random_budget_calls()
{
    if(!replay_wasi_scalar_budget)return ::std::uint64_t{};
    ::std::lock_guard lock{replay_wasi_scalar_budget->mutex};
    return replay_wasi_scalar_budget->counters[static_cast<unsigned>(::uwvm2::imported::wasi::wasip1::environment::budget_kind::random)].total_calls;
}
static bool replay_wasi_socket_fixture{};
static int replay_wasi_input_writer{-1};
static ::std::thread replay_wasi_input_thread{};
static ::std::atomic<bool> replay_wasi_input_stop{};
static ::std::atomic<bool> replay_wasi_input_drain{};
static void start_replay_wasi_input()
{
    if(!replay_wasi_io_fixture)return;
    replay_wasi_input_thread=::std::thread([]
    {
        unsigned char byte{1};::std::mutex mutex;::std::condition_variable changed;
        ::std::uint64_t next_instruction{};bool recording{};
        while(!replay_wasi_input_stop.load(::std::memory_order_acquire))
        {
            bool supply=true;
            if(::std::getenv("UWVM_TEST_IMPORT_PAUSE"))
            {
                auto journal=::uwvm2::runtime::lib::llvm_jit_checkpoint_replay_status_host_api();
                if(journal.mode==1u)
                {
                    // A native/QEMU scheduling delay is not proof of an open
                    // import interval. Let the other worker execute real Wasm
                    // instructions before supplying the next external byte.
                    if(!recording){recording=true;next_instruction=journal.instructions+128u;}
                    int queued{};REQUIRE(::ioctl(STDIN_FILENO,FIONREAD,&queued)==0);
                    // A complete stop must let a real outstanding read return.
                    // Its peer is already parked and can no longer advance the
                    // instruction counter used to establish the test interval.
                    supply=queued==0 && (journal.instructions>=next_instruction ||
                        replay_wasi_input_drain.load(::std::memory_order_acquire));
                    if(supply)next_instruction=journal.instructions+128u;
                }
            }
            if(supply)
            {
                auto written=::write(replay_wasi_input_writer,&byte,1u);
                if(written==1)++byte;
                else REQUIRE(written<0 && (errno==EAGAIN || errno==EINTR));
            }
            ::std::unique_lock lock{mutex};changed.wait_for(lock,::std::chrono::milliseconds{1});
        }
    });
}
static void stop_replay_wasi_input()
{
    if(!replay_wasi_io_fixture)return;
    replay_wasi_input_stop.store(true,::std::memory_order_release);replay_wasi_input_thread.join();
    REQUIRE(::close(replay_wasi_input_writer)==0);replay_wasi_input_writer=-1;
}
static ::std::pair<::std::uint64_t,unsigned> replay_wasi_audit_counts()
{
    if(!replay_wasi_io_fixture)return {};
    struct ::stat state{};REQUIRE(::fstat(STDOUT_FILENO,&state)==0 && S_ISREG(state.st_mode));
    int readable{};REQUIRE(::ioctl(STDIN_FILENO,FIONREAD,&readable)==0 && readable>=0);
    return {static_cast<::std::uint64_t>(state.st_size),static_cast<unsigned>(readable)};
}
static void configure_replay_wasi_fixture()
{
    replay_wasi_scalar_fixture=::std::getenv("UWVM_TEST_WASI_SCALAR")!=nullptr;
    replay_wasi_socket_fixture=::std::getenv("UWVM_TEST_WASI_SOCKET")!=nullptr;
    replay_wasi_io_fixture=::std::getenv("UWVM_TEST_WASI_IO")!=nullptr || replay_wasi_socket_fixture;
    if(!::std::getenv("UWVM_TEST_WASI_POLL") && !replay_wasi_io_fixture && !replay_wasi_scalar_fixture)return;
    namespace ws=::uwvm2::uwvm::imported::wasi::wasip1::storage;
    namespace api=::uwvm2::utils::control::host_interfaces;
    using permission=::uwvm2::imported::wasi::wasip1::environment::permission_t;
    REQUIRE(ws::try_create_targetless_wasip1_module_override(u8"checkpoint-instance"));
    auto* policy=ws::find_targetless_wasip1_module_override(u8"checkpoint-instance");
    REQUIRE(policy);policy->enabled=true;policy->enabled_is_set=true;
    policy->non_fd_permissions=permission::clock_monotonic|permission::monotonic_timer;
    if(replay_wasi_io_fixture)policy->non_fd_permissions=policy->non_fd_permissions|permission::stdin_read|permission::stdout_write;
    if(::std::getenv("UWVM_TEST_WASI_METADATA"))policy->non_fd_permissions=policy->non_fd_permissions|permission::stdio_metadata;
    if(replay_wasi_scalar_fixture)
    {
        policy->non_fd_permissions=policy->non_fd_permissions|permission::args|permission::environ|permission::random|permission::sched_yield|permission::monotonic_resolution;
        policy->force_argument_storage.emplace_back(u8"replay-argument");
        policy->add_or_replace_environment.emplace(u8"REPLAY",u8"environment");
        replay_wasi_scalar_budget=policy->env.limits.accounting;
        if(::std::getenv("UWVM_TEST_WASI_QUOTA"))
        {
            namespace budget=::uwvm2::imported::wasi::wasip1::environment;
            auto& limit=policy->env.limits.operations[static_cast<unsigned>(budget::budget_kind::random)];
            limit.window_calls=4u;limit.actions[2u]=budget::limit_action::wait;
        }
        for(auto name:{"args_get","args_get_wasm64","args_sizes_get","args_sizes_get_wasm64",
            "environ_get","environ_get_wasm64","environ_sizes_get","environ_sizes_get_wasm64",
            "random_get","random_get_wasm64","clock_time_get","clock_time_get_wasm64",
            "clock_res_get","clock_res_get_wasm64","sched_yield","sched_yield_wasm64"})
            REQUIRE(policy->interfaces.set(api::find(api::family::wasip1,::std::string_view{name}),api::access::allow));
    }
    policy->permission_presence=policy->non_fd_permissions;
    for(auto name:{"poll_oneoff","poll_oneoff_wasm64"})
        REQUIRE(policy->interfaces.set(api::find(api::family::wasip1,::std::string_view{name}),api::access::allow));
    if(replay_wasi_io_fixture)
    {
        for(auto name:{"fd_read","fd_read_wasm64","fd_write","fd_write_wasm64","fd_fdstat_get","fd_fdstat_get_wasm64"})
            REQUIRE(policy->interfaces.set(api::find(api::family::wasip1,::std::string_view{name}),api::access::allow));
        int pipe[2]{};
        if(replay_wasi_socket_fixture)
        {
            REQUIRE(::socketpair(AF_UNIX,SOCK_STREAM,0,pipe)==0);
            using rights=::uwvm2::imported::wasi::wasip1::abi::rights_t;
            policy->env.socket_rights.push_back({0,rights::right_fd_read,{}});
            for(auto name:{"sock_recv","sock_recv_wasm64"})
                REQUIRE(policy->interfaces.set(api::find(api::family::wasip1,::std::string_view{name}),api::access::allow));
        }
        else REQUIRE(::pipe(pipe)==0);
        REQUIRE(::dup2(pipe[0],STDIN_FILENO)==STDIN_FILENO);
        if(pipe[0]!=STDIN_FILENO)REQUIRE(::close(pipe[0])==0);
        replay_wasi_input_writer=pipe[1];auto flags=::fcntl(pipe[1],F_GETFL);REQUIRE(flags>=0);
        REQUIRE(::fcntl(pipe[1],F_SETFL,flags|O_NONBLOCK)==0);
    }
    policy->noinherit_system_environment=true;policy->noinherit_system_environment_is_set=true;
    policy->force_args_is_set=true;replay_wasi_fixture=true;
}
