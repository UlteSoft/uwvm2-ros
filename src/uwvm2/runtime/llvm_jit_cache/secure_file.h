/*************************************************************
 * UlteSoft WebAssembly Virtual Machine (Version 2)          *
 * Copyright (c) 2025-present UlteSoft. All rights reserved. *
 * Licensed under the APL-2.0 License (see LICENSE file).    *
 *************************************************************/
#pragma once
#ifndef UWVM_MODULE
# include "self_executable.h"
#endif
#ifndef UWVM_MODULE_EXPORT
# define UWVM_MODULE_EXPORT
#endif

#if defined(__linux__) && (defined(__GNUC__) || defined(__clang__))
# pragma GCC visibility push(hidden)
#endif

UWVM_MODULE_EXPORT namespace uwvm2::runtime::llvm_jit_cache::secure_file
{
#if defined(__linux__)
    using ::uwvm2::runtime::llvm_jit_cache::self_executable::owned_fd;

    /// @brief Non-secret diagnostics for the calling thread's most recent file operation.
    /// @details These observations never grant cache admission. They explain a
    /// failed check without exposing cached code, key material or guest data.
    struct failure_info
    {
        ::std::u8string_view operation{u8"none"};
        int system_error{};
    };
    inline thread_local failure_info last_failure{};

    inline void record_failure(::std::u8string_view operation, int system_error = 0) noexcept
    { last_failure = {operation, system_error}; }

    [[nodiscard]] inline bool trusted_directory(int fd, bool private_root) noexcept
    {
        struct ::stat info{};
        if(::fstat(fd, &info) != 0) { record_failure(u8"directory-stat", errno); return false; }
        if(!S_ISDIR(info.st_mode)) { record_failure(u8"not-a-directory"); return false; }
        auto const uid{::geteuid()};
        if((private_root && info.st_uid != uid) || (!private_root && info.st_uid != uid && info.st_uid != 0))
        { record_failure(u8"untrusted-directory-owner"); return false; }
        // Root-owned sticky /tmp may be an ancestor, never the private root.
        if((info.st_mode & 0022) != 0 && (private_root || info.st_uid != 0 || (info.st_mode & S_ISVTX) == 0))
        { record_failure(u8"directory-writable-by-group-or-other"); return false; }
        return true;
    }

    [[nodiscard]] inline bool component_name(::std::string_view value, char (&out)[256]) noexcept
    {
        if(value.empty() || value.size() > 255uz || value == "." || value == ".." ||
           value.find('/') != ::std::string_view::npos || value.find('\0') != ::std::string_view::npos)
        { record_failure(u8"invalid-path-component"); return false; }
        ::std::memcpy(out, value.data(), value.size());
        out[value.size()] = 0;
        return true;
    }

    [[nodiscard]] inline owned_fd child_directory(int parent, ::std::string_view component, bool create, bool private_root) noexcept
    {
        char name[256]{};
        if(!component_name(component, name)) { return {}; }
        // Open first: an existing directory on a read-only mount is a valid
        // ancestor. Create only after ENOENT, then repeat the same no-follow open.
        owned_fd child{::openat(parent, name, O_RDONLY | O_DIRECTORY | O_NOFOLLOW | O_CLOEXEC)};
        if(!child && create && errno == ENOENT)
        {
            if(::mkdirat(parent, name, 0700) != 0 && errno != EEXIST)
            { record_failure(u8"directory-create", errno); return {}; }
            child = owned_fd{::openat(parent, name, O_RDONLY | O_DIRECTORY | O_NOFOLLOW | O_CLOEXEC)};
        }
        if(!child) { record_failure(u8"directory-open-nofollow", errno); return {}; }
        if(!trusted_directory(child.value, private_root)) { return {}; }
        return child;
    }

    [[nodiscard]] inline owned_fd object_directory(::std::string_view root, ::std::string_view product,
                                                    ::std::string_view shard, bool create) noexcept
    {
        last_failure = {};
        if(root.size() > 4096uz || root.find('\0') != ::std::string_view::npos)
        { record_failure(u8"invalid-cache-root"); return {}; }
        bool const absolute{!root.empty() && root.front() == '/'};
        owned_fd dir{::open(absolute ? "/" : ".", O_RDONLY | O_DIRECTORY | O_NOFOLLOW | O_CLOEXEC)};
        if(!dir) { record_failure(u8"root-open", errno); return {}; }
        if(!trusted_directory(dir.value, false)) { return {}; }
        ::std::size_t pos{};
        while(pos != root.size())
        {
            while(pos != root.size() && root[pos] == '/') { ++pos; }
            if(pos == root.size()) { break; }
            auto const start{pos};
            while(pos != root.size() && root[pos] != '/') { ++pos; }
            auto const part{root.substr(start, pos - start)};
            if(part == ".") { continue; }
            dir = child_directory(dir.value, part, create, false);
            if(!dir) { return {}; }
        }
        // The configured root must be owner-controlled even when its ancestors
        // include /tmp. Every product/object/shard is opened relative to that fd.
        if(!trusted_directory(dir.value, true)) { return {}; }
        dir = child_directory(dir.value, product, create, true);
        if(!dir) { return {}; }
        dir = child_directory(dir.value, "objects", create, true);
        if(!dir) { return {}; }
        return child_directory(dir.value, shard, create, true);
    }

    struct blob_snapshot
    {
        ::std::unique_ptr<::std::byte[]> bytes{};
        ::std::size_t size{};
        bool ok{};
    };

    [[nodiscard]] inline bool trusted_object(struct ::stat const& info, ::std::size_t maximum) noexcept
    {
        return S_ISREG(info.st_mode) && info.st_uid == ::geteuid() && (info.st_mode & 0022) == 0 &&
               info.st_nlink == 1 && info.st_size > 0 && static_cast<::std::uintmax_t>(info.st_size) <= maximum &&
               static_cast<::std::uintmax_t>(info.st_size) <= static_cast<::std::uintmax_t>((::std::numeric_limits<::std::ptrdiff_t>::max)());
    }

    [[nodiscard]] inline blob_snapshot read_snapshot(int parent, ::std::string_view component, ::std::size_t maximum) noexcept
    {
        last_failure = {};
        blob_snapshot result{};
        char name[256]{};
        if(!component_name(component, name) || !trusted_directory(parent, true)) { return result; }
        // O_PATH does not invoke an unknown device/FIFO's open method. First
        // authenticate the actual inode, then open that pinned regular object
        // through the real procfs fd link, never through its mutable cache name.
        owned_fd pinned{::openat(parent, name, O_PATH | O_NOFOLLOW | O_CLOEXEC)};
        if(!pinned) { record_failure(u8"object-open-nofollow", errno); return result; }
        struct ::stat before{};
        if(::fstat(pinned.value, &before) != 0) { record_failure(u8"object-stat", errno); return result; }
        if(!trusted_object(before, maximum)) { record_failure(u8"object-type-owner-mode-links-or-size"); return result; }
        owned_fd proc{::open("/proc/self/fd", O_RDONLY | O_DIRECTORY | O_CLOEXEC)};
        struct ::statfs proc_type{};
        if(!proc || ::fstatfs(proc.value, &proc_type) != 0)
        { record_failure(u8"procfs-open-or-stat", errno); return result; }
        if(proc_type.f_type != 0x9fa0) { record_failure(u8"not-real-procfs"); return result; }
        char fd_name[32]{};
        auto const printed{::std::snprintf(fd_name, sizeof(fd_name), "%d", pinned.value)};
        if(printed <= 0 || static_cast<::std::size_t>(printed) >= sizeof(fd_name)) { return result; }
        owned_fd file{::openat(proc.value, fd_name, O_RDONLY | O_NONBLOCK | O_CLOEXEC)};
        struct ::stat opened{};
        if(!file || ::fstat(file.value, &opened) != 0 || !self_executable::same_content_identity(before, opened) ||
           !trusted_object(opened, maximum)) { record_failure(u8"pinned-object-identity-changed"); return result; }
        result.size = static_cast<::std::size_t>(before.st_size);
        result.bytes.reset(new(::std::nothrow) ::std::byte[result.size]);
        if(!result.bytes) { record_failure(u8"snapshot-allocation"); return result; }
        ::std::size_t offset{};
        while(offset != result.size)
        {
            auto const remaining{result.size - offset};
            auto const amount{remaining > 1024uz * 1024uz ? 1024uz * 1024uz : remaining};
            auto const count{::pread(file.value, result.bytes.get() + offset, amount, static_cast<::off_t>(offset))};
            if(count < 0) { if(errno == EINTR) { continue; } record_failure(u8"object-read", errno); return result; }
            if(count == 0) { record_failure(u8"object-truncated-during-read"); return result; }
            offset += static_cast<::std::size_t>(count);
        }
        struct ::stat after{};
        ::std::byte extra{};
        if(::pread(file.value, &extra, 1uz, before.st_size) != 0 || ::fstat(file.value, &after) != 0 ||
           !trusted_object(after, maximum) || !self_executable::same_content_identity(before, after) ||
           before.st_ctim.tv_sec != after.st_ctim.tv_sec || before.st_ctim.tv_nsec != after.st_ctim.tv_nsec) { record_failure(u8"object-changed-during-read"); return result; }
        if(!trusted_directory(parent, true)) { return result; }
        result.ok = true;
        return result;
    }

    template<typename FinalAdmission>
    [[nodiscard]] inline bool write_atomic(int parent, ::std::string_view final_component, ::std::string_view temporary_component,
                                           ::std::byte const* bytes, ::std::size_t size, FinalAdmission&& admitted) noexcept
    {
        last_failure = {};
        char final_name[256]{}, temporary_name[256]{};
        if(!component_name(final_component, final_name) || !component_name(temporary_component, temporary_name) ||
           !trusted_directory(parent, true)) { return false; }
        owned_fd file{::openat(parent, temporary_name, O_WRONLY | O_CREAT | O_EXCL | O_NOFOLLOW | O_CLOEXEC, 0600)};
        if(!file) { record_failure(u8"temporary-object-create", errno); return false; }
        bool published{};
        struct temporary_cleanup
        {
            int parent;
            char const* name;
            bool& published;
            ~temporary_cleanup() { if(!published) { ::unlinkat(parent, name, 0); } }
        } cleanup{parent, temporary_name, published};
        struct ::stat before{};
        if(::fstat(file.value, &before) != 0 || !S_ISREG(before.st_mode) || before.st_uid != ::geteuid() ||
           before.st_nlink != 1 || (before.st_mode & 0077) != 0) { record_failure(u8"temporary-object-identity"); return false; }
        ::std::size_t offset{};
        while(offset != size)
        {
            auto const remaining{size - offset};
            auto const amount{remaining > 1024uz * 1024uz ? 1024uz * 1024uz : remaining};
            auto const count{::write(file.value, bytes + offset, amount)};
            if(count < 0) { if(errno == EINTR) { continue; } record_failure(u8"object-write", errno); return false; }
            if(count == 0) { record_failure(u8"object-write-zero-progress"); return false; }
            offset += static_cast<::std::size_t>(count);
        }
        struct ::stat after{};
        if(::fstat(file.value, &after) != 0 || after.st_dev != before.st_dev || after.st_ino != before.st_ino ||
           after.st_size < 0 || static_cast<::std::uintmax_t>(after.st_size) != size || after.st_nlink != 1 ||
           after.st_uid != ::geteuid() || (after.st_mode & 0077) != 0)
        { record_failure(u8"temporary-object-changed"); return false; }
        if(::fsync(file.value) != 0) { record_failure(u8"object-sync", errno); return false; }
        struct ::stat named{};
        if(::fstatat(parent, temporary_name, &named, AT_SYMLINK_NOFOLLOW) != 0 || named.st_dev != before.st_dev ||
           named.st_ino != before.st_ino || !S_ISREG(named.st_mode) || named.st_nlink != 1) { record_failure(u8"temporary-name-replaced"); return false; }
        // Authenticate the retained executable immediately before publication.
        self_executable::digest_type digest{};
        if(!self_executable::current_digest(digest) || !trusted_directory(parent, true)) { return false; }
        // The real retained origin use is rechecked at the actual publication boundary.
        if(!admitted()) { return false; }
        if(::renameat(parent, temporary_name, parent, final_name) != 0) { record_failure(u8"object-publish", errno); return false; }
        published = true;
        if(::fsync(parent) != 0) { record_failure(u8"directory-sync", errno); return false; }
        return true;
    }
#endif
}

#if defined(__linux__) && (defined(__GNUC__) || defined(__clang__))
# pragma GCC visibility pop
#endif
