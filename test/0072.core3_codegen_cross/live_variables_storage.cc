// Exercise spill growth and value semantics of the native liveness kill list.
#include <llvm/CodeGen/LiveVariables.h>
#include <cstdint>
#include <cstdlib>
#include <utility>
static void require(bool good) { if(!good) { ::std::abort(); } }
int main()
{
    ::llvm::LiveVariables::VarInfo original;
    require(original.Kills.empty());
    auto const start{reinterpret_cast<::std::uintptr_t>(&original)};
    auto const inline_data{reinterpret_cast<::std::uintptr_t>(original.Kills.data())};
    require(inline_data >= start && inline_data + sizeof(void*) <= start + sizeof(original));
    // Opaque identities only: no MachineInstr pointer is dereferenced.
    for(::std::uintptr_t i{1u}; i != 4097u; ++i)
    {
        original.Kills.push_back(reinterpret_cast<::llvm::MachineInstr*>(i * 16u));
        if(i == 1u) { require(reinterpret_cast<::std::uintptr_t>(original.Kills.data()) == inline_data); }
    }
    auto copy{original};
    original.Kills.clear();
    require(copy.Kills.size() == 4096u && original.Kills.empty());
    auto moved{::std::move(copy)};
    for(::std::uintptr_t i{}; i != 4096u; ++i)
    {
        require(moved.Kills[i] == reinterpret_cast<::llvm::MachineInstr*>((i + 1u) * 16u));
    }
    moved.Kills.erase(moved.Kills.begin() + 1u, moved.Kills.end() - 1u);
    require(moved.Kills.size() == 2u);
    require(moved.Kills.front() == reinterpret_cast<::llvm::MachineInstr*>(16u));
    require(moved.Kills.back() == reinterpret_cast<::llvm::MachineInstr*>(4096u * 16u));
    original = moved;
    moved.Kills.clear();
    require(original.Kills.size() == 2u && moved.Kills.empty());
}
