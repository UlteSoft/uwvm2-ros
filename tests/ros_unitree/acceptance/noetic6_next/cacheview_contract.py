"""Source fragment embedded in both r6d metadata owners by the sealed factory.

This fragment uses the existing owner's read/parse/need/Path/os/stat interface.
It is not imported from an ambient path. The package contains its literal code
inside the reviewed owner, so its receipt and current facts are checked before
the unchanged producer is allowed to start. No failed status is converted.
"""

CACHE_VIEW = M / 'noetic-amd64-native-sdk-cacheview-r6d'
R6C = M / 'noetic-amd64-native-sdk-acquisition-r6c'
CACHE_PREPARER_SHA = '816acab660e49ed2e4193ca39d4a195db8da8c0b3af1ea766ed372f3dfaf8258'
CACHE_PARTIAL_SHA = 'c8cf4f99beed03466a108ead46b2c0c559df4c6a37448b9dfa3c547cb6c0ff2e'


def cache_pin(value, maximum=2 * GIB):
    need(type(value) is dict and set(value) == PIN and type(value['path']) is str and
         Path(value['path']).is_absolute() and str(Path(value['path'])) == value['path'] and
         '..' not in Path(value['path']).parts and not any(c in value['path'] for c in ('\0', '\n', '\r')) and
         type(value['sha256']) is str and len(value['sha256']) == 64 and all(c in '0123456789abcdef' for c in value['sha256']) and
         all(type(value[k]) is int and value[k] >= 0 for k in PIN - {'path', 'sha256'}) and
         value['inode'] > 0 and value['bytes'] <= maximum, 'bounded literal current cache full7 required')
    return value


def cache_directory(path):
    """Open the complete absolute spelling with no followed parent links."""
    path = Path(path)
    need(path.is_absolute() and str(path) != '/' and '..' not in path.parts, 'absolute finite cache directory required')
    fd = os.open('/', os.O_RDONLY | os.O_DIRECTORY | os.O_NOFOLLOW | os.O_CLOEXEC)
    try:
        for name in path.parts[1:]:
            child = os.open(name, os.O_RDONLY | os.O_DIRECTORY | os.O_NOFOLLOW | os.O_CLOEXEC, dir_fd=fd)
            os.close(fd)
            fd = child
        return fd
    except BaseException:
        os.close(fd)
        raise


def cache_facts(info):
    return [info.st_dev, info.st_ino, info.st_mode, info.st_size,
            info.st_mtime_ns, info.st_ctime_ns, info.st_nlink]


def cache_names(fd, guard):
    """Keep directory enumeration independent of an owner's held FD cursor."""
    guard()
    before = cache_facts(os.fstat(fd))
    reader = os.open('.', os.O_RDONLY | os.O_DIRECTORY | os.O_NOFOLLOW | os.O_CLOEXEC, dir_fd=fd)
    try:
        need(before == cache_facts(os.fstat(reader)), 'fresh cache directory reader differs')
        result = []
        with os.scandir(reader) as entries:
            for entry in entries:
                need(len(result) < 100 and 0 < len(entry.name) <= 255 and entry.name not in ('.', '..') and
                     not any(c in entry.name for c in ('\0', '\n', '\r')), 'bounded cache name list required')
                result.append(entry.name)
        need(before == cache_facts(os.fstat(reader)) == cache_facts(os.fstat(fd)), 'cache directory facts changed')
        return sorted(result)
    finally:
        os.close(reader)


