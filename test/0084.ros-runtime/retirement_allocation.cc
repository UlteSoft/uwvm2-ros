/** @file
 * @brief Fault-inject allocator refusal into authority retirement.
 * @details Live-member retirement must transfer an existing tombstone node;
 * allocation failure for a never-enrolled incarnation must fail closed.
 * Run in the existing bounded cgroup, separately from threaded test fixtures.
 */
#include <uwvm2/imported/ros/scene/authority.h>
#include <cassert>
#include <cstdlib>
#include <new>

namespace { thread_local bool refuse_allocation{}; }
void* operator new(std::size_t bytes)
{
    if(refuse_allocation)throw std::bad_alloc{};
    if(auto value=std::malloc(bytes?bytes:1))return value;
    throw std::bad_alloc{};
}
void operator delete(void* pointer) noexcept{std::free(pointer);}
void operator delete(void* pointer,std::size_t) noexcept{std::free(pointer);}
void* operator new[](std::size_t bytes){return ::operator new(bytes);}
void operator delete[](void* pointer) noexcept{::operator delete(pointer);}
void operator delete[](void* pointer,std::size_t) noexcept{::operator delete(pointer);}

int main()
{
    namespace s=uwvm2::imported::ros::scene;
    int first{},second{},third{};s::caller member{&first,1},peer{&second,1},unseen{&third,1};s::authority authority;
    assert(authority.enroll(member)==s::error::success&&authority.enroll(peer)==s::error::success);
    refuse_allocation=true;
    authority.retire_member(member);
    assert(authority.enroll(member)==s::error::denied);
    assert(authority.enroll(peer)==s::error::success);
    authority.retire_member(member); // Idempotent tombstone needs no allocation.
    authority.retire_member(unseen); // An impossible tombstone denies the authority.
    assert(authority.enroll(peer)==s::error::denied&&authority.enroll(unseen)==s::error::denied);
    refuse_allocation=false;
}
