"""Declare exact resource benchmark grants and guest imports.

@file ros_resource_cases.py
@brief Keep fixture selection separate from production authorization.
@details Every case supplies all mandatory limits, including allow-and-deny
auditing. Names refer exclusively to the locally created isolated test peer.
"""

CASES = {
    "graph-list-topics": (1, "graph_list_topics", (1, 2)),
    "graph-inspect-topic": (2, "graph_inspect_topic", (1, 2)),
    "graph-list-nodes": (3, "graph_list_nodes", (1, 2)),
    "graph-inspect-node": (4, "graph_inspect_node", (1, 2)),
    "graph-list-services": (5, "graph_list_services", (1, 2)),
    "graph-inspect-service": (6, "graph_inspect_service", (1, 2)),
    "parameter-get": (7, "parameter_get", (1, 2)),
    "parameter-set": (8, "parameter_set", (1, 2)),
    "parameter-list": (9, "parameter_list", (1, 2)),
    "parameter-describe": (10, "parameter_describe", (2,)),
    "parameter-exists": (11, "parameter_exists", (1,)),
    "parameter-search": (12, "parameter_search", (1,)),
}


def configuration(name, version, common):
    """Return a complete, exact capability for one supported fixture case."""
    number, imported, versions = CASES[name]
    assert version in versions
    scope = "/uwvm_perf/peer"
    node = scope + "/resource_peer"
    graph = number <= 6
    operation = imported.replace("graph_", "graph.").replace("parameter_", "param.").replace("_", "-")
    resource = scope if graph else node + "/value"
    if number == 2:
        resource = scope + "/topic"
    elif number == 4:
        resource = node
    elif number == 6:
        resource = scope + "/trigger"
    elif number == 9:
        resource = node
    grant = ["client", operation, resource,
             "builtin/graph-list-v1" if graph else "builtin/string-v1",
             "builtin:graph-list-v1" if graph else "builtin:param-string-v1"]
    limits = {key: value for key, value in common.items()
              if key not in ("max-request-bytes", "max-response-bytes") and
              (not key.startswith("qos.") or (not graph and version == 2))}
    limits["max-bytes-per-second"] = "67108864"
    if graph:
        limits.pop("validator", None)
        limits.update({"allowed-name-prefix": scope, "max-graph-result-bytes": "16384"})
    else:
        limits["max-parameter-bytes"] = "16384"
        if version == 2:
            limits.update({"parameter-node": node, "parameter-name": "not-applicable" if number == 9 else "value"})
        if number in (9, 12):
            limits["allowed-name-prefix"] = scope
        if number == 9 and version == 2:
            limits["list-depth"] = "10"
        if number == 12:
            limits["search-parent-namespaces"] = "false"
    return grant, limits
