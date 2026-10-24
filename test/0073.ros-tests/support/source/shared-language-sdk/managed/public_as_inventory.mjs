/** @brief Inventory exported AssemblyScript declarations with its real parser.
 * @details Parses the explicit source tree without executing any application.
 * Source/tool hashes, native AST ranges/flags, comments and parameter spans are
 * retained. Unknown export shapes remain unresolved. Documentation semantics,
 * formatting, compiled artifacts and Guests are separate qualification gates.
 * @see public_python_inventory.py
 */
import fs from "node:fs";
import path from "node:path";
import crypto from "node:crypto";
import {pathToFileURL} from "node:url";
import {gzipSync} from "node:zlib";

const args = {};
for (let at = 2; at < process.argv.length; at += 2) {
  const key = process.argv[at];
  if (!key.startsWith("--") || args[key.slice(2)] !== undefined || process.argv[at + 1] === undefined) {
    throw new Error("one explicit value per closed source/parser argument is required");
  }
  args[key.slice(2)] = process.argv[at + 1];
}
const required = ["root", "compiler", "tool-inputs", "tool-inputs-sha256", "out"];
if (Object.keys(args).some(key => !required.includes(key) && key !== "compression") ||
    (args.compression !== undefined && args.compression !== "gzip") || required.some(key => !args[key])) {
  throw new Error("complete explicit source/parser/output arguments required");
}
if (process.platform !== "linux" || process.env.UWVM_RESOURCE_HEAVY_LOCK_HELD !== "1") {
  throw new Error("the original admitted Linux finite owner is required");
}
function pin(file) {
  const real = fs.realpathSync(file), raw = fs.readFileSync(real);
  return {path: real, bytes: raw.length, sha256: crypto.createHash("sha256").update(raw).digest("hex")};
}
const root = fs.realpathSync(args.root), output = path.resolve(args.out);
fs.mkdirSync(output, {recursive: false});
const report = {format: "task04-assemblyscript-compiler-api-inventory-v1", status: "STARTED",
  source_root: root, parser: "actual AssemblyScript Parser", modules: [], entries: [], exports: [], unresolved_exports: [],
  compile_credit: 0, guest_credit: 0, complete_public_api_coverage: false,
  semantic_review: "PENDING; comments/tags are not semantic qualification", formatter_qualified: false, default_authority: []};
