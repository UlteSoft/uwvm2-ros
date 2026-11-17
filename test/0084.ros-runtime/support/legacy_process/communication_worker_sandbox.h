// Test-only historical process owner. Never include or link into the VM product.
/** @file
 * @brief Exact-image communication workers with no native socket authority.
 * @details Trusted manifests pin every executable, runtime and startup profile.
 * Preparation seals inputs; launch retains the existing cgroup and uses a fresh
 * user/PID/network namespace. Traffic arrives only through four directional
 * anonymous pipes after all-thread confinement. The separate communication
 * filter never broadens the model-worker policy. This header is host-only.
 */
#pragma once
#include "linux_process_domain.h"
#include "model_sandbox.h"
#include <uwvm2/imported/ros/scene/communication_worker_bootstrap_protocol.h>
#include <uwvm2/imported/ros/scene/communication_worker_syscalls.h>
namespace uwvm2::imported::ros::scene::communication_sandbox
{
    using error=resources::error;
    enum class image_role : std::uint8_t {unconfigured,bootstrap,runtime,configuration};
    struct image_spec
    {int source{-1};std::uint64_t bytes{};buffers::digest sha256{};buffers::text<255> destination;image_role role{};};
    struct limits
    {
        std::uint64_t image_bytes{},address_bytes{},cpu_seconds{},file_bytes{},open_files{},scratch_bytes{},scratch_slots{},
            request_bytes{},response_bytes{},stderr_bytes{},startup_ns{},poll_ns{},supervisor_bytes{},profile_bytes{},thread_count{};
    };
    struct manifest
    {image_spec launcher;::uwvm2::utils::container::std_vector<image_spec> files;limits bound;buffers::profile configuration;buffers::text<255> ros_prefix;execution_placement::policy placement;};
#if defined(__linux__)
    using descriptor=model_sandbox::descriptor;
    /** @brief Verify a nonblocking anonymous pipe end, including its kernel direction. */
    inline bool anonymous_pipe(int fd,bool receive,struct stat& information) noexcept
    {
        auto flags=::fcntl(fd,F_GETFL);if(fd<0||flags<0||::fstat(fd,&information)||!S_ISFIFO(information.st_mode)||
            (flags&O_ACCMODE)!=(receive?O_RDONLY:O_WRONLY)||(flags&O_NONBLOCK)==0)return false;
        std::array<char,64> path{};constexpr std::string_view prefix="/proc/self/fd/";std::copy(prefix.begin(),prefix.end(),path.begin());
        auto number=std::to_chars(path.data()+prefix.size(),path.data()+path.size()-1,fd);if(number.ec!=std::errc{})return false;*number.ptr=0;
        std::array<char,128> target{};auto got=::readlink(path.data(),target.data(),target.size());if(got<8||static_cast<std::size_t>(got)>=target.size())return false;
        std::string_view value{target.data(),static_cast<std::size_t>(got)};
        return value.starts_with("pipe:[")&&value.back()==']'&&value.substr(6,value.size()-7).find_first_not_of("0123456789")==std::string_view::npos;
    }
    inline bool destination(std::string_view value) noexcept
    {
        if(value.starts_with("/opt/ros/"))
        {
            // Reuse the same bounded component syntax with an allowed logical prefix.
            auto rest=value.substr(std::string_view{"/opt/ros/"}.size());std::array<char,256> mapped{};constexpr std::string_view prefix="/worker/";
            if(prefix.size()+rest.size()>255)return false;
            std::copy(prefix.begin(),prefix.end(),mapped.begin());std::copy(rest.begin(),rest.end(),mapped.begin()+prefix.size());
            return model_sandbox::destination({mapped.data(),prefix.size()+rest.size()});
        }
        return !value.starts_with("/model/")&&model_sandbox::destination(value);
    }
    inline bool valid(manifest const& value) noexcept
    {
        auto const& b=value.bound;
#if defined(__linux__)
        // A finite deployment amount must not truncate into RLIM_INFINITY.
        for(auto amount:{b.address_bytes,b.cpu_seconds,b.file_bytes,b.open_files})
            if(!linux_resource_limits::finite(amount))return false;
        for(auto const& image:value.files)
            if(!linux_resource_limits::finite(image.bytes))return false;
#endif
        if(!execution_placement::valid(value.placement)||value.files.empty()||value.files.size()>256||value.launcher.source<0||!value.launcher.bytes||value.launcher.role!=image_role::runtime||
           !value.ros_prefix.size||value.ros_prefix.size>value.ros_prefix.bytes.size()||!value.ros_prefix.view().starts_with("/opt/ros/")||!destination(value.ros_prefix.view())||
           !value.configuration.identity.size||value.configuration.identity.size>value.configuration.identity.bytes.size()||
           value.configuration.identity.view().find('\0')!=std::string_view::npos||
           !wire::utf8({reinterpret_cast<std::uint8_t const*>(value.configuration.identity.bytes.data()),value.configuration.identity.size})||
           !b.image_bytes||b.image_bytes>INT64_MAX||!b.address_bytes||b.address_bytes>INT64_MAX||
           !b.cpu_seconds||b.cpu_seconds>UINT32_MAX||!b.file_bytes||b.file_bytes>INT64_MAX||b.open_files<value.files.size()+32||b.open_files>1024||
           !b.scratch_bytes||b.scratch_bytes>INT64_MAX||b.scratch_slots||!b.request_bytes||b.request_bytes>4u*1024u*1024u||
           !b.response_bytes||b.response_bytes>4u*1024u*1024u||b.stderr_bytes>65536||!b.startup_ns||b.startup_ns>INT64_MAX||
           !b.poll_ns||b.poll_ns>100000000||b.supervisor_bytes<512u*1024u||b.supervisor_bytes>16u*1024u*1024u||b.supervisor_bytes%4096||
           !b.profile_bytes||b.profile_bytes>16u*1024u*1024u||!b.thread_count||b.thread_count>256||value.launcher.bytes>b.image_bytes)return false;
        std::uint64_t total=value.launcher.bytes;unsigned bootstraps{},backends{},profiles{};
        for(std::size_t i{};i<value.files.size();++i)
        {
            auto const& file=value.files[i];
            if(file.source<0||file.role<image_role::bootstrap||file.role>image_role::configuration||file.destination.size>file.destination.bytes.size()||!destination(file.destination.view())||
               file.bytes>(b.image_bytes-total)/2)return false;
            total+=file.bytes*2;
            if(file.role==image_role::bootstrap){if(file.destination.view()!="/worker/bootstrap")return false;++bootstraps;}
            if(file.destination.view()=="/worker/backend.so"){if(file.role!=image_role::runtime)return false;++backends;}
            if(file.destination.view()=="/worker/profile")
            {if(file.role!=image_role::configuration||file.bytes!=b.profile_bytes||file.sha256!=value.configuration.sha256)return false;++profiles;}
            for(std::size_t j{};j<i;++j)
            {
                auto other=value.files[j].destination.view(),name=file.destination.view();
                if(other==name||(other.starts_with(name)&&other.size()>name.size()&&other[name.size()]=='/')||
                   (name.starts_with(other)&&name.size()>other.size()&&name[other.size()]=='/'))return false;
            }
        }
        return bootstraps==1&&backends==1&&profiles==1;
    }
    /** @brief Reuse only the verified sealed-image primitive, never the model syscall profile. */
    inline error seal(image_spec const& source,descriptor& output)
    {
        if(source.role<image_role::bootstrap||source.role>image_role::configuration)return error::invalid;
        model_sandbox::image_spec converted{source.source,source.bytes,source.sha256,source.destination,
            source.role==image_role::bootstrap?model_sandbox::image_role::bootstrap:
            source.role==image_role::runtime?model_sandbox::image_role::runtime:model_sandbox::image_role::weights};
        return model_sandbox::seal(converted,output);
    }
    /** @brief Prepared exact-image process with bounded directional pipes and pidfd ownership.
     * @details prepare() performs no spawn. launch() is trusted deployment work,
     * never lazy guest execution. request_stop() only signals its own retained
     * pidfd and does not acquire a lock; reap() confirms actual process cleanup.
     * Forked copies fail before touching descriptors belonging to the parent.
     * This object is not concurrently relaunched; create a fresh object/generation
     * after retirement. Its owner joins all I/O users before destruction.
     */
    class process
    {
        struct image
        {
            descriptor file;
            buffers::text<255> destination;
            image_role role{};
        };

