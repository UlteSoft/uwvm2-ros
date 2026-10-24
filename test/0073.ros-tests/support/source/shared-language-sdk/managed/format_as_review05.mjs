/** Real clang-format syntax formatting checked against AS0.28.20's native AST. */
import fs from "node:fs";
import path from "node:path";
import crypto from "node:crypto";
import {spawnSync} from "node:child_process";
import {pathToFileURL} from "node:url";
const [root, compiler, closureFile, closureSha, formatter, formatterSha, output, baseline] = process.argv.slice(2);
if (!output || ![9, 10].includes(process.argv.length) || process.platform !== "linux" ||
    process.env.UWVM_RESOURCE_HEAVY_LOCK_HELD !== "1") throw new Error("explicit inputs and original Linux owner required");
function pin(file) {
  const body = fs.readFileSync(file);
  return {path: fs.realpathSync(file), bytes: body.length, sha256: crypto.createHash("sha256").update(body).digest("hex")};
}
if (pin(closureFile).sha256 !== closureSha || pin(formatter).sha256 !== formatterSha) throw new Error("actual tool pin changed");
const closure = JSON.parse(fs.readFileSync(closureFile, "utf8"));
for (const item of closure.files) if (JSON.stringify(pin(item.path)) !== JSON.stringify(item)) throw new Error("actual complete AS closure changed");
if (!closure.files.some(item => item.path === fs.realpathSync(compiler))) throw new Error("native parser is outside its closure");
const as = await import(pathToFileURL(compiler).href);
const version = JSON.parse(fs.readFileSync(path.resolve(path.dirname(compiler), "../package.json"), "utf8")).version;
if (version !== "0.28.20") throw new Error("review the actual compiler AST adapter before changing version");
function canonical(text, name) {
  const parser = new as.Parser();
  const comments = [];
  parser.onComment = (kind, content) => comments.push({kind, content});
  parser.parseFile(text, name, true);
  if (parser.diagnostics.length) throw new Error("native AS parser diagnostics: " + name);
  return {ast: as.ASTBuilder.build(parser.sources.at(-1)), comments};
}
function files(directory) {
  return fs.readdirSync(directory, {withFileTypes: true}).flatMap(item => {
    const file = path.join(directory, item.name);
    if (item.isSymbolicLink()) throw new Error("ordinary confined AS sources required");
    if (["examples", "test", "tests", "contracts"].includes(item.name)) return [];
    return item.isDirectory() ? files(file) : file.endsWith(".ts") ? [file] : [];
  }).sort();
}
const style = "{BasedOnStyle: LLVM, Language: JavaScript, IndentWidth: 4, ColumnLimit: 100, JavaScriptQuotes: Leave, ReflowComments: false, BreakStringLiterals: false, SortIncludes: Never}";
const rows = [];
if (baseline && JSON.stringify(files(root).map(file => path.relative(root, file))) !==
    JSON.stringify(files(baseline).map(file => path.relative(baseline, file)))) {
  throw new Error("documentation candidate and native-AST baseline must contain the same complete module set");
}
fs.mkdirSync(output, {recursive: false});
for (const file of files(root)) {
  const relative = path.relative(root, file), before = fs.readFileSync(file, "utf8");
  function format(text) {
    const result = spawnSync(formatter, ["--style=" + style, "--assume-filename=" + relative],
      {input: text, encoding: "utf8", maxBuffer: 2 << 20, timeout: 20000});
    if (result.status !== 0 || result.stderr) throw new Error("actual syntax formatter failed: " + relative + ": " + result.stderr);
    return result.stdout;
  }
  const first = format(before), second = format(first), ast = canonical(before, relative), formattedAst = canonical(first, relative);
  if (first !== second || ast.ast !== formattedAst.ast || JSON.stringify(ast.comments) !== JSON.stringify(formattedAst.comments)) {
    throw new Error("native AS AST/comments or repeat-format bytes changed: " + relative);
  }
  const reference = baseline ? path.join(baseline, relative) : null;
  const referencePin = reference ? pin(reference) : null;
  if (reference && canonical(fs.readFileSync(reference, "utf8"), relative).ast !== ast.ast) {
    throw new Error("documentation candidate changes the native executable AST: " + relative);
  }
  const target = path.join(output, "formatted", relative);
  fs.mkdirSync(path.dirname(target), {recursive: true});
  fs.writeFileSync(target, first);
  rows.push({relative, source: pin(file), product: pin(target), actual_native_ast_and_comments_equal: true,
    canonical_ast_sha256: crypto.createHash("sha256").update(ast.ast).digest("hex"), second_pass_byte_idempotence: true,
    documentation_baseline: referencePin, native_executable_ast_equal_to_baseline: reference ? true : null});
}
for (const row of rows) {
  if (JSON.stringify(pin(row.source.path)) !== JSON.stringify(row.source) ||
      (row.documentation_baseline && JSON.stringify(pin(row.documentation_baseline.path)) !== JSON.stringify(row.documentation_baseline))) {
    throw new Error("actual source or native-AST baseline changed during qualification");
  }
}
fs.writeFileSync(path.join(output, "results.json"), JSON.stringify({format: "task04-native-as-format-v1",
  status: "SCOPED_NATIVE_AS_AST_COMMENT_IDEMPOTENCE_PASS", style, files: rows, compiler_version: version,
  compiler: pin(compiler), closure: pin(closureFile), formatter: pin(formatter), node: pin(process.execPath),
  native_executable_ast_baseline: baseline ? fs.realpathSync(baseline) : null,
  build_credit: 0, guest_credit: 0, public_api_documentation_qualification: false, default_authority: []}, null, 2) + "\n");
