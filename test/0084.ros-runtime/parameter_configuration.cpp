/** @file
 * @brief Check complete explicit parameter definitions, hash coverage and aggregate freeze.
 */
#define UWVM_PARAMETER_BROKER_EMBEDDED
#include "parameter_broker.cpp"
#include <uwvm2/imported/ros/scene/parameter_configuration.h>

struct definition_fixture
{
    record body,file;
    s::buffers::profile selected,node;
    pc::descriptor descriptor;
    pv::algebra algebra;
    explicit definition_fixture(pv::algebra algebra=pv::algebra::ros2,bool override=false,bool none=false,
                                std::string_view search_root={},std::uint64_t candidates=0):algebra(algebra)
    {
        selected.identity.assign("parameters");node.identity.assign("node");node.sha256.fill(2);
        auto& out=body.output;assert(out.text(node.identity.view())&&out.blob(node.sha256)&&out.text(algebra==pv::algebra::ros1?"ros1":"ros2")&&
            out.boolean(false)&&out.boolean(false)&&out.f64(0)&&out.text(search_root)&&out.u64(candidates)&&out.u64(1));
        descriptor.name.assign("a");descriptor.description.assign("bounded");descriptor.additional_constraints.assign("approved");
        descriptor.dynamic_type=algebra==pv::algebra::ros1;descriptor.declared_type=algebra==pv::algebra::ros1?pv::kind::xmlrpc_struct:pv::kind::integer;
        descriptor.has_range=algebra==pv::algebra::ros2;descriptor.integers={0,100,1};
        assert(pc::write_descriptor(out,descriptor));
        if(algebra==pv::algebra::ros1)assert(out.u64(3)&&out.text("integer")&&out.text("xmlrpc_array")&&out.text("xmlrpc_struct"));
        else assert(out.u64(3)&&out.text("not_set")&&out.text("integer")&&out.text("integers"));
        assert(out.boolean(true)&&out.i64(0)&&out.i64(100)&&out.u64(1)&&out.boolean(false)&&out.boolean(override));
        if(override)
        {
            record value;
            if(none)assert(value.output.u64(0));
            else if(algebra==pv::algebra::ros1)
            {assert(value.output.u64(32)&&value.output.u64(2));integer(value.output,7);assert(value.output.u64(4)&&value.output.text("hello"));}
            else integer(value.output,3);
            assert(out.blob(value.bytes()));
        }
        frame();
    }
    void frame()
    {
        selected.sha256=s::buffers::hash(body.bytes());file.output=wire::writer{file.storage};
        assert(file.output.text(p::definition_magic)&&file.output.text(selected.identity.view())&&file.output.blob(selected.sha256)&&file.output.encoded(body.bytes()));
    }
    void configure(p::configuration& out,int omitted=-1) const
    {
        assert(out.profile(selected,node,algebra)==E::success);
        auto value=plan();
        std::array<std::uint64_t,20> values{value.max_scopes,value.max_parameters,value.max_total_bytes,value.max_events,value.max_event_bytes,value.max_cursors,
            value.max_workspace,value.max_batch,value.max_request_bytes,value.max_receipt_bytes,value.max_pending_bytes,value.max_cursor_bytes,
            value.validation_timeout_ns,value.cursor_timeout_ns,value.values.bytes,value.values.nodes,value.values.elements,value.values.depth,16384,8};
        for(std::size_t i{};i<values.size();++i)if(static_cast<int>(i)!=omitted)assert(out.limit(p::configuration::limit_names[i],values[i])==E::success);
    }
    p::plan freeze()
    {p::configuration config;configure(config);assert(config.definition(file.bytes())==E::success);p::plan result;assert(config.freeze(result)==E::success);return result;}
};
static result declare_configured(fixture& target,std::uint64_t scope,std::uint64_t sequence,pc::descriptor const& descriptor,std::int64_t value=1)
{record input;key(input.output,scope,sequence);assert(pc::write_descriptor(input.output,descriptor));integer(input.output,value);return target.call(pc::operation::declare,input);}
static void complete_configuration_and_phase_order()
{
    definition_fixture definition;p::configuration empty;p::plan output;
    assert(empty.freeze(output)==E::denied&&empty.definition(definition.file.bytes())==E::denied);
    for(std::size_t missing{};missing<p::configuration::limit_names.size();++missing)
    {
        p::configuration config;definition.configure(config,static_cast<int>(missing));
        assert(config.definition(definition.file.bytes())==E::denied&&config.freeze(output)==E::denied);
    }
    p::configuration config;definition.configure(config);
    assert(config.limit("scopes",1)==E::conflict&&config.limit("scope",1)==E::invalid);
    assert(config.profile(definition.selected,definition.node,pv::algebra::ros2)==E::conflict);
    assert(config.definition(definition.file.bytes())==E::success&&config.definition(definition.file.bytes())==E::conflict);
    assert(config.limit("batch",1)==E::conflict&&config.freeze(output)==E::success&&config.freeze(output)==E::conflict);
    assert(output.keys.size()==1&&output.keys[0].fixed_descriptor&&!output.keys[0].has_override&&output.keys[0].descriptor.description.view()=="bounded");
    p::configuration excessive;assert(excessive.limit("definition.bytes",UINT64_MAX)==E::invalid);
    assert(excessive.limit("value.depth",17)==E::too_large&&excessive.limit("value.depth",16)==E::success);
}
static void hash_identity_and_closed_schema()
{
    definition_fixture definition;
    for(std::size_t cut{};cut<definition.file.bytes().size();++cut)
    {p::configuration config;definition.configure(config);assert(config.definition(definition.file.bytes().first(cut))!=E::success);}
    auto tail=definition.body.bytes().size();assert(definition.body.output.u64(0));definition.frame();
    p::configuration trailing;definition.configure(trailing);assert(trailing.definition(definition.file.bytes())==E::invalid);
    definition.body.output=wire::writer{definition.body.storage};assert(definition.body.output.encoded(wire::bytes{definition.body.storage.data(),tail}));definition.frame();
    p::configuration tamper;definition.configure(tamper);definition.file.storage[definition.file.bytes().size()-1]^=1;
    assert(tamper.definition(definition.file.bytes())==E::invalid);definition.file.storage[definition.file.bytes().size()-1]^=1;
    auto node=definition.node;node.sha256[0]^=1;p::configuration foreign;
    assert(foreign.profile(definition.selected,node,pv::algebra::ros2)==E::success);
    auto limits=plan();static_cast<void>(limits);
    for(auto name:p::configuration::limit_names)
        assert(foreign.limit(name,name=="value.depth"?8:name=="definition.keys"?8:name=="definition.bytes"?16384:128)==E::success);
    assert(foreign.definition(definition.file.bytes())==E::denied);
    auto file_hash=s::buffers::hash(definition.file.bytes());assert(file_hash!=definition.selected.sha256); // digest never includes itself
}
static void descriptors_narrow_without_type_confusion()
{
    definition_fixture definition;auto policy=definition.freeze();fixture target{policy};auto scope=target.open();
    auto changed=definition.descriptor;changed.description.assign("forged");assert(declare_configured(target,scope,1,changed).status==E::denied);
    changed=definition.descriptor;changed.integers={-1,100,1};assert(declare_configured(target,scope,1,changed).status==E::denied);
    changed=definition.descriptor;changed.dynamic_type=true;assert(declare_configured(target,scope,1,changed).status==E::denied);
    changed=definition.descriptor;changed.declared_type=pv::kind::integers;changed.has_range=false;
    assert(declare_configured(target,scope,1,changed).status==E::denied);
    changed=definition.descriptor;changed.integers={2,20,2};assert(declare_configured(target,scope,1,changed,2).status==E::success&&target.value(scope,"a")==2);
    record outside;set_record(outside,scope,2,{{"a",3}});assert(receipt{target.call(pc::operation::set_atomic,outside)}.state=="failed"&&target.value(scope,"a")==2);
    definition_fixture none{pv::algebra::ros2,true,true};fixture unset{none.freeze()};auto empty=unset.open();
    assert(declare_configured(unset,empty,1,none.descriptor).status==E::success);auto got=unset.get(empty,"a");wire::reader read{got.bytes};
    assert(number(read)==1&&text(read)=="a"&&number(read)==0); // Explicit ROS2 not_set is a value, not absent override.
}
static void ros1_heterogeneous_override_and_explicit_absence()
{
    definition_fixture xml{pv::algebra::ros1,true};fixture target{xml.freeze()};auto scope=target.open();
    auto descriptor=xml.descriptor;descriptor.declared_type=pv::kind::integer;
    assert(declare_configured(target,scope,1,descriptor).status==E::success);auto got=target.get(scope,"a");wire::reader input{got.bytes};
    assert(number(input)==1&&text(input)=="a"&&number(input)==32&&number(input)==2&&number(input)==2);std::int64_t integer{};
    assert(input.i64(integer)&&integer==7&&number(input)==4&&text(input)=="hello");
    definition_fixture nil{pv::algebra::ros1,true,true};p::configuration forbidden;nil.configure(forbidden);assert(forbidden.definition(nil.file.bytes())==E::invalid);
    definition_fixture absent{pv::algebra::ros1,false};auto plain=absent.freeze();assert(!plain.keys[0].has_override&&plain.keys[0].override_value.empty());
    // The same ROS1 definition cannot silently choose ROS2's type union.
    p::configuration wrong;assert(wrong.profile(xml.selected,xml.node,pv::algebra::ros2)==E::success);
    for(auto name:p::configuration::limit_names)assert(wrong.limit(name,name=="value.depth"?8:name=="definition.keys"?8:name=="definition.bytes"?16384:128)==E::success);
    assert(wrong.definition(xml.file.bytes())==E::denied);
}
static void override_constraints_are_configuration_errors()
{
    definition_fixture invalid{pv::algebra::ros2,true};
    auto end=invalid.body.bytes().size();invalid.body.storage[end-8]=101;invalid.frame();
    p::configuration config;invalid.configure(config);assert(config.definition(invalid.file.bytes())==E::invalid);
    // A syntactically valid override may not violate either trusted range.
    invalid.body.storage[end-8]=100;invalid.frame();p::configuration endpoint;invalid.configure(endpoint);
    assert(endpoint.definition(invalid.file.bytes())==E::success);
    definition_fixture mismatch{pv::algebra::ros2,true};end=mismatch.body.bytes().size();
    // Replace integer(3) by empty integers, equal byte extent but different type.
    mismatch.body.storage[end-16]=7;mismatch.body.storage[end-8]=0;mismatch.frame();
    p::configuration wrong_type;mismatch.configure(wrong_type);assert(wrong_type.definition(mismatch.file.bytes())==E::invalid);
}
static void aggregate_and_recoverable_definition_allocation()
{
    definition_fixture definition{pv::algebra::ros2,true};auto policy=definition.freeze();p::budget total{},maximum;
    maximum={policy.max_scopes,policy.max_parameters,policy.max_total_bytes,policy.max_events,policy.max_event_bytes,policy.max_cursors,
        policy.max_workspace,policy.max_request_bytes,policy.max_receipt_bytes,policy.max_pending_bytes,policy.max_cursor_bytes,1u<<20};
    assert(p::accumulate(maximum,policy,total)==E::success);auto before=total;
    assert(p::accumulate(maximum,policy,total)==E::too_large&&total.scopes==before.scopes&&total.definition_bytes==before.definition_bytes);
    p::configuration config;definition.configure(config);assert(config.definition(definition.file.bytes())==E::success);p::plan output;
    assert(config.freeze(output,maximum,total)==E::too_large&&output.keys.empty());total={};
    assert(config.freeze(output,maximum,total)==E::success&&total.scopes==policy.max_scopes&&!output.keys.empty());
    unsigned failures{};
    for(std::size_t point=1;point<8;++point)
    {
        p::configuration trial;definition.configure(trial);fault_after=point;allocation_failed=false;
        auto result=trial.definition(definition.file.bytes());fault_after=0;
        if(!allocation_failed){assert(result==E::success);break;}
        ++failures;assert(result==E::no_memory);assert(trial.definition(definition.file.bytes())==E::success);p::plan recovered;assert(trial.freeze(recovered)==E::success);
    }
    assert(failures>=2);
}
static void parse_tool_file(char const* path)
{
    // Trusted test fixture, bounded before its complete canonical parser runs.
    auto input=p::owned_bytes::allocate(4u*1024u*1024u+1u);auto* file=std::fopen(path,"rb");assert(file);
    auto count=std::fread(input.writable().data(),1,input.writable().size(),file);assert(!std::ferror(file)&&std::feof(file));std::fclose(file);
    assert(count<=4u*1024u*1024u);auto bytes=input.view().first(count);wire::reader reader{bytes};std::string_view magic,identity,algebra;wire::bytes digest;
    s::buffers::profile selected,node;
    assert(reader.text(magic)&&magic==p::definition_magic&&reader.text(identity)&&selected.identity.assign(identity)&&reader.blob(digest)&&digest.size()==32);
    std::copy(digest.begin(),digest.end(),selected.sha256.begin());
    assert(reader.text(identity)&&node.identity.assign(identity)&&reader.blob(digest)&&digest.size()==32);std::copy(digest.begin(),digest.end(),node.sha256.begin());
    assert(reader.text(algebra)&&(algebra=="ros1"||algebra=="ros2"));p::configuration config;
    assert(config.profile(selected,node,algebra=="ros1"?pv::algebra::ros1:pv::algebra::ros2)==E::success);
    constexpr std::array<std::uint64_t,20> limits{4,128,1u<<20,32,1u<<20,8,1u<<20,128,1u<<20,1u<<20,1u<<20,1u<<20,
        1000000000,1000000000,1u<<20,65536,65536,16,4u<<20,128};
    for(std::size_t i{};i<limits.size();++i)assert(config.limit(p::configuration::limit_names[i],limits[i])==E::success);
    assert(config.definition(bytes)==E::success);p::plan policy;assert(config.freeze(policy)==E::success&&p::valid(policy));
    assert(policy.keys.size()>=8&&policy.selected==selected&&policy.node_profile==node);
    assert(policy.search_root.view()==(algebra=="ros1"?"/plant":"")&&policy.max_search_depth==(algebra=="ros1"?2u:0u));
    std::puts("passed canonical parameter definition tool roundtrip");
}
static void search_profile_is_hashed_and_closed()
{
    definition_fixture disabled{pv::algebra::ros1};auto inert=disabled.freeze();assert(inert.search_root.view().empty()&&!inert.max_search_depth);
    for(auto root:{"/","/plant","/plant/line_a"})
    {
        definition_fixture enabled{pv::algebra::ros1,false,false,root,256};auto policy=enabled.freeze();
        assert(policy.search_root.view()==root&&policy.max_search_depth==256&&policy.selected.sha256!=inert.selected.sha256);
        definition_fixture narrower{pv::algebra::ros1,false,false,root,1};assert(narrower.freeze().selected.sha256!=policy.selected.sha256);
        p::configuration forged;disabled.configure(forged);assert(forged.definition(enabled.file.bytes())==E::denied);
    }
    for(auto pair:{std::pair{"",1ULL},std::pair{"/",0ULL},std::pair{"/plant",257ULL},std::pair{"plant",1ULL},
        std::pair{"/plant/",1ULL},std::pair{"/plant//a",1ULL},std::pair{"/plant/../a",1ULL},std::pair{"/1bad",1ULL}})
    {
        definition_fixture invalid{pv::algebra::ros1,false,false,pair.first,pair.second};p::configuration config;invalid.configure(config);
        assert(config.definition(invalid.file.bytes())==E::invalid);
    }
    definition_fixture wrong{pv::algebra::ros2,false,false,"/plant",1};p::configuration config;wrong.configure(config);
    assert(config.definition(wrong.file.bytes())==E::invalid);
}
int main(int argc,char** argv)
{
    if(argc>1){assert(argc==2);parse_tool_file(argv[1]);return 0;}
    assert(run_parameter_broker_tests()==0);
    complete_configuration_and_phase_order();hash_identity_and_closed_schema();descriptors_narrow_without_type_confusion();
    ros1_heterogeneous_override_and_explicit_absence();override_constraints_are_configuration_errors();aggregate_and_recoverable_definition_allocation();
    search_profile_is_hashed_and_closed();
    std::puts("passed 7 parameter configuration security groups");
}
