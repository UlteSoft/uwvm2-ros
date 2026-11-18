/** @file
 * @brief Deployment-pinned mutual TLS for one preconnected local attachment.
 * @details TLS 1.3 authenticates both Ed25519 raw public keys and negotiates
 * ephemeral X25519 and AEAD. ALPN commits both identities to the service,
 * deployment, VM, scope, provider, session and schema. Kernel credentials and
 * pidfd liveness are checked beneath TLS. The provisioner gives each endpoint
 * a sealed 64-byte descriptor containing its seed and the exact other public
 * key. Guest code cannot select keys, peers, cryptographic options or sockets.
 * One TLS application write occupies one atomic seqpacket. Backpressure retains
 * a pending ciphertext only for an exact retry under fresh caller authority.
 * All plaintext is staged until a complete authenticated frame can be delivered.
 */
#pragma once
#include "attachment_tls_headers.h"
#include "driver_channel.h"
#include "erased_buffer.h"
#ifndef UWVM_MODULE
#include <fast_io_crypto.h>
#endif

namespace uwvm2::imported::ros::scene::attachment_tls
{
    using error=resources::error;
    enum class purpose : std::uint8_t { driver=1, model, deployment };
    using id=drivers::protocol::identity;
    using digest=std::array<std::uint64_t,4>;

    using protocol_label=std::array<std::uint8_t,33>;
    /** @brief Bind the complete deployment context in the authenticated TLS transcript.
     * @details ClientHello exposes an opaque fingerprint. The five identifiers
     * and the schema digest are sent in full only inside authenticated records.
     */
    inline protocol_label label(purpose service,id deployment,id vm,id scope,id provider,id session,digest contract) noexcept
    {
        constexpr std::string_view prefix="uwvm-ros/ipc";
        std::array<std::uint8_t,prefix.size()+1+5*16+32> fields{};
        std::copy(prefix.begin(),prefix.end(),fields.begin());
        fields[prefix.size()]=static_cast<std::uint8_t>(service);
        wire::writer out{std::span{fields}.subspan(prefix.size()+1)};
        for(auto field:{deployment,vm,scope,provider,session})for(auto word:field)out.u64(word);
        for(auto word:contract)out.u64(word);
        ::fast_io::sha256_context hash{};
        auto first=reinterpret_cast<std::byte const*>(fields.data());
        hash.update(first,first+fields.size());hash.do_final();
        protocol_label result{};result[0]=32;
        hash.digest_to_byte_ptr(reinterpret_cast<std::byte*>(result.data()+1));return result;
    }
    inline constexpr std::size_t maximum_frame=drivers::protocol::header_bytes+drivers::protocol::maximum_payload;
    inline constexpr std::size_t ciphertext_capacity(std::size_t frame) noexcept
    { return std::max<std::size_t>(8192,frame+2048); }
    /** @brief Charge the two BIO ring buffers, packet staging and plaintext staging.
     * @details The separate fixed charge reserves setup overhead. OpenSSL internal
     * allocation and process RSS are subject to the deployment memory limit.
     */
    inline constexpr std::size_t reservation(std::size_t frame) noexcept
    {return 262144+3*ciphertext_capacity(frame)+frame+4;}

    inline error check_identity(int fd) noexcept
    {
#if defined(UWVM_ROS_HAS_ATTACHMENT_TLS)
        if(fd<0)return error::invalid;
        constexpr int seals=F_SEAL_WRITE|F_SEAL_GROW|F_SEAL_SHRINK|F_SEAL_SEAL;
        int actual=::fcntl(fd,F_GET_SEALS);
        struct stat info{};
        if(actual<0||(actual&seals)!=seals||::fstat(fd,&info)||!S_ISREG(info.st_mode)||info.st_size!=64)return error::denied;
        return error::success;
#else
        (void)fd;return error::unsupported;
#endif
    }
#if defined(UWVM_ROS_HAS_ATTACHMENT_TLS)
    class session
    {
        /** @brief End private-context thread registrations before a serialized caller hands off the session.
         * @details OpenSSL deregisters only the destroying thread when a private library context is freed.
         * Preparation, traffic and teardown may run on distinct, still-live threads. Each use releases
         * its registrations on the same thread while the context is valid. An idle socket poll never
         * enters OpenSSL and requires no cleanup. The process-wide default context is left alone.
         */
        struct thread_registration
        {
            OSSL_LIB_CTX* context{};
            ~thread_registration() { if(context) OPENSSL_thread_stop_ex(context); }
        };

