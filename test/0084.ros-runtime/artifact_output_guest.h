/** @file
 * @brief Test-only adapter to the explicitly granted volatile scene cache.
 * @details Every mutation carries the next sequence; replaying the identical
 * last request must return the identical retained receipt. Querying a receipt
 * is non-acknowledging. Published bytes are read back and removed under exact
 * independent actions. No path, native descriptor or durability claim exists.
 */
#pragma once
extern "C" {
__attribute__((import_module("uwvm_ros_v2"),import_name("capability.open")))
std::uint32_t output_capability_open(std::uint32_t,std::uint64_t,std::uint64_t,std::uint64_t);
#define OUTPUT_IMPORT(symbol,external) \
__attribute__((import_module("uwvm_ros_v2"),import_name(external))) \
std::uint32_t symbol(std::uint64_t,std::uint32_t,std::uint64_t,std::uint64_t,std::uint64_t,std::uint64_t,std::uint64_t);
OUTPUT_IMPORT(output_begin,"artifact.begin")
OUTPUT_IMPORT(output_append,"artifact.append")
OUTPUT_IMPORT(output_commit,"artifact.commit")
OUTPUT_IMPORT(output_remove,"artifact.remove")
OUTPUT_IMPORT(output_status,"artifact.status")
OUTPUT_IMPORT(output_receipt,"artifact.receipt")
OUTPUT_IMPORT(output_read,"artifact.read")
#undef OUTPUT_IMPORT
}

namespace artifact_output_transport
{
    struct result { std::vector<unsigned char> bytes;bool denied{},published{},durable{}; };
    using function=std::uint32_t(*)(std::uint64_t,std::uint32_t,std::uint64_t,std::uint64_t,std::uint64_t,std::uint64_t,std::uint64_t);
    template<typename T>static std::uint64_t address(T* value){return reinterpret_cast<std::uintptr_t>(value);}
    static std::uint64_t capability{},next_sequence{1};
    static void open_capability()
    {
        if(capability)return;
        constexpr char alias[]="xmlout";
        require(output_capability_open(0,address(alias),sizeof(alias)-1,address(&capability))==0&&capability);
    }
    struct receipt {std::uint64_t words[9]{};};
    static receipt request(function call,void const* bytes,std::size_t size)
    {
        receipt value;std::uint64_t length{};
        require(call(capability,0,address(bytes),size,address(&value),sizeof(value),address(&length))==0&&length==sizeof(value));
        return value;
    }
    static receipt mutate(function call,std::vector<unsigned char> const& bytes,unsigned operation)
    {
        auto value=request(call,bytes.data(),bytes.size());
        require(value.words[0]==next_sequence&&value.words[1]==operation&&value.words[3]==0);
#if !defined(ARTIFACT_OUTPUT_BENCHMARK)
        auto repeated=request(call,bytes.data(),bytes.size());
        require(std::memcmp(&value,&repeated,sizeof(value))==0);
        auto queried=request(output_receipt,&next_sequence,sizeof(next_sequence));
        require(std::memcmp(&value,&queried,sizeof(value))==0);
#endif
        ++next_sequence;return value;
    }
    static std::vector<unsigned char> words(std::initializer_list<std::uint64_t> values)
    {
        std::vector<unsigned char> bytes(values.size()*8);std::size_t index{};
        for(auto value:values)for(unsigned shift{};shift<64;shift+=8)bytes[index++]=static_cast<unsigned char>(value>>shift);
        return bytes;
    }
    static result roundtrip(std::uint64_t,unsigned char const* source,std::size_t size,
                            unsigned char const*,unsigned chunk)
    {
        require(chunk&&chunk<=4096&&size<=65536);open_capability();
        auto begin=words({next_sequence,size});result value;
#if defined(EXPECT_ARTIFACT_OUTPUT_DENIAL)
        receipt denied;std::uint64_t length{};
        require(output_begin(capability,0,address(begin.data()),begin.size(),address(&denied),sizeof(denied),address(&length))==13);
        value.denied=true;return value;
#else
        auto created=mutate(output_begin,begin,1);auto object=created.words[2];
        require(object&&created.words[4]==0&&created.words[5]==0&&created.words[6]==size&&created.words[7]==1&&created.words[8]==0);
        for(std::size_t offset{};offset<size;)
        {
            auto count=std::min<std::size_t>(chunk,size-offset);
            auto appending=words({next_sequence,object,offset});
            appending.insert(appending.end(),source+offset,source+offset+count);
            auto appended=mutate(output_append,appending,2);offset+=count;
            require(appended.words[2]==object&&appended.words[4]==count&&appended.words[5]==offset&&
                appended.words[6]==size&&appended.words[7]==1&&appended.words[8]==0);
        }
        auto committed=mutate(output_commit,words({next_sequence,object,size}),3);
        require(committed.words[2]==object&&committed.words[4]==0&&committed.words[5]==size&&
            committed.words[6]==size&&committed.words[7]==2&&committed.words[8]==1);
#if !defined(ARTIFACT_OUTPUT_BENCHMARK)
        auto current=request(output_status,&object,sizeof(object));
        require(current.words[0]==committed.words[0]&&current.words[1]==3&&current.words[2]==object&&
            current.words[3]==0&&current.words[4]==0&&current.words[5]==size&&current.words[6]==size&&current.words[7]==2&&current.words[8]==1);
#endif
        value.published=true;value.durable=(committed.words[8]&2)!=0;value.bytes.resize(size);
        for(std::size_t offset{};offset<size;)
        {
            auto count=std::min<std::size_t>(chunk,size-offset);auto reading=words({object,offset,count});std::uint64_t length{};
            require(output_read(capability,0,address(reading.data()),reading.size(),address(value.bytes.data()+offset),count,address(&length))==0&&length==count);
            offset+=count;
        }
#if !defined(ARTIFACT_OUTPUT_BENCHMARK)
        auto eof=words({object,size,0});std::uint64_t length{99};
        require(output_read(capability,0,address(eof.data()),eof.size(),0,0,address(&length))==0&&length==0);
#endif
        auto removed=mutate(output_remove,words({next_sequence,object}),5);
        require(removed.words[2]==object&&removed.words[3]==0&&removed.words[4]==0&&removed.words[6]==0&&removed.words[7]==4&&removed.words[8]==0);
        return value;
#endif
    }
}
