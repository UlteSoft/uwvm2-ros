"use strict";
// Exercise the actual repaired workbench functions, not reimplementations.
const fs = require("fs"), vm = require("vm"), assert = require("assert");
if (process.argv.length !== 3) throw Error("Supply the prepared repaired.js path");
const text = fs.readFileSync(process.argv[2], "utf8");
function between(first, last) {
  const start = text.indexOf(first), end = text.indexOf(last, start);
  assert(start >= 0 && end > start, "Verified workbench function anchors");
  return text.slice(start, end);
}
class Range { constructor(...values) { this.values = values; } }
const options = {_dn: "yellow-gutter", GPi: "yellow-line", Ddn: "green-gutter", Ldn: "green-line"};
const decorate = vm.runInNewContext(between("function Rdn(", "var Z0e=") + ";Rdn",
  {G: Range, ...options, qPi: () => "yellow-column"});
let checks = 0;
for (const exact of [true, false, undefined]) {
  for (const top of [true, false]) for (const gutter of [true, false]) {
    const frame = {range: {startLineNumber: 8, startColumn: 4}, uwvmSourceExact: exact,
      getId: () => "selected", thread: {getTopStackFrame: () => ({getId: () => top ? "selected" : "other"})}};
    const rows = decorate(frame, gutter, false), names = Array.from(rows, row => row.options);
    const green = !top || exact === false;
    assert.equal(names.includes("green-line"), green);
    assert.equal(names.includes("yellow-line"), !green);
    assert.equal(names.includes("yellow-column"), !green);
    assert.equal(names.filter(name => name.endsWith("-line")).length, 1);
    assert.equal(names.filter(name => name.endsWith("-gutter")).length, gutter ? 1 : 0);
    assert.equal(frame.getId(), "selected");
    ++checks;
  }
}
const mapping = between("return Object.assign(new GT(this,n.id", "}))}catch(t)");
// The map callback ends with `)` and then the enclosing map/try delimiters.
const expression = mapping.slice("return ".length);
class Frame { constructor(...args) { this.args = args; } }
const map = vm.runInNewContext("(function(n,s,i,r){return " + expression + "})", {GT: Frame, G: Range});
for (const type of ["uwvm-llvm-full", "cppdbg"]) for (const exact of [true, false, undefined, 0, "false"]) {
  const original = {id: 19, name: "real-frame", line: 8, column: 4, canRestart: false, uwvmSourceExact: exact};
  const output = map.call({session: {configuration: {type}}}, original, {path: "real.cc"}, 0, 0);
  assert.strictEqual(output.uwvmSourceExact, type === "uwvm-llvm-full" && typeof exact === "boolean" ? exact : undefined);
  assert.equal(output.args[1], original.id);
  assert.equal(output.args[3], original.name);
  ++checks;
}
const inputMethod = between("renderInputBox(i,e,t,o,uwvmTypeElement)", "disposeElement(i,e,t)");
for (const action of ["enter", "escape", "blur", "dispose"]) {
  let input, finished = [], selected = [];
  const register = (element, event, handler) => { element.handlers[event] = handler; return {dispose() {}}; };
  const Input = vm.runInNewContext("(class {" + inputMethod + "})", {
    id: class { constructor() { input = this; this.inputElement = {handlers:{}}; } focus() {} select() {} dispose() {} },
    Al: {}, ne: {KEY_DOWN: "key", BLUR: "blur", CLICK: "click"}, yt: () => {},
    ud: fn => {let done=false;return (...args) => {if(!done){done=true;return fn(...args);}};},
    qe: rows => rows.forEach(row => row.dispose()), jo: register, $: register, Z: fn => ({dispose:fn})
  });
  const owner = new Input();owner.debugService={getViewModel:()=>({setSelectedExpression:(...a)=>selected.push(a)})};
  const name={style:{}}, value={style:{}}, box={style:{}}, type={style:{display:"inline"}};
  const lifetime=owner.renderInputBox(name,value,box,{initialValue:"last_error",onFinish:(...a)=>finished.push(a)},type);
  assert.equal(type.style.display,"none");assert.equal(name.style.display,"none");assert.equal(value.style.display,"none");
  if(action==="dispose") lifetime.dispose();
  else if(action==="blur") input.inputElement.handlers.blur();
  else input.inputElement.handlers.key({equals:code=>code===(action==="enter"?3:9),preventDefault(){},stopPropagation(){}});
  assert.equal(type.style.display,"inline");assert.equal(box.style.display,"none");
  assert.equal(finished.length,action==="dispose"?0:1);
  if(finished.length) assert.equal(finished[0][1],action!=="escape");
  lifetime.dispose();assert.equal(finished.length,action==="dispose"?0:1);
  ++checks;
}
console.log("PASS " + checks + " native marker, debugger isolation and WATCH input lifecycle cases");
