"""Check NVIDIA GR00T's numerical preprocessing on its recorded LIBERO episode."""
import argparse
from dataclasses import replace
import hashlib
import json
import os
from pathlib import Path
import sys

from run_scene_vm import verify_gate


REVISION = 'dac54b313160c201c450f97df0fd74ebd379c892'
EPISODE_SHA = '2a593521d58a1e6b3c27d6d91641c2d4c8a12422cc4534d9bfab49b01e532e68'
SOURCES_SHA = '7216b4fb23ec39418c53e3107c3ccb60ab4b731372b946b73572e2f7696c8d4c'
MODALITIES = Path('demo_data/libero_demo/meta/modality.json')
STATISTICS = Path('tests/fixtures/processor_config/statistics.json')


def sha(path):
    return hashlib.sha256(path.read_bytes()).digest()


def verify_sources(root):
    digest = hashlib.sha256()
    files = [*(root / 'gr00t').rglob('*.py'), root / MODALITIES, root / STATISTICS]
    for path in sorted(files):
        digest.update(path.relative_to(root).as_posix().encode() + b'\0' + sha(path))
    if digest.hexdigest() != SOURCES_SHA:
        raise ValueError('GR00T sources, modalities or fixture statistics differ from the pinned revision')


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--upstream', type=Path, required=True)
    parser.add_argument('--episode', type=Path, required=True)
    parser.add_argument('--output', type=Path, required=True)
    args = parser.parse_args()
    if not __debug__:
        raise RuntimeError('Qualification requires Python assertions')
    verify_gate()
    root = args.upstream.resolve(strict=True)
    verify_sources(root)
    assert sha(args.episode).hex() == EPISODE_SHA
    sys.path.insert(0, str(root))
    import numpy as np
    import pyarrow as pa
    import pyarrow.parquet as parquet
    import scipy
    from gr00t.configs.data.embodiment_configs import MODALITY_CONFIGS
    from gr00t.data.state_action.state_action_processor import StateActionProcessor
    from gr00t.eval._horizon_contract import PolicyHorizonSpec

    versions = [np.__version__, scipy.__version__, pa.__version__]
    assert sys.version_info[:2] == (3, 12) and versions == ['1.26.4', '1.15.3', '23.0.1']
    imported = [Path(module.__file__).resolve() for name, module in sys.modules.items()
                if (name == 'gr00t' or name.startswith('gr00t.')) and getattr(module, '__file__', None)]
    assert imported and all(path.is_relative_to(root / 'gr00t') for path in imported)
    frames = parquet.ParquetFile(args.episode).read(columns=['observation.state', 'action', 'frame_index'])
    count = frames.num_rows
    assert count >= 16 and frames['frame_index'].to_pylist() == list(range(count))
    spec = json.loads((root / MODALITIES).read_text())
    stats = json.loads((root / STATISTICS).read_text())
    tag = 'libero_sim'
    config = MODALITY_CONFIGS[tag]
    raw = {}
    for kind, column, width in [('state', 'observation.state', 8), ('action', 'action', 7)]:
        values = np.asarray(frames[column].to_pylist(), dtype=np.float32)
        assert values.shape == (count, width) and np.isfinite(values).all()
        raw[kind] = {key: values[:, bounds['start']:bounds['end']] for key, bounds in spec[kind].items()}
        assert set(raw[kind]) == set(config[kind].modality_keys)

    processor = StateActionProcessor({tag: config}, stats, clip_outliers=True)
    processor.eval()
    processed = processor.apply(raw['state'], raw['action'], tag)
    restored = processor.unapply(*processed, tag, raw_state=raw['state'])
    errors = {}
    for kind, normalized, decoded in zip(('state', 'action'), processed, restored, strict=True):
        largest = [0., 0.]
        for key, original in raw[kind].items():
            bounds = stats[tag][kind][key]
            low, high = np.asarray(bounds['min']), np.asarray(bounds['max'])
            assert low.shape == high.shape == (original.shape[1],) and np.all(high > low)
            expected = np.clip(2. * (original - low) / (high - low) - 1., -1., 1.)
            bounded = np.clip(original, low, high)
            for index, (actual, reference) in enumerate(((normalized[key], expected), (decoded[key], bounded))):
                np.testing.assert_allclose(actual, reference, atol=1.e-5, rtol=0)
                largest[index] = max(largest[index], float(np.max(np.abs(actual - reference))))
        errors[kind] = dict(zip(('normalized', 'decoded'), largest, strict=True))

    injected = {key: value[:1].copy() for key, value in raw['action'].items()}
    injected['yaw'][:] = np.asarray(stats[tag]['action']['yaw']['max']) + 1.
    clipped = processor.apply(raw['state'], injected, tag)[1]
    assert np.all(clipped['yaw'] == 1.)
    horizon = PolicyHorizonSpec.from_modality_config(config, n_action_steps=4)
    assert (horizon.n_action_steps, horizon.action_horizon, horizon.state_delta_indices,
            horizon.video_delta_indices) == (4, 16, (0,), (0,))
    missing = {key: value for key, value in raw['state'].items() if key != 'yaw'}
    sparse = dict(config, action=replace(config['action'], delta_indices=[0, 2, 4]))
    for check, exc, fragment in [
        (lambda: processor.apply(missing, raw['action'], tag), KeyError, 'yaw'),
        (lambda: PolicyHorizonSpec.from_modality_config(config, n_action_steps=0), ValueError, 'n_action_steps'),
        (lambda: PolicyHorizonSpec.from_modality_config(config, n_action_steps=17), ValueError, 'n_action_steps'),
        (lambda: PolicyHorizonSpec.from_modality_config(sparse), ValueError, 'contiguous'),
    ]:
        try:
            check()
        except exc as error:
            assert fragment in str(error)
        else:
            raise AssertionError(f'Invalid input accepted ({fragment})')

    result = dict(passed=True, scope='NVIDIA GR00T LIBERO preprocessing and execution horizon',
                  upstream_revision=REVISION, sources_sha256=SOURCES_SHA, episode_sha256=EPISODE_SHA,
                  cgroup=Path('/proc/self/cgroup').read_text().strip(), cpus=sorted(os.sched_getaffinity(0)),
                  python=sys.version, numpy=versions[0], scipy=versions[1], pyarrow=versions[2],
                  frames=count, state_width=8, action_width=7, max_absolute_errors=errors,
                  outlier_clipped=True, invalid_inputs_rejected=4, execution_steps=4, action_horizon=16,
                  vm_provider_exercised=False, checkpoint_inference_exercised=False)
    with args.output.open('x') as output:
        json.dump(result, output, indent=2, allow_nan=False)
        output.write('\n')
    print(json.dumps(result))


if __name__ == '__main__':
    main()
