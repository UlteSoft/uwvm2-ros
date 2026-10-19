// Values/diagnostics only. These methods do not import, grant or create entities.
pub fn Endpoint::role(self : Endpoint) -> Int { self.owners.role() }
pub fn Endpoint::maximum_body(self : Endpoint) -> Int { self.maximum_body }
