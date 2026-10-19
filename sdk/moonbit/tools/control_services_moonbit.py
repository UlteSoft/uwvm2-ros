"""Seven complete typed Service bindings over actual shared resolved control IR.

Called by the captured-source operator only. No schema parser, native entity,
registry, policy, compiler, network, arbitrary plugin or permission lives here.
The original root/Request/Response identities stay distinct and unchanged.
"""
import json


def emit(parent, bounded, ordered, interfaces):
    registered = [item for item in interfaces if item.registered]
    if ({item.name for item in registered} != bounded.control_registered_names() or
            sum(item.kind == 'service' for item in registered) != 7):
        raise parent.GenerationError('complete actual seven registered control Services required')
    known = {id(record) for record in ordered}
    lines = ['// Complete original service roots. Native strings are assertions, never grants.']
    facts = []
    for interface in registered:
        if interface.kind != 'service':
            continue
        if set(interface.components) != {'request', 'response'}:
            raise parent.GenerationError('complete original request/response pair required')
        request, response = interface.components['request'], interface.components['response']
        if id(request) not in known or id(response) not in known:
            raise parent.GenerationError('actual resolved complete component objects required')
        name, req, res = interface.symbol, request.symbol, response.symbol
        identity = 'None' if interface.identity is None else 'Some(' + json.dumps(interface.identity) + ')'
        facts.append({'symbol': name, 'native_type': interface.name,
                      'native_identity': interface.identity, 'codec_identity': interface.codec_identity,
                      'request': req, 'request_codec': request.codec_identity,
                      'response': res, 'response_codec': response.codec_identity})
        lines += [
            'pub fn native_type_' + name + '() -> String { ' + json.dumps(interface.name) + ' }',
            'pub fn codec_identity_' + name + '() -> String { ' + json.dumps(interface.codec_identity) + ' }',
            'pub fn native_identity_' + name + '() -> String? { ' + identity + ' }',
        ]
        for role, role_number, value, sent in (('Client', 3, res, req), ('Server', 4, req, res)):
            symbol = name + role
            lines += [
                'pub struct ' + symbol + ' {',
                '  endpoint : @owned_service.Endpoint',
                '  mut prepared : ' + value + '?',
                '  mut ready : ' + value + '?',
                '}',
                'pub fn ' + symbol + '::new(endpoint : @owned_service.Endpoint) -> Result[' + symbol + ', Int] {',
                '  if endpoint.role() != ' + str(role_number) + ' || endpoint.maximum_body() < ' + str(max(request.maximum, response.maximum)) + ' { return Err(22) }',
                '  Ok({ endpoint, prepared: None, ready: None })',
                '}',
                'pub fn ' + symbol + '::activate(self : ' + symbol + ') -> Int { self.endpoint.activate() }',
                'pub fn ' + symbol + '::inspect(self : ' + symbol + ') -> Int { self.endpoint.inspect() }',
                'pub fn ' + symbol + '::close(self : ' + symbol + ') -> Int { self.endpoint.close() }',
                'pub fn ' + symbol + '::value(self : ' + symbol + ') -> ' + value + '? { self.ready }',
                'pub fn ' + symbol + '::take(self : ' + symbol + ') -> Result[@owned_service.Delivery, Int] {',
                '  self.prepared = None', '  self.ready = None',
                '  let result = self.endpoint.take(fn(body) {',
                '    match ' + value + 'Codec::new().decode(body) {',
                '      Ok(value) => { self.prepared = Some(value); 0 }',
                '      Err(code) => code', '    }', '  })',
                '  // All decode/Some/Result allocation precedes commit inside Endpoint.take.',
                '  // After ACK only existing references move; abandoned never exposes success.',
                '  match result {',
                '    Ok(delivery) => if !delivery.is_control() && !delivery.was_abandoned() { self.ready = self.prepared }',
                '    Err(_) => ()', '  }',
                '  result', '}',
            ]
            if role == 'Client':
                lines += [
                    'pub fn ' + symbol + '::submit(self : ' + symbol + ', value : ' + sent + ') -> Result[@owned_service.Request, Int] {',
                    '  let bytes = match ' + sent + 'Codec::new().encode(value) { Ok(value) => value; Err(code) => return Err(code) }',
                    '  self.endpoint.submit(bytes)', '}',
                    'pub fn ' + symbol + '::abandon(self : ' + symbol + ', request : @owned_service.Request) -> Int { self.endpoint.abandon(request) }',
                    'pub fn ' + symbol + '::available(self : ' + symbol + ', wait_ns : UInt64) -> Result[Bool, Int] { self.endpoint.available(wait_ns) }',
                ]
            else:
                lines += [
                    'pub fn ' + symbol + '::request(self : ' + symbol + ', ticket : UInt64) -> Result[@owned_service.Request, Int] { self.endpoint.request(ticket) }',
                    'pub fn ' + symbol + '::respond(self : ' + symbol + ', request : @owned_service.Request, value : ' + sent + ') -> Int {',
                    '  let bytes = match ' + sent + 'Codec::new().encode(value) { Ok(value) => value; Err(code) => return code }',
                    '  self.endpoint.respond(request, bytes)', '}',
                ]
    return '\n'.join(lines) + '\n', facts


def emit_default_contracts(parent, bounded, ordered):
    """Future Guest fixture for ALL complete records; no raw or native call.

    A successful self roundtrip is deliberately only a local codec contract.
    Independent canonical C/C++ vectors and true Native trials remain required.
    """
    if len(ordered) != 32 or any(record.profile != bounded.CONTROL_PROFILE for record in ordered):
        raise parent.GenerationError('complete original32 control fixture required')
    lines = ['// Future local Guest value fixture. Not Native interoperability.',
             'pub fn complete_control_default_contracts() -> Int {']
    for record in ordered:
        lines += ['  {', '    let codec = ' + record.symbol + 'Codec::new()',
                  '    let original = ' + record.symbol + '::new()',
                  '    let bytes = match codec.encode(original) { Ok(value) => value; Err(code) => return code }',
                  '    let decoded = match codec.decode(bytes) { Ok(value) => value; Err(code) => return code }',
                  '    let again = match codec.encode(decoded) { Ok(value) => value; Err(code) => return code }',
                  '    if bytes != again { return 22 }',
                  '    match codec.decode(bytes + b"\\x00") { Ok(_) => return 22; Err(_) => () }',
                  '  }']
    lines += ['  0', '}']
    return '\n'.join(lines) + '\n'
