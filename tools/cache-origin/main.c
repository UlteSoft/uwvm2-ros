/**
 * @file main.c
 * @brief Restore one administrator-owned cache origin into a Linux UID keyring.
 * @details Run before starting the VM. The VM never reads this root-only store.
 * An existing, different kernel key is refused, never replaced or revoked.
 * This helper does not sign objects, grant guest rights, or authenticate files
 * on behalf of a caller. It requires real root, not a setuid installation.
 */
#define _GNU_SOURCE 1
#include <errno.h>
#include <fcntl.h>
#include <inttypes.h>
#include <limits.h>
#include <linux/keyctl.h>
#include <pwd.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/file.h>
#include <sys/prctl.h>
#include <sys/random.h>
#include <sys/resource.h>
#include <sys/stat.h>
#include <sys/syscall.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

/** @brief Match the runtime's immutable, direct UID-keyring origin contract. */
static char const origin_name[] = "uwvm:llvm-jit-origin:v1";
static unsigned long const origin_permissions = 0x001b0000ul;
static unsigned long const ring_permissions = 0x003f0000ul;

/** @brief Erase temporary secret storage before either process exits. */
static void erase(void* memory, size_t length)
{
    volatile unsigned char* bytes = memory;
    while(length-- != 0u) { *bytes++ = 0u; }
}

/** @brief Report only the failed operation and its public OS error. */
static bool failure(char const* operation)
{
    fprintf(stderr, "cache-origin: %s: %s\n", operation, strerror(errno));
    return false;
}

/** @brief Obtain complete kernel randomness without libc random-state buffers. */
static bool random_bytes(unsigned char* data, size_t length)
{
    while(length != 0u)
    {
        ssize_t const count = syscall(SYS_getrandom, data, length, 0u);
        if(count < 0 && errno == EINTR) { continue; }
        if(count <= 0) { return failure("getrandom"); }
        data += (size_t)count;
        length -= (size_t)count;
    }
    return true;
}

/** @brief Reject writable ancestors, symlinks and a non-private final store. */
static bool directory_is_trusted(int fd, bool private_directory)
{
    struct stat info;
    if(fstat(fd, &info) != 0) { return failure("stat state directory"); }
    if(!S_ISDIR(info.st_mode) || info.st_uid != 0u ||
       (info.st_mode & 0022u) != 0u ||
       (private_directory && (info.st_mode & 07777u) != 0700u))
    {
        errno = EPERM;
        return failure("state directory must have trusted root ownership and mode");
    }
    return true;
}

/** @brief Traverse the fixed store by retained descriptors without path races. */
static int open_store(void)
{
    char const* const components[] = {"var", "lib", "uwvm2-ros", "cache-origin"};
    int current = open("/", O_RDONLY | O_DIRECTORY | O_CLOEXEC | O_NOFOLLOW);
    if(current < 0) { failure("open filesystem root"); return -1; }
    if(!directory_is_trusted(current, false)) { close(current); return -1; }
    for(size_t index = 0; index != sizeof components / sizeof components[0]; ++index)
    {
        if(index >= 2u && mkdirat(current, components[index], 0700u) != 0 && errno != EEXIST)
        { failure("create root-owned state directory"); close(current); return -1; }
        int const child = openat(current, components[index], O_RDONLY | O_DIRECTORY | O_CLOEXEC | O_NOFOLLOW);
        if(child < 0) { failure("open state directory without following links"); close(current); return -1; }
        close(current);
        current = child;
        if(!directory_is_trusted(current, index == 3u)) { close(current); return -1; }
    }
    return current;
}

/** @brief State files must be private regular root files with one directory link. */
static bool file_is_trusted(int fd, off_t length)
{
    struct stat info;
    if(fstat(fd, &info) != 0) { return failure("stat state file"); }
    if(!S_ISREG(info.st_mode) || info.st_uid != 0u || info.st_gid != 0u ||
       (info.st_mode & 07777u) != 0600u || info.st_nlink != 1u || info.st_size != length)
    {
        errno = EPERM;
        return failure("state file owner, type, mode, links or size");
    }
    return true;
}

