// Test-only historical process owner. Never include or link into the VM product.
/** @file
 * @brief Prepared, isolated CPU inference service with authority-fenced bounded IPC.
 * @details Deployment owns this service until retire() joins its supervisor.
 * No guest operation spawns a process. Runtime loss fails the binding closed;
 * a deployment must explicitly prepare a fresh generation to recover. The
 * digest-approved synchronous backend is TCB and must not retain invocation
 * bytes after returning. Arbitrary third-party plugins are not accepted.
 */
#pragma once
#include "model_sandbox.h"
#include <uwvm2/imported/ros/scene/supervisor_stack.h>
#include <uwvm2/imported/ros/scene/model_worker_protocol.h>
#include <uwvm2/imported/ros/scene/model_dispatch.h>
#include <uwvm2/imported/ros/scene/model_provider.h>
#include <uwvm2/imported/ros/scene/buffer_contract.h>
#if defined(__linux__)
# include <pthread.h>
# include <sys/mman.h>
#endif

namespace uwvm2::imported::ros::scene::model_worker
{
#if defined(__linux__)
    /** @brief One serial native CPU worker; finite model queues remain in model_dispatch.
     * @details Every input copy, result publication and cancellation check uses
     * the authority-aware facade. Inference and pipe polling never hold its
     * locks. Nonblocking request_stop is safe under an authority lease; retire
     * must run outside that lease and before dispatcher destruction.
     */
    class service final : public models::provider_lifetime
    {
        model_dispatch& dispatcher;
        ::uwvm2::utils::container::std_string alias;
        std::uint64_t generation{};
        model_sandbox::process child;
        supervisor_stack stack;
        pthread_t thread{};
        ::pid_t owner{::getpid()};
        bool started{}, joined{};
        std::atomic<bool> stopped{};
        std::atomic<error> startup{error::again};
        std::mutex retirement_mutex;
        std::uint64_t stderr_seen{};

