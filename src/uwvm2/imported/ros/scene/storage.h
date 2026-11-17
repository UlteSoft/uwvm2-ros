/** @file
 * @brief Purpose-bound immutable inputs and transactional robot artifacts.
 * @details This is a trusted storage backend, not a guest authorization layer.
 * The caller must hold an independently admitted scene authority lease for
 * every operation. No guest path, directory FD, file FD, link, seek, truncate,
 * allocation or filesystem-management operation is exposed by this backend.
 *
 * Linux output stores use a private, initially empty directory. A store is
 * shared by directory device/inode identity, including aliases in different
 * policy environments. Logical capacity includes staged reservations and
 * committed bytes; file capacity includes every staged and committed object.
 * Closing an application capability does not change either account.
 *
 * Logical capacity and file counts are NOT a physical-block hard quota.
 * Deployment must use a separately enforced project/volume quota when a hard
 * disk bound is required. The free-space headroom check is advisory against
 * unrelated host writers. Nonempty-store restart is rejected until an audited
 * recovery manifest is implemented. No absent guarantee is reported as success.
 */
#pragma once
#include <uwvm2/utils/container/shared.h>
#include <uwvm2/utils/allocator/fast_io_strict/object.h>
#include <uwvm2/utils/container/standard.h>
#include "authority.h"
#include <algorithm>
#include <array>
#include <atomic>
#include <cerrno>
#include <cstddef>
#include <cstdint>
#include <limits>
#include <map>
#include <memory>
#include <mutex>
#include <span>
#include <string>
#include <utility>
#include <vector>
#if defined(__linux__)
#include <dirent.h>
#include <fcntl.h>
#include <linux/fs.h>
#include <linux/memfd.h>
#include <sys/file.h>
#include <sys/stat.h>
#include <sys/statvfs.h>
#include <sys/syscall.h>
#include <sys/types.h>
#include <unistd.h>
#endif

namespace uwvm2::imported::ros::scene::storage
{
    class immutable_input;
    using result=std::uint32_t;
    using object_id=std::uint64_t;
    inline constexpr result success=0,io_error=5,bad_handle=9,again=11,denied=13,
        conflict=17,invalid=22,no_space=28,too_large=90,unsupported=95;
    /** @brief Trusted principal identifying one environment and grant lifetime.
     * @details Environment IDs must be monotonic trusted identities, never
     * guest-supplied numbers or reusable group-name hashes. Two deployments
     * using the same textual alias remain different principals. Reissuing a
     * revoked grant requires a new environment ID or a separate grant identity.
     */
    struct principal
    {
        std::uint64_t environment{};
        ::uwvm2::utils::container::std_string grant;
        friend bool operator==(principal const&,principal const&)=default;
    };
    /** @brief Explicit finite store reservations and transfer bounds.
     * @details Zero is a real zero allowance. UINT64_MAX is rejected rather
     * than treated as an unlimited disk grant. `logical_bytes` is the sum of
     * staged maximum sizes and committed sizes. `objects` includes staged and
     * committed files, so empty-file floods remain bounded. A transfer must
     * fit completely before any byte is written; kernel short writes are
     * reported through their exact accepted byte count.
     */
    struct limits
    {
        std::uint64_t logical_bytes{},objects{},object_bytes{},transfer_bytes{},headroom_bytes{};
        friend bool operator==(limits const&,limits const&)=default;
    };
    enum class phase : std::uint8_t { staged, committed, integrity_failure };
    /** @brief Detached metadata; no file name, descriptor or content is exposed. */
    struct object_snapshot
    {
        object_id id{};principal owner;purpose use{};phase state{};
        std::uint64_t bytes{},reserved{};bool durable{};
    };
    /** @brief Store accounting that remains charged without an application handle. */
    struct usage
    {std::uint64_t reserved_bytes{},objects{},staged{},committed{};};
    /** @brief Distinguish publication from crash durability on commit failure.
     * @details If directory fsync fails after atomic publication, `published`
     * remains true and `durable` is false. The caller must not retry by creating
     * a second object; inspect or retry commit on this exact object instead.
     */
    struct publication {bool published{},durable{};};

