-- @file cgroup_gate.lua
-- @brief Wait for a trusted host to verify this test container's actual limits.
-- @details Xmake's file abstraction can report empty procfs/cgroupfs contents.
-- The host controller reads the live container PID's kernel cgroup and writes
-- a regular JSON receipt bound to a fresh launch nonce. No Python or shell is
-- added to the Xmake-only producer. The receipt is test infrastructure, not
-- application authority or an uwvm2-ros permission grant.

-- @brief Admit compilation after the matching actual-limit receipt exists.
-- @param filename Regular receipt file in this job's owned writable directory.
-- @param nonce Fresh launch identifier passed by the trusted host controller.
-- @return Verified receipt, preserving actual memory and swap limit values.
function verify(filename, nonce)
    import("core.base.json")
    assert(type(nonce) == "string" and #nonce == 32, "A fresh host launch nonce is required")
    for _ = 1, 300 do
        if os.isfile(filename) then
            local receipt = json.loadfile(filename)
            assert(receipt.nonce == nonce, "The cgroup receipt belongs to another launch")
            assert(receipt.status == "verified", "The host did not verify actual cgroup limits")
            assert(receipt.memory_max == "68719476736", "Actual memory.max must be 64 GiB")
            assert(receipt.memory_swap_max == "0", "Actual memory.swap.max must be zero")
            assert(type(receipt.pid) == "number" and receipt.pid > 0, "A live container PID is required")
            return receipt
        end
        os.sleep(100)
    end
    raise("Timed out waiting for actual cgroup verification; compilation was not started")
end
