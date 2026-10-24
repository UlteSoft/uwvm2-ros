// SPDX-License-Identifier: Apache-2.0
// This is a host parser inventory, never a ROS bridge or Guest substitute.
import { readFileSync } from 'node:fs';
import { createHash } from 'node:crypto';
import { parse } from '/usr/share/nodejs/acorn/dist/acorn.mjs';

const inventory = [];
for (const file of process.argv.slice(2)) {
  const source = readFileSync(file, 'utf8');
  const comments = [];
  const tree = parse(source, { ecmaVersion: 2022, sourceType: 'module', locations: true, onComment: comments });
  const exports = [];
  for (const statement of tree.body) {
    if (!statement.type.startsWith('Export')) continue;
    if (statement.type !== 'ExportNamedDeclaration' || statement.declaration?.type !== 'FunctionDeclaration') {
      throw new Error(`Unreviewed exported declaration in ${file}:${statement.loc.start.line}`);
    }
    const declaration = statement.declaration;
    const preceding = comments.filter(comment => comment.end <= statement.start).at(-1);
    if (!preceding || preceding.type !== 'Block' || source.slice(preceding.end, statement.start).trim()) {
      throw new Error(`Missing attached public API documentation for ${declaration.id.name}`);
    }
    for (const tag of ['brief', 'details', 'see']) {
      if (!preceding.value.includes(`@${tag} `)) throw new Error(`Missing ${tag}: ${declaration.id.name}`);
    }
    const parameters = declaration.params.map(parameter => {
      if (parameter.type !== 'Identifier') throw new Error('Explicit review required for a new parameter pattern');
      const expression = new RegExp(`@param\\s+\\{[^}]+\\}\\s+${parameter.name}\\s+\\[in\\]`);
      if (!expression.test(preceding.value)) throw new Error(`Missing parameter contract: ${parameter.name}`);
      return parameter.name;
    });
    exports.push({ name: declaration.id.name, kind: 'function', line: statement.loc.start.line,
      parameters, attached_documentation_sha256: createHash('sha256').update(preceding.value).digest('hex'),
      semantic_review: 'See Task06 JS-MONITOR-API-REVIEW.md; tag presence alone is not qualification' });
  }
  const documentedTypes = comments.filter(comment => /@typedef\s/.test(comment.value)).map(comment => {
    const match = /@typedef\s+\{[^}]+\}\s+(\w+)/.exec(comment.value);
    if (!match || !['brief', 'details', 'see'].every(tag => comment.value.includes(`@${tag} `))) {
      throw new Error('Unreviewed JSDoc public type');
    }
    return { name: match[1], kind: 'JSDOC_TYPEDEF', line: comment.loc.start.line,
      declaration_basis: 'Type declaration is a parsed comment attached to this exact module; it is not a runtime export',
      documentation_sha256: createHash('sha256').update(comment.value).digest('hex') };
  });
  inventory.push({ path: file, sha256: createHash('sha256').update(source).digest('hex'), exports, documentedTypes });
}
process.stdout.write(`${JSON.stringify({ parser: 'Acorn', ecmaVersion: 2022, inventory }, null, 2)}\n`);
