#!/usr/bin/env python3
"""Independent synthetic models for immutable native audit numeric settings.

No C++, native executable, ROS, Wasm, compiler, validator or live cgroup runs.
The expected token allowance is derived from complete admitted-record suffixes,
not the production refill recurrence. A separate rational-credit projection
compares repeated explicit parsing with one cache per immutable session grant.
All time, record sizes, sink results and identities are synthetic. Source checks
anchor ownership, initialization order and unchanged accounting to current SHA.
These models establish no native behavior, floating-point or speed benchmark.
"""
import sys
sys.dont_write_bytecode = True
import argparse
import ast
from dataclasses import dataclass, field
from fractions import Fraction
import hashlib
import itertools
import operator
import re
import json
from pathlib import Path
from types import MappingProxyType

ROOT = Path(__file__).resolve().parents[2]
MAX = (1 << 64) - 1
KEYS = ('audit-burst', 'audit-max-rate-hz', 'audit-max-records', 'audit-max-bytes')

@dataclass(frozen=True)
class Grant:
    alias: str
    raw: object
    expected: tuple

@dataclass(frozen=True)
class Config:
    grants: object
    per_second: int = 20
    records: int = 100
    bytes: int = 100000

def configuration(rows, **kw):
    """Independent closed alias catalog; no implicit numeric settings exist."""
    result = {}
    for grant in rows:
        if grant.alias in result:
            raise ValueError('duplicate alias')
        result[grant.alias] = grant
    return Config(MappingProxyType(result), **kw)

def grant(alias='clock', rate='1', burst=2, records=10, byte_limit=1000, mode='allow-and-deny'):
    raw = {'audit': mode, 'audit-burst': str(burst), 'audit-max-rate-hz': rate,
           'audit-max-records': str(records), 'audit-max-bytes': str(byte_limit)}
    # These are explicit independent golden numbers supplied by each scenario.
    return Grant(alias, MappingProxyType(raw), (Fraction(burst), Fraction(rate), records, byte_limit))

@dataclass(frozen=True)
class Event:
    time: Fraction
    size: int
    alias: str = 'clock'
    error: int = 13
    formatted: bool = True
    sink: bool = True
    operation: str = 'time.read-steady'

class Numbers:
    """Observe four explicit numeric conversions, never a native parser call."""
    def __init__(self, cached):
        self.cached = cached
        self.values = {}
        self.identities = {}
        self.parses = {}
    def get(self, cfg, cap, alias):
        if cfg.grants.get(alias) is not cap or cap.alias != alias:
            raise ValueError('foreign configured entry')
        identity = (id(cfg), id(cap))
        if alias in self.identities and self.identities[alias] != identity:
            raise ValueError('cached limiter reused with changed binding')
        if self.cached and alias in self.values:
            return self.values[alias]
        # Required fields are indexed explicitly. A missing field never turns
        # into a default, even when no native resource operation is attempted.
        values = (Fraction(int(cap.raw[KEYS[0]])), Fraction(cap.raw[KEYS[1]]),
                  int(cap.raw[KEYS[2]]), int(cap.raw[KEYS[3]]))
        if values[0] <= 0 or values[1] <= 0 or values[1] > 1000000 or values[2] <= 0 or values[3] <= 0:
            raise ValueError('invalid explicit audit numbers')
        self.parses[alias] = self.parses.get(alias, 0) + 4
        if self.cached:
            self.identities[alias] = identity
            self.values[alias] = values
        return values

class Projection:
    """Synthetic credit state using parsed numbers; no production code executes."""
    def __init__(self, cached, suppressed=0):
        self.numbers = Numbers(cached)
        self.window = Fraction(0)
        self.window_count = self.count = self.bytes = 0
        self.suppressed = suppressed
        self.entries = {}
    def suppress(self):
        self.suppressed = min(MAX, self.suppressed + 1)
        return 'SUPPRESSED'
    def apply(self, cfg, event, supplied=None):
        cap = cfg.grants.get(event.alias) if supplied is None else supplied
        if cap is not None and event.error == 0 and cap.raw['audit'] != 'allow-and-deny':
            return 'SKIPPED'
        if event.time - self.window >= 1:
            self.window = event.time
            self.window_count = 0
        if (not event.formatted or self.window_count >= cfg.per_second or self.count >= cfg.records
                or event.size > max(0, cfg.bytes - self.bytes)):
            return self.suppress()
        if cap is not None:
            burst, rate, maximum, byte_limit = self.numbers.get(cfg, cap, event.alias)
            state = self.entries.get(event.alias)
            if state is None:
                state = {'credit': burst, 'last': event.time, 'count': 0, 'bytes': 0}
                self.entries[event.alias] = state
            elapsed = event.time - state['last']
            state['credit'] = min(burst, state['credit'] + rate * elapsed)
            state['last'] = event.time
            if state['credit'] < 1 or state['count'] >= maximum or event.size > max(0, byte_limit-state['bytes']):
                return self.suppress()
            state['credit'] -= 1
            state['count'] += 1
            state['bytes'] += event.size
        self.window_count += 1
        self.count += 1
        self.bytes += event.size
        if not event.sink:
            self.suppressed = min(MAX, self.suppressed + 1)
            return 'WRITE_FAILED_CONSUMED'
        return 'WRITTEN'
    def receipt(self):
        return (self.window, self.window_count, self.count, self.bytes, self.suppressed,
                {key:(v['count'],v['bytes']) for key,v in self.entries.items()})

