#include <uwvm2/imported/ros/ipc/unix.h>
#include <atomic>
#include <condition_variable>
#include <exception>
#include <fast_io.h>
#include <fast_io_unit/string.h>
#include <cstring>
#include <cstdlib>
#include <new>
#include <stdexcept>
#include <string_view>
#include <thread>
#include <sys/syscall.h>

namespace test
{
    namespace ipc=uwvm2::imported::ros::ipc;
    thread_local std::size_t delayed_size{};
    thread_local unsigned delay_ms{},allocation_hits{};
    void require(bool value,char const* reason) { if(!value) { throw std::runtime_error(reason); } }
    void pause_allocation(std::size_t size)
    {
        if(size!=delayed_size||!delayed_size) { return; }
        delayed_size=0;++allocation_hits;std::this_thread::sleep_for(std::chrono::milliseconds(delay_ms));
    }
    struct peer
    {
        std::string directory,path;
        int listener{-1},fd{-1};
        std::thread thread;
        std::mutex mutex;
        std::condition_variable changed;
        bool holding{},release{};
        std::vector<ipc::frame> received;
        std::exception_ptr failure;
        peer()
        {
            char temp[]{"/tmp/uwvm-client-deadline-XXXXXX"};auto p{::mkdtemp(temp)};require(p,"private socket directory");directory=p;path=directory+"/s";
            listener=::socket(AF_UNIX,SOCK_STREAM|SOCK_CLOEXEC,0);require(listener>=0,"listener");
            sockaddr_un address{};address.sun_family=AF_UNIX;require(path.size()<sizeof address.sun_path,"socket name");
            std::memcpy(address.sun_path,path.c_str(),path.size()+1);
            require(!::bind(listener,reinterpret_cast<sockaddr*>(&address),sizeof address)&&!::chmod(path.c_str(),0600)&&!::listen(listener,1),"bind private listener");
            thread=std::thread{[this] { serve(); }};
        }
        void serve() noexcept
        {
            try
            {
                auto accepted{::accept4(listener,nullptr,nullptr,SOCK_CLOEXEC)};require(accepted>=0,"accept");
                { std::lock_guard<std::mutex> lock{mutex};fd=accepted; }
                ipc::frame hello;require(ipc::read_frame(accepted,hello,ipc::after(5000))&&hello.type==ipc::kind::hello,"hello");
                ipc::frame answer;answer.type=ipc::kind::response;answer.backend=hello.backend;answer.session_id=42;answer.response_id=hello.request_id;answer.timeout_ms=5000;
                require(ipc::write_frame(accepted,answer,ipc::after(5000)),"hello response");
                ipc::frame request;
                while(ipc::read_frame(accepted,request,ipc::after(5000)))
                {
                    ipc::request decoded;require(ipc::decode_request(request.payload,decoded),"request decode");
                    std::unique_lock<std::mutex> lock{mutex};received.push_back(request);
                    if(decoded.operation=="hold")
                    { holding=true;changed.notify_all();require(changed.wait_for(lock,std::chrono::seconds(3),[this] { return release; }),"release held response"); }
                    lock.unlock();answer.response_id=request.request_id;require(ipc::write_frame(accepted,answer,ipc::after(3000)),"operation response");
                }
            }
            catch(...) { std::lock_guard<std::mutex> lock{mutex};failure=std::current_exception();changed.notify_all(); }
        }
        void connect(ipc::client& client) { require(!client.connect(path,2,::geteuid(),1000),"authenticated client"); }
        void wait_held()
        { std::unique_lock<std::mutex> lock{mutex};require(changed.wait_for(lock,std::chrono::seconds(2),[this] { return holding||failure; })&&holding,"first request reached peer"); }
        void unblock() { std::lock_guard<std::mutex> lock{mutex};release=true;changed.notify_all(); }
        std::vector<ipc::frame> snapshot() { std::lock_guard<std::mutex> lock{mutex};return received; }
        void check() { std::lock_guard<std::mutex> lock{mutex};if(failure) { std::rethrow_exception(failure); } }
        ~peer()
        {
            unblock();{ std::lock_guard<std::mutex> lock{mutex};if(fd>=0) { ::shutdown(fd,SHUT_RDWR); } }
            ::shutdown(listener,SHUT_RDWR);if(thread.joinable()) { thread.join(); }if(fd>=0) { ::close(fd); }::close(listener);::unlink(path.c_str());::rmdir(directory.c_str());
        }
    };
    struct joining
    { peer& server;std::thread& first;std::thread& second;~joining() { server.unblock();if(first.joinable()) { first.join(); }if(second.joinable()) { second.join(); } } };
    void queue(bool expired)
    {
        peer server;ipc::client client;server.connect(client);ipc::call_result a,b;std::thread first,second;joining cleanup{server,first,second};
        first=std::thread{[&] { a=client.call("hold","alias",{},2000); }};server.wait_held();
        std::atomic<int> tid{};auto started{std::chrono::steady_clock::now()};
        second=std::thread{[&] { tid=static_cast<int>(::syscall(SYS_gettid));b=client.call("queued","alias",{},expired?30u:1000u); }};
        // Observe the actual waiter rather than assuming that creating a thread
        // puts it inside client::call. The only mutex it can wait for here is
        // the client's serialization mutex held by the first request.
        bool waiting{};auto until{ipc::after(1000)};
        while(std::chrono::steady_clock::now()<until&&!waiting)
        {
            char name[128]{},state[64]{};auto id{tid.load()};if(id)
            {
                // Format into fixed stack storage: this observation must not
                // introduce an allocation into the unit's allocation barriers.
                fast_io::obuffer_view path{name,name+sizeof name-1};
                fast_io::io::print(path,"/proc/self/task/",id,"/wchan");*path.end()='\0';
                try
                {
                    // procfs is live pseudo-file content, never a mapped file.
                    // One read has a 63-byte bound, matching a 64-byte fgets range;
                    // a missing/empty/unreadable observation remains pending.
                    fast_io::native_file input{fast_io::mnp::os_c_str(name),fast_io::open_mode::in};
                    auto end{fast_io::operations::read_some(input,state,state+sizeof state-1)};
                    waiting=std::string_view{state,static_cast<std::size_t>(end-state)}.find("futex")!=std::string_view::npos;
                }
                catch(...) {}
            }
            if(!waiting) { std::this_thread::sleep_for(std::chrono::milliseconds(1)); }
        }
        require(waiting,"actual client mutex waiter");std::this_thread::sleep_for(std::chrono::milliseconds(130));server.unblock();first.join();second.join();
        auto calls{server.snapshot()};
        fast_io::io::println("OBSERVED mode=queue-",std::string_view{expired?"expired":"remaining"}," error=",b.error," peer_requests=",calls.size()," queued_wire_budget=",calls.size()>1?calls[1].timeout_ms:0u," actual_wait_ms=",static_cast<long long>(std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now()-started).count()));
        require(!a.error&&a.payload.empty(),"first held operation");require(b.payload.empty(),"queued output empty");
        if(expired) { require(b.error==110&&calls.size()==1,"expired queued client request was transmitted"); }
        else { require(!b.error&&calls.size()==2&&calls[1].timeout_ms>0&&calls[1].timeout_ms<=870,"queued call reset wire timeout"); }
        require(!client.call("next","alias",{},1000).error,"unwritten expiry must preserve live session");
        auto all{server.snapshot()};require(all.size()==calls.size()+1&&all.back().session_id==42,"same session next request");client.close();server.check();
    }
    void encode(bool expired)
    {
        peer server;ipc::client client;server.connect(client);std::vector<std::uint8_t> data(4096,0x5a);
        allocation_hits=0;delay_ms=130;delayed_size=8+6+5+data.size();
        auto reply{client.call("encode","alias",std::move(data),expired?30u:1000u)};delayed_size=0;
        auto calls{server.snapshot()};
        fast_io::io::println("OBSERVED mode=encode-",std::string_view{expired?"expired":"remaining"}," error=",reply.error," peer_requests=",calls.size()," allocation_hits=",allocation_hits," wire_budget=",calls.empty()?0u:calls.front().timeout_ms);
        require(allocation_hits==1,"exact final request wire allocation barrier");
        if(expired) { require(reply.error==110&&reply.payload.empty()&&calls.empty(),"expired encoded client request was transmitted"); }
        else { require(!reply.error&&reply.payload.empty()&&calls.size()==1&&calls.front().timeout_ms>0&&calls.front().timeout_ms<=870,"encoded call reset wire timeout"); }
        require(!client.call("next","alias",{},1000).error,"encode expiry must preserve live session");client.close();server.check();
    }
}
void* operator new(std::size_t size) { test::pause_allocation(size);if(auto p=std::malloc(size?size:1)) { return p; }throw std::bad_alloc{}; }
void* operator new[](std::size_t size) { return ::operator new(size); }
void operator delete(void* p) noexcept { std::free(p); }
void operator delete[](void* p) noexcept { std::free(p); }
void operator delete(void* p,std::size_t) noexcept { std::free(p); }
void operator delete[](void* p,std::size_t) noexcept { std::free(p); }
int main(int argc,char** argv)
{
    try
    {
        std::string mode{argc==2?argv[1]:"all"};test::require(argc<=2&&(mode=="all"||mode=="queue-expired"||mode=="queue-remaining"||mode=="encode-expired"||mode=="encode-remaining"),"invalid mode");
        // Leave enough time for the independent driver to pin this actual ELF.
        std::this_thread::sleep_for(std::chrono::milliseconds(80));
        if(mode=="all"||mode=="queue-expired") { test::queue(true); }
        if(mode=="all"||mode=="queue-remaining") { test::queue(false); }
        if(mode=="all"||mode=="encode-expired") { test::encode(true); }
        if(mode=="all"||mode=="encode-remaining") { test::encode(false); }
        fast_io::io::println("PASS client call entry deadline, no unwritten expired frame, remaining wire budget, same session retry mode=",mode);return 0;
    }
    catch(std::exception const& error) { test::delayed_size=0;fast_io::io::perrln("FAIL ",fast_io::mnp::os_c_str(error.what()));return 1; }
}
