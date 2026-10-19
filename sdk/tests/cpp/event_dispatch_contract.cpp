/** @file
 * @brief Production EventSubscription/Executor with explicit local fake imports.
 * No ROS, Runtime or network success is inferred. Compile with -UNDEBUG; actual
 * execution belongs to the guarded Linux stage, not source authoring.
 */
#include <roswasm/roswasmcpp.hpp>
#include <cassert>
#include <cstring>
static uint64_t sequence=1,source_id=2,session_id=19;static unsigned polls,closes;
static bool malformed,idle;
static void put(unsigned char* p,uint64_t n){for(unsigned i=0;i<8;++i)p[i]=static_cast<unsigned char>(n>>(8*i));}
static uint32_t open_owner(uint32_t,uint64_t alias,uint64_t length,uint64_t slot){put(reinterpret_cast<unsigned char*>(uintptr_t(slot)),length==6&&std::memcmp(reinterpret_cast<void*>(uintptr_t(alias)),"source",6)==0?2:1);return 0;}
static uint32_t close_owner(uint64_t){closes++;return 0;}
static uint32_t watch(uint64_t o,uint64_t s,uint64_t c){return o==1&&s==2&&!c?0:13;}
static uint32_t poll(uint64_t,uint32_t,uint64_t out,uint64_t cap,uint64_t slot){
    polls++;if(idle)return 11;put(reinterpret_cast<unsigned char*>(uintptr_t(slot)),64);if(cap<64)return 90;
    auto p=reinterpret_cast<unsigned char*>(uintptr_t(out));std::memset(p,0,64);std::memcpy(p,"UWRE",4);p[4]=1;p[6]=1;p[8]=2;
    put(p+16,source_id);put(p+32,sequence);put(p+40,17);put(p+48,session_id);put(p+56,malformed?1:0);return 0;
}
int main(){
    roswasmc_backend backend{};backend.version=2;backend.open=open_owner;backend.close=close_owner;roswasmc_context context{&backend};
    roswasmcpp::Capability source{context,"source"},observer{context,"events"};roswasmc_event_backend api{2,watch,poll,nullptr,nullptr};
    roswasmcpp::EventStream stream{observer,source,api};roswasmcpp::EventSubscription entity{stream,64,{}};
    roswasmcpp::Executor executor;unsigned calls{};entity.on_event([&](auto const& h,auto body){
        assert(h.sequence==1&&body.empty());calls++;auto before=polls;assert(entity.poll_once()==17&&polls==before);
        assert(entity.close()==17&&closes==0);entity.on_event([&](auto const& next,auto){assert(next.sequence==2);calls++;});
    });
    auto id=executor.add(entity);assert(id);size_t delivered{};assert(executor.spin_some(1,delivered)==0&&delivered==1&&calls==1);
    sequence=2;assert(executor.spin_some(1,delivered)==0&&delivered==1&&calls==2);
    assert(executor.spin_some(1,delivered)==22&&calls==2);
    sequence=3;source_id=3;assert(entity.poll_once()==22&&calls==2);source_id=2;
    session_id=20;assert(entity.poll_once()==22&&calls==2);session_id=19;
    malformed=true;assert(entity.poll_once()==22&&calls==2);malformed=false;
    idle=true;assert(executor.spin_some(1,delivered)==0&&delivered==0);executor.remove(id);
    assert(entity.close()==0&&source.is_open());assert(entity.poll_once()==9);
}
