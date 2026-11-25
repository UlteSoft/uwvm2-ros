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
        union record_slot
        {
            extent value;
            // The starts bitmap publishes a slot only after constructing its record.
            // Fresh or recycled pages need no writes to the other 63 slots.
            record_slot() noexcept {}
        };
        struct index;
        struct page
        {
            ::std::uint64_t number{}, starts{};
            page* next{};
            index* table{};
            record_slot records[page_granules];
        };
        struct index
        {
            page* slots[page_slots]{};
            page* first{}; // occupied pages in address order for checkpoints
            page* spare{}; // at most one empty page while another page is live
        };
        page* recent_page_{}; // the sole page until a second page needs an index

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
            auto* entry{recent_page_};
            if(!entry || entry->number == number) { return entry; }
            if(!entry->table) { return nullptr; }
            entry=entry->table->slots[slot(number)];
            return entry && entry->number == number ? entry : nullptr;
        }
        // The caller looked up this page under the same lease. The first page
        // needs only the owner pointer. Allocate a hash index on the second.
        [[nodiscard]] page* open_page(::std::uint64_t number) noexcept
        {
            if(!recent_page_)
            {
                auto* first{allocate<page>()};
                if(first) { first->number=number; }
                return first;
            }
            auto* table{recent_page_->table};
            page* fresh{};
            if(!table)
            {
                auto const occupied{slot(recent_page_->number)};
                if(occupied == slot(number)) { return nullptr; }
                table=allocate<index>();
                if(!table) { return nullptr; }
                fresh=allocate<page>();
                if(!fresh) { destroy(table); return nullptr; }
                table->slots[occupied]=table->first=recent_page_;
                recent_page_->table=table;
            }
            else
            {
                if(table->slots[slot(number)]) { return nullptr; }
                fresh=table->spare ? ::std::exchange(table->spare,nullptr) : allocate<page>();
                if(!fresh) { return nullptr; }
            }
            fresh->number=number;
            fresh->table=table;
            auto** link{&table->first};
            while(*link && (*link)->number < number) { link=&(*link)->next; }
            fresh->next=*link;
            *link=table->slots[slot(number)]=fresh;
            return fresh;
        }
        void close_page(page* entry) noexcept
        {
            auto* table{entry->table};
            if(!table) { recent_page_=nullptr; destroy(entry); return; }
            table->slots[slot(entry->number)]=nullptr;
            auto** link{&table->first};
            while(*link != entry) { link=&(*link)->next; }
            *link=entry->next;
            if(entry == recent_page_) { recent_page_=table->first; }
            if(table->spare) { destroy(table->spare); }
            if(table->first) { table->spare=entry; }
            else { destroy(entry); destroy(table); }
        }
        void clear_pages() noexcept
        {
            if(auto* table=recent_page_->table)
            {
                while(auto* entry=table->first) { table->first=entry->next; destroy(entry); }
                if(table->spare) { destroy(table->spare); }
                destroy(table);
            }
            else { destroy(recent_page_); }
            recent_page_=nullptr;
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
                if(entry.records[bit].value.last >= first) { starts |= ::std::uint64_t{1} << bit; }
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
        [[nodiscard]] bool tree_may_intersect(::std::uint64_t first, ::std::uint64_t last) const noexcept
        { return first <= tree_last_ && tree_first_ <= last; }
        template<typename Visitor>
        void visit_tree(::std::uint64_t first, ::std::uint64_t last, Visitor& visitor) const noexcept
        {
            if(tree_may_intersect(first,last)) { visit(root_,first,last,visitor); }
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
            if(recent_page_) { clear_pages(); }
            tree_first_=UINT64_MAX;
            tree_last_=0;
        }
        constexpr void take(managed_allocations& other) noexcept
        {
            reset();
            root_=::std::exchange(other.root_,nullptr);
            recent_page_=::std::exchange(other.recent_page_,nullptr);
            tree_first_=::std::exchange(other.tree_first_,UINT64_MAX);
            tree_last_=::std::exchange(other.tree_last_,0);
        }
        [[nodiscard]] bool empty() const noexcept { return !root_ && !recent_page_; }

        // Visit in linear-address order, including ranges spanning both stores.
        template<typename Visitor>
        void visit(::std::uint64_t first,::std::uint64_t last,Visitor&& visitor) const noexcept
        {
            if(first > last) { return; }
            auto* first_page{recent_page_ && recent_page_->table ? recent_page_->table->first : recent_page_};
            for(auto* entry{first_page}; entry; entry=entry->next)
            {
                if(entry->number < first/page_granules) { continue; }
                if(entry->number > last/page_granules) { break; }
                for(auto mask{starts_in(*entry,first,last)};mask;mask &= mask-1u)
                {
                    auto const& current{entry->records[::std::countr_zero(mask)].value};
                    if(first < current.first) { visit_tree(first,current.first-1u,visitor); }
                    visitor(current);
                    if(current.last >= last) { return; }
                    first=current.last+1u;
                }
            }
            visit_tree(first,last,visitor);
        }

    private:
        template<typename Visitor>
        [[gnu::always_inline]] static void visit_page(page const* entry, ::std::uint64_t first, ::std::uint64_t last, Visitor& visitor) noexcept
        {
            if(entry)
            {
                for(auto mask{starts_in(*entry,first,last)};mask;mask &= mask-1u)
                { visitor(entry->records[::std::countr_zero(mask)].value); }
            }
        }
    public:
        // Borrowed from this directory for an insertion in the visited window,
        // under the same lease and before the next directory mutation.
        class insertion_page
        {
            friend class managed_allocations;
            page* entry_;
            explicit insertion_page(page* entry) noexcept : entry_{entry} {}
        public:
            [[nodiscard]] explicit operator bool() const noexcept { return entry_ != nullptr; }
        };
        // The extent must fit one indexed page. Its exclusion radius can reach
        // either adjacent page. A tree interval can contain cross-page objects
        // or permanent globals, so it disqualifies the whole expanded window.
        template<unsigned Neighbors = 0u, typename Visitor>
        [[gnu::always_inline]] insertion_page visit_indexed_window(::std::uint64_t first, ::std::uint64_t last, Visitor&& visitor) const noexcept
        {
            static_assert(Neighbors < page_granules);
            auto const number{first/page_granules};
            auto const lo{first-Neighbors}, hi{last+Neighbors};
            if(number != last/page_granules || lo > first || hi < last || tree_may_intersect(lo,hi)) { return insertion_page{nullptr}; }
            auto* entry{lookup_page(number)};
            if(!entry) { return insertion_page{nullptr}; }
            if(lo/page_granules != number) { visit_page(lookup_page(lo/page_granules),lo,hi,visitor); }
            visit_page(entry,lo,hi,visitor);
            if(hi/page_granules != number) { visit_page(lookup_page(hi/page_granules),lo,hi,visitor); }
            return insertion_page{entry};
        }
        template<typename Visitor>
        insertion_page visit_near(::std::uint64_t first,::std::uint64_t last,Visitor&& visitor) const noexcept
        {
            if(!recent_page_) { visit_tree(first,last,visitor); return insertion_page{nullptr}; }
            auto const lo{first/page_granules}, hi{last/page_granules};
            if(hi-lo > 1u) { visit(first,last,visitor); return insertion_page{nullptr}; }
            auto* entry{lookup_page(lo)};
            visit_page(entry,first,last,visitor);
            if(lo != hi) { visit_page(lookup_page(hi),first,last,visitor); }
            visit_tree(first,last,visitor);
            return insertion_page{lo == hi ? entry : nullptr};
        }

        // Borrowed until the next directory mutation, under this owner's lease.
        // A release can erase the indexed record it just validated directly.
        class location
        {
            friend class managed_allocations;
            extent* item_;
            page* page_;
            location(extent* item,page* entry) noexcept : item_{item},page_{entry} {}
        public:
            [[nodiscard]] extent* get() const noexcept { return item_; }
        };

        [[nodiscard]] location locate_indexed(::std::uint64_t first,::std::uint64_t last) noexcept
        {
            if(auto* entry=lookup_page(first/page_granules))
            {
                auto const bit{static_cast<unsigned>(first % page_granules)};
                if(entry->starts & (::std::uint64_t{1} << bit))
                {
                    auto& record{entry->records[bit].value};
                    return {record.last == last ? &record : nullptr,entry};
                }
            }
            return {nullptr,nullptr};
        }
        [[nodiscard]] location locate(::std::uint64_t first,::std::uint64_t last) noexcept
        {
            auto const found{locate_indexed(first,last)};
            if(found.page_ || first < tree_first_ || last > tree_last_) { return found; }
            node* n{root_};
            while(n && first != n->value.first) { n=n->child[first > n->value.first]; }
            return {n && n->value.last==last ? &n->value : nullptr,nullptr};
        }
        [[nodiscard]] extent* exact(::std::uint64_t first,::std::uint64_t last) noexcept
        { return locate(first,last).get(); }
        // The caller proves disjointness while holding the owner lease, before insertion.
        [[nodiscard]] bool add(extent value) noexcept
        {
            auto* item{allocate<node>()};
            if(!item && recent_page_ && recent_page_->table && recent_page_->table->spare)
            {
                destroy(::std::exchange(recent_page_->table->spare,nullptr));
                item=allocate<node>();
            }
            if(!item) { return false; }
            item->value=value;
            root_=insert(root_,item);
            if(value.first < tree_first_) { tree_first_=value.first; }
            if(tree_last_ < value.last) { tree_last_=value.last; }
            return true;
        }
        // The caller proved that value fits the borrowed page and is disjoint,
        // and still holds the same owner snapshot. Publication cannot allocate.
        void add_to_page(insertion_page target, extent value) noexcept
        {
            auto* entry{target.entry_};
            auto const bit{static_cast<unsigned>(value.first % page_granules)};
            ::new(static_cast<void*>(&entry->records[bit].value)) extent(value);
            entry->starts |= ::std::uint64_t{1} << bit;
            recent_page_=entry;
        }
        [[nodiscard,gnu::always_inline]] bool add_private(extent value, insertion_page hint=insertion_page{nullptr}) noexcept
        {
            auto const number{value.first/page_granules};
            if(number == value.last/page_granules)
            {
                auto* entry{hint.entry_ && hint.entry_->number == number ? hint.entry_ : lookup_page(number)};
                if(!entry) { entry=open_page(number); }
                if(entry)
                {
                    add_to_page(insertion_page{entry},value);
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
            auto const found{locate(first,last)};
            auto* current{found.get()};
            if(!current || !current->permanent || current->tag != tag) { return false; }
            remove(found);
            return true;
        }
        [[gnu::always_inline]] void remove(location found) noexcept
        {
            if(!found.get()) { return; }
            auto const first{found.item_->first};
            if(auto* entry=found.page_)
            {
                entry->starts &= ~(::std::uint64_t{1} << (first % page_granules));
                if(!entry->starts) { close_page(entry); }
                return;
            }
            remove_tree(first,found.item_->last);
        }
        void remove(extent const& owned) noexcept { remove(locate(owned.first,owned.last)); }

    private:
        void remove_tree(::std::uint64_t first, ::std::uint64_t last) noexcept
        {
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
