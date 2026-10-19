/** @file
 * Export identities from the selected, actually generated Unitree SDK only.
 * This C++17 program constructs no ROS Context/Node/endpoint, opens no socket,
 * loads no Guest-selected type and grants no authority. selection.hpp is a
 * closed preparation artifact containing literal native includes/types.
 * Successful stdout is one bounded JSON document consumed by bind_catalog.py.
 * The trusted controller must separately qualify the compiler, full-MD inputs,
 * linked/mapped support libraries and preserved official package/source lock.
 */
#include "selection.hpp"
#include <gateways/common/native_io.h>
#include <uwvm2/imported/ros/ipc/custom_wire.h>
#include <algorithm>
#include <array>
#include <cstdint>
#include <csignal>
#include <limits>
#include <map>
#include <set>
#include <stdexcept>
#include <string>
#include <string_view>
#include <type_traits>
#include <utility>
#include <vector>
#if UWVM_UNITREE_ROS_VERSION == 1
#include <ros/message_traits.h>
#elif UWVM_UNITREE_ROS_VERSION == 2
#include <gateways/ros2/compat/identity.h>
#else
#error "A literal ROS1 or ROS2 selection is required"
#endif

namespace unitree_native
{
    namespace io=::uwvm2::ros_gateway::native_io;
    constexpr std::size_t metadata_limit=4u*1024u*1024u;
    constexpr std::size_t record_limit=256,field_limit=4096,aggregate_field_limit=16384;
    struct metadata_budget
    {
        std::size_t records{},fields{};
        void record(std::size_t count)
        {
            if(records==record_limit||count>field_limit||count>aggregate_field_limit-fields)
            {throw std::length_error("official metadata record/field ceiling");}
            ++records;fields+=count;
        }
    };
    /** Bounded strings from the actually selected native library. Never use a
     * catalog field to fill missing introspection or synthesize a native name. */
    std::string_view text(char const* value,std::size_t limit=255)
    {
        if(!value) {throw std::runtime_error("null official metadata text");}
        std::size_t size{};while(size<=limit&&value[size]) {++size;}
        if(size>limit) {throw std::length_error("official metadata text ceiling");}
        return {value,size};
    }
    bool identifier(std::string_view value)
    {
        auto alpha=[](char c){return (c>='a'&&c<='z')||(c>='A'&&c<='Z')||c=='_';};
        if(value.empty()||!alpha(value.front())) {return false;}
        for(char c:value) {if(!alpha(c)&&!(c>='0'&&c<='9')) {return false;}}
        return true;
    }
    class json
    {
        std::string body_;
    public:
        void add(std::string_view value)
        {if(value.size()>metadata_limit-body_.size()) {throw std::length_error("native metadata4MiB ceiling");}body_.append(value);}
        void quote(std::string_view value)
        {
            if(!::uwvm2::imported::ros::ipc::custom_wire_utf8(reinterpret_cast<std::uint8_t const*>(value.data()),value.size()))
            {throw std::runtime_error("native metadata is not UTF8");}
            add("\"");constexpr char hex[]{"0123456789abcdef"};
            for(unsigned char c:value)
            {
                if(c=='"'||c=='\\') {char pair[]{'\\',static_cast<char>(c)};add({pair,2});}
                else if(c<32) {char escape[]{'\\','u','0','0',hex[c>>4],hex[c&15]};add({escape,6});}
                else {char byte{static_cast<char>(c)};add({&byte,1});}
            }
            add("\"");
        }
        std::string const& body() const noexcept {return body_;}
    };

#if UWVM_UNITREE_ROS_VERSION == 1
    template<class Message> void prepare_support() {}
    template<class Message> void message(json& out,std::string_view expected,bool& comma,metadata_budget&)
    {
        auto native_type=text(::ros::message_traits::DataType<Message>::value());
        auto native_md5=text(::ros::message_traits::MD5Sum<Message>::value());
        if(native_type!=expected||native_md5.size()!=32) {throw std::runtime_error("official ROS1 DataType/MD5 mismatch");}
        for(char c:native_md5) {if(!((c>='0'&&c<='9')||(c>='a'&&c<='f'))) {throw std::runtime_error("malformed official ROS1 MD5");}}
        auto definition=text(::ros::message_traits::Definition<Message>::value(),1024u*1024u);
        if(comma) {out.add(",");}comma=true;out.quote(expected);out.add(":{\"native_identity\":");out.quote(native_md5);
        out.add(",\"identity_source\":\"OFFICIAL_GENERATED_ROS1_TRAITS\",\"components\":{\"message\":{\"native_identity\":");
        out.quote(native_md5);out.add(",\"official_type\":");out.quote(native_type);
        out.add(",\"definition\":");out.quote(definition);out.add("}}}");
    }
#else
    namespace compat=::uwvm2::ros_gateway::ros2_compat;
    namespace introspection=::rosidl_typesupport_introspection_cpp;
    namespace ipc=::uwvm2::imported::ros::ipc;
#if !UWVM_ROS2_HAVE_INTROSPECTION_CPP || !UWVM_ROS2_HAVE_OPENSSL
#error "Unitree ROS2 metadata requires actual C++ introspection and OpenSSL SHA256"
#endif
    using members=compat::message_members;
    /** Resolve the selected official support dispatcher before a mapping stop.
     * Its cached introspection library and native dependency DSOs must be in
     * the actual stopped process's map, not inferred from header presence. */
    template<class Message> void prepare_support()
    {
        if(!::rosidl_typesupport_cpp::get_message_type_support_handle<Message>()||!compat::message_metadata<Message>())
        {throw std::runtime_error("selected native support/introspection unavailable");}
    }
    /** These are official introspection IDs, not C++ object sizes or guessed
     * catalog spellings. Wide strings/long double/unknown scalars are outside
     * the finite supported profile and fail rather than being reinterpreted. */
    std::string_view scalar(unsigned id)
    {
        if(id==introspection::ROS_TYPE_BOOLEAN) {return "bool";}
        if(id==introspection::ROS_TYPE_INT8) {return "int8";}if(id==introspection::ROS_TYPE_UINT8) {return "uint8";}
        if(id==introspection::ROS_TYPE_OCTET) {return "byte";}if(id==introspection::ROS_TYPE_CHAR) {return "char";}
        if(id==introspection::ROS_TYPE_INT16) {return "int16";}if(id==introspection::ROS_TYPE_UINT16) {return "uint16";}
        if(id==introspection::ROS_TYPE_INT32) {return "int32";}if(id==introspection::ROS_TYPE_UINT32) {return "uint32";}
        if(id==introspection::ROS_TYPE_INT64) {return "int64";}if(id==introspection::ROS_TYPE_UINT64) {return "uint64";}
        if(id==introspection::ROS_TYPE_FLOAT) {return "float32";}if(id==introspection::ROS_TYPE_DOUBLE) {return "float64";}
        if(id==introspection::ROS_TYPE_STRING) {return "string";}return {};
    }
    std::string name_of(members const& actual)
    {
        auto space=text(compat::metadata_namespace(actual,0));auto name=text(actual.message_name_);
        if(!identifier(name)) {throw std::runtime_error("invalid actual message name");}
        std::string result;
        for(std::size_t at{};at<space.size();++at)
        {
            char c{space[at]};
            if(c==':'&&at+1<space.size()&&space[at+1]==':') {result+='/';++at;}
            else if(c==':'||c=='\\') {throw std::runtime_error("invalid actual message namespace");}
            else {result+=c;}
        }
        auto slash=result.find('/');
        if(slash==std::string::npos)
        {
            if(!identifier(result)) {throw std::runtime_error("invalid old official package namespace");}
            result+="/msg"; // Pre-IDL official MessageMembers stores package only.
        }
        else if(!identifier(std::string_view{result}.substr(0,slash))||std::string_view{result}.substr(slash)!="/msg")
        {throw std::runtime_error("unsupported actual native type category");}
        result+='/';result+=name;return result;
    }
    struct field_owned
    {
        std::string name,type;std::uint32_t array_size{},string_bound{},sequence_bound{};
        bool is_array{},is_sequence{};
    };
    struct record_owned
    {std::string name;members const* source{};unsigned color{};std::vector<field_owned> fields;};
    class native_schema
    {
        std::vector<record_owned> records_;
        std::map<std::string,std::size_t> by_name_;
        metadata_budget& budget_;
        std::size_t visit(members const* actual,unsigned depth)
        {
            if(!actual||depth>16||!actual->size_of_||!actual->member_count_||actual->member_count_>field_limit||!actual->members_)
            {throw std::runtime_error("unsupported actual native record or depth");}
            auto name=name_of(*actual);auto found=by_name_.find(name);
            if(found!=by_name_.end())
            {
                auto const& previous=records_[found->second];
                if(previous.source!=actual||previous.color!=2) {throw std::runtime_error("cyclic/duplicate/conflicting native record name");}
                return found->second;
            }
            if(records_.size()==record_limit) {throw std::length_error("native record ceiling");}
            budget_.record(actual->member_count_);
            auto selected=records_.size();by_name_.emplace(name,selected);
            records_.push_back({std::move(name),actual,1,{}});
            auto& record=records_[selected];record.fields.reserve(actual->member_count_);
            std::set<std::string> fields;
            for(std::size_t at{};at!=actual->member_count_;++at)
            {
                auto const& item=actual->members_[at];field_owned field;
                field.name=std::string{text(item.name_)};
                if(!identifier(field.name)||!fields.insert(field.name).second||item.default_value_||item.offset_>=actual->size_of_)
                {throw std::runtime_error("unsupported/default/duplicate native field");}
                if(item.array_size_>UINT32_MAX||item.string_upper_bound_>UINT32_MAX||(!item.is_array_&&(item.array_size_||item.is_upper_bound_)))
                {throw std::runtime_error("unsupported native container capacity");}
                if(item.is_array_)
                {
                    if(item.is_upper_bound_)
                    {if(!item.array_size_) {throw std::runtime_error("zero bounded native sequence");}field.is_sequence=true;field.sequence_bound=static_cast<std::uint32_t>(item.array_size_);}
                    else if(item.array_size_) {field.is_array=true;field.array_size=static_cast<std::uint32_t>(item.array_size_);}
                    else {field.is_sequence=true;}
                }
                auto primitive=scalar(item.type_id_);
                if(!primitive.empty())
                {
                    if(item.members_||(primitive!="string"&&item.string_upper_bound_)) {throw std::runtime_error("conflicting native scalar metadata");}
                    field.type=std::string{primitive};field.string_bound=static_cast<std::uint32_t>(item.string_upper_bound_);
                }
                else
                {
                    if(item.type_id_!=introspection::ROS_TYPE_MESSAGE||item.string_upper_bound_||!item.members_||!item.members_->data||
                        text(item.members_->typesupport_identifier)!="rosidl_typesupport_introspection_cpp")
                    {throw std::runtime_error("unsupported nested native metadata");}
                    // The selected C++ introspection handle's data is the real
                    // nested MessageMembers, exactly as identity.h attests it.
                    auto nested=visit(static_cast<members const*>(item.members_->data),depth+1);
                    field.type=records_[nested].name;
                }
                record.fields.push_back(std::move(field));
            }
            record.color=2;return selected;
        }
    public:
        native_schema(members const* actual,std::string_view expected,metadata_budget& budget):budget_{budget}
        {
            records_.reserve(record_limit);auto root=visit(actual,1);
            if(root!=0||records_[root].name!=expected) {throw std::runtime_error("actual native root differs from selected type");}
        }
        /** Reuse the existing public RIHS01 canonical JSON implementation.
         * Every field below was traversed from actual official introspection;
         * no catalog record/hash is admitted as a replacement or default. */
        std::string canonical() const
        {
            std::vector<std::vector<ipc::custom_native_field>> fields(records_.size());
            std::vector<ipc::custom_native_record> records;records.reserve(records_.size());
            for(std::size_t index{};index!=records_.size();++index)
            {
                auto const& actual=records_[index];auto& selected=fields[index];selected.reserve(actual.fields.size());
                for(auto const& f:actual.fields) {selected.push_back({f.name,f.type,f.array_size,f.is_array,f.string_bound,f.is_sequence,f.sequence_bound});}
                records.push_back({actual.name,selected.data(),selected.size()});
            }
            std::vector<std::size_t> references;for(std::size_t i{1};i!=records.size();++i) {references.push_back(i);}
            std::sort(references.begin(),references.end(),[&](auto a,auto b){return records[a].type<records[b].type;});
            compat::rihs_json result;result.add("{\"type_description\": ");result.record(records[0]);result.add(", \"referenced_type_descriptions\": [");
            for(std::size_t at{};at!=references.size();++at) {if(at) {result.add(", ");}result.record(records[references[at]]);}
            result.add("]}");if(!result.valid()) {throw std::runtime_error("RIHS canonical metadata ceiling/grammar");}return result.body();
        }
    };
    std::string schema_hash(std::string_view canonical)
    {
        unsigned char digest[SHA256_DIGEST_LENGTH];
        if(canonical.size()>metadata_limit||!::SHA256(reinterpret_cast<unsigned char const*>(canonical.data()),canonical.size(),digest))
        {throw std::runtime_error("OpenSSL RIHS01 digest failed");}
        return io::hexadecimal(digest,sizeof digest,"RIHS01_");
    }
    template<class Support> std::pair<std::string,std::string_view> identity(Support const* support,std::string const& derived)
    {
        if(!support) {throw std::runtime_error("official native support absent");}
        if constexpr(compat::has_baked_hash<Support>::value)
        {
            // A hash-capable SDK with a missing/malformed hash is an error.
            // It must never silently enter the older introspection-only path.
            if(!support->get_type_hash_func) {throw std::runtime_error("official hash callback absent");}
            auto hash=support->get_type_hash_func(support);
            if(!hash||hash->version!=1||sizeof(hash->value)!=32) {throw std::runtime_error("malformed official hash result");}
            bool nonzero{};for(auto byte:hash->value) {nonzero=nonzero||byte!=0;}
            auto actual=io::hexadecimal(hash->value,sizeof hash->value,"RIHS01_");
            if(!nonzero||actual!=derived) {throw std::runtime_error("official hash disagrees with actual introspection schema");}
            return {std::move(actual),"OFFICIAL_GET_TYPE_HASH"};
        }
        else {return {derived,"LEGACY_INTROSPECTION_RIHS01"};}
    }
    template<class Message> void message(json& out,std::string_view expected,bool& comma,metadata_budget& budget)
    {
        auto support=::rosidl_typesupport_cpp::get_message_type_support_handle<Message>();
        native_schema schema{compat::message_metadata<Message>(),expected,budget};
        auto canonical=schema.canonical();auto selected=identity(support,schema_hash(canonical));
        if(comma) {out.add(",");}comma=true;out.quote(expected);out.add(":{\"native_identity\":");out.quote(selected.first);
        out.add(",\"identity_source\":");out.quote(selected.second);out.add(",\"components\":{\"message\":{\"native_identity\":");
        out.quote(selected.first);out.add(",\"official_type\":");out.quote(expected);out.add(",\"identity_source\":");out.quote(selected.second);
        out.add(",\"schema\":");out.add(canonical);out.add("}}}");
    }
#endif
}
int main(int argc,char** argv)
{
    try
    {
        bool mapped_stop{};
        if(argc==2&&std::strcmp(argv[1],"--mapped-stop")==0) {mapped_stop=true;}
        else if(argc!=1) {throw std::runtime_error("unknown exporter argument");}
#define UWVM_UNITREE_PREPARE_SUPPORT(Native,Name) unitree_native::prepare_support<Native>();
        UWVM_UNITREE_NATIVE_MESSAGES(UWVM_UNITREE_PREPARE_SUPPORT)
#undef UWVM_UNITREE_PREPARE_SUPPORT
        if(mapped_stop)
        {
            // Root must observe the owned birth/current cgroup and every actual
            // loaded support map while this cooperative stop is held. One I
            // byte releases export; EOF, EINTR, wrong/short input is refusal.
            if(::raise(SIGSTOP)!=0) {throw std::runtime_error("metadata mapping stop failed");}
            char gate{};if(::read(STDIN_FILENO,&gate,1)!=1||gate!='I') {throw std::runtime_error("metadata mapping gate refused");}
        }
        unitree_native::json output;unitree_native::metadata_budget budget;bool comma{};
        output.add("{\"format\":\"uwvm-ros-official-bounded-native-v2\",\"ros_version\":");
#if UWVM_UNITREE_ROS_VERSION == 1
        output.add("1");
#else
        output.add("2");
#endif
        output.add(",\"types\":{");
#define UWVM_UNITREE_EXPORT_MESSAGE(Native,Name) unitree_native::message<Native>(output,Name,comma,budget);
        UWVM_UNITREE_NATIVE_MESSAGES(UWVM_UNITREE_EXPORT_MESSAGE)
#undef UWVM_UNITREE_EXPORT_MESSAGE
        if(!comma) {throw std::runtime_error("empty native selection");}
        output.add("}}\n");
        if(!unitree_native::io::write_all(STDOUT_FILENO,output.body())) {return 1;}return 0;
    }
    catch(std::exception const&)
    {return unitree_native::io::write_all(STDERR_FILENO,"Unitree native identity export refused\n")?1:2;}
}
