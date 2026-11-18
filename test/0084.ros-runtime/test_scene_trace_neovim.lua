-- Run with nvim --headless -u NONE -l from the repository root.
-- The plugin and buffer are real; the callback is a deterministic DAP fixture.
local frames = {id=1}
local session = {config={type="uwvm"}, current_frame=frames, stopped_thread_id=1}
function session:request(command, args, reply)
  assert(command == "uwvm/hostTrace" and args.family == "rosv2")
  reply(nil, {stopId="12", calls={{id="18446744073709551615", family="rosv2",
        namespace="uwvm_ros_v2", ["function"]="abi", result="2", segments="0"}}})
end
package.loaded.dap = {session=function() return session end}
local plugin = dofile("tools/debug/neovim/uwvm.lua")
local finished = false
plugin.host_trace("rosv2", function(err, body, buffer)
  assert(not err and body.stopId == "12" and vim.bo[buffer].modifiable == false)
  local shown = table.concat(vim.api.nvim_buf_get_lines(buffer, 0, -1, false), "\n")
  assert(shown:find("18446744073709551615", 1, true) and shown:find("uwvm_ros_v2", 1, true))
  finished = true
end)
assert(vim.wait(1000, function() return finished end))
assert(not pcall(plugin.host_trace, "unknown"))
print("PASS immutable ROSv2 Neovim buffer and closed family selector")
