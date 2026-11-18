/** @file @brief Host-only validation for an operator-provisioned audit file.
 * @details The caller owns the supplied duplicate. A valid file is regular,
 * owned by the effective uid, mode 0600, singly linked, opened write-only
 * without append, and has the exact expected size. A successful check is a
 * point-in-time observation; an external process controlling the file can
 * still change it later.
 */
#pragma once
#if defined(__linux__)
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>
extern "C++" {
namespace uwvm2::imported::ros::audit_file
{
    inline bool validate(int descriptor, off_t expected, struct stat& info) noexcept
    {
        auto flags = ::fcntl(descriptor, F_GETFL);
        return flags >= 0 && (flags & O_ACCMODE) == O_WRONLY && !(flags & (O_APPEND | O_PATH)) &&
            !::fstat(descriptor, &info) && S_ISREG(info.st_mode) && info.st_nlink == 1 &&
            info.st_uid == ::geteuid() && (info.st_mode & 07777) == 0600 && info.st_size == expected;
    }
}
}
#endif
