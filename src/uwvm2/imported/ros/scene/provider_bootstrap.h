/** @file
 * @brief Validate a deployment-owned external driver's immutable bootstrap.
 * @details This parser is for the separate adapter process, not a guest import.
 * The trusted provisioner supplies a sealed 256-byte record and exactly three
 * companion descriptors: connected carrier, sealed TLS identity and peer clock.
 * All scalars are LE u64. Identifiers/digest use the driver protocol byte order.
 * No native structure layout, allocation, pathname, key bytes, process operation
 * or compatibility fallback is part of this record. It conveys no device grant.
 * Unknown service/profile/size, expired limits, a different process or descriptor
 * substitution fail before TLS. The existing channel separately checks pidfd,
 * per-packet credentials, same time namespace and both exact TLS public keys.
 * The provisioner must pin both children against later namespace changes.
 */
#pragma once
#include "driver_protocol.h"
#if defined(__linux__)
#include <fcntl.h>
#include <sys/stat.h>
#include <time.h>
#include <unistd.h>
#endif
namespace uwvm2::imported::ros::scene::provider_bootstrap
{
    inline constexpr std::size_t record_bytes=256;
    inline constexpr std::uint64_t magic=UINT64_C(0x00565244534f5257);
    inline constexpr std::uint64_t tls_ed25519_x25519_aes128gcm=1;
    struct credentials
    {
        std::uint64_t pid{},uid{},gid{};
        friend bool operator==(credentials const&,credentials const&)=default;
    };
    struct record
    {
        credentials local,peer;
        drivers::protocol::binding bound;
        std::uint64_t request_bytes{},response_bytes{},request_ns{},session_until_ns{};
        std::uint64_t channel_device{},channel_inode{},clock_device{},clock_inode{};
        friend bool operator==(record const&,record const&)=default;
    };
    /** @brief Decode without publishing partially validated attacker-controlled fields. */
    inline bool decode(wire::bytes bytes,record& output) noexcept
    {
        if(bytes.size()!=record_bytes)return false;
        wire::reader in{bytes};std::uint64_t tag{},size{},service{},profile{};record value;
        if(!in.u64(tag)||tag!=magic||!in.u64(size)||size!=record_bytes||!in.u64(service)||service!=1||
           !in.u64(profile)||profile!=tls_ed25519_x25519_aes128gcm)return false;
        for(auto* who:{&value.local,&value.peer})
            if(!in.u64(who->pid)||!who->pid||who->pid>INT32_MAX||!in.u64(who->uid)||who->uid>=UINT32_MAX||
               !in.u64(who->gid)||who->gid>=UINT32_MAX)return false;
        if(value.local.pid==value.peer.pid)return false;
        for(auto* id:{&value.bound.deployment,&value.bound.vm,&value.bound.module,&value.bound.driver,&value.bound.session})
            for(auto& word:*id)if(!in.u64(word))return false;
        for(auto& word:value.bound.contract)if(!in.u64(word))return false;
        if(!drivers::protocol::valid(value.bound)||!in.u64(value.request_bytes)||!value.request_bytes||value.request_bytes>65536||
           !in.u64(value.response_bytes)||!value.response_bytes||value.response_bytes>65536||
           !in.u64(value.request_ns)||!value.request_ns||value.request_ns>INT64_MAX||
           !in.u64(value.session_until_ns)||!value.session_until_ns||value.session_until_ns>INT64_MAX||
           !in.u64(value.channel_device)||!in.u64(value.channel_inode)||!value.channel_inode||
           !in.u64(value.clock_device)||!in.u64(value.clock_inode)||!value.clock_inode||!in.done())return false;
        output=value;return true;
    }
    /** @brief Load a sealed record and verify all borrowed kernel object identities.
     * @details The caller keeps ownership of every descriptor and closes all of
     * them on failure. Success does not authenticate the peer or grant authority;
     * it permits starting the mandatory bound mutual-TLS handshake. Startup is
     * bounded by the deployment's monotonic session deadline.
     */
    inline bool load(int manifest,int carrier,int identity,int clock,record& output) noexcept
    {
#if defined(__linux__)
        if(manifest<0||carrier<0||identity<0||clock<0||manifest==carrier||manifest==identity||manifest==clock||
           carrier==identity||carrier==clock||identity==clock)return false;
        constexpr int seals=F_SEAL_WRITE|F_SEAL_GROW|F_SEAL_SHRINK|F_SEAL_SEAL;
        struct stat info{},key{},socket{},time{};
        int a=::fcntl(manifest,F_GET_SEALS),b=::fcntl(identity,F_GET_SEALS);
        if(a<0||b<0||(a&seals)!=seals||(b&seals)!=seals||::fstat(manifest,&info)||!S_ISREG(info.st_mode)||
           info.st_size!=record_bytes||::fstat(identity,&key)||!S_ISREG(key.st_mode)||key.st_size!=64||
           ::fstat(carrier,&socket)||!S_ISSOCK(socket.st_mode)||::fstat(clock,&time))return false;
        std::array<std::uint8_t,record_bytes> bytes{};record value;
        if(::pread(manifest,bytes.data(),bytes.size(),0)!=static_cast<ssize_t>(bytes.size())||!decode(bytes,value))return false;
        if(value.local!=credentials{static_cast<std::uint64_t>(::getpid()),::getuid(),::getgid()}||
           ::geteuid()!=::getuid()||::getegid()!=::getgid()||
           value.channel_device!=static_cast<std::uint64_t>(socket.st_dev)||value.channel_inode!=static_cast<std::uint64_t>(socket.st_ino)||
           value.clock_device!=static_cast<std::uint64_t>(time.st_dev)||value.clock_inode!=static_cast<std::uint64_t>(time.st_ino))return false;
        ::timespec now{};
        if(::clock_gettime(CLOCK_MONOTONIC,&now)||now.tv_sec<0||now.tv_nsec<0||now.tv_nsec>=1000000000||
           static_cast<std::uint64_t>(now.tv_sec)>INT64_MAX/1000000000)return false;
        auto ns=static_cast<std::uint64_t>(now.tv_sec)*1000000000+static_cast<std::uint64_t>(now.tv_nsec);
        if(ns>=value.session_until_ns)return false;
        output=value;return true;
#else
        (void)manifest;(void)carrier;(void)identity;(void)clock;(void)output;return false;
#endif
    }
}