/** @brief Commit a fresh origin before publishing it to any running process. */
static bool create_secret(int directory, char const* name, unsigned char seed[32])
{
    char temporary[80];
    int const size = snprintf(temporary, sizeof temporary, ".pending-%ld", (long)getpid());
    if(size < 0 || (size_t)size >= sizeof temporary) { return false; }
    int const fd = openat(directory, temporary, O_WRONLY | O_CREAT | O_EXCL | O_NOFOLLOW | O_CLOEXEC, 0600u);
    if(fd < 0) { return failure("create exclusive pending state"); }
    bool ok = random_bytes(seed, 32u);
    size_t offset = 0;
    while(ok && offset != 32u)
    {
        ssize_t const amount = write(fd, seed + offset, 32u - offset);
        if(amount < 0 && errno == EINTR) { continue; }
        if(amount <= 0) { ok = failure("write pending origin"); break; }
        offset += (size_t)amount;
    }
    if(ok && fsync(fd) != 0) { ok = failure("sync pending origin"); }
    if(ok && !file_is_trusted(fd, 32)) { ok = false; }
    if(ok && linkat(directory, temporary, directory, name, 0) != 0)
    { ok = failure("publish origin without replacing existing state"); }
    if(close(fd) != 0) { ok = failure("close pending origin"); }
    if(unlinkat(directory, temporary, 0) != 0) { ok = failure("remove owned pending state"); }
    if(ok && fsync(directory) != 0) { ok = failure("sync origin directory"); }
    return ok;
}

/** @brief Read exactly one private origin; malformed existing state is never reset. */
static bool load_secret(int directory, uid_t uid, unsigned char seed[32])
{
    char name[48];
    int const length = snprintf(name, sizeof name, "uid-%" PRIuMAX ".secret", (uintmax_t)uid);
    if(length < 0 || (size_t)length >= sizeof name) { return false; }
    int fd = openat(directory, name, O_RDONLY | O_NONBLOCK | O_NOFOLLOW | O_CLOEXEC);
    if(fd < 0 && errno == ENOENT)
    {
        if(!create_secret(directory, name, seed)) { return false; }
        fd = openat(directory, name, O_RDONLY | O_NONBLOCK | O_NOFOLLOW | O_CLOEXEC);
    }
    if(fd < 0) { return failure("open root-only origin"); }
    bool ok = file_is_trusted(fd, 32);
    size_t offset = 0;
    while(ok && offset != 32u)
    {
        ssize_t const amount = read(fd, seed + offset, 32u - offset);
        if(amount < 0 && errno == EINTR) { continue; }
        if(amount <= 0) { ok = failure("read complete origin"); break; }
        offset += (size_t)amount;
    }
    if(ok) { ok = file_is_trusted(fd, 32); }
    close(fd);
    return ok;
}

/** @brief Use the kernel keyctl API without shell tools or secret argv/env data. */
static long control(int command, unsigned long a, unsigned long b, unsigned long c, unsigned long d)
{
    return syscall(SYS_keyctl, command, a, b, c, d);
}

static unsigned long key_number(int32_t key) { return (unsigned long)(long)key; }
static unsigned long pointer(void const* address) { return (unsigned long)(uintptr_t)address; }
static bool valid_serial(long key) { return key > 0 && key <= INT32_MAX; }

/** @brief Recursive SEARCH results must also be direct members of the UID ring. */
static bool direct_member(int32_t ring, int32_t key)
{
    int32_t members[4096];
    long const bytes = control(KEYCTL_READ, key_number(ring), pointer(members), sizeof members, 0u);
    if(bytes < 0 || (unsigned long)bytes > sizeof members || (size_t)bytes % sizeof members[0] != 0u) { return false; }
    for(size_t index = 0; index != (size_t)bytes / sizeof members[0]; ++index)
    { if(members[index] == key) { return true; } }
    return false;
}