        descriptor launcher, filter, empty_file, pidfd, runtime_pidfd, input, output, diagnostics, control, status_pipe, release_pipe;
        ::uwvm2::utils::container::std_vector<image> images;
        ::uwvm2::utils::container::std_vector<::uwvm2::utils::container::std_string> arguments;
        limits bounds{};
        execution_placement::resolved placement;
        buffers::profile configuration;
        ::pid_t owner{::getpid()}, child{};
        bool prepared{}, launched{}, launcher_authorized{}, reaped{}, channels_enabled{}, identified{}, ready_seen{}, attached_seen{};
        std::uint64_t launched_at{};std::size_t marker_offset{};
        std::array<char,4096> status_text{};
        std::size_t status_size{};
        std::uint64_t startup_file_limit{};
        int child_status{};
        std::atomic<int> signalling{-1}, runtime_signalling{-1};
        std::atomic<bool> stop_requested{};

        bool same_process() const noexcept { return owner == ::getpid(); }

        bool startup_expired() const noexcept
        {
            auto now=resources::steady_now();
            return now<0||static_cast<std::uint64_t>(now)<launched_at||static_cast<std::uint64_t>(now)-launched_at>bounds.startup_ns;
        }

        void argument(std::string_view value) { arguments.emplace_back(value); }

