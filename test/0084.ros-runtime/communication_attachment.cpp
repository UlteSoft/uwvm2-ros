/** @file @brief Hostile control records, endpoint aliases, rollback and expiry qualification. */
#include "communication_attachment_fixture.h"
int main()
{
    unsigned cases{};
    {
        fixture f;a::connection connection;auto before=descriptors();
        assert(connection.prepare(f.configuration())==error::success);f.handshake(connection);
        connection.request_stop();assert(!connection.session_live()&&!connection.attachment_confirmed());
        connection.disconnect();assert(descriptors()==before&&::kill(::getpid(),0)==0);++cases;
    }
    {
        fixture f;a::connection connection;auto before=descriptors();
        assert(!connection.session_live());
        assert(connection.prepare(f.configuration())==error::success);f.handshake(connection);
        auto* fence=connection.stop_flag();connection.disconnect();
        assert(fence->load()&&!connection.session_live()&&!connection.attachment_confirmed());
        assert(connection.begin()==error::denied&&connection.activate_channel()==error::denied);
        assert(connection.prepare(f.configuration())==error::denied&&descriptors()==before);
        connection.disconnect();assert(descriptors()==before);++cases;
    }
    for(auto offset:{0u,8u,16u,24u,32u,48u,64u,80u,96u,112u,144u,152u,160u,168u,176u})
    {
        fixture f;a::connection connection;assert(connection.prepare(f.configuration())==error::success&&connection.begin()==error::again);
        f.response(a::ready,offset);assert(connection.begin()==error::denied&&!connection.session_live());
        assert(connection.begin()==error::denied);++cases;
    }
    {
        fixture f;a::connection connection;assert(connection.prepare(f.configuration())==error::success&&connection.begin()==error::again);
        auto before=descriptors();f.response(a::ready,SIZE_MAX,true);
        assert(connection.begin()==error::denied&&descriptors()==before&&!connection.session_live());++cases;
    }
    for(unsigned mode{};mode<4;++mode)
    {
        fixture f;auto valid=f.configuration();a::connection connection;auto before=descriptors();
        int duplicate=-1;
        if(mode==0)f.selected.pipes[0]=f.pipes[0][0];
        if(mode==1){duplicate=::fcntl(f.selected.pipes[0],F_DUPFD_CLOEXEC,3);assert(duplicate>=0);f.selected.pipes[3]=duplicate;}
        if(mode==2)assert(!::fcntl(f.selected.pipes[0],F_SETFL,0));
        if(mode==3){duplicate=::open("/dev/null",O_WRONLY|O_NONBLOCK|O_CLOEXEC);assert(duplicate>=0);f.selected.pipes[0]=duplicate;}
        auto result=connection.prepare(f.configuration());assert(result==error::denied||result==error::conflict);
        if(duplicate>=0)::close(duplicate);
        if(mode==2)assert(!::fcntl(f.pipes[0][1],F_SETFL,O_NONBLOCK));
        assert(descriptors()==before);
        assert(connection.prepare(valid)==error::success);++cases;
    }
    {
        fixture f;a::connection first,second;auto config=f.configuration();
        assert(first.prepare(config)==error::success&&second.prepare(config)==error::success&&first.overlaps(second));++cases;
    }
    {
        fixture f;f.selected.session_until=static_cast<std::uint64_t>(uwvm2::imported::ros::resources::steady_now())+100000000;
        a::connection connection;assert(connection.prepare(f.configuration())==error::success);f.handshake(connection);
        ::poll(nullptr,0,120);assert(!connection.session_live()&&!connection.attachment_confirmed()&&connection.begin()==error::denied);++cases;
    }
    {
        fixture f;f.selected.session_until=1;a::connection connection;auto before=descriptors();
        assert(connection.prepare(f.configuration())==error::invalid&&descriptors()==before);++cases;
    }
    {
        s::communication_transport::packet_queue queue;assert(queue.initialize(2,16));std::array<std::uint8_t,8> secret;secret.fill(0xa5);
        assert(queue.push(1,1,secret));auto retained=queue.remaining();queue.advance(4);
        for(std::size_t i{};i<4;++i)assert(retained[i]==0);
        queue.advance(retained.size()-4);for(auto byte:retained)assert(byte==0);assert(queue.empty());
        assert(queue.push(1,2,secret));retained=queue.remaining();queue.erase();for(auto byte:retained)assert(byte==0);++cases;
    }
    std::printf("PASS external communication attachment: %u hostile/lifetime/erasure groups\n",cases);
}