        bool same_process() const noexcept { return owner==::getpid(); }
        static std::uint64_t now() noexcept { return static_cast<std::uint64_t>(resources::steady_now()); }
        bool cancelled(model_dispatch::work const* work,std::uint64_t deadline) const
        { return stopped.load(std::memory_order_acquire) || now()>=deadline || (work && work->cancellation_requested()); }
        bool drain_errors() noexcept
        {
            std::array<std::uint8_t,1024> scratch{};
            for(unsigned batch{};batch<8;++batch)
            {
                auto read=::read(child.stderr_fd(),scratch.data(),scratch.size());
                if(read>0)
                {
                    auto count=static_cast<std::uint64_t>(read);
                    if(stderr_seen>child.reservations().stderr_bytes || count>child.reservations().stderr_bytes-stderr_seen) { return false; }
                    stderr_seen+=count;continue;
                }
                if(!read || errno==EAGAIN) { return true; }
                if(errno!=EINTR) { return false; }
            }
            return true;
        }
        bool transfer(int fd,std::span<std::uint8_t> bytes,bool writing,std::uint64_t deadline,model_dispatch::work const* work)
        {
            std::size_t offset{};
            while(offset<bytes.size())
            {
                if(cancelled(work,deadline) || !drain_errors()) { return false; }
                auto count=writing?::write(fd,bytes.data()+offset,bytes.size()-offset): ::read(fd,bytes.data()+offset,bytes.size()-offset);
                if(count>0) { offset+=static_cast<std::size_t>(count);continue; }
                if(!count || (errno!=EINTR && errno!=EAGAIN)) { return false; }
                pollfd waiting{fd,static_cast<short>(writing?POLLOUT:POLLIN),0};
                auto milliseconds=static_cast<int>((child.reservations().poll_ns+999999)/1000000);
                if(::poll(&waiting,1,milliseconds)<0 && errno!=EINTR) { return false; }
            }
            return !cancelled(work,deadline);
        }
        bool marker(std::string_view expected,std::uint64_t deadline)
        {
            std::array<std::uint8_t,64> bytes{};
            return expected.size()<=bytes.size() && transfer(child.output_fd(),{bytes.data(),expected.size()},false,deadline,nullptr) &&
                   std::string_view{reinterpret_cast<char const*>(bytes.data()),expected.size()}==expected;
        }
        error begin()
        {
            auto status=child.launch();if(status!=error::success) { return status; }
            auto deadline=now()+child.reservations().startup_ns;
            do
            {
                if(cancelled(nullptr,deadline) || !drain_errors()) { return error::cancelled; }
                status=child.identify();
                if(status==error::again) { ::poll(nullptr,0,1); }
            }while(status==error::again);
            if(status!=error::success || !marker(ready,deadline)) { return error::io; }
            do
            {
                if(cancelled(nullptr,deadline)) { return error::cancelled; }
                status=child.enable_weights();
                if(status==error::again) { ::poll(nullptr,0,1); }
            }while(status==error::again);
            if(status!=error::success || !marker(attached,deadline)) { return error::io; }
            return dispatcher.attach(alias,generation);
        }
        /** @brief Confirm both actual namespace init and launcher have exited. */
        bool cleanup_process() noexcept
        {
            child.request_stop();
            int status{};
            for(;;)
            {
                auto result=child.reap(status);
                if(result==error::success || result==error::bad_handle) { return child.cleanup_confirmed(); }
                if(result!=error::again) { return false; } // no fabricated cleanup; caller retains unknown outcome
                pollfd process{child.process_fd(),POLLIN,0};::poll(&process,1,10);
            }
        }
        bool encode(model_dispatch::work& work,wire::writer& destination,std::size_t count)
        {
            if(!destination.u64(work.identity())) { return false; }
            auto status=work.with_parameters([&](wire::bytes bytes) { return destination.blob(bytes); });
            if(status!=error::success || !destination.u64(count)) { return false; }
            for(std::size_t i{};i<count;++i)
            {
                status=work.with_input(i,[&](wire::bytes bytes,buffers::layout const& layout,buffers::digest const&)
                {
                    std::array<std::uint8_t,512> metadata{};wire::writer writer{metadata};
                    return buffer_contract::encode_layout(writer,layout) && destination.blob(writer.written()) && destination.blob(bytes);
                });
                if(status!=error::success) { return false; }
            }
            return destination.good();
        }
        /** @brief Copy under authority, compute without locks, publish only current results. */
        bool execute(model_dispatch::work& work)
        {
            bool disclosed{}, terminal{};
            try
            {
                std::size_t count{};std::uint64_t deadline{};
                if(work.metadata(count,deadline)!=error::success) { work.finish(models::state::cancelled);return true; }
                auto measured=wire::writer::measuring(static_cast<std::size_t>(child.reservations().request_bytes));
                if(!encode(work,measured,count)) { work.finish(models::state::failed);return true; }
                auto reserve=measured.size()+child.reservations().response_bytes;
                models::registry::scratch_lease capacity;
                if(work.reserve_scratch(reserve,capacity)!=error::success) { work.finish(models::state::failed);return true; }
                byte_buffer storage;storage.resize_for_overwrite(static_cast<std::size_t>(reserve));
                auto request=storage.writable().first(measured.size());
                auto response_storage=storage.writable().subspan(measured.size());
                wire::writer writer{request};
                if(!encode(work,writer,count) || writer.size()!=measured.size()) { work.finish(models::state::cancelled);return true; }
                std::array<std::uint8_t,8> header{};wire::writer length{header};
                if(!length.u64(request.size())) { work.finish(models::state::failed);return true; }
                disclosed=true;
                if(!transfer(child.input_fd(),header,true,deadline,&work) || !transfer(child.input_fd(),request,true,deadline,&work)) { throw error::io; }
                for(;;)
                {
                    if(!transfer(child.output_fd(),header,false,deadline,&work)) { throw error::io; }
                    wire::reader length_reader{header};std::uint64_t extent{};
                    if(!length_reader.u64(extent) || extent<24 || extent>response_storage.size()) { throw error::invalid; }
                    auto packet=response_storage.first(static_cast<std::size_t>(extent));
                    if(!transfer(child.output_fd(),packet,false,deadline,&work)) { throw error::io; }
                    response kind{};std::uint64_t identity{};wire::bytes payload;
                    if(!decode_response(packet,kind,identity,payload) || identity!=work.identity()) { throw error::invalid; }
                    if(kind==response::partial)
                    {
                        if(work.emit(payload)!=error::success) { throw error::cancelled; }
                        continue;
                    }
                    terminal=true;
                    auto result=work.finish(kind==response::succeeded?models::state::succeeded:models::state::failed,payload);
                    if(result!=error::success) { work.finish(models::state::failed); }
                    return true;
                }
            }
            catch(...)
            {
                if(disclosed && !terminal && !cleanup_process()) { return false; }
                // Actual child cleanup precedes reclaiming input leases. A
                // malformed response can never become a successful result.
                work.finish(work.cancellation_requested()?models::state::cancelled:models::state::failed);
                return !disclosed;
            }
        }
        static void* run(void* opaque) noexcept
        {
            auto& self=*static_cast<service*>(opaque);
            sigset_t blocked;::sigemptyset(&blocked);::sigaddset(&blocked,SIGPIPE);::pthread_sigmask(SIG_BLOCK,&blocked,nullptr);
            try
            {
                auto status=self.begin();self.startup.store(status,std::memory_order_release);
                if(status==error::success)
                {
                    while(!self.stopped.load(std::memory_order_acquire))
                    {
                        model_dispatch::work work;
                        status=self.dispatcher.claim(self.alias,self.generation,work);
                        if(status==error::again)
                        {
                            if(!self.drain_errors()) { break; }
                            pollfd idle{self.child.output_fd(),POLLIN,0};
                            // Unsolicited bytes/EOF are protocol failure.
                            if(::poll(&idle,1,static_cast<int>((self.child.reservations().poll_ns+999999)/1000000))>0) { break; }
                            continue;
                        }
                        if(status!=error::success || !self.execute(work)) { break; }
                    }
                }
            }
            catch(std::bad_alloc const&) { self.startup.store(error::no_memory,std::memory_order_release); }
            catch(...) { self.startup.store(error::io,std::memory_order_release); }
            auto cleaned=self.cleanup_process();
            // Identity failure cannot authorize replacement of an unobserved
            // process tree. Fence the binding even when startup never attached.
            // Revocation preserves unsettled job/input reservations; it does
            // not turn disconnection into successful cleanup or completion.
            try
            {
                if(cleaned)self.dispatcher.detach(self.alias,self.generation);
                else self.dispatcher.revoke(self.alias);
            }
            catch(...) {}
            return nullptr;
        }
    public:
        service(model_dispatch& value,std::string_view name,std::uint64_t epoch):dispatcher{value},alias{name},generation{epoch}{}
        ~service() override { retire(); }
        /** @brief Prepare and start a real isolated generation before exposing any guest session.
         * @details The caller has already bound this exact alias and approved
         * the manifest's complete runtime/model digest closure. Startup failure
         * returns no owner and never attaches a fake or fallback backend.
         */
        static error prepare(model_dispatch& dispatcher,std::string_view alias,std::uint64_t generation,model_sandbox::manifest const& manifest,
                             std::shared_ptr<models::provider_lifetime>& output)
        {
            if(output || alias.empty() || !generation || !model_sandbox::valid(manifest)) { return error::invalid; }
            try
            {
                models::plan selected;auto status=dispatcher.limits(alias,selected);
                if(status!=error::success || selected.model!=manifest.model || selected.task!=manifest.task) { return error::denied; }
                auto owner=::uwvm2::utils::container::make_shared<service>(dispatcher,alias,generation);
                status=owner->child.prepare(manifest);if(status!=error::success) { return status; }
                status=owner->stack.prepare(manifest.bound.supervisor_bytes);if(status!=error::success) { return status; }
                pthread_attr_t attributes;
                if(::pthread_attr_init(&attributes)) { return error::io; }
                auto configured=::pthread_attr_setstack(&attributes,owner->stack.data(),owner->stack.size());
                auto created=configured?configured: ::pthread_create(&owner->thread,&attributes,run,owner.get());
                ::pthread_attr_destroy(&attributes);
                if(created) { return error::io; }
                owner->started=true;
                auto deadline=now()+manifest.bound.startup_ns+1000000000ull;
                while((status=owner->startup.load(std::memory_order_acquire))==error::again && now()<deadline) { ::poll(nullptr,0,1); }
                if(status!=error::success) { owner->retire();return status==error::again?error::cancelled:status; }
                output=std::move(owner);return error::success;
            }
            catch(std::bad_alloc const&) { return error::no_memory; }
            catch(std::length_error const&) { return error::too_large; }
            catch(...) { return error::io; }
        }
        /** @brief Signal cancellation without locks, joins or waiting for authority. */
        void request_stop() noexcept override
        { if(!same_process()) { return; } stopped.store(true,std::memory_order_release);child.request_stop(); }
        /** @brief Join all native access before dispatcher destruction; fork copies do nothing. */
        void retire() noexcept override
        {
            if(!same_process()) { return; }
            request_stop();
            std::lock_guard lock{retirement_mutex};
            if(started && !joined) { if(!::pthread_join(thread,nullptr)) { joined=true; } }
        }
    };
#endif
}