        void pair(std::string_view flag, std::string_view value)
        {
            argument(flag);
            argument(value);
        }

        static bool write_all(int fd, std::span<std::uint8_t const> bytes) noexcept
        {
            std::size_t offset{};
            while(offset < bytes.size())
            {
                auto result = ::write(fd, bytes.data() + offset, bytes.size() - offset);
                if(result < 0 && errno == EINTR) { continue; }
                if(result <= 0) { return false; }
                offset += result;
            }
            return true;
        }

        static bool pipe(descriptor& read, descriptor& write) noexcept
        {
            int values[2];
            if(::pipe2(values, O_CLOEXEC)) { return false; }
            read.reset(values[0]);
            write.reset(values[1]);
            return true;
        }

        static bool nonblocking(int fd) noexcept
        {
            auto flags = ::fcntl(fd, F_GETFL);
            return flags >= 0 && !::fcntl(fd, F_SETFL, flags | O_NONBLOCK);
        }

    public:
        process() = default;
        process(process const&) = delete;
        process& operator= (process const&) = delete;

        ~process()
        {
            if(same_process())
            {
                request_stop();
                if(launched && !reaped)
                {
                    int status{};
                    while(::waitpid(child, &status, 0) < 0 && errno == EINTR) {};
                    if(runtime_pidfd) { pollfd actual{runtime_pidfd.get(),POLLIN,0};while(::poll(&actual,1,-1)<0 && errno==EINTR) {} }
                }
            }
        }

