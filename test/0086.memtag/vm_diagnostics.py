"""Assertions for the product's human fault report; never accept an OS signal.

This test helper reads stderr presentation only. It is not a deployment event
API; production hosts use the authenticated diagnostic channel/snapshot API.
"""
import re


def human_fault(text):
    headers = re.findall(r"^uwvm: \[crash\] ([a-z-]+)$", text, re.MULTILINE)
    reasons = re.findall(r"^  Reason: ([^\r\n]+)$", text, re.MULTILINE)
    statuses = re.findall(r"^  Exit status: ([0-9]+)$", text, re.MULTILINE)
    if len(headers) != 1 or len(reasons) != 1 or len(statuses) != 1:
        return None
    return {"category": headers[0], "reason": reasons[0], "exit_status": int(statuses[0])}


def fault_matches(exit_code, text, expected_status, reason=""):
    fault = human_fault(text)
    category = {120: "unreachable", 121: "execution", 122: "memory-protection", 123: "runtime-internal"}.get(expected_status)
    return (exit_code == expected_status and fault is not None and
            fault["exit_status"] == expected_status and fault["category"] == category and
            (not reason or reason.lower() in fault["reason"].lower()))
