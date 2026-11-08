#pragma once
#include <uwvm2/extension/memtag/debug_view.h>
#include <uwvm2/object/memory/linear/impl.h>
template<typename Memory>
bool memtag_test_host_copy(Memory& memory,std::uint64_t offset,void* buffer,std::size_t count,bool write=false) {
 return uwvm2::object::memory::linear::with_memory_access_snapshot(memory,[&](std::byte* base,std::size_t length) noexcept {
  return uwvm2::extension::memtag::copy_debug_memory(memory.memtag_shadow,{base,length},offset,buffer,count,write);
 });
}