        /** @brief Verify/seal every image and build a fixed non-shell argument vector. */
        error prepare(manifest const& selected)
        {
            if(!same_process()) { return error::denied; }
            if(stop_requested.load(std::memory_order_acquire)) { return error::cancelled; }
            if(prepared || launched) { return error::conflict; }
            if(!valid(selected)) { return error::invalid; }
            execution_placement::resolved chosen;auto status=chosen.prepare(selected.placement);
            if(status!=error::success)return status;
            descriptor approved;
            status = seal(selected.launcher, approved);
            if(status != error::success) { return status; }
            ::uwvm2::utils::container::std_vector<image> staged;
            staged.reserve(selected.files.size());
            for(auto const& file: selected.files)
            {
                descriptor sealed;
                status = seal(file, sealed);
                if(status != error::success) { return status; }
                staged.push_back({std::move(sealed), file.destination, file.role});
            }
            syscall_filter policy{syscall_phase::startup};
            auto bytes = std::as_bytes(policy.view());
            descriptor policy_fd{static_cast<int>(::syscall(SYS_memfd_create, "uwvm-communication-seccomp", MFD_CLOEXEC | MFD_ALLOW_SEALING | 8u))};
            descriptor scratch_fd{static_cast<int>(::syscall(SYS_memfd_create, "uwvm-communication-empty", MFD_CLOEXEC | MFD_ALLOW_SEALING | 8u))};
            if(!policy_fd || !scratch_fd || !write_all(policy_fd.get(), {reinterpret_cast<std::uint8_t const*>(bytes.data()), bytes.size()}) ||
               ::fcntl(policy_fd.get(), F_ADD_SEALS, 1 | 2 | 4 | 8 | 32) < 0 || ::fcntl(scratch_fd.get(), F_ADD_SEALS, 1 | 2 | 4 | 8 | 32) < 0 ||
               ::lseek(policy_fd.get(), 0, SEEK_SET) < 0)
            {
                return error::io;
            }
            arguments.clear();
            arguments.reserve(32 + selected.files.size() * 20);
            argument("bwrap");
            for(auto flag: {"--unshare-user",
                            "--unshare-ipc",
                            "--unshare-pid",
                            "--unshare-net",
                            "--unshare-uts",
                            "--unshare-cgroup",
                            "--disable-userns",
                            "--assert-userns-disabled",
                            "--clearenv",
                            "--new-session",
                            "--die-with-parent",
                            "--as-pid-1"})
            {
                argument(flag);
            }
            pair("--cap-drop", "ALL");
            ::uwvm2::utils::container::std_set<::uwvm2::utils::container::std_string, std::less<>> directories;
            for(auto const& file: selected.files)
            {
                auto path = file.destination.view();
                std::size_t at = 1;
                while((at = path.find('/', at)) != path.npos)
                {
                    directories.emplace(path.substr(0, at));
                    ++at;
                }
            }
            if(directories.size() > 256) { return error::too_large; }
            for(auto const& directory: directories) { pair("--dir", directory); }
            ::uwvm2::utils::container::std_set<::uwvm2::utils::container::std_string,std::less<>> library_directories;
            for(auto const& file:selected.files)
            {
                if(file.role!=image_role::runtime)continue;
                auto name=file.destination.view();library_directories.emplace(name.substr(0,name.rfind('/')));
            }
            ::uwvm2::utils::container::std_string library_path;
            for(auto const& directory:library_directories)
            {
                if(!library_path.empty())library_path.push_back(':');
                library_path.append(directory);
                if(library_path.size()>4095)return error::too_large;
            }
            argument("--setenv");argument("LD_LIBRARY_PATH");argument(library_path);
            argument("--setenv");argument("RMW_IMPLEMENTATION");argument("rmw_fastrtps_cpp");
            argument("--setenv");argument("AMENT_PREFIX_PATH");argument(selected.ros_prefix.view());
            argument("--setenv");argument("RCUTILS_LOGGING_USE_STDOUT");argument("0");
            // Only sealed images and trusted deployment data are inherited.
            // Directional traffic pipes arrive only after the first TSYNC fence.
            // stdin/out/err=0..2, control=3, launcher=4, code=5.., filter,
            // empty scratch, JSON identity output, namespace release input.
            std::size_t code_count{};
            startup_file_limit = selected.bound.file_bytes;
            for(auto const& file: selected.files)
            {
                pair("--perms",file.role==image_role::configuration?"0400":"0500");
                argument("--ro-bind-data");
                argument(::uwvm2::utils::container::to_string(code_count++ + 5));
                argument(file.destination.view());
                startup_file_limit = std::max(startup_file_limit, file.bytes);
            }
            pair("--size", ::uwvm2::utils::container::to_string(selected.bound.scratch_bytes));
            pair("--perms", "0700");
            pair("--tmpfs", "/scratch");
            for(std::uint64_t i{}; i != selected.bound.scratch_slots; ++i)
            {
                pair("--perms", "0600");
                argument("--file");
                argument(::uwvm2::utils::container::to_string(code_count + 6 + i));
                argument(::uwvm2::utils::container::concat("/scratch/slot-", i));
            }
            pair("--proc","/proc");
            pair("--remount-ro","/proc");
            pair("--remount-ro", "/");
            pair("--chdir", "/worker");
            pair("--seccomp", ::uwvm2::utils::container::to_string(code_count + 5));
            pair("--info-fd", ::uwvm2::utils::container::to_string(code_count + 6 + selected.bound.scratch_slots));
            pair("--block-fd", ::uwvm2::utils::container::to_string(code_count + 7 + selected.bound.scratch_slots));
            argument("--");
            argument("/worker/bootstrap");
            argument(::uwvm2::utils::container::to_string(selected.bound.request_bytes));
            argument(::uwvm2::utils::container::to_string(selected.bound.response_bytes));
            argument(::uwvm2::utils::container::to_string(selected.bound.profile_bytes));
            argument(::uwvm2::utils::container::to_string(selected.bound.thread_count));
            // Bootstrap re-verifies the actual read-only namespace objects and
            // binds executable maps to their device/inode identities, not paths.
            constexpr char hexadecimal[]="0123456789abcdef";
            for(auto const& file:selected.files)
            {
                if(file.role==image_role::configuration)continue;
                argument(file.destination.view());argument(::uwvm2::utils::container::to_string(file.bytes));
                std::array<char,64> hash{};
                for(std::size_t i{};i<file.sha256.size();++i){hash[i*2]=hexadecimal[file.sha256[i]>>4];hash[i*2+1]=hexadecimal[file.sha256[i]&15];}
                argument({hash.data(),hash.size()});
            }
            launcher = std::move(approved);
            images = std::move(staged);
            filter = std::move(policy_fd);
            empty_file = std::move(scratch_fd);
            bounds = selected.bound;
            configuration=selected.configuration;
            placement=std::move(chosen);
            prepared = true;
            return error::success;
        }