    inline bool input_purpose(purpose value) noexcept
    {
        return value==purpose::package||value==purpose::configuration||value==purpose::calibration||
               value==purpose::map||value==purpose::model_weights;
    }
    inline bool output_purpose(purpose value) noexcept
    {
        return value==purpose::calibration||value==purpose::map||value==purpose::log||
               value==purpose::recording||value==purpose::cache;
    }
    inline bool valid(principal const& value) noexcept
    {return value.environment&&scene::identifier(value.grant);}
    inline bool valid(limits const& value) noexcept
    {
        return value.logical_bytes<=static_cast<std::uint64_t>(INT64_MAX)&&
               value.object_bytes<=value.logical_bytes&&value.objects!=UINT64_MAX&&
               value.transfer_bytes<=static_cast<std::uint64_t>(INT64_MAX)&&value.headroom_bytes!=UINT64_MAX;
    }

#if defined(__linux__)
    namespace detail
    {
        /** @brief Exact native file/transfer extents accepted by this build.
         * @details Linux may expose 32-bit off_t or ssize_t even though the
         * scene record uses 64-bit lengths. Never narrow those lengths before
         * validation. Large-file feature macros must be selected consistently
         * by the build before any system header, not changed in this header.
         * Aggregate store bytes can exceed one file's extent; each object and
         * each syscall transfer must still fit its corresponding native type.
         */
        static_assert(std::numeric_limits<::off_t>::is_integer&&std::numeric_limits<::off_t>::is_signed);
        static_assert(std::numeric_limits<::ssize_t>::is_integer&&std::numeric_limits<::ssize_t>::is_signed);
        inline constexpr std::uint64_t offset_limit=std::numeric_limits<::off_t>::digits>=63?
            static_cast<std::uint64_t>(INT64_MAX):static_cast<std::uint64_t>(std::numeric_limits<::off_t>::max());
        inline constexpr std::uint64_t transfer_limit=std::numeric_limits<::ssize_t>::digits>=63?
            static_cast<std::uint64_t>(INT64_MAX):static_cast<std::uint64_t>(std::numeric_limits<::ssize_t>::max());
        /** @brief Validate the complete file extent before a native offset cast.
         * @details The subtraction form rejects overflowing offset+length.
         * The end is a representable file size, including a zero-byte EOF read.
         */
        inline bool native_range(std::uint64_t offset,std::uint64_t length) noexcept
        {return offset<=offset_limit&&length<=transfer_limit&&length<=offset_limit-offset;}
        struct descriptor
        {
            int value{-1};
            descriptor()=default;explicit descriptor(int fd) noexcept:value(fd){}
            descriptor(descriptor const&)=delete;descriptor& operator=(descriptor const&)=delete;
            descriptor(descriptor&& other) noexcept:value(std::exchange(other.value,-1)){}
            descriptor& operator=(descriptor&& other) noexcept
            {if(this!=&other){reset();value=std::exchange(other.value,-1);}return *this;}
            ~descriptor(){reset();}
            void reset() noexcept{if(value>=0){::close(value);value=-1;}}
        };
        inline result last_error() noexcept{return errno>0?static_cast<result>(errno):io_error;}
        inline bool same_file(struct ::stat const& a,struct ::stat const& b) noexcept
        {return a.st_dev==b.st_dev&&a.st_ino==b.st_ino;}
        inline ::uwvm2::utils::container::std_string name(char const* prefix,object_id id)
        {
            ::uwvm2::utils::container::std_string value(prefix);constexpr char digits[]="0123456789abcdef";
            for(unsigned shift=64;shift;){shift-=4;value.push_back(digits[(id>>shift)&15u]);}
            return value;
        }
        inline bool empty_directory(int descriptor) noexcept
        {
            int copy=::openat(descriptor,".",O_RDONLY|O_CLOEXEC|O_DIRECTORY|O_NOFOLLOW);
            if(copy<0)return false;
            DIR* stream=::fdopendir(copy);if(!stream){::close(copy);return false;}
            bool empty=true;
            for(;;)
            {
                errno=0;auto entry=::readdir(stream);
                if(!entry){if(errno)empty=false;break;}
                if(std::string_view(entry->d_name)!="."&&std::string_view(entry->d_name)!="..")
                {empty=false;break;}
            }
            ::closedir(stream);return empty;
        }
    }