/** @brief Authenticate key owner, immutable permissions, direct binding and bytes. */
static bool same_origin(int32_t ring, int32_t key, uid_t uid, gid_t gid, unsigned char const seed[32])
{
    char actual[256], expected[256];
    int const size = snprintf(expected, sizeof expected, "user;%" PRIuMAX ";%" PRIuMAX ";%08lx;%s",
                              (uintmax_t)uid, (uintmax_t)gid, origin_permissions, origin_name);
    if(size < 0 || (size_t)size >= sizeof expected) { return false; }
    long const count = control(KEYCTL_DESCRIBE, key_number(key), pointer(actual), sizeof actual, 0u);
    if(count != (long)size + 1 || memcmp(actual, expected, (size_t)count) != 0 || !direct_member(ring, key)) { return false; }
    unsigned char found[32] = {0};
    long const bytes = control(KEYCTL_READ, key_number(key), pointer(found), sizeof found, 0u);
    unsigned difference = 0u;
    for(size_t index = 0; index != sizeof found; ++index) { difference |= (unsigned)(found[index] ^ seed[index]); }
    erase(found, sizeof found);
    return bytes == 32 && difference == 0u && direct_member(ring, key) &&
           control(KEYCTL_SEARCH, key_number(ring), pointer("user"), pointer(origin_name), 0u) == key;
}

/** @brief Publish through an exclusive move; concurrent foreign keys are retained. */
static bool publish_origin(int32_t destination, uid_t uid, gid_t gid, unsigned char const seed[32])
{
    unsigned char nonce[16];
    if(!random_bytes(nonce, sizeof nonce)) { return false; }
    char name[80];
    size_t offset = (size_t)snprintf(name, sizeof name, "uwvm:origin-bootstrap:");
    char const digits[] = "0123456789abcdef";
    for(size_t index = 0; index != sizeof nonce; ++index)
    { name[offset++] = digits[nonce[index] >> 4u]; name[offset++] = digits[nonce[index] & 15u]; }
    name[offset] = '\0';
    long const parent = control(KEYCTL_GET_KEYRING_ID, key_number(KEY_SPEC_THREAD_KEYRING), 1u, 0u, 0u);
    if(!valid_serial(parent)) { return failure("open temporary thread keyring"); }
    long const temporary = syscall(SYS_add_key, "keyring", name, NULL, 0u, (int32_t)parent);
    if(!valid_serial(temporary)) { return failure("create temporary keyring"); }
    bool ok = control(KEYCTL_SETPERM, (unsigned long)temporary, ring_permissions, 0u, 0u) == 0;
    long key = -1;
    bool moved = false;
    if(ok)
    {
        key = syscall(SYS_add_key, "user", origin_name, seed, 32u, (int32_t)temporary);
        ok = valid_serial(key) && control(KEYCTL_SETPERM, (unsigned long)key, origin_permissions, 0u, 0u) == 0;
    }
    if(ok)
    {
        moved = control(KEYCTL_MOVE, (unsigned long)key, (unsigned long)temporary, key_number(destination), KEYCTL_MOVE_EXCL) == 0;
        if(!moved && errno != EEXIST) { ok = failure("publish immutable origin"); }
        if(ok)
        {
            long const winner = control(KEYCTL_SEARCH, key_number(destination), pointer("user"), pointer(origin_name), 0u);
            ok = valid_serial(winner) && same_origin(destination, (int32_t)winner, uid, gid, seed);
        }
    }
    if(valid_serial(key) && !moved && control(KEYCTL_UNLINK, (unsigned long)key, (unsigned long)temporary, 0u, 0u) != 0) { ok = false; }
    if(control(KEYCTL_UNLINK, (unsigned long)temporary, (unsigned long)parent, 0u, 0u) != 0) { ok = false; }
    return ok;
}

