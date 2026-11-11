// Exercise the production verifier and the sealed descriptor used by dlopen.
// run_trusted_driver.py creates local certificates and constructor witnesses.
#include <uwvm2/utils/security/trusted_driver.h>
#include <cassert>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <dlfcn.h>
#include <string>
#include <vector>
#include <sys/stat.h>

namespace driver = uwvm2::utils::security::trusted_driver;

int main(int argc, char** argv)
{
    assert(argc >= 4);
    std::string const mode{argv[1]};
    assert(!driver::process_verifier.selected());
    if(mode == "unavailable")
    {
        auto error = driver::process_verifier.select(argv[2]);
        assert(error && !driver::process_verifier.selected());
        driver::image image;
        assert(image.prepare(argv[3]) && image.descriptor() < 0);
        std::printf("unavailable: %s\n", error);
        return 0;
    }
    if(mode == "select-reject")
    {
        auto error = driver::process_verifier.select(argv[2]);
        assert(error && !driver::process_verifier.selected());
        std::printf("certificate rejected: %s\n", error);
        return 0;
    }
    driver::image image;
    std::string load_path{argv[3]};
    if(mode != "off")
    {
        auto selected = driver::process_verifier.select(argv[2]);
        if(selected) { std::fprintf(stderr, "select: %s\n", selected); return 1; }
        assert(driver::process_verifier.selected());
        assert(driver::process_verifier.select(argv[2])); // no mutable trust pin
        auto error = image.prepare(argv[3]);
        if(mode == "reject")
        {
            assert(error && image.descriptor() < 0);
            std::printf("image rejected before dlopen: %s\n", error);
            return 0;
        }
        if(error) { std::fprintf(stderr, "prepare: %s\n", error); return 1; }
        assert(image.descriptor() >= 0);
        assert(image.prepare(argv[3])); // cannot silently replace an admitted image
        int const original = image.descriptor();
        driver::image moved{std::move(image)};
        assert(image.descriptor() == -1 && moved.descriptor() == original);
        image = std::move(moved);
        assert(moved.descriptor() == -1 && image.descriptor() == original);
        constexpr int seals = F_SEAL_WRITE | F_SEAL_GROW | F_SEAL_SHRINK | F_SEAL_SEAL;
        assert((::fcntl(original, F_GET_SEALS) & seals) == seals);
        assert((::fcntl(original, F_GETFD) & FD_CLOEXEC) != 0);
        unsigned char byte{};
        errno = 0;
        assert(::pwrite(original, &byte, 1, 0) == -1 && errno == EPERM);
        errno = 0;
        assert(::ftruncate(original, 0) == -1 && errno == EPERM);
        struct stat status{};
        assert(::fstat(original, &status) == 0 && status.st_size > 0);
        if(mode == "replace" || mode == "replace-prepare")
        {
            assert(argc == 5);
            std::vector<unsigned char> before(static_cast<std::size_t>(status.st_size));
            std::vector<unsigned char> after(before.size());
            assert(::pread(original, before.data(), before.size(), 0) == status.st_size);
            // Replace the supplied pathname AFTER successful verification.
            // Constructors must still come from the retained, sealed payload.
            assert(::rename(argv[4], argv[3]) == 0);
            assert(::pread(original, after.data(), after.size(), 0) == status.st_size);
            assert(before == after);
        }
        load_path = "/proc/self/fd/" + std::to_string(original);
        std::printf("sealed payload bytes: %lld\n", static_cast<long long>(status.st_size));
        if(mode == "prepare" || mode == "replace-prepare")
        {
            std::puts("prepared only; sealed dlopen/constructor not exercised");
            return 0;
        }
    }
    else
    {
        assert(!driver::process_verifier.selected());
        assert(image.prepare(argv[3]) && image.descriptor() == -1);
    }
    auto* library = ::dlopen(load_path.c_str(), RTLD_NOW | RTLD_LOCAL);
    if(!library) { std::fprintf(stderr, "dlopen: %s\n", ::dlerror()); return 1; }
    auto value = reinterpret_cast<int (*)()>(::dlsym(library, "trusted_driver_value"));
    assert(value && value() == 42);
    assert(::dlclose(library) == 0);
    std::puts("verified constructor and exported function completed");
}
