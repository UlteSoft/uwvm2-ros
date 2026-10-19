#!/usr/bin/env python3
"""Source/independent Python models only; never compile or execute ROS/Wasm.

The candidate oracle enumerates complete path segments. A second projection
models the zero-allocation C++ predicate using string ranges. Agreement does
not execute the C++ implementation: real Worker/Gateway/native fixtures remain
PENDING until run in the qualified Linux 64 GiB cgroup. Source checks anchor
the intended guards and byte accounting to exact current file hashes.
"""
from __future__ import annotations

import argparse
import hashlib
import json
from pathlib import Path
import sys

sys.dont_write_bytecode = True
ROOT = Path(__file__).resolve().parents[2]
COUNTS = {"candidate_models": 0, "query_trace_models": 0, "action_budget_models": 0, "source_checks": 0}


def check(value: bool, category: str, message: str) -> None:
    COUNTS[category] += 1
    if not value:
        raise AssertionError(message)


def within(prefix: str, name: str) -> bool:
    """Independent canonical segment-prefix oracle."""
    if prefix == "/":
        return True
    return name.split("/")[1:len(prefix.split("/"))] == prefix.split("/")[1:]


def candidates(resource: str, prefix: str, enabled: bool) -> list[str]:
    components = resource.split("/")[1:]
    key, namespace = components[-1], components[:-1]
    names = ["/" + "/".join(namespace[:depth] + [key])
             for depth in range(len(namespace), -1, -1)]
    if not enabled:
        names = names[:1]
    return [name for name in names if within(prefix, name)]


def range_predicate(resource: str, prefix: str, enabled: bool, name: str) -> bool:
    """Python projection of C++ range checks, not execution of that helper."""
    inside = prefix == "/" or name == prefix or (name.startswith(prefix) and name[len(prefix):].startswith("/"))
    if not inside:
        return False
    if name == resource:
        return True
    if not enabled:
        return False
    separator = resource.rfind("/")
    key = resource[separator:]
    if not name.endswith(key):
        return False
    original_namespace, candidate_namespace = resource[:separator], name[:-len(key)]
    return not candidate_namespace or (original_namespace.startswith(candidate_namespace)
                                      and original_namespace[len(candidate_namespace):].startswith("/"))


def trace(resource: str, prefix: str, enabled: bool, present: set[str],
          timeout: int = 100, costs: dict[str, int] | None = None,
          errors: dict[str, int] | None = None, request: bytes = b"") -> tuple[int, str, list[str]]:
    """Bounded-query behavioral model with an observable native-query trace."""
    if request:
        return 22, "", []
    if not within(prefix, resource):
        return 13, "", []
    queried, elapsed = [], 0
    for candidate in candidates(resource, prefix, enabled):
        if elapsed >= timeout:
            return 110, "", queried
        queried.append(candidate)
        error = (errors or {}).get(candidate, 0)
        elapsed += (costs or {}).get(candidate, 1)
        if error:
            return error, "", queried
        if elapsed >= timeout:
            return 110, "", queried
        if candidate in present:
            return 0, candidate, queried
    return 2, "", queried


def candidate_checks() -> None:
    resources = ["/", "/key", "/scope/key", "/scope/ns/key", "/scope/ns/child/key", "/scope_ns/child/key"]
    prefixes = ["/", "/scope", "/scope/ns", "/scope/ns/child", "/scope_ns", "/scope_bad", "/key", "/scope/key"]
    malformed = ["", "key", "/other", "/scope//key", "/scope/ns/../key", "/scope/ns_bad/key",
                 "/scope_bad/key", "/scope/ns/child/key/extra", "/scope/ns/child/sibling/key",
                 "/scope/ns/child/other", "/scope/ns/child/key\x00", "/\ufeffkey", "//", "/scope/", "/scope/ns/child/key/"]
    for resource in resources:
        for prefix in prefixes:
            for enabled in (False, True):
                expected = set(candidates(resource, prefix, enabled))
                names = set(malformed + resources + candidates(resource, "/", True))
                for name in names:
                    check(range_predicate(resource, prefix, enabled, name) == (name in expected),
                          "candidate_models", f"candidate mismatch: {(resource, prefix, enabled, name)}")
    check(candidates("/scope/ns/child/key", "/scope", True) ==
          ["/scope/ns/child/key", "/scope/ns/key", "/scope/key"], "candidate_models", "parent order")
    check(candidates("/scope/ns/child/key", "/scope/ns", True) ==
          ["/scope/ns/child/key", "/scope/ns/key"], "candidate_models", "narrow prefix")


