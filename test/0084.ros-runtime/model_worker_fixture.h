#pragma once
#include "support/legacy_process/model_sandbox.h"
#include <cassert>
namespace x=uwvm2::imported::ros::scene::model_sandbox;
static x::image_spec image(char const* path, std::string_view destination, x::image_role role, uwvm2::utils::container::std_vector<x::descriptor>& ownership)
{
    x::descriptor file{::open(path, O_RDONLY | O_CLOEXEC)};
    assert(file);
    struct stat info{};
    assert(!::fstat(file.get(), &info) && info.st_size >= 0);
    ::fast_io::sha256_context hashing{};
    std::array<std::byte, 65536> bytes{};
    std::uint64_t offset{};
    while(offset < static_cast<std::uint64_t>(info.st_size))
    {
        auto count = ::pread(file.get(), bytes.data(), bytes.size(), offset);
        assert(count > 0);
        hashing.update(bytes.data(), bytes.data() + count);
        offset += count;
    }
    x::image_spec value;
    value.source = file.get();
    value.bytes = offset;
    value.role = role;
    assert(value.destination.assign(destination));
    hashing.do_final();
    hashing.digest_to_byte_ptr(reinterpret_cast<std::byte*>(value.sha256.data()));
    ownership.emplace_back(std::move(file));
    return value;
}


static x::manifest worker_manifest(char** paths,uwvm2::utils::container::std_vector<x::descriptor>& ownership)
{
    x::manifest manifest;
    manifest.launcher=image(paths[8],"/worker/launcher",x::image_role::runtime,ownership);
    constexpr std::array<std::string_view,8> names{"/worker/bootstrap","/worker/backend.so","/model/weights","/lib64/ld-linux-x86-64.so.2","/lib/x86_64-linux-gnu/libc.so.6","/lib/x86_64-linux-gnu/libstdc++.so.6","/lib/x86_64-linux-gnu/libgcc_s.so.1","/lib/x86_64-linux-gnu/libm.so.6"};
    for(std::size_t i{};i<names.size();++i)manifest.files.push_back(image(paths[i],names[i],i==0?x::image_role::bootstrap:i==2?x::image_role::weights:x::image_role::runtime,ownership));
    manifest.bound={64*1024*1024,256*1024*1024,5,4096,128,65536,1,4096,4096,1024,5000000000ull,1000000,1024*1024};
    return manifest;
}