        /** @brief Spawn only the prepared launcher, with no inherited host descriptors/env.
         * @details The post-fork child uses only descriptor/resource syscalls and
         * exec. Every allocating argument/descriptor preparation precedes fork.
         * Process limits are hard limits; the existing 64 GiB cgroup is retained.
         * All descriptor setup precedes fork. A one-use sequenced-packet gate
         * authorizes exec only after pidfd ownership is published. Failed launch
         * shuts down the gate, including inherited duplicates, so an unpinned
         * child exits on EOF without a numeric-PID signal or blocking wait.
         * launch_started() records ownership even when this function fails;
         * the supervisor must reap that child before releasing its reservation.
         */
        error launch()
        {
            if(!same_process()) { return error::denied; }
            if(stop_requested.load(std::memory_order_acquire)) { return error::cancelled; }
            if(!prepared || launched) { return error::conflict; }
            descriptor in_read, in_write, out_read, out_write, err_read, err_write, status_read, status_write, release_read, release_write;
            int sockets[2];
            if(::socketpair(AF_UNIX, SOCK_SEQPACKET | SOCK_CLOEXEC, 0, sockets)) { return error::io; }
            descriptor host_control{sockets[0]}, worker_control{sockets[1]};
            if(::shutdown(host_control.get(),SHUT_RD)||::shutdown(worker_control.get(),SHUT_WR))return error::io;
            if(!pipe(in_read, in_write) || !pipe(out_read, out_write) || !pipe(err_read, err_write) || !pipe(status_read,status_write)) { return error::io; }
            // A dead receiver must return an error without raising SIGPIPE
            // in a host thread that did not opt into a signal-mask policy.
            int releases[2];
            if(::socketpair(AF_UNIX,SOCK_SEQPACKET|SOCK_CLOEXEC,0,releases))return error::io;
            release_read.reset(releases[0]);release_write.reset(releases[1]);
            if(::shutdown(release_read.get(),SHUT_WR)||::shutdown(release_write.get(),SHUT_RD))return error::io;
            // Child protocol/diagnostic I/O must not sleep inside read/write
            // beyond a frame deadline. F_SETFL remains unavailable in workers.
            if(!nonblocking(in_read.get())||!nonblocking(out_write.get())||!nonblocking(err_write.get()))return error::io;
            // The host ends have distinct open descriptions from worker ends.
            // Refusal here cannot leave an already authorized child running.
            if(!nonblocking(in_write.get()) || !nonblocking(out_read.get()) || !nonblocking(err_read.get()) ||
               !nonblocking(host_control.get()) || !nonblocking(status_read.get())) { return error::io; }
            int gates[2];
            if(::socketpair(AF_UNIX, SOCK_SEQPACKET | SOCK_CLOEXEC, 0, gates)) { return error::io; }
            descriptor gate_host{gates[0]}, gate_child{gates[1]};
            ::uwvm2::utils::container::std_vector<int> sources;
            sources.reserve(images.size() + 8 + bounds.scratch_slots);
            sources.push_back(in_read.get());
            sources.push_back(out_write.get());
            sources.push_back(err_write.get());
            sources.push_back(worker_control.get());
            sources.push_back(launcher.get());
            for(auto const& file: images) { sources.push_back(file.file.get()); }
            sources.push_back(filter.get());
            for(std::uint64_t i{};i<bounds.scratch_slots;++i) { sources.push_back(empty_file.get()); }
            sources.push_back(status_write.get());
            sources.push_back(release_read.get());
            ::uwvm2::utils::container::std_vector<descriptor> copies;
            copies.reserve(sources.size());
            for(auto fd: sources)
            {
                descriptor copy{::fcntl(fd, F_DUPFD_CLOEXEC, 1024)};
                if(!copy) { return error::io; }
                copies.emplace_back(std::move(copy));
            }
            ::uwvm2::utils::container::std_vector<char*> argv;
            argv.reserve(arguments.size() + 1);
            for(auto& value: arguments) { argv.push_back(value.data()); }
            argv.push_back(nullptr);
            descriptor parent_identity{static_cast<int>(::syscall(SYS_pidfd_open,owner,0))};
            if(!parent_identity)return error::unsupported;
            int born_identity{-1};bool parent_restored{};
            auto spawned = linux_process_domain::spawn(born_identity,parent_restored);
            if(spawned < 0) { return error::io; }
            if(!spawned)
            {
                auto fail = []() noexcept { ::_exit(126); };
                if(!linux_process_domain::enter(parent_identity.get())) { fail(); }
                gate_host.reset();
                std::uint64_t permitted{};
                auto read = ::recv(gate_child.get(), &permitted, sizeof(permitted), 0);
                if(read != sizeof(permitted) || permitted != 1) { fail(); }
                if(!placement.apply_namespace_child(parent_identity.get(),owner)){fail();}
                for(std::size_t i{}; i != copies.size(); ++i)
                {
                    if(::dup2(copies[i].get(), static_cast<int>(i)) < 0) { fail(); }
                }
                if(::syscall(SYS_close_range, static_cast<unsigned>(copies.size()), UINT_MAX, 0)) { fail(); }
                if(::fcntl(4, F_SETFD, FD_CLOEXEC) < 0) { fail(); }
                auto limit = [&](int resource, std::uint64_t maximum) noexcept
                {
                    if(!linux_resource_limits::apply(resource,maximum)) { fail(); }
                };
                limit(RLIMIT_AS, bounds.address_bytes);
                limit(RLIMIT_CPU, bounds.cpu_seconds);
                limit(RLIMIT_FSIZE, startup_file_limit);
                limit(RLIMIT_NOFILE, bounds.open_files);
                limit(RLIMIT_CORE, 0);
                limit(RLIMIT_MEMLOCK, 0);
                char* environment[]{nullptr};
                ::syscall(SYS_execveat, 4, "", argv.data(), environment, AT_EMPTY_PATH);
                fail();
            }
            child = spawned;
            launched = true;launched_at=static_cast<std::uint64_t>(resources::steady_now());
            gate_child.reset();
            pidfd.reset(born_identity);
            if(!pidfd)
            {
                // Kernel clone3 success is required to supply this descriptor.
                // If the contract is unavailable, reject the still-gated child.
                (void)::shutdown(gate_host.get(),SHUT_WR);return error::unsupported;
            }
            signalling.store(pidfd.get(), std::memory_order_release);
            if(!parent_restored||!linux_process_domain::map_identity(child,pidfd.get(),::geteuid(),::getegid()))
            {
                (void)::shutdown(gate_host.get(),SHUT_WR);request_stop();return error::io;
            }
            if(stop_requested.load(std::memory_order_acquire))
            {
                (void)::shutdown(gate_host.get(), SHUT_WR);
                request_stop();
                return error::cancelled;
            }
            std::uint64_t permit = 1;
            if(::send(gate_host.get(), &permit, sizeof(permit), MSG_NOSIGNAL) != sizeof(permit))
            {
                (void)::shutdown(gate_host.get(), SHUT_WR);
                request_stop();
                return error::io;
            }
            // SOCK_SEQPACKET sends this permit atomically. Only this successful
            // send can authorize exec; failures cannot release a partial permit.
            launcher_authorized = true;
            input = std::move(in_write);
            output = std::move(out_read);
            diagnostics = std::move(err_read);
            control = std::move(host_control);
            status_pipe = std::move(status_read);
            release_pipe = std::move(release_write);
            return error::success;
        }

