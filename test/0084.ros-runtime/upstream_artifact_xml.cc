/** @file
 * @brief Consume real BehaviorTree.CPP XML using its unmodified TinyXML2 source.
 * @details Native and Wasm execute identical parser calls. Only the input
 * adapter differs: native reads pinned test files; Wasm opens exact scene
 * artifact capabilities and reads bounded immutable chunks. XML include
 * attributes remain data and never imply filesystem or plugin authority.
 * This is a parser/input portability slice, not the complete robot executor.
 */
#include "tinyxml2.h"
#include "artifact_fixtures.h"
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <vector>

static void require(bool value)
{
    if(!value)
    {
#if defined(__wasm__)
        __builtin_trap();
#else
        std::abort();
#endif
    }
}

#if defined(__wasm__)
extern "C" {
__attribute__((import_module("uwvm_ros_v2"),import_name("capability.open")))
std::uint32_t scene_capability_open(std::uint32_t,std::uint64_t,std::uint64_t,std::uint64_t);
__attribute__((import_module("uwvm_ros_v2"),import_name("artifact.open")))
std::uint32_t scene_artifact_open(std::uint64_t,std::uint32_t,std::uint64_t,std::uint64_t,std::uint64_t,std::uint64_t,std::uint64_t);
__attribute__((import_module("uwvm_ros_v2"),import_name("artifact.describe")))
std::uint32_t scene_artifact_describe(std::uint64_t,std::uint32_t,std::uint64_t,std::uint64_t,std::uint64_t,std::uint64_t,std::uint64_t);
__attribute__((import_module("uwvm_ros_v2"),import_name("artifact.read")))
std::uint32_t scene_artifact_read(std::uint64_t,std::uint32_t,std::uint64_t,std::uint64_t,std::uint64_t,std::uint64_t,std::uint64_t);
__attribute__((import_module("uwvm_ros_v2"),import_name("resource.close")))
std::uint32_t scene_resource_close(std::uint64_t,std::uint32_t,std::uint64_t,std::uint64_t,std::uint64_t,std::uint64_t,std::uint64_t);
}
template<typename T>static std::uint64_t address(T* value){return reinterpret_cast<std::uintptr_t>(value);}

static std::vector<char> load(artifact_fixture const& fixture,unsigned chunk)
{
    std::uint64_t cap{},object{},required{};
    require(scene_capability_open(0,address(fixture.alias),std::strlen(fixture.alias),address(&cap))==0);
    std::uint64_t opening[5]{fixture.generation};std::memcpy(opening+1,fixture.digest,32);
    auto status=scene_artifact_open(cap,0,address(opening),40,address(&object),8,address(&required));
#if defined(EXPECT_ARTIFACT_DENIAL)
    require(status==13);return {};
#else
    require(status==0&&required==8);
    std::uint64_t description[8]{};
    require(scene_artifact_describe(cap,0,address(&object),8,address(description),64,address(&required))==0&&required==64);
    require(description[0]==fixture.generation&&description[1]==fixture.size&&description[2]==2&&description[3]==64);
    require(std::memcmp(description+4,fixture.digest,32)==0);
    std::vector<char> bytes(fixture.size);
    for(std::uint64_t offset{};offset<fixture.size;)
    {
        auto request=static_cast<std::uint64_t>(chunk);
        std::uint64_t words[3]{object,offset,request};char block[64]{};
        require(scene_artifact_read(cap,0,address(words),24,address(block),sizeof(block),address(&required))==0);
        auto expected=fixture.size-offset<request?fixture.size-offset:request;
        require(required==expected&&required>0);std::memcpy(bytes.data()+offset,block,required);offset+=required;
    }
    std::uint64_t eof[3]{object,fixture.size,chunk};char block[64]{};
    require(scene_artifact_read(cap,0,address(eof),24,address(block),sizeof(block),address(&required))==0&&required==0);
    require(scene_resource_close(cap,0,address(&object),8,0,0,address(&required))==0&&required==0);
    return bytes;
#endif
}
#else
static std::vector<char> load(artifact_fixture const& fixture,unsigned)
{
    std::FILE* file=std::fopen(fixture.path,"rb");require(file);
    std::vector<char> bytes(fixture.size);
    require(std::fread(bytes.data(),1,bytes.size(),file)==bytes.size());
    require(std::fgetc(file)==EOF&&std::fclose(file)==0);return bytes;
}
#endif

struct summary final:tinyxml2::XMLVisitor
{
    std::uint64_t hash{1469598103934665603ULL};unsigned elements{},attributes{},texts{},includes{};
    void add(char const* text)
    {for(auto p=reinterpret_cast<unsigned char const*>(text);*p;++p){hash^=*p;hash*=1099511628211ULL;}hash^=255;hash*=1099511628211ULL;}
    bool VisitEnter(tinyxml2::XMLElement const& element,tinyxml2::XMLAttribute const* attribute) override
    {
        ++elements;add(element.Name());if(std::strcmp(element.Name(),"include")==0)++includes;
        for(;attribute;attribute=attribute->Next()){++attributes;add(attribute->Name());add(attribute->Value());}
        return true;
    }
    bool Visit(tinyxml2::XMLText const& text) override{++texts;add(text.Value());return true;}
};

static int execute()
{
    unsigned checks{};
    for(auto const& fixture:artifact_fixtures)
    {
        std::uint64_t first_hash{};int first_error{};bool first=true;
        for(unsigned chunk:{1u,7u,31u,64u})
        {
            auto bytes=load(fixture,chunk);
#if defined(EXPECT_ARTIFACT_DENIAL)
            require(bytes.empty());++checks;continue;
#else
            tinyxml2::XMLDocument document;
            auto error=document.Parse(bytes.empty()?"":bytes.data(),bytes.size());
            require((error==tinyxml2::XML_SUCCESS)==fixture.valid);
            summary parsed;if(!error)require(document.Accept(&parsed));
            if(first){first=false;first_hash=parsed.hash;first_error=error;}
            else require(parsed.hash==first_hash&&error==first_error);
            std::printf("%s chunk=%u error=%d elements=%u attributes=%u texts=%u includes=%u hash=%llu\n",
                        fixture.alias,chunk,error,parsed.elements,parsed.attributes,parsed.texts,parsed.includes,
                        static_cast<unsigned long long>(parsed.hash));
            ++checks;
#endif
        }
    }
    std::printf("checks:%u\n",checks);return 0;
}

#if defined(__wasm__)
extern "C" void _start(){require(execute()==0);require(std::fflush(stdout)==0);}
#else
int main(){return execute();}
#endif