def query_checks() -> None:
    initial, parent, grandparent, outside = "/scope/ns/child/key", "/scope/ns/key", "/scope/key", "/key"
    rows = [
        (False, "/scope", {initial, parent}, 100, {}, {}, b"", (0, initial, [initial])),
        (False, "/scope", {parent}, 100, {}, {}, b"", (2, "", [initial])),
        (True, "/scope", {initial, parent}, 100, {}, {}, b"", (0, initial, [initial])),
        (True, "/scope", {parent, grandparent}, 100, {}, {}, b"", (0, parent, [initial, parent])),
        (True, "/scope", {grandparent}, 100, {}, {}, b"", (0, grandparent, [initial, parent, grandparent])),
        (True, "/scope", {outside}, 100, {}, {}, b"", (2, "", [initial, parent, grandparent])),
        (True, "/scope/ns", {grandparent, outside}, 100, {}, {}, b"", (2, "", [initial, parent])),
        (True, "/scope_bad", {initial, outside}, 100, {}, {}, b"", (13, "", [])),
        (True, "/", {outside}, 100, {}, {}, b"", (0, outside, [initial, parent, grandparent, outside])),
        (True, "/scope", {parent}, 0, {}, {}, b"", (110, "", [])),
        (True, "/scope", {parent}, 10, {initial: 10}, {}, b"", (110, "", [initial])),
        (True, "/scope", {initial}, 10, {initial: 11}, {}, b"", (110, "", [initial])),
        (True, "/scope", {grandparent}, 10, {initial: 6, parent: 6}, {}, b"", (110, "", [initial, parent])),
        (True, "/scope", {grandparent}, 100, {}, {parent: 107}, b"", (107, "", [initial, parent])),
        (True, "/scope", {parent}, 100, {}, {}, b"substitute", (22, "", [])),
    ]
    for enabled, prefix, present, timeout, costs, errors, request, expected in rows:
        actual = trace(initial, prefix, enabled, present, timeout, costs, errors, request)
        check(actual == expected, "query_trace_models", f"trace mismatch: {actual} != {expected}")
        check(all(within(prefix, name) for name in actual[2]), "query_trace_models", "outside prefix queried")
    # The canonical resource ceiling also bounds the namespace walk itself.
    deepest = "/a" * 509 + "/key"
    check(len(deepest) <= 1024 and len(candidates(deepest, "/", True)) == 510,
          "query_trace_models", "canonical name walk is not structurally bounded")


def action_checks() -> None:
    for count in range(49):
        for kind in range(1, 5):
            request = (1).to_bytes(8, "little") + bytes([kind]) + count.to_bytes(4, "little") + (7).to_bytes(4, "little") * count
            advertised = request[:8] + request[9:] if kind == 1 else request
            for responder_delta in (-1, 0, 1):
                for advertiser_delta in (-1, 0, 1):
                    responder_limit = len(request) + responder_delta
                    advertiser_limit = len(advertised) + advertiser_delta
                    wire_oracle = len(request) <= responder_limit and len(advertised) <= advertiser_limit
                    predicate = len(request) <= responder_limit and len(request) - (1 if kind == 1 else 0) <= advertiser_limit
                    check(predicate == wire_oracle, "action_budget_models", "advertiser/responder framing mismatch")
    check(len((1).to_bytes(8, "little") + (0).to_bytes(4, "little")) == 12,
          "action_budget_models", "empty feedback canonical envelope")


INPUTS = [
    "src/uwvm2/imported/ros/common/parameter_search_contract.h",
    "src/uwvm2/imported/ros/common/message.h",
    "src/uwvm2/imported/ros/policy/schema.h",
    "gateways/common/config.h", "gateways/ros1/resources.h", "gateways/ros1/action.h",
    "tests/ros1_search/contract_test.cc", "tests/ros1_search/gateway_contract_test.cc",
    "tests/ros1_search/native_ros1.cc", "tests/ros_action/feedback_budget_ros1.cc",
    "tests/ros1_search/README.md", "tests/ros1_search/verify_models.py",
    "src/uwvm2/imported/ros/policy/policy.h",
]