        /** @brief Accept the sealed launcher's bounded startup report and release setup.
         * @details The report contains a PID local to the already owned domain;
         * it is syntax, never a host process-control identity. The kernel supplied
         * the outer namespace-init pidfd at birth, before launcher authorization.
         * A current liveness check and MSG_NOSIGNAL release reject receiver death
         * without losing whole-tree ownership or publishing readiness.
         */
        error identify() noexcept
        {
            if(!same_process()) { return error::denied; }
            if(stop_requested.load(std::memory_order_acquire)) { return error::cancelled; }
            if(!launched || reaped) { return error::bad_handle; }
            if(identified) { return error::success; }
            if(startup_expired()) { request_stop(); return error::timeout; }
            auto count = ::read(status_pipe.get(), status_text.data()+status_size, status_text.size()-status_size);
            if(count < 0) { return errno == EAGAIN || errno == EINTR ? error::again : error::io; }
            if(!count) { return error::io; }
            status_size += static_cast<std::size_t>(count);
            std::string_view text{status_text.data(),status_size};
            if(text.find("}\n") == text.npos) { return status_size == status_text.size() ? error::too_large : error::again; }
            constexpr std::string_view marker = "\"child-pid\": ";
            auto key=text.find(marker);
            if(key==text.npos || !text.starts_with("{\n")) { return error::invalid; }
            auto prefix=text.substr(0,key+marker.size());
            auto position = prefix.size();
            unsigned long number{};
            while(position < text.size() && text[position]>='0' && text[position]<='9')
            {
                if(number > (INT_MAX - 9ul)/10ul) { return error::invalid; }
                number=number*10+static_cast<unsigned>(text[position++]-'0');
            }
            if(!number || position==prefix.size() || position==text.size() || (text[position]!=',' && text[position]!=' ' && text[position]!='\n')) { return error::invalid; }
            descriptor actual{::fcntl(pidfd.get(),F_DUPFD_CLOEXEC,0)};
            if(!actual)return error::io;
            // The sealed launcher reports a PID local to the outer domain.
            // Its value grants no authority; both pins identify the kernel-born
            // domain init, which owns every nested launcher/worker descendant.
            std::array<pollfd,2> live{{{pidfd.get(),POLLIN,0},{actual.get(),POLLIN,0}}};
            if(::poll(live.data(),live.size(),0)!=0) { return error::denied; }
            runtime_pidfd=std::move(actual);
            runtime_signalling.store(runtime_pidfd.get(),std::memory_order_release);
            if(stop_requested.load(std::memory_order_acquire)) { request_stop(); return error::cancelled; }
            std::uint8_t released{1};
            if(::send(release_pipe.get(),&released,1,MSG_NOSIGNAL)!=1) { request_stop(); return error::io; }
            release_pipe.reset();
            identified=true;
            return error::success;
        }

