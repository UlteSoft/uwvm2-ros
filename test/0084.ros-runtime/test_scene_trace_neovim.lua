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

local selected = session
local dap = package.loaded.dap
dap.session = function() return selected end
dap.adapters = {}
dap.listeners = {after = {source = {}, attach = {}}, before = {event_terminated = {}}}
plugin.setup({root = vim.fn.getcwd()})
local stop = "18446744073709551615"
local mode, used = "normal", {}
function session:request(command, args, reply)
  assert(command == "uwvm/sceneState" and args.count == 8)
  assert(args.selection == "authority" or args.selection == "resources" or args.selection == "audit")
  used[#used+1] = vim.deepcopy(args)
  if mode == "denied" then reply({message = "Permission denied"}); return end
  local rows, total = {}, args.selection == "resources" and 9 or 1
  for _ = args.start + 1, math.min(args.start + args.count, total) do
    rows[#rows+1] = args.selection == "audit"
      and {configured=true,admitted=stop,limited="2",busy="0",clock_failed="0",sink_failed="0"}
      or {kind=args.selection}
  end
  local body = {selection=args.selection, scope="process-owned", available=true, start=args.start, rows=rows,
    stopId=mode == "wrong-stop" and "4" or stop, total=total, more=args.start+#rows<total,
    next=args.start+#rows, payloadsRead=false, nativeOperationsIssued=false}
  if mode == "retire" then self.current_frame = {id=12} end
  reply(nil, body)
end

local function shown()
  return table.concat(vim.api.nvim_buf_get_lines(0, 0, -1, false), "\n")
end
for _, name in ipairs({"authority", "resources", "audit"}) do
  vim.cmd("UwvmScene " .. name)
  assert(vim.wait(1000, function() return vim.api.nvim_buf_get_name(0):find("/scene-" .. name, 1, true) end))
  assert(not vim.bo.modifiable and shown():find(stop, 1, true))
  if name == "resources" then
    vim.cmd("UwvmSceneNext")
    assert(vim.wait(1000, function() return shown():find('"start": 8', 1, true) end))
    assert(used[#used].ifStop == stop)
    assert(not pcall(plugin.scene_next))
  elseif name == "audit" then
    for _, key in ipairs({"configured","admitted","limited","busy","clock_failed","sink_failed"}) do
      assert(shown():find(key,1,true))
    end
  end
end
local count = #used
assert(not pcall(plugin.scene_state, "enable") and #used == count)
local function done(call, wanted)
  local complete = false
  call(function(err, _, buffer)
    assert((err and err.message:find(wanted,1,true)) or (not wanted and buffer))
    complete = true
  end)
  assert(vim.wait(1000, function() return complete end))
end
done(function(cb) plugin.scene_state("resources", cb) end)
local original = vim.api.nvim_get_current_buf()
selected = {config=session.config, current_frame=session.current_frame, stopped_thread_id=1}
assert(not pcall(plugin.scene_next) and #used == count+1)
selected = session
for _, entry in ipairs({{"denied","Permission denied"},{"wrong-stop","Scene stop changed"},
                        {"retire","Debugger session or stop changed"}}) do
  mode = entry[1]
  done(plugin.scene_next, entry[2])
  assert(vim.api.nvim_get_current_buf() == original)
end
print("PASS scene commands, page continuation, exact stop, read-only buffers, denial and retired session")

-- A resumed stop may reuse the integer frame ID. Also exercise an older
-- selection that completes after a newer selection on the same stop.
local pending = {}
function session:request(_, args, reply)
  pending[#pending+1] = function()
    reply(nil, {selection=args.selection,scope="process-owned",available=true,start=0,rows={},
      stopId="3",total=0,more=false,next=0,payloadsRead=false,nativeOperationsIssued=false})
  end
end
local reused_frame_retired = false
plugin.scene_state("authority", function(err)
  reused_frame_retired = err and err.message:find("reply retired",1,true)
end)
session.current_frame = {id=session.current_frame.id}
pending[1]()
assert(vim.wait(1000, function() return reused_frame_retired end))
assert(vim.api.nvim_get_current_buf() == original)
local retired, newest = false, false
plugin.scene_state("authority", function(err)
  retired = err and err.message:find("reply retired",1,true)
end)
plugin.scene_state("audit", function(err, _, buffer)
  assert(not err and buffer)
  newest = true
end)
pending[3](); pending[2]()
assert(vim.wait(1000, function() return newest and retired end))
assert(vim.api.nvim_buf_get_name(0):find("/scene-audit",1,true))
print("PASS Neovim overlapping scene selections and reused frame IDs")
