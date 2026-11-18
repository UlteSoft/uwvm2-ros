// A backing-memory-owned directory of nonoverlapping managed extents, protected
// by that owner's tag lease. Every node is charged to the memtag resource pool.
#pragma once
#include <uwvm2/utils/allocator/resource_limit.h>
#ifndef UWVM_MODULE
# include <cstddef>
# include <cstdint>
# include <new>
# include <utility>
#endif
#ifndef UWVM_MODULE_EXPORT
# define UWVM_MODULE_EXPORT
#endif
UWVM_MODULE_EXPORT namespace uwvm2::extension::memtag
{
    class managed_allocations
    {
    public:
        struct extent
        {
            ::std::uint64_t first{}, last{}; // inclusive linear granule indices
            ::std::uint8_t tag{}, parent{};
            bool active{}, permanent{};
        };
    private:
        struct node
        {
            extent value{};
            node* child[2]{};
            unsigned height{1};
        };
        node* root_{};
        static unsigned height(node const* item) noexcept { return item == nullptr ? 0u : item->height; }
        static void update(node* item) noexcept
        {
            auto a{height(item->child[0])}, b{height(item->child[1])};
            item->height = 1u + (a < b ? b : a);
        }
        static node* rotate(node* root, unsigned side) noexcept
        {
            node* top{root->child[side]};
            root->child[side] = top->child[1u-side];
            top->child[1u-side] = root;
            update(root); update(top);
            return top;
        }
        static node* balance(node* item) noexcept
        {
            update(item);
            auto a{height(item->child[0])}, b{height(item->child[1])};
            if(a <= b+1u && b <= a+1u) { return item; }
            unsigned side{static_cast<unsigned>(a < b)};
            node* heavy{item->child[side]};
            if(height(heavy->child[side]) < height(heavy->child[1u-side]))
            { item->child[side] = rotate(heavy,1u-side); }
            return rotate(item,side);
        }
        static node* insert(node* tree, node* item) noexcept
        {
            if(!tree) { return item; }
            unsigned side{static_cast<unsigned>(item->value.first > tree->value.first)};
            tree->child[side] = insert(tree->child[side],item);
            return balance(tree);
        }
        static node* detach_first(node* tree, node*& first) noexcept
        {
            if(!tree->child[0]) { first=tree; return tree->child[1]; }
            tree->child[0] = detach_first(tree->child[0],first);
            return balance(tree);
        }
        static void destroy(node* item) noexcept
        {
            item->~node();
            namespace alloc=::uwvm2::utils::allocator::resource;
            alloc::free_bytes<alloc::pool::memtag>(reinterpret_cast<::std::byte*>(item));
        }
        static node* erase(node* tree, ::std::uint64_t first) noexcept
        {
            if(first != tree->value.first)
            {
                unsigned side{static_cast<unsigned>(first > tree->value.first)};
                tree->child[side] = erase(tree->child[side],first);
                return balance(tree);
            }
            node* left{tree->child[0]}, *right{tree->child[1]};
            destroy(tree);
            if(!right) { return left; }
            node* next{};
            right=detach_first(right,next);
            next->child[0]=left;
            next->child[1]=right;
            return balance(next);
        }
        static void clear(node* item) noexcept
        {
            if(!item) { return; }
            clear(item->child[0]); clear(item->child[1]); destroy(item);
        }
        template<typename Visitor>
        static void visit(node const* item, ::std::uint64_t first, ::std::uint64_t last, Visitor& visitor) noexcept
        {
            if(!item) { return; }
            auto const& v{item->value};
            if(v.first > first) { visit(item->child[0],first,last,visitor); }
            if(v.first <= last && v.last >= first) { visitor(v); }
            if(v.last < last) { visit(item->child[1],first,last,visitor); }
        }
    public:
        constexpr managed_allocations() noexcept = default;
        managed_allocations(managed_allocations const&) = delete;
        managed_allocations& operator=(managed_allocations const&) = delete;
        constexpr ~managed_allocations() { reset(); }
        constexpr void reset() noexcept
        { if(root_) { clear(root_); root_=nullptr; } }
        constexpr void take(managed_allocations& other) noexcept
        { reset(); root_=::std::exchange(other.root_,nullptr); }
        [[nodiscard]] bool empty() const noexcept { return !root_; }
        template<typename Visitor> void visit(::std::uint64_t first,::std::uint64_t last,Visitor&& visitor) const noexcept
        { visit(root_,first,last,visitor); }
        [[nodiscard]] extent* exact(::std::uint64_t first,::std::uint64_t last) noexcept
        {
            node* n{root_};
            while(n && first != n->value.first) { n=n->child[first > n->value.first]; }
            return n && n->value.last==last ? &n->value : nullptr;
        }
        // The caller proves disjointness while holding the owner lease, before insertion.
        [[nodiscard]] bool add(extent value) noexcept
        {
            namespace alloc=::uwvm2::utils::allocator::resource;
            auto* bytes{alloc::try_bytes<alloc::pool::memtag>(sizeof(node))};
            if(!bytes) { return false; }
            root_=insert(root_,::new(static_cast<void*>(bytes)) node{value});
            return true;
        }
        // Host static-manifest commit and rollback. The caller holds the owner
        // write lease or owns an unpublished instance exclusively.
        [[nodiscard]] bool add_permanent(::std::uint64_t first, ::std::uint64_t last, ::std::uint8_t tag) noexcept
        {
            if(first > last || !tag) { return false; }
            bool overlap{};
            visit(first, last, [&](extent const&) noexcept { overlap = true; });
            return !overlap && add({first, last, tag, 0, true, true});
        }
        [[nodiscard]] bool remove_permanent(::std::uint64_t first, ::std::uint64_t last, ::std::uint8_t tag) noexcept
        {
            auto* current{exact(first, last)};
            if(!current || !current->permanent || current->tag != tag) { return false; }
            remove(*current);
            return true;
        }
        void remove(extent const& owned) noexcept { root_=erase(root_,owned.first); }
    };
}