        /** @brief Observe the exact bootstrap phase marker without accepting application data.
         * @details Only this startup owner reads stdout until attached succeeds.
         * Unexpected bytes, EOF or elapsed startup budget fail closed. The caller
         * must request_stop/reap on failure and cannot publish a live guest owner.
         */
        error readiness(bool attached=false) noexcept
        {
            if(!same_process())return error::denied;
            if(stop_requested.load(std::memory_order_acquire))return error::cancelled;
            if(!identified||reaped||(attached&&!channels_enabled))return error::conflict;
            if(attached?attached_seen:ready_seen)return error::success;
            if(startup_expired())
            {request_stop();return error::timeout;}
            auto expected=attached?communication_worker::attached:communication_worker::ready;
            std::array<char,64> bytes{};auto amount=::read(output.get(),bytes.data(),expected.size()-marker_offset);
            if(amount<0)return errno==EAGAIN||errno==EINTR?error::again:error::io;
            if(!amount)return error::io;
            if(std::string_view{bytes.data(),static_cast<std::size_t>(amount)}!=expected.substr(marker_offset,static_cast<std::size_t>(amount)))return error::invalid;
            marker_offset+=static_cast<std::size_t>(amount);if(marker_offset!=expected.size())return error::again;
            marker_offset=0;if(attached)attached_seen=true;else ready_seen=true;return error::success;
        }
        /** @brief Transfer four approved anonymous-pipe ends after all-thread confinement.
         * @details Roles are fixed and ordered; direction is also enforced by the
         * kernel pipe access mode. No UDP socket, path or guest-selected native FD
         * is accepted. The caller keeps its broker idle until readiness(true).
         */
        error activate(std::span<communication_worker::role_fd const> selected)
        {
            using namespace communication_worker;
            if(!same_process())return error::denied;
            if(stop_requested.load(std::memory_order_acquire))return error::cancelled;
            if(!ready_seen||reaped||channels_enabled)return error::conflict;
            if(startup_expired()){request_stop();return error::timeout;}
            if(selected.size()!=roles.size())return error::invalid;
            std::array<int,4> descriptors{};std::array<descriptor,4> pinned{};std::array<struct stat,4> information{};
            std::array<std::uint8_t,40> payload{};wire::writer writer{payload};
            if(!writer.u64(selected.size()))return error::invalid;
            for(std::size_t i{};i<selected.size();++i)
            {
                auto entry=selected[i];if(entry.role!=roles[i]||entry.descriptor<0)return error::denied;
                pinned[i].reset(::fcntl(entry.descriptor,F_DUPFD_CLOEXEC,4));if(!pinned[i])return error::io;
                if(!anonymous_pipe(pinned[i].get(),receiving(entry.role),information[i]))return error::denied;
                for(std::size_t j{};j<i;++j)if(information[j].st_dev==information[i].st_dev&&information[j].st_ino==information[i].st_ino)return error::invalid;
                descriptors[i]=pinned[i].get();if(!writer.u64(static_cast<std::uint64_t>(entry.role)))return error::invalid;
            }
            alignas(cmsghdr) std::array<std::uint8_t,CMSG_SPACE(sizeof(int)*4)> ancillary{};
            iovec vector{payload.data(),writer.size()};msghdr message{};message.msg_iov=&vector;message.msg_iovlen=1;
            message.msg_control=ancillary.data();message.msg_controllen=ancillary.size();auto header=CMSG_FIRSTHDR(&message);
            header->cmsg_level=SOL_SOCKET;header->cmsg_type=SCM_RIGHTS;header->cmsg_len=CMSG_LEN(sizeof(int)*4);
            std::memcpy(CMSG_DATA(header),descriptors.data(),sizeof(descriptors));
            auto sent=::sendmsg(control.get(),&message,MSG_DONTWAIT|MSG_NOSIGNAL);
            if(sent<0)return errno==EAGAIN||errno==EINTR?error::again:error::io;
            if(static_cast<std::size_t>(sent)!=writer.size())return error::io;
            channels_enabled=true;control.reset();
            if(stop_requested.load(std::memory_order_acquire)){request_stop();return error::cancelled;}
            return error::success;
        }