class Oracle:
    """Independent history oracle, with no cached strings or refill state.

    At time t each suffix of admitted grant records places a lower bound on
    token deficit: suffix_length - rate*(t-suffix_start). The greatest positive
    deficit gives the remaining burst allowance. Suppressed requests do not
    appear in these histories. Sink failures do, because quotas are consumed
    before the output attempt in the established audit contract.
    """
    def __init__(self, suppressed=0):
        self.reset = Fraction(0)
        self.window_records = []
        self.all_records = []
        self.suppressed = suppressed
        self.by_alias = {}
    def suppress(self):
        self.suppressed = min(MAX, self.suppressed+1)
        return 'SUPPRESSED'
    def apply(self, cfg, event):
        cap = cfg.grants.get(event.alias)
        if cap is not None and event.error == 0 and cap.raw['audit'] == 'deny-only':
            return 'SKIPPED'
        if event.time-self.reset >= 1:
            self.reset=event.time
            self.window_records=[]
        if (not event.formatted or len(self.window_records)>=cfg.per_second or len(self.all_records)>=cfg.records
                or sum(size for _,size in self.all_records)+event.size>cfg.bytes):
            return self.suppress()
        if cap is not None:
            burst,rate,maximum,byte_limit=cap.expected
            history=self.by_alias.setdefault(event.alias,[])
            deficit=max([Fraction(0)]+[Fraction(len(history)-i)-rate*(event.time-time)
                        for i,(time,_) in enumerate(history)])
            if burst-deficit<1 or len(history)>=maximum or sum(size for _,size in history)+event.size>byte_limit:
                return self.suppress()
            history.append((event.time,event.size))
        self.window_records.append((event.time,event.size))
        self.all_records.append((event.time,event.size))
        if not event.sink:
            self.suppressed=min(MAX,self.suppressed+1)
            return 'WRITE_FAILED_CONSUMED'
        return 'WRITTEN'
    def receipt(self):
        return (self.reset,len(self.window_records),len(self.all_records),sum(size for _,size in self.all_records),self.suppressed,
                {key:(len(v),sum(size for _,size in v)) for key,v in self.by_alias.items()})

