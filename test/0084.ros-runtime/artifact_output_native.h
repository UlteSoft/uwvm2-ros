/** @file
 * @brief Native bounded-memory reference adapter for upstream XML output.
 * @details This baseline performs the same finite chunk copies as the guest.
 * It is not the scene policy implementation and does not qualify disk I/O,
 * crash durability or permission checks. The real VM suite tests those gates
 * independently. Each native roundtrip owns its finite temporary buffer.
 */
#pragma once
namespace artifact_output_transport
{
    struct result { std::vector<unsigned char> bytes;bool denied{},published{},durable{}; };
    static result roundtrip(std::uint64_t,unsigned char const* source,std::size_t size,
                            unsigned char const*,unsigned chunk)
    {
        require(chunk&&chunk<=4096&&size<=65536);
        result value;std::vector<unsigned char> staged;staged.reserve(size);
        for(std::size_t offset{};offset<size;)
        {
            auto count=std::min<std::size_t>(chunk,size-offset);
            staged.insert(staged.end(),source+offset,source+offset+count);offset+=count;
        }
        value.bytes.assign(staged.begin(),staged.end());value.published=true;return value;
    }
}