    /** @brief Immutable input copied by a trusted provider into a sealed memfd.
     * @details Only regular files with WRITE/GROW/SHRINK/SEAL kernel seals are
     * accepted. Ordinary read-only descriptors do not imply immutable content
     * and are refused. The launcher must verify expected digest/type before
     * binding this object to a guest grant. `create` duplicates the FD; reads
     * use pread and cannot alter another reader's file offset. Fork-inherited
     * objects are denied rather than sharing an untracked registry lifetime.
     */
    class immutable_input
    {
        detail::descriptor fd;
        principal owner;
        purpose use{};
        std::uint64_t length{},transfer{};
        ::pid_t process{::getpid()};
        immutable_input()=default;
    public:
        immutable_input(immutable_input const&)=delete;
        immutable_input& operator=(immutable_input const&)=delete;
        /** @brief Admit a trusted sealed input without granting any guest access.
         * @details Limits outside this build's off_t/ssize_t extent return
         * too_large before duplicating the supplied FD. They are never clipped
         * or narrowed. The same configuration can require a large-file build
         * on a 32-bit host; an unavailable guarantee remains an admission error.
         */
        static result create(int supplied,principal who,purpose use,std::uint64_t maximum_bytes,
                             std::uint64_t transfer_bytes,std::shared_ptr<immutable_input>& output)
        {
            if(!valid(who)||!input_purpose(use)||maximum_bytes>static_cast<std::uint64_t>(INT64_MAX)||
               transfer_bytes>static_cast<std::uint64_t>(INT64_MAX))return invalid;
            if(maximum_bytes>detail::offset_limit||transfer_bytes>detail::transfer_limit)return too_large;
            detail::descriptor copy{::fcntl(supplied,F_DUPFD_CLOEXEC,0)};
            if(copy.value<0)return detail::last_error();
            struct ::stat status{};if(::fstat(copy.value,&status))return detail::last_error();
            auto flags=::fcntl(copy.value,F_GETFL);if(flags<0)return detail::last_error();
            if(!S_ISREG(status.st_mode)||(flags&O_ACCMODE)==O_WRONLY||status.st_size<0)return denied;
            int seals=::fcntl(copy.value,F_GET_SEALS);
            constexpr int required=F_SEAL_WRITE|F_SEAL_GROW|F_SEAL_SHRINK|F_SEAL_SEAL;
            if(seals<0)return unsupported;
            if((seals&required)!=required)return denied;
            if(static_cast<std::uint64_t>(status.st_size)>maximum_bytes)return too_large;
            auto next=std::shared_ptr<immutable_input>{::uwvm2::utils::container::adopt_shared(::uwvm2::utils::allocator::fast_io_strict::object<immutable_input>([&](immutable_input* uwvm_allocated) { return ::new(static_cast<void*>(uwvm_allocated)) immutable_input; }))};
            next->fd=std::move(copy);next->owner=std::move(who);next->use=use;
            next->length=static_cast<std::uint64_t>(status.st_size);next->transfer=transfer_bytes;
            output=std::move(next);return success;
        }
        /** @brief Read an exact bounded range after the caller authorizes artifact.read.
         * @details Owner mismatch, overflow and excessive transfer fail without
         * copying data. A final short read is reported with its actual size;
         * offsets past EOF are rejected and offsets at EOF return zero bytes.
         */
        result read(principal const& who,std::uint64_t offset,std::span<std::byte> output,std::size_t& count) const noexcept
        {
            count=0;if(process!=::getpid()||who!=owner)return denied;
            if(offset>length||output.size()>transfer)return too_large;
            auto requested=static_cast<std::size_t>(std::min<std::uint64_t>(output.size(),length-offset));
            if(!detail::native_range(offset,requested))return too_large;
            if(!requested)return success;
            auto done=::pread(fd.value,output.data(),requested,static_cast<::off_t>(offset));
            if(done<0)return detail::last_error();
            count=static_cast<std::size_t>(done);return success;
        }
        /** @brief Inspect size and purpose without exposing data or native identities. */
        result describe(principal const& who,std::uint64_t& bytes,purpose& kind) const noexcept
        {if(process!=::getpid()||who!=owner)return denied;bytes=length;kind=use;return success;}
    };