def main():
    parser=argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--report',type=Path,required=True)
    args=parser.parse_args()
    if args.report.exists():parser.error('fresh report required')
    checks=events_checked=cases=0
    rows=[]
    def check(name,value):
        nonlocal checks
        checks+=1
        if not value:raise AssertionError(name)
    def refuses(name,fn):
        try:fn()
        except (ValueError,KeyError,TypeError):
            check(name,True);return
        raise AssertionError(name+' was admitted')
    times=[Fraction(x,4) for x in (0,0,0,1,2,3,4,4,5,7,8,12,16,20,24,32)]
    sizes=[1,17,18,16,1,31,17,1,1024,17,16,0,32,1,128,17]
    global_settings=[(20,100,100000),(2,100,100000),(20,2,100000),(20,100,34)]
    for rate,burst,maximum,byte_limit,(per_second,total_records,total_bytes) in itertools.product(
            ('0.25','0.5','1','2','64','1000000'),(1,2,7),(1,2,10),(17,34,1024),global_settings):
        cfg=configuration([grant(rate=rate,burst=burst,records=maximum,byte_limit=byte_limit)],
                          per_second=per_second,records=total_records,bytes=total_bytes)
        uncached=Projection(False);cached=Projection(True);oracle=Oracle()
        for i,(time,size) in enumerate(zip(times,sizes)):
            event=Event(time,size,sink=i%5!=0,formatted=i%7!=6)
            expected=oracle.apply(cfg,event)
            check('uncached projection equals independent history oracle',uncached.apply(cfg,event)==expected)
            check('cached projection equals independent history oracle',cached.apply(cfg,event)==expected)
            check('complete accounting remains identical',uncached.receipt()==cached.receipt()==oracle.receipt())
            events_checked+=1
        check('one four-number parse for one initialized alias',cached.numbers.parses=={'clock':4})
        check('repeated parsing cannot use fewer conversions',sum(uncached.numbers.parses.values())>=4)
        cases+=1
    rows.append({'name':'independent history oracle / fractional rates / burst and byte-record boundaries / sink failures',
                 'status':'PASS','scenarios':cases,'synthetic_events':events_checked})
    a=grant();b=grant(alias='second',mode='deny-only',records=1)
    cfg=configuration([a,b],per_second=3,records=5,bytes=100)
    cached=Projection(True);uncached=Projection(False);oracle=Oracle()
    sequence=[Event(Fraction(0),10),Event(Fraction(0),10,'second',0),Event(Fraction(0),10,'second'),
              Event(Fraction(0),10,'unknown'),Event(Fraction(1),10,operation='capability.close'),
              Event(Fraction(2),10,operation='time.read-steady'),Event(Fraction(3),10,operation='capability.revoke'),
              Event(Fraction(4),10,'second'),Event(Fraction(5),10,'unknown')]
    for event in sequence:
        result=oracle.apply(cfg,event)
        check('mixed aliases / close-reopen-revoke accounting unchanged',cached.apply(cfg,event)==uncached.apply(cfg,event)==result)
        check('mixed complete histories match',cached.receipt()==uncached.receipt()==oracle.receipt())
    check('each configured alias parsed once across close/reopen',cached.numbers.parses=={'clock':4,'second':4})
    check('unknown aliases cannot create grant cache state','unknown' not in cached.numbers.values)
    skipped=Projection(True);skip_cfg=configuration([grant(mode='deny-only')])
    check('deny-only success skips before cache/window',skipped.apply(skip_cfg,Event(Fraction(9),1024,error=0))=='SKIPPED'
          and skipped.window==0 and not skipped.numbers.parses)
    for cfg_kw,event in [({'per_second':0},Event(Fraction(0),1)),({'records':0},Event(Fraction(0),1)),
                         ({'bytes':1},Event(Fraction(0),2)),({},Event(Fraction(0),1024,formatted=False))]:
        pending=Projection(True);own=configuration([grant()],**cfg_kw)
        check('global refusal precedes numeric cache initialization',pending.apply(own,event)=='SUPPRESSED' and not pending.numbers.parses)
    saturated=Projection(True,MAX-1);saturated_oracle=Oracle(MAX-1);zero=configuration([],records=0)
    for time in (0,1,2):
        event=Event(Fraction(time),1,'unknown')
        check('suppression saturates identically',saturated.apply(zero,event)==saturated_oracle.apply(zero,event)
              and saturated.receipt()==saturated_oracle.receipt())
    check('suppression cannot wrap',saturated.suppressed==MAX)
    # Independently partition the byte interval instead of subtracting two
    # unsigned counters. This includes saturated totals and their zero-size
    # boundary; real formatted audit records always have positive length.
    for maximum,used,size in itertools.product((1,17,1024,1<<30),
            (0,1,16,17,1024,(1<<30)-1,1<<30,MAX),(0,1,17,1024,1<<30,MAX)):
        expected=(size==0) if used>=maximum else used+size<=maximum
        check('byte refusal clamp matches independent bounded interval',
              (size<=maximum-min(used,maximum))==expected)
    # The largest permitted non-byte quota is represented exactly by double;
    # a long idle interval refills only the explicitly configured burst.
    ceiling=configuration([grant(rate='1000000',burst=1048576,records=1048576,byte_limit=1<<30)])
    large_cache=Projection(True);large_oracle=Oracle()
    for time in (Fraction(0),Fraction(0),Fraction(1000000),Fraction(1000000)):
        event=Event(time,1024)
        check('explicit upper-bound burst remains clamped',
              large_cache.apply(ceiling,event)==large_oracle.apply(ceiling,event)
              and large_cache.receipt()==large_oracle.receipt())
    check('large idle refill parses the numeric limits once',large_cache.numbers.parses=={'clock':4})
    refuses('duplicate alias catalog',lambda:configuration([a,a]))
    for key in KEYS:
        missing=dict(a.raw);missing.pop(key)
        bad=Grant('clock',MappingProxyType(missing),a.expected);own=configuration([bad])
        refuses('missing explicit numeric field '+key,lambda own=own:Projection(True).apply(own,Event(Fraction(0),1)))
    foreign=grant();bound=Projection(True);bound.apply(cfg,Event(Fraction(0),1))
    refuses('foreign copied cap cannot seed configured cache',lambda:Projection(True).apply(cfg,Event(Fraction(0),1),foreign))
    replaces=configuration([foreign])
    refuses('reused limiter rejects changed configuration and same alias',lambda:bound.apply(replaces,Event(Fraction(1),1)))
    refuses('model configuration fields immutable',lambda:operator.setitem(a.raw,'audit-burst','7'))
    rows.append({'name':'alias/configuration binding, explicit-only settings, unknown-alias isolation, skip/suppression edges','status':'PASS'})

    # Extract selection/marker definitions only; never import the native runner
    # or execute its main. This additive suite must leave both inventories exact.
    unit_path=ROOT/'tests/ros_aarch64/run_unit.py';tree=ast.parse(unit_path.read_text())
    wanted={'SUITES','WASI_REGRESSIONS','SEARCH_CONTRACTS','CUSTOM_CONTRACTS','AUDIT_CACHE_CONTRACTS','ROS2_ENVIRONMENT_CONTRACTS','FULL_CONTRACT_NAMES'}
    nodes=[node for node in tree.body if isinstance(node,ast.Assign) and any(isinstance(t,ast.Name) and t.id in wanted for t in node.targets)
           or isinstance(node,ast.FunctionDef) and node.name in ('selected_suites','semantic_result')]
    selected_scope={'re':re}
    exec(compile(ast.Module(body=nodes,type_ignores=[]),str(unit_path)+'<selection-model-only>','exec'),selected_scope)
    select=selected_scope['selected_suites'];marker=selected_scope['semantic_result']
    expected_original={'security','ipc','ipc-client-deadline','sorted-rows','sorted-rows-cpp17','sdk'}
    check('original six suites unchanged',{row[0] for row in select('original')}==expected_original and len(select('original'))==6)
    wasi=select('wasip1-default-deny')
    check('separate seventeen WASI suites unchanged',len(wasi)==17 and all(row[0].startswith('wasip1-') for row in wasi))
    check('all retains exactly original six plus seventeen WASI',len(select('all'))==23 and 'native-audit-cache' not in {row[0] for row in select('all')})
    check('cache unit opt-in C++17 selection exact',select('native-audit-cache')==[('native-audit-cache','tests/ros_native_audit_io/cache_unit.cc',17,None,False)])
    check('cache unit uses full SDK MD evidence path','native-audit-cache' in selected_scope['FULL_CONTRACT_NAMES'])
    check('separate ROS2 environment opt-in C++17 selection exact',select('ros2-environment-contract')==[('ros2-environment-contract','tests/ros2_environment/unit.cc',17,None,False)]
          and 'ros2-environment-contract' in selected_scope['FULL_CONTRACT_NAMES'])
    env_completion='PASS C++17 ROS2 checked environment contract checks=13\n'
    check('exact independent environment marker admitted',marker('ros2-environment-contract',env_completion,False))
    check('environment duplicate marker refused',not marker('ros2-environment-contract',env_completion*2,False))
    check('unrelated environment marker refused',not marker('ros2-environment-contract','PASS unrelated\n',False))
    completion='PASS C++17 native audit numeric cache checks=151 cached_aliases=3 hot_cpp_allocations=0\n'
    check('exact cache marker admitted',marker('native-audit-cache',completion,False))
    for bad in (completion*2,completion.replace('checks=151','checks=0'),completion.replace('cached_aliases=3','cached_aliases=2'),
                completion.replace('hot_cpp_allocations=0','hot_cpp_allocations=1'),completion+'FAIL synthetic\n',
                'PASS unrelated source\n',completion.replace('C++17','C++20')):
        check('invalid cache markers cannot satisfy selection',not marker('native-audit-cache',bad,False))
    rows.append({'name':'opt-in C++17 suite / unchanged6+17 inventories / strict semantic-marker selection','status':'PASS'})

    names=['gateways/common/server.h','gateways/common/config.h','gateways/common/native_io.h',
           'gateways/ros1/main.cc','gateways/ros2/main.cc','tests/ros_native_audit_io/cache_models.py','tests/ros_native_audit_io/README.md',
           'tests/ros_native_audit_io/cache_unit.cc','tests/ros_aarch64/run_unit.py']
    source={n:(ROOT/n).read_text() for n in names}
    server=source[names[0]];config=source[names[1]]
    start=server.index('inline void audit(');end=server.index('inline void serve_connection(',start);audit=server[start:end]
    init=audit[audit.index('if(!grant.initialized)'):audit.index('else if(grant.configured_owner')]
    later=audit[audit.index('else if(grant.configured_owner'):]
    for key in KEYS:check('numeric source field parsed once only in init '+key,init.count('"'+key+'"')==1 and '"'+key+'"' not in later)
    check('cache bound to original cfg map entry','cfg.policy.capabilities.find(r.alias)' in init and '&configured->second!=c' in init and 'c->alias!=r.alias' in init)
    check('subsequent reuse checks both identities','grant.configured_owner!=&cfg||grant.configured_grant!=c||c->alias!=r.alias' in later)
    check('global refusal precedes initialization',audit.index('if(!formatted||global.written')<audit.index('if(!grant.initialized)'))
    check('mode skip precedes all accounting',audit.index('if(!error&&c&&field(*c,"audit")!="allow-and-deny")')<audit.index('auto now'))
    check('cache publication follows complete explicit parsing',init.index('grant.configured_owner=&cfg')>max(init.index('"'+key+'"') for key in KEYS))
    check('same cached refill expression','std::min(grant.audit_burst,grant.tokens+std::chrono::duration<double>(now-grant.updated).count()*grant.audit_rate)' in later)
    check('accounting still precedes output',audit.index('grant.tokens-=1;++grant.total_records;grant.total_bytes+=text.size();')<audit.index('++global.written;++global.total_records;global.total_bytes+=text.size();')<audit.index('write_audit_text(text)'))
    check('same quota comparisons and underflow clamp','grant.tokens<1||grant.total_records>=grant.audit_record_limit||text.size()>grant.audit_byte_limit-std::min(grant.total_bytes,grant.audit_byte_limit)' in later)
    check('same saturating write suppression','if(global.suppressed!=std::numeric_limits<std::uint64_t>::max())' in audit and 'if(!native_io::write_audit_text(text)) { suppress(); }' in audit)
    formatter=audit[audit.index('native_io::fixed_text<1024>'):audit.index('auto text')]
    check('record contains no payload/resource/type values','r.data' not in formatter and 'c->resource' not in formatter and 'c->type' not in formatter)
    check('record operation and alias remain dynamic','record.append(r.operation)' in formatter and 'record.append(r.alias)' in formatter)
    check('session owns both quota tables','audit_limiter log_limit;' in server and 'std::map<std::string,audit_limiter,std::less<>> grant_logs;' in server)
    check('audit receives cfg-selected original entry','auto cap{cfg.policy.capabilities.find(r.alias)}' in server and 'c?grant_logs[r.alias]:log_limit,cfg' in server)
    check('config aliases and fields reject duplicates','!c.policy.capabilities.emplace(cap.alias,std::move(cap)).second' in config and '!i->second.limits.emplace(t[2],t[3]).second' in config)
    check('loader validates every configured entry','for(auto const& p:c.policy.capabilities) { validate(c.policy.ros_version,p.second); }' in config)
    check('configuration retained read-only through joined threads','serve_connection(int fd,configuration const& cfg' in server and 'run(configuration const& cfg' in server and 'for(auto& w:workers) { w.thread.join(); }' in server)
    rows.append({'name':'source ownership/binding/parse-once/accounting-order/no-payload anchors','status':'PASS'})
    report={'status':'PURE_SOURCE_AND_INDEPENDENT_MODEL_PASS','checks':checks,'scenario_count':cases,'synthetic_event_count':events_checked,'rows':rows,
            'native_CPP_compilation':'NOT_PERFORMED','native_audit_IO_or_ROS_execution':'NOT_PERFORMED','speed_benchmark':'NOT_PERFORMED',
            'model_parser_counts':'Four explicit Python rational/integer conversions per initialized alias; not native std::stod instrumentation.',
            'limitations':['Exact rational timestamps/rates model the chosen binary-exact edge sequences; they do not execute C++ double arithmetic.',
                           'Source anchors and production lifetime review do not prove behavior of callers that mutate a const configuration or reuse dangling pointers.',
                           'Output failures are synthetic booleans; actual native nonblocking sink behavior retains its separate test/report.'],
            'inputs':{n:{'bytes':len((ROOT/n).read_bytes()),'sha256':hashlib.sha256((ROOT/n).read_bytes()).hexdigest()} for n in names}}
    args.report.write_text(json.dumps(report,indent=2)+'\n')
    print('PURE_SOURCE_AND_INDEPENDENT_MODEL_PASS',checks,'checks;',cases,'scenarios;',events_checked,'events; native/benchmark NOT_PERFORMED')

if __name__=='__main__':main()
