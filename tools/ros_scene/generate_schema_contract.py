#!/usr/bin/env python3
"""Generate bounded canonical shapes, never VM registration or authorization.

The catalogue remains the reviewed source for field order and explicit union
tags. Semantic/provider constraints are not replaced by structural validation.
Run --check in the existing resource gate to reject stale generated headers.
"""
import argparse
import json
from pathlib import Path

ROOT = Path(__file__).resolve().parents[2]


def generate():
    catalogue = json.loads((ROOT/'tools/ros_scene/interface-catalogue.json').read_text())
    definitions = catalogue['schemas']
    types = {}
    nodes = []
    edges = []
    choices = []
    texts = []

    def declare(name):
        if name in types:
            return types[name]
        index = len(nodes)
        types[name] = index
        nodes.append(None)
        if name in ('u32', 'u64', 'i64', 'f64', 'bool', 'bytes', 'text'):
            node = (name.replace('bool', 'boolean'), 0, 0, 0)
        elif name.startswith(('array<', 'optional<')):
            kind, child = name.split('<', 1)
            node = (kind, 0, 0, declare(child[:-1]))
        else:
            record = definitions[name]
            def fields(items):
                children = []
                for field, kind in items:
                    if field in record.get('enums', {}):
                        assert kind == 'text', (name, field)
                        key = name+'__'+field
                        if key not in types:
                            types[key] = len(nodes)
                            start = len(texts)
                            texts.extend(record['enums'][field])
                            nodes.append(('selector', start, len(record['enums'][field]), 0))
                        children.append(types[key])
                    else:
                        children.append(declare(kind))
                start = len(edges)
                edges.extend(children)
                return start, len(children)
            if 'fields' in record:
                start, count = fields(record['fields'])
                node = ('record', start, count, 0)
            else:
                variants = []
                for tag, items in record['variant'].items():
                    start, count = fields(items)
                    variants.append((record['variant_tags'][tag], start, count))
                start = len(choices)
                choices.extend(variants)
                node = ('variant', start, len(variants), 0)
        nodes[index] = node
        return index

    for name in definitions:
        declare(name)
    operations = []
    for op in catalogue['operations']:
        if op['wire'] not in ('proposed_record', 'canonical_record'):
            continue
        shape = []
        for direction in ('request', 'result'):
            name = op['name'].replace('.', '_')+'__'+direction
            definitions[name] = {'fields': [(f['name'], f['type']) for f in op[direction]]}
            shape.append(declare(name))
        operations.append((op['name'], *shape))
    assert all(x is not None for x in nodes)
    identifier = lambda name: name.replace('<', '_of_').replace('>', '').replace('bool', 'boolean')
    lines = ['/** @file',
             ' * @brief Generated canonical record shapes; no imports or authority.',
             ' * @details Regenerate with tools/ros_scene/generate_schema_contract.py.',
             ' * Every traversal has explicit byte/node/element/depth reservations.',
             ' * This validates framing, UTF-8, tags and closed selectors only.',
             ' * Providers must additionally validate immutable profiles, ownership,',
             ' * physical bounds, secondary capabilities and result commit semantics.',
             ' */', '#pragma once', '#include "wire.h"', '#include <array>',
             'namespace uwvm2::imported::ros::scene::schema_contract', '{',
             '    enum class kind : unsigned char {u32,u64,i64,f64,boolean,bytes,text,array,optional,record,variant,selector};',
             '    enum class type : std::uint16_t {']
    for name, index in types.items():
        lines.append('        '+identifier(name)+'='+str(index)+',')
    lines += ['    };',
              '    struct shape {kind tag;std::uint32_t first,count,child;};',
              '    struct alternative {std::uint64_t tag;std::uint32_t first,count;};',
              '    inline constexpr std::array<shape,'+str(len(nodes))+'> shapes{{']
    lines += ['        {kind::%s,%d,%d,%d},' % node for node in nodes]
    lines += ['    }};', '    inline constexpr std::array<type,'+str(len(edges))+'> fields{{']
    reverse = {i: identifier(n) for n, i in types.items()}
    lines += ['        type::'+reverse[i]+',' for i in edges]
    lines += ['    }};', '    inline constexpr std::array<alternative,'+str(len(choices))+'> alternatives{{']
    lines += ['        {%d,%d,%d},' % choice for choice in choices]
    lines += ['    }};', '    inline constexpr std::array<std::string_view,'+str(len(texts))+'> selectors{{']
    lines += ['        '+json.dumps(word)+',' for word in texts]
    lines += ['    }};', '    struct operation {std::string_view name;type request,result;};',
              '    inline constexpr std::array<operation,'+str(len(operations))+'> operations{{']
    lines += ['        {%s,type::%s,type::%s},' % (json.dumps(name), reverse[request], reverse[result])
              for name, request, result in operations]
    lines += ['    }};']
    lines += r'''
    /** @brief Explicit per-record reservations; all zero means no decode work.
     * @details Depth has an independent hard ceiling of 64 even for a trusted
     * larger setting. Nodes/elements are whole-record budgets, not per-array
     * allowances that a nested value can multiply. No allocation occurs.
     */
    struct limits
    {std::size_t bytes{},nodes{},elements{};unsigned depth{};};
    class validator
    {
        wire::reader& input;limits remaining;
        bool members(std::uint32_t first,std::uint32_t count,unsigned depth) noexcept
        {
            if(first>fields.size()||count>fields.size()-first)return false;
            for(std::uint32_t n{};n<count;++n)if(!visit(fields[first+n],depth))return false;
            return true;
        }
        bool visit(type selected,unsigned depth) noexcept
        {
            auto index=static_cast<std::size_t>(selected);
            if(!remaining.nodes||depth>remaining.depth||depth>64||index>=shapes.size())return false;
            --remaining.nodes;auto const& item=shapes[index];std::uint64_t number{};
            switch(item.tag)
            {
            case kind::u32:{std::uint32_t value{};return input.u32(value);}
            case kind::u64:case kind::i64:case kind::f64:return input.u64(number);
            case kind::boolean:{bool value{};return input.boolean(value);}
            case kind::bytes:{wire::bytes value;return input.blob(value);}
            case kind::text:{std::string_view value;return input.text(value);}
            case kind::selector:
            {
                std::string_view value;if(!input.text(value))return false;
                for(std::uint32_t n{};n<item.count;++n)if(value==selectors[item.first+n])return true;
                return false;
            }
            case kind::record:return members(item.first,item.count,depth+1);
            case kind::optional:
                if(!input.u64(number)||number>1)return false;
                return !number||visit(static_cast<type>(item.child),depth+1);
            case kind::array:
                if(!input.u64(number)||number>remaining.elements)return false;
                remaining.elements-=static_cast<std::size_t>(number);
                for(std::uint64_t n{};n<number;++n)if(!visit(static_cast<type>(item.child),depth+1))return false;
                return true;
            case kind::variant:
                if(!input.u64(number))return false;
                for(std::uint32_t n{};n<item.count;++n)
                {auto const& choice=alternatives[item.first+n];if(number==choice.tag)return members(choice.first,choice.count,depth+1);}
                return false;
            }
            return false;
        }
    public:
        validator(wire::reader& value,limits bounds) noexcept:input(value),remaining(bounds){}
        /** @brief Consume exactly one permitted shape, leaving no trailing bytes. */
        bool validate(type selected) noexcept
        {return input.remaining()<=remaining.bytes&&visit(selected,0)&&input.done();}
    };
    inline bool validate(wire::bytes data,type selected,limits bounds) noexcept
    {wire::reader input{data};validator check{input,bounds};return check.validate(selected);}
    inline operation const* find(std::string_view name) noexcept
    {for(auto const& op:operations)if(op.name==name)return &op;return nullptr;}
}
'''.strip('\n').splitlines()
    return '\n'.join(lines)+'\n'


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--check', action='store_true')
    args = parser.parse_args()
    output = ROOT/'src/uwvm2/imported/ros/scene/schema_contract.h'
    text = generate()
    if args.check:
        assert output.read_text() == text, 'canonical schema header is stale'
        print('PASS canonical schema generation')
    else:
        output.write_text(text)


if __name__ == '__main__':
    main()