/** @brief Drop all supplementary and root credentials before selecting the UID ring. */
static bool restore(uid_t uid, gid_t gid, unsigned char const seed[32])
{
    if(syscall(SYS_setgroups, 0u, NULL) != 0 || setresgid(gid, gid, gid) != 0 || setresuid(uid, uid, uid) != 0 ||
       prctl(PR_SET_DUMPABLE, 0ul, 0ul, 0ul, 0ul) != 0 || prctl(PR_SET_NO_NEW_PRIVS, 1ul, 0ul, 0ul, 0ul) != 0)
    { return failure("drop helper credentials and disable dumps"); }
    long const ring = control(KEYCTL_GET_KEYRING_ID, key_number(KEY_SPEC_USER_KEYRING), 1u, 0u, 0u);
    if(!valid_serial(ring)) { return failure("open target UID keyring"); }
    long const existing = control(KEYCTL_SEARCH, (unsigned long)ring, pointer("user"), pointer(origin_name), 0u);
    if(valid_serial(existing))
    {
        if(same_origin((int32_t)ring, (int32_t)existing, uid, gid, seed)) { return true; }
        errno = EKEYREJECTED;
        return failure("existing kernel origin differs; stop and provision before VM startup after reboot");
    }
    if(existing != -1 || errno != ENOKEY) { return failure("search existing kernel origin"); }
    if(!publish_origin((int32_t)ring, uid, gid, seed))
    { errno = EKEYREJECTED; return failure("immutable origin publication or verification"); }
    return true;
}

/** @brief Provision or verify one numeric local account; never install setuid. */
int main(int argc, char** argv)
{
    struct rlimit const no_core = {0u, 0u};
    if(getuid() != 0u || geteuid() != 0u || getgid() != 0u || getegid() != 0u)
    { fprintf(stderr, "cache-origin: run as real root; setuid/setgid use is forbidden\n"); return 1; }
    if(prctl(PR_SET_DUMPABLE, 0ul, 0ul, 0ul, 0ul) != 0 || setrlimit(RLIMIT_CORE, &no_core) != 0)
    { failure("disable core dumps"); return 1; }
    if(argc != 3 || strcmp(argv[1], "--uid") != 0 || argv[2][0] == '\0')
    { fprintf(stderr, "usage: uwvm2-ros-cache-origin --uid NUMERIC_UID\n"); return 2; }
    uintmax_t number = 0u;
    for(char const* next = argv[2]; *next != '\0'; ++next)
    {
        if(*next < '0' || *next > '9' || number > (UINT32_MAX - 1u - (unsigned)(*next - '0')) / 10u)
        { fprintf(stderr, "cache-origin: invalid numeric UID\n"); return 2; }
        number = number * 10u + (unsigned)(*next - '0');
    }
    uid_t const uid = (uid_t)number;
    struct passwd const* account = getpwuid(uid);
    if(account == NULL) { fprintf(stderr, "cache-origin: target UID has no local account\n"); return 2; }
    gid_t const gid = account->pw_gid;
    umask(0077u);
    int const directory = open_store();
    if(directory < 0) { return 1; }
    int const lock = openat(directory, ".lock", O_RDWR | O_CREAT | O_NONBLOCK | O_NOFOLLOW | O_CLOEXEC, 0600u);
    if(lock < 0 || !file_is_trusted(lock, 0) || flock(lock, LOCK_EX | LOCK_NB) != 0)
    { failure("exclusive provisioning lease"); if(lock >= 0) { close(lock); } close(directory); return 1; }
    unsigned char seed[32] = {0};
    bool ok = load_secret(directory, uid, seed);
    if(ok)
    {
        pid_t const child = fork();
        if(child == 0)
        {
            close(lock);
            close(directory);
            bool const restored = restore(uid, gid, seed);
            erase(seed, sizeof seed);
            _exit(restored ? 0 : 1);
        }
        erase(seed, sizeof seed);
        if(child < 0) { ok = failure("fork UID provisioner"); }
        else
        {
            int status = 0;
            pid_t waited;
            do { waited = waitpid(child, &status, 0); } while(waited < 0 && errno == EINTR);
            ok = waited == child && WIFEXITED(status) && WEXITSTATUS(status) == 0;
        }
    }
    erase(seed, sizeof seed);
    close(lock);
    close(directory);
    if(ok) { printf("cache-origin: uid=%" PRIuMAX " immutable kernel origin verified\n", (uintmax_t)uid); }
    return ok ? 0 : 1;
}
