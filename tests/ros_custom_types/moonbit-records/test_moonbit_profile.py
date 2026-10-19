"""Future Linux SOURCE tests of the actual emitter; NOT invoked by the author.

Caller supplies source-pinned modules/root and an existing owned empty scratch.
These are real source generations/refusals, not a successful transport model.
Actual MoonBit compile/test and Guest/native delivery are separate commands.
"""
from pathlib import Path
import hashlib
import json


def suite(moonbit, parent, bounded, *, source_root, scratch, guard):
    root, scratch = Path(source_root), Path(scratch)
    if not scratch.is_dir() or any(scratch.iterdir()) or scratch.resolve().is_relative_to(root.resolve()):
        raise ValueError('owned empty external scratch required')
    results = []
    def need(ok, why):
        if not ok:
            raise AssertionError(why)
    catalogs = [
        ('fixture', root/'tests/ros_custom_types/moonbit-records/fixture/catalog.preparation.json'),
        ('joint1', root/'tests/ros_custom_types/official-standard/ros1/catalog.preparation.json'),
        ('joint2', root/'tests/ros_custom_types/official-standard/ros2/catalog.preparation.json'),
        ('robot1', root/'sdk/robots/xarm/catalogs/ros1/catalog.preparation.json'),
        ('robot2', root/'sdk/robots/xarm/catalogs/ros2/catalog.preparation.json'),
        ('cio1', root/'sdk/robots/xarm/catalogs/cio_state/ros1/catalog.preparation.json'),
        ('cio2', root/'sdk/robots/xarm/catalogs/cio_state/ros2/catalog.preparation.json')]
    for role, catalog in catalogs:
        guard()
        original = catalog.read_bytes()
        records, ordered, interfaces, pins = bounded.load_catalog(parent, catalog, False)
        proof = moonbit.generate(parent, bounded, catalog, scratch/role)
        need(proof['source_inputs'] == pins and len(proof['records']) == len(ordered), 'complete actual IR closure')
        for actual, record in zip(proof['records'], ordered):
            need(all(actual[key] == value for key, value in {
                'symbol':record.symbol,'ros_version':record.version,'type':record.name,
                'adapter_codec_identity':record.codec_identity,'native_identity':record.identity,
                'minimum_wire_bytes':record.minimum,'maximum_wire_bytes':record.maximum,
                'complete_value_semantics':bounded.standard_semantics(record)}.items()), 'actual IR identity/bound/semantics')
        need(set(proof['outputs']) == moonbit.OUTPUTS, 'closed actual outputs')
        for name, pin in proof['outputs'].items():
            body = (scratch/role/name).read_bytes()
            need(pin == {'bytes':len(body),'sha256':hashlib.sha256(body).hexdigest()}, 'actual generated source receipt')
        need(catalog.read_bytes() == original and proof['native_registry_emitted'] is False and
             proof['compiler_execution'] == 'NOT_RUN', 'no source mutation or fabricated native/compiler qualification')
        results.append({'case':'actual-emission-'+role,'passed':True,'scope':'source/IR/output only'})
    # Corrupt a real already-resolved IR object in private scratch memory. The
    # policy must refuse rather than rename/omit an unrepresentable native field.
    records, ordered, interfaces, _ = bounded.load_catalog(parent, catalogs[0][1], False)
    field = next(record.members[0] for record in ordered if record.members)
    old = field.name
    try:
        for forbidden in ('fn', 'Uppercase', 'defer'):
            field.name = forbidden
            refused = False
            try:
                moonbit.checked_ir(parent, bounded, records, ordered, interfaces)
            except parent.GenerationError:
                refused = True
            need(refused, 'native name refusal, no silent rewrite')
    finally:
        field.name = old
    results.append({'case':'unrepresentable-native-field-refusal','passed':True,'scope':'source policy only'})
    # The old managed fixture deliberately contains native field `sealed`.
    # Keep it unchanged and require an explicit whole-schema MoonBit refusal.
    rejected = False
    try:
        moonbit.generate(parent, bounded,
            root/'tests/ros_custom_types/managed-records/fixture/catalog.preparation.json',
            scratch/'unrepresentable-original-managed-fixture')
    except parent.GenerationError:
        rejected = True
    need(rejected and not (scratch/'unrepresentable-original-managed-fixture').exists(),
         'reserved native name must refuse before output; no omission/projection')
    results.append({'case':'original-complete-reserved-field-catalog-refusal','passed':True,'scope':'source policy only'})
    return {'kind':'actual-moonbit-source-emitter-policy-results', 'cases':results,
            'compiler_runs':0,'Guest_runs':0,'native_deliveries':0}
