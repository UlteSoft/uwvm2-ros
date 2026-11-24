// A backing-memory-owned directory of nonoverlapping managed extents, protected
// by that owner's tag lease. Its nodes and index pages use the memtag resource pool.
#pragma once
#include <uwvm2/utils/allocator/resource_limit.h>
#ifndef UWVM_MODULE
# include <bit>
# include <cstddef>
# include <cstdint>
# include <new>
# include <utility>
# include "instruction.h"
#endif
#ifndef UWVM_MODULE_EXPORT
# define UWVM_MODULE_EXPORT
#endif
UWVM_MODULE_EXPORT namespace uwvm2::extension::memtag
{
    class managed_allocations
    {
    public:
        using extent = allocation_extent;
    private:
        struct node
        {
            extent value{};
            node* child[2]{};
            unsigned height{1};
        };
        node* root_{};
        ::std::uint64_t tree_first_{UINT64_MAX}, tree_last_{};

        // Short private lifetimes frequently reuse a small set of address pages.
        // A set bit identifies the sole extent beginning at that granule. Every
        // indexed extent is contained in one page. Both endpoints and all flags
        // stay in its ordinary extent record, including outstanding reservations.
        static constexpr unsigned page_granules{64}, page_slots{32};
        struct page
        {
            ::std::uint64_t number{}, starts{};
            page* next{};
            extent records[page_granules]{};
        };
        struct index
        {
            page* slots[page_slots]{};
            page* first{}; // occupied pages, in address order for checkpoints
            page* spare{}; // at most one empty page while another page is live
        };
        index* pages_{};

        template<typename T>
        static T* allocate() noexcept
        {
            namespace alloc=::uwvm2::utils::allocator::resource;
            auto* bytes{alloc::try_bytes<alloc::pool::memtag>(sizeof(T))};
            return bytes ? ::new(static_cast<void*>(bytes)) T{} : nullptr;
        }
        template<typename T>
        static void destroy(T* item) noexcept
        {
            item->~T();
            namespace alloc=::uwvm2::utils::allocator::resource;
            alloc::free_bytes<alloc::pool::memtag>(reinterpret_cast<::std::byte*>(item));
        }
        static constexpr unsigned slot(::std::uint64_t number) noexcept
        { return static_cast<unsigned>(number % page_slots); }
        [[nodiscard]] page* lookup_page(::std::uint64_t number) const noexcept
        {
            if(!pages_) { return nullptr; }
            auto* entry{pages_->slots[slot(number)]};
            return entry && entry->number == number ? entry : nullptr;
        }
        [[nodiscard]] page* open_page(::std::uint64_t number) noexcept
        {
            if(!pages_ && !(pages_ = allocate<index>())) { return nullptr; }
            auto& entry{pages_->slots[slot(number)]};
            if(entry) { return entry->number == number ? entry : nullptr; }
            auto* fresh{pages_->spare ? ::std::exchange(pages_->spare,nullptr) : allocate<page>()};
            if(!fresh)
            {
                if(!pages_->first) { destroy(pages_); pages_=nullptr; }
                return nullptr;
            }
            fresh->number=number;
            auto** link{&pages_->first};
            while(*link && (*link)->number < number) { link=&(*link)->next; }
            fresh->next=*link;
            *link=entry=fresh;
            return fresh;
        }
        void close_page(page* entry) noexcept
        {
            pages_->slots[slot(entry->number)]=nullptr;
            auto** link{&pages_->first};
            while(*link != entry) { link=&(*link)->next; }
            *link=entry->next;
            if(pages_->spare) { destroy(pages_->spare); }
            if(pages_->first) { pages_->spare=entry; }
            else
            {
                destroy(entry);
                destroy(pages_);
                pages_=nullptr;
            }
        }
        void clear_pages() noexcept
        {
            while(auto* entry=pages_->first) { pages_->first=entry->next; destroy(entry); }
            if(pages_->spare) { destroy(pages_->spare); }
            destroy(pages_);
            pages_=nullptr;
        }
        // Pick every intersecting extent, including one that starts before
        // the window. Disjointness makes the predecessor the only such start.
        static ::std::uint64_t starts_in(page const& entry, ::std::uint64_t first, ::std::uint64_t last) noexcept
        {
            auto const base{entry.number*page_granules};
            auto const start{first > base ? static_cast<unsigned>(first-base) : 0u};
            auto const end{last-base < page_granules ? static_cast<unsigned>(last-base) : page_granules-1u};
            auto const before{(::std::uint64_t{1} << start)-1u};
            auto const through{UINT64_MAX >> (page_granules-1u-end)};
            auto starts{entry.starts & through & ~before};
            if(auto prior=entry.starts & before)
            {
                auto const bit{static_cast<unsigned>(::std::bit_width(prior)-1u)};
                if(entry.records[bit].last >= first) { starts |= ::std::uint64_t{1} << bit; }
            }
            return starts;
        }
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
        template<typename Visitor>
        void visit_tree(::std::uint64_t first, ::std::uint64_t last, Visitor& visitor) const noexcept
        {
            if(first <= tree_last_ && tree_first_ <= last) { visit(root_,first,last,visitor); }
        }
    public:
        // Checkpoint staging uses add(), one charged tree node per portable row.
        static constexpr ::std::size_t bytes_per_extent{sizeof(node) + sizeof(::uwvm2::utils::allocator::resource::byte_header)};
        constexpr managed_allocations() noexcept = default;
        managed_allocations(managed_allocations const&) = delete;
        managed_allocations& operator=(managed_allocations const&) = delete;
        constexpr ~managed_allocations() { reset(); }
        constexpr void reset() noexcept
        {
            if(root_) { clear(root_); root_=nullptr; }
            if(pages_) { clear_pages(); }
            tree_first_=UINT64_MAX;
            tree_last_=0;
        }
        constexpr void take(managed_allocations& other) noexcept
        {
            reset();
            root_=::std::exchange(other.root_,nullptr);
            pages_=::std::exchange(other.pages_,nullptr);
            tree_first_=::std::exchange(other.tree_first_,UINT64_MAX);
            tree_last_=::std::exchange(other.tree_last_,0);
        }
        [[nodiscard]] bool empty() const noexcept { return !root_ && !pages_; }

        // Visit in linear-address order, including ranges spanning both stores.
        template<typename Visitor>
        void visit(::std::uint64_t first,::std::uint64_t last,Visitor&& visitor) const noexcept
        {
            if(first > last) { return; }
            for(auto* entry{pages_ ? pages_->first : nullptr}; entry; entry=entry->next)
            {
                if(entry->number < first/page_granules) { continue; }
                if(entry->number > last/page_granules) { break; }
                for(auto mask{starts_in(*entry,first,last)};mask;mask &= mask-1u)
                {
                    auto const& current{entry->records[::std::countr_zero(mask)]};
                    if(first < current.first) { visit_tree(first,current.first-1u,visitor); }
                    visitor(current);
                    if(current.last >= last) { return; }
                    first=current.last+1u;
                }
            }
            visit_tree(first,last,visitor);
        }

        // Conflict and color queries are insensitive to enumeration order.
        // Resolve local windows by direct page index; bound work on large ranges.
        template<typename Visitor>
        void visit_near(::std::uint64_t first,::std::uint64_t last,Visitor&& visitor) const noexcept
        {
            auto const lo{first/page_granules}, hi{last/page_granules};
            if(pages_ && hi-lo <= 1u)
            {
                for(auto number{lo}; number <= hi; ++number)
                {
                    if(auto* entry=lookup_page(number))
                    {
                        for(auto mask{starts_in(*entry,first,last)};mask;mask &= mask-1u)
                        { visitor(entry->records[::std::countr_zero(mask)]); }
                    }
                }
            }
            else if(pages_) { visit(first,last,visitor); return; }
            visit_tree(first,last,visitor);
        }

        [[nodiscard]] extent* exact(::std::uint64_t first,::std::uint64_t last) noexcept
        {
            if(auto* entry=lookup_page(first/page_granules))
            {
                auto const bit{static_cast<unsigned>(first % page_granules)};
                if(entry->starts & (::std::uint64_t{1} << bit))
                { return entry->records[bit].last == last ? &entry->records[bit] : nullptr; }
            }
            if(first < tree_first_ || last > tree_last_) { return nullptr; }
            node* n{root_};
            while(n && first != n->value.first) { n=n->child[first > n->value.first]; }
            return n && n->value.last==last ? &n->value : nullptr;
        }
        // The caller proves disjointness while holding the owner lease, before insertion.
        [[nodiscard]] bool add(extent value) noexcept
        {
            auto* item{allocate<node>()};
            if(!item && pages_ && pages_->spare)
            {
                destroy(::std::exchange(pages_->spare,nullptr));
                item=allocate<node>();
            }
            if(!item) { return false; }
            item->value=value;
            root_=insert(root_,item);
            if(value.first < tree_first_) { tree_first_=value.first; }
            if(tree_last_ < value.last) { tree_last_=value.last; }
            return true;
        }
        [[nodiscard]] bool add_private(extent value) noexcept
        {
            auto const number{value.first/page_granules};
            if(number == value.last/page_granules)
            {
                auto* entry{lookup_page(number)};
                if(!entry) { entry=open_page(number); }
                if(entry)
                {
                    auto const bit{static_cast<unsigned>(value.first % page_granules)};
                    entry->records[bit]=value;
                    entry->starts |= ::std::uint64_t{1} << bit;
                    return true;
                }
            }
            return add(value);
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
        void remove(extent const& owned) noexcept
        {
            auto const first{owned.first}, last{owned.last};
            if(auto* entry=lookup_page(first/page_granules))
            {
                auto const bit{::std::uint64_t{1} << (first % page_granules)};
                if(entry->starts & bit)
                {
                    entry->starts &= ~bit;
                    if(!entry->starts) { close_page(entry); }
                    return;
                }
            }
            root_=erase(root_,first);
            if(first == tree_first_)
            {
                auto* n{root_};
                while(n && n->child[0]) { n=n->child[0]; }
                tree_first_=n ? n->value.first : UINT64_MAX;
            }
            if(last == tree_last_)
            {
                auto* n{root_};
                while(n && n->child[1]) { n=n->child[1]; }
                tree_last_=n ? n->value.last : 0;
            }
        }
    };
}
