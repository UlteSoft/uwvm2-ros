// Trusted deployment input only. This is NOT a native-code sandbox.
// Optional Linux sign-file appended CMS verification. External deployment
// owns PKI/revocation/provenance/dependency policy. The host explicitly pins a
// previously approved signer certificate; embedded certificates are NOT trusted.
// When selected, VM verifies and loads the SAME immutable payload bytes.
#pragma once
#include <array>
#include <cstdint>
#include <cstddef>
#include <string_view>
#include <utility>
#include <limits>
#include <cstring>
#if defined(__linux__)
#include <cerrno>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/syscall.h>
#include <sys/mman.h>
#include <linux/memfd.h>
#endif
#if defined(UWVM_WSEA_OPENSSL)
#include <openssl/evp.h>
#include <openssl/cms.h>
#include <openssl/pem.h>
#endif
namespace uwvm2::utils::security::trusted_driver
{
    class launch_verifier final
    {
#if defined(UWVM_WSEA_OPENSSL)
        X509* signer_{};
#endif
        bool selected_{};
    public:
        launch_verifier()=default;
        launch_verifier(launch_verifier const&)=delete;
        ~launch_verifier(){
#if defined(UWVM_WSEA_OPENSSL)
            if(signer_)X509_free(signer_);
#endif
        }
        [[nodiscard]] bool selected()const noexcept{return selected_;}
        [[nodiscard]] char const* select(char const* certificate)noexcept{
            if(selected_)return "driver signature verification was already selected";
#if defined(UWVM_WSEA_OPENSSL)
            BIO* file=BIO_new_file(certificate,"rb");
            if(!file)return "cannot open the host-selected signer certificate";
            X509* cert=PEM_read_bio_X509(file,nullptr,nullptr,nullptr);BIO_free(file);
            if(!cert)return "expected one PEM X.509 signer certificate";
            auto* key=X509_get0_pubkey(cert);
            if(!key || EVP_PKEY_get_security_bits(key)<112){X509_free(cert);return "signer key strength is below 112 security bits";}
            signer_=cert;selected_=true;return nullptr;
#else
            (void)certificate;return "OpenSSL driver verifier unavailable";
#endif
        }
#if defined(UWVM_WSEA_OPENSSL)
        [[nodiscard]] X509* signer()const noexcept{return signer_;}
#endif
    };
    inline launch_verifier process_verifier{};
    class image final
    {
        int fd_{-1};
    public:
        constexpr image() noexcept = default;
        image(image const&)=delete;
        image& operator=(image const&)=delete;
        image(image&& other) noexcept:fd_(::std::exchange(other.fd_,-1)){}
        image& operator=(image&& other) noexcept
        {
            if(this!=&other){reset();fd_=::std::exchange(other.fd_,-1);}return *this;
        }
        ~image(){reset();}
        void reset() noexcept {
#if defined(__linux__)
            if(fd_>=0)::close(fd_);
#endif
            fd_=-1;
        }
        [[nodiscard]] int descriptor()const noexcept{return fd_;}
        [[nodiscard]] char const* prepare(char const* path) noexcept
        {
#if defined(__linux__) && defined(UWVM_WSEA_OPENSSL)
            if(fd_>=0)return "driver image already prepared";
            if(!process_verifier.selected())return "driver signature verification was not selected";
            image input,copy;
            input.fd_=::open(path,O_RDONLY|O_CLOEXEC|O_NONBLOCK);
            if(input.fd_<0)return "cannot open driver input";
            struct stat status{};
            if(::fstat(input.fd_,&status)!=0 || !S_ISREG(status.st_mode))
                return "driver input must be a regular file";
            copy.fd_=static_cast<int>(::syscall(SYS_memfd_create,"uwvm-trusted-driver",MFD_CLOEXEC|MFD_ALLOW_SEALING));
            if(copy.fd_<0)return "cannot create immutable driver image";
            unsigned char buffer[65536];
            for(;;){
                auto n=::read(input.fd_,buffer,sizeof(buffer));
                if(n<0){if(errno==EINTR)continue;return "driver read failed";}
                if(!n)break;
                ::std::size_t written{};
                while(written<static_cast<::std::size_t>(n)){
                    auto v=::write(copy.fd_,buffer+written,static_cast<::std::size_t>(n)-written);
                    if(v>0)written+=static_cast<::std::size_t>(v);
                    else if(v<0&&errno==EINTR)continue;else return "driver image copy failed";
                }
            }
            auto seal=[](int fd)noexcept{return ::fcntl(fd,F_ADD_SEALS,F_SEAL_WRITE|F_SEAL_GROW|F_SEAL_SHRINK|F_SEAL_SEAL)==0;};
            if(!seal(copy.fd_))return "driver image could not be sealed";
            if(::fstat(copy.fd_,&status)!=0)return "cannot stat sealed driver";
            // Linux UAPI layout: CMS bytes, 12-byte module_signature, marker.
            // Read bytes explicitly: no alignment or native-endian struct casts.
            constexpr char marker[]="~Module signature appended~\n";
            constexpr ::std::size_t trailer_size=12u+sizeof(marker)-1u;
            unsigned char trailer[trailer_size]{};
            if(status.st_size<=static_cast<off_t>(trailer_size) ||
                ::pread(copy.fd_,trailer,trailer_size,status.st_size-trailer_size)!=static_cast<ssize_t>(trailer_size) ||
                ::std::memcmp(trailer+12u,marker,sizeof(marker)-1u)!=0)
                return "driver has no Linux appended module signature";
            for(unsigned i{};i<8u;++i)
                if(trailer[i]!=(i==2u?2u:0u))return "invalid Linux signature trailer";
            ::std::uint64_t signature_size{};
            for(unsigned i=8u;i<12u;++i)signature_size=(signature_size<<8u)|trailer[i];
            if(signature_size==0u || signature_size>=static_cast<::std::uint64_t>(status.st_size)-trailer_size ||
                signature_size>static_cast<::std::uint64_t>((::std::numeric_limits<long>::max)()))
                return "invalid CMS signature length";
            auto const payload_size=static_cast<::std::uint64_t>(status.st_size)-trailer_size-signature_size;
            auto const page=::sysconf(_SC_PAGESIZE);
            if(page<=0)return "cannot query mapping alignment";
            auto const base=payload_size/static_cast<unsigned long>(page)*static_cast<unsigned long>(page);
            auto const skip=payload_size-base;
            if(signature_size+skip>SIZE_MAX)return "CMS mapping does not fit host address space";
            struct mapping{
                void* data{MAP_FAILED};::std::size_t size{};
                ~mapping(){if(data!=MAP_FAILED)::munmap(data,size);}
            } mapped;
            mapped.size=static_cast<::std::size_t>(signature_size+skip);
            mapped.data=::mmap(nullptr,mapped.size,PROT_READ,MAP_PRIVATE,copy.fd_,static_cast<off_t>(base));
            if(mapped.data==MAP_FAILED)return "cannot map sealed CMS signature";
            auto const* first=static_cast<unsigned char const*>(mapped.data)+skip;
            auto const* cursor=first;
            struct cms_owner {CMS_ContentInfo* value{};~cms_owner(){if(value)CMS_ContentInfo_free(value);}} cms;
            cms.value=d2i_CMS_ContentInfo(nullptr,&cursor,static_cast<long>(signature_size));
            if(!cms.value || cursor!=first+signature_size || CMS_is_detached(cms.value)!=1)
                return "malformed or non-detached CMS signature";
            auto* signers=CMS_get0_SignerInfos(cms.value);
            if(!signers || sk_CMS_SignerInfo_num(signers)==0)return "CMS has no signer";
            for(int i{};i<sk_CMS_SignerInfo_num(signers);++i){
                X509_ALGOR* algorithm{};
                CMS_SignerInfo_get0_algs(sk_CMS_SignerInfo_value(signers,i),nullptr,nullptr,&algorithm,nullptr);
                if(!algorithm)return "CMS digest algorithm unavailable";
                int nid=OBJ_obj2nid(algorithm->algorithm);
                if(nid!=NID_sha256 && nid!=NID_sha384 && nid!=NID_sha512 &&
                    nid!=NID_sha3_256 && nid!=NID_sha3_384 && nid!=NID_sha3_512)
                    return "CMS digest must be SHA-2 or SHA-3 with at least 256 bits";
            }
            // Detached BIO reads exactly the signed prefix. The signed image
            // is already immutable; keep the payload immutable before verify.
            image payload;
            payload.fd_=static_cast<int>(::syscall(SYS_memfd_create,"uwvm-verified-driver",MFD_CLOEXEC|MFD_ALLOW_SEALING));
            if(payload.fd_<0)return "cannot create verified driver payload";
            ::std::uint64_t copied{};
            while(copied<payload_size){
                auto amount=static_cast<::std::size_t>(payload_size-copied<sizeof(buffer)?payload_size-copied:sizeof(buffer));
                auto n=::pread(copy.fd_,buffer,amount,static_cast<off_t>(copied));
                if(n<0&&errno==EINTR)continue;
                if(n<=0)return "sealed payload read failed";
                ::std::size_t written{};
                while(written<static_cast<::std::size_t>(n)){
                    auto v=::write(payload.fd_,buffer+written,static_cast<::std::size_t>(n)-written);
                    if(v>0)written+=static_cast<::std::size_t>(v);
                    else if(v<0&&errno==EINTR)continue;else return "payload copy failed";
                }
                copied+=static_cast<::std::uint64_t>(n);
            }
            if(!seal(payload.fd_) || ::lseek(payload.fd_,0,SEEK_SET)!=0)return "cannot seal signed payload";
            struct certs_owner{STACK_OF(X509)* value{sk_X509_new_null()};~certs_owner(){sk_X509_free(value);}} certs;
            if(!certs.value || !sk_X509_push(certs.value,process_verifier.signer()))return "signer allocation failed";
            struct bio_owner{BIO* value{};~bio_owner(){if(value)BIO_free(value);}} data{BIO_new_fd(payload.fd_,BIO_NOCLOSE)};
            // Explicitly pinned signer, like a trusted kernel keyring entry.
            // External deployment performs PKI policy; NEVER disable content
            // or attribute verification. Do not trust certificates in the CMS.
            if(!data.value || CMS_verify(cms.value,certs.value,nullptr,data.value,nullptr,
                CMS_BINARY|CMS_NOINTERN|CMS_NO_SIGNER_CERT_VERIFY)!=1)
                return "driver signature invalid or signer is not the host-pinned certificate";
            copy=::std::move(payload);
            unsigned char elf[64]{};
            auto n=::pread(copy.fd_,elf,sizeof(elf),0);
            if(n<52 || elf[0]!=0x7f || elf[1]!='E' || elf[2]!='L' || elf[3]!='F' ||
                elf[4]!=(sizeof(void*)==8u?2u:1u) || elf[5]!=1u || elf[6]!=1u ||
                elf[16]!=3u || elf[17]!=0u)
                return "driver must be a matching little-endian ELF shared object";
            unsigned const machine=elf[18]+256u*elf[19];
#if defined(__x86_64__)
            if(machine!=62u)return "driver architecture is not x86_64";
#elif defined(__i386__)
            if(machine!=3u)return "driver architecture is not i686";
#elif defined(__aarch64__)
            if(machine!=183u)return "driver architecture is not AArch64";
#elif defined(__arm__)
            if(machine!=40u)return "driver architecture is not ARM";
            unsigned const flags=elf[36]|(unsigned(elf[37])<<8u)|(unsigned(elf[38])<<16u)|(unsigned(elf[39])<<24u);
#if defined(__ARM_PCS_VFP)
            if((flags&0x600u)!=0x400u)return "driver must use the ARM hard-float ABI";
#else
            if((flags&0x400u)!=0u)return "driver must use the ARM soft-float ABI";
#endif
#else
            return "unsupported trusted-driver host architecture";
#endif
            if(sizeof(void*)==8u && n!=64)return "truncated ELF64 driver";
            fd_=::std::exchange(copy.fd_,-1);
            return nullptr;
#else
            (void)path;return "trusted-driver verifier is unavailable";
#endif
        }
    };
}