def cache_tree(snapshot, root, expected_files, remember, guard, *, sealed):
    """Require every current directory and file to match the prepared catalog.

    This is a complete finite tree comparison, not a pathname/digest selection.
    Empty, extra, renamed, linked, or mounted siblings are refused. The whole
    original42 tree is checked separately from the41 independent CoW clones.
    """
    need(type(snapshot) is dict and set(snapshot) == {'root', 'root_facts', 'entries', 'files', 'directories', 'sha256', 'rows'} and
         snapshot['root'] == str(root) and type(snapshot['rows']) is list and len(snapshot['rows']) <= 100,
         'closed bounded complete cache catalog required')
    catalog, wanted = snapshot['rows'], {}
    for row in catalog:
        need(type(row) is dict and set(row) == ({'relative', 'kind', 'facts'} if row.get('kind') == 'directory' else
             {'relative', 'kind', 'facts', 'file'}) and row['kind'] in ('file', 'directory') and
             type(row['relative']) is str and row['relative'] not in wanted and
             (row['relative'] == '' or (not Path(row['relative']).is_absolute() and '..' not in Path(row['relative']).parts and
              str(Path(row['relative'])) == row['relative'])) and
             type(row['facts']) is list and len(row['facts']) == 7 and all(type(v) is int for v in row['facts']),
             'unique literal full-fact cache row required')
        wanted[row['relative']] = row
    need({name for name, row in wanted.items() if row['kind'] == 'file'} == set(expected_files) and
         {name for name, row in wanted.items() if row['kind'] == 'directory'} ==
         {''} | {str(Path(name).parent) for name in expected_files}, 'exact cache paths including directories required')
    fd = cache_directory(root)
    root_info = os.fstat(fd)
    seen = set()

    def walk(held, relative, depth):
        guard()
        need(depth <= 2 and relative in wanted and relative not in seen, 'cache depth/name differs')
        seen.add(relative)
        row = wanted[relative]
        initial = os.fstat(held)
        need(row['kind'] == 'directory' and cache_facts(initial) == row['facts'] and
             initial.st_dev == root_info.st_dev and (not sealed or stat.S_IMODE(initial.st_mode) == 0o555),
             'prepared cache directory identity/mode changed')
        first = cache_names(held, guard)
        for name in first:
            rel = name if not relative else relative + '/' + name
            need(rel in wanted and rel not in seen, 'unlisted current cache entry')
            info = os.stat(name, dir_fd=held, follow_symlinks=False)
            entry = wanted[rel]
            need(cache_facts(info) == entry['facts'] and info.st_dev == root_info.st_dev, 'cache entry facts/mount identity changed')
            if entry['kind'] == 'directory':
                child = os.open(name, os.O_RDONLY | os.O_DIRECTORY | os.O_NOFOLLOW | os.O_CLOEXEC, dir_fd=held)
                try:
                    walk(child, rel, depth + 1)
                    need(cache_facts(os.fstat(child)) == cache_facts(os.stat(name, dir_fd=held, follow_symlinks=False)), 'cache child name changed')
                finally:
                    os.close(child)
            else:
                seen.add(rel)
                need(stat.S_ISREG(info.st_mode) and info.st_nlink == 1 and
                     (not sealed or stat.S_IMODE(info.st_mode) == 0o444) and
                     cache_pin(entry['file'], GIB) == expected_files[rel] and
                     entry['file']['path'] == str(root / rel), 'ordinary single-link cache file differs')
                remember(entry['file'])
                need(cache_facts(info) == cache_facts(os.stat(name, dir_fd=held, follow_symlinks=False)), 'cache file changed after hash')
        need(first == cache_names(held, guard) and cache_facts(initial) == cache_facts(os.fstat(held)), 'whole cache directory changed')

    try:
        walk(fd, '', 0)
        reopened = cache_directory(root)
        try:
            need(cache_facts(root_info) == snapshot['root_facts'] == cache_facts(os.fstat(reopened)), 'absolute cache root changed')
        finally:
            os.close(reopened)
    finally:
        os.close(fd)
    need(seen == set(wanted) and snapshot['entries'] == len(wanted) and snapshot['files'] == len(expected_files) and
         snapshot['directories'] == len(wanted) - len(expected_files) and snapshot['sha256'] ==
         hashlib.sha256(b''.join((json.dumps(row, sort_keys=True, separators=(',', ':')) + '\n').encode() for row in catalog)).hexdigest(),
         'complete cache catalog count/digest differs')


