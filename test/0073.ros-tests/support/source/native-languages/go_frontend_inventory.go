// Command go_frontend_inventory collects parser-attached public Go contracts.
//
// Build and run only inside the original admitted finite owner, with the pinned
// official Go toolchain and an explicitly bounded private cache/output budget.
// This collector does not compile or execute a Guest, type-check a package,
// regenerate SDK source, format input or decide semantic documentation acceptance.
package main

import (
	"bytes"
	"crypto/sha256"
	"encoding/hex"
	"encoding/json"
	"flag"
	"fmt"
	"go/ast"
	"go/build"
	"go/doc"
	"go/parser"
	"go/printer"
	"go/token"
	"os"
	"path/filepath"
	"regexp"
	"runtime"
	"sort"
	"strings"
)

type parameter struct {
	Name string `json:"name"`
	Type string `json:"type"`
}

type declaration struct {
	Kind           string      `json:"kind"`
	Qualified      string      `json:"qualified_name"`
	File           string      `json:"file"`
	Line           int         `json:"line"`
	Column         int         `json:"column"`
	EndLine        int         `json:"end_line"`
	Source         string      `json:"exact_source_declaration"`
	Doc            string      `json:"native_attached_doc"`
	DocSHA256      string      `json:"native_attached_doc_sha256"`
	DocLine        int         `json:"native_attached_doc_line"`
	Parameters     []parameter `json:"actual_parameters"`
	TypeParameters []string    `json:"actual_type_parameters"`
	Results        []parameter `json:"actual_results"`
	Issues         []string    `json:"structural_contract_issues"`
	References     []reference `json:"see_references"`
}

type reference struct {
	Name   string `json:"name"`
	Status string `json:"status"`
	SHA256 string `json:"file_sha256,omitempty"`
}

type inputFile struct {
	Path     string `json:"path"`
	SHA256   string `json:"sha256"`
	Bytes    int    `json:"bytes"`
	Selected bool   `json:"selected_by_actual_go_build_context"`
}

type collector struct {
	set       *token.FileSet
	sources   map[string][]byte
	rows      []declaration
	callables map[string]bool
}

func hash(data []byte) string {
	sum := sha256.Sum256(data)
	return hex.EncodeToString(sum[:])
}

func spelling(set *token.FileSet, node any) string {
	var result bytes.Buffer
	if err := printer.Fprint(&result, set, node); err != nil {
		panic(err)
	}
	return result.String()
}

func fields(set *token.FileSet, list *ast.FieldList) []parameter {
	result := []parameter{}
	if list == nil {
		return result
	}
	for _, field := range list.List {
		kind := spelling(set, field.Type)
		if len(field.Names) == 0 {
			result = append(result, parameter{Name: "", Type: kind})
		}
		for _, name := range field.Names {
			result = append(result, parameter{Name: name.Name, Type: kind})
		}
	}
	return result
}

func typeNames(list *ast.FieldList) []string {
	result := []string{}
	if list != nil {
		for _, field := range list.List {
			for _, name := range field.Names {
				result = append(result, name.Name)
			}
		}
	}
	return result
}

func baseName(node ast.Expr) string {
	switch value := node.(type) {
	case *ast.Ident:
		return value.Name
	case *ast.StarExpr:
		return baseName(value.X)
	case *ast.IndexExpr:
		return baseName(value.X)
	case *ast.IndexListExpr:
		return baseName(value.X)
	case *ast.SelectorExpr:
		return value.Sel.Name
	}
	return ""
}

func attached(primary, group *ast.CommentGroup, groupSize int) *ast.CommentGroup {
	if primary != nil {
		return primary
	}
	if groupSize == 1 {
		return group
	}
	return nil
}

func (c *collector) add(kind, name string, node ast.Node, end token.Pos,
	comment *ast.CommentGroup, function *ast.FuncType, receiver *ast.FieldList,
	typeParameters *ast.FieldList) {
	start := c.set.PositionFor(node.Pos(), false)
	stop := c.set.PositionFor(end, false)
	source := c.sources[start.Filename]
	if stop.Filename != start.Filename || start.Offset < 0 || stop.Offset > len(source) || stop.Offset < start.Offset {
		panic("Parser source range does not bind its captured ordinary input")
	}
	text, docLine := "", 0
	if comment != nil {
		text = comment.Text()
		docLine = c.set.PositionFor(comment.Pos(), false).Line
	}
	row := declaration{
		Kind: kind, Qualified: name, File: filepath.Base(start.Filename),
		Line: start.Line, Column: start.Column, EndLine: stop.Line,
		Source: string(source[start.Offset:stop.Offset]), Doc: text,
		DocSHA256: hash([]byte(text)), DocLine: docLine,
		Parameters: []parameter{}, TypeParameters: typeNames(typeParameters),
		Results: []parameter{}, Issues: []string{}, References: []reference{},
	}
	if function != nil {
		row.Parameters = append(fields(c.set, receiver), fields(c.set, function.Params)...)
		row.Results = fields(c.set, function.Results)
		c.callables[name] = true
	}
	c.rows = append(c.rows, row)
}

