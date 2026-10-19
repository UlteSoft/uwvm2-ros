//go:build tinygo && (wasm || tinygo.wasm) && ros2 && !ros1 && rmw_service_v1

package rmwservice

func(e *endpoint)inspect()uint32{
 if n:=e.live();n!=0{return n}
 for i:=range e.inspection{e.inspection[i]=0xa5};e.diagnostic.Inspection.Attempted=true;n:=e.group.io.inspect(e);e.diagnostic.Inspection.Code=n
 if n!=0{e.primary(n);return n}
 e.diagnostic.InspectionPresent=true;e.diagnostic.Inspected=e.inspection
 r,n:=parseReceipt(e.inspection[:],2,e.role,e.owners[0].handle)
 if n==0&&(r!=e.receipt){n=13}
 if n==0{for i:=range e.original{if i!=6&&e.original[i]!=e.inspection[i]{n=13;break}}}
 e.diagnostic.InspectionValidation=n;if n!=0{return e.terminal(n)};return 0
}
func(e *endpoint)find(t uint64)*transaction{if t==0{return nil};for i:=0;i<e.used;i++{if e.history[i].ticket==t{return &e.history[i]}};return nil}
func(e *endpoint)submit(bytes []byte)(uint64,uint32){
 if len(bytes)>e.requestMaximum{return 0,90};if n:=e.inspect();n!=0{return 0,n};if e.used==len(e.history){return 0,90}
 // This irreversible local slot reservation precedes the actual send, including
 // Again11. No retry recycles an admission attempt or replays an unknown send.
 t:=&e.history[e.used];e.used++;t.state=reserved;e.diagnostic.TransactionSlots=e.used
 copy(e.input,bytes);e.ticket=[8]byte{};e.diagnostic.Submit.Attempted=true;n:=e.group.io.submit(e,len(bytes));e.diagnostic.Submit.Code=n
 if n!=0{e.primary(n);if n!=11{e.terminal(n)};return 0,n}
 value:=u64(e.ticket[:]);if value==0||e.find(value)!=nil{return 0,e.terminal(22)}
 t.ticket=value;t.state=admitted;e.diagnostic.LastTicket=value;return value,0
}
func(e *endpoint)respond(ticket uint64,bytes []byte)uint32{
 if len(bytes)>e.responseMaximum{return 90};if n:=e.live();n!=0{return n};t:=e.find(ticket);if t==nil||t.state!=delivered{return 9};if n:=e.inspect();n!=0{return n}
 copy(e.input,bytes);t.state=done // once-only even when native response/ACK is lost
 e.diagnostic.Respond.Attempted=true;n:=e.group.io.respond(e,ticket,len(bytes));e.diagnostic.Respond.Code=n;if n!=0{return e.terminal(n)};return 0
}
func(e *endpoint)abandon(ticket uint64)uint32{
 if n:=e.live();n!=0{return n};t:=e.find(ticket);if t==nil||t.state!=admitted{return 9};if n:=e.inspect();n!=0{return n}
 t.state=abandoned;e.diagnostic.Abandon.Attempted=true;n:=e.group.io.abandon(e,ticket);e.diagnostic.Abandon.Code=n;if n!=0{return e.terminal(n)};return 0
}
func(e *endpoint)cancel(d record,primary uint32)uint32{
 e.primary(primary);e.diagnostic.Cancel.Attempted=true;n:=e.group.io.cancel(e,d.Sequence,d.Reservation);e.diagnostic.Cancel.Code=n
 if n!=0{e.terminal(primary)};return primary
}
func(e *endpoint)commit(d record)uint32{
 e.diagnostic.Commit.Attempted=true;n:=e.group.io.commit(e,d.Sequence,d.Reservation);e.diagnostic.Commit.Code=n
 if n!=0{return e.terminal(n)};e.diagnostic.LastCommittedSequence=d.Sequence;return 0
}
func(e *endpoint)borrow()(record,uint32){
 var d record;if n:=e.inspect();n!=0{return d,n};if e.role==4&&e.used==len(e.history){return d,90}
 maximum:=e.requestMaximum;if e.role==3{maximum=e.responseMaximum}
 // Capacity is exact for this component, not the possibly larger Group scratch.
 packet:=e.packet[:overhead+maximum];for i:=range packet{packet[i]=0xa5}
 // The fixed endpoint slice was selected before its first capability open.
 n:=e.group.io.peek(e);e.diagnostic.Peek=Outcome{true,n}
 if n!=0{if n!=11{e.primary(n)};if n!=11&&n!=90&&n!=95{e.terminal(n)};return d,n}
 e.diagnostic.LastPacketPresent=true;copy(e.diagnostic.LastPacketPrefix[:],packet[:overhead])
 var token bool;d,n,token=parseRecord(packet,e.receipt,e.owners[1].handle,e.group.options.WriterBytes,maximum)
 if !token{return d,e.terminal(n)}
 if d.Sequence<=e.diagnostic.LastCommittedSequence||d.Reservation<=e.diagnostic.LastReservation||(e.role==3&&(e.diagnostic.LastCommittedSequence==^uint64(0)||d.Sequence!=e.diagnostic.LastCommittedSequence+1)){return d,e.terminal(13)}
 e.diagnostic.LastReservation=d.Reservation
 if n!=0{n=e.cancel(d,n);if n!=90&&n!=95{e.terminal(n)};return d,n}
 t:=e.find(d.Ticket)
 if e.role==3 {
  if d.Ticket!=0&&(t==nil||(t.state!=admitted&&t.state!=abandoned)){n=e.cancel(d,13);return d,e.terminal(n)}
 }else if d.Kind==2 {
  if t!=nil{n=e.cancel(d,13);return d,e.terminal(n)}
  // Native writer/sequence is independent from the opaque Gateway ticket.
  // Preserve and compare all actual bytes, including an all-zero writer.
  for i:=0;i<e.used;i++{h:=&e.history[i];if h.nativePresent&&h.writerBytes==d.WriterBytes&&h.writer==d.Writer&&h.nativeSequence==d.RequestSequence{n=e.cancel(d,13);return d,e.terminal(n)}}
 }
 return d,0
}
func(e *endpoint)takeControl(d record)uint32{
 if n:=e.commit(d);n!=0{return n}
 if t:=e.find(d.Ticket);t!=nil{t.state=done}
 if d.Kind==7||d.Ticket==0{reason:=d.Error;if reason==0{reason=125};e.poison(reason)}
 return 0
}
func(e *endpoint)availableNow(wait uint64)(bool,uint32){
 if e.role!=3||e.ownerCount!=3{return false,95};if wait>e.group.options.MaximumWaitNanoseconds||wait>60000000000{return false,22};if n:=e.inspect();n!=0{return false,n}
 e.available[0]=0xa5;e.diagnostic.Available.Attempted=true;n:=e.group.io.available(e,wait);e.diagnostic.Available.Code=n
 if n!=0{e.primary(n);return false,n};if e.available[0]>1{return false,e.terminal(22)};return e.available[0]==1,0
}
func(e *endpoint)close(revoke bool)error{
 if e==nil||e.group==nil{return errCode(9)};g:=e.group;if g.busy{return errCode(17)}
 if e.closed{return cleanupError(e)};if n:=g.enter();n!=0{return errCode(n)};defer g.leave()
 if revoke {e.retire();for i:=e.ownerCount-1;i>=0;i--{if !e.owners[i].live{continue};e.owners[i].live=false // actual revoke retires locally before its remote result
 e.diagnostic.Revoke[i].Attempted=true;e.diagnostic.Revoke[i].Code=g.io.revoke(e.owners[i].handle)}}
 e.cleanup();return cleanupError(e)
}
func cleanupError(e *endpoint)error{
 if e.diagnostic.Retire.Attempted&&e.diagnostic.Retire.Code!=0{return errCode(e.diagnostic.Retire.Code)}
 for i:=e.ownerCount-1;i>=0;i--{if e.diagnostic.Revoke[i].Attempted&&e.diagnostic.Revoke[i].Code!=0{return errCode(e.diagnostic.Revoke[i].Code)};if e.diagnostic.Close[i].Attempted&&e.diagnostic.Close[i].Code!=0{return errCode(e.diagnostic.Close[i].Code)}};return nil
}
