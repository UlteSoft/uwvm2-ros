/** @file @brief Native OpenSSL reference for one-time attachment provisioning.
 * @details Same random-source calls, reciprocal sealed identities, retained FD
 * policy and credential-enabled seqpacket pair as the Rust provisioner. This
 * measures setup, not a Wasm import, TLS handshake or per-message latency.
 */
#include <array>
#include <algorithm>
#include <cassert>
#include <charconv>
#include <chrono>
#include <cstdio>
#include <string_view>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/random.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <unistd.h>
#include <openssl/evp.h>
#include <openssl/crypto.h>
struct descriptor
{
 int fd;
 explicit descriptor(int raw):fd(::fcntl(raw,F_DUPFD_CLOEXEC,3)){assert(fd>=0);::close(raw);}
 ~descriptor(){::close(fd);}
 descriptor(descriptor const&)=delete;
};
static int identity()
{
 int fd=::memfd_create("native-attachment-identity",MFD_CLOEXEC|MFD_ALLOW_SEALING);assert(fd>=0);
 return fd;
}
static void sealed(descriptor const& fd,std::array<unsigned char,64> const& bytes)
{
 assert(::write(fd.fd,bytes.data(),bytes.size())==64&&::fchmod(fd.fd,0400)==0);
 assert(::fcntl(fd.fd,F_ADD_SEALS,F_SEAL_WRITE|F_SEAL_GROW|F_SEAL_SHRINK|F_SEAL_SEAL)==0);
}
__attribute__((noinline)) static void provision()
{
 std::array<std::array<unsigned char,32>,2> seed{},public_key{};
 std::array<std::array<unsigned char,64>,2> records{};
 for(unsigned i{};i<2;++i)
 {
  assert(::getrandom(seed[i].data(),32,GRND_NONBLOCK)==32);
  EVP_PKEY* key=EVP_PKEY_new_raw_private_key_ex(nullptr,"ED25519",nullptr,seed[i].data(),32);assert(key);
  std::size_t size=32;assert(EVP_PKEY_get_raw_public_key(key,public_key[i].data(),&size)==1&&size==32);EVP_PKEY_free(key);
 }
 assert(public_key[0]!=public_key[1]);
 for(unsigned i{};i<2;++i){std::copy(seed[i].begin(),seed[i].end(),records[i].begin());std::copy(public_key[1-i].begin(),public_key[1-i].end(),records[i].begin()+32);}
 descriptor a{identity()};sealed(a,records[0]);
 descriptor b{identity()};sealed(b,records[1]);
 std::array<int,2> raw{};assert(::socketpair(AF_UNIX,SOCK_SEQPACKET|SOCK_CLOEXEC|SOCK_NONBLOCK,0,raw.data())==0);
 descriptor c{raw[0]},d{raw[1]};int yes=1;
 for(auto fd:{c.fd,d.fd})assert(::setsockopt(fd,SOL_SOCKET,SO_PASSCRED,&yes,sizeof(yes))==0);
 OPENSSL_cleanse(seed.data(),sizeof(seed));OPENSSL_cleanse(records.data(),sizeof(records));
}
int main(int argc,char** argv)
{
 assert(argc==2);std::string_view text{argv[1]};unsigned count{};
 auto parsed=std::from_chars(text.data(),text.data()+text.size(),count);assert(parsed.ec==std::errc{}&&parsed.ptr==text.data()+text.size()&&count&&count<=100000);
 for(unsigned i{};i<100;++i)provision();auto start=std::chrono::steady_clock::now();
 for(unsigned i{};i<count;++i)provision();
 auto ns=std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::steady_clock::now()-start).count();
 std::printf("{\"pairs\":%u,\"ns_per_pair\":%.6f,\"pid\":%d}\n",count,static_cast<double>(ns)/count,::getpid());
}