function save() {
  const raw = Buffer.from(JSON.stringify(report, null, 2) + "\n");
  if (args.compression === "gzip") {
    const compressed = gzipSync(raw, {level: 9});
    fs.writeFileSync(path.join(output, "inventory.json.gz"), compressed);
    fs.writeFileSync(path.join(output, "summary.json"), JSON.stringify({status: report.status,
      modules: report.modules.length, scoped_entries: report.entries.length,
      missing_documentation: report.missing_documentation?.length ?? null,
      unresolved_exports: report.unresolved_exports.length, unresolved_see: report.unresolved_see?.length ?? null,
      complete_public_api_coverage: false, compiler_artifact_credit: 0, guest_credit: 0,
      payload: {file: "inventory.json.gz", bytes: compressed.length,
        sha256: crypto.createHash("sha256").update(compressed).digest("hex"),
        uncompressed_bytes: raw.length, uncompressed_sha256: crypto.createHash("sha256").update(raw).digest("hex")}}, null, 2) + "\n");
  } else fs.writeFileSync(path.join(output, "inventory.json"), raw);
}
function files(directory) {
  return fs.readdirSync(directory, {withFileTypes: true}).flatMap(item => {
    const file = path.join(directory, item.name);
    if (item.isSymbolicLink()) throw new Error("ordinary confined source/input tree required");
    return item.isDirectory() ? files(file) : item.isFile() ? [file] : [];
  }).sort();
}
function docs(text) {
  const tags = {};
  // onComment supplies the delimiters too. Strip those before recognizing a
  // first-line @brief; never infer a declaration from comment text.
  const content = (text || "").replace(/^\/\*\*?/, "").replace(/\*\/$/, "");
  for (const raw of content.split("\n")) {
    const line = raw.trim().replace(/^\*\s?/, "");
    if (line.startsWith("@")) {
      const space = line.indexOf(" "), key = line.slice(1, space < 0 ? undefined : space);
      (tags[key] ||= []).push(space < 0 ? "" : line.slice(space + 1));
    }
  }
  return {text, tags, missing: ["brief", "details", "see"].filter(key => !tags[key])};
}
try {
  const inputPin = pin(args["tool-inputs"]);
  if (inputPin.sha256 !== args["tool-inputs-sha256"]) throw new Error("selected actual parser closure changed");
  const closure = JSON.parse(fs.readFileSync(args["tool-inputs"], "utf8"));
  if (closure.language !== "as" || closure.format !== "uwvm-managed-example-inputs-v1") throw new Error("actual AS tool closure required");
  for (const row of closure.files) {
    if (JSON.stringify(pin(row.path)) !== JSON.stringify(row)) throw new Error("actual parser/compiler input differs");
  }
  const parserFile = fs.realpathSync(args.compiler);
  if (!closure.files.some(row => row.path === parserFile)) throw new Error("selected parser is outside the pinned compiler closure");
  const packageFile = path.resolve(path.dirname(parserFile), "../package.json");
  const packageInfo = JSON.parse(fs.readFileSync(packageFile, "utf8"));
  if (packageInfo.version !== "0.28.20") throw new Error("review the AST adapter before changing the exact AS version");
  const as = await import(pathToFileURL(parserFile).href);
  if (typeof as.Parser !== "function") throw new Error("the actual compiler Parser export is unavailable");
  report.tools = {node: pin(process.execPath), node_version: process.version, parser: pin(parserFile),
    compiler_package: pin(packageFile), compiler_version: packageInfo.version, complete_input_closure: inputPin,
    individually_checked_tool_files: closure.files.length};
  for (const file of files(root).filter(file => file.endsWith(".ts"))) {
    const relative = path.relative(root, file);
    if (relative.split(path.sep).some(part => ["test", "tests", "contracts", "examples"].includes(part))) continue;
    const raw = fs.readFileSync(file), text = raw.toString("utf8");
    if (!Buffer.from(text, "utf8").equals(raw)) throw new Error("strict UTF-8 API source required");
    const parser = new as.Parser(), comments = [];
    parser.onComment = (kind, content, range) => comments.push({kind, content, start: range.start, end: range.end});
    parser.parseFile(text, relative, true);
    if (parser.diagnostics.length) throw new Error("actual AS parser diagnostics in " + relative);
    const source = parser.sources.find(item => item.normalizedPath === relative) || parser.sources[parser.sources.length - 1];
    if (!source || source.text !== text) throw new Error("actual parsed source attachment changed");
    const sourcePin = pin(file);
    report.modules.push({source: sourcePin, path: relative, parser_source_path: source.normalizedPath});
    const label = node => node.name?.text || ((node.flags & 524288) !== 0 ? "constructor" : null);
    const span = node => node?.range ? {start: node.range.start, end: node.range.end,
      units: "UTF-16 source code units", text: text.slice(node.range.start, node.range.end)} : null;
    const locallyExported = new Set(source.statements.filter(node => node.kind === 36 && !node.path)
      .flatMap(node => (node.members || []).map(item => item.localName.text)));
    function inspect(node, parent = "", member = false, exposed = false) {
      if (!node || !node.range) return;
      // Numeric kinds/flags are pinned to the verified0.28.20 generated API.
      if (!member && node.kind === 36) {
        report.exports.push({module: relative.replace(/\.ts$/, ""), source: sourcePin, range: span(node),
          from: node.path?.value || null,
          members: node.members?.map(item => ({local: item.localName.text, exported: item.exportedName.text,
            range: span(item)})) || null});
        return;
      }
      if (!member && [37, 38].includes(node.kind)) {
        report.unresolved_exports.push({source: sourcePin, kind: node.kind, range: span(node),
          reason: "parsed reexport/default-export form requires source-graph reconciliation"});
        return;
      }
      if (Array.isArray(node.declarations)) {
        for (const child of node.declarations) inspect(child, parent, member, exposed || (node.flags & 2) !== 0);
        return;
      }
      const flags = node.flags || 0, name = label(node);
      if ((!member && !exposed && (flags & 2) === 0 && !(!parent && locallyExported.has(name))) ||
          (member && (flags & (512 | 1024)) !== 0)) return;
      if (!name) { report.unresolved_exports.push({source: sourcePin, kind: node.kind, range: span(node)}); return; }
      const qualified = parent ? parent + "." + name : name;
      const starts = [node.range.start, ...(node.decorators || []).map(item => item.range.start)];
      const begin = Math.min(...starts);
      let attachment = "adjacent-native-declaration";
      let comment = comments.filter(item => item.end <= begin && text.slice(item.end, begin).trim() === "").at(-1);
      let parameterProperty = null;
      // The actual parser synthesizes a public FieldDeclaration for a public
      // constructor parameter. Both native ranges start after `public`, so
      // ordinary adjacency misses that parameter's own comment. Reconcile only
      // identical native field/parameter ranges in the same parsed class.
      if (member && node.kind === 55 && (flags & 256) !== 0) {
        const ownerName = parent.split(".").at(-1);
        const owner = source.statements.find(item => item.name?.text === ownerName && Array.isArray(item.members));
        const constructor = owner?.members.find(item => (item.flags & 524288) !== 0);
        const parameters = constructor?.signature?.parameters || [];
        const at = parameters.findIndex(item => item.name?.text === name &&
          item.range.start === node.range.start && item.range.end === node.range.end && (item.flags & 256) !== 0);
        if (at >= 0) {
          const boundary = at ? parameters[at - 1].range.end : constructor.range.start;
          const attached = comments.filter(item => item.start >= boundary && item.end <= begin &&
            /^\s*public\s*$/.test(text.slice(item.end, begin))).at(-1);
          parameterProperty = {constructor: span(constructor), parameter: span(parameters[at]),
            exact_native_ranges_match: true};
          if (attached) { comment = attached; attachment = "native-constructor-parameter-property"; }
        }
      }
      const parameters = (node.signature?.parameters || []).map(item => ({name: item.name?.text,
        flags: item.flags, kind: item.kind, source: span(item)}));
      const accessor = (flags & 2048) !== 0 ? "#get" : (flags & 4096) !== 0 ? "#set" : "";
      report.entries.push({id: relative.replace(/\.ts$/, "") + ":" + qualified + accessor,
        declaration_name: qualified, source: sourcePin, kind: node.kind, flags, range: span(node),
        classification: Array.isArray(node.members) || Array.isArray(node.values) ? "type-or-namespace" :
          node.kind === 54 ? "enum-value" : node.signature ? "callable" : "field-or-declaration",
        parameters, return_type: span(node.signature?.returnType), initializer: span(node.initializer),
        top_level: !parent, exported: exposed || (flags & 2) !== 0,
        local_export_reachable: !parent && locallyExported.has(name),
        documentation: docs(comment ? comment.content : null), comment_range: comment || null,
        documentation_attachment: comment ? attachment : null, constructor_parameter_property: parameterProperty,
        semantic_review: "PENDING manual source-specific contract review"});
      if (Array.isArray(node.members)) for (const child of node.members) inspect(child, qualified, node.kind !== 60);
      if (Array.isArray(node.values)) for (const child of node.values) inspect(child, qualified, true);
    }
    for (const node of source.statements) inspect(node);
  }
  const exported = new Map(report.modules.map(item => [item.path.replace(/\.ts$/, ""), new Map()]));
  const localDefinitions = new Map(report.modules.map(item => [item.path.replace(/\.ts$/, ""), new Map()]));
  for (const entry of report.entries) {
    if (entry.top_level) {
      localDefinitions.get(entry.id.split(":")[0]).set(entry.declaration_name, entry.id);
      if (entry.exported) exported.get(entry.id.split(":")[0]).set(entry.declaration_name, entry.id);
    }
  }
  function targetModule(item) {
    if (!item.from) return item.module;
    if (!item.from.startsWith(".")) return null;
    const candidate = path.posix.normalize(path.posix.join(path.posix.dirname(item.module), item.from));
    return [candidate.replace(/\.ts$/, ""), candidate + "/index"].find(name => exported.has(name)) || null;
  }
  // Resolve only real parser-visible local/relative exports, including chains.
  // External packages, defaults, ambiguous exports and missing names stay gaps.
  const ambiguous = new Set();
  for (let pass = 0; pass <= report.modules.length; ++pass) {
    let changed = false;
    for (const item of report.exports) {
      const target = targetModule(item);
      if (!target) continue;
      const source = (item.from ? exported : localDefinitions).get(target), destination = exported.get(item.module);
      const rows = item.members || [...source.keys()].map(name => ({local: name, exported: name}));
      for (const row of rows) {
        const resolved = source.get(row.local);
        if (!resolved) continue;
        const key = item.module + ":" + row.exported;
        if (destination.has(row.exported) && destination.get(row.exported) !== resolved) {
          ambiguous.add(key); continue;
        }
        if (!destination.has(row.exported)) { destination.set(row.exported, resolved); changed = true; }
      }
    }
    if (!changed) break;
  }
  for (const item of report.exports) {
    const target = targetModule(item), source = target ? (item.from ? exported : localDefinitions).get(target) : null;
    item.resolved_module = target;
    item.bindings = (item.members || (source ? [...source.keys()].map(name => ({local: name, exported: name})) : []))
      .map(row => ({...row, resolved: source?.get(row.local) || null,
        ambiguous: ambiguous.has(item.module + ":" + row.exported)}));
    if (!target || item.bindings.some(row => !row.resolved || row.ambiguous)) {
      report.unresolved_exports.push({source: item.source, range: item.range,
        reason: "missing/external/ambiguous parsed export binding", from: item.from, bindings: item.bindings});
    }
  }
  const ids = new Set(report.entries.map(item => item.id));
  report.unresolved_see = [];
  for (const entry of report.entries) {
    const module = entry.id.split(":")[0];
    entry.see_references = (entry.documentation.tags.see || []).map(text => {
      const name = text.trim().split(/\s+/)[0], parts = name.split(".");
      const rootName = parts.shift(), alias = exported.get(module).get(rootName);
      const local = module + ":" + name;
      const candidate = alias ? alias + (parts.length ? "." + parts.join(".") : "") : null;
      const matches = report.entries.filter(item => item.declaration_name === name);
      const resolved = ids.has(name) ? name : ids.has(local) ? local : candidate && ids.has(candidate) ? candidate :
        matches.length === 1 ? matches[0].id : null;
      const reference = {target: name, resolved, status: resolved ? "RESOLVED_DECLARATION" : "UNRESOLVED"};
      if (!resolved) report.unresolved_see.push({id: entry.id, reference});
      return reference;
    });
  }
  report.source_hashes_rechecked = report.modules.every(item => JSON.stringify(pin(item.source.path)) === JSON.stringify(item.source));
  if (!report.source_hashes_rechecked) throw new Error("actual source changed during parser inventory");
  report.status = "PARSER_INVENTORY_REVIEW_REQUIRED";
  report.missing_documentation = report.entries.filter(item => item.documentation.missing.length).map(item => ({id: item.id, fields: item.documentation.missing}));
  report.limits = ["Only exported declarations and parser-visible public members in this source root are scoped.",
    "Default/export-import forms, external packages, namespace exports, generated families and constructor parameter property completeness require reconciliation.",
    "No source/tag/declaration count qualifies an executable example, complete API contract or official vendor program."];
} catch (error) {
  report.status = "FAILED_RETAINED"; report.error = String(error); throw error;
} finally { save(); }
