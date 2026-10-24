// Genuine Acorn AST/token/attached API inspection; no SDK/Guest program runs.
import assert from 'node:assert/strict';
import { readFileSync, writeFileSync } from 'node:fs';
import { createHash } from 'node:crypto';
import * as acorn from '/usr/share/nodejs/acorn/dist/acorn.mjs';

const [beforePath, afterPath, output] = process.argv.slice(2);
const hash = source => createHash('sha256').update(source).digest('hex');
function parse(path) {
  const source = readFileSync(path, 'utf8'), comments = [], tokens = [];
  const tree = acorn.parse(source, { ecmaVersion: 'latest', sourceType: 'module', locations: true,
    onComment: comments, onToken: tokens });
  const syntax = JSON.stringify(tree, (key, value) => ['start', 'end', 'loc', 'raw'].includes(key) ? undefined :
    typeof value === 'bigint' ? { actualBigInt: value.toString() } : value);
  const normalizedTokens = tokens.map(token => [token.type.label, typeof token.value === 'bigint' ? token.value.toString() : token.value]);
  return { source, tree, comments, syntax, tokens: JSON.stringify(normalizedTokens) };
}
const before = parse(beforePath), after = parse(afterPath);
assert.ok(after.syntax === before.syntax, 'actual complete Acorn AST changed during formatting: ' + hash(before.syntax) + ' -> ' + hash(after.syntax));
assert.ok(after.tokens === before.tokens, 'actual complete Acorn token values changed during formatting: ' + hash(before.tokens) + ' -> ' + hash(after.tokens));
const inventory = [];
function item(name, kind, node, attachment = node) {
  const comment = after.comments.filter(value => value.end <= attachment.start && /^\s*$/.test(after.source.slice(value.end, attachment.start))).at(-1);
  assert.ok(comment?.type === 'Block' && comment.value.startsWith('*'), 'actual attached public JSDoc required: ' + name);
  for (const tag of ['@brief', '@details', '@see']) assert.ok(comment.value.includes(tag), name + ': ' + tag);
  inventory.push({ name, kind, line: node.loc.start.line, source_sha256: hash(after.source),
    declaration_sha256: hash(after.source.slice(node.start, node.end)), comment_sha256: hash(after.source.slice(comment.start, comment.end)),
    semantic_review: 'Separate substantive semantics review required; tags and parser attachment alone do not qualify it' });
}
for (const exported of after.tree.body.filter(node => node.type === 'ExportNamedDeclaration')) {
  const node = exported.declaration;
  if (node.type === 'ClassDeclaration') {
    item(node.id.name, 'class', node, exported);
    for (const method of node.body.body) item(node.id.name + '.' + method.key.name, 'method', method);
  } else if (node.type === 'FunctionDeclaration') item(node.id.name, 'function', node, exported);
  else if (node.type === 'VariableDeclaration') {
    for (const declaration of node.declarations) item(declaration.id.name, 'constant', declaration, exported);
  } else assert.fail('Uninventoried public actual declaration');
}
for (const comment of after.comments.filter(value => value.value.includes('@typedef'))) {
  for (const tag of ['@brief', '@details', '@see']) assert.ok(comment.value.includes(tag));
  const name = comment.value.match(/@typedef[^\n]*\b([A-Za-z_$][A-Za-z_0-9$]*)\s*$/m)?.[1];
  assert.ok(name, 'explicit named public JSDoc type required');
  inventory.push({ name, kind: 'JSDocType', line: comment.loc.start.line, source_sha256: hash(after.source),
    comment_sha256: hash(after.source.slice(comment.start, comment.end)) });
}
writeFileSync(output, JSON.stringify({ status: 'PASS_ACTUAL_ACORN_FORMAT_AND_API_ATTACHMENTS',
  before_sha256: hash(before.source), after_sha256: hash(after.source), tokens_sha256: hash(after.tokens),
  full_ast_sha256: hash(after.syntax), public_api_inventory: inventory, sdk_builds: 0, wasm_guest: 0, native_ros: 0 }, null, 2) + '\n');
