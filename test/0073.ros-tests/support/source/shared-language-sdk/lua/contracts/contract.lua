-- Run this exact program inside the real embedded Lua/WASI interpreter.
local wire=require("roswasm.wire")
local records=require("bounded_records")
local ros=require("roswasm")
local raw=require("roswasm_native")
local checks=0
local function check(ok) checks=checks+1;assert(ok,"Lua contract assertion "..checks) end
local function refuses(fn) local ok=pcall(fn);check(not ok) end
local scalar=records.Ros2Managed_fixtureMsgScalars
local value=scalar.new{flag=true,u8=255,i8=-128,u16=65535,i16=-32768,
  u32=4294967295,i32=-2147483648,u64=wire.uint64("ffffffffffffffff"),
  i64=math.mininteger,f32=-0.0,f64=1.25,text="π"}
local hex="01ff80ffff0080ffffffff00000080ffffffffffffffff0000000000000080"..
          "00000080000000000000f43f02000000cf80"
local golden=hex:gsub("..",function(byte) return string.char(tonumber(byte,16)) end)
check(scalar.encode(value)==golden)
local decoded=scalar.decode(golden)
check(decoded.u64==wire.uint64("ffffffffffffffff") and decoded.i64==math.mininteger)
check(decoded.text=="π" and 1/decoded.f32==-math.huge)
check(wire.uint64_hex(decoded.u64)=="ffffffffffffffff")
refuses(function() scalar.decode(string.char(2)..golden:sub(2)) end)
refuses(function() scalar.decode(golden.."\0") end)
refuses(function() scalar.decode(golden:sub(1,-2)) end)
value.text="\237\160\128";refuses(function() scalar.encode(value) end)
value.text="a\0b";refuses(function() scalar.encode(value) end)
value.text="π";value.u64=18446744073709551615.0;refuses(function() scalar.encode(value) end)
value.u64=wire.uint64("ffffffffffffffff");value.u8=256;refuses(function() scalar.encode(value) end)
value.u8=255;value.f32=1e100;refuses(function() scalar.encode(value) end)
local outer=records.Ros2Managed_fixtureMsgOuter
local composite=outer.new()
check(composite.fixed[1]~=composite.fixed[2])
check(composite.rotation.w==1 and composite.rotations[2].w==1)
composite.fixed[2].scalar.u64=wire.uint64("ffffffffffffffff")
local copy=outer.decode(outer.encode(composite))
check(copy.fixed[2].scalar.u64==wire.uint64("ffffffffffffffff"))
copy.fixed[2].scalar.u64=wire.uint64("0000000000000000")
check(composite.fixed[2].scalar.u64==wire.uint64("ffffffffffffffff"))
composite.items={records.Ros2Managed_fixtureMsgInner.new(),records.Ros2Managed_fixtureMsgInner.new(),records.Ros2Managed_fixtureMsgInner.new()}
refuses(function() outer.encode(composite) end)
local session=ros.session(4)
local capability,code=session:open("explicitly-ungranted-lua")
check(capability==nil and code==13)
local report,status=session:close()
check(status==0 and report.success and #report.outcomes==1 and not report.outcomes[1].attempted)
local zero=string.rep("\0",8)
local abi=raw.prepare("abi_version",zero,"",0,zero,zero)
check(raw.invoke(abi)==0 and abi:value()=="\1"..string.rep("\0",7))
refuses(function() raw.invoke(abi) end)
refuses(function() raw.prepare("debug_update_policy",zero,"",0,zero,zero) end)
refuses(function() raw.prepare("event_ready",zero,"",31,zero,zero) end)
refuses(function() raw.prepare("capability_open",zero,"",0,zero,zero) end)
io.write("PASS_LUA_REAL_GUEST_CHECKS=",checks,"\n")
