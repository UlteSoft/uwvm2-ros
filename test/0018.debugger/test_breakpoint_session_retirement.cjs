"use strict";
const assert = require("node:assert/strict");
const {register} = require("../../tools/debug/breakpoint_sync");
class SourceBreakpoint { constructor(uri) { this.location = {uri}; } }
const privatePoint = id => new SourceBreakpoint({scheme: "uwvm-wasm", query: "session=" + id + "&ref=4"});
const expired = privatePoint("expired"), live = privatePoint("live"), ending = privatePoint("ending");
const source = new SourceBreakpoint({scheme: "file", query: ""});
let terminate;
const tab=point=>({input:{uri:point.location.uri}});
const expiredTab=tab(expired),liveTab=tab(live),endingTab=tab(ending),sourceTab=tab(source);
const dirtyTab={...tab(expired),isDirty:true};
const groups={all:[{tabs:[expiredTab,liveTab,endingTab,sourceTab,dirtyTab]}],close(rows,preserveFocus){
  assert.equal(preserveFocus,true);
  this.all[0].tabs=this.all[0].tabs.filter(row=>!rows.includes(row));return Promise.resolve(true);
}};
const api = {
  breakpoints: [expired, live, ending, source],
  onDidChangeBreakpoints() { return {dispose() {}}; },
  addBreakpoints(rows) { this.breakpoints.push(...rows); },
  removeBreakpoints(rows) { this.breakpoints = this.breakpoints.filter(row => !rows.includes(row)); },
  onDidTerminateDebugSession(fn) { terminate = fn; return {dispose() {}}; }
};
register({debug: api, SourceBreakpoint,window:{tabGroups:groups}}, {subscriptions: []}, new Map([["live", {}], ["ending", {}]]));
assert.deepEqual(api.breakpoints, [live, ending, source]);
assert.deepEqual(groups.all[0].tabs,[liveTab,endingTab,sourceTab,dirtyTab]);
terminate({id: "ending"});
assert.deepEqual(api.breakpoints, [live, source]);
assert.deepEqual(groups.all[0].tabs,[liveTab,sourceTab,dirtyTab]);
console.log("PASS expired private documents retire without deleting live-session or source breakpoints");
