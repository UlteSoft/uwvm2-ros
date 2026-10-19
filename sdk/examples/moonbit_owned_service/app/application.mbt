// Caller-selected real ROS2 control applications. Construct the application
// BEFORE its first open; retain it through every failure and explicit close.
// Returned codes never mean that Native work was unattempted or may be replayed.
pub(all) struct Selection {
  source_alias : String
  peer_alias : String
  availability_alias : String?
  expected_qos : Array[Bytes]
  maximum_body : Int
  history_budget : Int
}

pub struct Application {
  session : @ros2.Session
  node : @ros2.Node
  selection : Selection
  mut started : Bool
  mut closed : Bool
  mut primary : Int
  mut endpoint : @owned_service.Endpoint?
  mut description : Result[@records.Ros2Type_description_interfacesSrvGetTypeDescriptionClient, Int]
  mut parameter_types : Result[@records.Ros2Rcl_interfacesSrvGetParameterTypesServer, Int]
  mut request : Result[@owned_service.Request, Int]
  mut response : @records.Ros2Type_description_interfacesSrvGetTypeDescription_Response?
  mut endpoint_cleanup : Int
  mut session_cleanup : @ros2.CleanupReport?
  mut session_cleanup_code : Int
}

pub fn prepare(selection : Selection) -> Result[Application, Int] {
  if selection.maximum_body < 0 || selection.maximum_body > 4193626 ||
      selection.history_budget < 1 || selection.history_budget > 65536 ||
      selection.expected_qos.length() != 2 { return Err(22) }
  let session = match @ros2.new_session() { Ok(value) => value; Err(code) => return Err(code) }
  let node = match session.node("moonbit-owned-control") { Ok(value) => value; Err(code) => return Err(code) }
  // No capability or Native endpoint has been opened. All failure/code/result
  // slots and this retained carrier exist before any effect starts.
  Ok({ session, node, selection, started: false, closed: false, primary: 0,
    endpoint: None, description: Err(9), parameter_types: Err(9), request: Err(9),
    response: None, endpoint_cleanup: 0, session_cleanup: None, session_cleanup_code: 0 })
}

pub fn Application::primary(self : Application) -> Int { self.primary }
pub fn Application::fail(self : Application, code : Int) -> Int { self.remember(if code == 0 { 22 } else { code }) }
pub fn Application::endpoint(self : Application) -> @owned_service.Endpoint? { self.endpoint }
pub fn Application::endpoint_cleanup(self : Application) -> Int { self.endpoint_cleanup }
pub fn Application::session_cleanup(self : Application) -> @ros2.CleanupReport? { self.session_cleanup }
pub fn Application::session(self : Application) -> @ros2.Session { self.session }
pub fn Application::description_response(self : Application) -> @records.Ros2Type_description_interfacesSrvGetTypeDescription_Response? { self.response }

fn Application::remember(self : Application, code : Int) -> Int {
  if code != 0 && self.primary == 0 { self.primary = code }
  code
}

fn Application::open_endpoint(self : Application, role : Int) -> Result[@owned_service.Endpoint, Int] {
  if self.started || self.closed { return Err(9) }
  self.started = true
  let selected = self.selection
  let opened = if role == 3 {
    self.node.owned_client_owners(selected.source_alias, selected.peer_alias, selected.availability_alias)
  } else {
    match selected.availability_alias { Some(_) => return Err(22); None => () }
    self.node.owned_service_owners(selected.source_alias, selected.peer_alias)
  }
  let owners = match opened { Ok(value) => value; Err(code) => return Err(code) }
  let prepared = @owned_service.prepare_endpoint(owners, selected.expected_qos, selected.maximum_body, selected.history_budget)
  match prepared { Ok(value) => self.endpoint = Some(value); Err(_) => () }
  prepared
}

/// One genuine request, with the complete original default include_type_sources
/// retained unless the caller explicitly chooses another boolean. Canonical
/// encoding is validated before opening capabilities; no partial projection.
pub fn Application::start_description(
  self : Application, type_name : String, type_hash : String, include_type_sources : Bool,
  availability_wait_ns : UInt64,
) -> Int {
  if self.started || self.closed { return self.remember(9) }
  if availability_wait_ns > 60000000000UL { return self.remember(22) }
  let request = @records.Ros2Type_description_interfacesSrvGetTypeDescription_Request::new()
  request.type_name = type_name
  request.type_hash = type_hash
  request.include_type_sources = include_type_sources
  match @records.Ros2Type_description_interfacesSrvGetTypeDescription_RequestCodec::new().encode(request) {
    Ok(_) => (); Err(code) => return self.remember(code)
  }
  let endpoint = match self.open_endpoint(3) { Ok(value) => value; Err(code) => return self.remember(code) }
  self.description = @records.Ros2Type_description_interfacesSrvGetTypeDescriptionClient::new(endpoint)
  let client = match self.description { Ok(value) => value; Err(code) => return self.remember(code) }
  let code = client.activate()
  if code != 0 { return self.remember(code) }
  match self.selection.availability_alias {
    Some(_) => match client.available(availability_wait_ns) {
      Ok(true) => (); Ok(false) => return self.remember(11); Err(code) => return self.remember(code)
    }
    None => if availability_wait_ns != 0UL { return self.remember(22) }
  }
  // Store the actual preallocated Result/row returned by submit; no request is
  // replayed on a nonzero or unknown outcome. It stays accessible for abandon.
  self.request = client.submit(request)
  match self.request { Ok(_) => 0; Err(code) => self.remember(code) }
}