func (c *collector) members(typeName string, list *ast.FieldList, kind string) {
	for _, field := range list.List {
		names := field.Names
		if len(names) == 0 {
			if name := baseName(field.Type); ast.IsExported(name) {
				names = []*ast.Ident{{Name: name}}
			}
		}
		comment := field.Doc
		if comment == nil {
			comment = field.Comment
		}
		function, _ := field.Type.(*ast.FuncType)
		for _, name := range names {
			if ast.IsExported(name.Name) {
				c.add(kind, typeName+"."+name.Name, field, field.End(), comment, function, nil, nil)
			}
		}
	}
}

func (c *collector) collect(file *ast.File) {
	for _, item := range file.Decls {
		switch value := item.(type) {
		case *ast.FuncDecl:
			if !ast.IsExported(value.Name.Name) {
				continue
			}
			name, kind := value.Name.Name, "function"
			if value.Recv != nil {
				owner := baseName(value.Recv.List[0].Type)
				if !ast.IsExported(owner) {
					continue
				}
				name, kind = owner+"."+name, "method"
			}
			end := value.End()
			if value.Body != nil {
				end = value.Body.Pos()
			}
			c.add(kind, name, value, end, value.Doc, value.Type, value.Recv, value.Type.TypeParams)
		case *ast.GenDecl:
			for _, specification := range value.Specs {
				switch spec := specification.(type) {
				case *ast.TypeSpec:
					if !ast.IsExported(spec.Name.Name) {
						continue
					}
					function, _ := spec.Type.(*ast.FuncType)
					c.add("type", spec.Name.Name, spec, spec.End(),
						attached(spec.Doc, value.Doc, len(value.Specs)), function, nil, spec.TypeParams)
					switch shape := spec.Type.(type) {
					case *ast.StructType:
						c.members(spec.Name.Name, shape.Fields, "field")
					case *ast.InterfaceType:
						c.members(spec.Name.Name, shape.Methods, "interface_method_or_embedding")
					}
				case *ast.ValueSpec:
					for _, name := range spec.Names {
						if ast.IsExported(name.Name) {
							c.add(value.Tok.String(), name.Name, spec, spec.End(),
								attached(spec.Doc, value.Doc, len(value.Specs)), nil, nil, nil)
						}
					}
				}
			}
		}
	}
}

var parameterTag = regexp.MustCompile(`(?m)^@param\[(?:in|out|in,out)\]\s+([A-Za-z_][A-Za-z0-9_]*)\b`)
var typeParameterTag = regexp.MustCompile(`(?m)^@tparam\s+([A-Za-z_][A-Za-z0-9_]*)\b`)
var seeTag = regexp.MustCompile(`(?m)^@see\s+([^\n]+)`)

func taggedNames(pattern *regexp.Regexp, text string) map[string]int {
	result := map[string]int{}
	for _, match := range pattern.FindAllStringSubmatch(text, -1) {
		result[match[1]]++
	}
	return result
}

func resolve(name, root string, symbols map[string]bool) reference {
	result := reference{Name: name, Status: "UNRESOLVED_REQUIRES_REVIEW"}
	if symbols[name] {
		result.Status = "ACTUAL_PUBLIC_AST_SYMBOL"
		return result
	}
	if root == "" || filepath.IsAbs(name) || !strings.Contains(name, "/") {
		return result
	}
	clean := filepath.Clean(name)
	if clean != name || clean == ".." || strings.HasPrefix(clean, ".."+string(filepath.Separator)) {
		return result
	}
	path := filepath.Join(root, clean)
	resolved, err := filepath.EvalSymlinks(path)
	if err != nil {
		return result
	}
	relative, err := filepath.Rel(root, resolved)
	if err != nil || relative == ".." || strings.HasPrefix(relative, ".."+string(filepath.Separator)) {
		return result
	}
	info, err := os.Stat(resolved)
	if err != nil || !info.Mode().IsRegular() || info.Size() > 1024*1024 {
		return result
	}
	data, err := os.ReadFile(resolved)
	if err == nil {
		result.Status, result.SHA256 = "EXACT_REFERENCE_FILE_IN_SELECTED_ROOT", hash(data)
	}
	return result
}