        OSSL_LIB_CTX* library_{};
        OSSL_PROVIDER* provider_{};
        SSL_CTX* context_{};
        SSL* ssl_{};
        BIO* network_{};
        EVP_PKEY* peer_key_{};
        protocol_label label_{};
        erased_buffer packed_,cipher_;
        std::size_t pending_{},retry_size_{},handshake_bytes_{};
        unsigned flights_{};
        bool want_input_{},finished_{},failed_{};

        static int select_label(SSL*,unsigned char const** out,unsigned char* length,unsigned char const* input,
                                unsigned int size,void* user) noexcept
        {
            auto const& expected=static_cast<session*>(user)->label_;
            if(size!=expected.size()||!std::equal(expected.begin(),expected.end(),input))return SSL_TLSEXT_ERR_ALERT_FATAL;
            *out=input+1;*length=input[0];return SSL_TLSEXT_ERR_OK;
        }
        error fail() noexcept
        {
            failed_=true;packed_.erase();cipher_.erase();pending_=retry_size_=0;
            return error::denied;
        }
        bool authenticated() const noexcept
        {
            unsigned char const* alpn{};unsigned int size{};
            SSL_get0_alpn_selected(ssl_,&alpn,&size);
            return SSL_version(ssl_)==TLS1_3_VERSION&&!SSL_session_reused(ssl_)&&
                SSL_get_verify_result(ssl_)==X509_V_OK&&SSL_get_negotiated_group(ssl_)==NID_X25519&&
                SSL_get_negotiated_client_cert_type(ssl_)==TLSEXT_cert_type_rpk&&
                SSL_get_negotiated_server_cert_type(ssl_)==TLSEXT_cert_type_rpk&&
                SSL_get0_peer_rpk(ssl_)&&EVP_PKEY_eq(SSL_get0_peer_rpk(ssl_),peer_key_)==1&&
                size==label_[0]&&alpn&&std::equal(label_.begin()+1,label_.end(),alpn);
        }
        bool drain_output() noexcept
        {
            auto available=BIO_ctrl_pending(network_);
            if(!available||pending_||available>cipher_.view().size())return false;
            return BIO_read_ex(network_,cipher_.writable().data(),available,&pending_)==1&&pending_==available;
        }
        error flush(drivers::channel& local) noexcept
        {
            auto status=local.send_tls_packet(cipher_.view().first(pending_));
            if(status==error::again)return status;
            if(status!=error::success)return fail();
            erase_bytes(cipher_.writable().first(pending_));pending_=0;
            return error::success;
        }
        error ingest(drivers::channel& local,bool handshake,thread_registration& use) noexcept
        {
            std::size_t size{},written{};
            auto status=local.receive(cipher_.writable(),size);
            if(status==error::again)return status;
            if(status!=error::success||!size)return fail();
            if(handshake)
            {
                if(++flights_>32||size>65536-handshake_bytes_)return fail();
                handshake_bytes_+=size;
            }
            use.context=library_;
            if(BIO_write_ex(network_,cipher_.view().data(),size,&written)!=1||written!=size)return fail();
            erase_bytes(cipher_.writable().first(size));return error::success;
        }
        error advance(drivers::channel& local,thread_registration& use) noexcept
        {
            if(failed_)return error::denied;
            if(finished_&&!pending_)return error::success;
            if(pending_)
            {
                auto status=flush(local);if(status!=error::success)return status;
                if(finished_)return error::success;
            }
            if(want_input_)
            {
                auto status=ingest(local,true,use);if(status!=error::success)return status;
                want_input_=false;
            }
            use.context=library_;
            ERR_clear_error();
            int completed=SSL_do_handshake(ssl_);
            if(completed==1)
            {
                if(!authenticated()||SSL_has_pending(ssl_)||BIO_ctrl_pending(SSL_get_rbio(ssl_)))return fail();
                finished_=true;
            }
            else
            {
                if(SSL_get_error(ssl_,completed)!=SSL_ERROR_WANT_READ)return fail();
                want_input_=true;
            }
            if(BIO_ctrl_pending(network_))
            {
                if(!drain_output())return fail();
                auto status=flush(local);if(status!=error::success)return status;
            }
            return finished_?error::success:error::again;
        }
    public:
        session()=default;session(session const&)=delete;session& operator=(session const&)=delete;
        ~session()
        {
            SSL_free(ssl_);BIO_free(network_);EVP_PKEY_free(peer_key_);SSL_CTX_free(context_);
            if(provider_)OSSL_PROVIDER_unload(provider_);
            OSSL_LIB_CTX_free(library_);
        }
        /** @brief Configure the exact peer before disclosure. This performs no channel I/O. */
        error prepare(int identity_fd,protocol_label name,std::size_t maximum,bool server)
        {
            if(ssl_||maximum<drivers::protocol::header_bytes||maximum>maximum_frame)return error::invalid;
            auto status=check_identity(identity_fd);if(status!=error::success)return status;
            struct key_buffer{std::array<std::uint8_t,64> data{};~key_buffer(){erase_bytes(data);}} keys;
            if(::pread(identity_fd,keys.data.data(),64,0)!=64)return error::io;
            library_=OSSL_LIB_CTX_new();
            if(!library_)return error::no_memory;
            thread_registration use{library_};
            if(!(provider_=OSSL_PROVIDER_load(library_,"default")))return error::no_memory;
            context_=SSL_CTX_new_ex(library_,"provider=default",TLS_method());
            if(!context_)return error::no_memory;
            constexpr unsigned char raw=TLSEXT_cert_type_rpk;
            if(!SSL_CTX_set_min_proto_version(context_,TLS1_3_VERSION)||!SSL_CTX_set_max_proto_version(context_,TLS1_3_VERSION)||
               !SSL_CTX_set1_groups_list(context_,"X25519")||!SSL_CTX_set1_sigalgs_list(context_,"ed25519")||
               !SSL_CTX_set_ciphersuites(context_,"TLS_AES_128_GCM_SHA256")||
               !SSL_CTX_set1_client_cert_type(context_,&raw,1)||!SSL_CTX_set1_server_cert_type(context_,&raw,1)||
               !SSL_CTX_set_num_tickets(context_,0)||!SSL_CTX_set_max_early_data(context_,0)||!SSL_CTX_dane_enable(context_))return error::unsupported;
            SSL_CTX_set_options(context_,SSL_OP_NO_TICKET|SSL_OP_NO_COMPRESSION|SSL_OP_NO_RENEGOTIATION|SSL_OP_CLEANSE_PLAINTEXT);
            SSL_CTX_set_session_cache_mode(context_,SSL_SESS_CACHE_OFF);
            SSL_CTX_set_max_cert_list(context_,4096);
            SSL_CTX_set_verify(context_,SSL_VERIFY_PEER|SSL_VERIFY_FAIL_IF_NO_PEER_CERT,nullptr);
            label_=name;SSL_CTX_set_alpn_select_cb(context_,select_label,this);
            ssl_=SSL_new(context_);if(!ssl_)return error::no_memory;
            EVP_PKEY* mine=EVP_PKEY_new_raw_private_key_ex(library_,"ED25519","provider=default",keys.data.data(),32);
            peer_key_=EVP_PKEY_new_raw_public_key_ex(library_,"ED25519","provider=default",keys.data.data()+32,32);
            bool assigned=mine&&peer_key_&&EVP_PKEY_eq(mine,peer_key_)==0&&SSL_use_PrivateKey(ssl_,mine)==1;
            EVP_PKEY_free(mine);if(!assigned)return error::invalid;
            if(SSL_dane_enable(ssl_,server?nullptr:"peer.invalid")<=0||SSL_add_expected_rpk(ssl_,peer_key_)!=1)return error::invalid;
            if(server)SSL_set_accept_state(ssl_);
            else
            {
                if(SSL_set_alpn_protos(ssl_,label_.data(),static_cast<unsigned>(label_.size()))!=0)return error::invalid;
                SSL_set_connect_state(ssl_);
            }
            packed_.resize_for_overwrite(maximum+4);
            cipher_.resize_for_overwrite(ciphertext_capacity(maximum));
            BIO* internal{};
            if(BIO_new_bio_pair(&internal,cipher_.view().size(),&network_,cipher_.view().size())!=1)return error::no_memory;
            SSL_set_bio(ssl_,internal,internal);return error::success;
        }
        error send(drivers::channel& local,wire::bytes header,wire::bytes body) noexcept
        {
            if(failed_)return error::denied;
            thread_registration use{};
            auto header_size=drivers::protocol::header_bytes;
            if(header.size()!=header_size||body.size()>packed_.view().size()-4-header_size)return error::invalid;
            auto size=header_size+body.size();
            if(retry_size_)
            {
                auto bytes=packed_.view().subspan(4);
                if(retry_size_!=size||CRYPTO_memcmp(bytes.data(),header.data(),header_size)||
                   (!body.empty()&&CRYPTO_memcmp(bytes.data()+header_size,body.data(),body.size())))return fail();
            }
            else
            {
                auto status=advance(local,use);if(status!=error::success)return status;
                auto bytes=packed_.writable().first(size+4);
                for(unsigned n{};n<4;++n)bytes[n]=static_cast<std::uint8_t>(size>>(8*n));
                std::copy(header.begin(),header.end(),bytes.begin()+4);
                std::copy(body.begin(),body.end(),bytes.begin()+4+header_size);
                std::size_t written{};
                use.context=library_;
                if(SSL_write_ex(ssl_,bytes.data(),bytes.size(),&written)!=1||written!=bytes.size()||!drain_output())return fail();
                retry_size_=size;
            }
            auto status=flush(local);
            if(status==error::success){erase_bytes(packed_.writable().first(retry_size_+4));retry_size_=0;}
            return status;
        }
        error receive(drivers::channel& local,std::span<std::uint8_t> output,std::size_t& count) noexcept
        {
            if(failed_)return error::denied;
            if(retry_size_)return error::again;
            thread_registration use{};
            auto status=advance(local,use);if(status!=error::success)return status;
            status=ingest(local,false,use);if(status!=error::success)return status;
            std::size_t used{};
            auto plain=packed_.writable();
            do
            {
                std::size_t received{};
                if(used==plain.size()||SSL_read_ex(ssl_,plain.data()+used,plain.size()-used,&received)!=1||!received)return fail();
                used+=received;
            }while(SSL_has_pending(ssl_)||BIO_ctrl_pending(SSL_get_rbio(ssl_)));
            if(used<4+drivers::protocol::header_bytes||used-4>output.size()||BIO_ctrl_pending(network_))return fail();
            std::uint32_t length{};
            for(unsigned n{};n<4;++n)length|=static_cast<std::uint32_t>(plain[n])<<(8*n);
            if(length!=used-4)return fail();
            std::copy_n(plain.begin()+4,length,output.begin());
            erase_bytes(plain.first(used));count=length;return error::success;
        }
    };
#endif
    /** @brief One provisioned session with an immutable transport choice.
     * @details The caller serializes this object and retains its authorization
     * across each send. Failing an encrypted session can only close that session.
     */
    class channel
    {
        drivers::channel local_;
#if defined(UWVM_ROS_HAS_ATTACHMENT_TLS)
        ::uwvm2::utils::container::std_unique_ptr<session> secure_;
#endif
    public:
        error bind(int fd,drivers::credentials peer,int key_fd,purpose service,id deployment,id vm,id scope,id provider,
                   id session_id,digest contract,std::size_t maximum,bool server=false,int peer_time_namespace_fd=-1)
        {
            if(key_fd<-1)return error::invalid;
            if(key_fd==-1)return local_.bind(fd,peer,peer_time_namespace_fd);
#if defined(UWVM_ROS_HAS_ATTACHMENT_TLS)
            auto candidate=::uwvm2::utils::container::make_unique<session>();
            auto status=candidate->prepare(key_fd,label(service,deployment,vm,scope,provider,session_id,contract),maximum,server);if(status!=error::success)return status;
            status=local_.bind(fd,peer,peer_time_namespace_fd);if(status==error::success)secure_=std::move(candidate);
            return status;
#else
            (void)service;(void)deployment;(void)vm;(void)scope;(void)provider;(void)session_id;(void)contract;(void)maximum;(void)server;(void)peer_time_namespace_fd;return error::unsupported;
#endif
        }
        bool same_endpoint(channel const& other)const noexcept{return local_.same_endpoint(other.local_);}
        auto endpoint_identity()const noexcept{return local_.endpoint_identity();}
        explicit operator bool()const noexcept{return static_cast<bool>(local_);}
        void close()noexcept
        {
            local_.close();
#if defined(UWVM_ROS_HAS_ATTACHMENT_TLS)
            secure_.reset();
#endif
        }
        error send(wire::bytes header,wire::bytes body)noexcept
        {
#if defined(UWVM_ROS_HAS_ATTACHMENT_TLS)
            if(secure_)return local_.same_process()?secure_->send(local_,header,body):error::denied;
#endif
            return local_.send(header,body);
        }
        error receive(std::span<std::uint8_t> output,std::size_t& count)noexcept
        {
#if defined(UWVM_ROS_HAS_ATTACHMENT_TLS)
            if(secure_)return local_.same_process()?secure_->receive(local_,output,count):error::denied;
#endif
            return local_.receive(output,count);
        }
    };
}
