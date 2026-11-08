-- Host-owned UWVM debugger views for Neovim and nvim-dap.
-- No guest evaluation, process attachment or memory grant is provided here.
local M = {}
local api = vim.api
local namespace = api.nvim_create_namespace("uwvm_debug_snapshots")
local serial = 0
local wasm_buffers = {}

local function active()
  local session = require("dap").session()
  if not session or session.config.type ~= "uwvm" then
    error("Select an active UWVM debug session")
  end
  return session
end

local function request(command, arguments, callback, stopped)
  local session = active()
  local frame = session.current_frame
  local thread = session.stopped_thread_id
  if stopped and (not frame or not thread) then error("Stop the owned Wasm instance first") end
  session:request(command, arguments, function(err, body)
    vim.schedule(function()
      if not err and (require("dap").session() ~= session or
          (stopped and (session.stopped_thread_id ~= thread or
            not session.current_frame or session.current_frame.id ~= frame.id))) then
        err = {message = "Debugger session or stop changed; reply retired"}
      end
      if err then
        if callback then callback(err) else vim.notify(err.message or tostring(err), vim.log.levels.ERROR) end
        return
      end
      callback(nil, body, session)
    end)
  end)
end

local function snapshot(title, lines, filetype, highlights)
  serial = serial + 1
  local buffer = api.nvim_create_buf(false, true)
  api.nvim_buf_set_name(buffer, "uwvm-snapshot://" .. serial .. "/" .. title)
  vim.bo[buffer].buftype = "nofile"
  vim.bo[buffer].bufhidden = "wipe"
  vim.bo[buffer].swapfile = false
  api.nvim_buf_set_lines(buffer, 0, -1, false, lines)
  vim.bo[buffer].filetype = filetype
  if highlights then
    for _, row in ipairs(highlights) do
      local group = "UwvmMemoryTag_" .. row.color:sub(2)
      api.nvim_set_hl(0, group, {fg = row.color})
      api.nvim_buf_set_extmark(buffer, namespace, row.line, 0,
        {end_row = row.line + 1, end_col = 0, hl_group = group})
    end
  end
  vim.bo[buffer].modifiable = false
  api.nvim_set_current_buf(buffer)
  return buffer
end

