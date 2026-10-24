--- Genuine complete-value Lua/C++ fixture; never a Host-Lua acceptance substitute.
--- Requires actual same-Wasm ROS/packet/JSON modules, real Lua5.4.8/EH and SDK.
local U = require("unitree_sport")
local function refused(code, action)
    local ok, value = pcall(action)
    assert(not ok and U.isError(value) and value.code == code)
end
assert(U.queryAvailable(), "full Unitree query fixture requires the actual JSON target")
local function original()
    local value = U.Request.new()
    value.header.identity.id = 9007199254740993
    value.header.identity.api_id = -77
    value.header.lease.id = -9007199254740993
    value.header.policy.priority = -101
    value.parameter, value.binary = "keep", {0,255,1,128}
    return value
end
local function fill(kind)
    local value = U.codec(kind).new()
    if kind == "RequestIdentity" then value.id=9007199254740993;value.api_id=-77
    elseif kind == "RequestLease" then value.id=-9007199254740993
    elseif kind == "RequestPolicy" then value.priority=-2147483648;value.noreply=true
    elseif kind == "RequestHeader" then value.identity=fill("RequestIdentity");value.lease=fill("RequestLease");value.policy=fill("RequestPolicy")
    elseif kind == "Request" then value=original()
    elseif kind == "ResponseStatus" then value.code=-2147483648
    elseif kind == "ResponseHeader" then value.identity=fill("RequestIdentity");value.status=fill("ResponseStatus")
    else value.header=fill("ResponseHeader");value.data="original status/data";value.binary={-128,-1,0,127}
    end
    return value