func (c *collector) audit(referenceRoot string) {
	symbols := map[string]bool{}
	for _, row := range c.rows {
		if symbols[row.Qualified] {
			panic("Duplicate actual public declaration in selected backend: " + row.Qualified)
		}
		symbols[row.Qualified] = true
	}
	for index := range c.rows {
		row := &c.rows[index]
		for _, tag := range []string{"@brief", "@details", "@see"} {
			if !strings.Contains(row.Doc, tag) {
				row.Issues = append(row.Issues, "missing_attached:"+tag)
			}
		}
		actual := map[string]bool{}
		documented := taggedNames(parameterTag, row.Doc)
		for position, item := range row.Parameters {
			if item.Name == "" {
				row.Issues = append(row.Issues, fmt.Sprintf("unnamed_actual_parameter_requires_manual_contract:%d", position+1))
				continue
			}
			actual[item.Name] = true
			if documented[item.Name] != 1 {
				row.Issues = append(row.Issues, "exact_parameter_contract:"+item.Name)
			}
		}
		for name := range documented {
			if !actual[name] {
				row.Issues = append(row.Issues, "unknown_parameter_contract:"+name)
			}
		}
		if c.callables[row.Qualified] && len(row.Results) != 0 && !strings.Contains(row.Doc, "@return") {
			row.Issues = append(row.Issues, "missing_actual_return_contract")
		}
		types := taggedNames(typeParameterTag, row.Doc)
		actualTypes := map[string]bool{}
		for _, name := range row.TypeParameters {
			actualTypes[name] = true
			if types[name] != 1 {
				row.Issues = append(row.Issues, "exact_type_parameter_contract:"+name)
			}
		}
		for name := range types {
			if !actualTypes[name] {
				row.Issues = append(row.Issues, "unknown_type_parameter_contract:"+name)
			}
		}
		for _, match := range seeTag.FindAllStringSubmatch(row.Doc, -1) {
			for _, name := range strings.Split(match[1], ";") {
				ref := resolve(strings.TrimSpace(name), referenceRoot, symbols)
				row.References = append(row.References, ref)
				if ref.Status == "UNRESOLVED_REQUIRES_REVIEW" {
					row.Issues = append(row.Issues, "unresolved_see:"+ref.Name)
				}
			}
		}
		sort.Strings(row.Issues)
	}
}

