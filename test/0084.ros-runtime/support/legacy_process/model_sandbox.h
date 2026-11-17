// Test-only historical process owner. Never include or link into the VM product.
/** @file
 * @brief Fixed-image Linux CPU worker isolation; no guest-selected executable.
 * @details Trusted prepare supplies exact regular-file descriptors, sizes,
 * digests and logical destinations. Images first enter sealed memfds; runtime
 * files are copied again by bubblewrap, and both copies count against image_bytes.
 * Model descriptors remain solely in the supervisor until final-filter readiness.
 * A fixed approved bootstrap installs the final no-exec/no-executable-mapping
 * filter before the supervisor enables read access to model data. Every child
 * stays inside the parent's existing cgroup; no cgroup migration is performed.
 */
#pragma once
#include "linux_process_domain.h"
#include <uwvm2/imported/ros/scene/linux_resource_limits.h>
#include <uwvm2/imported/ros/scene/buffers.h>
#include <uwvm2/imported/ros/scene/wire.h>
#include <uwvm2/utils/container/unique.h>
#include "execution_placement.h"
#include <cerrno>
#include <limits>
#include <charconv>
#if defined(__linux__)
# include <fcntl.h>
# include <linux/audit.h>
# include <linux/filter.h>
# include <linux/memfd.h>
# include <linux/seccomp.h>
# include <sys/poll.h>
# include <signal.h>
# include <sys/prctl.h>
# include <sys/eventfd.h>
# include <sys/resource.h>
# include <sys/stat.h>
# include <sys/socket.h>
# include <sys/syscall.h>
# include <sys/wait.h>
# include <unistd.h>
#endif

namespace uwvm2::imported::ros::scene::model_sandbox
{
    using error = resources::error;
    enum class image_role : std::uint8_t
    {
        unconfigured,
        bootstrap,
        runtime,
        weights
    };

    /** @brief Borrowed trusted input descriptor; the sandbox never follows a host path. */
    struct image_spec
    {
        int source{-1};
        std::uint64_t bytes{};
        buffers::digest sha256{};
        buffers::text<255> destination;
        image_role role{};
    };

    /** @brief Explicit worker resource reservations; zero grants no process. */
    struct limits
    {
        std::uint64_t image_bytes{}, address_bytes{}, cpu_seconds{}, file_bytes{}, open_files{}, scratch_bytes{}, scratch_slots{}, request_bytes{},
            response_bytes{}, stderr_bytes{}, startup_ns{}, poll_ns{}, supervisor_bytes{};
    };

    /** @brief Complete approved image list and exact official launcher content. */
    struct manifest
    {
        image_spec launcher;
        ::uwvm2::utils::container::std_vector<image_spec> files;
        limits bound;
        buffers::profile model, task;
        execution_placement::policy placement;
    };
#if defined(__linux__)
    /** @brief Move-only kernel descriptor ownership, with no heap allocation. */
    class descriptor
    {
        int value{-1};

    public:
        descriptor() = default;

        explicit descriptor(int fd) noexcept : value{fd} {}

        descriptor(descriptor const&) = delete;
        descriptor& operator= (descriptor const&) = delete;

        descriptor(descriptor&& other) noexcept : value{std::exchange(other.value, -1)} {}

        descriptor& operator= (descriptor&& other) noexcept
        {
            if(this != &other)
            {
                reset();
                value = std::exchange(other.value, -1);
            }
            return *this;
        }

        ~descriptor() { reset(); }

        int get() const noexcept { return value; }

        explicit operator bool() const noexcept { return value >= 0; }

        int release() noexcept { return std::exchange(value, -1); }

        void reset(int fd = -1) noexcept
        {
            auto previous = std::exchange(value, fd);
            if(previous >= 0) { ::close(previous); }
        }
    };
#endif

