/** @file @brief Real external model inference; failure never becomes false cleanup. */
#include <uwvm2/imported/ros/scene/model_attachment_configuration.h>
#include <sys/wait.h>
#include <signal.h>
#include <cassert>
#include <cstdio>
#include <vector>
#include <thread>
namespace s=uwvm2::imported::ros::scene;
namespace c=uwvm2::imported::ros::model_contract;
using s::error;
static s::buffers::profile profile(std::string_view name)
{s::buffers::profile value;assert(value.identity.assign(name));value.sha256.fill(42);return value;}
static s::models::plan model_plan()
{
    s::models::plan p;p.model=profile("matrix.cpu");p.task=profile("matrix.multiply");
    assert(p.parameter_type.assign("mode")&&p.output_type.assign("matrix.result")&&p.encoding.assign("bytes"));
    p.streaming=true;p.sessions=2;p.requests=2;p.input_bytes=32;p.input_count=1;p.parameter_bytes=8;p.output_bytes=64;p.output_items=4;p.workspace_bytes=32768;p.deadline_ns=10000000000ull;
    p.validate_parameters=[](s::wire::bytes data) noexcept {s::wire::reader reader{data};std::uint64_t mode{};return reader.u64(mode)&&mode>=1&&mode<=5&&reader.done();};
    p.validate_output=[](s::wire::bytes data,bool) noexcept {return data.size()==16||data.size()==8;};return p;
}
static std::vector<std::uint8_t> words(std::initializer_list<std::uint64_t> values)
{std::vector<std::uint8_t> result(values.size()*8);s::wire::writer writer{result};for(auto value:values)assert(writer.u64(value));return result;}
struct peer_input{int fd;pid_t pid;s::model_attachment::protocol::binding binding;};
struct fixture
{
    std::shared_ptr<s::authority> permissions=uwvm2::utils::container::make_shared<s::authority>();
    s::buffers::registry buffers;s::model_dispatch dispatcher{permissions,buffers};
    std::shared_ptr<s::models::provider_lifetime> worker;
    int instance{};s::caller who{&instance,1};s::handle model_cap{},data_cap{},input{},session{};
    fixture(peer_input source)
    {
        assert(permissions->expose("capability.open",s::exposure::allow)==error::success);
        for(auto const& operation:c::operations)assert(permissions->expose(uwvm2::utils::container::std_string(operation.name),s::exposure::allow)==error::success);
        assert(permissions->expose("buffer.read",s::exposure::allow)==error::success);
        s::grant model;model.alias="brain";model.resource="matrix";model.kind=s::purpose::vision_model;model.binding="local-model";model.max_resources=model.max_handles=2;
        for(auto const& operation:c::operations)model.actions.emplace(operation.name);
        s::grant data;data.alias="data";data.resource="tensor";data.kind=s::purpose::buffer;data.binding="sealed-buffer";data.actions={"buffer.read"};data.max_handles=2;
        assert(permissions->configure(std::move(model))==error::success&&permissions->configure(std::move(data))==error::success);
        assert(permissions->enroll(who)==error::success&&permissions->open(who,"brain",model_cap)==error::success&&permissions->open(who,"data",data_cap)==error::success);
        s::buffers::plan b;b.schema=profile("tensor");b.objects=2;b.bytes=32;b.object_bytes=b.transfer_bytes=16;b.workspace_bytes=32;b.leases=2;b.elements=16;b.rank=1;b.element_mask=1;b.order_mask=1;b.color_mask=1;
        assert(buffers.bind("data",b)==error::success&&dispatcher.bind("brain",model_plan())==error::success);
        auto prepared=s::model_attachment::service::prepare(dispatcher,"brain",1,source.fd,{static_cast<std::uint64_t>(source.pid),::getuid(),::getgid()},source.binding,
            {4096,4096,1000000000,100000,1048576,s::models::now()+20000000000ull},worker);
        if(prepared!=error::success)std::fprintf(stderr,"service prepare=%u\n",static_cast<unsigned>(prepared));
        assert(prepared==error::success);
        s::buffers::layout layout;layout.schema=b.schema;layout.rank=1;layout.shape[0]=layout.bytes=16;layout.strides[0]=1;
        assert(buffers.create("data",layout,2,[&](auto id){input=id;return true;})==error::success);
        auto values=words({std::bit_cast<std::uint64_t>(5.0),std::bit_cast<std::uint64_t>(6.0)});std::uint64_t accepted{};
        assert(buffers.write("data",input,0,values,accepted)==error::success&&buffers.seal("data",input,s::buffers::hash(values))==error::success);
        std::array<std::uint8_t,160> encoded{};s::wire::writer writer{encoded};assert(c::write_profile(writer,model_plan().model));
        std::vector<std::uint8_t> result;assert(call(c::operation::open,writer.written(),result)==error::success);s::wire::reader reader{result};assert(reader.u64(session)&&reader.done());
    }
    ~fixture(){if(worker)worker->retire();}
    error call(c::operation action,s::wire::bytes input,std::vector<std::uint8_t>& output)
    {
        auto copy=[&](s::wire::bytes bytes){output.assign(bytes.begin(),bytes.end());return true;};std::size_t required{};
        return dispatcher.invoke(who,action,model_cap,input,4096,required,copy);
    }
    s::handle submit(std::uint64_t mode,std::uint64_t lifetime=5000000000ull)
    {
        std::array<std::uint8_t,512> encoded{};s::wire::writer writer{encoded};auto parameters=words({mode});
        assert(writer.u64(session)&&writer.u64(1)&&writer.u64(session)&&c::write_profile(writer,model_plan().task)&&writer.text("mode")&&writer.text("bytes")&&writer.blob(parameters));
        assert(writer.u64(1)&&writer.u64(data_cap)&&writer.u64(input)&&writer.u64(0)&&writer.u64(0)&&writer.boolean(false)&&writer.boolean(false));
        assert(writer.u64(32)&&writer.u64(2)&&writer.u64(8192)&&writer.u64(s::models::now()+lifetime));
        std::vector<std::uint8_t> result;assert(call(c::operation::submit,writer.written(),result)==error::success);s::wire::reader reader{result};s::handle request{};assert(reader.u64(request));return request;
    }
    std::string phase(s::handle request)
    {
        std::vector<std::uint8_t> result;assert(call(c::operation::request_describe,words({request}),result)==error::success);
        s::wire::reader reader{result};std::uint64_t id{};std::string_view phase;assert(reader.u64(id)&&id==request&&reader.text(phase));return std::string{phase};
    }
    std::string wait(s::handle request,bool running=false)
    {
        auto deadline=s::models::now()+6000000000ull;std::string observed;
        do {observed=phase(request);if(running?observed=="running":observed!="queued"&&observed!="running")return observed;::poll(nullptr,0,1);}while(s::models::now()<deadline);
        std::fprintf(stderr,"timed out phase=%s\n",observed.c_str());assert(false);return {};
    }
    void release(s::handle request)
    {
        std::vector<std::uint8_t> result;error status;auto deadline=s::models::now()+5000000000ull;
        do {status=call(c::operation::request_release,words({request}),result);if(status==error::again)::poll(nullptr,0,1);}while(status==error::again&&s::models::now()<deadline);
        assert(status==error::success);s::buffers::usage use;assert(buffers.inspect("data",use)==error::success&&use.leases==0);
    }
};
namespace p=s::model_attachment::protocol;
static p::binding binding()
{return {{1,2},{3,4},{5,6},{7,8},{9,10},{11,12,13,14}};}
static void peer(int fd,pid_t parent,unsigned mode)
{
    s::drivers::channel channel;
    assert(channel.bind(fd,{static_cast<std::uint64_t>(parent),::getuid(),::getgid()})==error::success);::close(fd);
    std::array<std::uint8_t,p::header_bytes+p::fragment_bytes> packet{};
    auto receive=[&](p::header& header,s::wire::bytes& body)
    {
        std::size_t size{};error status;auto deadline=s::models::now()+10000000000ull;
        do{status=channel.receive(packet,size);if(status==error::again)::poll(nullptr,0,1);}while(status==error::again&&s::models::now()<deadline);
        return status==error::success&&p::decode(std::span{packet}.first(size),header,body);
    };
    auto send=[&](p::header const& h,s::wire::bytes body)
    {
        std::array<std::uint8_t,p::header_bytes> header{};assert(p::encode(h,body.size(),header));
        error status;do{status=channel.send(header,body);if(status==error::again)::poll(nullptr,0,1);}while(status==error::again);
        return status==error::success;
    };
    p::header h;s::wire::bytes body;assert(receive(h,body)&&h.type==p::kind::hello&&h.bound==binding());
    auto hello=h;hello.type=p::kind::ready;assert(send(hello,body));
    std::vector<std::uint8_t> request;
    for(;;)
    {
        if(!receive(h,body))break;
        if(h.type==p::kind::release)break;
        assert(h.bound==binding()&&h.sequence==1&&h.offset==request.size());
        if(h.type==p::kind::request_end)break;
        assert(h.type==p::kind::request);request.insert(request.end(),body.begin(),body.end());
    }
    if(request.empty()){while(::kill(parent,0)==0)::poll(nullptr,0,1);::_exit(0);}
    s::model_worker::invocation call;assert(s::model_worker::decode(request,call));
    if(mode==2){::signal(SIGSEGV,SIG_DFL);::raise(SIGSEGV);::_exit(99);}
    if(mode==3){while(::kill(parent,0)==0)::poll(nullptr,0,1);::_exit(0);}
    auto emit=[&](s::model_worker::response kind,s::wire::bytes values,bool malformed=false)
    {
        std::array<std::uint8_t,128> data{};s::wire::writer writer{data};
        assert(writer.u64(static_cast<std::uint64_t>(kind))&&writer.u64(call.request)&&writer.blob(values));
        auto reply=h;reply.type=p::kind::response;reply.offset=0;
        if(mode==4)++reply.sequence;
        if(mode==5)++reply.bound.provider[0];
        if(mode==6)++reply.deadline;
        if(mode==8)reply.offset=1;
        if(mode==7)
        {
            int duplicate=::open("/dev/null",O_RDONLY);assert(duplicate>=0);
            std::array<std::uint8_t,p::header_bytes> header{};assert(p::encode(reply,writer.size(),header));
            ::iovec parts[2]{{header.data(),header.size()},{data.data(),writer.size()}};
            alignas(cmsghdr) std::array<unsigned char,CMSG_SPACE(sizeof(int))> ancillary{};
            ::msghdr message{};message.msg_iov=parts;message.msg_iovlen=2;message.msg_control=ancillary.data();message.msg_controllen=ancillary.size();
            auto* control=CMSG_FIRSTHDR(&message);control->cmsg_level=SOL_SOCKET;control->cmsg_type=SCM_RIGHTS;control->cmsg_len=CMSG_LEN(sizeof(int));
            std::memcpy(CMSG_DATA(control),&duplicate,sizeof(int));
            // fd is retained separately by this test peer, solely to inject ancillary data.
            assert(::sendmsg(198,&message,MSG_NOSIGNAL)>0);::close(duplicate);return;
        }
        // Small fragments exercise exact offset accounting without large fixture buffers.
        for(std::size_t offset{};offset<writer.size();offset+=8)
        {
            reply.offset=mode==8?1:offset;
            if(!send(reply,writer.written().subspan(offset,std::min<std::size_t>(8,writer.size()-offset))))return;
            if(malformed)return;
        }
        reply.type=p::kind::response_end;reply.offset=writer.size();(void)send(reply,{});
    };
    auto values=words({std::bit_cast<std::uint64_t>(17.0),std::bit_cast<std::uint64_t>(39.0)});
    if(mode==1)emit(s::model_worker::response::partial,words({42}));
    emit(s::model_worker::response::succeeded,values,mode>=4);
    // Deliberately remain alive after release. VM teardown must not kill us.
    while(::kill(parent,0)==0)::poll(nullptr,0,1);
    ::_exit(0);
}
struct external
{
    pid_t pid{};int fd{-1};
    external(unsigned mode)
    {
        int sockets[2];assert(!::socketpair(AF_UNIX,SOCK_SEQPACKET|SOCK_CLOEXEC,0,sockets));
        int one=1;for(auto socket:sockets)assert(!::setsockopt(socket,SOL_SOCKET,SO_PASSCRED,&one,sizeof(one)));
        auto parent=::getpid();pid=::fork();assert(pid>=0);
        if(!pid){::close(sockets[0]);assert(::dup2(sockets[1],198)==198);peer(sockets[1],parent,mode);::_exit(0);}
        ::close(sockets[1]);fd=sockets[0];
    }
    ~external(){::close(fd);int status{};if(::waitpid(pid,&status,WNOHANG)==0){assert(!::kill(pid,SIGKILL));assert(::waitpid(pid,&status,0)==pid);}}
    peer_input input()const{return {fd,pid,binding()};}
    bool alive()const{return !::kill(pid,0);}
};
int main()
{
    for(unsigned mode=0;mode<9;++mode)
    {
        external source{mode};fixture f{source.input()};auto request=f.submit(1,mode==3?50000000:5000000000ull);
        if(mode<=1)
        {
            assert(f.wait(request)=="succeeded");f.release(request);
            f.worker->retire();assert(source.alive());
        }
        else
        {
            assert(f.wait(request)=="outcome_unknown");f.worker->retire();
            s::buffers::usage use;assert(f.buffers.inspect("data",use)==error::success&&use.leases==1);
            if(mode!=2)assert(source.alive());
            std::vector<std::uint8_t> out;assert(f.call(c::operation::request_release,words({request}),out)==error::again);
        }
    }
    {
        external source{3};fixture f{source.input()};auto request=f.submit(1);assert(f.wait(request,true)=="running");
        // Wait until input disclosure, then revoke its exact source capability.
        ::poll(nullptr,0,20);
        assert(f.permissions->revoke("data",[&](auto alias){f.buffers.revoke(alias);})==error::success);
        assert(f.wait(request)=="outcome_unknown");f.worker->retire();assert(source.alive());
    }
    {
        s::model_attachment::configuration config;
        auto limits=s::model_attachment::limits{4096,4096,1000000000,100000,1048576,s::models::now()+10000000000ull};
        assert(config.freeze(model_plan())==error::invalid);
        assert(config.channel(3,{1,0,0},binding(),limits)==error::success);
        assert(config.channel(3,{1,0,0},binding(),limits)==error::conflict);
        assert(config.freeze(model_plan())==error::success);
        assert(config.channel(3,{1,0,0},binding(),limits)==error::denied);
        limits.poll_ns=0;assert(!s::model_attachment::valid(limits));
        std::array<std::uint8_t,p::header_bytes> encoded{};
        assert(p::encode({p::kind::release,binding()},0,encoded));
        p::header h;auto sentinel=h;s::wire::bytes body;encoded[0]^=1;
        assert(!p::decode(encoded,h,body)&&h==sentinel);
    }
    std::puts("passed 11 external model security groups");
}
