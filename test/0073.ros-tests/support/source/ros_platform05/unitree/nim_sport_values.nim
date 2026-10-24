## Genuine full-codec Nim/C++ cross-checks and command-output oracle.
## Runs only as a finite Linux-owner Wasm Guest with no additional permissions.
import unitree_sport
import unitree_api_records
import roswasm
import std/[json, os, strutils]

proc packetLookup(family: uint32; name: cstring; length: csize_t;
    id, shape: ptr uint32): uint32 {.importc: "uwvm_unitree_packet_lookup", cdecl.}
proc packetPrepare(family, id: uint32; input: pointer; length: csize_t;
    x, y, z: cfloat; number: int32; flag: uint32; points: ptr cfloat;
    count: csize_t; output: pointer; capacity: csize_t;
    written: ptr csize_t): uint32 {.importc: "uwvm_unitree_packet_prepare", cdecl.}

template refuses(expectedStatus: uint32; body: untyped) =
  block:
    var observed = false
    try:
      body
    except RosError as error:
      doAssert error.code == expectedStatus
      observed = true
    doAssert observed

proc original(): Request =
  result = initRos2Unitree_apiMsgRequest()
  result.header.identity.id = 9007199254740993'i64
  result.header.identity.api_id = -77
  result.header.lease.id = -9007199254740993'i64
  result.header.policy.priority = -101
  result.parameter = "keep"
  result.binary = @[0'u8, 255'u8, 1'u8, 128'u8]

proc path(): array[30, PathPoint] =
  for i in 0..<30:
    result[i] = PathPoint(timeFromStart: float32(i), x: 0, y: -0.0'f32,
      yaw: 0, vx: 0, vy: 0, vyaw: 0)