def admit_cache_view(expected, remember, guard):
    """Bind the actual content-only receipt; fresh package authority stays later."""
    def document(pin, path):
        cache_pin(pin, 4 * MIB)
        need(pin['path'] == str(path), 'exact cache-view document path required')
        remember(pin)
        body, actual = read(path, 4 * MIB)
        need(actual == pin, 'opened cache-view document differs from selected current pin')
        return parse(body)

    value = document(expected, CACHE_VIEW / 'report.json')
    need(value['kind'] == 'noetic-r6d-finite-CoW-DEB-candidate-view' and
         value['status'] == 'PREPARED_COW_DEB_CANDIDATES_ONLY_NOT_NEW_SIGNATURE_AUTHORITY' and
         value['root'] == str(CACHE_VIEW / 'debs') and value['registered_candidates'] == value['reflinks_used'] == 41 and
         value['actors_started'] == 0 and value['cleanup'] == {'status': 'PASS_NO_ACTORS', 'remaining': []} and value['errors'] == [] and
         all(value[k] is False for k in ('network_executed', 'signature_authority_reestablished', 'compiler_executed',
             'package_extraction_executed', 'target_program_executed', 'trim_executed', 'old_failed_status_promoted',
             'old_inputs_modified', 'ordinary_copy_fallback')) and
         all(value[k] is True for k in ('source_preserved', 'inputs_preserved', 'products_preserved')) and
         value['preparer']['sha256'] == CACHE_PREPARER_SHA and value['source_before'] == value['source_after'] and
         value['source_before']['root'] == str(S) and value['source_before']['files'] == 17882 and
         value['requirement'] == 'FRESH_SIGNED513_CLOSURE_AND_CONTROLS_AND_ALL513_EXTRACTION_AND_FINAL_TREE_REQUIRED',
         'genuine finite candidate-only preparation scope required')
    # The source snapshot uses its complete MF-bound streaming digest. Its root
    # and file count are checked above; MF35 itself remains a separate owner pin.
    remember(cache_pin(value['preparer'], MIB))
    remember(cache_pin(value['control'], MIB))
    sources = value['source_reports']
    need(type(sources) is dict and set(sources) == {'inner', 'outer'} and
         (sources['inner']['bytes'], sources['inner']['sha256']) ==
         (1048328, 'dae93ed7054d73e483221317bd131de300dea7cbb1fa63d308835c1f74592940') and
         (sources['outer']['bytes'], sources['outer']['sha256']) ==
         (58936, '23d3ab88e91faae9f9622f6246b54ad17865fd91fa6c20aaa345d2ea285160e9'), 'exact genuine failed r6c raw provenance required')
    inner = document(sources['inner'], R6C / 'report.json')
    outer = document(sources['outer'], M / 'noetic-amd64-native-sdk-acquisition-root-supervisor-r6c/report.json')
    need(inner['status'] == 'FAILED_ACQUISITION' and inner['packages'] == [] and outer['status'] == 'FAIL' and
         outer['actual_terminal_or_interrupted_inner_report'] == sources['inner'], 'old cancelled reports remain failed')
    selected = {}
    for pin in inner['product_pins']:
        cache_pin(pin)
        path = Path(pin['path'])
        if path.is_relative_to(R6C / 'model-DEBs'):
            relative = str(path.relative_to(R6C / 'model-DEBs'))
            need(relative not in selected, 'repeated old registered cache pin')
            selected[relative] = pin
    candidates = {}
    need(type(value['candidate_pins']) is list and len(value['candidate_pins']) == 41 and len(selected) == 41, 'exact41 current cloned and original candidate pins required')
    for pin in value['candidate_pins']:
        cache_pin(pin, GIB)
        path = Path(pin['path'])
        need(path.is_relative_to(CACHE_VIEW / 'debs'), 'candidate outside prepared view')
        relative = str(path.relative_to(CACHE_VIEW / 'debs'))
        parts = relative.split('/')
        need(len(parts) == 2 and len(parts[0]) == 5 and parts[0].isdigit() and parts[1].endswith('.deb') and
             relative in selected and relative not in candidates and 0 < pin['bytes'] <= GIB and
             (pin['bytes'], pin['sha256'], pin['device']) == (selected[relative]['bytes'], selected[relative]['sha256'], selected[relative]['device']) and
             pin['inode'] != selected[relative]['inode'], 'independent-inode full-byte candidate relation differs')
        candidates[relative] = pin
    need(set(candidates) == set(selected) and {name.split('/')[0] for name in selected} == {'%05d' % i for i in range(330, 371)}, 'exact registered ordinal330..370 selection required')
    need(type(value['excluded_original_files']) is list and len(value['excluded_original_files']) == 1, 'exactly one original excluded partial required')
    partial = cache_pin(value['excluded_original_files'][0], GIB)
    need(partial['path'] == str(R6C / 'model-DEBs/00371/libboost-graph1.71-dev_1.71.0-6ubuntu6_amd64.deb') and
         (partial['bytes'], partial['sha256']) == (87900, CACHE_PARTIAL_SHA) and value['historical_signed_partial'] ==
         {'bytes': 317296, 'sha256': 'c4c342d737ec15ca5b2e8f1e29a50f7c84b777b7634020b8127d08fab8899ac0'},
         'old truncated file excluded only from new view')
    all_old = {**selected, str(Path(partial['path']).relative_to(R6C / 'model-DEBs')): partial}
    before = document(value['source_catalog_before'], CACHE_VIEW / 'source42-before.json')
    after = document(value['source_catalog_after'], CACHE_VIEW / 'source42-after.json')
    view = document(value['view_catalog'], CACHE_VIEW / 'view41.json')
    ledger = document(value['clone_ledger'], CACHE_VIEW / 'clones41.json')
    need(before == after and type(ledger) is list and len(ledger) == 41, 'original42 catalogs or finite clone ledger differs')
    seen = set()
    for row in ledger:
        need(type(row) is dict and set(row) == {'relative', 'source_current', 'view_current', 'clone'} and
             row['relative'] in candidates and row['relative'] not in seen and row['source_current'] == selected[row['relative']] and
             row['view_current'] == candidates[row['relative']] and row['clone']['kind'] ==
             'same-filesystem-independent-inode-FICLONE-full-read-validation' and row['clone']['ioctl_performed'] is True and
             row['clone']['ordinary_copy_fallback'] is False, 'actual finite CoW clone evidence differs')
        seen.add(row['relative'])
    cache_tree(before, R6C / 'model-DEBs', all_old, remember, guard, sealed=False)
    cache_tree(view, CACHE_VIEW / 'debs', candidates, remember, guard, sealed=True)
    return {'receipt': expected, 'source_reports': sources, 'source_catalog': value['source_catalog_before'],
            'view_catalog': value['view_catalog'], 'candidates': list(candidates.values()),
            'scope': 'CONTENT_CANDIDATES_ONLY_FRESH513_SIGNATURES_AND_ALL513_EXTRACTION_REQUIRED'}