func run() error {
	directory := flag.String("package", "", "Pinned ordinary package directory; read only")
	backend := flag.String("backend", "", "Exact ros1 or ros2 build selection")
	referenceRoot := flag.String("reference-root", "", "Optional pinned installed SDK for external documentation references")
	output := flag.String("output", "", "New inventory JSON file inside the admitted current work directory")
	flag.Parse()
	if *directory == "" || (*backend != "ros1" && *backend != "ros2") || *output == "" || flag.NArg() != 0 {
		return fmt.Errorf("explicit package, one backend and new output are required")
	}
	root, err := filepath.EvalSymlinks(*directory)
	if err != nil {
		return err
	}
	root, err = filepath.Abs(root)
	if err != nil {
		return err
	}
	if *referenceRoot != "" {
		resolved, err := filepath.EvalSymlinks(*referenceRoot)
		if err != nil {
			return err
		}
		*referenceRoot, err = filepath.Abs(resolved)
		if err != nil {
			return err
		}
	}
	work, err := os.Getwd()
	if err != nil {
		return err
	}
	destination, err := filepath.Abs(*output)
	if err != nil {
		return err
	}
	parent, err := filepath.EvalSymlinks(filepath.Dir(destination))
	if err != nil {
		return err
	}
	parentRelative, err := filepath.Rel(work, parent)
	if err != nil || parentRelative == ".." || strings.HasPrefix(parentRelative, ".."+string(filepath.Separator)) {
		return fmt.Errorf("output must remain inside original admitted work")
	}
	for _, readOnlyRoot := range []string{root, *referenceRoot} {
		if readOnlyRoot == "" {
			continue
		}
		relative, err := filepath.Rel(readOnlyRoot, filepath.Join(parent, filepath.Base(destination)))
		if err == nil && relative != ".." && !strings.HasPrefix(relative, ".."+string(filepath.Separator)) {
			return fmt.Errorf("output cannot be placed inside a read-only input root")
		}
	}
	context := build.Default
	context.GOOS, context.GOARCH, context.Compiler, context.CgoEnabled = "wasip1", "wasm", "gc", false
	context.BuildTags = []string{*backend}
	context.ToolTags = nil // No ambient host architecture or instrumentation tags.
	entries, err := os.ReadDir(root)
	if err != nil {
		return err
	}
	c := collector{set: token.NewFileSet(), sources: map[string][]byte{}, rows: []declaration{}, callables: map[string]bool{}}
	inputs := []inputFile{}
	parsed := map[string]*ast.File{}
	packageName, total := "", 0
	for _, entry := range entries {
		name := entry.Name()
		if !strings.HasSuffix(name, ".go") || strings.HasSuffix(name, "_test.go") {
			continue
		}
		info, err := entry.Info()
		if err != nil || !info.Mode().IsRegular() || info.Size() > 1024*1024 {
			return fmt.Errorf("bounded ordinary Go source required: %s", name)
		}
		path := filepath.Join(root, name)
		data, err := os.ReadFile(path)
		if err != nil {
			return err
		}
		total += len(data)
		if total > 4*1024*1024 {
			return fmt.Errorf("captured one-package source bound exceeded")
		}
		selected, err := context.MatchFile(root, name)
		if err != nil {
			return err
		}
		inputs = append(inputs, inputFile{Path: name, SHA256: hash(data), Bytes: len(data), Selected: selected})
		if !selected {
			continue
		}
		file, err := parser.ParseFile(c.set, path, data, parser.ParseComments|parser.AllErrors)
		if err != nil {
			return err
		}
		if packageName != "" && packageName != file.Name.Name {
			return fmt.Errorf("selected source mixes distinct packages")
		}
		packageName = file.Name.Name
		c.sources[path], parsed[path] = data, file
		c.collect(file)
	}
	if len(parsed) == 0 {
		return fmt.Errorf("actual Go build context selected no package source")
	}
	c.audit(*referenceRoot)
	sort.Slice(c.rows, func(a, b int) bool { return c.rows[a].Qualified < c.rows[b].Qualified })
	// go/doc is an additional native documentation frontend, not an alternative
	// regex export inventory. Collect AST contracts first because go/doc filters ASTs.
	packageDoc := doc.New(&ast.Package{Name: packageName, Files: parsed}, packageName, doc.AllMethods)
	front := map[string]any{"package_doc": packageDoc.Doc, "functions": []string{}, "types": []string{}, "methods": []string{}, "values": []string{}}
	for _, item := range packageDoc.Funcs {
		front["functions"] = append(front["functions"].([]string), item.Name)
	}
	for _, item := range packageDoc.Types {
		front["types"] = append(front["types"].([]string), item.Name)
		for _, function := range item.Funcs {
			front["functions"] = append(front["functions"].([]string), function.Name)
		}
		for _, method := range item.Methods {
			front["methods"] = append(front["methods"].([]string), item.Name+"."+method.Name)
		}
		for _, value := range append(item.Consts, item.Vars...) {
			front["values"] = append(front["values"].([]string), value.Names...)
		}
	}
	for _, item := range append(packageDoc.Consts, packageDoc.Vars...) {
		front["values"] = append(front["values"].([]string), item.Names...)
	}
	for _, input := range inputs {
		data, err := os.ReadFile(filepath.Join(root, input.Path))
		if err != nil || hash(data) != input.SHA256 {
			return fmt.Errorf("read-only source changed during parser inventory: %s", input.Path)
		}
	}
	report := map[string]any{
		"kind": "task03-official-Go-parser-attached-contract-inventory-v1",
		"status": "INVENTORY_ONLY_SEMANTIC_REVIEW_REQUIRED", "actual_go_runtime": runtime.Version(),
		"parser": "official go/parser and go/ast with ParseComments; actual go/doc package frontend",
		"selected_GOOS": context.GOOS, "selected_GOARCH": context.GOARCH, "selected_backend": *backend,
		"actual_go_release_tags": context.ReleaseTags, "explicit_build_tags": context.BuildTags,
		"package_source": root, "reference_root": *referenceRoot, "source_inputs": inputs,
		"inputs_unchanged": true, "public_declarations": c.rows, "native_go_doc": front,
		"documentation_acceptance": false, "semantic_content_review_required": true,
		"formatter_runs": 0, "Guest_runs": 0, "package_type_checks": 0, "default_authority": []string{},
	}
	file, err := os.OpenFile(destination, os.O_WRONLY|os.O_CREATE|os.O_EXCL, 0600)
	if err != nil {
		return err
	}
	defer file.Close()
	encoder := json.NewEncoder(file)
	encoder.SetIndent("", "  ")
	return encoder.Encode(report)
}

func main() {
	if err := run(); err != nil {
		fmt.Fprintln(os.Stderr, err)
		os.Exit(1)
	}
}