    /** @brief Shared bounded artifact store over one deployment-owned directory.
     * @details The directory must be owned by the effective UID, inaccessible
     * to group/others, initially empty, and exclusively managed by the trusted
     * broker. An advisory flock excludes another cooperative process; a static
     * device/inode registry prevents duplicate quota accounts in this process.
     * Untrusted same-UID writers are outside this filesystem threat boundary.
     * All operations are serialized; no provider callback runs under this lock.
     */
    class store : public std::enable_shared_from_this<store>
    {
        struct object
        {
            object_snapshot data;
            detail::descriptor fd;
            struct ::stat identity{};
            ::uwvm2::utils::container::std_string staged_name,published_name;
        };
        detail::descriptor root;
        limits bounds;
        ::pid_t process{::getpid()};
        mutable std::mutex mutex;
        ::uwvm2::utils::container::std_map<object_id,object> objects;
        std::uint64_t reserved{};
        bool retired{};
        inline static ::pid_t const registry_process{::getpid()};
        inline static std::mutex registry_mutex;
        inline static ::uwvm2::utils::container::std_map<std::pair<::dev_t,::ino_t>,std::weak_ptr<store>> registry;
        inline static std::atomic<object_id> next_id{1};
        store()=default;
        static object_id allocate() noexcept
        {
            auto value=next_id.load(std::memory_order_relaxed);
            while(value&&value!=UINT64_MAX)
                if(next_id.compare_exchange_weak(value,value+1,std::memory_order_relaxed))return value;
            return 0;
        }
        bool live() const noexcept{return !retired&&process==::getpid();}
        object* find(principal const& who,object_id id) noexcept
        {
            auto it=objects.find(id);
            return live()&&it!=objects.end()&&it->second.data.owner==who?&it->second:nullptr;
        }
        result check_identity(object& value) noexcept
        {
            if(value.data.state==phase::integrity_failure)return denied;
            struct ::stat descriptor{},named{};
            auto const& name=value.data.state==phase::staged?value.staged_name:value.published_name;
            if(::fstat(value.fd.value,&descriptor)||::fstatat(root.value,name.c_str(),&named,AT_SYMLINK_NOFOLLOW))return detail::last_error();
            if(!S_ISREG(descriptor.st_mode)||descriptor.st_nlink!=1||!detail::same_file(value.identity,descriptor)||
               !detail::same_file(descriptor,named)||descriptor.st_size<0||
               static_cast<std::uint64_t>(descriptor.st_size)!=value.data.bytes)
            {value.data.state=phase::integrity_failure;return denied;}
            return success;
        }
        result headroom(std::uint64_t additional) const noexcept
        {
            struct ::statvfs volume{};if(::fstatvfs(root.value,&volume))return detail::last_error();
            auto block=static_cast<std::uint64_t>(volume.f_frsize);
            if(!block)return io_error;
            auto available=static_cast<std::uint64_t>(volume.f_bavail);
            available=available>UINT64_MAX/block?UINT64_MAX:available*block;
            std::uint64_t unwritten=additional;
            for(auto const& [id,value]:objects)
            {
                auto pending=value.data.reserved-value.data.bytes;
                if(pending>UINT64_MAX-unwritten)return no_space;
                unwritten+=pending;
            }
            if(unwritten>available||bounds.headroom_bytes>available-unwritten)return no_space;
            return success;
        }
        result erase(principal const& who,object_id id,bool committed) noexcept
        {
            auto value=find(who,id);if(!value)return bad_handle;
            if(value->data.state!=(committed?phase::committed:phase::staged))return denied;
            if(auto e=check_identity(*value))return e;
            auto const& name=committed?value->published_name:value->staged_name;
            if(::unlinkat(root.value,name.c_str(),0))return detail::last_error();
            // Linux close consumes the descriptor even on EINTR/EIO. Never
            // retry close against a potentially reused native descriptor.
            int fd=std::exchange(value->fd.value,-1);int closed=::close(fd);
            result status=closed?detail::last_error():success;
            reserved-=value->data.reserved;objects.erase(id);
            if(::fsync(root.value)&&!status)status=detail::last_error();
            return status;
        }
    public:
        store(store const&)=delete;store& operator=(store const&)=delete;
        /** @brief Close private descriptors; committed data and its root survive.
         * @details Destruction does not claim successful deletion. Staged files
         * are best-effort unlinked. Any leftover makes subsequent empty-store
         * admission fail, preventing a reset of quota accounting over old data.
         */
        ~store()
        {
            if(process!=::getpid())return;
            for(auto& [id,value]:objects)
            {
                struct ::stat named{};
                if(value.data.state==phase::staged&&
                   !::fstatat(root.value,value.staged_name.c_str(),&named,AT_SYMLINK_NOFOLLOW)&&
                   detail::same_file(value.identity,named))
                    (void)::unlinkat(root.value,value.staged_name.c_str(),0);
            }
        }
        /** @brief Bind one trusted private directory and exact finite limits.
         * @details Repeated aliases to the same live inode return the same
         * account only when limits match exactly. No recursive scan, adoption
         * of preexisting files, path creation or policy mutation occurs.
         * Per-object and transfer limits must fit this build's off_t/ssize_t;
         * too_large is returned before opening the directory when they do not.
         */
        static result create(int supplied,limits requested,std::shared_ptr<store>& output)
        {
            // Do not touch inherited std::mutex state after a multithreaded
            // fork: its owning thread may not exist in the child.
            if(registry_process!=::getpid())return conflict;
            if(!valid(requested))return invalid;
            if(requested.object_bytes>detail::offset_limit||requested.transfer_bytes>detail::transfer_limit)return too_large;
            detail::descriptor candidate{::openat(supplied,".",O_RDONLY|O_DIRECTORY|O_NOFOLLOW|O_CLOEXEC)};
            if(candidate.value<0)return detail::last_error();
            struct ::stat identity{};if(::fstat(candidate.value,&identity))return detail::last_error();
            if(!S_ISDIR(identity.st_mode)||identity.st_uid!=::geteuid()||(identity.st_mode&0077))return denied;
            std::lock_guard lock{registry_mutex};auto key=std::pair{identity.st_dev,identity.st_ino};
            auto found=registry.find(key);
            if(found!=registry.end())if(auto existing=found->second.lock())
            {
                std::lock_guard existing_lock{existing->mutex};
                if(existing->process!=::getpid()||existing->bounds!=requested||existing->retired)return conflict;
                output=std::move(existing);return success;
            }
            if(::flock(candidate.value,LOCK_EX|LOCK_NB))return detail::last_error();
            if(!detail::empty_directory(candidate.value))return conflict;
            auto value=std::shared_ptr<store>{::uwvm2::utils::container::adopt_shared(::uwvm2::utils::allocator::fast_io_strict::object<store>([&](store* uwvm_allocated) { return ::new(static_cast<void*>(uwvm_allocated)) store; }))};value->root=std::move(candidate);value->bounds=requested;
            registry[key]=value;output=std::move(value);return success;
        }
        /** @brief Reserve a whole artifact and create a private staging file.
         * @details Admission reserves bytes and one file before publication of
         * its ID. No sparse offsets or later capacity growth are allowed. The
         * caller must compensate failed guest-result delivery with abort; a
         * failed abort retains its reservation and is observable by inspection.
         */
        result begin(principal who,purpose use,std::uint64_t maximum_bytes,object_id& output)
        {
            if(process!=::getpid()||!valid(who)||!output_purpose(use))return denied;
            std::lock_guard lock{mutex};if(!live())return denied;
            if(maximum_bytes>bounds.object_bytes||maximum_bytes>bounds.logical_bytes-reserved||objects.size()>=bounds.objects)return again;
            if(auto e=headroom(maximum_bytes))return e;
            auto id=allocate();if(!id)return too_large;
            object pending;pending.data={id,std::move(who),use,phase::staged,0,maximum_bytes,false};
            pending.staged_name=detail::name("staged-",id);pending.published_name=detail::name("artifact-",id);
            // Allocate all registry/name memory BEFORE filesystem creation.
            auto [slot,inserted]=objects.emplace(id,std::move(pending));if(!inserted)return conflict;
            auto& value=slot->second;
            int fd=::openat(root.value,value.staged_name.c_str(),O_CREAT|O_EXCL|O_NOFOLLOW|O_CLOEXEC|O_RDWR,0600);
            if(fd<0){auto e=detail::last_error();objects.erase(slot);return e;}
            value.fd=detail::descriptor{fd};reserved+=maximum_bytes;
            if(::fstat(fd,&value.identity))
            {
                auto e=detail::last_error();
                // A created-but-uninspectable file stays charged and cannot
                // be mistaken for an uncreated object after a failure.
                value.data.state=phase::integrity_failure;return e;
            }
            output=id;return success;
        }
        /** @brief Append a bounded contiguous chunk to an owned staged artifact.
         * @details Offset is derived from the trusted accepted-byte count.
         * The whole supplied chunk must fit the reservation before pwrite.
         * A short write returns success with its actual count; an error returns
         * zero count. Committed objects, wrong principals and integrity drift
         * are rejected without further writes. No append retry is implicit.
         */
        result append(principal const& who,object_id id,std::span<std::byte const> bytes,std::size_t& written) noexcept
        {
            written=0;if(process!=::getpid())return denied;std::lock_guard lock{mutex};auto value=find(who,id);if(!value)return bad_handle;
            if(value->data.state!=phase::staged)return denied;
            if(bytes.size()>bounds.transfer_bytes||bytes.size()>value->data.reserved-value->data.bytes)return too_large;
            if(!detail::native_range(value->data.bytes,bytes.size()))return too_large;
            if(auto e=check_identity(*value))return e;
            if(auto e=headroom(0))return e;
            if(bytes.empty())return success;
            auto count=::pwrite(value->fd.value,bytes.data(),bytes.size(),static_cast<::off_t>(value->data.bytes));
            if(count<0)return detail::last_error();
            written=static_cast<std::size_t>(count);value->data.bytes+=written;return success;
        }
        /** @brief Publish an owned artifact atomically and attempt crash durability.
         * @details The caller performs its trusted content/schema validation
         * before commit while holding the authority lease. Publication uses
         * renameat2(RENAME_NOREPLACE), never overwrite. No further append is
         * admitted after rename succeeds, even if directory fsync fails.
         * Retrying this exact committed object only retries directory fsync.
         */
        result commit(principal const& who,object_id id,publication& output) noexcept
        {
            output={};if(process!=::getpid())return denied;std::lock_guard lock{mutex};auto value=find(who,id);if(!value)return bad_handle;
            if(auto e=check_identity(*value))return e;
            if(value->data.state==phase::staged)
            {
                if(::fsync(value->fd.value))return detail::last_error();
                if(::syscall(SYS_renameat2,root.value,value->staged_name.c_str(),root.value,value->published_name.c_str(),RENAME_NOREPLACE))
                    return detail::last_error();
                value->data.state=phase::committed;
                reserved-=value->data.reserved-value->data.bytes;value->data.reserved=value->data.bytes;
            }
            output.published=true;
            if(::fsync(root.value))return detail::last_error();
            value->data.durable=true;output.durable=true;return success;
        }
        /** @brief Read only committed data from an owned artifact.
         * @details This backend does not expose staged content to ordinary
         * readers. A trusted validator may use `validate` instead. Reads never
         * consume data and cannot change a stored artifact's capacity account.
         */
        result read(principal const& who,object_id id,std::uint64_t offset,std::span<std::byte> output,std::size_t& count) noexcept
        {
            count=0;if(process!=::getpid())return denied;std::lock_guard lock{mutex};auto value=find(who,id);if(!value)return bad_handle;
            if(value->data.state!=phase::committed)return denied;
            if(offset>value->data.bytes||output.size()>bounds.transfer_bytes)return too_large;
            if(auto e=check_identity(*value))return e;
            auto requested=static_cast<std::size_t>(std::min<std::uint64_t>(output.size(),value->data.bytes-offset));
            if(!detail::native_range(offset,requested))return too_large;
            if(!requested)return success;
            auto done=::pread(value->fd.value,output.data(),requested,static_cast<::off_t>(offset));
            if(done<0)return detail::last_error();
            count=static_cast<std::size_t>(done);return success;
        }
        /** @brief Allow trusted validation to read staged bytes without exporting an FD.
         * @details This is not a guest import and must not be bound to a plain
         * artifact.read grant. It provides bounded chunks to a trusted codec
         * using the same owner and integrity checks as the mutation path.
         */
        result validate(principal const& who,object_id id,std::uint64_t offset,std::span<std::byte> output,std::size_t& count) noexcept
        {
            count=0;if(process!=::getpid())return denied;std::lock_guard lock{mutex};auto value=find(who,id);if(!value)return bad_handle;
            if(value->data.state!=phase::staged)return denied;
            if(offset>value->data.bytes||output.size()>bounds.transfer_bytes)return too_large;
            if(auto e=check_identity(*value))return e;
            auto requested=static_cast<std::size_t>(std::min<std::uint64_t>(output.size(),value->data.bytes-offset));
            if(!detail::native_range(offset,requested))return too_large;
            if(!requested)return success;
            auto done=::pread(value->fd.value,output.data(),requested,static_cast<::off_t>(offset));
            if(done<0)return detail::last_error();
            count=static_cast<std::size_t>(done);return success;
        }
        /** @brief Abort a staged object; unlink failure keeps its complete reservation. */
        result abort(principal const& who,object_id id) noexcept
        {if(process!=::getpid())return denied;std::lock_guard lock{mutex};return erase(who,id,false);}
        /** @brief Remove a committed object only under separate removal authority. */
        result remove(principal const& who,object_id id) noexcept
        {if(process!=::getpid())return denied;std::lock_guard lock{mutex};return erase(who,id,true);}
        /** @brief Read one owned metadata record; this does not grant content access. */
        result describe(principal const& who,object_id id,object_snapshot& output)
        {if(process!=::getpid())return denied;std::lock_guard lock{mutex};auto value=find(who,id);if(!value)return bad_handle;output=value->data;return success;}
        /** @brief Read bounded store accounting without touching the filesystem. */
        usage accounting() const noexcept
        {
            if(process!=::getpid())return {};
            std::lock_guard lock{mutex};usage value{reserved,objects.size(),0,0};
            for(auto const& [id,entry]:objects)
            {if(entry.data.state==phase::staged)++value.staged;else if(entry.data.state==phase::committed)++value.committed;}
            return value;
        }
        /** @brief Detach bounded metadata for trusted debugging; never read content. */
        bool inspect(::uwvm2::utils::container::std_vector<object_snapshot>& output,std::size_t maximum) const
        {
            if(process!=::getpid())return false;
            std::unique_lock lock{mutex,std::try_to_lock};if(!lock.owns_lock()||objects.size()>maximum)return false;
            ::uwvm2::utils::container::std_vector<object_snapshot> rows;rows.reserve(objects.size());
            for(auto const& [id,value]:objects)rows.push_back(value.data);
            output=std::move(rows);return true;
        }
        /** @brief Retire the store without resetting capacity or deleting published artifacts.
         * @details New operations fail. The trusted lifecycle owner retains
         * this object and its accounting until cleanup/recovery is resolved.
         * Retirement is intentionally not a guest operation or a quota reset.
         */
        void retire() noexcept{if(process!=::getpid())return;std::lock_guard lock{mutex};retired=true;}
    };
#endif
}
