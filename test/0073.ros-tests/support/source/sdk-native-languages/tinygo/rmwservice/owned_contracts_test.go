//go:build tinygo && (wasm || tinygo.wasm) && ros2 && !ros1 && rmw_service_v1

package rmwservice

// Authored private-transport models, never a production fallback or Native proof.
import (
	"strings"
	"testing"
	ros "uwvm2.local/roswasm"
)

type model struct {
	opens, closes, revokes, activations, inspections, submits, responds, abandons, peeks, commits, cancels, queries, retires int
	closeOrder                                                                                                               [6]uint64
	active                                                                                                                   [16]bool
	control                                                                                                                  uint8
	submitCode, respondCode, commitCode, cancelCode, queryCode, closeCode, revokeCode                                        uint32
	submittedTicket, sequence, nonce                                                                                         uint64
	body                                                                                                                     []byte
	omitTicket, badScope, badBool                                                                                            bool
}

func (m *model) ops() transport {
	return transport{
		abi: func() uint32 { return 1 }, open: func(o *owner) uint32 {
			m.opens++
			m.active[m.opens] = true
			put(o.slot[:], uint64(m.opens)<<32|uint64(m.opens), 8)
			return 0
		},
		close: func(h uint64) uint32 {
			m.closeOrder[m.closes] = h
			m.closes++
			i := int(uint32(h))
			if i <= 0 || i >= len(m.active) || !m.active[i] {
				return 9
			}
			m.active[i] = false
			return m.closeCode
		}, revoke: func(h uint64) uint32 {
			m.revokes++
			i := int(uint32(h))
			if i <= 0 || i >= len(m.active) || !m.active[i] {
				return 9
			}
			m.active[i] = false
			return m.revokeCode
		},
		activate: func(e *endpoint) uint32 {
			m.activations++
			e.original = fixtureReceipt(e.owners[0].handle, e.role)
			if m.badScope {
				put(e.original[24:], 999, 8)
			}
			return 0
		},
		inspect: func(e *endpoint) uint32 { m.inspections++; e.inspection = e.original; e.inspection[6] = 2; return 0 },
		submit: func(e *endpoint, n int) uint32 {
			m.submits++
			if m.submitCode != 0 {
				return m.submitCode
			}
			if !m.omitTicket {
				put(e.ticket[:], m.submittedTicket, 8)
			}
			return 0
		},
		respond: func(*endpoint, uint64, int) uint32 { m.responds++; return m.respondCode }, abandon: func(*endpoint, uint64) uint32 { m.abandons++; return 0 },
		peek: func(e *endpoint) uint32 {
			m.peeks++
			b := fixturePacket(e.receipt, e.owners[1].handle, m.submittedTicket, m.sequence, m.nonce, m.body)
			if m.control != 0 {
				v := b[160:]
				v[6] = m.control
				put(v[12:], 0, 4)
				put(v[72:], 0, 8)
				put(v[80:], 0, 8)
				put(v[104:], 0, 8)
				v[121] = 0
				v[122] = 3
				put(v[124:], 125, 4)
			}
			if len(b) > len(e.packet) {
				return 90
			}
			copy(e.packet, b)
			put(e.packet[96:], uint64(len(e.packet)), 8)
			return 0
		},
		commit: func(*endpoint, uint64, uint64) uint32 { m.commits++; return m.commitCode }, cancel: func(*endpoint, uint64, uint64) uint32 { m.cancels++; return m.cancelCode },
		available: func(e *endpoint, w uint64) uint32 {
			m.queries++
			if m.queryCode != 0 {
				return m.queryCode
			}
			if m.badBool {
				e.available[0] = 2
			} else {
				e.available[0] = 1
			}
			return 0
		}, retire: func(*endpoint) uint32 { m.retires++; return 0 },
	}
}
func modelGroup(t *testing.T, m *model) *Group {
	t.Helper()
	g, err := newGroup(Options{Endpoints: 2, TransactionsPerEndpoint: 3, MaximumRequest: 2, MaximumResponse: 2, WriterBytes: 16, MaximumWaitNanoseconds: 7, StorageBudgetBytes: 1 << 20}, m.ops())
	if err != nil {
		t.Fatal(err)
	}
	return g
}
func definition() Definition[byte, byte] {
	codec := Codec[byte]{Maximum: 2, Encode: func(v byte) ([]byte, error) { return []byte{v}, nil }, Decode: func(b []byte) (byte, error) {
		if len(b) != 1 {
			return 0, ros.Error{Code: 22}
		}
		return b[0], nil
	}}
	return Definition[byte, byte]{NativeType: "contract_only/srv/Fixture", CanonicalIdentity: "UWVMROS1_" + strings.Repeat("a", 64), Request: codec, Response: codec}
}
func modelClient(t *testing.T, g *Group, d Definition[byte, byte], query bool) *Client[byte, byte] {
	t.Helper()
	a := ClientAliases{Call: "call", Observer: "observe"}
	if query {
		a.Availability = "query"
	}
	c, err := NewClient(g, d, a, [2]QoS{fixtureQoS(), fixtureQoS()})
	if err != nil {
		t.Fatal(err)
	}
	return c
}
func isCode(err error, n uint32) bool { return codeOf(err) == n }
func TestNoAvailabilityOwner95NoQuery(t *testing.T) {
	m := &model{}
	g := modelGroup(t, m)
	c := modelClient(t, g, definition(), false)
	if _, err := c.Available(0); !isCode(err, 95) || m.queries != 0 || m.opens != 2 {
		t.Fatal(err, m)
	}
	if err := g.Close(); err != nil || m.closes != 2 || m.retires != 1 {
		t.Fatal(err, m)
	}
}
func TestFullU64TicketAndSignedSameTake(t *testing.T) {
	m := &model{submittedTicket: ^uint64(0), sequence: 1, nonce: 1, body: []byte{8}}
	g := modelGroup(t, m)
	defer g.Close()
	c := modelClient(t, g, definition(), true)
	ticket, err := c.Submit(4)
	if err != nil || ticket != ^uint64(0) {
		t.Fatal(ticket, err)
	}
	v, err := c.Receive()
	if err != nil || !v.HasValue || v.Value != 8 || v.Metadata.Ticket != ticket || v.Metadata.RequestSequence != -1 || m.commits != 1 {
		t.Fatal(v, err, m)
	}
}
func TestZeroSuccessTicketRetiresWithoutReplay(t *testing.T) {
	m := &model{omitTicket: true}
	g := modelGroup(t, m)
	c := modelClient(t, g, definition(), false)
	if _, err := c.Submit(1); !isCode(err, 22) || m.retires != 1 {
		t.Fatal(err, m)
	}
	if _, err := c.Submit(1); !isCode(err, 125) || m.submits != 1 {
		t.Fatal(err, m)
	}
	g.Close()
	if m.retires != 1 || m.closes != 2 {
		t.Fatal(m)
	}
}
func TestAbandonedMalformedStillCompleteDecode(t *testing.T) {
	m := &model{submittedTicket: 77, sequence: 1, nonce: 1, body: nil}
	g := modelGroup(t, m)
	defer g.Close()
	c := modelClient(t, g, definition(), false)
	c.Submit(4)
	if err := c.Abandon(77); err != nil {
		t.Fatal(err)
	}
	v, err := c.Receive()
	if !isCode(err, 22) || v.HasValue || v.Abandoned || m.cancels != 1 || m.commits != 0 || m.retires != 1 {
		t.Fatal(v, err, m)
	}
}
func TestCancelledBorrowUsesFreshNonceAndNoConsume95(t *testing.T) {
	m := &model{submittedTicket: 77, sequence: 1, nonce: 1, body: []byte{8}}
	g := modelGroup(t, m)
	defer g.Close()
	d := definition()
	refuse := true
	d.Response.Decode = func(b []byte) (byte, error) {
		if refuse {
			return 0, ros.Error{Code: 95}
		}
		return b[0], nil
	}
	c := modelClient(t, g, d, false)
	c.Submit(4)
	if _, err := c.Receive(); !isCode(err, 95) || m.cancels != 1 || m.commits != 0 {
		t.Fatal(err, m)
	}
	refuse = false
	m.nonce = 2
	v, err := c.Receive()
	if err != nil || !v.HasValue || m.commits != 1 {
		t.Fatal(v, err, m)
	}
}
func TestCommitLostACKNoValueNoReplay(t *testing.T) {
	m := &model{submittedTicket: 77, sequence: 1, nonce: 1, body: []byte{8}, commitCode: 110}
	g := modelGroup(t, m)
	defer g.Close()
	c := modelClient(t, g, definition(), false)
	c.Submit(4)
	v, err := c.Receive()
	if !isCode(err, 110) || v.HasValue || m.retires != 1 || m.commits != 1 {
		t.Fatal(v, err, m)
	}
	c.Receive()
	if m.peeks != 1 || m.commits != 1 {
		t.Fatal(m)
	}
}
func TestCancelFailureKeepsPrimaryAndCleanupSeparate(t *testing.T) {
	m := &model{submittedTicket: 77, sequence: 1, nonce: 1, body: []byte{8}, cancelCode: 110}
	g := modelGroup(t, m)
	defer g.Close()
	d := definition()
	d.Response.Decode = func([]byte) (byte, error) { return 0, ros.Error{Code: 95} }
	c := modelClient(t, g, d, false)
	c.Submit(4)
	_, err := c.Receive()
	diag, _ := c.Diagnostic()
	if !isCode(err, 95) || diag.Primary != 95 || diag.Cancel.Code != 110 || !diag.Retire.Attempted || m.retires != 1 {
		t.Fatal(err, diag)
	}
}
func TestDecoderWholeGroupCloseRevokeAndRecursive17(t *testing.T) {
	m := &model{submittedTicket: 77, sequence: 1, nonce: 1, body: []byte{8}}
	g := modelGroup(t, m)
	defer g.Close()
	d := definition()
	var c *Client[byte, byte]
	d.Response.Decode = func(b []byte) (byte, error) {
		if !isCode(g.Close(), 17) || !isCode(c.Revoke(), 17) {
			t.Fatal("lease")
		}
		if _, err := c.Submit(1); !isCode(err, 17) {
			t.Fatal(err)
		}
		if m.closes != 0 || m.retires != 0 || m.revokes != 0 {
			t.Fatal(m)
		}
		return b[0], nil
	}
	c = modelClient(t, g, d, false)
	c.Submit(4)
	if _, err := c.Receive(); err != nil {
		t.Fatal(err)
	}
}
func TestAllReverseClosesOnceWithIndependentErrors(t *testing.T) {
	m := &model{closeCode: 9}
	g := modelGroup(t, m)
	c := modelClient(t, g, definition(), true)
	if !isCode(g.Close(), 9) || m.closes != 3 || m.retires != 1 || m.closeOrder[0] != 0x300000003 || m.closeOrder[2] != 0x100000001 {
		t.Fatal(m)
	}
	g.Close()
	diag, _ := c.Diagnostic()
	for i := range diag.Close {
		if !diag.Close[i].Attempted || diag.Close[i].Code != 9 {
			t.Fatal(diag)
		}
	}
	if m.closes != 3 || m.retires != 1 {
		t.Fatal(m)
	}
}
func TestRevokeAndCloseAreIndependentOnce(t *testing.T) {
	m := &model{revokeCode: 13, closeCode: 9}
	g := modelGroup(t, m)
	c := modelClient(t, g, definition(), true)
	if err := c.Revoke(); !isCode(err, 13) || m.revokes != 3 || m.closes != 0 || m.retires != 1 {
		t.Fatal(err, m)
	}
	g.Close()
	c.Revoke()
	if m.revokes != 3 || m.closes != 0 || m.retires != 1 {
		t.Fatal(m)
	}
}
func TestQueryBoundAndBooleanSentinel(t *testing.T) {
	m := &model{}
	g := modelGroup(t, m)
	defer g.Close()
	c := modelClient(t, g, definition(), true)
	if _, err := c.Available(8); !isCode(err, 22) || m.queries != 0 {
		t.Fatal(err)
	}
	v, err := c.Available(0)
	if err != nil || !v || m.queries != 1 {
		t.Fatal(v, err)
	}
	m.badBool = true
	if v, err = c.Available(7); v || !isCode(err, 22) || m.retires != 1 {
		t.Fatal(v, err, m)
	}
}
func TestServerSameNativeWriterSequenceDuplicate(t *testing.T) {
	m := &model{submittedTicket: 77, sequence: 1, nonce: 1, body: []byte{8}}
	g := modelGroup(t, m)
	defer g.Close()
	s, err := NewServer(g, definition(), ServerAliases{"advertise", "responder"}, [2]QoS{fixtureQoS(), fixtureQoS()})
	if err != nil {
		t.Fatal(err)
	}
	if _, err = s.TakeRequest(); err != nil {
		t.Fatal(err)
	}
	m.submittedTicket = 78
	m.sequence = 2
	m.nonce = 2
	if _, err = s.TakeRequest(); !isCode(err, 13) || m.cancels != 1 || m.retires != 1 {
		t.Fatal(err, m)
	}
}
func TestServerUnknownResponseOutcomeNeverReplay(t *testing.T) {
	m := &model{submittedTicket: 77, sequence: 1, nonce: 1, body: []byte{8}, respondCode: 110}
	g := modelGroup(t, m)
	defer g.Close()
	s, err := NewServer(g, definition(), ServerAliases{"advertise", "responder"}, [2]QoS{fixtureQoS(), fixtureQoS()})
	if err != nil {
		t.Fatal(err)
	}
	s.TakeRequest()
	if err = s.Respond(77, 9); !isCode(err, 110) {
		t.Fatal(err)
	}
	s.Respond(77, 9)
	if m.responds != 1 || m.retires != 1 {
		t.Fatal(m)
	}
}
func TestScopeAndAliasPreflight(t *testing.T) {
	m := &model{}
	g := modelGroup(t, m)
	defer g.Close()
	modelClient(t, g, definition(), false)
	before := m.opens
	if _, err := NewClient(g, definition(), ClientAliases{"call", "other", ""}, [2]QoS{fixtureQoS(), fixtureQoS()}); !isCode(err, 17) || m.opens != before {
		t.Fatal(err, m)
	}
	m.badScope = true
	if _, err := NewServer(g, definition(), ServerAliases{"advertise", "respond"}, [2]QoS{fixtureQoS(), fixtureQoS()}); !isCode(err, 13) || m.retires != 1 || m.closes != 2 {
		t.Fatal(err, m)
	}
}
func TestAgainReservesHistoryNoAutomaticSendRetry(t *testing.T) {
	m := &model{submitCode: 11}
	g := modelGroup(t, m)
	g.entries[0].history = g.entries[0].history[:1]
	defer g.Close()
	c := modelClient(t, g, definition(), false)
	if _, err := c.Submit(1); !isCode(err, 11) {
		t.Fatal(err)
	}
	if _, err := c.Submit(1); !isCode(err, 90) || m.submits != 1 {
		t.Fatal(err, m)
	}
}
func TestNativeInspectMutationBeforeBusiness(t *testing.T) {
	m := &model{}
	g := modelGroup(t, m)
	defer g.Close()
	c := modelClient(t, g, definition(), false)
	c.e.original[224] = 1
	if _, err := c.Submit(1); !isCode(err, 22) || m.submits != 0 || m.retires != 1 {
		t.Fatal(err, m)
	}
}