    inline bool destination(std::string_view value) noexcept
    {
        if(value.size() < 2 || value.size() > 255 || value.front() != '/' || value.back() == '/' || value.find('\0') != value.npos) { return false; }
        std::size_t begin = 1, components{};
        while(begin < value.size())
        {
            auto end = value.find('/', begin);
            if(end == value.npos) { end = value.size(); }
            auto part = value.substr(begin, end - begin);
            if(part.empty() || part == "." || part == ".." || ++components > 16) { return false; }
            for(auto ch: part)
            {
                if(!((ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z') || (ch >= '0' && ch <= '9') || ch == '.' || ch == '_' || ch == '-' || ch == '+'))
                {
                    return false;
                }
            }
            begin = end + 1;
        }
        for(auto allowed:
            {std::string_view{"/worker/"}, std::string_view{"/lib/"}, std::string_view{"/lib64/"}, std::string_view{"/usr/lib/"}, std::string_view{"/model/"}})
        {
            if(value.starts_with(allowed)) { return true; }
        }
        return false;
    }

    inline bool valid(manifest const& value) noexcept
    {
        auto const& b = value.bound;
#if defined(__linux__)
        // A finite deployment amount must not truncate into RLIM_INFINITY.
        for(auto amount:{b.address_bytes,b.cpu_seconds,b.file_bytes,b.open_files})
            if(!linux_resource_limits::finite(amount))return false;
        for(auto const& image:value.files)
            if(image.role!=image_role::weights&&!linux_resource_limits::finite(image.bytes))return false;
#endif
        if(!execution_placement::valid(value.placement)||value.files.empty() || value.files.size() > 64 || value.launcher.source < 0 || !value.launcher.bytes || !b.image_bytes ||
           b.image_bytes > INT64_MAX || !b.address_bytes || b.address_bytes > INT64_MAX || !b.cpu_seconds || b.cpu_seconds > UINT32_MAX || !b.file_bytes ||
           b.file_bytes > INT64_MAX || b.open_files < value.files.size() + b.scratch_slots + 16 || b.open_files > 256 || !b.scratch_bytes || b.scratch_bytes > INT64_MAX ||
           b.scratch_slots > 16 || !b.request_bytes || b.request_bytes > 256ull * 1024 * 1024 || !b.response_bytes ||
           b.response_bytes > 4ull * 1024 * 1024 + 64 || b.stderr_bytes > 65536 || !b.startup_ns || b.startup_ns > INT64_MAX || !b.poll_ns ||
           b.poll_ns > 100000000 || b.supervisor_bytes < 512u*1024u || b.supervisor_bytes > 16u*1024u*1024u || b.supervisor_bytes%4096)
        {
            return false;
        }
        if(value.launcher.bytes > b.image_bytes || value.launcher.role != image_role::runtime) { return false; }
        std::uint64_t total = value.launcher.bytes;
        std::size_t bootstraps{}, backends{};
        for(std::size_t i{}; i != value.files.size(); ++i)
        {
            auto const& file = value.files[i];
            if(file.source < 0 || file.role < image_role::bootstrap || file.role > image_role::weights || file.destination.size > 255 ||
               !destination(file.destination.view()) || file.bytes > b.image_bytes - total)
            {
                return false;
            }
            if((file.role == image_role::weights) != file.destination.view().starts_with("/model/")) { return false; }
            auto copies = file.role == image_role::weights ? 1u : 2u;
            if(file.bytes > (b.image_bytes - total) / copies) { return false; }
            total += file.bytes * copies;
            if(file.role == image_role::bootstrap)
            {
                if(file.destination.view() != "/worker/bootstrap") { return false; }
                ++bootstraps;
            }
            if(file.destination.view() == "/worker/backend.so")
            {
                if(file.role != image_role::runtime) { return false; }
                ++backends;
            }
            for(std::size_t j{}; j != i; ++j)
            {
                auto other = value.files[j].destination.view(), name = file.destination.view();
                if(other == name || (other.size() > name.size() && other.starts_with(name) && other[name.size()] == '/') ||
                   (name.size() > other.size() && name.starts_with(other) && name[other.size()] == '/'))
                {
                    return false;
                }
            }
        }
        return total <= b.image_bytes && bootstraps == 1 && backends == 1;
    }

    /** @brief Copy and verify immutable bytes, then permanently seal write/resize/exec mode.
     * @details NX data creation is mandatory, never silently downgraded on older
     * kernels. NX seals prevent exec/chmod changes; final seccomp separately
     * prevents executable mappings. No path or directory is accepted here.
     */
#if defined(__linux__)
    inline error seal(image_spec const& source, descriptor& output)
    {
        if(output || source.source < 0 || source.role < image_role::bootstrap || source.role > image_role::weights || source.bytes > static_cast<std::uint64_t>((std::numeric_limits<off_t>::max)())) { return error::invalid; }
        struct stat information{};
        if(::fstat(source.source, &information) || !S_ISREG(information.st_mode) || information.st_size < 0 ||
           static_cast<std::uint64_t>(information.st_size) != source.bytes)
        {
            return error::invalid;
        }
        constexpr unsigned allow_sealing = 2, cloexec = 1, noexec_seal = 8, executable = 16;
        bool code = source.role == image_role::bootstrap || source.role == image_role::runtime;
        descriptor copy{static_cast<int>(::syscall(SYS_memfd_create, "uwvm-model-image", cloexec | allow_sealing | (code ? executable : noexec_seal)))};
        if(!copy) { return error::unsupported; }
        ::fast_io::sha256_context hashing{};
        std::array<std::uint8_t, 65536> buffer{};
        std::uint64_t offset{};
        std::array<std::uint8_t, 4> magic{};
        while(offset < source.bytes)
        {
            auto count = static_cast<std::size_t>(std::min<std::uint64_t>(buffer.size(), source.bytes - offset));
            auto got = ::pread(source.source, buffer.data(), count, static_cast<off_t>(offset));
            if(got < 0 && errno == EINTR) { continue; }
            if(got <= 0) { return error::io; }
            if(!offset && got >= 4) { std::copy_n(buffer.begin(), 4, magic.begin()); }
            auto first = reinterpret_cast<std::byte const*>(buffer.data());
            hashing.update(first, first + got);
            std::size_t written{};
            while(written < static_cast<std::size_t>(got))
            {
                auto result = ::write(copy.get(), buffer.data() + written, static_cast<std::size_t>(got) - written);
                if(result < 0 && errno == EINTR) { continue; }
                if(result <= 0) { return error::io; }
                written += result;
            }
            offset += static_cast<std::uint64_t>(got);
        }
        buffers::digest digest{};
        hashing.do_final();
        hashing.digest_to_byte_ptr(reinterpret_cast<std::byte*>(digest.data()));
        if(digest != source.sha256) { return error::conflict; }
        if(code && magic != std::array<std::uint8_t, 4>{0x7f, 'E', 'L', 'F'}) { return error::invalid; }
        if(::fchmod(copy.get(), code ? 0500 : 0000)) { return error::io; }
        constexpr int seals = 1 | 2 | 4 | 8 | 32;  // SEAL_SEAL, SHRINK, GROW, WRITE, EXEC
        if(::fcntl(copy.get(), F_ADD_SEALS, seals) < 0 || (::fcntl(copy.get(), F_GET_SEALS) & seals) != seals || ::lseek(copy.get(), 0, SEEK_SET) < 0)
        {
            return error::unsupported;
        }
        output = std::move(copy);
        return error::success;
    }

    /** @brief Fixed bounded BPF assembly; unknown syscalls are denied. */
    class syscall_filter
    {
        std::array<sock_filter, 768> instructions{};
        std::size_t count{};
        bool overflow{};

        void emit(unsigned short code, unsigned char yes, unsigned char no, std::uint32_t value) noexcept
        {
            if(count < instructions.size()) { instructions[count++] = {code, yes, no, value}; }
            else { overflow = true; }
        }

        void statement(unsigned short code, std::uint32_t value) noexcept { emit(code, 0, 0, value); }

        void load(unsigned offset) noexcept { statement(BPF_LD | BPF_W | BPF_ABS, offset); }

        static constexpr std::uint32_t allow = SECCOMP_RET_ALLOW, deny = SECCOMP_RET_ERRNO | ENOSYS;

        void plain(unsigned number) noexcept
        {
            emit(BPF_JMP | BPF_JEQ | BPF_K, 0, 1, number);
            statement(BPF_RET | BPF_K, allow);
        }

        void flags(unsigned number, unsigned argument, std::uint32_t forbidden) noexcept
        {
            emit(BPF_JMP | BPF_JEQ | BPF_K, 0, 4, number);
            load(offsetof(seccomp_data, args) + argument * 8);
            emit(BPF_JMP | BPF_JSET | BPF_K, 0, 1, forbidden);
            statement(BPF_RET | BPF_K, deny);
            statement(BPF_RET | BPF_K, allow);
        }

    public:
        /** @brief Startup allows the bootstrap exec; final filter removes code creation.
         * @details The fixed bootstrap loads only approved runtime dependencies
         * before final_filter, and reads no model/IPC bytes until it succeeds.
         */
        explicit syscall_filter(bool final_filter = false) noexcept
        {
            load(offsetof(seccomp_data, arch));
# if defined(__x86_64__)
            constexpr auto arch = AUDIT_ARCH_X86_64;
# elif defined(__aarch64__)
            constexpr auto arch = AUDIT_ARCH_AARCH64;
# elif defined(__i386__)
            constexpr auto arch = AUDIT_ARCH_I386;
# elif defined(__arm__)
            constexpr auto arch = AUDIT_ARCH_ARM;
# else
            constexpr unsigned arch = 0;
# endif
            emit(BPF_JMP | BPF_JEQ | BPF_K, 1, 0, arch);
            statement(BPF_RET | BPF_K, SECCOMP_RET_KILL_PROCESS);
            load(offsetof(seccomp_data, nr));
# if defined(__x86_64__)
            emit(BPF_JMP | BPF_JSET | BPF_K, 0, 1, 0x40000000u);
            statement(BPF_RET | BPF_K, SECCOMP_RET_KILL_PROCESS);
# endif
            if(final_filter)
            {
# ifdef SYS_execve
                emit(BPF_JMP | BPF_JEQ | BPF_K, 0, 1, SYS_execve);
                statement(BPF_RET | BPF_K, deny);
# endif
# ifdef SYS_execveat
                emit(BPF_JMP | BPF_JEQ | BPF_K, 0, 1, SYS_execveat);
                statement(BPF_RET | BPF_K, deny);
# endif
# ifdef SYS_prctl
                emit(BPF_JMP | BPF_JEQ | BPF_K, 0, 1, SYS_prctl);
                statement(BPF_RET | BPF_K, deny);
# endif
# ifdef SYS_prlimit64
                emit(BPF_JMP | BPF_JEQ | BPF_K, 0, 6, SYS_prlimit64);
                load(offsetof(seccomp_data, args) + 16);
                emit(BPF_JMP | BPF_JEQ | BPF_K, 0, 2, 0);
                load(offsetof(seccomp_data, args) + 20);
                emit(BPF_JMP | BPF_JEQ | BPF_K, 1, 0, 0);
                statement(BPF_RET | BPF_K, deny);
                statement(BPF_RET | BPF_K, allow);
# endif
# ifdef SYS_mmap
#  if defined(__i386__) || defined(__arm__)
                // Legacy mmap uses a user pointer to mmap_arg_struct on these
                // ABIs. Seccomp cannot inspect its embedded protection field;
                // deny it entirely instead of treating args[2] as PROT_*.
                // mmap2 remains available with directly inspectable arguments.
                emit(BPF_JMP | BPF_JEQ | BPF_K, 0, 1, SYS_mmap);
                statement(BPF_RET | BPF_K, deny);
#  else
                flags(SYS_mmap, 2, 4);
#  endif
# endif
# ifdef SYS_mmap2
                flags(SYS_mmap2, 2, 4);
# endif
# ifdef SYS_mprotect
                flags(SYS_mprotect, 2, 4);
# endif
                statement(BPF_RET | BPF_K, allow);
                return;
            }
# ifdef SYS_open
            flags(SYS_open, 1, O_CREAT | 020000000u);
# endif
            flags(SYS_openat, 2, O_CREAT | 020000000u);
            emit(BPF_JMP | BPF_JEQ | BPF_K, 0, 6, SYS_prctl);
            load(offsetof(seccomp_data, args));
            emit(BPF_JMP | BPF_JEQ | BPF_K, 2, 0, PR_SET_NO_NEW_PRIVS);
            emit(BPF_JMP | BPF_JEQ | BPF_K, 1, 0, PR_SET_SECCOMP);
            statement(BPF_RET | BPF_K, deny);
            statement(BPF_RET | BPF_K, allow);
            load(offsetof(seccomp_data, nr));
# ifdef SYS_prlimit64
            // Startup may set only its own FSIZE; all other setters fail closed.
            emit(BPF_JMP | BPF_JEQ | BPF_K, 0, 13, SYS_prlimit64);
            load(offsetof(seccomp_data, args) + 16);
            emit(BPF_JMP | BPF_JEQ | BPF_K, 0, 3, 0);
            load(offsetof(seccomp_data, args) + 20);
            emit(BPF_JMP | BPF_JEQ | BPF_K, 0, 1, 0);
            statement(BPF_RET | BPF_K, allow);
            load(offsetof(seccomp_data, args));
            emit(BPF_JMP | BPF_JEQ | BPF_K, 0, 4, 0);
            load(offsetof(seccomp_data, args) + 8);
            emit(BPF_JMP | BPF_JEQ | BPF_K, 0, 2, RLIMIT_FSIZE);
            statement(BPF_RET | BPF_K, allow);
            statement(BPF_RET | BPF_K, deny);
            statement(BPF_RET | BPF_K, deny);
            load(offsetof(seccomp_data, nr));
# endif
# ifdef SYS_recvmsg
            emit(BPF_JMP | BPF_JEQ | BPF_K, 0, 4, SYS_recvmsg);
            load(offsetof(seccomp_data, args));
            emit(BPF_JMP | BPF_JEQ | BPF_K, 0, 1, 3);
            statement(BPF_RET | BPF_K, allow);
            statement(BPF_RET | BPF_K, deny);
# endif
# ifdef SYS_fcntl
            emit(BPF_JMP | BPF_JEQ | BPF_K, 0, 6, SYS_fcntl);
            load(offsetof(seccomp_data, args)+8);
            emit(BPF_JMP | BPF_JEQ | BPF_K, 3, 0, F_GET_SEALS);
            emit(BPF_JMP | BPF_JEQ | BPF_K, 2, 0, F_GETFL);
            emit(BPF_JMP | BPF_JEQ | BPF_K, 1, 0, F_GETFD);
            statement(BPF_RET | BPF_K, deny);
            statement(BPF_RET | BPF_K, allow);
# endif
# define UWVM_MODEL_ALLOW(name) plain(SYS_##name)
            UWVM_MODEL_ALLOW(read);
            UWVM_MODEL_ALLOW(write);
            UWVM_MODEL_ALLOW(readv);
            UWVM_MODEL_ALLOW(writev);
            UWVM_MODEL_ALLOW(pread64);
            UWVM_MODEL_ALLOW(pwrite64);
            UWVM_MODEL_ALLOW(close);
            UWVM_MODEL_ALLOW(lseek);
            UWVM_MODEL_ALLOW(fstat);
            UWVM_MODEL_ALLOW(getdents64);
# ifdef SYS_newfstatat
            UWVM_MODEL_ALLOW(newfstatat);
# endif
# ifdef SYS_fstatat64
            UWVM_MODEL_ALLOW(fstatat64);
# endif
# ifdef SYS_fstat64
            UWVM_MODEL_ALLOW(fstat64);
# endif
            UWVM_MODEL_ALLOW(mprotect);
            UWVM_MODEL_ALLOW(munmap);
            UWVM_MODEL_ALLOW(mremap);
            UWVM_MODEL_ALLOW(brk);
            UWVM_MODEL_ALLOW(madvise);
            UWVM_MODEL_ALLOW(rt_sigaction);
            UWVM_MODEL_ALLOW(rt_sigprocmask);
            UWVM_MODEL_ALLOW(rt_sigreturn);
            UWVM_MODEL_ALLOW(sigaltstack);
            UWVM_MODEL_ALLOW(futex);
            UWVM_MODEL_ALLOW(clock_gettime);
            UWVM_MODEL_ALLOW(nanosleep);
            UWVM_MODEL_ALLOW(getpid);
            UWVM_MODEL_ALLOW(gettid);
            UWVM_MODEL_ALLOW(getuid);
            UWVM_MODEL_ALLOW(geteuid);
            UWVM_MODEL_ALLOW(getgid);
            UWVM_MODEL_ALLOW(getegid);
            UWVM_MODEL_ALLOW(sched_getaffinity);
            UWVM_MODEL_ALLOW(sched_yield);
            UWVM_MODEL_ALLOW(uname);
            UWVM_MODEL_ALLOW(getrandom);
            UWVM_MODEL_ALLOW(set_tid_address);
            UWVM_MODEL_ALLOW(set_robust_list);
            UWVM_MODEL_ALLOW(exit);
            UWVM_MODEL_ALLOW(exit_group);
            UWVM_MODEL_ALLOW(execve);
# ifdef SYS_mmap
            UWVM_MODEL_ALLOW(mmap);
# endif
# ifdef SYS_mmap2
            UWVM_MODEL_ALLOW(mmap2);
# endif
# ifdef SYS_arch_prctl
            UWVM_MODEL_ALLOW(arch_prctl);
# endif
# ifdef SYS_rseq
            UWVM_MODEL_ALLOW(rseq);
# endif
# ifdef SYS_statx
            UWVM_MODEL_ALLOW(statx);
# endif
# ifdef SYS_access
            UWVM_MODEL_ALLOW(access);
# endif
# ifdef SYS_faccessat
            UWVM_MODEL_ALLOW(faccessat);
# endif
# ifdef SYS_faccessat2
            UWVM_MODEL_ALLOW(faccessat2);
# endif
# ifdef SYS_readlink
            UWVM_MODEL_ALLOW(readlink);
# endif
# ifdef SYS_readlinkat
            UWVM_MODEL_ALLOW(readlinkat);
# endif
# ifdef SYS_getrlimit
            UWVM_MODEL_ALLOW(getrlimit);
# endif
# ifdef SYS_clock_nanosleep
            UWVM_MODEL_ALLOW(clock_nanosleep);
# endif
# undef UWVM_MODEL_ALLOW
            statement(BPF_RET | BPF_K, deny);
        }

        std::span<sock_filter const> view() const noexcept { return overflow ? std::span<sock_filter const>{} : std::span<sock_filter const>{instructions.data(), count}; }

        /** @brief Install final restrictions in the approved bootstrap before untrusted data. */
        bool install() const noexcept
        {
            if(overflow || !count) { return false; }
            sock_fprog program{static_cast<unsigned short>(count), const_cast<sock_filter*>(instructions.data())};
            return !::prctl(PR_SET_NO_NEW_PRIVS, 1, 0, 0, 0) && !::prctl(PR_SET_SECCOMP, SECCOMP_MODE_FILTER, &program);
        }
    };

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
        buffers::profile model, task;
        ::pid_t owner{::getpid()}, child{};
        bool prepared{}, launched{}, launcher_authorized{}, reaped{}, weights_enabled{}, identified{};
        std::array<char,4096> status_text{};
        std::size_t status_size{};
        std::uint64_t startup_file_limit{};
        int child_status{};
        std::atomic<int> signalling{-1}, runtime_signalling{-1};
        std::atomic<bool> stop_requested{};

        bool same_process() const noexcept { return owner == ::getpid(); }

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
            syscall_filter policy;
            auto bytes = std::as_bytes(policy.view());
            descriptor policy_fd{static_cast<int>(::syscall(SYS_memfd_create, "uwvm-model-seccomp", MFD_CLOEXEC | MFD_ALLOW_SEALING | 8u))};
            descriptor scratch_fd{static_cast<int>(::syscall(SYS_memfd_create, "uwvm-model-empty", MFD_CLOEXEC | MFD_ALLOW_SEALING | 8u))};
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
                if(file.role == image_role::weights) { continue; }
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
            // Only runtime descriptors are inherited. Weight descriptors are
            // delivered later over the dedicated receive-only control socket.
            // stdin/out/err=0..2, control=3, launcher=4, code=5.., filter,
            // empty scratch, JSON identity output, namespace release input.
            std::size_t code_count{};
            startup_file_limit = selected.bound.file_bytes;
            for(auto const& file: selected.files)
            {
                if(file.role == image_role::weights) { continue; }
                pair("--perms", "0500");
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
            pair("--remount-ro", "/");
            pair("--chdir", "/worker");
            pair("--seccomp", ::uwvm2::utils::container::to_string(code_count + 5));
            pair("--info-fd", ::uwvm2::utils::container::to_string(code_count + 6 + selected.bound.scratch_slots));
            pair("--block-fd", ::uwvm2::utils::container::to_string(code_count + 7 + selected.bound.scratch_slots));
            argument("--");
            argument("/worker/bootstrap");
            argument(::uwvm2::utils::container::to_string(selected.bound.request_bytes));
            argument(::uwvm2::utils::container::to_string(selected.bound.response_bytes));
            argument(::uwvm2::utils::container::to_string(selected.bound.file_bytes));
            launcher = std::move(approved);
            images = std::move(staged);
            filter = std::move(policy_fd);
            empty_file = std::move(scratch_fd);
            bounds = selected.bound;
            model = selected.model;task=selected.task;
            placement=std::move(chosen);
            prepared = true;
            return error::success;
        }

        /** @brief Spawn only the prepared launcher, with no inherited host descriptors/env.
         * @details The post-fork child uses only descriptor/resource syscalls and
         * exec. Every allocating argument/descriptor preparation precedes fork.
         * Process limits are hard limits; the existing 64 GiB cgroup is retained.
         * Failure can still own a created child: was_launched() distinguishes it
         * from pre-spawn refusal, and the supervisor must reap that child. All
         * error returns precede launch authorization; the one-use socket gate
         * rejects on EOF. No failed launch waits while holding an authority lock.
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
            if(!pipe(in_read, in_write) || !pipe(out_read, out_write) || !pipe(err_read, err_write) || !pipe(status_read,status_write)) { return error::io; }
            // A dead receiver must return an error without raising SIGPIPE
            // in a host thread that did not opt into a signal-mask policy.
            int releases[2];
            if(::socketpair(AF_UNIX,SOCK_SEQPACKET|SOCK_CLOEXEC,0,releases))return error::io;
            release_read.reset(releases[0]);release_write.reset(releases[1]);
            if(::shutdown(release_read.get(),SHUT_WR)||::shutdown(release_write.get(),SHUT_RD))return error::io;
            // A rejected launch must wake the child even without a usable
            // pidfd. shutdown() also closes inherited duplicates' write side.
            int gates[2];
            if(::socketpair(AF_UNIX, SOCK_SEQPACKET | SOCK_CLOEXEC, 0, gates)) { return error::io; }
            descriptor gate_host{gates[0]}, gate_child{gates[1]};
            // These endpoints are distinct from the child's open descriptions.
            // Perform every fallible descriptor setup before creating a process.
            if(!nonblocking(in_write.get()) || !nonblocking(out_read.get()) || !nonblocking(err_read.get()) ||
               !nonblocking(host_control.get()) || !nonblocking(status_read.get())) { return error::io; }
            ::uwvm2::utils::container::std_vector<int> sources;
            sources.reserve(images.size() + 8 + bounds.scratch_slots);
            sources.push_back(in_read.get());
            sources.push_back(out_write.get());
            sources.push_back(err_write.get());
            sources.push_back(worker_control.get());
            sources.push_back(launcher.get());
            for(auto const& file: images) { if(file.role != image_role::weights) { sources.push_back(file.file.get()); } }
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
            launched = true;
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

        /** @brief Deliver immutable NX weight descriptors after verified bootstrap readiness.
         * @details Weights are never inherited by bubblewrap or present in its
         * filesystem. The fixed control socket has no receive authority from
         * any other peer; sandbox socket/connect/sendmsg syscalls remain denied.
         */
        error enable_weights()
        {
            if(!same_process()) { return error::denied; }
            if(stop_requested.load(std::memory_order_acquire)) { return error::cancelled; }
            if(!identified || reaped || weights_enabled) { return error::conflict; }
            std::array<std::uint8_t,20000> payload{};
            std::array<int,64> descriptors{};
            std::size_t number{};
            for(auto const& image:images) { if(image.role == image_role::weights) { descriptors[number++]=image.file.get(); } }
            wire::writer writer{payload};
            if(!writer.u64(number)) { return error::invalid; }
            for(auto const& image:images)
            {
                if(image.role != image_role::weights) { continue; }
                struct stat info{};
                if(::fstat(image.file.get(),&info) || !writer.text(image.destination.view()) || !writer.u64(static_cast<std::uint64_t>(info.st_size))) { return error::io; }
            }
            alignas(cmsghdr) std::array<std::uint8_t,CMSG_SPACE(sizeof(int)*64)> ancillary{};
            iovec vector{payload.data(),writer.size()};
            msghdr message{};message.msg_iov=&vector;message.msg_iovlen=1;
            if(number)
            {
                message.msg_control=ancillary.data();message.msg_controllen=CMSG_SPACE(sizeof(int)*number);
                auto header=CMSG_FIRSTHDR(&message);header->cmsg_level=SOL_SOCKET;header->cmsg_type=SCM_RIGHTS;header->cmsg_len=CMSG_LEN(sizeof(int)*number);
                std::memcpy(CMSG_DATA(header),descriptors.data(),sizeof(int)*number);
            }
            auto sent=::sendmsg(control.get(),&message,MSG_DONTWAIT|MSG_NOSIGNAL);
            if(sent<0) { return errno==EAGAIN || errno==EINTR ? error::again : error::io; }
            if(static_cast<std::size_t>(sent)!=writer.size()) { return error::io; }
            weights_enabled=true;control.reset();
            return error::success;
        }

        int input_fd() const noexcept { return same_process() ? input.get() : -1; }

        int output_fd() const noexcept { return same_process() ? output.get() : -1; }

        int stderr_fd() const noexcept { return same_process() ? diagnostics.get() : -1; }

        int process_fd() const noexcept { return same_process() ? pidfd.get() : -1; }

        /** @brief Whether this owner created a child, even if launch later failed.
         * @details Only the launch/reap supervisor reads this flag. It is not a
         * readiness or liveness claim; successful reap separately confirms exit.
         * A failed launch must retain cleanup ownership until that observation.
         */
        bool was_launched() const noexcept { return same_process() && launched; }

        /** @brief Confirm that the born namespace init has actually been reaped.
         * @details Kernel teardown of this init includes all descendants in
         * nested PID namespaces. No startup report, receipt, timeout or successful
         * signal can set this flag. Even rejected map/exec/gate paths retain the
         * direct-child ownership until reap. Physical effects remain separate.
         */
        bool cleanup_confirmed() const noexcept
        {return same_process() && (!launched || reaped);}

        limits const& reservations() const noexcept { return bounds; }

        /** @brief Latch cancellation before signalling only retained process identities.
         * @details Cancellation before launch or pidfd publication remains in
         * force. A later startup cannot lose the stop or disclose model data.
         * This does not wait and an old pidfd cannot target a reused numeric PID.
         */
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
}  // namespace uwvm2::imported::ros::scene::model_sandbox