def source_checks() -> dict[str, dict[str, int | str]]:
    files = {name: (ROOT / name).read_text(encoding="utf-8") for name in INPUTS}
    def source(value: bool, message: str) -> None:
        check(value, "source_checks", message)
    helper = files[INPUTS[0]]
    source('field->second == "true"' in helper and 'cap.operation == "param.search"' in helper, "explicit opt-in guard")
    source('name[prefix.size()] == \'/\'' in helper, "segment prefix boundary")
    source('original_namespace[candidate_namespace.size()] == \'/\'' in helper, "ancestor segment boundary")
    source('candidate.erase(parent, separator - parent)' in helper, "fixed leaf must survive parent transition")
    source('candidate_namespace.empty()' in helper, "root namespace candidate")
    worker = files[INPUTS[1]].split("inline result validate_response_payload", 1)[1]
    source('if(!encoded || cap.operation != "param.search") { return encoded; }' in worker, "framing before search-name validation")
    source('parameter_search_name_allowed(cap, name)' in worker, "Worker validates returned search name")
    policy = files[INPUTS[2]]
    source('v == version::ros1 && cap.operation == "param.search" && key == "search-parent-namespaces"' in policy, "Worker field applicability")
    source('else if(parent_search)' in policy and 'value != "true" && value != "false"' in policy, "Worker boolean spelling")
    source('search-parent-namespaces' not in policy.split("inline result validate_capability", 1)[0], "additive field must not become required")
    cli = files[INPUTS[12]].split('result set_capability(', 1)[1].split('target_policy const* find_target', 1)[0]
    source('v == version::ros1 && it->second.operation == "param.search" && key == "search-parent-namespaces"' in cli, "CLI optional field applicability")
    source('&& !parent_search' in cli and 'parent_search && value != "true" && value != "false"' in cli, "CLI exact optional boolean acceptance")
    source(cli.index('parent_search && value != "true"') < cli.index('limits.emplace(key, value)'), "CLI validates boolean before insertion")
    gateway = files[INPUTS[3]]
    source('if(v==ros::version::ros1&&c.operation=="param.search") { allowed.insert("search-parent-namespaces"); }' in gateway, "Gateway field applicability")
    source('k=="search-parent-namespaces"&&value!="true"&&value!="false"' in gateway, "Gateway boolean spelling")
    search = files[INPUTS[4]].split('if(cap.operation=="param.search")', 1)[1].split('XmlRpc::XmlRpcValue args,response;', 1)[0]
    source(search.index('if(!request.empty())') < search.index('parameter_search_name_allowed(cap,cap.resource)'), "request refusal order")
    source(search.index('parameter_search_name_allowed(cap,cap.resource)') < search.index('for(;;)'), "initial prefix guard")
    source(search.index('parameter_search_name_allowed(cap,candidate)') < search.index('search.execute_bounded'), "prefix guard before each native query")
    source(search.index('if(left<=0)') < search.index('search.execute_bounded'), "shared deadline before query")
    source(search.index('if(std::chrono::steady_clock::now()>=deadline)') > search.index('search.execute_bounded'), "shared deadline after query")
    source('search_args[1]=candidate;' in search and 'search.execute_bounded("hasParam"' in search, "native query uses fixed candidate")
    source('search_response[2].getType()!=XmlRpc::XmlRpcValue::TypeBoolean' in search, "native response kind validation")
    source('parameter_search_parents_enabled(cap)||!ros::parameter_search_next_candidate(candidate)' in search, "no implicit parent transition")
    source('searchParam' not in search, "no native unrestricted parent search")
    action = files[INPUTS[5]]
    source('data.size()>maximum||data.size()-(kind==1?1u:0u)>limit(g->capability' in action, "responder/full and advertiser/projected feedback sizes")
    source(action.index('data.size()-(kind==1?1u:0u)') < action.index('e->feedback.publish(feedback)'), "budget checks precede publication")
    source('if(result){d.push_back(kind);}' in action, "native feedback omits only outcome byte")
    source('for(unsigned count:{0U,1U,2U,48U})' in files[INPUTS[9]], "native feedback boundary inventory")
    source('for(std::uint8_t kind:{std::uint8_t{2},std::uint8_t{3}})' in files[INPUTS[9]], "terminal success/abort boundary inventory")
    return {name: {"bytes": len((ROOT / name).read_bytes()), "sha256": hashlib.sha256((ROOT / name).read_bytes()).hexdigest()}
            for name in INPUTS}


def main() -> None:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--output", type=Path, help="Write a new JSON report; existing reports are never replaced.")
    args = parser.parse_args()
    candidate_checks(); query_checks(); action_checks(); inputs = source_checks()
    report = {"status": "SOURCE_AND_MODEL_PASS", "counts": COUNTS, "inputs": inputs,
              "real_compilation": "NOT_PERFORMED", "cpp_execution": "NOT_PERFORMED",
              "ros_execution": "NOT_PERFORMED", "wasm_execution": "NOT_PERFORMED",
              "native_query_count_qualification": "NOT_PERFORMED",
              "limitations": ["Python projections do not execute C++.",
                              "Models do not prove native XMLRPC interruptibility or ROS network behavior.",
                              "All prepared C++/ROS fixtures require the verified Linux 64 GiB cgroup."]}
    serialized = json.dumps(report, indent=2, sort_keys=True) + "\n"
    if args.output:
        with args.output.open("x", encoding="utf-8") as target:
            target.write(serialized)
    print(serialized, end="")


if __name__ == "__main__":
    main()