-- Preserve the adapter's lossless strings (including uint64 decimal values)
-- and JSON escapes. Pretty printing changes whitespace outside strings only.
local function pretty_json(value)
  local raw = vim.json.encode(value)
  local output, depth, quoted, escaped = {}, 0, false, false
  local function newline() output[#output + 1] = "\n" .. string.rep("  ", depth) end
  for i = 1, #raw do
    local ch = raw:sub(i, i)
    if quoted then
      output[#output + 1] = ch
      if escaped then escaped = false elseif ch == "\\" then escaped = true elseif ch == '"' then quoted = false end
    elseif ch == '"' then
      quoted = true; output[#output + 1] = ch
    elseif ch == "{" or ch == "[" then
      output[#output + 1] = ch; depth = depth + 1; newline()
    elseif ch == "}" or ch == "]" then
      depth = depth - 1; newline(); output[#output + 1] = ch
    elseif ch == "," then output[#output + 1] = ch; newline()
    elseif ch == ":" then output[#output + 1] = ": "
    else output[#output + 1] = ch end
  end
  return vim.split(table.concat(output), "\n", {plain = true})
end

local function finish(callback, err, body, render)
  if err then
    if callback then callback(err) else vim.notify(err.message or tostring(err), vim.log.levels.ERROR) end
    return
  end
  local ok, buffer = pcall(render, body)
  if not ok then
    if callback then callback({message = tostring(buffer)})
    else vim.notify(tostring(buffer), vim.log.levels.ERROR) end
    return
  end
  if callback then callback(nil, body, buffer) end
end

function M.wasm_breakpoints(points, callback)
  local session = active()
  local frame = session.current_frame
  local identity = wasm_buffers[api.nvim_get_current_buf()]
  assert(frame and frame.source and identity and identity.session == session and
    identity.reference == frame.source.sourceReference and identity.frame == frame.id,
    "Select the current owned Wasm instruction document")
  request("setBreakpoints", {source = frame.source, breakpoints = points}, function(err, body)
    if callback then callback(err, body)
    elseif err then vim.notify(err.message or tostring(err), vim.log.levels.ERROR)
    else
      for _, point in ipairs(body.breakpoints or {}) do
        vim.notify(point.verified and "Wasm breakpoint registered" or (point.message or "Breakpoint rejected"))
      end
    end
  end, true)
end

function M.disassemble(count, callback, instruction_offset)
  local session = active()
  local frame = session.current_frame
  count = count or 64
  assert(type(count) == "number" and count % 1 == 0 and count >= 1 and count <= 4096,
    "Select a display page of 1 through 4096 instructions")
  assert(frame and frame.instructionPointerReference, "Select an owned instruction frame")
  request("disassemble", {memoryReference = frame.instructionPointerReference,
    instructionOffset = instruction_offset or 0, instructionCount = count, resolveSymbols = true},
    function(err, body)
      finish(callback, err, body, function(value)
        assert(type(value.instructions) == "table", "Missing instruction rows")
        local lines = {"UWVM instruction snapshot; frame " .. tostring(frame.id),
          "Owned instruction reference: " .. frame.instructionPointerReference, ""}
        for _, row in ipairs(value.instructions) do
          lines[#lines + 1] = string.format("%s  %-24s %s", row.address or "",
            row.instructionBytes or "", row.instruction or "")
        end
        return snapshot("instructions", lines, "asm")
      end)
    end, true)
end

function M.host_trace(family, callback)
  family = family or "all"
  assert(family == "all" or family == "wasip1" or family == "ros1" or family == "ros2",
    "Select all, wasip1, ros1 or ros2")
  request("uwvm/hostTrace", {family = family}, function(err, body)
    finish(callback, err, body, function(value)
      return snapshot("host-trace-" .. family, pretty_json(value), "json")
    end)
  end)
end

function M.host_trace_bytes(call, segment, callback)
  assert(tostring(call):match("^%d+$") and tostring(segment):match("^%d+$"),
    "Select decimal call and segment identities")
  request("uwvm/hostTraceBytes", {call = tostring(call), segment = tostring(segment)}, function(err, body)
    finish(callback, err, body, function(value)
      return snapshot("host-trace-bytes", pretty_json(value), "json")
    end)
  end)
end

function M.export_host_trace(path, callback)
  assert(type(path) == "string" and path:sub(1, 1) == "/", "Select an absolute host output path")
  request("uwvm/exportHostTrace", {path = path}, function(err, body)
    if callback then callback(err, body)
    elseif err then vim.notify(err.message or tostring(err), vim.log.levels.ERROR)
    else vim.notify("Saved UWVM host trace: " .. path) end
  end)
end

function M.memory_tags(module, memory, offset, count, callback)
  for _, value in ipairs({module, memory, offset, count}) do
    assert(type(value) == "string" and value:match("^%d+$"), "Use decimal memory selectors")
  end
  local length = tonumber(count)
  assert(length and length <= 256, "A tag preview page contains at most 256 bytes")
  request("uwvm/memoryTags", {memoryReference = "wasm-memory:" .. module .. ":" .. memory .. ":" .. offset,
    count = length, includeBytes = true}, function(err, body)
    finish(callback, err, body, function(value)
      assert(value.status == "available" or value.status == "disabled" or value.status == "unsupported-memory",
        "Unknown tag observation status")
      assert(type(value.tags) == "table" and type(value.stopId) == "string", "Missing tag snapshot identity")
      assert(value.count == length and type(value.data) == "string", "Mismatched memory preview")
      local bytes = vim.base64.decode(value.data)
      assert(#bytes == length, "Incomplete memory bytes")
      local function hex(first, last)
        local pieces = {}
        for index = first, last do pieces[#pieces + 1] = string.format("%02x", bytes:byte(index)) end
        return table.concat(pieces, " ")
      end
      local lines = {"UWVM memory snapshot; stop " .. value.stopId,
        "Memory " .. module .. ":" .. memory .. " byte offset " .. offset .. " count " .. count,
        "Allocation tags: " .. value.status, ""}
      local colors = {}
      local remainder = 0
      for digit in offset:gmatch(".") do remainder = (remainder * 10 + tonumber(digit)) % 16 end
      local position = 1
      for index, tag in ipairs(value.tags) do
        assert(type(tag.color) == "string" and tag.color:match("^#%x%x%x%x%x%x$") and
          type(tag.hex) == "string" and tag.hex:match("^0x%x+$"), "Malformed tag palette")
        colors[#colors + 1] = {line = #lines, color = tag.color}
        local last = math.min(length, position + 15 - (index == 1 and remainder or 0))
        lines[#lines + 1] = "byte=" .. (index == 1 and offset or tostring(tag.offset)) ..
          " granule=" .. tostring(tag.offset) .. " tag=" .. tag.hex .. "  " .. hex(position, last)
        position = last + 1
      end
      if value.status == "available" then
        assert(position == length + 1, "Incomplete allocation-tag coverage")
      else
        assert(#value.tags == 0, "Disabled tagging must not synthesize tags")
        for first = 1, length, 16 do lines[#lines + 1] = hex(first, math.min(first + 15, length)) end
      end
      return snapshot("memory-tags", lines, "text", colors)
    end)
  end, true)
end

function M.attach(socket_dir, level, options)
  assert(type(socket_dir) == "string" and socket_dir:sub(1, 1) == "/", "Select the owned broker's absolute socket directory")
  level = level or "source"
  assert(level == "source" or level == "wasm" or level == "native", "Select source, wasm or native")
  local permission = (options or {}).debugPermissions
  assert(permission ~= "none", "Debugger disabled; run the generated runtime task without attaching")
  assert(permission == nil or permission == "full" or permission == "read-only" or permission == "limited", "Invalid debugger permissions")
  local config = vim.tbl_extend("force", options or {},
    {type = "uwvm", request = "attach", name = "UWVM " .. level,
      socketDir = socket_dir, stepLevel = level})
  require("dap").run(config)
end

function M.setup(options)
  options = options or {}
  assert(type(options.root) == "string" and options.root:sub(1, 1) == "/", "Set the absolute UWVM repository path")
  local environment = vim.fn.environ()
  environment.UWVM_DAP_SOURCE_SESSION = nil -- Generic DAP Source/sourceReference, not VS Code URI handling.
  local arguments = {options.root .. "/tools/debug/dap_adapter.py"}
  if options.profile then
    assert(type(options.profile) == "string" and options.profile:sub(1,1) == "/", "Select an absolute host-owned transport profile")
    arguments = {options.root .. "/tools/debug/import_ide.py", "adapter", "--profile", options.profile}
  end
  require("dap").adapters.uwvm = {type = "executable", command = options.python or "/usr/bin/python3",
    args = arguments, env = environment,
    options = {source_filetype = "wat"}}
  require("dap").listeners.after.source.uwvm_source_identity = function(session, err, _, arguments)
    if err or session.config.type ~= "uwvm" then return end
    local frame = session.current_frame
    local buffer = api.nvim_get_current_buf()
    if frame and frame.source and arguments.sourceReference == frame.source.sourceReference and
        vim.b[buffer].dap_source_buf then
      wasm_buffers[buffer] = {session = session, reference = arguments.sourceReference, frame = frame.id}
      vim.bo[buffer].modifiable = false
    end
  end
  local function retire(session)
    for buffer, identity in pairs(wasm_buffers) do
      if identity.session == session then wasm_buffers[buffer] = nil end
    end
  end
  require("dap").listeners.before.event_terminated.uwvm_source_identity = retire
  require("dap").listeners.after.attach.uwvm_source_identity = function(session)
    if session.config.type == "uwvm" then session.on_close.uwvm_source_identity = retire end
  end
  local group = api.nvim_create_augroup("UwvmSourceIdentity", {clear = true})
  api.nvim_create_autocmd("BufWipeout", {group = group, callback = function(event) wasm_buffers[event.buf] = nil end})
  local commands = {
    UwvmDiagnostics = {function() request("uwvm/diagnostics", {}, function(err, body)
      finish(nil, err, body, function(value) return snapshot("diagnostics", pretty_json(value), "json") end)
    end, false) end, {nargs = 0}},
    UwvmDump = {function(args)
      local stop, path = args.args:match("^([1-9][0-9]*) (/.+)$")
      assert(stop and #stop <= 20, "Use current STOP_ID and absolute VM-host PATH")
      request("uwvm/dump", {stopId=stop, path=path}, function(err, body)
        finish(nil, err, body, function(value) return snapshot("dump", pretty_json(value), "json") end)
      end, false)
    end, {nargs = "+"}},
    UwvmWasmBreakpoint = {function() M.wasm_breakpoints({{line = api.nvim_win_get_cursor(0)[1]}}) end, {nargs = 0}},
    UwvmMemoryTags = {function(args) assert(#args.fargs == 4, "Use MODULE MEMORY OFFSET COUNT"); M.memory_tags(unpack(args.fargs)) end, {nargs = "+"}},
    UwvmDisassemble = {function(args) M.disassemble(args.args ~= "" and tonumber(args.args) or nil) end, {nargs = "?"}},
    UwvmHostTrace = {function(args) M.host_trace(args.args ~= "" and args.args or nil) end, {nargs = "?"}},
    UwvmHostTraceBytes = {function(args) assert(#args.fargs == 2, "Use CALL SEGMENT"); M.host_trace_bytes(unpack(args.fargs)) end, {nargs = "+"}},
    UwvmExportHostTrace = {function(args) M.export_host_trace(args.args) end, {nargs = 1, complete = "file"}},
  }
  for name, item in pairs(commands) do
    api.nvim_create_user_command(name, item[1], vim.tbl_extend("force", {force = true}, item[2]))
  end
end

M.snapshot_namespace = namespace
return M
