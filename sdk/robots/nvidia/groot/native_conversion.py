"""Complete typed ROS values <-> actual GR00T N1.7 policy values.

No SDK import, file, network, model or CUDA operation occurs at module load.
NumPy and actual generated ROS classes are explicitly supplied by the trusted
native owner. Application nonce is never labelled a native RMW request ID.
"""
import math
import numbers


class Refused(ValueError):
    def __init__(self, code, reason):
        super().__init__(reason)
        self.code = code


def require(ok, code, reason):
    if not ok:
        raise Refused(code, reason)


def text(value, maximum):
    require(type(value) is str, 22, 'actual text required')
    try:
        raw = value.encode('utf-8', 'strict')
    except UnicodeError as error:
        raise Refused(22, 'invalid Unicode') from error
    require(b'\0' not in raw, 22, 'canonical text forbids NUL')
    require(len(raw) <= maximum, 90, 'complete text exceeds selected quota')
    return value


def shape(values, rank, maximum):
    require(len(values) == rank, 22, 'complete fixed shape rank required')
    require(all(isinstance(x,numbers.Integral) and not isinstance(x,bool) and 0 < x <= 0xffffffff for x in values),
            22, 'positive exact unsigned dimensions required')
    result = tuple(int(x) for x in values)
    count = 1
    for extent in result:
        require(count <= maximum // extent, 90, 'complete tensor exceeds selected quota')
        count *= extent
    return result, count


def selected_modalities(policy):
    """Take the real model configuration once; never invent key/horizon defaults."""
    raw = policy.get_modality_config()
    require(type(raw) is dict and set(raw) == {'video', 'state', 'language', 'action'},
            95, 'selected complete policy modality set unsupported')
    limits = {'video':3, 'state':4, 'language':2, 'action':8}
    result = {}
    for name, limit in limits.items():
        source = raw[name]
        keys = tuple(source.modality_keys)
        require(0 < len(keys) <= limit and len(set(keys)) == len(keys), 95,
                'selected modality key inventory exceeds complete bridge')
        for key in keys:
            text(key, 64)
        delta = tuple(source.delta_indices)
        require(delta and all(isinstance(x,numbers.Integral) and not isinstance(x,bool) for x in delta), 95, 'actual temporal configuration required')
        if name == 'language':
            require(len(delta) == 1, 95, 'actual SDK language path requires one timestep')
        result[name] = (keys, len(delta))
    return result


def keyed(rows, expected, maximum):
    require(len(rows) <= maximum, 90, 'too many complete keyed values')
    result = {}
    for row in rows:
        key = text(row.key, 64)
        require(key not in result, 22, 'duplicate keyed value')
        result[key] = row
    require(set(result) == set(expected), 22, 'missing or extra full model modality key')
    return result


def observation(request, configuration, np):
    """Allocate all private owned model inputs before exactly one inference."""
    require(type(request.application_nonce) is int and 0 < request.application_nonce < 2**64,
            22, 'positive application nonce required')
    video = keyed(request.video, configuration['video'][0], 3)
    state = keyed(request.state, configuration['state'][0], 4)
    language = keyed(request.language, configuration['language'][0], 2)
    result = {'video':{}, 'state':{}, 'language':{}}
    for key, row in video.items():
        dims, count = shape(row.shape, 5, 768)
        require(dims[0] == 1 and dims[1] == configuration['video'][1] and dims[4] == 3,
                22, 'actual B/T/RGB dimensions differ')
        require(len(row.pixels) == count and all(isinstance(x,numbers.Integral) and not isinstance(x,bool) and 0 <= x <= 255 for x in row.pixels),
                22, 'complete uint8 pixel extent required')
        result['video'][key] = np.array(row.pixels, dtype=np.uint8).reshape(dims).copy()
    for key, row in state.items():
        dims, count = shape(row.shape, 3, 320)
        require(dims[0] == 1 and dims[1] == configuration['state'][1] and len(row.values) == count,
                22, 'complete state B/T/D extent differs')
        require(all(not isinstance(x,bool) and isinstance(x,numbers.Real) and math.isfinite(x) for x in row.values),
                22, 'finite full state required')
        value = np.array(row.values, dtype=np.float32).reshape(dims).copy()
        require(bool(np.isfinite(value).all()), 22, 'state float32 overflow')
        result['state'][key] = value
    for key, row in language.items():
        dims, count = shape(row.shape, 2, 4)
        require(dims == (1,1) and len(row.texts) == count, 22, 'complete actual language B/T shape differs')
        result['language'][key] = [[text(row.texts[0],256)]]
    return result


def prepared_response(nonce, action, info, configuration, np, response_type, tensor_type):
    """Validate/copy EVERY returned action before publishing any response.

    Fixed genuine Gr00tPolicy._get_action returns empty info. A new nonempty
    info domain is explicitly unsupported95, never erased or projected. No
    nonempty/NaN/oversized array is narrowed to make a partial success.
    """
    require(type(info) is dict and not info, 95, 'nonempty actual info needs a separately typed extension')
    require(type(action) is dict and set(action) == set(configuration['action'][0]),
            95, 'actual complete action key set differs')
    output = response_type()
    output.status = 0
    output.application_nonce = nonce
    output.info_is_empty = True
    rows = []
    for key in configuration['action'][0]:
        value = action[key]
        require(isinstance(value,np.ndarray) and value.dtype == np.float32 and value.ndim == 3,
                95, 'actual float32 B/T/D action required')
        dims, count = shape(tuple(int(x) for x in value.shape),3,320)
        require(dims[0] == 1 and dims[1] == configuration['action'][1] and count == value.size,
                22, 'actual complete action horizon differs')
        require(bool(np.isfinite(value).all()), 22, 'nonfinite actual action refused')
        row = tensor_type()
        row.key = text(key,64)
        row.shape = list(dims)
        row.values = value.reshape(-1).tolist()  # all values, no stride/pointer exposure
        rows.append(row)
    require(len(rows) <= 8, 90, 'whole action result quota')
    output.action = rows
    return output