func TestCopiedGroupCannotBypassLease(t *testing.T) {
	m := &model{}
	g := modelGroup(t, m)
	defer g.Close()
	modelClient(t, g, definition(), false)
	copyOf := *g
	if err := copyOf.Close(); !isCode(err, 22) || m.closes != 0 || m.retires != 0 {
		t.Fatal(err, m)
	}
}
func TestAbandonedCompleteValueDiscardedOnlyAfterACK(t *testing.T) {
	m := &model{submittedTicket: 77, sequence: 1, nonce: 1, body: []byte{8}}
	g := modelGroup(t, m)
	defer g.Close()
	c := modelClient(t, g, definition(), false)
	c.Submit(4)
	c.Abandon(77)
	v, err := c.Receive()
	if !isCode(err, 125) || v.HasValue || !v.Abandoned || m.commits != 1 || m.cancels != 0 {
		t.Fatal(v, err, m)
	}
}

// Genuine capability_revoke invalidates the handle before its remote result.
// The private model now refuses any close on that same attempted-revoked owner.
func TestRevokeSuccessDoesNotInventCloseBadHandle(t *testing.T) {
	m := &model{}
	g := modelGroup(t, m)
	c := modelClient(t, g, definition(), true)
	if err := c.Revoke(); err != nil {
		t.Fatal(err)
	}
	d, _ := c.Diagnostic()
	for i := 0; i < 3; i++ {
		if !d.Revoke[i].Attempted || d.Revoke[i].Code != 0 || d.Close[i].Attempted {
			t.Fatal(d)
		}
	}
	if m.closes != 0 || m.revokes != 3 || m.retires != 1 {
		t.Fatal(m)
	}
	g.Close()
	if m.closes != 0 || m.revokes != 3 || m.retires != 1 {
		t.Fatal(m)
	}
}
func TestControlACKTailHasNoImportClientOrServer(t *testing.T) {
	for _, server := range []bool{false, true} {
		m := &model{sequence: 1, nonce: 1, control: 7}
		g := modelGroup(t, m)
		if server {
			s, err := NewServer(g, definition(), ServerAliases{"ad", "responder"}, [2]QoS{fixtureQoS(), fixtureQoS()})
			if err != nil {
				t.Fatal(err)
			}
			v, err := s.TakeRequest()
			if !isCode(err, 125) || v.HasValue || v.Metadata.Kind != 7 || v.Metadata.ControlFlags != 3 {
				t.Fatal(v, err)
			}
		} else {
			c := modelClient(t, g, definition(), false)
			v, err := c.Receive()
			if !isCode(err, 125) || v.HasValue || v.Metadata.Kind != 7 || v.Metadata.ControlFlags != 3 {
				t.Fatal(v, err)
			}
		}
		if m.commits != 1 || m.retires != 0 || m.closes != 0 || m.revokes != 0 {
			t.Fatal("post-ACK effects", m)
		}
		g.Close()
		if m.retires != 1 || m.closes != 2 || m.commits != 1 {
			t.Fatal(m)
		}
	}
}