/// One explicitly scheduled poll. Only raw peek11 is idle; codec11 is failure.
/// Full response and metadata are retained by the typed wrapper before ACK.
pub fn Application::step_description(self : Application) -> Int {
  if self.primary != 0 || self.closed { return if self.primary != 0 { self.primary } else { 9 } }
  let client = match self.description { Ok(value) => value; Err(code) => return self.remember(code) }
  match client.take() {
    Err(code) => {
      let idle = match self.endpoint { Some(value) => value.last_take_idle(); None => false }
      if code == 11 && idle { 11 } else { self.remember(code) }
    }
    Ok(delivery) => {
      if delivery.is_control() { return self.remember(if delivery.error() == 0U { 125 } else { delivery.error().to_int() }) }
      if delivery.was_abandoned() { return self.remember(125) }
      self.response = client.value()
      match self.response { Some(_) => 0; None => self.remember(22) }
    }
  }
}

pub fn Application::abandon_description(self : Application) -> Int {
  let client = match self.description { Ok(value) => value; Err(code) => return self.remember(code) }
  let request = match self.request { Ok(value) => value; Err(code) => return self.remember(code) }
  self.remember(client.abandon(request))
}

pub fn Application::start_parameter_types(self : Application) -> Int {
  let endpoint = match self.open_endpoint(4) { Ok(value) => value; Err(code) => return self.remember(code) }
  self.parameter_types = @records.Ros2Rcl_interfacesSrvGetParameterTypesServer::new(endpoint)
  let server = match self.parameter_types { Ok(value) => value; Err(code) => return self.remember(code) }
  self.remember(server.activate())
}

/// Real caller-supplied parameter resolver runs AFTER successful full decode and
/// ACK, before one response. It supplies the complete ordered result, one entry
/// for each actual name. An error never becomes an empty successful response.
pub fn Application::step_parameter_types(
  self : Application, resolve_type : (String) -> Result[Int, Int],
) -> Int {
  if self.primary != 0 || self.closed { return if self.primary != 0 { self.primary } else { 9 } }
  let server = match self.parameter_types { Ok(value) => value; Err(code) => return self.remember(code) }
  let delivery = match server.take() {
    Ok(value) => value
    Err(code) => {
      let idle = match self.endpoint { Some(value) => value.last_take_idle(); None => false }
      return if code == 11 && idle { 11 } else { self.remember(code) }
    }
  }
  if delivery.is_control() { return self.remember(if delivery.error() == 0U { 125 } else { delivery.error().to_int() }) }
  let value = match server.value() { Some(value) => value; None => return self.remember(22) }
  let response = @records.Ros2Rcl_interfacesSrvGetParameterTypes_Response::new()
  for name in value.names {
    let type_value = match resolve_type(name) { Ok(value) => value; Err(code) => return self.remember(if code == 0 { 22 } else { code }) }
    if type_value < 0 || type_value > 9 { return self.remember(22) }
    response.types.push(type_value)
  }
  let request = match server.request(delivery.ticket()) { Ok(value) => value; Err(code) => return self.remember(code) }
  self.remember(server.respond(request, response))
}

/// Always call after start, including a failed start. Endpoint retains Native
/// once plus every raw result; Session independently closes any partial opens.
/// Nothing prints until all cleanup is complete. A cleanup failure is nonzero
/// even when business work succeeded. Exact primary and all cleanup facts remain
/// separately observable on this pre-effect application carrier.
pub fn Application::close(self : Application) -> Int {
  if self.closed { return self.exit_code() }
  self.closed = true
  match self.endpoint { Some(value) => self.endpoint_cleanup = value.close(); None => () }
  let report = self.session.shutdown()
  self.session_cleanup_code = report.code
  self.session_cleanup = Some(report)
  self.exit_code()
}

pub fn Application::exit_code(self : Application) -> Int {
  if self.primary != 0 { self.primary }
  else if self.endpoint_cleanup != 0 { self.endpoint_cleanup }
  else { self.session_cleanup_code }
}
