/*************************************************************
 * UlteSoft WebAssembly Virtual Machine (Version 2)          *
 * Copyright (c) 2025-present UlteSoft. All rights reserved. *
 * Licensed under the APL-2.0 License (see LICENSE file).    *
 *************************************************************/
#pragma once

#include <cerrno>
#include <cstdint>
#include <string_view>
#if defined(__linux__)
# include <linux/magic.h>
# include <sys/stat.h>
# include <sys/statfs.h>
# include <sys/sysmacros.h>
# include <unistd.h>
#endif

namespace uwvm2::runtime::llvm_jit_cache::stdio_admission
{
    /// @brief Describe a standard descriptor without exposing paths or key material.
    struct result
    {
        bool admitted{};
        ::std::u8string_view reason{};
        int system_error{};
    };

#if defined(__linux__)
    /**
     * @brief Admit ordinary file contents, without granting a filesystem namespace.
     * @details A regular inode alone is insufficient: procfs also exposes regular
     * files backed by live process state. Only reviewed data filesystems are
     * admitted. Unknown filesystems fail closed; neither path spelling, access
     * mode nor apparent ownership can turn a pseudo-file into ordinary storage.
     * Existing WASI descriptor operations cannot obtain another path from this
     * file. File contents and timestamps do not participate in object identity.
     */
    [[nodiscard]] inline bool ordinary_data_filesystem(::std::uint32_t type) noexcept
    {
        switch(type)
        {
            case EXT4_SUPER_MAGIC:
            case XFS_SUPER_MAGIC:
            case BTRFS_SUPER_MAGIC:
            case TMPFS_MAGIC:
            case OVERLAYFS_SUPER_MAGIC:
            case NFS_SUPER_MAGIC:
            // Stable Linux UAPI values; older ROS sysroot magic.h files do not
            // publish these names. The reviewed allowlist is unchanged.
            // include/uapi/linux/magic.h in the Linux v6.12 source tree.
            case 0x65735546u: // FUSE_SUPER_MAGIC
            case 0xFF534D42u: // CIFS_SUPER_MAGIC
            case 0xFE534D42u: // SMB2_SUPER_MAGIC
            case 0x00c36400u: // CEPH_SUPER_MAGIC
            case V9FS_MAGIC: return true;
            default: return false;
        }
    }
#endif

    /**
     * @brief Check the actual inherited standard descriptor before reading a signing key.
     * @details The caller holds the canonical WASI descriptor cell lock during
     * the cold, pre-execution provider census. Pipes, sockets and terminals have
     * no WASIp1 descriptor-passing or path-opening operation. Ordinary regular
     * files are permitted, while directories, pseudo-files and unknown devices
     * remain excluded. This check never opens a replacement path or changes a
     * descriptor's rights, flags, contents or lifetime.
     */
    [[nodiscard]] inline result inspect(int handle) noexcept
    {
#if defined(__linux__)
        if(handle < 0) { return {false, u8"invalid-native-descriptor", EBADF}; }
        struct ::stat metadata{};
        if(::fstat(handle, &metadata) != 0) { return {false, u8"descriptor-stat-failed", errno}; }
        if(S_ISFIFO(metadata.st_mode) || S_ISSOCK(metadata.st_mode)) { return {true, u8"stream", 0}; }
        if(S_ISCHR(metadata.st_mode))
        {
            if(metadata.st_rdev == ::makedev(1u, 3u) || ::isatty(handle) == 1) { return {true, u8"null-or-terminal", 0}; }
            return {false, u8"unreviewed-device", 0};
        }
        if(!S_ISREG(metadata.st_mode)) { return {false, u8"non-file-descriptor", 0}; }
        struct ::statfs filesystem{};
        if(::fstatfs(handle, &filesystem) != 0) { return {false, u8"descriptor-filesystem-stat-failed", errno}; }
        if(!ordinary_data_filesystem(static_cast<::std::uint32_t>(filesystem.f_type))) { return {false, u8"pseudo-or-unreviewed-filesystem", 0}; }
        return {true, u8"ordinary-file", 0};
#else
        (void)handle;
        return {false, u8"unsupported-host", 0};
#endif
    }
}  // namespace uwvm2::runtime::llvm_jit_cache::stdio_admission
