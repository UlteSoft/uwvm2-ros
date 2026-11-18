-- Driven by run_scene_neovim_live.py. The adapter, broker, nvim-dap, and VM are real.
vim.opt.runtimepath:append(vim.env.UWVM_NVIM_DAP)
local dap = require("dap")
local uwvm = dofile(vim.env.UWVM_NVIM_ROOT .. "/tools/debug/neovim/uwvm.lua")
uwvm.setup({root = vim.env.UWVM_NVIM_ROOT})
uwvm.attach(vim.env.UWVM_NVIM_SOCKET, "wasm", {stopOnEntry = true})
assert(vim.wait(15000, function()
  local s = dap.session()
  return s and s.initialized and s.current_frame and s.stopped_thread_id
end), "No real paused DAP stack frame")
local selections = {"authority", "resources"}
if vim.env.UWVM_NVIM_AUDIT == "1" then table.insert(selections, "audit") end
local pages = {}
for _, selection in ipairs(selections) do
  vim.cmd("UwvmScene " .. selection)
  assert(vim.wait(5000, function()
    return vim.api.nvim_buf_get_name(0):find("/scene-" .. selection, 1, true)
  end), "No scene snapshot for " .. selection)
  assert(not vim.bo.modifiable and vim.bo.buftype == "nofile")
  local page = vim.json.decode(table.concat(vim.api.nvim_buf_get_lines(0, 0, -1, false), "\n"))
  assert(page.selection == selection and page.scope == "process-owned" and page.available)
  assert(page.payloadsRead == false and page.nativeOperationsIssued == false)
  assert(type(page.stopId) == "string" and page.stopId:match("^[1-9][0-9]*$"))
  local row = assert(page.rows[1])
  if selection == "authority" then assert(row.alias.text == "signal" and row.kind == "grant")
  elseif selection == "resources" then assert(row.alias.text == "signal" and row.kind == "resource")
  else assert(row.configured ~= nil and type(row.admitted) == "string") end
  pages[selection] = page
end
assert(vim.fn.writefile({vim.json.encode(pages)}, vim.env.UWVM_NVIM_CAPTURE) == 0)
dap.disconnect({terminateDebuggee = false})
assert(vim.wait(5000, function() return not dap.session() end))