proc selected(shape: ArgumentShape): Arguments =
  case shape
  of NoValues: argumentsNone()
  of XYZValues: argumentsXYZ(1.25, -0.0'f32, 0.5)
  of IntegerValue: argumentsInteger(low(int32))
  of BooleanValue: argumentsBoolean(true)
  of HeightValue: argumentsHeight(-0.125)
  of PathValues: argumentsTrajectory(path())
  of PositionValues: argumentsPosition(1.25, -0.0'f32, 0.5)

proc numeric(value: JsonNode): float64 =
  if value.kind == JInt: float64(value.getBiggestInt()) else: value.getFloat()

proc sameJson(a, b: JsonNode): bool =
  if a.kind in {JInt, JFloat} and b.kind in {JInt, JFloat}:
    return numeric(a) == numeric(b)
  if a.kind != b.kind: return false
  case a.kind
  of JObject:
    if a.len != b.len: return false
    for key, value in a:
      if not b.hasKey(key) or not sameJson(value, b[key]): return false
    true
  of JArray:
    if a.len != b.len: return false
    for i in 0..<a.len:
      if not sameJson(a[i], b[i]): return false
    true
  else: a == b

proc reference(family: Family; operation: Operation): Request =
  let input = encode(original())
  var output: array[6181, byte]
  var written: csize_t
  var flat: array[210, cfloat]
  for i, point in path():
    flat[7*i] = point.timeFromStart
    flat[7*i+2] = point.y
  var x, y, z: cfloat
  var number: int32
  var flag: uint32
  var count: csize_t
  var points: ptr cfloat
  case argumentShape(operation)
  of XYZValues, PositionValues: x = 1.25; y = -0.0; z = 0.5
  of IntegerValue: number = low(int32)
  of BooleanValue: flag = 1
  of HeightValue: x = -0.125
  of PathValues: points = addr flat[0]; count = 30
  of NoValues: discard
  let id = uint32(operationID(operation))
  doAssert packetPrepare(uint32(ord(family)+1), id, unsafeAddr input[0],
    csize_t(input.len), x, y, z, number, flag, points, count,
    addr output[0], csize_t(output.len), addr written) == 0
  doAssert written <= csize_t(output.len)
  var complete = @output
  complete.setLen(int(written))
  decodeRos2Unitree_apiMsgRequest(complete)

proc equivalent(actual, expected: Request; operation: Operation) =
  var a = actual
  var b = expected
  if argumentShape(operation) == NoValues:
    doAssert a.parameter == b.parameter
  else:
    doAssert sameJson(parseJson(a.parameter), parseJson(b.parameter))
    if argumentShape(operation) in {XYZValues, PositionValues, PathValues}:
      doAssert "-0" in a.parameter
  a.parameter = ""
  b.parameter = ""
  doAssert encode(a) == encode(b)

proc readExact(count: int): seq[byte] =
  result = newSeq[byte](count)
  if count > 0: doAssert stdin.readBuffer(addr result[0], count) == count

proc readFrame(): Request =
  let prefix = readExact(4)
  var length: uint32
  for i, b in prefix: length = length or (uint32(b) shl (8*i))
  doAssert length >= 37 and length <= 6181
  decodeRos2Unitree_apiMsgRequest(readExact(int(length)))

proc emitFrame(request: Request) =
  let bytes = encode(request)
  var prefix: array[4, byte]
  for i in 0..<4: prefix[i] = byte((uint32(bytes.len) shr (8*i)) and 255)
  doAssert stdout.writeBuffer(addr prefix[0], 4) == 4
  doAssert stdout.writeBuffer(unsafeAddr bytes[0], bytes.len) == bytes.len
  flushFile(stdout)

proc values() =
  var methods = 0
  for family in [Go2, B2]:
    doAssert operationNames(family).len == (if family == Go2: 39 else: 21)
    for name in operationNames(family):
      let operation = operationByName(family, name)
      var id, shape: uint32
      doAssert packetLookup(uint32(ord(family)+1), name.cstring, csize_t(name.len),
        addr id, addr shape) == 0
      doAssert id == uint32(operationID(operation))
      doAssert shape == uint32(ord(argumentShape(operation)))
      let source = original()
      var prepared = prepare(source, operation, selected(argumentShape(operation)))
      equivalent(prepared, reference(family, operation), operation)
      prepared.binary[0] = 42
      doAssert source.binary[0] == 0
      inc methods
  let move = operationByName(Go2, "Move")
  let request = original()
  refuses(22): discard prepare(request, move, argumentsNone())
  refuses(22): discard prepare(request, move, argumentsXYZ(NaN.float32, 0, 0))
  refuses(22): discard operationByName(B2, "Heart")
  refuses(22): discard argumentsTrajectory(newSeq[PathPoint](29))
  var large = request
  large.parameter = repeat("x", 4096)
  large.binary = newSeq[uint8](2048)
  large.header.policy.noreply = true
  let kept = prepare(large, operationByName(Go2, "AutoRecoveryGet"), argumentsNone())
  doAssert encode(kept).len == 6181 and kept.header.policy.noreply
  large.parameter.add('x')
  refuses(22): discard prepare(large, move, argumentsXYZ(0, 0, 0))
  refuses(22): discard decodeRos2Unitree_apiMsgRequest(@[byte(0)])
  var pending = newPending(Go2)
  let shared = pending
  var live = prepare(request, move, argumentsXYZ(0, 0, 0))
  for id in 1'i64..16'i64:
    live.header.identity.id = id
    register(pending, live, 10, 100)
  live.header.identity.id = 17
  refuses(11): register(pending, live, 10, 100)
  doAssert nextIdentity(pending) == 17
  forget(pending, 1, 1008)
  live.header.identity.id = 1
  refuses(17): register(pending, live, 10, 100)
  live.header.identity.id = 17
  refuses(22): register(pending, live, 9, 100)
  register(pending, live, 11, 100)
  var response = initRos2Unitree_apiMsgResponse()
  response.header.identity.id = 17
  response.header.identity.api_id = 1008
  response.header.status.code = low(int32)
  response.data = "native error stays exact"
  response.binary = @[-128'i8, -1'i8, 0'i8, 127'i8]
  checkResponse(pending, response, 12)
  var wrong = response
  wrong.header.identity.api_id = 2055
  refuses(9): checkResponse(pending, wrong, 12)
  var accepted = accept(pending, response, 13)
  doAssert encode(accepted) == encode(response)
  accepted.binary[0] = 0
  doAssert response.binary[0] == -128
  refuses(9): checkResponse(pending, response, 13)
  live.header.identity.id = 18
  refuses(22): register(pending, live, 12, 100)
  register(pending, live, 13, 100)
  doAssert expire(pending, 18, 1008, 14) == 11
  refuses(22): discard expire(pending, 18, 1008, 13)
  doAssert expire(pending, 18, 1008, 100) == 110
  live.header.identity.id = 18
  refuses(17): register(pending, live, 100, 200)
  close(pending)
  close(shared)
  refuses(9): discard nextIdentity(shared)
  refuses(9): register(shared, live, 100, 200)
  let exhausted = newPending(B2, high(int64))
  refuses(75): discard nextIdentity(exhausted)
  close(exhausted)
  var query = initRos2Unitree_apiMsgResponse()
  query.header.identity.api_id = 2055
  query.data = "{\"data\":true,\"extra\":[\"[\\\"\",null]}"
  doAssert autoRecoveryValue(query)
  query.data = "{\"data\":false}"
  doAssert not autoRecoveryValue(query)
  for text in ["{}", "{\"data\":1}", "{\"data\":\"true\"}", "true", "{\"data\":true} trailing",
      "{\"data\":true,\"extra\":" & repeat("[", 17) & "0" & repeat("]", 17) & "}"]:
    query.data = text
    refuses(22): discard autoRecoveryValue(query)
  query.header.status.code = low(int32)
  var nativeObserved = false
  try: discard autoRecoveryValue(query)
  except NativeStatusError as error:
    doAssert error.nativeCode == low(int32)
    nativeObserved = true
  doAssert nativeObserved
  let invalid = openConversation(Go2, "same", "same")
  doAssert invalid.owner.isNil and invalid.report.primary == 22
  doAssert invalid.report.subscriptionClose == 0 and invalid.report.publisherClose == 0
  echo "UNITREE_NIM_VALUES_OK operations=", methods, " fixed_slots=16"

let argv = commandLineParams()
if argv.len == 0:
  values()
elif argv == @["emit-request"]:
  emitFrame(original())
elif argv == @["list"]:
  for family in [Go2, B2]:
    for name in operationNames(family):
      echo (if family == Go2: "go2" else: "b2"), " ", name, " ",
        ord(argumentShape(operationByName(family, name)))
elif argv == @["check-commands"]:
  for family in [Go2, B2]:
    for name in operationNames(family):
      let operation = operationByName(family, name)
      equivalent(readFrame(), reference(family, operation), operation)
  doAssert stdin.endOfFile()
  echo "UNITREE_NIM_COMMANDS_OK operations=60 complete_records=1"
else:
  quit(22)