end
local kinds=U.recordKinds();assert(#kinds==8)
for _,kind in ipairs(kinds) do
    local codec=U.codec(kind);local source=fill(kind);local bytes=codec.encode(source)
    assert(codec.encode(codec.decode(bytes))==bytes)
    assert(codec.native_identity_qualified==false)
    refused(22,function()codec.decode(bytes:sub(1,#bytes-1))end)
    refused(22,function()codec.decode(bytes.."\0")end)
    source.unselected=true
    refused(22,function()codec.encode(source)end)
end
local source=original()
local expected=string.pack("<i8i8i8i4I1s4I4",9007199254740993,-77,-9007199254740993,-101,0,"keep",4)..string.char(0,255,1,128)
assert(U.Request.encode(source)==expected)
local response=U.Response.new()
response.header.identity.id=9007199254740993;response.header.identity.api_id=2055
response.header.status.code=-2147483648;response.data="kept";response.binary={-128,-1,0,127}
local responseWire=string.pack("<i8i8i4s4I4",9007199254740993,2055,-2147483648,"kept",4)..string.char(128,255,0,127)
assert(U.Response.encode(response)==responseWire)
assert(U.Response.decode(responseWire).binary[1]==-128)
refused(22,function()U.Request.new(source)end)
local invalid=original();invalid.header.identity.id=9007199254740993.0
refused(22,function()U.Request.encode(invalid)end)
invalid=original();invalid.binary={[2]=7}
refused(22,function()U.Request.encode(invalid)end)
invalid=original();invalid.header.policy.noreply=1
refused(22,function()U.Request.encode(invalid)end)
invalid=original();invalid.parameter=string.rep("x",4097)
refused(22,function()U.Request.encode(invalid)end)
invalid=original();invalid.parameter="bad\0text"
refused(22,function()U.Request.encode(invalid)end)
invalid=setmetatable(original(),{})
refused(22,function()U.Request.encode(invalid)end)
local function points()
    local out={}
    for i=1,30 do out[i]={timeFromStart=i-1,x=0,y=-0.0,yaw=0,vx=0,vy=0,vyaw=0}end
    return out
end
local function args(shape)
    if shape==0 then return {} end
    if shape==1 or shape==6 then return {x=1.25,y=-0.0,z=0.5}end
    if shape==2 then return {number=-2147483648}end
    if shape==3 then return {flag=true}end
    if shape==4 then return {x=-0.125}end
    return points()
end
local methods=0
for _,family in ipairs({"go2","b2"})do
    local names=U.operationNames(family);assert(#names==(family=="go2" and 39 or 21))
    for _,name in ipairs(names)do
        local op=U.operation(family,name);local shape=U.argumentShape(op);local parameters=args(shape)
        U.validateArguments(op,parameters)
        local request=original();local before=U.Request.encode(request)
        local prepared=U.prepare(request,op,parameters)
        assert(prepared.header.identity.api_id==U.operationID(op))
        assert(prepared.header.identity.id==request.header.identity.id)
        assert(prepared.header.lease.id==request.header.lease.id)
        assert(prepared.header.policy.priority==-101 and not prepared.header.policy.noreply)
        assert(#prepared.binary==4 and prepared.binary[2]==255 and prepared.binary[4]==128)
        if shape==0 then assert(prepared.parameter=="keep")end
        if shape==1 or shape==5 or shape==6 then assert(prepared.parameter:find("-0",1,true))end
        prepared.binary[1]=42;prepared.parameter="changed"
        assert(U.Request.encode(request)==before)
        methods=methods+1
    end
end
assert(methods==60)
local move=U.operation("go2","Move")
refused(22,function()U.validateArguments(move,{x=math.huge,y=0,z=0})end)
refused(22,function()U.validateArguments(move,{x=1e300,y=0,z=0})end)
refused(22,function()U.validateArguments(move,{})end)
refused(22,function()U.validateArguments(U.operation("go2","Damp"),{x=1})end)
refused(22,function()U.operation("b2","Heart")end)
refused(22,function()U.operationID({})end)
local t10,t11,t12,t13,t14,t100,t200=U.ticks("10"),U.ticks("11"),U.ticks("12"),U.ticks("13"),U.ticks("14"),U.ticks("100"),U.ticks("200")
assert(U.ticks("18446744073709551615")==string.rep("\255",8))
refused(22,function()U.ticks("18446744073709551616")end)
assert(U.compareTicks(U.ticks("9223372036854775808"),U.ticks("9223372036854775807"))==1)
local pending=U.newPending("go2");local alias=pending
local copied=setmetatable({},getmetatable(pending))
refused(9,function()copied:nextIdentity()end)
local request=U.prepare(original(),move,{x=0,y=0,z=0})
for id=1,16 do request.header.identity.id=id;pending:register(request,t10,t100)end
request.header.identity.id=17
refused(11,function()pending:register(request,t10,t100)end)
assert(pending:nextIdentity()==17)
pending:forget(1,1008);request.header.identity.id=1
refused(17,function()pending:register(request,t10,t100)end)
request.header.identity.id=17
refused(22,function()pending:register(request,U.ticks("9"),t100)end)
pending:register(request,t11,t100)
response.header.identity.id=17;response.header.identity.api_id=1008
pending:checkResponse(response,t12)
response.header.identity.api_id=2055
refused(9,function()pending:accept(response,t12)end)
response.header.identity.api_id=1008
local accepted=pending:accept(response,t13)
assert(accepted.header.status.code==-2147483648 and accepted.binary[1]==-128)
accepted.binary[1]=0;assert(response.binary[1]==-128)
refused(9,function()pending:checkResponse(response,t13)end)
request.header.identity.id=18
refused(22,function()pending:register(request,t12,t100)end)
pending:register(request,t13,t100)
assert(pending:expire(18,1008,t14)==11)
refused(22,function()pending:expire(18,1008,t13)end)
assert(pending:expire(18,1008,t100)==110)
refused(17,function()pending:register(request,t100,t200)end)
assert(pending:close()==0 and alias:close()==0)
refused(9,function()alias:nextIdentity()end)
local exhausted=U.newPending("b2",math.maxinteger)
refused(75,function()exhausted:nextIdentity()end)
assert(exhausted:close()==0)
response.header.identity.api_id=2055;response.header.status.code=0;response.data='{"data":true}'
local query=U.autoRecoveryValue(response);assert(query.has_value and query.value)
response.data='{"data":false}'
query=U.autoRecoveryValue(response);assert(query.has_value and query.value==false)
for _,text in ipairs({'{}','{"data":1}','{"data":"true"}','{"data":true} trailing',
    '{"data":true,"extra":'..string.rep('[',17)..'0'..string.rep(']',17)..'}'})do
    response.data=text;refused(22,function()U.autoRecoveryValue(response)end)
end
response.header.status.code=-2147483648
query=U.autoRecoveryValue(response);assert(not query.has_value and query.value==nil and query.native_code==-2147483648)
refused(22,function()U.openConversation("go2","same","same")end)
io.stdout:write("UNITREE_LUA_VALUES_OK records=8 operations=60 fixed_slots=16\n")
return 0
