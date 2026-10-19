// SPDX-License-Identifier: Apache-2.0
#pragma once
#if UWVM2_HAS_THREADED_PROFILE_HOST_API
#ifndef UWVM_MODULE
#include <array>
#include <atomic>
#include <cerrno>
#include <chrono>
#include <cstdint>
#include <memory>
#include <thread>
#include <signal.h>
#include <fcntl.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fast_io.h>
#include <uwvm2/runtime/lib/uwvm_runtime.h>
#include <uwvm2/runtime/lib/uwvm_runtime_posix_abi.h>
#include <uwvm2/uwvm/cmdline/params/wasi_thread_signals.h>
#endif
namespace uwvm2::uwvm::run::threaded_signal_cli
{
namespace runtime=::uwvm2::runtime::lib;
namespace posix=runtime::posix_abi;
using clock=::std::chrono::steady_clock;
using policy=::uwvm2::uwvm::cmdline::params::details::wasi_thread_signal_policy;
static_assert(clock::is_steady&&sizeof(clock::rep)>=8u);
static_assert(::std::atomic<unsigned>::is_always_lock_free);
// Process-lived POD, never a Source/controller pointer. One installation attempt
// per standalone process; this mailbox is never reused for a replacement Source.
inline ::std::atomic<unsigned> reservation{},active{},mailbox{},process_id{},owner_uid{};
inline void default_signal(int number) noexcept
{
    auto const saved=errno;struct ::sigaction reset{};reset.sa_handler=SIG_DFL;
    if(posix::sigemptyset_noexcept(&reset.sa_mask)!=0||posix::sigaction_noexcept(number,&reset,nullptr)!=0)
        posix::_exit_noexcept(128+number);
    if(posix::raise_noexcept(number)!=0)posix::_exit_noexcept(128+number);
    errno=saved;
}
inline void notify(int number,::siginfo_t*info,void*) noexcept
{
    auto const saved=errno;
    if((number!=SIGINT&&number!=SIGTERM)||!info||active.load(::std::memory_order_acquire)!=1u)
    {default_signal(number);errno=saved;return;}
    bool kernel{};
#ifdef SI_KERNEL
    kernel=number==SIGINT&&info->si_code==SI_KERNEL&&info->si_pid==0;
#endif
    bool const external=info->si_pid>0&&static_cast<unsigned>(info->si_pid)!=process_id.load(::std::memory_order_relaxed)&&
        static_cast<unsigned>(info->si_uid)==owner_uid.load(::std::memory_order_relaxed);
    if(!kernel&&!external){default_signal(number);errno=saved;return;}
    auto const bit=number==SIGINT?1u:2u;
    auto const old=mailbox.fetch_or(bit,::std::memory_order_relaxed);
    // Two pending bits saturate. Duplicate notifications latch bit4; do not
    // invent an exact OS signal count/order from standard coalescing signals.
    if(old&bit)mailbox.fetch_or(4u,::std::memory_order_relaxed);
    errno=saved;
}
[[nodiscard]] inline bool deadline_after(clock::time_point now,::std::uint32_t ms,clock::time_point&out) noexcept
{
    if(!ms||ms>60000u)return false;
    auto const delta=::std::chrono::duration_cast<clock::duration>(::std::chrono::milliseconds{ms});
    if(delta<=clock::duration::zero()||now>clock::time_point::max()-delta)return false;
    out=now+delta;return true;
}
[[nodiscard]] inline bool complete(runtime::threaded_retirement const&r) noexcept
{return r.physically_joined&&r.native_error==0&&r.preparing==0u&&r.native_pending==0u&&
    r.compiler_tickets_pending==0u&&r.native_cleanup_pending==0u;}
struct diagnostics
{
    int mask_query{},mask_member_error{};
    bool mask_queried{},int_blocked{},term_blocked{};
    int first_error{},last_error{},query_int{},query_term{},install_int{},install_term{},restore_int{},restore_term{},last_push{};
    ::std::uint64_t observed_batches{},delivered_frames{};
    unsigned pending{};
    bool queried_int{},queried_term{},attempted_int{},attempted_term{},restored_int{},restored_term{};
    bool saturated{},forcing{},native_joined{},source_retired{},handlers_restored{};
};
class controller final
{
    policy selected_{};diagnostics facts_{};
    ::std::array<struct ::sigaction,2> previous_{};::std::array<bool,2> installed_{};
    ::std::shared_ptr<runtime::threaded_signal_sender const> sender_;
    ::std::thread::id native_thread_{};
    clock::time_point grace_{clock::time_point::max()},retire_{clock::time_point::max()};
    bool claimed_{},finalized_{},may_be_live_{},grace_started_{};
    void note(int error) noexcept{if(error){if(!facts_.first_error)facts_.first_error=error;facts_.last_error=error;}}
    [[nodiscard]] bool local() const noexcept{return native_thread_==::std::this_thread::get_id();}
    void force(clock::time_point now) noexcept
    {if(!facts_.forcing){facts_.forcing=true;if(!deadline_after(now,selected_.retire_ms,retire_))contain("retirement deadline overflow");}}
    void collect(clock::time_point now) noexcept
    {
        auto const value=mailbox.exchange(0u,::std::memory_order_acquire);if(!value)return;
        if(facts_.observed_batches==UINT64_MAX){note(EOVERFLOW);force(now);return;}
        ++facts_.observed_batches;facts_.saturated=facts_.saturated||(value&4u)!=0u;facts_.pending|=value&3u;
        if(facts_.pending&&!grace_started_)
        {grace_started_=true;if(!deadline_after(now,selected_.grace_ms,grace_))contain("grace deadline overflow");}
    }
    [[nodiscard]] int restore() noexcept
    {
        active.store(0u,::std::memory_order_release);mailbox.exchange(0u,::std::memory_order_acquire);facts_.pending=0u;
        int first{};
        for(unsigned i=2u;i--!=0u;)
        {
            if(!installed_[i])continue;auto const number=i?SIGTERM:SIGINT;struct ::sigaction actual{};int error{};
            if(posix::sigaction_noexcept(number,nullptr,&actual)!=0)error=errno;
            else if((actual.sa_flags&SA_SIGINFO)==0||actual.sa_sigaction!=&notify)error=EBUSY;
            else if(posix::sigaction_noexcept(number,&previous_[i],nullptr)!=0)error=errno;
            else{installed_[i]=false;(i?facts_.restored_term:facts_.restored_int)=true;}
            (i?facts_.restore_term:facts_.restore_int)=error;note(error);if(!first&&error)first=error;
        }
        facts_.handlers_restored=!installed_[0]&&!installed_[1];return first;
    }
public:
    controller() noexcept=default;
    controller(controller const&)=delete;controller&operator=(controller const&)=delete;
    [[nodiscard]] bool selected() const noexcept{return selected_.selected();}
    [[nodiscard]] diagnostics const&report() const noexcept{return facts_;}
    [[nodiscard]] bool prepare(policy const&input) noexcept
    {
        if(claimed_||finalized_)return false;selected_=input;if(!selected())return true;
        auto const&p=selected_.channel;
        if(p.read_fd<3u||p.read_fd>INT32_MAX||!p.cookie||!p.maximum_frames||p.maximum_frames>1048576u||
           p.maximum_pipe_bytes<32u||p.maximum_pipe_bytes>1048576u||!selected_.grace_ms||selected_.grace_ms>60000u||
           !selected_.retire_ms||selected_.retire_ms>60000u){note(EINVAL);return false;}
        // Identity is captured only for an actually selected installer. The
        // ordinary default-OFF run performs no Native thread-ID read.
        native_thread_=::std::this_thread::get_id();
        unsigned expected{};
        if(!reservation.compare_exchange_strong(expected,1u,::std::memory_order_acq_rel)){note(EBUSY);return false;}
        claimed_=true;
        // Do not silently change an inherited signal mask. A blocked ordinary
        // collector could otherwise wait forever without ever seeing a signal.
        ::sigset_t current_mask{};facts_.mask_queried=true;
        facts_.mask_query=::pthread_sigmask(SIG_BLOCK,nullptr,&current_mask);
        if(facts_.mask_query){note(facts_.mask_query);finalized_=true;return false;}
        auto const masked_int=::sigismember(&current_mask,SIGINT),masked_term=::sigismember(&current_mask,SIGTERM);
        if(masked_int<0||masked_term<0){facts_.mask_member_error=errno;note(errno);finalized_=true;return false;}
        facts_.int_blocked=masked_int!=0;facts_.term_blocked=masked_term!=0;
        if(facts_.int_blocked||facts_.term_blocked){note(EBUSY);finalized_=true;return false;}
        for(unsigned i{};i<2u;++i)
        {
            (i?facts_.queried_term:facts_.queried_int)=true;
            if(posix::sigaction_noexcept(i?SIGTERM:SIGINT,nullptr,&previous_[i])!=0)
            {auto const error=errno;(i?facts_.query_term:facts_.query_int)=error;note(error);finalized_=true;return false;}
        }
        if(previous_[0].sa_handler!=SIG_DFL||previous_[1].sa_handler!=SIG_DFL)
        {note(EBUSY);finalized_=true;return false;}
        struct ::sigaction action{};action.sa_sigaction=&notify;action.sa_flags=SA_SIGINFO|SA_RESTART;
        if(posix::sigemptyset_noexcept(&action.sa_mask)!=0||::sigaddset(&action.sa_mask,SIGINT)!=0||::sigaddset(&action.sa_mask,SIGTERM)!=0)
        {note(errno);finalized_=true;return false;}
        auto const pid=posix::getpid_noexcept();if(pid<=0){note(EINVAL);finalized_=true;return false;}
        process_id.store(static_cast<unsigned>(pid),::std::memory_order_relaxed);
        owner_uid.store(static_cast<unsigned>(::geteuid()),::std::memory_order_relaxed);
        mailbox.store(0u,::std::memory_order_relaxed);active.store(1u,::std::memory_order_release);
        for(unsigned i{};i<2u;++i)
        {
            (i?facts_.attempted_term:facts_.attempted_int)=true;
            if(posix::sigaction_noexcept(i?SIGTERM:SIGINT,&action,nullptr)!=0)
            {
                auto const error=errno;(i?facts_.install_term:facts_.install_int)=error;note(error);
                if(restore()!=0)contain("partial signal installation rollback failed");
                finalized_=true;return false;
            }
            installed_[i]=true;
        }
        return true;
    }
    [[nodiscard]] int pending_before_launch() noexcept
    {
        if(!selected())return 0;if(!local())contain("foreign CLI thread");collect(clock::now());
        return facts_.pending?(facts_.pending&2u?128+SIGTERM:128+SIGINT):0;
    }
    void launch_attempted() noexcept{if(selected())may_be_live_=true;}
    void fail_native(int error) noexcept{note(error);force(clock::now());}
    void tick() noexcept
    {
        if(!selected()||facts_.native_joined)return;if(!local())contain("foreign CLI thread");
        auto const now=clock::now();collect(now);if(grace_started_&&now>=grace_)force(now);
        if(facts_.forcing||!facts_.pending)return;
        if(!sender_)sender_=runtime::runtime_threaded_profile_signal_sender_host_api();
        if(!sender_)return; // no forged sender during real preparation/retirement
        auto const bit=(facts_.pending&2u)?2u:1u;
        auto const result=runtime::runtime_threaded_profile_signal_push_host_api(*sender_,bit==2u?
            runtime::threaded_signal_event::terminate:runtime::threaded_signal_event::interrupt);
        facts_.last_push=result;
        if(!result){facts_.pending&=~bit;if(facts_.delivered_frames==UINT64_MAX){note(EOVERFLOW);force(now);}else ++facts_.delivered_frames;}
        else if(result!=EAGAIN&&result!=EBUSY){note(result);force(now);}
    }
    [[nodiscard]] clock::time_point next_deadline() noexcept
    {
        clock::time_point result{};
        if(!deadline_after(clock::now(),100u,result))contain("wait-slice deadline overflow");
        if(facts_.forcing){if(retire_<result)result=retire_;}
        else if(grace_started_&&grace_<result)result=grace_;
        return result;
    }
    [[nodiscard]] runtime::threaded_retirement wait_next() noexcept
    {
        tick();auto const deadline=next_deadline();
        auto actual=facts_.forcing?runtime::runtime_threaded_profile_retire_host_api(deadline,runtime::threaded_management_kind::stop_only):
            runtime::runtime_threaded_profile_wait_host_api(deadline);
        if(complete(actual))
        {facts_.native_joined=true;active.store(0u,::std::memory_order_release);mailbox.exchange(0u,::std::memory_order_acquire);facts_.pending=0u;}
        else if(facts_.forcing&&clock::now()>=retire_)contain("physical retirement deadline expired; join remains pending");
        return actual;
    }
    [[nodiscard]] clock::time_point source_deadline() noexcept
    {clock::time_point out{};if(!deadline_after(clock::now(),selected_.retire_ms,out))contain("Source deadline overflow");return out;}
    void complete_source_retirement() noexcept
    {
        if(!selected()||finalized_)return;
        if(!local()||(may_be_live_&&!facts_.native_joined))contain("Source return before actual Native physical join");
        ::std::uint32_t code{};unsigned kind{};
        if(runtime::runtime_threaded_profile_result_host_api({},&code,&kind)!=runtime::threaded_main_status::disabled)
            contain("actual Runtime Source profile is still retained");
        facts_.source_retired=true;may_be_live_=false;
        if(restore()!=0)contain("signal disposition restoration failed");sender_.reset();finalized_=true;
    }
    [[noreturn]] void contain(char const*reason) noexcept
    {
        active.store(0u,::std::memory_order_release);
        // Explicit process-containment phase only. Make Root's stderr OFD
        // nonblocking before best-effort diagnostics; no endless buffered IO can
        // replace _exit. Regular files are skipped because O_NONBLOCK does not
        // bound their write latency. This terminal change is never normal-path.
        ::sigset_t pipe{};struct ::stat output_state{};bool writable{};
        if(::fstat(STDERR_FILENO,&output_state)==0&&(S_ISFIFO(output_state.st_mode)||S_ISSOCK(output_state.st_mode))&&
           posix::sigemptyset_noexcept(&pipe)==0&&::sigaddset(&pipe,SIGPIPE)==0&&::pthread_sigmask(SIG_BLOCK,&pipe,nullptr)==0)
        {auto const flags=::fcntl(STDERR_FILENO,F_GETFL);writable=flags>=0&&::fcntl(STDERR_FILENO,F_SETFL,flags|O_NONBLOCK)==0;}
        if(writable)try
        {::fast_io::io::perr(::fast_io::u8err(),u8"uwvm: [error] Forced threaded-signal CLI process termination125; pending physical/Source retirement is NOT successful join. reason=",
            ::fast_io::mnp::code_cvt(::fast_io::mnp::os_c_str(reason)),u8" native_joined=",facts_.native_joined,u8" source_retired=",facts_.source_retired,
            u8" first_error=",facts_.first_error,u8" last_error=",facts_.last_error,u8" last_push=",facts_.last_push,
            u8" mask_query=",facts_.mask_query,u8" pending=",facts_.pending,u8" saturated=",facts_.saturated,u8" restore_int=",facts_.restore_int,u8" restore_term=",facts_.restore_term,u8"\n");}
        catch(...){ /* broken/full stderr cannot delay actual process exit */ }
        posix::_exit_noexcept(125); // OS containment, never a fake join/cleanup
    }
    ~controller()
    {
        if(!selected()||finalized_)return;
        if(may_be_live_)contain("CLI scope unwound with Native/Source owners retained");
        if(claimed_&&restore()!=0)contain("prelaunch signal rollback failed");
    }
};
}
#endif