        int input_fd() const noexcept { return same_process() ? input.get() : -1; }

        int output_fd() const noexcept { return same_process() ? output.get() : -1; }

        int stderr_fd() const noexcept { return same_process() ? diagnostics.get() : -1; }

        int process_fd() const noexcept { return same_process() ? pidfd.get() : -1; }

        /** @brief Whether fork actually occurred, including a failed launch. */
        bool launch_started() const noexcept{return same_process()&&launched;}

        /** @brief Confirm that the born namespace init has actually been reaped.
         * @details Kernel teardown of this init includes all descendants in
         * nested PID namespaces. No startup report, receipt, timeout or successful
         * signal can set this flag. Even rejected map/exec/gate paths retain the
         * direct-child ownership until reap. Physical effects remain separate.
         */
        bool cleanup_confirmed() const noexcept
        {return same_process() && (!launched || reaped);}

        /** @brief Whether the startup path retained a pin to the owned process domain.
         * @details This is identity evidence, not readiness or liveness. The
         * domain is already owned at birth; failed startup does not require this
         * duplicate before cleanup_confirmed() can establish complete teardown.
         */
        bool namespace_identity_confirmed() const noexcept
        {return same_process()&&runtime_signalling.load(std::memory_order_acquire)>=0;}

        /** @brief Borrow the pinned outer namespace-init identity, never a numeric PID.
         * @details Trusted supervisors duplicate this descriptor while retaining
         * the process object. It identifies the exact sealed worker incarnation;
         * a port, DDS GUID or a numeric PID cannot substitute for this evidence.
         */
        int namespace_process_fd() const noexcept
        {return same_process()?runtime_signalling.load(std::memory_order_acquire):-1;}

        /** @brief Cached startup evidence for serialized trusted supervision. */
        bool privately_ready() const noexcept
        {return same_process()&&identified&&ready_seen&&!reaped&&!stop_requested.load(std::memory_order_acquire);}
        bool attachment_confirmed() const noexcept
        {return privately_ready()&&attached_seen;}
        buffers::profile const& configuration_profile() const noexcept{return configuration;}

        limits const& reservations() const noexcept { return bounds; }

        /** @brief Nonblocking cancellation; an old pidfd cannot target a reused PID. */
        void request_stop() noexcept
        {
            if(!same_process()) { return; }
            stop_requested.store(true,std::memory_order_release);
            auto actual = runtime_signalling.load(std::memory_order_acquire);
            if(actual >= 0) { ::syscall(SYS_pidfd_send_signal, actual, SIGKILL, nullptr, 0); }
            auto fd = signalling.load(std::memory_order_acquire);
            if(fd >= 0) { ::syscall(SYS_pidfd_send_signal, fd, SIGKILL, nullptr, 0); }
        }

        /** @brief Observe actual domain-init exit without blocking under authority.
         * @details The direct child is PID 1 of a new namespace from birth.
         * waitpid succeeds only after its namespace teardown; an absent startup
         * report does not leave untracked descendants outside this ownership.
         */
        error reap(int& status) noexcept
        {
            if(!same_process()) { return error::denied; }
            if(!launched) { return error::bad_handle; }
            if(reaped)
            {
                status = child_status;
                return error::success;
            }
            if(runtime_pidfd) { pollfd actual{runtime_pidfd.get(),POLLIN,0}; if(::poll(&actual,1,0)!=1 || !(actual.revents&POLLIN)) { return error::again; } }
            auto done = ::waitpid(child, &status, WNOHANG);
            if(done == 0) { return error::again; }
            if(done < 0) { return errno == EINTR ? error::again : error::io; }
            child_status = status;
            reaped = true;
            input.reset();
            return error::success;
        }
    };
#endif
}
