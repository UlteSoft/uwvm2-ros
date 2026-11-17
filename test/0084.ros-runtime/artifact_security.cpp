/** @file
 * @brief Qualify immutable artifact admission, rollback and deployment publication.
 * @details Driver-only native safety tests use the real sealed-input provisioner
 * in private tmpfs. Trusted callbacks inject preparation failure solely to test
 * all-or-nothing publication; guest reads always use actual sealed snapshots.
 */
#include <uwvm2/uwvm/imported/scene/input.h>
#include <uwvm2/imported/ros/scene/deployment.h>
#include <cassert>
#include <cstdlib>
#include <filesystem>
#include <future>
#include <iostream>
#include <sys/inotify.h>

namespace s=uwvm2::imported::ros::scene;
namespace r=uwvm2::imported::ros::resources;
namespace fs=std::filesystem;
using operation=uwvm2::imported::ros::artifact_contract::operation;
using local_operation=uwvm2::imported::ros::runtime_contract::operation;
using uwvm2::uwvm::imported::scene::prepare_input;
static unsigned passed;
static void mark(char const* name){++passed;std::cout<<"PASS "<<name<<'\n';}
struct directory
{
    std::string path;
    directory()
    {
        auto base=std::getenv("UWVM_INPUT_TEST_TMP");assert(base&&*base);
        auto pattern=std::string(base)+"/artifact-XXXXXX";
        auto created=::mkdtemp(pattern.data());assert(created);path=created;
    }
    ~directory(){fs::remove_all(path);}
};
static void write_file(std::string const& path,std::string_view bytes)
{
    int fd=::open(path.c_str(),O_WRONLY|O_CREAT|O_TRUNC|O_CLOEXEC,0600);assert(fd>=0);
    std::size_t done{};
    while(done<bytes.size()){auto count=::write(fd,bytes.data()+done,bytes.size()-done);assert(count>0);done+=count;}
    assert(::close(fd)==0);
}
static s::artifacts::input_plan plan(std::string path,std::string_view data,std::uint64_t transfer=16)
{
    s::artifacts::input_plan result;result.path=std::move(path);result.generation=7;result.bytes=data.size();result.transfer=transfer;
    fast_io::sha256_context hash{};auto first=reinterpret_cast<std::byte const*>(data.data());
    hash.update(first,first+data.size());hash.do_final();std::array<std::byte,32> digest{};hash.digest_to_byte_ptr(digest.data());
    for(std::size_t i{};i<digest.size();++i)result.digest[i]=std::to_integer<std::uint8_t>(digest[i]);
    return result;
}
static std::vector<std::uint8_t> encode(std::initializer_list<std::uint64_t> words)
{
    std::vector<std::uint8_t> output(words.size()*8);std::size_t i{};
    for(auto value:words)s::put(output,i++,value);
    return output;
}
static std::vector<std::uint8_t> open_request(s::artifacts::input_plan const& input)
{
    auto bytes=encode({input.generation});bytes.insert(bytes.end(),input.digest.begin(),input.digest.end());return bytes;
}
static std::size_t descriptors()
{std::size_t count{};for(auto const& entry:fs::directory_iterator("/proc/self/fd")){(void)entry;++count;}return count;}
static s::grant grant(std::string alias,std::set<std::string,std::less<>> actions={"artifact.open","artifact.read","artifact.describe","resource.close"})
{
    s::grant value;value.alias=std::move(alias);value.kind=s::purpose::package;value.resource="approved-bytes";
    value.binding="sealed-input";value.actions=std::move(actions);value.max_resources=1;return value;
}
struct reply{s::error status{};std::size_t required{};std::vector<std::uint8_t> bytes;unsigned deliveries{};};
static reply artifact(s::broker& broker,s::caller who,s::handle cap,operation action,s::bytes request,std::size_t capacity=64)
{
    reply answer;answer.status=broker.invoke_artifact(who,action,cap,request,capacity,answer.required,[&](s::bytes data)
    {++answer.deliveries;answer.bytes.assign(data.begin(),data.end());return true;});return answer;
}
static reply local(s::broker& broker,s::caller who,s::handle cap,local_operation action,s::bytes request,std::size_t capacity=0)
{
    reply answer;answer.status=broker.invoke(who,action,cap,request,capacity,answer.required,[&](s::bytes data)
    {++answer.deliveries;answer.bytes.assign(data.begin(),data.end());return true;});return answer;
}
struct fixture
{
    directory root;int identity{};s::broker broker;s::caller caller{&identity,1};
    std::string payload="0123456789abcdefXYZ";
    s::artifacts::input_plan approved;
    std::map<std::string,s::handle> caps;
    std::vector<std::weak_ptr<s::storage::immutable_input>> retained;
    fixture()
    {
        auto path=root.path+"/source";write_file(path,payload);approved=plan(path,payload);
        for(auto const& entry:uwvm2::imported::ros::artifact_contract::operations)assert(broker.permissions.expose(std::string(entry.name),s::exposure::allow)==s::error::success);
        for(auto action:{"capability.open","capability.close","resource.close","guard.create","guard.trigger"})
            assert(broker.permissions.expose(action,s::exposure::allow)==s::error::success);
    }
    void add(s::grant value)
    {
        auto alias=value.alias;assert(broker.permissions.configure(value)==s::error::success);caps.emplace(alias,0);
        s::storage::principal principal{941,alias};std::shared_ptr<s::storage::immutable_input> input;
        assert(prepare_input(approved,principal,value.kind,input)==s::error::success);retained.push_back(input);
        assert(broker.inputs.bind(principal,value.kind,value.resource,value.binding,approved,std::move(input))==s::error::success);
    }
    void start()
    {
        assert(broker.permissions.enroll(caller)==s::error::success);
        for(auto& [alias,cap]:caps)assert(broker.permissions.open(caller,alias,cap)==s::error::success);
    }
    reply call(std::string const& alias,operation action,s::bytes request,std::size_t capacity=64)
    {return artifact(broker,caller,caps.at(alias),action,request,capacity);}
    s::handle open(std::string const& alias)
    {auto answer=call(alias,operation::open,open_request(approved),8);assert(answer.status==s::error::success);return s::word(answer.bytes,0);}
    void empty()
    {std::vector<s::artifacts::snapshot> rows;assert(broker.inputs.inspect(rows,16)&&rows.empty());}
};
static void transactional_outputs()
{
    fixture f;f.add(grant("input"));f.start();auto request=open_request(f.approved);std::size_t required{};
    auto short_output=f.call("input",operation::open,request,7);
    assert(short_output.status==s::error::too_large&&short_output.required==8&&!short_output.deliveries);f.empty();
    for(unsigned attempt{};attempt!=128;++attempt)
    {
        assert(f.broker.invoke_artifact(f.caller,operation::open,f.caps.at("input"),request,8,required,[](s::bytes){return false;})==s::error::invalid);
        f.empty();
        bool threw{};
        try{f.broker.invoke_artifact(f.caller,operation::open,f.caps.at("input"),request,8,required,[](s::bytes)->bool{throw std::runtime_error("delivery");});}
        catch(std::runtime_error const&){threw=true;}
        assert(threw);f.empty();
    }
    auto bad=f.approved;++bad.generation;assert(f.call("input",operation::open,open_request(bad),8).status==s::error::denied);
    bad=f.approved;bad.digest[0]^=1;assert(f.call("input",operation::open,open_request(bad),8).status==s::error::denied);f.empty();
    auto id=f.open("input");assert(f.call("input",operation::open,request,8).status==s::error::again);
    auto read=encode({id,4,8});
    assert(f.broker.invoke_artifact(f.caller,operation::read,f.caps.at("input"),read,8,required,[](s::bytes){return false;})==s::error::invalid);
    auto bytes=f.call("input",operation::read,read,8);assert(bytes.status==s::error::success&&std::string(bytes.bytes.begin(),bytes.bytes.end())=="456789ab");
    bool threw{};
    try{f.broker.invoke_artifact(f.caller,operation::read,f.caps.at("input"),read,8,required,[](s::bytes)->bool{throw std::runtime_error("delivery");});}
    catch(std::runtime_error const&){threw=true;}
    assert(threw&&f.call("input",operation::read,read,8).bytes==bytes.bytes);
    auto describe=encode({id});
    assert(f.broker.invoke_artifact(f.caller,operation::describe,f.caps.at("input"),describe,64,required,[](s::bytes){return false;})==s::error::invalid);
    auto metadata=f.call("input",operation::describe,describe);assert(metadata.status==s::error::success&&metadata.required==64);
    assert(s::word(metadata.bytes,0)==7&&s::word(metadata.bytes,1)==f.payload.size()&&s::word(metadata.bytes,2)==1&&s::word(metadata.bytes,3)==16);
    assert(std::equal(metadata.bytes.begin()+32,metadata.bytes.end(),f.approved.digest.begin()));
    assert(local(f.broker,f.caller,f.caps.at("input"),local_operation::resource_close,describe).status==s::error::success);f.empty();
    assert(!f.retained.front().expired()); // Closing a guest handle does not refund the retained snapshot.
    fs::remove(f.approved.path);auto next=f.open("input");assert(next!=id);
    assert(f.call("input",operation::read,encode({id,0,1}),1).status==s::error::bad_handle);
    assert(f.call("input",operation::read,encode({next,0,16}),16).bytes==std::vector<std::uint8_t>(f.payload.begin(),f.payload.begin()+16));
    mark("failed/throwing delivery rolls back unpublished opens; reads are replayable; close retains snapshot and returns handle capacity");
}
static void exact_ownership_and_actions()
{
    fixture f;for(auto alias:{"input","other"})f.add(grant(alias));
    f.add(grant("open-only",{"artifact.open"}));f.add(grant("describe-only",{"artifact.describe"}));
    auto guard=grant("signal",{"guard.create","guard.trigger","resource.close"});guard.kind=s::purpose::guard;guard.binding="local";
    assert(f.broker.permissions.configure(guard)==s::error::success);f.caps.emplace("signal",0);f.start();
    auto id=f.open("input"),open_only=f.open("open-only");
    for(auto action:{operation::read,operation::describe})
    {
        auto request=action==operation::read?encode({id,0,1}):encode({id});
        assert(f.call("other",action,request).status==s::error::bad_handle);
    }
    assert(f.call("open-only",operation::read,encode({open_only,0,1})).status==s::error::denied);
    assert(f.call("open-only",operation::describe,encode({open_only})).status==s::error::denied);
    assert(f.call("describe-only",operation::open,open_request(f.approved)).status==s::error::denied);
    assert(f.call("describe-only",operation::read,encode({id,0,1})).status==s::error::denied);
    assert(f.call("describe-only",operation::describe,encode({id})).status==s::error::bad_handle);
    auto signal=local(f.broker,f.caller,f.caps.at("signal"),local_operation::guard_create,{},8);
    assert(signal.status==s::error::success);auto signal_id=s::word(signal.bytes,0);assert(signal_id!=id&&signal_id!=open_only);
    assert(f.call("input",operation::read,encode({signal_id,0,1})).status==s::error::bad_handle);
    assert(local(f.broker,f.caller,f.caps.at("input"),local_operation::resource_close,encode({signal_id})).status==s::error::bad_handle);
    assert(local(f.broker,f.caller,f.caps.at("signal"),local_operation::resource_close,encode({id})).status==s::error::bad_handle);
    assert(local(f.broker,f.caller,f.caps.at("signal"),local_operation::guard_trigger,encode({signal_id})).status==s::error::success);
    fixture other;other.add(grant("input"));other.start();
    assert(other.call("input",operation::read,encode({id,0,1})).status==s::error::bad_handle);
    assert(artifact(other.broker,other.caller,f.caps.at("input"),operation::read,encode({id,0,1})).status==s::error::bad_handle);
    int unknown{};assert(artifact(f.broker,{&unknown,1},f.caps.at("input"),operation::read,encode({id,0,1})).status==s::error::denied);
    s::handle untouched=123;assert(f.broker.permissions.open(f.caller,"ungranted",untouched)==s::error::denied&&untouched==123);
    mark("exact alias/environment/action isolation, distinct artifact/guard identities and absent-grant refusal");
}
static void bounds_and_attenuation()
{
    fixture f;f.add(grant("input"));f.start();auto id=f.open("input");
    auto tail=f.call("input",operation::read,encode({id,17,16}),16);
    assert(tail.status==s::error::success&&tail.required==2&&std::string(tail.bytes.begin(),tail.bytes.end())=="YZ");
    auto eof=f.call("input",operation::read,encode({id,f.payload.size(),1}),1);
    assert(eof.status==s::error::success&&eof.required==0&&eof.bytes.empty()&&eof.deliveries==1);
    for(auto request:{encode({id,20,0}),encode({id,0,17}),encode({id,UINT64_MAX,2}),encode({id,0,s::transfer_limit+1})})
        assert(f.call("input",operation::read,request).status==s::error::too_large);
    assert(f.call("input",operation::read,encode({id,0}),16).status==s::error::invalid);
    assert(f.call("input",operation::read,encode({id,0,1}),s::transfer_limit+1).status==s::error::invalid);
    assert(f.call("input",static_cast<operation>(255),{},0).status==s::error::unsupported);
    auto short_read=f.call("input",operation::read,encode({id,0,8}),7);
    assert(short_read.status==s::error::too_large&&short_read.required==8&&!short_read.deliveries);
    std::size_t required{};unsigned cancellations{};
    assert(f.broker.invoke_artifact(f.caller,operation::read,f.caps.at("input"),encode({id,0,8}),8,required,
        [](s::bytes){assert(false);return true;},[&]{return ++cancellations==2;})==s::error::cancelled&&cancellations==2);
    assert(f.call("input",operation::read,encode({id,0,8}),8).status==s::error::success);
    fixture limited;auto cap=grant("input");cap.limits.calls=1;cap.limits.bytes=48;cap.limits.window=std::chrono::hours(1);cap.limits.uncertain=true;
    limited.add(cap);limited.start();auto request=open_request(limited.approved);
    assert(limited.call("input",operation::open,request,7).status==s::error::too_large);
    auto good=limited.call("input",operation::open,request,8);assert(good.status==s::error::success);
    assert(limited.call("input",operation::open,request,8).status==s::error::again);
    assert(limited.broker.permissions.close(limited.caller,limited.caps.at("input"))==s::error::success);
    assert(limited.broker.permissions.open(limited.caller,"input",limited.caps.at("input"))==s::error::success);
    assert(limited.call("input",operation::open,request,8).status==s::error::again);
    fixture cancelled;cancelled.add(grant("input"));cancelled.start();
    assert(cancelled.broker.invoke_artifact(cancelled.caller,operation::open,cancelled.caps.at("input"),open_request(cancelled.approved),8,
        required,[](s::bytes){assert(false);return true;},[]{return true;})==s::error::cancelled);cancelled.empty();
    assert(cancelled.open("input"));
    mark("transfer/extent/overflow/EOF bounds, pre/post-read cancellation and no admission charge for undersized output");
}
static void configure(s::deployment& target,std::string name,std::string module,s::artifacts::input_plan const& input)
{
    assert(target.create(name)==s::error::success);assert(target.bind(name,std::move(module))==s::error::success);
    for(auto action:{"artifact.open","artifact.read","artifact.describe","resource.close","capability.open"})
        assert(target.expose(name,action,s::exposure::allow)==s::error::success);
    assert(target.allow(name,grant("input"))==s::error::success);assert(target.input(name,"input",input)==s::error::success);
}
static void inert_validation_and_budgets()
{
    directory root;auto source=root.path+"/source";write_file(source,"approved");auto approved=plan(source,"approved");
    int watch=::inotify_init1(IN_CLOEXEC|IN_NONBLOCK);assert(watch>=0);assert(::inotify_add_watch(watch,source.c_str(),IN_OPEN)>=0);
    unsigned provisions{};auto provider=[&](auto const& input,auto principal,auto use,auto& output)
    {++provisions;return prepare_input(input,principal,use,output);};
    for(unsigned mode{};mode!=3;++mode)
    {
        s::deployment target;configure(target,"robot","main",approved);
        if(mode==1)assert(target.input_budget("robot",approved.bytes-1,1)==s::error::success);
        if(mode==2)assert(target.input_budget("robot",approved.bytes,0)==s::error::success);
        assert(target.validate()==s::error::invalid);assert(target.prepare_resources(provider)==s::error::invalid&&provisions==0);
    }
    s::deployment target;configure(target,"robot","main",approved);
    assert(target.input_budget("robot",approved.bytes,1)==s::error::success);
    assert(target.validate()==s::error::success&&provisions==0);
    for(auto selected:{s::debug::selection::authority,s::debug::selection::resources})
    {s::debug::request query;query.selected=selected;s::debug::view view;target.inspect(query,view);assert(view.result==s::debug::status::ok&&provisions==0);}
    std::array<std::byte,4096> events{};assert(::read(watch,events.data(),events.size())<0&&errno==EAGAIN);
    int identity{};std::shared_ptr<s::broker> output;assert(target.select("main",{&identity,1},output)==s::error::denied&&!output);
    assert(target.prepare_resources(provider)==s::error::success&&provisions==1);assert(::read(watch,events.data(),events.size())>0);
    assert(target.prepare_resources(provider)==s::error::success&&provisions==1); // Idempotent: no repeated source access.
    assert(target.select("main",{&identity,1},output)==s::error::success);
    assert(target.input_budget("robot",approved.bytes,2)==s::error::invalid);
    assert(::close(watch)==0);
    mark("zero-default aggregate byte/object reservations, inert validation/inspection and idempotent explicit provisioning");
}
static void atomic_preparation_and_retirement()
{
    directory root;auto first=root.path+"/first",second=root.path+"/second";write_file(first,"first");write_file(second,"second");
    auto a=plan(first,"first"),b=plan(second,"second");std::vector<std::weak_ptr<s::storage::immutable_input>> retained;
    std::shared_ptr<s::broker> left,right;int identity{},peer{};s::caller who{&identity,19},other{&peer,19};
    {
        s::deployment target;configure(target,"alpha","main",a);configure(target,"zeta","peer",b);
        assert(target.input_budget("alpha",a.bytes,1)==s::error::success);assert(target.input_budget("zeta",b.bytes,1)==s::error::success);
        std::vector<s::storage::principal> principals;unsigned provisions{};bool fail=true;auto initial=descriptors();
        auto provider=[&](auto const& input,auto principal,auto use,auto& output)
        {
            ++provisions;principals.push_back(principal);
            if(fail&&input.path==second)return s::error::io;
            auto status=prepare_input(input,principal,use,output);if(status==s::error::success)retained.push_back(output);return status;
        };
        assert(target.prepare_resources(provider)==s::error::io&&provisions==2);
        assert(retained.size()==1&&retained.front().expired()&&descriptors()==initial);
        assert(target.select("main",who,left)==s::error::denied&&!left);assert(target.select("peer",other,right)==s::error::denied&&!right);
        fail=false;assert(target.prepare_resources(provider)==s::error::success&&provisions==4);
        assert(principals[0].environment!=principals[1].environment&&principals[0].grant==principals[1].grant);
        assert(principals[0].environment==principals[2].environment&&principals[1].environment==principals[3].environment);
        assert(!retained[1].expired()&&!retained[2].expired());
        fs::remove(first);fs::remove(second);
        assert(target.select("main",who,left)==s::error::success);assert(target.select("peer",other,right)==s::error::success&&left!=right);
        s::handle cap{},peer_cap{};assert(left->permissions.open(who,"input",cap)==s::error::success);assert(right->permissions.open(other,"input",peer_cap)==s::error::success);
        auto opened=artifact(*left,who,cap,operation::open,open_request(a),8);assert(opened.status==s::error::success);auto id=s::word(opened.bytes,0);
        assert(artifact(*right,other,peer_cap,operation::read,encode({id,0,1}),1).status==s::error::bad_handle);
        target.retire_member(who);
        assert(artifact(*left,who,cap,operation::read,encode({id,0,1}),1).status==s::error::denied);
        assert(target.select("main",who,right)==s::error::denied);assert(target.select("main",{&identity,20},right)==s::error::denied);
        auto peer_open=artifact(*right,other,peer_cap,operation::open,open_request(b),8);assert(peer_open.status==s::error::success);
        auto peer_id=s::word(peer_open.bytes,0);assert(right->revoke("input")==s::error::success);
        assert(artifact(*right,other,peer_cap,operation::read,encode({peer_id,0,1}),1).status==s::error::denied);
        assert(artifact(*right,other,peer_cap,operation::open,open_request(b),8).status==s::error::denied);
        target.retire_generation(19);assert(target.select("peer",{&peer,20},right)==s::error::denied);
        assert(target.prepare_resources(provider)==s::error::denied&&provisions==4);
    }
    left.reset();right.reset();for(auto const& input:retained)assert(input.expired());
    mark("atomic multi-environment preparation/FD cleanup, distinct principals, member/grant/epoch retirement and no resurrection");
}
static void revocation_serializes_delivery()
{
    fixture f;f.add(grant("input"));f.start();auto id=f.open("input");std::promise<void> entered;auto started=entered.get_future();
    std::future<s::error> revoke;std::atomic<bool> completed{};std::size_t required{};
    assert(f.broker.invoke_artifact(f.caller,operation::read,f.caps.at("input"),encode({id,0,8}),8,required,[&](s::bytes data)
    {
        assert(std::string(data.begin(),data.end())=="01234567");
        revoke=std::async(std::launch::async,[&]{entered.set_value();auto status=f.broker.revoke("input");completed=true;return status;});
        started.get();assert(!completed.load());return true;
    })==s::error::success);
    assert(revoke.get()==s::error::success&&completed.load());
    assert(f.call("input",operation::read,encode({id,0,8}),8).status==s::error::denied);
    mark("authority lease serializes concurrent revocation with read delivery");
}
static void native_offset_boundary()
{
    fixture f;f.add(grant("input"));f.start();auto id=f.open("input");
    auto retained=f.retained.front().lock();assert(retained);auto output=retained;
    auto oversized=f.approved;oversized.bytes=std::uint64_t{1}<<31;
    auto status=prepare_input(oversized,{941,"input"},s::purpose::package,output);
    // With a narrow off_t, refuse before source access/conversion. A wide
    // off_t reaches the real exact-extent check against the small source.
    assert(status==(sizeof(::off_t)==4?s::error::too_large:s::error::conflict)&&output==retained);
    assert(f.call("input",operation::read,encode({id,std::uint64_t{1}<<31,1}),1).status==s::error::too_large);
    assert(f.call("input",operation::read,encode({id,0,8}),8).status==s::error::success);
    mark("2 GiB native offset boundary refuses without narrowing and preserves prior ownership");
}
int main()
{
    assert(::geteuid()!=0);
    std::cout<<"native widths off_t="<<sizeof(::off_t)*8<<" size_t="<<sizeof(std::size_t)*8<<'\n';
    transactional_outputs();exact_ownership_and_actions();bounds_and_attenuation();inert_validation_and_budgets();
    atomic_preparation_and_retirement();revocation_serializes_delivery();native_offset_boundary();
    std::cout<<"passed "<<passed<<" artifact broker/deployment safety groups\n";
}
