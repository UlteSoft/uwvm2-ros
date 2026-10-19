"""Trusted bounded-v2 schema preparation and six-language source emission.

This module is called by sdk/tools/custom_types.py; it does not execute ROS
generators, load a native library, obtain a type identity or create a grant.
The finite profile preserves every declared field and explicitly bounds every
otherwise dynamic field. Official native type support must independently agree
with the emitted native schemas before a Gateway may create an entity.
"""
from __future__ import annotations

from dataclasses import dataclass, field
from pathlib import Path
import hashlib
import json
import re
import math
import struct

PROFILE = 'uwvm-ros-custom-bounded-v2'
STANDARD_PROFILE = 'uwvm-ros-official-standard-bounded-v1'
CONTROL_PROFILE = 'uwvm-ros-original-node-control-c-cpp-v1'
MAX_TYPES, MAX_NODES, MAX_TOTAL_NODES, MAX_WIRE, MAX_DEPTH = 128, 4096, 16384, 32768, 16


@dataclass
class Field:
    """One exact native field plus a stricter finite canonical capacity."""
    name: str
    type: str
    array: str = ''
    count: int = 0
    string_bound: int = 0
    native_string_bound: int = 0
    native_sequence_bound: int = 0
    # Exact normalized official TypeDescription literal; None means undeclared.
    # Scalar bool/integer/finite-float defaults are normalized from original
    # ROS2 declarations. Arrays/strings/nested defaults require their own grammar.
    native_default: str | None = None
    # Explicit control profile only: original ROS2 spelling (byte is IDL octet,
    # char is IDL uint8). Canonical storage remains unsigned eight-bit.
    native_scalar: str = ''
    # Explicit control-only canonical byte domain; the real native type remains string.
    string_encoding: str = 'utf8'


@dataclass
class Record:
    """Complete message/component value; no native projection is permitted."""
    version: int
    name: str
    members: list[Field]
    identity: str | None
    source: dict
    component: str = ''
    owner: str = ''
    kind: str = 'message'
    profile: str = PROFILE
    minimum: int = 0
    maximum: int = 0
    nodes: int = 0
    depth: int = 1
    codec_identity: str = ''
    has_native_defaults: bool = False
    # Explicit logical wire cap is distinct from full inline/cartesian capacity.
    aggregate_wire_bytes: int = 0
    cartesian_maximum: int = 0
    # Complete source constants are not instance fields and consume no wire bytes.
    # Non-control profiles keep this empty and retain their original identities.
    constants: list[dict] = field(default_factory=list)

    @property
    def symbol(self):
        return 'Ros' + str(self.version) + ''.join(p[:1].upper() + p[1:] for p in self.name.split('/'))

    @property
    def c_symbol(self):
        return 'roswasmc_bounded_' + str(self.version) + '_' + self.name.replace('/', '_')

    @property
    def native(self):
        if self.version == 1 and self.name in ('time', 'duration'):
            return '::ros::Time' if self.name == 'time' else '::ros::Duration'
        if self.component:
            if self.version == 1 and self.kind == 'action':
                return '::' + self.name.replace('/', '::')
            component = {'request': 'Request', 'response': 'Response', 'goal': 'Goal',
                         'result': 'Result', 'feedback': 'Feedback'}[self.component]
            return '::' + self.owner.replace('/', '::') + '::' + component
        return '::' + self.name.replace('/', '::')


@dataclass
class Interface:
    """One official message, service or action and its canonical components."""
    version: int
    name: str
    kind: str
    identity: str | None
    components: dict[str, Record] = field(default_factory=dict)
    codec_identity: str = ''
    registered: bool = True
    profile: str = PROFILE

    @property
    def symbol(self):
        return 'Ros' + str(self.version) + ''.join(p[:1].upper() + p[1:] for p in self.name.split('/'))


def native_name(parent, version, name, kind, standard=False):
    """Validate exact version/category names without allowing builtin shadowing."""
    if type(version) is not int or version not in (1, 2) or not isinstance(name, str):
        raise parent.GenerationError('bounded catalog requires ROS version1/2 and a native name')
    parts = name.split('/')
    if len(parts) != (2 if version == 1 else 3):
        raise parent.GenerationError('native type must contain its exact version-specific category')
    if version == 2 and parts[1] != {'message': 'msg', 'service': 'srv', 'action': 'action'}[kind]:
        raise parent.GenerationError('file category and native ROS2 type category differ')
    if standard and (version, name) in standard_names():
        if kind != 'message':
            raise parent.GenerationError('official standard dependency must be a complete Message')
        return name
    if standard and parts[0] in {'builtin','builtin_interfaces','std_msgs','std_srvs','geometry_msgs','sensor_msgs','rcl_interfaces','lifecycle_msgs','action_msgs','service_msgs','unique_identifier_msgs','actionlib_tutorials','action_tutorials_interfaces','nav_msgs'}:
        raise parent.GenerationError('standard namespace/type is outside the explicit official family: ' + name)
    parent.native_name(version, parts[0] + ('/msg/' if version == 2 else '/') + parts[-1])
    return name


def standard_names():
    """A finite official first family, never a reserved-package wildcard."""
    return {(1, 'std_msgs/Header'), (1, 'sensor_msgs/JointState'),
            (2, 'std_msgs/msg/Header'), (2, 'sensor_msgs/msg/JointState'),
            (2, 'builtin_interfaces/msg/Time'), (2, 'builtin_interfaces/msg/Duration')} | set(standard_extended_shapes())


def standard_extended_shapes():
    """Exact complete geometry/nav families, never an arbitrary namespace grant.

    Native ROS2 defaults remain subject to the separate complete-default
    implementation. This table never erases them from actual source input.
    Existing builtin Twist can appear as a dependency, never a new endpoint.
    """
    original = {
        'geometry_msgs/Accel': (('linear', 'geometry_msgs/Vector3', '', 0, 0, 0), ('angular', 'geometry_msgs/Vector3', '', 0, 0, 0)),
        'geometry_msgs/AccelStamped': (('header', 'std_msgs/Header', '', 0, 0, 0), ('accel', 'geometry_msgs/Accel', '', 0, 0, 0)),
        'geometry_msgs/AccelWithCovariance': (('accel', 'geometry_msgs/Accel', '', 0, 0, 0), ('covariance', 'float64', 'array', 36, 0, 0)),
        'geometry_msgs/AccelWithCovarianceStamped': (('header', 'std_msgs/Header', '', 0, 0, 0), ('accel', 'geometry_msgs/AccelWithCovariance', '', 0, 0, 0)),
        'geometry_msgs/Point': (('x', 'float64', '', 0, 0, 0), ('y', 'float64', '', 0, 0, 0), ('z', 'float64', '', 0, 0, 0)),
        'geometry_msgs/Point32': (('x', 'float32', '', 0, 0, 0), ('y', 'float32', '', 0, 0, 0), ('z', 'float32', '', 0, 0, 0)),
        'geometry_msgs/PointStamped': (('header', 'std_msgs/Header', '', 0, 0, 0), ('point', 'geometry_msgs/Point', '', 0, 0, 0)),
        'geometry_msgs/Polygon': (('points', 'geometry_msgs/Point32', 'sequence', 0, 0, 0),),
        'geometry_msgs/PolygonStamped': (('header', 'std_msgs/Header', '', 0, 0, 0), ('polygon', 'geometry_msgs/Polygon', '', 0, 0, 0)),
        'geometry_msgs/Pose': (('position', 'geometry_msgs/Point', '', 0, 0, 0), ('orientation', 'geometry_msgs/Quaternion', '', 0, 0, 0)),
        'geometry_msgs/PoseArray': (('header', 'std_msgs/Header', '', 0, 0, 0), ('poses', 'geometry_msgs/Pose', 'sequence', 0, 0, 0)),
        'geometry_msgs/PoseStamped': (('header', 'std_msgs/Header', '', 0, 0, 0), ('pose', 'geometry_msgs/Pose', '', 0, 0, 0)),
        'geometry_msgs/PoseWithCovariance': (('pose', 'geometry_msgs/Pose', '', 0, 0, 0), ('covariance', 'float64', 'array', 36, 0, 0)),
        'geometry_msgs/PoseWithCovarianceStamped': (('header', 'std_msgs/Header', '', 0, 0, 0), ('pose', 'geometry_msgs/PoseWithCovariance', '', 0, 0, 0)),
        'geometry_msgs/Quaternion': (('x', 'float64', '', 0, 0, 0), ('y', 'float64', '', 0, 0, 0), ('z', 'float64', '', 0, 0, 0), ('w', 'float64', '', 0, 0, 0)),
        'geometry_msgs/QuaternionStamped': (('header', 'std_msgs/Header', '', 0, 0, 0), ('quaternion', 'geometry_msgs/Quaternion', '', 0, 0, 0)),
        'geometry_msgs/Transform': (('translation', 'geometry_msgs/Vector3', '', 0, 0, 0), ('rotation', 'geometry_msgs/Quaternion', '', 0, 0, 0)),
        'geometry_msgs/TransformStamped': (('header', 'std_msgs/Header', '', 0, 0, 0), ('child_frame_id', 'string', '', 0, 0, 0), ('transform', 'geometry_msgs/Transform', '', 0, 0, 0)),
        'geometry_msgs/Twist': (('linear', 'geometry_msgs/Vector3', '', 0, 0, 0), ('angular', 'geometry_msgs/Vector3', '', 0, 0, 0)),
        'geometry_msgs/TwistStamped': (('header', 'std_msgs/Header', '', 0, 0, 0), ('twist', 'geometry_msgs/Twist', '', 0, 0, 0)),
        'geometry_msgs/TwistWithCovariance': (('twist', 'geometry_msgs/Twist', '', 0, 0, 0), ('covariance', 'float64', 'array', 36, 0, 0)),
        'geometry_msgs/TwistWithCovarianceStamped': (('header', 'std_msgs/Header', '', 0, 0, 0), ('twist', 'geometry_msgs/TwistWithCovariance', '', 0, 0, 0)),
        'geometry_msgs/Vector3': (('x', 'float64', '', 0, 0, 0), ('y', 'float64', '', 0, 0, 0), ('z', 'float64', '', 0, 0, 0)),
        'geometry_msgs/Vector3Stamped': (('header', 'std_msgs/Header', '', 0, 0, 0), ('vector', 'geometry_msgs/Vector3', '', 0, 0, 0)),
        'geometry_msgs/Wrench': (('force', 'geometry_msgs/Vector3', '', 0, 0, 0), ('torque', 'geometry_msgs/Vector3', '', 0, 0, 0)),
        'geometry_msgs/WrenchStamped': (('header', 'std_msgs/Header', '', 0, 0, 0), ('wrench', 'geometry_msgs/Wrench', '', 0, 0, 0)),
        'nav_msgs/Odometry': (('header', 'std_msgs/Header', '', 0, 0, 0), ('child_frame_id', 'string', '', 0, 0, 0), ('pose', 'geometry_msgs/PoseWithCovariance', '', 0, 0, 0), ('twist', 'geometry_msgs/TwistWithCovariance', '', 0, 0, 0)),
        'nav_msgs/Path': (('header', 'std_msgs/Header', '', 0, 0, 0), ('poses', 'geometry_msgs/PoseStamped', 'sequence', 0, 0, 0)),
    }
    result = {}
    for version in (1, 2):
        def qualified(name):
            if version == 1 or "/" not in name:
                return name
            package, leaf = name.split("/", 1)
            return package + "/msg/" + leaf
        for name, fields in original.items():
            result[(version, qualified(name))] = tuple((field, qualified(typ), shape, count, text, sequence)
                for field, typ, shape, count, text, sequence in fields)
    return result


def standard_topic_names():
    """Explicit new endpoint types; existing builtins/dependency-only types stay reserved."""
    return standard_names() - {(1, "std_msgs/Header"), (2, "std_msgs/msg/Header"),
        (2, "builtin_interfaces/msg/Time"), (2, "builtin_interfaces/msg/Duration"),
        (1, "geometry_msgs/Twist"), (2, "geometry_msgs/msg/Twist")}


def standard_reference(parent, version, package, spelling):
    """Preserve ROS1 primitive names and resolve only the official Header alias.

    No native field is renamed. ROS1 time/duration remain native ros::Time and
    ros::Duration, not fabricated message types or identities. ROS2 names keep
    their actual msg category. Every nonprimitive dependency remains catalogued.
    """
    if version == 1 and spelling in ('time', 'duration'):
        return spelling
    if version == 1 and spelling == 'Header':
        return 'std_msgs/Header'
    bits = spelling.split('/')
    if len(bits) == 1:
        name = package + ('/msg/' if version == 2 else '/') + spelling
    elif version == 2 and len(bits) == 2:
        name = bits[0] + '/msg/' + bits[1]
    else:
        name = spelling
    return native_name(parent, version, name, 'message', True)


def standard_semantics(record):
    if record.profile != STANDARD_PROFILE:
        return ''
    if record.version == 1 and record.name in ('time', 'duration'):
        return 'normalized-duration' if record.name == 'duration' else 'normalized-time'
    if record.version == 2 and record.name in ('builtin_interfaces/msg/Time', 'builtin_interfaces/msg/Duration'):
        return 'normalized-time'
    if record.name in ('sensor_msgs/JointState', 'sensor_msgs/msg/JointState'):
        return 'joint-state-optional-equal-length-arrays'
    return ''


def semantic_condition(record, style, value):
    """Return a complete-value invalidity predicate for the explicit mode only.

    This guards native conversion and every generated Guest codec. Existing
    custom-mode output has no added predicate or canonical identity change.
    """
    rule = standard_semantics(record)
    def field(name):
        if style == 'c': return value + '->' + name
        if style == 'go': return value + '.' + name[:1].upper() + name[1:]
        if style == 'js': return value + '[' + json.dumps(name) + ']'
        return value + '.' + name
    if rule.startswith('normalized-'):
        name = 'nanosec' if record.version == 2 else 'nsec'
        access = field(name)
        return (access + '<0||' if rule == 'normalized-duration' else '') + access + '>=1000000000'
    if rule == 'joint-state-optional-equal-length-arrays':
        def length(name):
            access = field(name)
            if style == 'c': return access + '.size'
            if style == 'native': return access + '.size()'
            if style == 'rust': return access + '.len()'
            if style == 'go': return 'len(' + access + ')'
            return access + '.length'
        sizes = [length(name) for name in ('name','position','velocity','effort')]
        return '||'.join('(' + a + '!=0&&' + b + '!=0&&' + a + '!=' + b + ')'
                        for i,a in enumerate(sizes) for b in sizes[i+1:])
    return ''


def standard_shape(parent, record):
    """Admit exact official names/fields before copying any native identity.

    Canonical capacities may narrow dynamic fields, but native declarations and
    original primitive spelling are unchanged. Constants and unreviewed defaults stay refused.
    """
    name = record.name
    if (record.version, name) not in standard_names():
        return
    def signature(member):
        return (member.name,member.type,member.array,member.count if member.array == 'array' else 0,
                member.native_string_bound,member.native_sequence_bound)
    scalar = lambda name,typ:(name,typ,'',0,0,0)
    seq = lambda name,typ:(name,typ,'sequence',0,0,0)
    extended = standard_extended_shapes().get((record.version, name))
    if extended is not None:
        expected = list(extended)
    elif name in ('std_msgs/Header','std_msgs/msg/Header'):
        expected = ([scalar('seq','uint32'),scalar('stamp','time'),scalar('frame_id','string')]
                    if record.version == 1 else
                    [scalar('stamp','builtin_interfaces/msg/Time'),scalar('frame_id','string')])
    elif name in ('sensor_msgs/JointState','sensor_msgs/msg/JointState'):
        expected = [scalar('header','std_msgs/Header' if record.version == 1 else 'std_msgs/msg/Header'),
                    seq('name','string'),seq('position','float64'),seq('velocity','float64'),seq('effort','float64')]
    else:
        expected = [scalar('sec','int32'),scalar('nanosec','uint32')]
    if [signature(member) for member in record.members] != expected:
        raise parent.GenerationError('official standard field order/type/native bounds differ: ' + name)
    defaults = [member.native_default for member in record.members]
    if (record.version, name) == (2, 'geometry_msgs/msg/Quaternion'):
        # This first native variant is the complete actual ROS2 default-bearing
        # Quaternion, never a source with its four defaults removed. ROS1 and
        # every unrelated/custom declaration retain their original contract.
        if defaults != ['0.0', '0.0', '0.0', '1.0']:
            raise parent.GenerationError('complete official ROS2 Quaternion defaults x/y/z0,w1 required')
    elif any(value is not None for value in defaults):
        raise parent.GenerationError('native defaults outside the explicit Quaternion family are refused')


def identity(parent, version, value, required, label):
    """Accept only caller-exported official identities, never compute a native hash."""
    pattern = r'[0-9a-f]{32}' if version == 1 else r'RIHS01_[0-9a-f]{64}'
    if value is not None and (not isinstance(value, str) or not re.fullmatch(pattern, value)):
        raise parent.GenerationError('invalid official native identity: ' + label)
    if required and value is None:
        raise parent.GenerationError('official native identity is required for native emission: ' + label)
    return value


def scalar_native_default(parent, type_, literal):
    """Normalize an original ROS2 scalar default without executing its text.

    The pinned original rosidl_adapter parses bool/int/float, its IDL adapter
    preserves that value, and serialize_field records Python str(value).
    Thus bool metadata is exactly True/False, not the C/Java spelling. Native
    RIHS deliberately excludes defaults: full actual TypeDescription comparison
    remains mandatory. This helper calculates no native identity or capability.
    """
    if type(literal) is not str or not literal or len(literal) > 128:
        raise parent.GenerationError('bounded scalar default literal required')
    if type_ == 'bool':
        word=literal.lower()
        if word not in ('true','false','0','1'):
            raise parent.GenerationError('ROS2 bool default must be true/false/0/1')
        return 'True' if word in ('true','1') else 'False'
    if re.fullmatch(r'u?int(?:8|16|32|64)', type_):
        if not re.fullmatch(r'[+-]?(?:[0-9]+|0[xX][0-9a-fA-F]+|0[oO][0-7]+|0[bB][01]+)', literal):
            raise parent.GenerationError('ROS2 integer default requires an exact integer literal')
        try:
            value=int(literal,10)
        except ValueError:
            value=int(literal,0)
        unsigned=type_.startswith('u');bits=int(type_[4 if unsigned else 3:])
        lower=0 if unsigned else -(1 << (bits-1));upper=(1 << (bits if unsigned else bits-1))-1
        if not lower <= value <= upper:
            raise parent.GenerationError('native scalar default exceeds the original integer width')
        return str(value)
    if type_ in ('float32','float64'):
        if not re.fullmatch(r'[+-]?(?:[0-9]+(?:\.[0-9]*)?|\.[0-9]+)(?:[eE][+-]?[0-9]+)?', literal):
            raise parent.GenerationError('finite decimal ROS2 floating default required')
        value=float(literal)
        if not math.isfinite(value):
            raise parent.GenerationError('nonfinite scalar default is outside the canonical profile')
        if type_ == 'float32':
            try:
                rounded=struct.unpack('<f',struct.pack('<f',value))[0]
            except (OverflowError,struct.error):
                raise parent.GenerationError('scalar default overflows the original float32') from None
            if not math.isfinite(rounded):
                raise parent.GenerationError('scalar default overflows the original float32')
        # Keep the original annotation value for full native metadata proof;
        # emitters separately round typed float32 initialization exactly once.
        return str(value)
    raise parent.GenerationError('declared array/string/nested default needs a complete supported grammar')


def scalar_default_expression(type_, normalized, language):
    """Emit a typed literal/expression from already admitted immutable IR.

    This function is NOT a second source parser. It refuses a corrupted/future
    normalized IR. Exact float bits avoid Java/C# decimal underflow rejection;
    Java uint64 defaults retain all64 bits, and JS uses BigInt for both64 types.
    Ordinary no-default records never enter this function.
    """
    if type(normalized) is not str or len(normalized)>128:
        raise ValueError('admitted normalized scalar default required')
    if type_=='bool':
        if normalized not in ('True','False'):raise ValueError('normalized bool default required')
        if language=='python':return normalized
        if language=='c':return '1' if normalized=='True' else '0'
        return normalized.lower()
    if re.fullmatch(r'u?int(?:8|16|32|64)',type_):
        if not re.fullmatch(r'-?(?:0|[1-9][0-9]*)',normalized):raise ValueError('normalized integer required')
        n=int(normalized);unsigned=type_.startswith('u');bits=int(type_[4 if unsigned else 3:])
        if not (0 if unsigned else -(1<<(bits-1)))<=n<=(1<<(bits if unsigned else bits-1))-1:
            raise ValueError('normalized integer width differs')
        if language=='c':
            if n==-(1<<63):return '(-INT64_C(9223372036854775807)-INT64_C(1))'
            macro=('UINT' if unsigned else 'INT')+str(max(32,bits))+'_C'
            return ('-' if n<0 else '')+macro+'('+str(abs(n))+')'
        if language=='javascript':return normalized+('n' if bits==64 else '')
        if language=='java':
            if unsigned and bits==64:return '0x'+format(n,'016x')+'L'
            return normalized+('L' if bits==64 or type_=='uint32' else '')
        if language=='csharp':return normalized+('UL' if type_=='uint64' else 'L' if type_=='int64' else 'U' if type_=='uint32' else '')
        # Rust/Go/AS have a destination type; every full-width admitted integer
        # (including signed MIN) is an exact constant, never a floating number.
        if language in ('rust','tinygo','assemblyscript','python'):return normalized
        raise ValueError('scalar default target language is not implemented')
    if type_ in ('float32','float64'):
        n=float(normalized)
        if not math.isfinite(n) or str(n)!=normalized:raise ValueError('normalized finite float required')
        fmt='<f' if type_=='float32' else '<d';width=32 if type_=='float32' else 64
        try:raw=struct.pack(fmt,n)
        except (OverflowError,struct.error):raise ValueError('normalized float width differs') from None
        value=struct.unpack(fmt,raw)[0]
        if not math.isfinite(value):raise ValueError('finite target scalar default required')
        bits=int.from_bytes(raw,'little');hexbits=format(bits,'08x' if width==32 else '016x')
        if language=='c':return value.hex()+('F' if width==32 else '')
        if language=='rust':return 'f'+str(width)+'::from_bits(0x'+hexbits+')'
        if language=='tinygo':return 'math.Float'+str(width)+'frombits(0x'+hexbits+')'
        if language=='java':return ('Float.intBitsToFloat(0x'+hexbits+')' if width==32 else 'Double.longBitsToDouble(0x'+hexbits+'L)')
        if language=='csharp':return ('BitConverter.Int32BitsToSingle(unchecked((int)0x'+hexbits+'U))' if width==32 else 'BitConverter.Int64BitsToDouble(unchecked((long)0x'+hexbits+'UL))')
        if language=='assemblyscript':return 'reinterpret<f'+str(width)+'>(u'+str(width)+'(0x'+hexbits+'))'
        if language in ('javascript','python'):return repr(value)
        raise ValueError('scalar default target language is not implemented')
    raise ValueError('normalized scalar default type is not implemented')



def control_sources():
    """Exact actual original source family, not a reserved-package wildcard.

    These full UTF-8 source SHA/extent pins bind comments, constants, defaults
    and aliases, independently of runtime RIHS (which does not include defaults).
    Real installed native source/hash/description/layout still must be proved.
    """
    return {'builtin_interfaces/msg/Duration': {'sha256': '57da13a4201e7d258a2931323145b953e47e46967494e62ba66c2faca4fbade2', 'bytes': 498}, 'builtin_interfaces/msg/Time': {'sha256': 'dc5a01ddde3aecffee21eb24974fe77cc969f873218a2b1811aa0ac8a456b8bd', 'bytes': 421}, 'rcl_interfaces/msg/FloatingPointRange': {'sha256': '1ab3ea0d423cece5169061a33388ce45a0c00a67df45995e78450b3563ea6bdf', 'bytes': 1186}, 'rcl_interfaces/msg/IntegerRange': {'sha256': 'a7a970ee3f920e209d245868ef4ea0e959b5d600fd7e95f27913c3ab1ddfbace', 'bytes': 1036}, 'rcl_interfaces/msg/ListParametersResult': {'sha256': '505555153e477cf78a31b1f0463cbdc6b3fc0dc677afd42454340ebb4f50b186', 'bytes': 192}, 'rcl_interfaces/msg/Log': {'sha256': '55cfd6664610a922e7f549920b2c4a515b50e83c868238e1aaf4a27413d7b374', 'bytes': 1539}, 'rcl_interfaces/msg/LoggerLevel': {'sha256': '3ff1f091ebd9b65e3ca225bd91cd016457401415bee99286fb0b1cf5d6c5bcc2', 'bytes': 307}, 'rcl_interfaces/msg/Parameter': {'sha256': '2d49c337bdbbcfefccc5d24796b2eef8a025511ddff8a899c84e5c84638d5261', 'bytes': 317}, 'rcl_interfaces/msg/ParameterDescriptor': {'sha256': '66146fcc63a324ef4769b954f2ce6c9f92941274a40874275fb31b960db65e50', 'bytes': 1172}, 'rcl_interfaces/msg/ParameterEvent': {'sha256': '99f239f007b55c675d4a21fb654a690f30d52395da48cf18c038384bbe857a29', 'bytes': 547}, 'rcl_interfaces/msg/ParameterEventDescriptors': {'sha256': 'f46edb36a5d3aab37c1767300451e5d2355f2392a637ac8cce35ac507bc12636', 'bytes': 271}, 'rcl_interfaces/msg/ParameterType': {'sha256': 'abf7010b64fe2a087d3fd64627261d475323b8c0e765636f175ddb21ca6f4edc', 'bytes': 422}, 'rcl_interfaces/msg/ParameterValue': {'sha256': '5f7c95ce49c1c8a39b8c15171346de7bd1dc046a60ac33b2508cac1add5eedff', 'bytes': 1098}, 'rcl_interfaces/msg/SetLoggerLevelsResult': {'sha256': '79a9bd8065dca9a5bc6f1740b9209579e5537ecf5c659fc2741c8c207798794d', 'bytes': 132}, 'rcl_interfaces/msg/SetParametersResult': {'sha256': 'fb29a40cbabef53f6de34ccdc294053557b5d073bef47ea8b43d268e806c3e5a', 'bytes': 321}, 'rcl_interfaces/srv/DescribeParameters': {'sha256': '6047a59261d6c19a4e8e921f06702cc0672e0e8b6a6854fe601b30edf02e5981', 'bytes': 278}, 'rcl_interfaces/srv/GetLoggerLevels': {'sha256': '80d7c2ffe896e3b8d0ee228b78a5783d544887bbd3bd4549ef9d2a83403bae3c', 'bytes': 220}, 'rcl_interfaces/srv/GetParameterTypes': {'sha256': 'e5745ed79201f3ea515c13917f8d28780d16ed53c35f1b8d0f27c89945777bbe', 'bytes': 322}, 'rcl_interfaces/srv/GetParameters': {'sha256': '3ae49531812d8813350fad5261905a88939f8352f8a08fd238dcae19a08e59f7', 'bytes': 518}, 'rcl_interfaces/srv/ListParameters': {'sha256': '8254b67551049b2dc477d2fad286b645588227188482436d1b3bc0041731ed8b', 'bytes': 369}, 'rcl_interfaces/srv/SetLoggerLevels': {'sha256': 'f418e17d4db720944d1edae8073ae6df1499677c8f1a11ca19b0bcba6a19d44d', 'bytes': 152}, 'rcl_interfaces/srv/SetParameters': {'sha256': '6a0dcce330e51f954350e4ca3095fff7a7c0a0fa1c9d986b1dad340afb05f70a', 'bytes': 158}, 'rcl_interfaces/srv/SetParametersAtomically': {'sha256': '8593b435156b70299a4664839dc1b799ff22a7b7c0c27cec7b33bcc336389954', 'bytes': 238}, 'rosgraph_msgs/msg/Action': {'sha256': 'c25eb0d13b798d7b43ccd1a3a6ee398117dfc42f6a7b54b163f80b87bc31a1a0', 'bytes': 286}, 'rosgraph_msgs/msg/Clock': {'sha256': 'a8d1d14b0057bea4040f2e9662279e6ee270bd9fd6d00e1b2bcfb449bf4d521c', 'bytes': 160}, 'rosgraph_msgs/msg/Graph': {'sha256': '9cb4b7bd37f7b55c33e4bf713d6754b31b962efd930bd7863c692a07f79fe242', 'bytes': 80}, 'rosgraph_msgs/msg/InterfaceType': {'sha256': 'ad4254ebeabbe03a427daa8eb7ae29af0e43a893128e81c14ef59b6bd40e14d7', 'bytes': 266}, 'rosgraph_msgs/msg/Node': {'sha256': 'bbb49b7563bd21a46c7396612d7f864ff107afcda76465d5268bda99899ed68a', 'bytes': 733}, 'rosgraph_msgs/msg/QoSProfile': {'sha256': 'ec8c8fd026b207726c86043be766296ac7d2b3bafe00be0b50107f3873249298', 'bytes': 1424}, 'rosgraph_msgs/msg/Service': {'sha256': 'c96a75eb1368fbe833e077c4ea43a17f8f2a85f5b0aa20a38c605bc826517c77', 'bytes': 388}, 'rosgraph_msgs/msg/Topic': {'sha256': '62dee70bf4de7aff59a4c27b5df44c5533a732846cd7422da57ccc675fae1391', 'bytes': 214}, 'rosgraph_msgs/msg/TypeHash': {'sha256': '00e1273c383a62270c20c2384e9a50743d824af7d5b3af2be4564678eae8ddeb', 'bytes': 82}, 'service_msgs/msg/ServiceEventInfo': {'sha256': '9c4c2d2213a2a829926aba4efdaf028a1e9a3d3c28745699b3ab7700efa06311', 'bytes': 692}, 'type_description_interfaces/msg/Field': {'sha256': 'ed81e086cf714ef3ad193514110fd78f5d12a913647d0a88893d115ad100d744', 'bytes': 346}, 'type_description_interfaces/msg/FieldType': {'sha256': '8e2a8f0f439b6705fe5bfe8931138a973af65a23ee3045517d60b6e5ee144de2', 'bytes': 6162}, 'type_description_interfaces/msg/IndividualTypeDescription': {'sha256': 'e774d19d7e4b1bc8587057a557199da885c737e92bbb7f6d814ce950fbfd34a2', 'bytes': 491}, 'type_description_interfaces/msg/KeyValue': {'sha256': '390b6b7359235355f3aea6974ea5440a0a8cedbc7e752c4c4e47f3f9916ab175', 'bytes': 104}, 'type_description_interfaces/msg/TypeDescription': {'sha256': '3406a564a11c8097355c06241a2738ede19bde58056f6a95111c1121eaa9d43f', 'bytes': 285}, 'type_description_interfaces/msg/TypeSource': {'sha256': '83582ebc93c8ee67da1f40623734f4682fe278c55ebaf56bab15b0a7fe59ad55', 'bytes': 816}, 'type_description_interfaces/srv/GetTypeDescription': {'sha256': '3e26a2ac0c055cc8c1b6bd50c4b5c9cdd750a5604027c20cc23519a1905b4f5f', 'bytes': 1182}}


def control_registered_names():
    return {'rcl_interfaces/srv/ListParameters', 'rcl_interfaces/srv/GetParameterTypes', 'rcl_interfaces/srv/DescribeParameters', 'rcl_interfaces/srv/SetParameters', 'rcl_interfaces/srv/SetParametersAtomically', 'type_description_interfaces/srv/GetTypeDescription', 'rcl_interfaces/msg/ParameterEvent', 'rcl_interfaces/srv/GetParameters', 'rcl_interfaces/msg/Log'}


def control_native_name(parent, version, name, kind):
    if type(version) is not int or version != 2 or not isinstance(name,str) or name not in control_sources() or kind != ('service' if '/srv/' in name else 'message'):
        raise parent.GenerationError('control profile requires an exact selected original ROS2 source')
    return name


def control_reference(parent, package, spelling):
    bits=spelling.split('/')
    name=package+'/msg/'+spelling if len(bits)==1 else bits[0]+'/msg/'+bits[1] if len(bits)==2 else spelling
    return control_native_name(parent,2,name,'message')


def control_member_name(parent,name):
    # Only C and C++ output is implemented in this explicit profile. In
    # particular `type` is a legal unchanged C/C++ field, not a Rust keyword
    # escape or permission to emit broken managed-language records.
    keywords=set('auto break case char const continue default do double else enum extern float for goto if inline int long register restrict return short signed sizeof static struct switch typedef union unsigned void volatile while _Alignas _Alignof _Atomic _Bool _Complex _Generic _Imaginary _Noreturn _Static_assert _Thread_local alignas alignof asm bool catch class consteval constexpr constinit const_cast co_await co_return co_yield decltype delete dynamic_cast explicit export false friend mutable namespace new noexcept nullptr operator private protected public reinterpret_cast requires static_assert static_cast template this thread_local throw true try typeid typename using virtual wchar_t char8_t char16_t char32_t and or xor not bitand bitor compl and_eq or_eq xor_eq not_eq'.split())
    if not re.fullmatch(r'[a-z][a-z0-9]*(?:_[a-z0-9]+)*',name) or name in keywords:
        raise parent.GenerationError('original control field is not an exact legal C/C++ identifier: '+name)


def control_constant(parent,type_,name,literal):
    if not re.fullmatch(r'[A-Z][A-Z0-9_]*',name) or type_ not in ('bool','int8','uint8','int16','uint16','int32','uint32','int64','uint64','float32','float64'):
        raise parent.GenerationError('control constants require complete selected scalar declarations')
    return {'name':name,'type':type_,'value':scalar_native_default(parent,type_,literal),'literal':literal}


def control_summary(records, interfaces):
    result = {'implemented_record_outputs':['c','cpp'],
      'other_language_outputs':'REQUIRED_SEPARATE_IMPLEMENTATION_NOT_EMITTED',
      'native_registry_emitted':False,
      'native_registry_status':'REQUIRED_COMPLETE_C_LOG_AND_C_GETTYPEDESCRIPTION_PLUS_CPP_PARAMETER_OBJECT_PROOF',
      'declared_constants':{r.name:r.constants for r in records if r.constants},
      'original_scalar_aliases':{r.name:{m.name:m.native_scalar for m in r.members if m.native_scalar} for r in records if any(m.native_scalar for m in r.members)},
      'native_identity_policy':'independent actual exported complete hashes/source/TypeDescription/Event/layout; no nullable-default inference',
      'implicit_permissions':[]}
    # Preserve the entire historical summary unless the explicit new domain is
    # present. Wire capacity is never labelled an object/stack/RSS bound.
    table = {(r.version, r.name): r for r in records}
    changed = [r for r in records if wire_extensions(r, table)]
    if changed:
        result.update(canonical_string_domains={r.name: {m.name: m.string_encoding for m in r.members if m.string_encoding != 'utf8'} for r in changed if any(m.string_encoding != 'utf8' for m in r.members)},
          aggregate_wire_bytes={r.name: r.aggregate_wire_bytes for r in changed if r.aggregate_wire_bytes},
          cartesian_wire_capacities={r.name: r.cartesian_maximum for r in changed},
          inline_storage_size_status='REQUIRES_ACTUAL_TARGET_SIZEOF_AND_STACK_BUDGET_NOT_WIRE_QUOTA')
    return result


def generate_control(parent,path,output,records,ordered,interfaces,pins,guest_only):
    """Write actual complete C/C++ canonical source, never native stand-ins.

    This first coherent stage intentionally refuses native-registry emission.
    Native C and C++ object adapters/ownership are a separate required stage.
    No selected scalar alias or constant is erased from its identity/source IR.
    """
    if not guest_only:
        raise parent.GenerationError('control stage1 requires --guest-only; complete native registry is a separate required stage')
    products={'bounded_records.h':emit_c(parent,records,ordered),'bounded_records.hpp':emit_cpp(ordered),
              'bounded_interfaces.h':emit_c_interfaces(interfaces),'bounded_interfaces.hpp':emit_cpp_interfaces(interfaces)}
    schema={'profile':CONTROL_PROFILE,'source_inputs':pins,'native_registry_emitted':False,
      'records':[{'type':r.name,'kind':r.kind,'component':r.component,'owner':r.owner,'source':r.source,
         'native_identity':r.identity,'adapter_codec_identity':r.codec_identity,'minimum_wire_bytes':r.minimum,'maximum_wire_bytes':r.maximum,
         'constants':r.constants,'members':[{'name':m.name,'canonical_type':m.type,'native_scalar':m.native_scalar or m.type,
           'array':m.array,'canonical_capacity':m.count,'native_sequence_bound':m.native_sequence_bound,
           'canonical_string_bytes':m.string_bound,'native_string_bound':m.native_string_bound,'native_default':m.native_default} for m in r.members]} for r in ordered]}
    for item, record in zip(schema['records'], ordered):
        if wire_extensions(record, records):
            item.update(cartesian_maximum_wire_bytes=record.cartesian_maximum, aggregate_wire_bytes=record.aggregate_wire_bytes)
        for member_item, member in zip(item['members'], record.members):
            if member.string_encoding != 'utf8':
                member_item['canonical_string_encoding'] = member.string_encoding
    if any(wire_extensions(record, records) for record in ordered):
        products['control_wire.hpp'] = emit_control_wire(parent, records, ordered)
        products['control_manifest.hpp'] = emit_manifest(parent, records, ordered, interfaces)
    products['control-schema.json']=json.dumps(schema,indent=2)+'\n'
    proof=summary(interfaces,pins);proof.update(control_summary(ordered,interfaces))
    proof.update(status='GENERATED_CONTROL_C_CPP_CANONICAL_SOURCE_ONLY',
      generator_sha256=hashlib.sha256(parent.bounded_read(Path(parent.__file__),parent.MAX_SOURCE_BYTES)).hexdigest(),
      bounded_generator_sha256=hashlib.sha256(parent.bounded_read(Path(__file__),parent.MAX_SOURCE_BYTES)).hexdigest(),
      outputs={name:{'bytes':len(body.encode()),'sha256':hashlib.sha256(body.encode()).hexdigest()} for name,body in products.items()})
    # Reopen all admitted source bytes before committing any new source tree.
    current=parent.bounded_read(path,parent.MAX_CATALOG_BYTES)
    if len(current)!=pins['catalog']['bytes'] or hashlib.sha256(current).hexdigest()!=pins['catalog']['sha256']:
        raise parent.GenerationError('control catalog changed after source resolution')
    root=path.resolve().parent
    for pin in pins['definitions']:
        source=(root/pin['file']).resolve()
        if not source.is_relative_to(root):raise parent.GenerationError('control source escaped after resolution')
        raw=parent.bounded_read(source,parent.MAX_SOURCE_BYTES)
        if len(raw)!=pin['bytes'] or hashlib.sha256(raw).hexdigest()!=pin['sha256']:
            raise parent.GenerationError('control source changed after source resolution')
    if output.exists():raise parent.GenerationError('select a fresh control source directory')
    output.mkdir(parents=True)
    for name,body in products.items():(output/name).write_text(body,encoding='utf-8')
    (output/'generation.json').write_text(json.dumps(proof,indent=2)+'\n',encoding='utf-8')
    return proof

def parse_fields(parent, text, version, package, bounds, used, prefix='', standard=False, native_type='', control=False, constants=None):
    """Parse a complete .msg component with explicit finite dynamic capacities.

    ROS1 .msg syntax remains official: string and Type[] need catalog bounds.
    ROS2 string<=N and Type[<=N] keep their actual native bounds; a catalog may
    narrow a bound but cannot enlarge it. Unknown defaults/constants/aliases/recursion
    and unbounded fields without a supplied limit are refused explicitly.
    """
    result = []
    def literal(digits, maximum, label):
        if len(digits)>10 or not digits.isdecimal():
            raise parent.GenerationError('bounded positive literal required: '+label)
        value=int(digits)
        if not 1<=value<=maximum:
            raise parent.GenerationError('native/canonical bound exceeds finite profile: '+label)
        return value
    for number, raw in enumerate(text.splitlines(), 1):
        line = raw.split('#', 1)[0].strip()
        if not line:
            continue
        if control and re.match(r'[A-Za-z][A-Za-z0-9_]*\s+[A-Z][A-Z0-9_]*\s*=',line):
            constant=re.fullmatch(r'([A-Za-z][A-Za-z0-9_]*)\s+([A-Z][A-Z0-9_]*)\s*=\s*([^\s]+)',line)
            if not constant:raise parent.GenerationError('unsupported original control constant declaration')
            item=control_constant(parent,*constant.groups())
            if constants is None or any(c['name']==item['name'] for c in constants):
                raise parent.GenerationError('duplicate/missing control constant owner')
            constants.append(item);continue
        match = re.fullmatch(r'([A-Za-z][A-Za-z0-9_/]*)(?:<=([0-9]+))?(?:\[([0-9]*|<=[0-9]+)\])?\s+([A-Za-z][A-Za-z0-9_]*)(?:\s+([^\s]+))?', line)
        if not match:
            raise parent.GenerationError(f'line{number}: unsupported bounded field/default/constant syntax')
        base, text_bound, array, name, default_literal = match.groups()
        native_default = None
        if default_literal is not None:
            if version!=2 or array is not None or text_bound is not None:
                raise parent.GenerationError('ROS2 scalar defaults require an exact scalar field')
            native_default=scalar_native_default(parent,base,default_literal)
        if control:control_member_name(parent,name)
        else:parent.field_name(name, version, bounded=True)
        native_scalar=base if control and base in ('byte','char') else ''
        if native_scalar:
            if native_default is not None:raise parent.GenerationError('aliased scalar default requires an independent implementation')
            base='uint8'
        if base in ('wstring', 'byte', 'char') or (base in ('time', 'duration', 'Header') and not (standard and version == 1)):
            raise parent.GenerationError('unsupported ambiguous/wide field: ' + base)
        if text_bound is not None and (version != 2 or base != 'string'):
            raise parent.GenerationError('only official ROS2 bounded string declarations accept <=N')
        key = prefix + name
        supplied = bounds.get(key, {})
        if key in bounds:
            used.add(key)
        if not isinstance(supplied, dict) or set(supplied) - ({'string', 'sequence', 'encoding'} if control else {'string', 'sequence'}):
            raise parent.GenerationError('field bounds require admitted string/sequence limits and explicit control encoding only')
        encoding = supplied.get('encoding', 'utf8')
        if 'encoding' in supplied and not (control and native_type == 'type_description_interfaces/msg/TypeSource' and name == 'raw_file_contents' and base == 'string' and array is None and encoding == 'raw_bytes'):
            raise parent.GenerationError('raw_bytes is reserved for the exact original control TypeSource.raw_file_contents field')
        for bound_key, value in supplied.items():
            if bound_key == 'encoding':
                continue
            if type(value) is not int or not 1 <= value <= MAX_WIRE:
                raise parent.GenerationError('field bound must be a positive literal <=32768')
        native_string = literal(text_bound,MAX_WIRE-4,'string') if text_bound is not None else 0
        string_bound = supplied.get('string', native_string)
        if base == 'string':
            if not 1 <= string_bound <= MAX_WIRE - 4 or (native_string and string_bound > native_string):
                raise parent.GenerationError('every string needs an explicit finite byte bound no larger than its native bound')
        elif 'string' in supplied:
            raise parent.GenerationError('string bound applies only to a string field')
        shape, count, native_sequence = '', 0, 0
        if array is not None:
            if array.startswith('<='):
                if version != 2:
                    raise parent.GenerationError('ROS1 arrays use official [] syntax and separate trusted bounds')
                native_sequence = literal(array[2:],MAX_NODES,'sequence');shape = 'sequence';count = supplied.get('sequence', native_sequence)
            elif array:
                shape = 'array';count = literal(array,MAX_NODES,'fixed array')
            else:
                shape = 'sequence';count = supplied.get('sequence', 0)
            if not 1 <= count <= MAX_NODES or (native_sequence and count > native_sequence):
                raise parent.GenerationError('array/sequence capacity must be1..4096 and within official native capacity')
        if 'sequence' in supplied and shape != 'sequence':
            raise parent.GenerationError('sequence bound applies only to a variable array')
        resolved = base if base in parent.SCALARS or base == 'string' else (control_reference(parent,package,base) if control else standard_reference(parent, version, package, base) if standard else parent.reference(version, package, base))
        result.append(Field(name, resolved, shape, count, string_bound if base == 'string' else 0,
                            native_string, native_sequence, native_default, native_scalar, encoding))
    if len(result) >= MAX_NODES or len({member.name for member in result}) != len(result):
        raise parent.GenerationError('duplicate fields or bounded record field ceiling exceeded')
    if not control:parent.bounded_member_names(member.name for member in result)
    return result


def load_catalog(parent, path: Path, require_identity=True):
    """Resolve an acyclic complete schema and pin all exact input definitions."""
    raw = parent.bounded_read(path, parent.MAX_CATALOG_BYTES)
    catalog = parent.strict_json(raw)
    if not isinstance(catalog, dict) or set(catalog) != {'format', 'types'} or catalog['format'] not in (PROFILE, STANDARD_PROFILE, CONTROL_PROFILE):
        raise parent.GenerationError('bounded catalog requires exactly format and types')
    standard = catalog['format'] == STANDARD_PROFILE
    control = catalog['format'] == CONTROL_PROFILE
    specs = catalog['types']
    if not isinstance(specs, list) or not 1 <= len(specs) <= MAX_TYPES:
        raise parent.GenerationError('bounded catalog requires1..128 interfaces')
    root, records, interfaces, inputs, seen = path.resolve().parent, {}, [], [], set()
    for spec in specs:
        if not isinstance(spec, dict) or set(spec) - ({'ros_version', 'type', 'file', 'native_identity', 'component_identities', 'bounds', 'registered', 'wire_limits'} if control else {'ros_version', 'type', 'file', 'native_identity', 'component_identities', 'bounds', 'registered'} if standard else {'ros_version', 'type', 'file', 'native_identity', 'component_identities', 'bounds'}) or not {'ros_version', 'type', 'file'} <= set(spec):
            raise parent.GenerationError('unsupported/missing bounded catalog entry key')
        if not isinstance(spec['file'], str):
            raise parent.GenerationError('definition file must be a relative string')
        source = (root / spec['file']).resolve()
        if not source.is_relative_to(root):
            raise parent.GenerationError('definition escapes bounded catalog root')
        kind = {'.msg': 'message', '.srv': 'service', '.action': 'action'}.get(source.suffix)
        if kind is None:
            raise parent.GenerationError('bounded-v2 accepts official .msg/.srv/.action; unsupported IDL preprocessing must be generated into these complete definitions first')
        version, name = spec['ros_version'], spec['type']
        name = control_native_name(parent,version,name,kind) if control else native_name(parent, version, name, kind, standard)
        registered = spec.get('registered', True)
        if control and (type(registered) is not bool or (registered and name not in control_registered_names())):
            raise parent.GenerationError('control registration is limited to the exact default endpoint family')
        if standard and (type(registered) is not bool or (kind != 'message' and not registered)):
            raise parent.GenerationError('explicit Message dependency or registered interface required')
        if standard and (version, name) in standard_names() and registered and (version, name) not in standard_topic_names():
            raise parent.GenerationError('official standard dependency-only and existing builtin IDs cannot be registered as new Topics')
        if (version, name) in seen:
            raise parent.GenerationError('duplicate native interface across message/service/action categories')
        seen.add((version, name))
        body = parent.bounded_read(source, parent.MAX_SOURCE_BYTES)
        try:
            text = body.decode('utf-8')
        except UnicodeError as error:
            raise parent.GenerationError('native definitions must be UTF-8') from error
        pin = {'file': spec['file'], 'bytes': len(body), 'sha256': hashlib.sha256(body).hexdigest()}
        if control and {'bytes':len(body),'sha256':pin['sha256']}!=control_sources()[name]:
            raise parent.GenerationError('control original full source differs: '+name)
        inputs.append(pin)
        sections = re.split(r'^\s*---\s*(?:#[^\n]*)?$', text, flags=re.MULTILINE)
        roles = {'message': ['message'], 'service': ['request', 'response'], 'action': ['goal', 'result', 'feedback']}[kind]
        if len(sections) != len(roles):
            raise parent.GenerationError('wrong number of complete .srv/.action components')
        wire_limits = spec.get('wire_limits', {})
        if not isinstance(wire_limits, dict) or ('wire_limits' in spec and (not control or not wire_limits)) or set(wire_limits) - set(roles):
            raise parent.GenerationError('control wire_limits must name exact existing component roles')
        if any(type(value) is not int or not 1 <= value <= MAX_WIRE for value in wire_limits.values()):
            raise parent.GenerationError('control aggregate wire limit must be a strict positive integer <=32768')
        native_identity = identity(parent, version, spec.get('native_identity'), require_identity, name)
        components = spec.get('component_identities', {})
        if not isinstance(components, dict) or set(components) - set(roles) or (kind == 'message' and components):
            raise parent.GenerationError('component_identities must name only the exact Service/Action roles')
        bounds, used = spec.get('bounds', {}), set()
        if not isinstance(bounds, dict) or not all(isinstance(key, str) for key in bounds):
            raise parent.GenerationError('bounds must map complete declared field paths to explicit limits')
        interface = Interface(version, name, kind, native_identity, registered=registered, profile=catalog['format'])
        for role, section in zip(roles, sections):
            part_name = name if kind == 'message' else name + ('_' if version == 2 else '') + role.title()
            part_identity = native_identity if kind == 'message' else identity(parent, version, components.get(role), require_identity, name + ':' + role)
            constants=[]
            members = parse_fields(parent, section, version, name.split('/')[0], bounds, used, '' if kind == 'message' else role + '.', standard, part_name, control, constants)
            if not members and kind == 'message' and (not control or registered):
                raise parent.GenerationError('empty native Topics are outside this profile; empty Service/Action components are supported')
            record = Record(version, part_name, members, part_identity, pin, '' if kind == 'message' else role, name, kind)
            record.profile = catalog['format']
            record.constants = constants
            record.aggregate_wire_bytes = wire_limits.get(role, 0)
            if standard:
                standard_shape(parent, record)
            key = (version, part_name)
            if key in records:
                raise parent.GenerationError('component/message native record name collision')
            records[key] = record;interface.components[role] = record
        if set(bounds) != used:
            raise parent.GenerationError('bounds contain an undeclared field path')
        interfaces.append(interface)
    if standard:
        # ROS1 primitive values have no MD5/RIHS or MSG dependency section.
        # Their actual C++ fields/types are verified in generated native traits.
        needed = {m.type for r in records.values() if r.version == 1 for m in r.members if m.type in ('time','duration')}
        for name in sorted(needed):
            scalar = 'uint32' if name == 'time' else 'int32'
            item = Record(1,name,[Field('sec',scalar),Field('nsec',scalar)],None,
                {'ros1_native_primitive':name,'native_fields':'actual ros::Time/Duration sec/nsec; compiled static traits required'},kind='native-primitive',profile=STANDARD_PROFILE)
            records[(1,name)] = item
    # C++/Rust/JS/Go/AS share the concatenated public type spelling.  Package
    # separators/capitalization and generated companion suffixes must not make
    # two distinct official types occupy one emitted namespace.  C's uppercase
    # macro namespace is checked independently.  Refusal preserves native names.
    public, c_names, c_macros = set(), set(), set()
    for record in records.values():
        symbols = {record.symbol, record.symbol + 'Codec', 'Decode' + record.symbol,
                   'read' + record.symbol, record.symbol + 'CodecIdentity',
                   record.symbol + 'MaxWireBytes'}
        c_symbols = {record.c_symbol} | {record.c_symbol + suffix for suffix in
                    ('_valid', '_wire_size', '_write_fields', '_read_fields', '_encode', '_decode')}
        for member in record.members:
            if member.type == 'string':
                c_symbols.add(record.c_symbol + '_' + member.name + '_text')
            if member.array == 'sequence':
                c_symbols.add(record.c_symbol + '_' + member.name + '_sequence')
        macros = {record.c_symbol.upper() + suffix for suffix in
                  ('_MAX_WIRE_BYTES', '_MIN_WIRE_BYTES', '_CODEC_IDENTITY')}
        if wire_extensions(record, records):
            c_symbols.update(record.c_symbol + suffix for suffix in ('_shape', '_add', '_measure', '_read_status'))
            macros.update(record.c_symbol.upper() + suffix for suffix in ('_CARTESIAN_MAX_WIRE_BYTES', '_AGGREGATE_WIRE_BYTES'))
        if public.intersection(symbols) or c_names.intersection(c_symbols) or c_macros.intersection(macros):
            raise parent.GenerationError('native records collide in generated language namespace')
        public.update(symbols);c_names.update(c_symbols);c_macros.update(macros)
    for interface in interfaces:
        if interface.kind != 'message':
            symbols = {interface.symbol + 'Client', interface.symbol + 'Server'}
            if public.intersection(symbols):
                raise parent.GenerationError('native interface collides with generated client/server namespace')
            public.update(symbols)
    ordered, active = [], set()
    def resolve(record, depth=1):
        key = (record.version, record.name)
        if depth > MAX_DEPTH or key in active:
            raise parent.GenerationError('recursive/excess-depth bounded records are unsupported')
        if record.codec_identity:
            if depth + record.depth - 1 > MAX_DEPTH:
                raise parent.GenerationError('cached bounded dependency exceeds depth16')
            return
        active.add(key);minimum,maximum,cartesian,nodes,member_ir = 0,0,0,1,[]
        record.has_native_defaults = False
        for member in record.members:
            if member.type in parent.SCALARS:
                low = high = full_high = parent.SCALARS[member.type][1];count = 1;child_ir = {'scalar': member.type};child_depth = 1
            elif member.type == 'string':
                low,high,count,child_depth = 4,4+member.string_bound,1,1
                full_high = high
                child_ir = ({'raw_bytes': member.string_bound, 'encoding': 'raw_bytes'} if member.string_encoding == 'raw_bytes' else {'utf8_bytes': member.string_bound, 'nul': 'forbidden'})
            else:
                child = records.get((record.version, member.type))
                if child is None or (child.kind != 'message' and not (standard and record.version == 1 and child.kind == 'native-primitive')):
                    raise parent.GenerationError('nested message dependency must be explicitly catalogued: ' + member.type)
                resolve(child, depth+1)
                low,high,count,child_depth = child.minimum,child.maximum,child.nodes,child.depth
                full_high = child.cartesian_maximum
                child_ir = {'message': member.type, 'canonical_identity': child.codec_identity}
                record.has_native_defaults |= child.has_native_defaults
            if member.native_scalar:
                child_ir['native_scalar'] = member.native_scalar
            if member.native_default is not None:
                record.has_native_defaults = True
                # Add this key only for declared defaults; every existing
                # no-default canonical identity domain stays byte-identical.
                child_ir['native_default'] = member.native_default
            if member.array:
                high *= member.count;full_high *= member.count;count = count*member.count+1;child_depth += 1
                low = low*member.count if member.array == 'array' else 4
                if member.array == 'sequence':
                    high += 4;full_high += 4
                child_ir = {member.array: member.count, 'element': child_ir}
            minimum += low;maximum += high;cartesian += full_high;nodes += count
            record.depth = max(record.depth, child_depth+1)
            if (maximum > MAX_WIRE and not record.aggregate_wire_bytes) or cartesian > (1 << 64) - 1 or nodes > MAX_NODES or record.depth > MAX_DEPTH:
                raise parent.GenerationError('bounded wire32KiB/uint64-cartesian/node4096/depth16 ceiling exceeded')
            member_ir.append({'name': member.name, 'wire': child_ir})
        if record.aggregate_wire_bytes and minimum > record.aggregate_wire_bytes:
            raise parent.GenerationError('aggregate wire limit is smaller than the complete record minimum')
        record.minimum,record.maximum,record.nodes = minimum,min(maximum,record.aggregate_wire_bytes) if record.aggregate_wire_bytes else maximum,nodes
        record.cartesian_maximum = cartesian
        ir = {'profile': record.profile,'ros_version':record.version,'type':record.name,'members':member_ir,
              'byte_order':'little-endian','booleans':'0-or-1','floats':'finite-ieee754'}
        if record.aggregate_wire_bytes:
            ir['aggregate_wire_bytes'] = record.aggregate_wire_bytes
        if record.constants:
            ir['declared_constants'] = record.constants
        if standard_semantics(record):
            ir['complete_value_semantics'] = standard_semantics(record)
        record.codec_identity = 'UWVMROS1_' + hashlib.sha256(parent.canonical_json(ir)).hexdigest()
        active.remove(key);ordered.append(record)
    for record in records.values():
        resolve(record)
    for record in ordered:
        if record.has_native_defaults:
            new_public = 'Default' + record.symbol
            new_c = record.c_symbol + '_default'
            if new_public in public or new_c in c_names:
                raise parent.GenerationError('declared-default initializer collides with a generated namespace')
            public.add(new_public);c_names.add(new_c)
    if sum(record.nodes for record in ordered) > MAX_TOTAL_NODES:
        raise parent.GenerationError('total expanded generation nodes exceed16384')
    for interface in interfaces:
        ir = {'profile':interface.profile,'ros_version':interface.version,'type':interface.name,'kind':interface.kind,
              'components':{role:record.codec_identity for role,record in interface.components.items()},
              'tickets':'runtime-owned-u64','result_kinds':[2,3,4] if interface.kind=='action' else []}
        interface.codec_identity = interface.components['message'].codec_identity if interface.kind=='message' else 'UWVMROS1_'+hashlib.sha256(parent.canonical_json(ir)).hexdigest()
    return records,ordered,interfaces,{'catalog':{'bytes':len(raw),'sha256':hashlib.sha256(raw).hexdigest()},'definitions':inputs}


def c_element(parent, record, member, records):
    """Use standalone C values with inline capacities, never borrowed pointers."""
    if member.type == 'string':
        return record.c_symbol + '_' + member.name + '_text'
    if member.type in parent.SCALARS:
        return parent.SCALARS[member.type][2]
    return records[(record.version, member.type)].c_symbol


def wire_extensions(record, records, active=None):
    """The old emission path remains exact unless a true descendant opts in."""
    if record.aggregate_wire_bytes or any(m.string_encoding == 'raw_bytes' for m in record.members):
        return True
    active = set() if active is None else active
    key = (record.version, record.name)
    if key in active:
        return False  # load_catalog's independent resolver rejects the cycle.
    active.add(key)
    try:
        return any(child is not None and wire_extensions(child, records, active)
                   for m in record.members
                   for child in [records.get((record.version, m.type))])
    finally:
        active.remove(key)


def emit_control_wire(parent, records, ordered):
    """Closed generated grammar only; no native IDs, registry, grant or lookup."""
    lines = ['/** Explicit control raw-byte/aggregate canonical grammar; SOURCE only, no authority. */',
             '#pragma once', '#include <uwvm2/imported/ros/ipc/custom_wire.h>',
             'namespace uwvm_control_wire {', 'using namespace ::uwvm2::imported::ros::ipc;']
    for record in ordered:
        def element(member):
            if member.type == 'string':
                kind = 'raw_bytes' if member.string_encoding == 'raw_bytes' else 'utf8'
                return '{custom_wire_kind::' + kind + ',' + str(member.string_bound) + ',nullptr,0}'
            if member.type in parent.SCALARS:
                return '{custom_wire_kind::' + parent.SCALARS[member.type][0] + ',0,nullptr,0}'
            return 'schema_' + records[(record.version, member.type)].symbol
        members = []
        for index, member in enumerate(record.members):
            if member.array:
                name = 'element_' + record.symbol + '_' + str(index)
                lines.append('inline constexpr custom_wire_node ' + name + '=' + element(member) + ';')
                members.append('{custom_wire_kind::' + member.array + ',' + str(member.count) + ',&' + name + ',1}')
            else:
                members.append(element(member))
        pointer = 'nullptr'
        if members:
            pointer = 'children_' + record.symbol
            lines.append('inline constexpr custom_wire_node ' + pointer + '[]={' + ','.join(members) + '};')
        kind = 'bounded_record' if record.aggregate_wire_bytes else 'record'
        lines.append('inline constexpr custom_wire_node schema_' + record.symbol + '{custom_wire_kind::' + kind + ',' + str(record.aggregate_wire_bytes) + ',' + pointer + ',' + str(len(members)) + '};')
    return '\n'.join(lines + ['}', ''])


def emit_c_extended_functions(parent, record, records):
    """Full shape, checked measure, and one-commit codecs for explicit new domains.

    This function is never selected for a legacy record without a raw/aggregate
    descendant. Internal readers/writers borrow only private prevalidated storage.
    The C compiler may reserve a complete inline temporary's stack frame on entry;
    a logical length preflight does not claim to avoid that physical reservation.
    """
    cs = record.c_symbol
    lines = [f'#define {cs.upper()}_CARTESIAN_MAX_WIRE_BYTES UINT64_C({record.cartesian_maximum})',
             f'#define {cs.upper()}_AGGREGATE_WIRE_BYTES {record.aggregate_wire_bytes}u',
             f'/** Shape validation retains every field; aggregate quota is checked separately. */',
             f'static inline int {cs}_shape({cs} const* value) {{if(!value)return 0;']
    def each(member, body, guard=False):
        a = 'value->' + member.name
        if member.array == 'sequence':
            return ([f'  if({a}.size>{member.count}u)return 0;'] if guard else []) + [f'  {{size_t i;for(i=0;i<{a}.size;++i){{'] + body(a + '.data[i]') + ['  }}']
        if member.array == 'array':
            return [f'  {{size_t i;for(i=0;i<{member.count}u;++i){{'] + body(a + '[i]') + ['  }}']
        return body(a)
    for member in record.members:
        def shape(a):
            if member.type == 'string':
                check = '' if member.string_encoding == 'raw_bytes' else f'||!uwvm_bounded_utf8({a}.data,{a}.size)'
                return [f'  if({a}.size>{member.string_bound}u{check})return 0;']
            if member.type not in parent.SCALARS:
                child = records[(record.version, member.type)]
                suffix = '_shape' if wire_extensions(child, records) else '_valid'
                return [f'  if(!{child.c_symbol}{suffix}(&{a}))return 0;']
            if member.type == 'bool':
                return [f'  if({a}>1)return 0;']
            if member.type.startswith('float'):
                width = parent.SCALARS[member.type][1]
                mask = 'UINT32_C(0x7f800000)' if width == 4 else 'UINT64_C(0x7ff0000000000000)'
                return [f'  {{uint{width*8}_t bits;uwvm_bounded_copy(&bits,&{a},{width});if((bits&{mask})=={mask})return 0;}}']
            return []
        lines += each(member, shape, True)
    if semantic_condition(record, 'c', 'value'):
        lines += ['  if(' + semantic_condition(record, 'c', 'value') + ')return 0;']
    lines += ['  return 1;}',
              f'/** Checked addition under this record\'s full admitted wire extent. */',
              f'static inline int {cs}_add(size_t* n,size_t amount) {{if(*n>{record.maximum}u||amount>{record.maximum}u-*n)return 0;*n+=amount;return 1;}}',
              f'/** Full validation and measure; quota90 preserves the caller\'s size sentinel. */',
              f'static inline uint32_t {cs}_measure({cs} const* value,size_t* length) {{size_t n=0;',
              f'  if(!length||!{cs}_shape(value)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;']
    for member in record.members:
        if member.array == 'sequence':
            lines += [f'  if(!{cs}_add(&n,4))return UWVM_BOUNDED_TOO_LARGE;']
        def measure(a):
            if member.type == 'string':
                return [f'  if(!{cs}_add(&n,4)||!{cs}_add(&n,{a}.size))return UWVM_BOUNDED_TOO_LARGE;']
            if member.type in parent.SCALARS:
                return [f'  if(!{cs}_add(&n,{parent.SCALARS[member.type][1]}))return UWVM_BOUNDED_TOO_LARGE;']
            child = records[(record.version, member.type)]
            if wire_extensions(child, records):
                begin = f'{{size_t nested=0;uint32_t error={child.c_symbol}_measure(&{a},&nested);if(error)return error;'
            else:
                begin = f'{{size_t nested={child.c_symbol}_wire_size(&{a});'
            return ['  ' + begin + f'if(!{cs}_add(&n,nested))return UWVM_BOUNDED_TOO_LARGE;}}']
        lines += each(member, measure)
    lines += ['  *length=n;return 0;}',
              f'static inline int {cs}_valid({cs} const* value) {{size_t n=0;return {cs}_measure(value,&n)==0;}}',
              f'/** Compatibility size view; use measure for an unambiguous error/status. */',
              f'static inline size_t {cs}_wire_size({cs} const* value) {{size_t n=0;return {cs}_measure(value,&n)?0:n;}}',
              f'/** Internal no-fail writer, entered only after complete measure/overlap proof. */',
              f'static inline void {cs}_write_fields({cs} const* value,unsigned char* p,size_t* offset) {{']
    for member in record.members:
        if member.array == 'sequence':
            lines += [f'  uwvm_bounded_put(p+*offset,value->{member.name}.size,4);*offset+=4;']
        def write(a):
            if member.type == 'string':
                return [f'  uwvm_bounded_put(p+*offset,{a}.size,4);*offset+=4;uwvm_bounded_copy(p+*offset,{a}.data,{a}.size);*offset+={a}.size;']
            if member.type not in parent.SCALARS:
                return [f'  {records[(record.version, member.type)].c_symbol}_write_fields(&{a},p,offset);']
            width = parent.SCALARS[member.type][1]
            if member.type.startswith('float'):
                return [f'  {{uint{width*8}_t bits;uwvm_bounded_copy(&bits,&{a},{width});uwvm_bounded_put(p+*offset,bits,{width});*offset+={width};}}']
            return [f'  uwvm_bounded_put(p+*offset,(uint64_t){a},{width});*offset+={width};']
        lines += each(member, write)
    lines += ['}',
              f'/** Internal reader/status keeps all intermediate mutations private. */',
              f'static inline uint32_t {cs}_read_status(unsigned char const* p,size_t n,size_t* offset,{cs}* value) {{size_t measured=0;',
              '  if(!value||!offset||*offset>n||(n&&!p))return UWVM_BOUNDED_INVALID;']
    for member in record.members:
        if member.array == 'sequence':
            a = 'value->' + member.name
            lines += [f'  if(*offset>n||n-*offset<4)return UWVM_BOUNDED_INVALID;{a}.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;',
                      f'  if({a}.size>{member.count}u)return UWVM_BOUNDED_INVALID;']
        def read(a):
            if member.type == 'string':
                return [f'  if(*offset>n||n-*offset<4)return UWVM_BOUNDED_INVALID;{a}.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;',
                        f'  if({a}.size>{member.string_bound}u||{a}.size>n-*offset)return UWVM_BOUNDED_INVALID;uwvm_bounded_copy({a}.data,p+*offset,{a}.size);*offset+={a}.size;']
            if member.type not in parent.SCALARS:
                child = records[(record.version, member.type)]
                if wire_extensions(child, records):
                    return [f'  {{uint32_t error={child.c_symbol}_read_status(p,n,offset,&{a});if(error)return error;}}']
                return [f'  if(!{child.c_symbol}_read_fields(p,n,offset,&{a}))return UWVM_BOUNDED_INVALID;']
            width = parent.SCALARS[member.type][1]
            return [f'  if(*offset>n||n-*offset<{width})return UWVM_BOUNDED_INVALID;{{uint{width*8}_t bits=(uint{width*8}_t)uwvm_bounded_get(p+*offset,{width});uwvm_bounded_copy(&{a},&bits,{width});}}*offset+={width};']
        lines += each(member, read)
    lines += [f'  return {cs}_measure(value,&measured);', '}',
              f'static inline int {cs}_read_fields(unsigned char const* p,size_t n,size_t* offset,{cs}* value) {{return {cs}_read_status(p,n,offset,value)==0;}}',
              f'/** No writes before full shape, nested quotas, exact size and disjoint destinations. */',
              f'static inline uint32_t {cs}_encode({cs} const* value,void* output,size_t capacity,size_t* length) {{size_t n=0,offset=0;uint32_t error;',
              f'  if(!length)return UWVM_BOUNDED_INVALID;error={cs}_measure(value,&n);if(error)return error;',
              '  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;',
              f'  {cs}_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;}}',
              f'/** The compiler may reserve the full temporary stack frame even on preflight failure. */',
              f'static inline uint32_t {cs}_decode(void const* input,size_t length,{cs}* value) {{size_t offset=0;uint32_t error;',
              f'  if(!value||(length&&!input)||length<{record.minimum}u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;',
              f'  if(length>{record.maximum}u)return UWVM_BOUNDED_TOO_LARGE;',
              f'  {{{cs} next={{0}};error={cs}_read_status((unsigned char const*)input,length,&offset,&next);if(error)return error;if(offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;}}}}']
    return lines


def emit_c(parent, records, ordered):
    """Emit bounded C11 records and transactionally checked caller-buffer codecs."""
    lines = ['/** @file Generated bounded-v2 canonical values; no native ROS serialization or authority.',
        ' * Every dynamic field has inline caller-owned capacity. Encode validates and measures',
        ' * the full value before writes; decode commits one temporary only after full validation.',
        ' * Supply distinct record, byte and size-output storage; overlap is explicitly refused.',
        ' * Empty Service/Action components use a C layout dummy that is never a wire field. */',
        '#ifndef UWVM_ROS_BOUNDED_RECORDS_H', '#define UWVM_ROS_BOUNDED_RECORDS_H',
        '#include <stdint.h>', '#include <stddef.h>', '#include <float.h>', '#include <limits.h>',
        '#define UWVM_BOUNDED_INVALID 22u', '#define UWVM_BOUNDED_TOO_LARGE 90u',
        '#if defined(__cplusplus)', '#define UWVM_BOUNDED_ASSERT(x) static_assert(x,"canonical representation required")',
        '#else', '#define UWVM_BOUNDED_ASSERT(x) _Static_assert(x,"canonical representation required")', '#endif',
        'UWVM_BOUNDED_ASSERT(CHAR_BIT==8&&sizeof(float)==4&&sizeof(double)==8&&FLT_RADIX==2&&FLT_MANT_DIG==24&&DBL_MANT_DIG==53&&FLT_MAX_EXP==128&&DBL_MAX_EXP==1024);',
        '/** Copy exact primitive/string bytes; compiler intrinsics avoid byte-at-a-time copies. */',
        'static inline void uwvm_bounded_copy(void* out,void const* in,size_t n){',
        '#if defined(__GNUC__) || defined(__clang__)', '__builtin_memcpy(out,in,n);',
        '#else', 'size_t i;for(i=0;i<n;++i)((unsigned char*)out)[i]=((unsigned char const*)in)[i];', '#endif', '}',
        'static inline uint64_t uwvm_bounded_get(unsigned char const* p,unsigned n){uint64_t v=0;unsigned i;for(i=0;i<n;++i)v|=(uint64_t)p[i]<<(8*i);return v;}',
        'static inline void uwvm_bounded_put(unsigned char* p,uint64_t v,unsigned n){unsigned i;for(i=0;i<n;++i)p[i]=(unsigned char)(v>>(8*i));}',
        '/** Strict Unicode scalar UTF-8, rejecting embedded NUL as required by this profile. */',
        'static inline int uwvm_bounded_utf8(unsigned char const* p,size_t n){size_t i=0;while(i<n){unsigned b=p[i++],k,code,minimum;if(!b)return 0;if(b<128)continue;if(b>=194&&b<=223){k=1;code=b&31;minimum=128;}else if(b>=224&&b<=239){k=2;code=b&15;minimum=2048;}else if(b>=240&&b<=244){k=3;code=b&7;minimum=65536;}else return 0;if(k>n-i)return 0;while(k--){b=p[i++];if((b&192)!=128)return 0;code=(code<<6)|(b&63);}if(code<minimum||code>1114111||(code>=55296&&code<=57343))return 0;}return 1;}',
        'static inline int uwvm_bounded_overlap(void const* a,size_t an,void const* b,size_t bn){uintptr_t x=(uintptr_t)a,y=(uintptr_t)b;if((an&&!a)||(bn&&!b)||an>UINTPTR_MAX-x||bn>UINTPTR_MAX-y)return 1;return an&&bn&&x<y+bn&&y<x+an;}']
    for record in ordered:
        cs = record.c_symbol
        for member in record.members:
            if member.type == 'string':
                lines += [f'/** UTF-8 byte storage for {record.name}.{member.name}; size excludes padding/terminators. */',
                          f'typedef struct {{uint32_t size;unsigned char data[{member.string_bound}];}} {c_element(parent,record,member,records)};']
                if member.string_encoding == 'raw_bytes':
                    lines[-2] = f'/** Raw counted byte storage for {record.name}.{member.name}; every in-range byte including NUL is semantic. */'
            if member.array == 'sequence':
                lines += [f'/** At most{member.count} initialized logical elements; unused capacity is never serialized. */',
                          f'typedef struct {{uint32_t size;{c_element(parent,record,member,records)} data[{member.count}];}} {cs}_{member.name}_sequence;']
        lines += [f'/** Complete ROS{record.version} {record.name} value; no field projects away native data. */',f'typedef struct {cs} {{']
        for member in record.members:
            type_name = cs+'_'+member.name+'_sequence' if member.array=='sequence' else c_element(parent,record,member,records)
            suffix = f'[{member.count}]' if member.array=='array' else ''
            lines += [f'  /** Declared field {member.name}; canonical capacity is part of CODEC_IDENTITY. */',f'  {type_name} {member.name}{suffix};']
        if not record.members:
            lines += ['  /** Local C layout placeholder; empty native component has exactly zero canonical bytes. */','  unsigned char local_empty_placeholder;']
        lines += [f'}} {cs};',f'#define {cs.upper()}_MAX_WIRE_BYTES {record.maximum}u',
                  f'#define {cs.upper()}_MIN_WIRE_BYTES {record.minimum}u',f'#define {cs.upper()}_CODEC_IDENTITY {json.dumps(record.codec_identity)}']
        for constant in record.constants:
            lines += [f'/** Original scalar constant {constant["name"]}={constant["literal"]}; no instance/wire field. */',
                      f'#define {cs.upper()}_{constant["name"]} '+scalar_default_expression(constant['type'],constant['value'],'c')]
        if record.has_native_defaults:
            lines += ['/** Explicit declared-default initialization; ordinary C/C++ zero aggregates stay zero.',
                      ' * Nested/fixed elements are initialized independently; sequence lengths stay zero.',
                      ' * This initializer performs no I/O, registration, allocation or capability operation. */',
                      f'static inline uint32_t {cs}_default({cs}* value) {{',
                      f'  {cs} next={{0}};if(!value)return UWVM_BOUNDED_INVALID;']
            for member in record.members:
                if member.native_default is not None:
                    expression=scalar_default_expression(member.type,member.native_default,'c')
                    lines += [f'  next.{member.name}={expression};']
                elif member.type not in parent.SCALARS and member.type != 'string':
                    child=records[(record.version,member.type)]
                    if child.has_native_defaults and member.array != 'sequence':
                        if member.array == 'array':
                            lines += [f'  {{size_t i;for(i=0;i<{member.count}u;++i)(void){child.c_symbol}_default(&next.{member.name}[i]);}}']
                        else:lines += [f'  (void){child.c_symbol}_default(&next.{member.name});']
            lines += ['  *value=next;return 0;','}']
        if wire_extensions(record, records):
            lines += emit_c_extended_functions(parent, record, records)
            continue
        lines += [f'/** Validate every initialized field without allocation, writes or ROS calls. */',f'static inline int {cs}_valid({cs} const* value) {{','  if(!value)return 0;']
        def each(member, prefix, body):
            value = prefix + '->' + member.name
            if member.array=='sequence':
                return [f'  if({value}.size>{member.count}u)return 0;',f'  {{size_t i;for(i=0;i<{value}.size;++i){{']+body(value+'.data[i]')+['  }}']
            if member.array=='array':
                return [f'  {{size_t i;for(i=0;i<{member.count}u;++i){{']+body(value+'[i]')+['  }}']
            return body(value)
        for member in record.members:
            def validate(a):
                if member.type=='string':return [f'  if({a}.size>{member.string_bound}u||!uwvm_bounded_utf8({a}.data,{a}.size))return 0;']
                if member.type not in parent.SCALARS:return [f'  if(!{records[(record.version,member.type)].c_symbol}_valid(&{a}))return 0;']
                if member.type=='bool':return [f'  if({a}>1)return 0;']
                if member.type.startswith('float'):
                    width=parent.SCALARS[member.type][1];mask='UINT32_C(0x7f800000)' if width==4 else 'UINT64_C(0x7ff0000000000000)'
                    return [f'  {{uint{width*8}_t bits;uwvm_bounded_copy(&bits,&{a},{width});if((bits&{mask})=={mask})return 0;}}']
                return []
            lines += each(member,'value',validate)
        if semantic_condition(record, 'c', 'value'):
            lines += ['  if('+semantic_condition(record, 'c', 'value')+')return 0;']
        lines += ['  return 1;','}',f'/** Return exact active-prefix wire size; the full value must already be valid. */',f'static inline size_t {cs}_wire_size({cs} const* value) {{size_t n=0;']
        for member in record.members:
            if member.array=='sequence':lines += ['  n+=4;']
            def measure(a):
                if member.type=='string':return [f'  n+=4+{a}.size;']
                if member.type in parent.SCALARS:return [f'  n+={parent.SCALARS[member.type][1]};']
                return [f'  n+={records[(record.version,member.type)].c_symbol}_wire_size(&{a});']
            # Range guards here return size0 only for misuse of this internal
            # prevalidated routine; public encode always calls valid first.
            lines += [line.replace('return 0;','return 0;') for line in each(member,'value',measure)]
        lines += ['  return n;','}',f'/** Internal writer enters only after full validation, capacity and overlap checks. */',f'static inline void {cs}_write_fields({cs} const* value,unsigned char* p,size_t* offset) {{']
        for member in record.members:
            if member.array=='sequence':lines += [f'  uwvm_bounded_put(p+*offset,value->{member.name}.size,4);*offset+=4;']
            def write(a):
                if member.type=='string':return [f'  uwvm_bounded_put(p+*offset,{a}.size,4);*offset+=4;uwvm_bounded_copy(p+*offset,{a}.data,{a}.size);*offset+={a}.size;']
                if member.type not in parent.SCALARS:return [f'  {records[(record.version,member.type)].c_symbol}_write_fields(&{a},p,offset);']
                width=parent.SCALARS[member.type][1]
                if member.type.startswith('float'):return [f'  {{uint{width*8}_t bits;uwvm_bounded_copy(&bits,&{a},{width});uwvm_bounded_put(p+*offset,bits,{width});*offset+={width};}}']
                return [f'  uwvm_bounded_put(p+*offset,(uint64_t){a},{width});*offset+={width};']
            # void writer omits guards already proven by the public entry.
            lines += [line for line in each(member,'value',write) if 'return 0;' not in line]
        lines += ['}',f'/** Internal reader fills only the public decoder temporary and checks every cursor move. */',f'static inline int {cs}_read_fields(unsigned char const* p,size_t n,size_t* offset,{cs}* value) {{']
        for member in record.members:
            if member.array=='sequence':lines += [f'  if(*offset>n||n-*offset<4)return 0;value->{member.name}.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;']
            def read(a):
                if member.type=='string':return [f'  if(*offset>n||n-*offset<4)return 0;{a}.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;',f'  if({a}.size>{member.string_bound}u||{a}.size>n-*offset)return 0;uwvm_bounded_copy({a}.data,p+*offset,{a}.size);*offset+={a}.size;']
                if member.type not in parent.SCALARS:return [f'  if(!{records[(record.version,member.type)].c_symbol}_read_fields(p,n,offset,&{a}))return 0;']
                width=parent.SCALARS[member.type][1]
                return [f'  if(*offset>n||n-*offset<{width})return 0;{{uint{width*8}_t bits=(uint{width*8}_t)uwvm_bounded_get(p+*offset,{width});uwvm_bounded_copy(&{a},&bits,{width});}}*offset+={width};']
            lines += each(member,'value',read)
        lines += [f'  return {cs}_valid(value);','}',
            f'/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */',
            f'static inline uint32_t {cs}_encode({cs} const* value,void* output,size_t capacity,size_t* length) {{',
            f'  size_t n,offset=0;if(!length||!{cs}_valid(value))return UWVM_BOUNDED_INVALID;n={cs}_wire_size(value);',
            '  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;',
            f'  {cs}_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;','}',
            f'/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */',
            f'static inline uint32_t {cs}_decode(void const* input,size_t length,{cs}* value) {{',
            f'  {cs} next={{0}};size_t offset=0;if(!value||(length&&!input)||length<{record.minimum}u||length>{record.maximum}u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;',
            f'  if(!{cs}_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;','}']
    return '\n'.join(lines+['#undef UWVM_BOUNDED_ASSERT','#endif',''])


def emit_cpp(ordered):
    """Reuse the standalone bounded C codec through the existing typed facade."""
    lines=['/** @file C++20 typed bounded canonical records, independent of native ROS libraries.',
           ' * Includes use the existing roswasmcpp Publisher/Subscription codecs and do not grant',
           ' * aliases. Custom Service/Action component codecs also accept the raw typed RPC facade. */',
           '#pragma once','#include "bounded_records.h"','#include <roswasm/roswasmcpp.hpp>',
           'namespace uwvm_custom {']
    lines += [f'/** Complete bounded guest value for {record.name}. */\nusing {record.symbol}={record.c_symbol};' for record in ordered]
    lines += ['}','namespace roswasmcpp {']
    for record in ordered:
        lines += [f'/** Checked codec for {record.name}; canonical maximum is not an actual message length. */',
            f'template<> struct Codec<uwvm_custom::{record.symbol}> {{',
            f'  static constexpr size_t max_wire_bytes={record.maximum};',
            f'  static constexpr char const* codec_identity={json.dumps(record.codec_identity)};']
        if record.has_native_defaults:
            lines += ['  /** Real C-value declared initialization; T{} intentionally remains zero aggregate storage. */',
                      f'  static uwvm_custom::{record.symbol} default_value() noexcept {{uwvm_custom::{record.symbol} value={{0}};(void){record.c_symbol}_default(&value);return value;}}']
        lines += [f'  static bool encode(uwvm_custom::{record.symbol} const& value,wire::bytes& out) {{',
            f'    if(!{record.c_symbol}_valid(&value))return false;size_t n={record.c_symbol}_wire_size(&value),used=0;wire::bytes next(n);',
            f'    if({record.c_symbol}_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}}',
            f'  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::{record.symbol}& out) {{return !{record.c_symbol}_decode(bytes.data(),bytes.size(),&out);}}','};']
    return '\n'.join(lines+['}',''])


def js_default(member, record, records):
    if member.array=='sequence':return '[]'
    if member.native_default is not None:base=scalar_default_expression(member.type,member.native_default,'javascript')
    elif member.type=='string':base='""'
    elif member.type=='bool':base='false'
    elif member.type in ('uint64','int64'):base='0n'
    elif member.type in ('float32','float64') or member.type in ('uint8','int8','uint16','int16','uint32','int32'):base='0'
    else:base=records[(record.version,member.type)].symbol+'Codec.defaultValue()'
    return f'Array.from({{length:{member.count}}},()=>({base}))' if member.array=='array' else base


def emit_js(parent,records,ordered):
    """Emit complete standalone JavaScript typed codecs with bounded readers."""
    lines=['/** Generated bounded-v2 canonical codecs. No ROS, filesystem or network imports.',
           ' * Records are ordinary owned values. Decode throws before exposing malformed bytes;',
           ' * encode validates the complete value before allocating its one wire output.',
           ' * 64-bit integers remain BigInt. Strings are strict UTF-8 with no embedded NUL.',
           ' * Pass these codec objects to existing SDK typed Node factories explicitly. */',
           'const utf8Encoder=new TextEncoder(),utf8Decoder=new TextDecoder("utf-8",{fatal:true,ignoreBOM:true});',
           'function bad(){throw new TypeError("invalid bounded canonical value");}',
           'function textBytes(value,bound){if(typeof value!=="string"||value.length>bound||value.includes("\\0"))bad();for(let i=0;i<value.length;++i){const c=value.charCodeAt(i);if(c>=0xd800&&c<=0xdbff){if(++i===value.length)bad();const d=value.charCodeAt(i);if(d<0xdc00||d>0xdfff)bad();}else if(c>=0xdc00&&c<=0xdfff)bad();}const b=utf8Encoder.encode(value);if(b.length>bound)bad();return b;}',
           'function integer(value,width,signed){if(width===8){if(typeof value!=="bigint"||value<(signed?-(1n<<63n):0n)||value>(signed?(1n<<63n)-1n:(1n<<64n)-1n))bad();}else if(!Number.isInteger(value)||value<(signed?-(2**(width*8-1)):0)||value>(signed?2**(width*8-1)-1:2**(width*8)-1))bad();}',
           'class Reader{constructor(bytes){if(!(bytes instanceof Uint8Array)||bytes.length>32768)bad();this.b=bytes;this.v=new DataView(bytes.buffer,bytes.byteOffset,bytes.byteLength);this.at=0;}take(n){if(!Number.isInteger(n)||n<0||n>this.b.length-this.at)bad();const p=this.at;this.at+=n;return p;}u32(){return this.v.getUint32(this.take(4),true);}text(bound){const n=this.u32();if(n>bound)bad();const p=this.take(n),s=utf8Decoder.decode(this.b.subarray(p,p+n));if(s.includes("\\0"))bad();return s;}}',
           'class Writer{constructor(size){if(size>32768)bad();this.b=new Uint8Array(size);this.v=new DataView(this.b.buffer);this.at=0;}text(value,bound){const b=textBytes(value,bound);this.v.setUint32(this.at,b.length,true);this.at+=4;this.b.set(b,this.at);this.at+=b.length;}}']
    for record in ordered:
        sym=record.symbol
        lines += [f'/** Complete ROS{record.version} {record.name} codec; maxima are not actual lengths. */',
                  f'export const {sym}Codec=Object.freeze({{codecIdentity:{json.dumps(record.codec_identity)},maxWireBytes:{record.maximum},minWireBytes:{record.minimum},',
                  'defaultValue(){return {'+','.join(json.dumps(m.name)+':'+js_default(m,record,records) for m in record.members)+'};},',
                  f'validateValue(value){{{sym}Size(value);return true;}},',
                  f'encode(value){{const n={sym}Size(value),w=new Writer(n);{sym}Write(value,w);if(w.at!==n)bad();return w.b;}},',
                  f'decode(bytes){{if(bytes.length<{record.minimum}||bytes.length>{record.maximum})bad();const r=new Reader(bytes),value={sym}Read(r);if(r.at!==bytes.length)bad();return value;}},',
                  'valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});',
                  f'function {sym}Size(value){{if(value===null||typeof value!=="object")bad();let n=0;']
        for member in record.members:
            a='value['+json.dumps(member.name)+']'
            if member.array:
                compare='!==' if member.array=='array' else '>'
                lines += [f'if(!Array.isArray({a})||{a}.length{compare}{member.count})bad();']
                if member.array=='sequence':lines += ['n+=4;']
                lines += [f'for(const element of {a}){{'];a='element'
            if member.type=='string':lines += [f'n+=4+textBytes({a},{member.string_bound}).length;']
            elif member.type not in parent.SCALARS:lines += [f'n+={records[(record.version,member.type)].symbol}Size({a});']
            elif member.type=='bool':lines += [f'if(typeof {a}!=="boolean")bad();n+=1;']
            elif member.type.startswith('float'):
                lines += [f'if(typeof {a}!=="number"||!Number.isFinite({a})'+(f'||!Number.isFinite(Math.fround({a}))' if member.type=='float32' else '')+f')bad();n+={parent.SCALARS[member.type][1]};']
            else:
                lines += [f'integer({a},{parent.SCALARS[member.type][1]},{str(member.type.startswith("int")).lower()});n+={parent.SCALARS[member.type][1]};']
            if member.array:lines += ['}']
        if semantic_condition(record, 'js', 'value'):
            lines += ['if('+semantic_condition(record, 'js', 'value')+')bad();']
        lines += [f'if(n>{record.maximum})bad();return n;}}',f'function {sym}Write(value,w){{']
        for member in record.members:
            a='value['+json.dumps(member.name)+']'
            if member.array=='sequence':lines += [f'w.v.setUint32(w.at,{a}.length,true);w.at+=4;']
            if member.array:lines += [f'for(const element of {a}){{'];a='element'
            if member.type=='string':lines += [f'w.text({a},{member.string_bound});']
            elif member.type not in parent.SCALARS:lines += [f'{records[(record.version,member.type)].symbol}Write({a},w);']
            else:
                width=parent.SCALARS[member.type][1]
                method='Uint8' if member.type=='bool' else {'u8':'Uint8','i8':'Int8','u16':'Uint16','i16':'Int16','u32':'Uint32','i32':'Int32','u64':'BigUint64','i64':'BigInt64','f32':'Float32','f64':'Float64'}[parent.SCALARS[member.type][0]]
                lines += [f'w.v.set{method}(w.at,{a}'+('?1:0' if member.type=='bool' else '')+f',true);w.at+={width};']
            if member.array:lines += ['}']
        lines += ['}',f'function {sym}Read(r){{const value={{}};']
        for member in record.members:
            lines += ['{']
            a='value['+json.dumps(member.name)+']'
            if member.array:
                lines += [f'{{const count='+('r.u32();' if member.array=='sequence' else str(member.count)+';')+f'if(count>{member.count})bad();{a}=[];for(let i=0;i<count;++i){{']
            if member.type=='string':expression=f'r.text({member.string_bound})'
            elif member.type not in parent.SCALARS:expression=f'{records[(record.version,member.type)].symbol}Read(r)'
            else:
                width=parent.SCALARS[member.type][1]
                method='Uint8' if member.type=='bool' else {'u8':'Uint8','i8':'Int8','u16':'Uint16','i16':'Int16','u32':'Uint32','i32':'Int32','u64':'BigUint64','i64':'BigInt64','f32':'Float32','f64':'Float64'}[parent.SCALARS[member.type][0]]
                expression=f'r.v.get{method}(r.take({width}),true)'
            lines += [f'const decoded={expression};']
            if member.type=='bool':lines += ['if(decoded>1)bad();']
            if member.type.startswith('float'):lines += ['if(!Number.isFinite(decoded))bad();']
            decoded='decoded!==0' if member.type=='bool' else 'decoded'
            lines += [f'{a}.push({decoded});' if member.array else f'{a}={decoded};']
            if member.array:lines += ['}}']
            lines += ['}']
        if semantic_condition(record, 'js', 'value'):
            lines += ['if('+semantic_condition(record, 'js', 'value')+')bad();']
        lines += ['return value;}']
    return '\n'.join(lines+[''])


def rust_element(parent,record,member,records):
    if member.type=='string':return 'String'
    return parent.SCALARS[member.type][3] if member.type in parent.SCALARS else records[(record.version,member.type)].symbol


def rust_member_name(name):
    """Spell the admitted official HG `fn` field as a Rust raw identifier.

    This source-only spelling is used at every declaration/init/access site;
    Field.name, canonical IR, native C++ traits and all other emitters retain
    exact `fn`. The parser still refuses other language keywords/collisions;
    no broad Rust-keyword exception or fixed-v1 spelling change is implied.
    """
    return 'r#fn' if name == 'fn' else name


def emit_rust(parent,records,ordered):
    """Emit owned Rust records with a bounded, shared-cursor Canonical codec."""
    # Generated container loops bind a local `count`. Keep the cursor helper
    # distinct so fixed arrays of strings can still read each element's length.
    lines=['//! Generated bounded-v2 canonical values; no I/O, threads, clocks or authority.',
           '//! Enable the existing SDK alloc feature. Containers/UTF-8 are validated before',
           '//! output allocation; decoder lengths are checked before bounded allocations.',
           '//! Empty interface components are zero-sized semantic values, not native byte copies.',
           'extern crate alloc;','use alloc::{vec::Vec,string::String};','use roswasm::{Error,codes};','use roswasm::typed::Canonical;',
           'fn invalid()->Error {Error::new(codes::INVALID)}',
           'fn take<\'a>(bytes:&\'a [u8],at:&mut usize,n:usize)->Result<&\'a [u8],Error>{if *at>bytes.len()||n>bytes.len()-*at{return Err(invalid());}let start=*at;*at+=n;Ok(&bytes[start..*at])}',
           'fn read_count(bytes:&[u8],at:&mut usize)->Result<usize,Error>{Ok(u32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?) as usize)}',
           'fn put(out:&mut [u8],at:&mut usize,bytes:&[u8]){out[*at..*at+bytes.len()].copy_from_slice(bytes);*at+=bytes.len();}']
    for record in ordered:
        sym=record.symbol
        lines += [f'/// Complete bounded value for ROS{record.version} {record.name}.', '#[derive(Clone,Debug,PartialEq)]',f'pub struct {sym} {{']
        for member in record.members:
            type_name=rust_element(parent,record,member,records)
            if member.array=='array':type_name=f'[{type_name};{member.count}]'
            elif member.array=='sequence':type_name=f'Vec<{type_name}>'
            lines += [f'  /// Declared {member.name}; canonical bounds are part of CODEC_IDENTITY.',f'  pub {rust_member_name(member.name)}:{type_name},']
        lines += ['}',f'impl Default for {sym} {{fn default()->Self {{Self {{']
        lines += [rust_member_name(m.name)+':'+(scalar_default_expression(m.type,m.native_default,'rust') if m.native_default is not None else 'core::array::from_fn(|_|Default::default())' if m.array=='array' else 'Default::default()')+',' for m in record.members]
        lines += ['}}}',f'impl {sym} {{',f'  pub const MAX_WIRE_BYTES:usize={record.maximum};',f'  pub const CODEC_IDENTITY:&\'static str={json.dumps(record.codec_identity)};',
                  '  /// Validate all fields and measure only their initialized canonical prefix.',
                  '  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;']
        for member in record.members:
            a='self.'+rust_member_name(member.name)
            if member.array:
                if member.array=='sequence':lines += [f'if {a}.len()>{member.count}{{return Err(invalid());}}n+=4;']
                lines += [f'for element in &{a} {{'];a='element'
            if member.type=='string':lines += [f'if {a}.len()>{member.string_bound}||{a}.as_bytes().contains(&0){{return Err(invalid());}}n+=4+{a}.len();']
            elif member.type not in parent.SCALARS:lines += [f'n+={a}.wire_size()?;']
            else:
                if member.type.startswith('float'):lines += [f'if !{a}.is_finite(){{return Err(invalid());}}']
                lines += [f'n+={parent.SCALARS[member.type][1]};']
            if member.array:lines += ['}']
        if semantic_condition(record, 'rust', 'self'):
            lines += ['if '+semantic_condition(record, 'rust', 'self')+'{return Err(invalid());}']
        lines += [f'if n>{record.maximum}{{return Err(invalid());}}Ok(n)}}',
                  '  /// Encode after complete validation; output remains unchanged on refusal.',
                  '  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}',
                  '  fn write_fields(&self,out:&mut [u8],at:&mut usize){']
        for member in record.members:
            a='self.'+rust_member_name(member.name)
            if member.array=='sequence':lines += [f'put(out,at,&({a}.len() as u32).to_le_bytes());']
            if member.array:lines += [f'for element in &{a} {{'];a='element'
            if member.type=='string':lines += [f'put(out,at,&({a}.len() as u32).to_le_bytes());put(out,at,{a}.as_bytes());']
            elif member.type not in parent.SCALARS:lines += [f'{a}.write_fields(out,at);']
            elif member.type=='bool':lines += [f'put(out,at,&[u8::from('+('*element' if member.array else a)+')]);']
            else:lines += [f'put(out,at,&{a}.to_le_bytes());']
            if member.array:lines += ['}']
        lines += ['}', '  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();']
        for member in record.members:
            a='value.'+rust_member_name(member.name)
            if member.array:
                lines += ['{let count='+('read_count(bytes,at)?;' if member.array=='sequence' else str(member.count)+'usize;')+f'if count>{member.count}{{return Err(invalid());}}']
                if member.array=='sequence':lines += [f'{a}=Vec::with_capacity(count);']
                lines += ['for i in 0..count {']
            if member.type=='string':
                lines += [f'let size=read_count(bytes,at)?;if size>{member.string_bound}{{return Err(invalid());}}let raw=take(bytes,at,size)?;if raw.contains(&0){{return Err(invalid());}}let decoded=core::str::from_utf8(raw).map_err(|_|invalid())?.into();']
            elif member.type not in parent.SCALARS:lines += [f'let decoded={records[(record.version,member.type)].symbol}::read_fields(bytes,at)?;']
            elif member.type=='bool':lines += ['let byte=take(bytes,at,1)?[0];if byte>1{return Err(invalid());}let decoded=byte!=0;']
            else:
                width=parent.SCALARS[member.type][1]
                lines += [f'let decoded={parent.SCALARS[member.type][3]}::from_le_bytes(take(bytes,at,{width})?.try_into().map_err(|_|invalid())?);']
                if member.type.startswith('float'):lines += ['if !decoded.is_finite(){return Err(invalid());}']
            lines += [f'{a}.push(decoded);' if member.array=='sequence' else f'{a}[i]=decoded;' if member.array=='array' else f'{a}=decoded;']
            if member.array:lines += ['}}']
        if semantic_condition(record, 'rust', 'value'):
            lines += ['if '+semantic_condition(record, 'rust', 'value')+'{return Err(invalid());}']
        lines += ['Ok(value)}','}',f'impl Canonical for {sym} {{',
                  '  /// Allocate one exact validated canonical output; never native serialization.',
                  f'  fn encode(&self)->Result<Vec<u8>,Error>{{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];self.encode_into(&mut out)?;Ok(out)}}',
                  f'  fn decode(bytes:&[u8])->Result<Self,Error>{{if bytes.len()<{record.minimum}||bytes.len()>{record.maximum}{{return Err(invalid());}}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){{return Err(invalid());}}Ok(value)}}','}']
    return '\n'.join(lines+[''])


def emit_go(parent,records,ordered):
    """Emit Go/TinyGo Message values without reflection, native I/O or retries."""
    lines=['// Code generated by bounded_profile.py. Bounded canonical records perform no I/O.',
           '// Put this file in a separate customtypes package and use the selected SDK backend.',
           '// Encode validates complete values before allocation. Decode never returns a partial value.',
           'package customtypes','import("encoding/binary";"math";"errors";"unicode/utf8";"strings")',
           'var boundedInvalid=errors.New("invalid bounded canonical record")',
           'var _=math.Float64bits;var _=utf8.Valid;var _=strings.IndexByte',
           'func boundedTake(b []byte,at *int,n int)([]byte,error){if n<0||*at>len(b)||n>len(b)-*at{return nil,boundedInvalid};start:=*at;*at+=n;return b[start:*at],nil}',
           'func boundedCount(b []byte,at *int)(uint32,error){raw,err:=boundedTake(b,at,4);if err!=nil{return 0,err};return binary.LittleEndian.Uint32(raw),nil}',
           'func boundedPut(out []byte,at *int,b []byte){copy(out[*at:],b);*at+=len(b)}']
    def go_type(record,member):
        base='string' if member.type=='string' else parent.SCALARS[member.type][4] if member.type in parent.SCALARS else records[(record.version,member.type)].symbol
        return '['+str(member.count)+']'+base if member.array=='array' else '[]'+base if member.array=='sequence' else base
    for record in ordered:
        sym=record.symbol
        lines += [f'// {sym} preserves every declared ROS{record.version} {record.name} field.',f'type {sym} struct {{']
        lines += [f'// {m.name}: explicit canonical bounds are committed in CodecIdentity.\n{m.name[:1].upper()+m.name[1:]} {go_type(record,m)}' for m in record.members]
        lines += ['}',f'const {sym}CodecIdentity={json.dumps(record.codec_identity)}',f'const {sym}MaxWireBytes={record.maximum}']
        if record.has_native_defaults:
            lines += ['// Explicit declared initialization; the Go zero value is deliberately not relabeled.',
                      f'func Default{sym}(){sym}{{value:={sym}{{}}']
            for member in record.members:
                field_name=member.name[:1].upper()+member.name[1:]
                if member.native_default is not None:
                    expression=scalar_default_expression(member.type,member.native_default,'tinygo')
                    lines += [f'value.{field_name}={expression}']
                elif member.type not in parent.SCALARS and member.type != 'string':
                    child=records[(record.version,member.type)]
                    if child.has_native_defaults and member.array != 'sequence':
                        if member.array == 'array':
                            lines += [f'for i:=range value.{field_name}{{value.{field_name}[i]=Default{child.symbol}()}}']
                        else:lines += [f'value.{field_name}=Default{child.symbol}()']
            lines += ['return value}']
        lines += [f'// WireSize validates all active elements and measures exact canonical bytes.\nfunc(value {sym}) WireSize()(int,error){{n:=0']
        for member in record.members:
            lines += ['{']
            a='value.'+member.name[:1].upper()+member.name[1:]
            if member.array:
                if member.array=='sequence':lines += [f'if len({a})>{member.count}{{return 0,boundedInvalid}};n+=4']
                lines += [f'for _,element:=range {a} {{_ = element'];a='element'
            if member.type=='string':lines += [f'if len({a})>{member.string_bound}||!utf8.ValidString({a})||strings.IndexByte({a},0)>=0{{return 0,boundedInvalid}};n+=4+len({a})']
            elif member.type not in parent.SCALARS:lines += [f'size,err:={a}.WireSize();if err!=nil{{return 0,err}};n+=size']
            else:
                if member.type.startswith('float'):lines += [f'if math.IsNaN(float64({a}))||math.IsInf(float64({a}),0){{return 0,boundedInvalid}}']
                lines += [f'n+={parent.SCALARS[member.type][1]}']
            if member.array:lines += ['}']
            lines += ['}']
        if semantic_condition(record, 'go', 'value'):
            lines += ['if '+semantic_condition(record, 'go', 'value')+'{return 0,boundedInvalid}']
        lines += [f'if n>{record.maximum}{{return 0,boundedInvalid}};return n,nil}}',
                  f'// Encode satisfies the SDK Message interface with one bounded output allocation.\nfunc(value {sym}) Encode()([]byte,error){{n,err:=value.WireSize();if err!=nil{{return nil,err}};out:=make([]byte,n);at:=0;value.writeBounded(out,&at);return out,nil}}',
                  f'func(value {sym}) writeBounded(out []byte,at *int){{']
        for member in record.members:
            a='value.'+member.name[:1].upper()+member.name[1:]
            if member.array=='sequence':lines += [f'binary.LittleEndian.PutUint32(out[*at:],uint32(len({a})));*at+=4']
            if member.array:lines += [f'for _,element:=range {a} {{'];a='element'
            if member.type=='string':lines += [f'binary.LittleEndian.PutUint32(out[*at:],uint32(len({a})));*at+=4;copy(out[*at:],{a});*at+=len({a})']
            elif member.type not in parent.SCALARS:lines += [f'{a}.writeBounded(out,at)']
            elif member.type=='bool':lines += [f'if {a}{{out[*at]=1}}else{{out[*at]=0}};*at+=1']
            else:
                width=parent.SCALARS[member.type][1]
                if width==1:lines += [f'out[*at]=byte({a});*at+=1']
                else:
                    bits=f'math.Float{width*8}bits({a})' if member.type.startswith('float') else f'uint{width*8}({a})'
                    lines += [f'binary.LittleEndian.PutUint{width*8}(out[*at:],{bits});*at+={width}']
            if member.array:lines += ['}']
        lines += ['}',f'func read{sym}(b []byte,at *int)({sym},error){{var value {sym}']
        for index,member in enumerate(record.members):
            lines += ['{'];a='value.'+member.name[:1].upper()+member.name[1:]
            if member.array:
                lines += [('count,err:=boundedCount(b,at);if err!=nil{return '+sym+'{},err};' if member.array=='sequence' else 'count:=uint32('+str(member.count)+');')+f'if count>{member.count}{{return {sym}{{}},boundedInvalid}}']
                if member.array=='sequence':lines += [f'{a}=make({go_type(record,member)},int(count))']
                lines += ['for i:=0;i<int(count);i++{'];a+='[i]'
            if member.type=='string':lines += [f'size,err:=boundedCount(b,at);if err!=nil||size>{member.string_bound}{{return {sym}{{}},boundedInvalid}};raw,err:=boundedTake(b,at,int(size));if err!=nil||!utf8.Valid(raw){{return {sym}{{}},boundedInvalid}};{a}=string(raw);if strings.IndexByte({a},0)>=0{{return {sym}{{}},boundedInvalid}}']
            elif member.type not in parent.SCALARS:lines += [f'decoded,err:=read{records[(record.version,member.type)].symbol}(b,at);if err!=nil{{return {sym}{{}},err}};{a}=decoded']
            else:
                width=parent.SCALARS[member.type][1]
                lines += [f'raw,err:=boundedTake(b,at,{width});if err!=nil{{return {sym}{{}},err}}']
                if member.type=='bool':lines += [f'if raw[0]>1{{return {sym}{{}},boundedInvalid}};{a}=raw[0]!=0']
                else:
                    bits='raw[0]' if width==1 else f'binary.LittleEndian.Uint{width*8}(raw)'
                    expression=f'math.Float{width*8}frombits({bits})' if member.type.startswith('float') else f'{parent.SCALARS[member.type][4]}({bits})'
                    lines += [f'{a}={expression}']
                    if member.type.startswith('float'):lines += [f'if math.IsNaN(float64({a}))||math.IsInf(float64({a}),0){{return {sym}{{}},boundedInvalid}}']
            if member.array:lines += ['}']
            lines += ['}']
        if semantic_condition(record, 'go', 'value'):
            lines += ['if '+semantic_condition(record, 'go', 'value')+'{return '+sym+'{},boundedInvalid}']
        lines += ['return value,nil}',f'// Decode{sym} rejects every trailing/truncated/invalid prefix before exposing a value.',
                  f'func Decode{sym}(b []byte)({sym},error){{if len(b)<{record.minimum}||len(b)>{record.maximum}{{return {sym}{{}},boundedInvalid}};at:=0;value,err:=read{sym}(b,&at);if err!=nil||at!=len(b){{return {sym}{{}},boundedInvalid}};return value,nil}}']
    return '\n'.join(lines+[''])


def emit_as(parent,records,ordered):
    """Emit AssemblyScript classes using the checked SDK Reader/Writer contract."""
    lines=['/** Generated bounded-v2 canonical values; no native I/O or authority.',
           ' * Move only the relative SDK module path if relocating this generated directory.',
           ' * Managed values are encoded into linear-memory byte arrays before an import.',
           ' * valid() must succeed before exposing decode(); malformed decode returns a default. */',
           'import {Codec,Reader,Writer,validUnicode,validUtf8} from "../../assemblyscript/codecs";']
    def base(member,record):
        return 'string' if member.type=='string' else parent.SCALARS[member.type][6] if member.type in parent.SCALARS else records[(record.version,member.type)].symbol
    def default(member,record):
        return scalar_default_expression(member.type,member.native_default,'assemblyscript') if member.native_default is not None else '""' if member.type=='string' else 'false' if member.type=='bool' else '0' if member.type in parent.SCALARS else 'new '+records[(record.version,member.type)].symbol+'()'
    for record in ordered:
        sym=record.symbol
        lines += [f'/** Complete ROS{record.version} {record.name} managed value. */',f'export class {sym} {{']
        for member in record.members:
            typ=base(member,record)
            if member.array:
                typ='Array<'+typ+'>';value='new '+typ+'()'
            else:value=default(member,record)
            lines += [f'/** Declared {member.name}; initialized elements alone are serialized. */',f'{member.name}:{typ}={value};']
        if any(m.array=='array' for m in record.members):
            lines += ['constructor(){']
            for member in record.members:
                if member.array=='array':lines += [f'for(let i=0;i<{member.count};++i)this.{member.name}.push({default(member,record)});']
            lines += ['}']
        lines += [f'/** Verify UTF-16/scalar/container limits before any wire allocation. */',f'validValue():bool {{']
        for member in record.members:
            a='this.'+member.name
            if member.array:
                compare='!=' if member.array=='array' else '>'
                lines += [f'if({a}.length{compare}{member.count})return false;',f'for(let i=0;i<{a}.length;++i){{'];a+='[i]'
            if member.type=='string':lines += [f'if(!validUnicode({a})||{a}.indexOf("\\0")>=0||String.UTF8.byteLength({a})>{member.string_bound})return false;']
            elif member.type not in parent.SCALARS:lines += [f'if(!{a}.validValue())return false;']
            elif member.type.startswith('float'):lines += [f'if(!isFinite({a}))return false;']
            if member.array:lines += ['}']
        if semantic_condition(record, 'as', 'this'):
            lines += ['if('+semantic_condition(record, 'as', 'this')+')return false;']
        lines += ['return true;}','/** Internal shared writer; the public codec first validates the complete value. */','writeBounded(w:Writer):void {']
        for member in record.members:
            a='this.'+member.name
            if member.array=='sequence':lines += [f'w.u32(<u32>{a}.length);']
            if member.array:lines += [f'for(let i=0;i<{a}.length;++i){{'];a+='[i]'
            if member.type=='string':lines += [f'w.text({a});']
            elif member.type not in parent.SCALARS:lines += [f'{a}.writeBounded(w);']
            elif member.type=='bool':lines += [f'w.u8({a}?1:0);']
            elif member.type.startswith('float'):
                width=parent.SCALARS[member.type][1];lines += [f'w.integer(<u64>reinterpret<u{width*8}>({a}),{width});']
            else:lines += [f'w.integer(<u64>{a},{parent.SCALARS[member.type][1]});']
            if member.array:lines += ['}']
        lines += ['}',f'/** Private-by-convention bounded decoder; only codec.valid exposes acceptance. */',f'static readBounded(r:Reader):{sym} {{const value=new {sym}();']
        for member in record.members:
            lines += ['{'];a='value.'+member.name
            if member.array:
                lines += [('const count=r.u32();' if member.array=='sequence' else 'const count:u32='+str(member.count)+';')+f'if(!r.valid||count>{member.count}){{r.valid=false;return value;}}',
                          f'{a}=new Array<{base(member,record)}>();for(let i:u32=0;i<count;++i){{']
            if member.type=='string':lines += [f'const n=r.u32();if(n>{member.string_bound}){{r.valid=false;return value;}}const b=r.take(n);if(!r.valid||!validUtf8(b,false)){{r.valid=false;return value;}}const decoded=String.UTF8.decode(b.buffer);']
            elif member.type not in parent.SCALARS:lines += [f'const decoded={records[(record.version,member.type)].symbol}.readBounded(r);']
            elif member.type=='bool':lines += ['const bit=r.u8();if(bit>1)r.valid=false;const decoded=bit!=0;']
            elif member.type.startswith('float'):
                width=parent.SCALARS[member.type][1];lines += [f'const decoded=reinterpret<f{width*8}>(<u{width*8}>r.integer({width}));if(!isFinite(decoded))r.valid=false;']
            else:lines += [f'const decoded=<{base(member,record)}>r.integer({parent.SCALARS[member.type][1]});']
            lines += [f'{a}.push(decoded);' if member.array else f'{a}=decoded;']
            if member.array:lines += ['}']
            lines += ['}']
        if semantic_condition(record, 'as', 'value'):
            lines += ['if('+semantic_condition(record, 'as', 'value')+')r.valid=false;']
        lines += ['return value;}','}',f'/** SDK typed Codec for {record.name}; no native registration is implied. */',f'export class {sym}Codec implements Codec<{sym}> {{',
                  f'static readonly codecIdentity:string={json.dumps(record.codec_identity)};',f'static readonly maxWireBytes:i32={record.maximum};',
                  f'encode(value:{sym}):Uint8Array {{if(!value.validValue())return new Uint8Array(0);const w=new Writer({record.maximum});value.writeBounded(w);return w.finish();}}',
                  f'valid(bytes:Uint8Array):bool {{if(bytes.length<{record.minimum}||bytes.length>{record.maximum})return false;const r=new Reader(bytes);{sym}.readBounded(r);return r.done();}}',
                  f'decode(bytes:Uint8Array):{sym} {{if(!this.valid(bytes))return new {sym}();const r=new Reader(bytes);return {sym}.readBounded(r);}}','}']
    return '\n'.join(lines+[''])


def emit_manifest(parent,records,ordered,interfaces):
    """Emit one immutable common provider shared by Worker and both Gateways.

    The exact original control domain is source-selected and never a Guest
    registry/upload or capability. A preparation catalog has no native IDs:
    blank identities deliberately make its manifest unavailable until a future
    explicit catalog freezes independently exported actual native identities.
    Native Gateway profile proof is still independently mandatory.
    """
    control = any(record.profile == CONTROL_PROFILE for record in ordered)
    if control and not all(record.profile == CONTROL_PROFILE for record in ordered):
        raise parent.GenerationError('original control and generic registry domains cannot mix')
    lines=['/** @file Immutable bounded-v2 canonical registry; include through the trusted build only.',
           ' * Official native identities are caller-exported and verified again by native type support.',
           ' * Canonical schemas register supported types; every operation still needs an explicit alias. */',
           '#pragma once']
    if control:
        # Separate trusted provider; never suppress the legacy empty/global
        # service functions or overwrite a concurrently selected vendor table.
        lines += ['namespace uwvm2::imported::ros::ipc::original_control_generated {']
    else:
        lines += ['#define UWVM_ROS_CUSTOM_BOUNDED_INTERFACES 1',
                  'namespace uwvm2::imported::ros::ipc {']
    for record in ordered:
        def node(member):
            if member.type=='string':return '{custom_wire_kind::'+('raw_bytes' if member.string_encoding=='raw_bytes' else 'utf8')+','+str(member.string_bound)+',nullptr,0}'
            if member.type in parent.SCALARS:return '{custom_wire_kind::'+parent.SCALARS[member.type][0]+',0,nullptr,0}'
            return 'custom_schema_'+records[(record.version,member.type)].symbol
        for index,member in enumerate(record.members):
            if member.array:
                lines += [f'inline constexpr custom_wire_node custom_element_{record.symbol}_{index}='+node(member)+';']
        children=[]
        for index,member in enumerate(record.members):
            children.append('{custom_wire_kind::'+member.array+','+str(member.count)+f',&custom_element_{record.symbol}_{index},1'+'}' if member.array else node(member))
        record_kind = 'bounded_record' if record.aggregate_wire_bytes else 'record'
        record_bound = record.aggregate_wire_bytes
        if children:
            lines += [f'inline constexpr custom_wire_node custom_children_{record.symbol}[]={{'+','.join(children)+'};',
                      f'inline constexpr custom_wire_node custom_schema_{record.symbol}{{custom_wire_kind::{record_kind},{record_bound},custom_children_{record.symbol},{len(children)}}};']
        else:lines += [f'inline constexpr custom_wire_node custom_schema_{record.symbol}{{custom_wire_kind::{record_kind},{record_bound},nullptr,0}};']
        native_identity = record.identity or ''
        lines += [f'inline constexpr custom_topic_descriptor custom_record_{record.symbol}{{{record.version},{json.dumps(record.name)},{json.dumps(native_identity)},{json.dumps(record.codec_identity)},{record.maximum},nullptr,0,&custom_schema_{record.symbol},{record.minimum}'+(',false,true' if control else ',true' if record.profile==STANDARD_PROFILE and (record.version, record.name) in standard_topic_names() else '')+'};']
    topics=[interface.components['message'] for interface in interfaces if interface.kind=='message' and interface.registered]
    if topics:
        lines += ['inline constexpr custom_topic_descriptor custom_generated_topics[]={'+','.join('custom_record_'+r.symbol for r in topics)+'};',
                  'inline constexpr custom_topic_registry custom_topic_entries() noexcept {return {custom_generated_topics,sizeof(custom_generated_topics)/sizeof(custom_generated_topics[0])};}']
    else:lines += ['inline constexpr custom_topic_registry custom_topic_entries() noexcept {return {nullptr,0};}']
    for kind in ('service','action'):
        selected=[interface for interface in interfaces if interface.kind==kind and (not control or interface.registered)]
        roles=('request','response') if kind=='service' else ('goal','result','feedback')
        if selected:
            lines += [f'inline constexpr custom_{kind}_descriptor custom_generated_{kind}s[]={{']
            for interface in selected:
                lines += ['{'+','.join([str(interface.version),json.dumps(interface.name),json.dumps((interface.identity or '') if control else interface.identity),json.dumps(interface.codec_identity)]+['&custom_record_'+interface.components[role].symbol for role in roles])+'},']
            lines += ['};',f'inline constexpr custom_{kind}_registry custom_{kind}_entries() noexcept {{return {{custom_generated_{kind}s,sizeof(custom_generated_{kind}s)/sizeof(custom_generated_{kind}s[0])}};}}']
        else:lines += [f'inline constexpr custom_{kind}_registry custom_{kind}_entries() noexcept {{return {{nullptr,0}};}}']
    return '\n'.join(lines+['}',''])


def native_definition(record):
    """Preserve official ROS1 field grammar; canonical limits stay separate."""
    return ''.join(m.type+('[]' if m.array=='sequence' else '['+str(m.count)+']' if m.array=='array' else '')+' '+m.name+(' '+m.native_default if m.native_default is not None else '')+'\n' for m in record.members)


def ros2_interface_schema(parent, records, interface):
    """Project the complete official RIHS01 Service/Action root declarations.

    No identity is invented here. The supplied native identity is compared at
    Gateway startup to a native baked hash, or (on pre-Iron SDKs) to the RIHS01
    recomputation of this complete declaration AFTER native payload metadata has
    independently matched. Service roots include events; Action roots include
    both protocol services and their events, UUID and Time. Sources:
    https://github.com/ros2/rosidl/blob/jazzy/rosidl_parser/rosidl_parser/definition.py
    https://github.com/ros2/rosidl/blob/jazzy/rosidl_generator_type_description/rosidl_generator_type_description/__init__.py
    https://github.com/ros2/rcl_interfaces/blob/jazzy/service_msgs/msg/ServiceEventInfo.msg
    The .msg char field is IDL uint8 per rosidl_adapter/msg/__init__.py. Empty
    .msg components retain the official uint8 representation placeholder.
    """
    declarations = {}

    def scalar(name, type_, array_size=0, sequence=False, capacity=0):
        return (name, type_, array_size, bool(array_size), 0, sequence, capacity)

    def message(record):
        if record.name in declarations:
            return
        fields = [(member.name, member.type,
                   member.count if member.array == 'array' else 0,
                   member.array == 'array', member.native_string_bound,
                   member.array == 'sequence', member.native_sequence_bound) +
                  ((member.native_default,) if member.native_default is not None else ())
                  for member in record.members]
        if not fields:
            fields = [scalar('structure_needs_at_least_one_member', 'uint8')]
        declarations[record.name] = fields
        for member in record.members:
            if member.type not in parent.SCALARS and member.type != 'string':
                message(records[(2, member.type)])

    for component in interface.components.values():
        message(component)
    declarations['builtin_interfaces/msg/Time'] = [scalar('sec', 'int32'), scalar('nanosec', 'uint32')]
    declarations['service_msgs/msg/ServiceEventInfo'] = [
        scalar('event_type', 'uint8'), scalar('stamp', 'builtin_interfaces/msg/Time'),
        scalar('client_gid', 'uint8', 16), scalar('sequence_number', 'int64')]

    def service(name, request, response):
        declarations[name] = [scalar('request_message', request),
                              scalar('response_message', response), scalar('event_message', name + '_Event')]
        declarations[name + '_Event'] = [scalar('info', 'service_msgs/msg/ServiceEventInfo'),
                                        scalar('request', request, sequence=True, capacity=1),
                                        scalar('response', response, sequence=True, capacity=1)]

    name = interface.name
    if interface.kind == 'service':
        service(name, interface.components['request'].name, interface.components['response'].name)
    elif interface.kind == 'action':
        declarations['unique_identifier_msgs/msg/UUID'] = [scalar('uuid', 'uint8', 16)]
        goal, result, feedback = (interface.components[part].name for part in ('goal', 'result', 'feedback'))
        send, get = name + '_SendGoal', name + '_GetResult'
        declarations[send + '_Request'] = [scalar('goal_id', 'unique_identifier_msgs/msg/UUID'), scalar('goal', goal)]
        declarations[send + '_Response'] = [scalar('accepted', 'bool'), scalar('stamp', 'builtin_interfaces/msg/Time')]
        declarations[get + '_Request'] = [scalar('goal_id', 'unique_identifier_msgs/msg/UUID')]
        declarations[get + '_Response'] = [scalar('status', 'int8'), scalar('result', result)]
        service(send, send + '_Request', send + '_Response')
        service(get, get + '_Request', get + '_Response')
        declarations[name + '_FeedbackMessage'] = [scalar('goal_id', 'unique_identifier_msgs/msg/UUID'), scalar('feedback', feedback)]
        declarations[name] = [scalar('goal', goal), scalar('result', result), scalar('feedback', feedback),
                              scalar('send_goal_service', send), scalar('get_result_service', get),
                              scalar('feedback_message', name + '_FeedbackMessage')]
    else:
        raise parent.GenerationError('a complete ROS2 interface root requires a service or action')
    # Emit only the exact reachable closure; disconnected standard projections
    # must never be accepted just because their definitions look familiar.
    selected = []
    def visit(type_):
        if type_ in selected:
            return
        if type_ not in declarations:
            raise parent.GenerationError('missing official interface declaration: ' + type_)
        selected.append(type_)
        for field in declarations[type_]:
            if field[1] not in parent.SCALARS and field[1] != 'string':
                visit(field[1])
    visit(name)
    if len(selected) > 256:
        raise parent.GenerationError('complete official protocol closure exceeds 256 records')
    return [(type_, declarations[type_]) for type_ in selected]


def emit_ros2_interface_trait(parent, records, interface):
    """Emit a header-only trusted schema trait without Gateway-private includes."""
    lines = ['namespace uwvm2::imported::ros::ipc {',
             '/** Complete official protocol declaration for the selected compiled interface.',
             ' * Native payload metadata and RIHS01 are independently checked at startup.',
             ' * These virtual event/protocol declarations create no native endpoint or grant. */',
             'template<> struct custom_native_interface_schema<::' + interface.name.replace('/', '::') + '> {']
    schema = ros2_interface_schema(parent, records, interface)
    for index, (_, fields) in enumerate(schema):
        entries = []
        for field in fields:
            entries.append('{' + ','.join(json.dumps(value) if isinstance(value, str) else
                                        str(value).lower() if isinstance(value, bool) else str(value)
                                        for value in field) + '}')
        lines.append('inline static constexpr custom_native_field fields_' + str(index) + '[]={' + ','.join(entries) + '};')
    lines.append('inline static constexpr custom_native_record records[]={')
    for index, (type_, fields) in enumerate(schema):
        lines.append('{' + json.dumps(type_) + ',fields_' + str(index) + ',' + str(len(fields)) + '},')
    lines += ['};', 'static constexpr custom_native_schema schema() noexcept {return {records,sizeof(records)/sizeof(records[0])};}', '};', '}']
    return lines


def emit_native(parent,records,ordered,interfaces,version):
    """Emit concrete official native conversions with full structural metadata."""
    selected=[record for record in ordered if record.version==version]
    lines=[f'/** @file Generated ROS{version} bounded-v2 native value conversion.',
           ' * Includes only concrete officially generated message/service/action headers.',
           ' * Native identities and complete metadata must be qualified by the Gateway before',
           ' * any entity creation. valid() allocates nothing; vector encode allocates only',
           ' * after complete validation/measurement. Native decode builds a bounded temporary',
           ' * and commits once. CDR/TCPROS bytes are never forwarded as untyped guest data. */',
           '#pragma once','#include "bounded_records.h"','#include <vector>','#include <array>',
           '#include <string>','#include <type_traits>','#include <utility>',
           '#include <uwvm2/imported/ros/ipc/custom_type.h>']
    if version==1:lines += ['#include <boost/array.hpp>','#include <ros/message_traits.h>','#include <ros/service_traits.h>']
    else:lines += [
        '/** Select the actual SDK container; legacy ROS2 does not provide runtime_cpp. */',
        '#if __has_include(<rosidl_runtime_cpp/bounded_vector.hpp>)',
        '#include <rosidl_runtime_cpp/bounded_vector.hpp>',
        '#elif __has_include(<rosidl_generator_cpp/bounded_vector.hpp>)',
        '#include <rosidl_generator_cpp/bounded_vector.hpp>',
        '#else',
        '#error "Actual ROS2 BoundedVector header required; no replacement container"',
        '#endif',
        '#if __has_include(<rosidl_buffer/buffer.hpp>)', '#include <rosidl_buffer/buffer.hpp>', '#endif']
    heads=set()
    if any(record.version == 1 and record.name in ('time','duration') for record in selected):
        heads.update({'ros/time.h','ros/duration.h'})
    for interface in interfaces:
        if interface.version!=version:continue
        parts=interface.name.split('/')
        head=parts[0]+'/'+(parts[1]+'/' if version==2 else '')+(parent.ros2_header(parts[-1])+'.hpp' if version==2 else parts[-1]+('Action' if interface.kind=='action' else '')+'.h')
        heads.add(head)
    lines += ['#include <'+head+'>' for head in sorted(heads)]
    lines += ['#ifndef UWVM_CUSTOM_BOUNDED_NATIVE_TRAITS','#define UWVM_CUSTOM_BOUNDED_NATIVE_TRAITS',
        'namespace uwvm_custom_bounded_native {',
        '/** Normalized complete ROS1 declaration subsection; never a native hash algorithm. */',
        'struct definition_entry {std::string_view type,fields;};',
        'template<class T> struct fixed_shape {using element=void;static constexpr size_t size=0;};',
        'template<class T,size_t N> struct fixed_shape<std::array<T,N>> {using element=T;static constexpr size_t size=N;};',
        'template<class T> struct sequence_shape {using element=void;static constexpr bool supported=false;static constexpr size_t bound=0;};',
        'template<class T,class A> struct sequence_shape<std::vector<T,A>> {using element=T;static constexpr bool supported=true;static constexpr size_t bound=0;};',
        'template<class T> bool native_accessible(T const&) noexcept {return true;}',
        '/** Only generated empty Service/Action components use this adapter. The Gateway',
        ' * separately attests the complete empty native metadata before an endpoint exists.',
        ' * Pre-IDL empty structs have no dummy member; present members must be exact uint8. */',
        'template<class T,class=void> struct empty_component {static bool zero(T const&) noexcept {return true;}static void initialize(T&) noexcept {}};',
        'template<class T> struct empty_component<T,std::void_t<decltype(std::declval<T&>().structure_needs_at_least_one_member)>> {using member=std::remove_cv_t<std::remove_reference_t<decltype(std::declval<T&>().structure_needs_at_least_one_member)>>;static_assert(std::is_same<member,uint8_t>::value,"official empty component placeholder differs");static bool zero(T const& value) noexcept {return value.structure_needs_at_least_one_member==0;}static void initialize(T& value) noexcept {value.structure_needs_at_least_one_member=0;}};',
        'template<class T> struct string_shape {static constexpr bool supported=false;};',
        'template<class A> struct string_shape<std::basic_string<char,std::char_traits<char>,A>> {static constexpr bool supported=true;};',
        '}','#endif']
    if version==1:
        lines += ['#ifndef UWVM_CUSTOM_BOUNDED_BOOST_TRAITS','#define UWVM_CUSTOM_BOUNDED_BOOST_TRAITS','namespace uwvm_custom_bounded_native {',
                  'template<class T,size_t N> struct fixed_shape<boost::array<T,N>> {using element=T;static constexpr size_t size=N;};','}','#endif']
    else:
        lines += ['#ifndef UWVM_CUSTOM_BOUNDED_ROSIDL_TRAITS','#define UWVM_CUSTOM_BOUNDED_ROSIDL_TRAITS','namespace uwvm_custom_bounded_native {',
                  '#if __has_include(<rosidl_runtime_cpp/bounded_vector.hpp>)',
                  'template<class T,size_t N,class A> struct sequence_shape<::rosidl_runtime_cpp::BoundedVector<T,N,A>> {using element=T;static constexpr bool supported=true;static constexpr size_t bound=N;};',
                  '#elif __has_include(<rosidl_generator_cpp/bounded_vector.hpp>)',
                  'template<class T,size_t N,class A> struct sequence_shape<::rosidl_generator_cpp::BoundedVector<T,N,A>> {using element=T;static constexpr bool supported=true;static constexpr size_t bound=N;};',
                  '#endif',
                  '#if __has_include(<rosidl_buffer/buffer.hpp>)',
                  '/** Lyrical uint8[] uses this exact official container. Reject non-CPU storage',
                  ' * before noexcept validation accesses data; never trigger an implicit GPU copy. */',
                  'template<class T,class A> struct sequence_shape<rosidl::Buffer<T,A>> {using element=T;static constexpr bool supported=std::is_same<T,uint8_t>::value;static constexpr size_t bound=0;};',
                  'template<class T,class A> bool native_accessible(rosidl::Buffer<T,A> const& value) noexcept {return dynamic_cast<rosidl::CpuBufferImpl<T,A> const*>(value.get_impl())!=nullptr;}',
                  '#endif','}','#endif']
    lines += ['namespace uwvm_custom_bounded_native {']
    for record in selected:
        sym,cs=record.symbol,record.c_symbol
        # Native ROS1 time/duration are value primitives, not Message identities.
        native_identity = '' if record.kind == 'native-primitive' else record.identity
        lines += [f'/** Complete official {record.name} converter, including all nested dependencies. */',f'struct {sym}Codec {{',
                  f'using Native={record.native};using Record={cs};',f'static constexpr char const* type={json.dumps(record.name)};',
                  f'static constexpr char const* native_identity={json.dumps(native_identity)};',f'static constexpr char const* codec_identity={json.dumps(record.codec_identity)};',
                  f'static constexpr size_t wire_bytes={record.maximum};static constexpr bool bounded_profile=true;']
        for index,member in enumerate(record.members):
            alias='field_'+str(index);raw='raw_'+str(index)
            lines += [f'using {raw}=std::remove_cv_t<std::remove_reference_t<decltype(std::declval<Native&>().{member.name})>>;']
            if member.array=='array':
                lines += [f'static_assert(fixed_shape<{raw}>::size=={member.count},"official fixed array length differs");',f'using {alias}=typename fixed_shape<{raw}>::element;']
            elif member.array=='sequence':
                lines += [f'static_assert(sequence_shape<{raw}>::supported&&sequence_shape<{raw}>::bound=={member.native_sequence_bound},"official sequence shape/bound differs");',f'using {alias}=typename sequence_shape<{raw}>::element;']
            else:lines += [f'using {alias}={raw};']
            if member.type=='string':lines += [f'static_assert(string_shape<{alias}>::supported,"official field must be an 8-bit allocator-aware string");']
            else:
                expected='bool' if member.type=='bool' and version==2 else parent.SCALARS[member.type][2] if member.type in parent.SCALARS else records[(record.version,member.type)].native
                lines += [f'static_assert(std::is_same<{alias},{expected}>::value,"official scalar/nested field type differs");']
        lines += ['/** Validate native fields without copying and commit the exact active canonical size. */',
                  'static bool encoded_size(Native const& value,size_t& out) noexcept {size_t n=0;']
        if semantic_condition(record, 'native', 'value'):
            lines += ['if('+semantic_condition(record, 'native', 'value')+')return false;']
        if not record.members and version==2:lines += ['if(!empty_component<Native>::zero(value))return false;']
        for member in record.members:
            access='value.'+member.name
            lines += ['{']
            if member.array=='sequence':lines += [f'if(!native_accessible({access})||{access}.size()>{member.count})return false;n+=4;']
            if member.array:
                count=access+'.size()' if member.array=='sequence' else str(member.count)
                lines += [f'for(size_t i=0;i<{count};++i){{'];access += '[i]'
            if member.type=='string':lines += [f'if({access}.size()>{member.string_bound}||!uwvm_bounded_utf8(reinterpret_cast<unsigned char const*>({access}.data()),{access}.size()))return false;n+=4+{access}.size();']
            elif member.type not in parent.SCALARS:lines += [f'size_t nested=0;if(!{records[(record.version,member.type)].symbol}Codec::encoded_size({access},nested))return false;n+=nested;']
            else:
                width=parent.SCALARS[member.type][1]
                if member.type=='bool' and version==1:lines += [f'if({access}!=0&&{access}!=1)return false;']
                if member.type.startswith('float'):
                    mask='UINT32_C(0x7f800000)' if width==4 else 'UINT64_C(0x7ff0000000000000)'
                    lines += [f'uint{width*8}_t bits=0;uwvm_bounded_copy(&bits,&{access},{width});if((bits&{mask})=={mask})return false;']
                lines += [f'n+={width};']
            if member.array:lines += ['}']
            lines += ['}']
        lines += [f'if(n>{record.maximum})return false;out=n;return true;}}']
        def elementwise(member, mode):
            source='value.'+member.name if mode=='to' else 'record.'+member.name
            destination='record.'+member.name if mode=='to' else 'value.'+member.name
            lines=[]
            if member.array=='sequence':
                if mode=='to':lines += [f'if(!native_accessible({source})||{source}.size()>{member.count})return false;{destination}.size=static_cast<uint32_t>({source}.size());']
                else:lines += [f'{destination}.resize({source}.size);']
            if member.array:
                count=source+'.size()' if mode=='to' and member.array=='sequence' else source+'.size' if member.array=='sequence' else str(member.count)
                lines += [f'for(size_t i=0;i<{count};++i){{']
                source += '[i]' if mode=='to' or member.array=='array' else '.data[i]'
                destination += '.data[i]' if mode=='to' and member.array=='sequence' else '[i]'
            if member.type=='string':
                if mode=='to':lines += [f'if({source}.size()>{member.string_bound}||!uwvm_bounded_utf8(reinterpret_cast<unsigned char const*>({source}.data()),{source}.size()))return false;',f'{destination}.size=static_cast<uint32_t>({source}.size());uwvm_bounded_copy({destination}.data,{source}.data(),{source}.size());']
                else:lines += [f'{destination}.assign(reinterpret_cast<char const*>({source}.data),{source}.size);']
            elif member.type not in parent.SCALARS:
                codec=records[(record.version,member.type)].symbol+'Codec'
                lines += [f'if(!{codec}::'+('to_record' if mode=='to' else 'from_record')+f'({source},{destination}))return false;']
            elif member.type.startswith('float'):
                lines += [f'uwvm_bounded_copy(&{destination},&{source},{parent.SCALARS[member.type][1]});']
            else:
                if member.type=='bool' and mode=='to' and version==1:lines += [f'if({source}!=0&&{source}!=1)return false;']
                lines += [f'{destination}={source};']
            if member.array:lines += ['}']
            return lines
        lines += ['/** Copy a fully bounded native projection into a private C value, never into wire output. */','static bool to_record(Native const& value,Record& record) noexcept {']
        if not record.members and version==2:lines += ['if(!empty_component<Native>::zero(value))return false;']
        for member in record.members:lines += elementwise(member,'to')
        lines += [f'return {cs}_valid(&record);','}',
                  '/** Populate only a fresh native temporary after complete canonical decoding. */','static bool from_record(Record const& record,Native& value) {']
        for member in record.members:lines += elementwise(member,'from')
        if not record.members and version==2:lines += ['empty_component<Native>::initialize(value);']
        lines += ['return true;}','static bool valid(Native const& value) noexcept {size_t n=0;return encoded_size(value,n);}',
                  'static bool validate(Native const& value) noexcept {return valid(value);}',
                  '/** This pointer overload succeeds only when the capacity is the exact active wire length. */',
                  f'static bool encode(Native const& value,uint8_t* out,size_t capacity) {{Record record{{}};size_t n=0;if(!to_record(value,record)||{cs}_wire_size(&record)!=capacity)return false;return !{cs}_encode(&record,out,capacity,&n);}}',
                  '/** Validate and size before the sole canonical output allocation; commit output once. */',
                  f'static bool encode(Native const& value,std::vector<uint8_t>& out) {{Record record{{}};if(!to_record(value,record))return false;size_t n={cs}_wire_size(&record),used=0;std::vector<uint8_t> next(n);if({cs}_encode(&record,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}}',
                  f'static bool decode(uint8_t const* bytes,size_t size,Native& out) {{Record record{{}};if({cs}_decode(bytes,size,&record))return false;Native next{{}};if(!from_record(record,next))return false;out=std::move(next);return true;}}']
        if record.kind == 'native-primitive':
            # This is a concrete ros::Time/Duration value converter, not a
            # generated ROS Message. It has no invented MD5 or MSG section.
            lines += ['};']
            continue
        closure=[]
        def visit(item):
            if item in closure:return
            closure.append(item)
            for member in item.members:
                if member.type not in parent.SCALARS and member.type!='string':visit(records[(item.version,member.type)])
        visit(record)
        lines += ['inline static constexpr definition_entry definitions[]={']
        lines += ['{'+json.dumps(item.name)+','+json.dumps(native_definition(item))+'},' for item in closure if item.kind != 'native-primitive']
        lines += ['};','static constexpr size_t definition_count=sizeof(definitions)/sizeof(definitions[0]);']
        for index,item in enumerate(closure):
            fields=[]
            for member in item.members:
                fields.append('{'+','.join([json.dumps(member.name),json.dumps(member.type),str(member.count if member.array=='array' else 0),str(member.array=='array').lower(),str(member.native_string_bound),str(member.array=='sequence').lower(),str(member.native_sequence_bound)]+([json.dumps(member.native_default)] if member.native_default is not None else []))+'}')
            if not item.members and version==2:fields=['{"structure_needs_at_least_one_member","uint8",0,false,0,false,0}']
            if fields:lines += [f'inline static constexpr ::uwvm2::imported::ros::ipc::custom_native_field schema_fields_{index}[]={{'+','.join(fields)+'};']
        lines += ['inline static constexpr ::uwvm2::imported::ros::ipc::custom_native_record schema_records[]={']
        for index,item in enumerate(closure):
            pointer='schema_fields_'+str(index) if item.members or version==2 else 'nullptr'
            count=len(item.members) if item.members else 1 if version==2 else 0
            lines += ['{'+json.dumps(item.name)+','+pointer+','+str(count)+'},']
        lines += ['};','static constexpr ::uwvm2::imported::ros::ipc::custom_native_schema native_schema() noexcept {return {schema_records,sizeof(schema_records)/sizeof(schema_records[0])};}','};']
    lines += ['}']
    selected_interfaces=[interface for interface in interfaces if interface.version==version and interface.registered]
    if version==2:
        for interface in selected_interfaces:
            if interface.kind in ('service','action'):
                lines += emit_ros2_interface_trait(parent,records,interface)
    for kind,macro,roles in [('message','TYPES',('message',)),('service','SERVICE_TYPES',('request','response')),('action','ACTION_TYPES',('goal','result','feedback'))]:
        entries=[interface for interface in selected_interfaces if interface.kind==kind]
        lines += [f'#define UWVM_ROS_CUSTOM_ROS{version}_{macro}(M)'+(' \\' if entries else '')]
        for index,interface in enumerate(entries):
            native='::'+interface.name.replace('/','::')+('Action' if version==1 and kind=='action' else '')
            values=[native,json.dumps(interface.name),json.dumps(interface.identity),json.dumps(interface.codec_identity)]
            if kind=='message':values += [str(interface.components['message'].maximum)]
            values += ['::uwvm_custom_bounded_native::'+interface.components[role].symbol+'Codec' for role in roles]
            lines += ['  M('+','.join(values)+')'+(' \\' if index+1<len(entries) else '')]
    lines += [f'#define UWVM_ROS_CUSTOM_ROS{version}_TOPIC_TYPES(M) UWVM_ROS_CUSTOM_ROS{version}_TYPES(M)']
    return '\n'.join(lines+[''])



def emit_c_interfaces(interfaces):
    """Emit typed custom RPCs over existing checked Capability imports/ownership.

    These functions never reinterpret builtin Service/Fibonacci factories.
    The caller opens each explicit alias; Action clients reuse the existing
    caller-owned role bundle solely for ownership/rollback/cancel/cleanup.
    """
    lines=['/** @file Complete typed custom Service/Action operations over existing C capabilities.',
           ' * Generated canonical components are distinct from Runtime ticket/outcome envelopes.',
           ' * No alias is opened here. For Action clients initialize one existing action_bundle',
           ' * with explicit send/cancel-own/feedback/result aliases (optional cancel-any), and',
           ' * keep its context and goal table stable. Cancel and checked close/revoke use that',
           ' * bundle unchanged. Idle11 is not success; retain owners for caller-driven polling.',
           ' * All outputs commit only after exact framing, matching IDs and component validation. */',
           '#ifndef UWVM_BOUNDED_INTERFACES_H','#define UWVM_BOUNDED_INTERFACES_H',
           '#include "bounded_records.h"','#include <roswasm/action_bundle.h>',
           '/** Caller destinations cannot overwrite the stable owner/context/backend. Live C object storage remains caller responsibility. */',
           'static inline int uwvm_bounded_cap_destination(roswasmc_capability const* cap,void const* out,size_t size){roswasmc_context const* context=cap?cap->context:NULL;return cap&&roswasmc_action_bundle_range(out,size)&&!roswasmc_action_bundle_overlap(out,size,cap,sizeof *cap)&&(!context||(!roswasmc_action_bundle_overlap(out,size,context,sizeof *context)&&(!context->backend||!roswasmc_action_bundle_overlap(out,size,context->backend,sizeof *context->backend))));}']
    for interface in interfaces:
        if interface.kind=='message':continue
        prefix='roswasmc_bounded_'+str(interface.version)+'_'+interface.name.replace('/','_')
        if interface.kind=='service':
            rq,rp=[interface.components[k].c_symbol for k in ('request','response')]
            rqm,rpm=[interface.components[k].maximum for k in ('request','response')]
            lines += [f'/** Call one explicitly authorized custom Service exactly once; no replay. */',
             f'static inline roswasmc_result {prefix}_call(roswasmc_capability const* cap,{rq} const* request,{rp}* response) {{',
             f'unsigned char input[{max(1,rqm)}],output[{max(1,rpm)}];size_t n=0;roswasmc_result r;{rp} next={{0}};',
             f'if(!response||!uwvm_bounded_cap_destination(cap,response,sizeof *response)||{rq}_encode(request,input,{rqm},&n))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);',
             f'r=roswasmc_service_call(cap,input,n,output,{rpm});if(r.error)return r;',
             f'if(!r.has_length||r.length>{rpm}||{rp}_decode(output,(size_t)r.length,&next)){{r.error=UWVM_ROS_INVALID;return r;}}*response=next;return r;}}',
             f'/** Poll one native request; returned nonzero ticket is never a capability handle. */',
             f'static inline roswasmc_result {prefix}_poll(roswasmc_capability const* cap,uint64_t* ticket,{rq}* request) {{',
             f'unsigned char output[{rqm+8}];roswasmc_result r;{rq} next={{0}};uint64_t id;',
             'if(!ticket||!request||!uwvm_bounded_cap_destination(cap,ticket,sizeof *ticket)||!uwvm_bounded_cap_destination(cap,request,sizeof *request)||uwvm_bounded_overlap(ticket,sizeof *ticket,request,sizeof *request))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);',
             f'r=roswasmc_poll(cap,output,sizeof output);if(r.error)return r;if(!r.has_length||r.length<8||r.length>sizeof output||(id=uwvm_bounded_get(output,8))==0||{rq}_decode(output+8,(size_t)r.length-8,&next)){{r.error=UWVM_ROS_INVALID;return r;}}*ticket=id;*request=next;return r;}}',
             f'/** Respond to one owned live ticket using the independently granted respond alias. */',
             f'static inline roswasmc_result {prefix}_respond(roswasmc_capability const* cap,uint64_t ticket,{rp} const* response) {{',
             f'unsigned char input[{rpm+8}];size_t n=0;roswasmc_result r;if(!ticket||{rp}_encode(response,input+8,{rpm},&n))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);uwvm_bounded_put(input,ticket,8);r=roswasmc_service_respond(cap,input,n+8,NULL,0);if(!r.error&&(!r.has_length||r.length))r.error=UWVM_ROS_INVALID;return r;}}']
        else:
            gc,rc,fc=[interface.components[k].c_symbol for k in ('goal','result','feedback')]
            gm,rm,fm=[interface.components[k].maximum for k in ('goal','result','feedback')]
            lines += [f'/** Send one complete custom Goal using an explicitly initialized owned role bundle. */',
             f'static inline roswasmc_result {prefix}_send(roswasmc_action_bundle* bundle,{gc} const* goal,uint64_t* id) {{',
             f'unsigned char input[{max(1,gm)}],output[8];size_t n=0;uint64_t next;roswasmc_result r;uint32_t error=roswasmc_action_bundle_check(bundle);if(error)return roswasmc_action_bundle_failure(error);',
             'if(!roswasmc_action_bundle_destination(bundle,id,sizeof *id))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);if(bundle->count==bundle->capacity)return roswasmc_action_bundle_failure(UWVM_ROS_TOO_LARGE);',
             f'if({gc}_encode(goal,input,{gm},&n))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);r=roswasmc_action_send_goal(&bundle->cap[0],input,n,output,sizeof output);if(r.error)return r;',
             'if(!r.has_length||r.length!=8||(next=uwvm_bounded_get(output,8))==0||roswasmc_action_bundle_owns(bundle,next)){r.error=UWVM_ROS_INVALID;return r;}bundle->goals[bundle->count++]=next;*id=next;return r;}',
             f'/** Read custom feedback only for a locally owned exact Goal ID; idle leaves output untouched. */',
             f'static inline roswasmc_result {prefix}_feedback(roswasmc_action_bundle const* bundle,uint64_t id,{fc}* feedback) {{',
             f'unsigned char input[8],output[{fm+8}];{fc} next={{0}};roswasmc_result r;uint32_t error=roswasmc_action_bundle_check(bundle);if(error)return roswasmc_action_bundle_failure(error);',
             'if(!roswasmc_action_bundle_destination(bundle,feedback,sizeof *feedback))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);if(!roswasmc_action_bundle_owns(bundle,id))return roswasmc_action_bundle_failure(UWVM_ROS_BAD_HANDLE);uwvm_bounded_put(input,id,8);',
             f'r=roswasmc_action_receive_feedback(&bundle->cap[2],input,8,output,sizeof output);if(r.error)return r;if(!r.has_length||r.length<8||r.length>sizeof output||uwvm_bounded_get(output,8)!=id||{fc}_decode(output+8,(size_t)r.length-8,&next)){{r.error=UWVM_ROS_INVALID;return r;}}*feedback=next;return r;}}',
             f'/** A validated terminal result removes exactly one local Goal; cancel acknowledgement does not. */',
             f'static inline roswasmc_result {prefix}_result(roswasmc_action_bundle* bundle,uint64_t id,uint8_t* kind,{rc}* result) {{',
             f'unsigned char input[8],output[{rm+9}];{rc} next={{0}};size_t i;roswasmc_result r;uint32_t error=roswasmc_action_bundle_check(bundle);if(error)return roswasmc_action_bundle_failure(error);',
             'if(!roswasmc_action_bundle_destination(bundle,kind,sizeof *kind)||!roswasmc_action_bundle_destination(bundle,result,sizeof *result)||uwvm_bounded_overlap(kind,sizeof *kind,result,sizeof *result))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);if(!roswasmc_action_bundle_owns(bundle,id))return roswasmc_action_bundle_failure(UWVM_ROS_BAD_HANDLE);uwvm_bounded_put(input,id,8);',
             f'r=roswasmc_action_receive_result(&bundle->cap[3],input,8,output,sizeof output);if(r.error)return r;if(!r.has_length||r.length<9||r.length>sizeof output||uwvm_bounded_get(output,8)!=id||output[8]<2||output[8]>4||{rc}_decode(output+9,(size_t)r.length-9,&next)){{r.error=UWVM_ROS_INVALID;return r;}}for(i=0;i<bundle->count;++i)if(bundle->goals[i]==id){{--bundle->count;bundle->goals[i]=bundle->goals[bundle->count];bundle->goals[bundle->count]=0;break;}}*kind=output[8];*result=next;return r;}}',
             f'/** Poll one incoming custom Goal under an independently activated advertise alias. */',
             f'static inline roswasmc_result {prefix}_poll(roswasmc_capability const* cap,uint64_t* id,{gc}* goal) {{',
             f'unsigned char output[{gm+8}];{gc} next={{0}};uint64_t selected;roswasmc_result r;if(!id||!goal||!uwvm_bounded_cap_destination(cap,id,sizeof *id)||!uwvm_bounded_cap_destination(cap,goal,sizeof *goal)||uwvm_bounded_overlap(id,sizeof *id,goal,sizeof *goal))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);r=roswasmc_poll(cap,output,sizeof output);if(r.error)return r;if(!r.has_length||r.length<8||r.length>sizeof output||(selected=uwvm_bounded_get(output,8))==0||{gc}_decode(output+8,(size_t)r.length-8,&next)){{r.error=UWVM_ROS_INVALID;return r;}}*id=selected;*goal=next;return r;}}']
            for role,cs,maximum in [('feedback',fc,fm),('result',rc,rm)]:
                extra=',uint8_t kind' if role=='result' else ''
                invalid='kind<2||kind>4||' if role=='result' else ''
                marker='kind' if role=='result' else '1'
                lines += [f'/** Send one typed custom {role}; host owns ticket, deadline and one-shot terminal state. */',
                 f'static inline roswasmc_result {prefix}_respond_{role}(roswasmc_capability const* cap,uint64_t id{extra},{cs} const* value) {{unsigned char input[{maximum+9}];size_t n=0;roswasmc_result r;if(!id||{invalid}{cs}_encode(value,input+9,{maximum},&n))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);uwvm_bounded_put(input,id,8);input[8]={marker};r=roswasmc_action_respond(cap,input,n+9,NULL,0);if(!r.error&&(!r.has_length||r.length))r.error=UWVM_ROS_INVALID;return r;}}']
    return '\n'.join(lines+['#endif',''])



def emit_cpp_interfaces(interfaces):
    """Typed unique owners over the generated C operations, preserving role separation."""
    lines=['/** @file C++20 concrete custom typed RPC owners, independent of builtin factories.',
        ' * Context/goal storage is stable inside a nonmoving owner. Each named role is explicit.',
        ' * Use checked close/revoke before destruction to observe all cleanup errors. Native',
        ' * permission, ticket ownership, deadlines, quotas and result kind remain host checks.',
        ' * This synchronous caller-driven facade creates no thread, clock or implicit retry. */',
        '#pragma once','#include "bounded_interfaces.h"','#include "bounded_records.hpp"',
        '#include <array>','#include <memory>','#include <functional>','#include <string_view>','#include <utility>',
        'namespace uwvm_custom {',
        '/** Borrow the selected immutable backend; no alias or permission is created. */',
        'inline roswasmc_string alias(std::string_view text) {return {text.data(),text.size()};}',
        '/** Private stable two-role ownership; handlers cannot borrow/retire raw capabilities. */',
        'class ServerOwners {protected:roswasmc_context context_;roswasmc_capability advertise_{},respond_{};uint32_t error_{};bool busy_{};roswasmc_cleanup_report rollback_{};',
        'struct Pass {bool& busy;explicit Pass(bool& value):busy{value}{busy=true;}~Pass(){busy=false;}};',
        'ServerOwners(roswasmc_context const& context,std::string_view advertise,std::string_view respond,bool action):context_{context.backend}{',
        'roswasmc_capability_init(&advertise_,&context_);roswasmc_capability_init(&respond_,&context_);',
        'error_=roswasmc_check_operation(&context_,action?ROSWASMC_ACTION_RESPOND:ROSWASMC_SERVICE_RESPOND);',
        'if(!error_&&(!roswasmc_action_bundle_alias(alias(advertise))||!roswasmc_action_bundle_alias(alias(respond))||advertise==respond))error_=22;',
        'if(!error_&&(!context_.backend->abi_version||context_.backend->abi_version()!=1))error_=95;',
        'if(!error_)error_=action?roswasmc_check_operation(&context_,ROSWASMC_ACTION_ADVERTISE):(context_.backend->advertise_service?0:95);',
        'if(!error_)error_=roswasmc_open(&advertise_,advertise.data(),advertise.size());if(!error_)error_=roswasmc_open(&respond_,respond.data(),respond.size());',
        'if(!error_&&advertise_.handle==respond_.handle){respond_.handle=0;error_=17;}',
        'if(!error_){if(action){auto r=roswasmc_action_advertise(&advertise_,nullptr,0,nullptr,0);error_=r.error;if(!error_&&(!r.has_length||r.length))error_=22;}else error_=roswasmc_service_advertise(&advertise_);}',
        'if(error_){std::array<uint32_t,2> statuses{};rollback_=is_open()?cleanup(error_,statuses,false):roswasmc_cleanup_init(error_);}}',
        'roswasmc_cleanup_report cleanup(uint32_t primary,std::array<uint32_t,2>& statuses,bool revoke){auto r=roswasmc_cleanup_init(primary);if(busy_){r.cleanup_error=17;return r;}if(!is_open()){r.cleanup_error=9;return r;}roswasmc_capability* caps[]={&advertise_,&respond_};for(size_t i=2;i>0;--i){uint32_t code=0;if(roswasmc_is_open(caps[i-1])){code=revoke?roswasmc_revoke(caps[i-1]):roswasmc_close(caps[i-1]);++r.attempted;if(code){++r.failed;if(!r.cleanup_error)r.cleanup_error=code;}}statuses[i-1]=code;}return r;}',
        'public:ServerOwners(ServerOwners const&)=delete;ServerOwners& operator=(ServerOwners const&)=delete;ServerOwners(ServerOwners&&)=delete;',
        '~ServerOwners(){if(is_open()){std::array<uint32_t,2> statuses{};cleanup(0,statuses,false);}}',
        '/** Construction and rollback errors stay inspectable; no constructor retries an open. */',
        'uint32_t error()const noexcept{return error_?error_:is_open()?0:9;}bool is_open()const noexcept{return roswasmc_is_open(&advertise_)||roswasmc_is_open(&respond_);}roswasmc_cleanup_report construction_cleanup()const{return rollback_;}',
        '/** Preflight both owners before the first close; a callback refuses17 with no imports. */',
        'roswasmc_cleanup_report close_checked(uint32_t primary,std::array<uint32_t,2>& statuses){return cleanup(primary,statuses,false);}',
        '/** Explicit alias revocation; never automatic rollback or destructor behavior. */',
        'roswasmc_cleanup_report revoke_checked(uint32_t primary,std::array<uint32_t,2>& statuses){return cleanup(primary,statuses,true);}',
        '};']
    for interface in interfaces:
        if interface.kind=='message':continue
        prefix='roswasmc_bounded_'+str(interface.version)+'_'+interface.name.replace('/','_')
        name=interface.symbol
        if interface.kind=='service':
            rq,rp=[interface.components[k].symbol for k in ('request','response')]
            lines += [f'/** Complete custom {interface.name} client; default policy returns13 from construction. */',
              f'class {name}Client {{roswasmc_context context_;roswasmc_capability cap_{{}};uint32_t error_{{}};',
              'public:',f'using Request={rq};using Response={rp};',
              f'{name}Client(roswasmc_context const& context,std::string_view selected):context_{{context.backend}} {{roswasmc_capability_init(&cap_,&context_);error_=roswasmc_check_operation(&context_,ROSWASMC_SERVICE_CALL);if(!error_&&(!context_.backend->abi_version||context_.backend->abi_version()!=1))error_=95;if(!error_&&!roswasmc_action_bundle_alias(alias(selected)))error_=UWVM_ROS_INVALID;if(!error_)error_=roswasmc_open(&cap_,selected.data(),selected.size());}}',
              f'{name}Client({name}Client const&)=delete;{name}Client& operator=({name}Client const&)=delete;{name}Client({name}Client&&)=delete;',
              f'~{name}Client(){{if(roswasmc_is_open(&cap_))roswasmc_close(&cap_);}}',
              'uint32_t error() const noexcept{return error_?error_:is_open()?0:9;}bool is_open() const noexcept{return roswasmc_is_open(&cap_);}',
              f'roswasmc_result call(Request const& request,Response& response){{if(error())return roswasmc_action_bundle_failure(error());return {prefix}_call(&cap_,&request,&response);}}',
              'uint32_t close(){return roswasmc_close(&cap_);}uint32_t revoke(){return roswasmc_revoke(&cap_);}',
              '};']
        else:
            gc,rc,fc=[interface.components[k].symbol for k in ('goal','result','feedback')]
            lines += [f'/** Complete owned custom {interface.name} Action client with four separate roles. */',
              f'class {name}Client {{roswasmc_context context_;std::array<uint64_t,1024> goals_{{}};roswasmc_action_bundle bundle_{{}};',
              'public:',f'using Goal={gc};using Result={rc};using Feedback={fc};',
              f'{name}Client(roswasmc_context const& context,roswasmc_action_aliases const& aliases,size_t slots=8):context_{{context.backend}} {{roswasmc_action_bundle_init(&context_,&bundle_,&aliases,goals_.data(),slots);}}',
              f'{name}Client({name}Client const&)=delete;{name}Client& operator=({name}Client const&)=delete;{name}Client({name}Client&&)=delete;',
              f'~{name}Client(){{if(bundle_.active){{std::array<uint32_t,5> statuses{{}};roswasmc_action_bundle_close_checked(&bundle_,0,statuses.data(),statuses.size());}}}}',
              'uint32_t error() const noexcept{return bundle_.error?bundle_.error:bundle_.active?0:9;}bool is_open() const noexcept{return bundle_.active;}bool owns(uint64_t id)const{return roswasmc_action_bundle_owns(&bundle_,id);}',
              f'roswasmc_result send_goal(Goal const& value,uint64_t& id){{return {prefix}_send(&bundle_,&value,&id);}}',
              f'roswasmc_result feedback(uint64_t id,Feedback& value){{return {prefix}_feedback(&bundle_,id,&value);}}',
              f'roswasmc_result result(uint64_t id,uint8_t& kind,Result& value){{return {prefix}_result(&bundle_,id,&kind,&value);}}',
              'roswasmc_result cancel_own(uint64_t id,int& accepted){return roswasmc_action_bundle_cancel_own(&bundle_,id,&accepted);}',
              'roswasmc_result cancel_any(uint64_t id,int& accepted){return roswasmc_action_bundle_cancel_any(&bundle_,id,&accepted);}',
              'roswasmc_result cancel_all(int& accepted){return roswasmc_action_bundle_cancel_all(&bundle_,&accepted);}',
              'roswasmc_cleanup_report close_checked(uint32_t primary,std::array<uint32_t,5>& statuses){return roswasmc_action_bundle_close_checked(&bundle_,primary,statuses.data(),statuses.size());}',
              'roswasmc_cleanup_report revoke_checked(uint32_t primary,std::array<uint32_t,5>& statuses){return roswasmc_action_bundle_revoke_checked(&bundle_,primary,statuses.data(),statuses.size());}',
              '};']
        if interface.kind=='service':
            lines += [f'/** Typed custom {interface.name} server; advertise/respond stay separate authorities. */',
              f'class {name}Server:public ServerOwners {{',
              f'using Handler=std::function<{rp}({rq} const&)>;std::shared_ptr<Handler> handler_;',
              'public:',f'using Request={rq};using Response={rp};',
              f'{name}Server(roswasmc_context const& context,std::string_view advertise,std::string_view respond):ServerOwners{{context,advertise,respond,false}} {{}}',
              '/** Replacement applies next pass; the same mutable functor object survives current dispatch. */',
              'void on_request(Handler handler){handler_=handler?std::make_shared<Handler>(std::move(handler)):nullptr;}',
              f'roswasmc_result poll_once(uint64_t& ticket,Request& request){{if(busy_)return roswasmc_action_bundle_failure(17);if(error())return roswasmc_action_bundle_failure(error());Pass guard{{busy_}};return {prefix}_poll(&advertise_,&ticket,&request);}}',
              f'roswasmc_result respond(uint64_t ticket,Response const& response){{if(error())return roswasmc_action_bundle_failure(error());return {prefix}_respond(&respond_,ticket,&response);}}',
              '/** One bounded poll/decode/callback/respond pass. Idle11 is returned unchanged. */',
              '/** One caller-driven executor pass; Idle11 preserves pending owners. */',
              'uint32_t poll_once(){return process_once();}',
              f'uint32_t process_once(){{if(busy_)return 17;if(error())return error();auto handler=handler_;if(!handler)return 22;Pass guard{{busy_}};uint64_t ticket{{}};Request request{{}};auto r={prefix}_poll(&advertise_,&ticket,&request);if(r.error)return r.error;Response response=(*handler)(request);return {prefix}_respond(&respond_,ticket,&response).error;}}',
              '};']
        else:
            lines += [f'/** Typed custom {interface.name} server with explicit feedback/terminal component codecs. */',
              f'class {name}Server:public ServerOwners {{',
              'public:',f'using Goal={gc};using Result={rc};using Feedback={fc};',
              '/** Handler may choose separate feedback/result effects; kind must be2..4 when terminal. */',
              'struct Replies {Feedback feedback{};Result result{};uint8_t kind{2};bool send_feedback{true},send_result{true};};',
              'private:using Handler=std::function<Replies(uint64_t,Goal const&)>;std::shared_ptr<Handler> handler_;',
              'public:',f'{name}Server(roswasmc_context const& context,std::string_view advertise,std::string_view respond):ServerOwners{{context,advertise,respond,true}} {{}}',
              'void on_goal(Handler handler){handler_=handler?std::make_shared<Handler>(std::move(handler)):nullptr;}',
              f'roswasmc_result poll_once(uint64_t& id,Goal& goal){{if(busy_)return roswasmc_action_bundle_failure(17);if(error())return roswasmc_action_bundle_failure(error());Pass guard{{busy_}};return {prefix}_poll(&advertise_,&id,&goal);}}',
              f'roswasmc_result respond_feedback(uint64_t id,Feedback const& value){{if(error())return roswasmc_action_bundle_failure(error());return {prefix}_respond_feedback(&respond_,id,&value);}}',
              f'roswasmc_result respond_result(uint64_t id,uint8_t kind,Result const& value){{if(error())return roswasmc_action_bundle_failure(error());return {prefix}_respond_result(&respond_,id,kind,&value);}}',
              '/** One pass, at most one feedback plus one terminal response; never implicit replay. */',
              '/** One caller-driven executor pass; Idle11 preserves pending owners. */',
              'uint32_t poll_once(){return process_once();}',
              f'uint32_t process_once(){{if(busy_)return 17;if(error())return error();auto handler=handler_;if(!handler)return 22;Pass guard{{busy_}};uint64_t id{{}};Goal goal{{}};auto r={prefix}_poll(&advertise_,&id,&goal);if(r.error)return r.error;auto replies=(*handler)(id,goal);if(replies.send_result&&(replies.kind<2||replies.kind>4||!{interface.components["result"].c_symbol}_valid(&replies.result)))return 22;if(replies.send_feedback&&!{interface.components["feedback"].c_symbol}_valid(&replies.feedback))return 22;if(replies.send_feedback){{r={prefix}_respond_feedback(&respond_,id,&replies.feedback);if(r.error)return r.error;}}return replies.send_result?{prefix}_respond_result(&respond_,id,replies.kind,&replies.result).error:0;}}',
              '};']
    return '\n'.join(lines+['}',''])


def summary(interfaces,pins):
    """Describe exactly the already resolved source snapshot, without rereading it."""
    return {'format':interfaces[0].profile if interfaces else PROFILE,'status':'SCHEMA_PREPARATION_ONLY','source_inputs':pins,
            'types':[{'ros_version':item.version,'type':item.name,'kind':item.kind,'native_identity':item.identity,
                      'adapter_codec_identity':item.codec_identity,
                      'components':{role:{'type':record.name,'native_identity':record.identity,'minimum_wire_bytes':record.minimum,
                                          'maximum_wire_bytes':record.maximum,'expanded_nodes':record.nodes,
                                          'adapter_codec_identity':record.codec_identity}
                                    for role,record in item.components.items()}}
                     for item in interfaces],
            'native_execution':'NOT_RUN','compiler_execution':'NOT_RUN',
            'native_registry_emitted':False,'implicit_permissions':[]}


def check(parent,path):
    """Prepare metadata without writing, native registration or execution."""
    _,ordered,interfaces,pins=load_catalog(parent,path,False)
    proof=summary(interfaces,pins)
    if any(r.profile==CONTROL_PROFILE for r in ordered):proof.update(control_summary(ordered,interfaces))
    return proof


def emit_managed_interfaces(parent, interfaces):
    """Emit four additive RPC modules only for actual Service/Action catalogs.

    Existing record codecs and message-only outputs remain unchanged. Templates
    are trusted generator source, selected by a closed literal table; their exact
    bytes enter source_inputs and are reopened before the source revision commits.
    """
    selected = [item for item in interfaces if item.kind != 'message']
    if not selected:
        return {}, []
    bodies = {key: [] for key in ('rs', 'mjs', 'go', 'ts')}
    components = {record.symbol: record for item in selected for record in item.components.values()}
    bodies['rs'] += [f'/// Complete canonical component bound for {record.name}.\nimpl Component for {record.symbol} {{const MAX_WIRE:usize={record.maximum};const ROS_VERSION:u8={record.version};}}'
                     for record in components.values()]
    imports = ','.join(value for name in components for value in (name, name + 'Codec'))
    for item in selected:
        name, version = item.symbol, item.version
        if item.kind == 'service':
            q, s = [item.components[key].symbol for key in ('request', 'response')]
            qm, sm = [item.components[key].maximum for key in ('request', 'response')]
            bodies['rs'] += [f'/// Typed {item.name} call/real-submit owner; every alias is explicit.\npub type {name}Client=ServiceClient<{q},{s}>;',
                             f'/// Typed {item.name} advertiser/responder with independent grants.\npub type {name}Server=ServiceServer<{q},{s}>;']
            bodies['mjs'] += [f'/** Complete {item.name} typed client; optional observer is independently granted. */\nexport class {name}Client extends TypedServiceClient {{constructor(session,alias,observerAlias=null,capacity=8){{super(session,{version},alias,records.{q}Codec,records.{s}Codec,observerAlias,capacity);}}}}',
                              f'/** Complete {item.name} typed server; callback replacement is caller-managed. */\nexport class {name}Server extends TypedServer {{constructor(session,advertise,respond,capacity=8){{super(session,{version},advertise,respond,records.{q}Codec,records.{s}Codec,false,capacity);}}}}']
            bodies['ts'] += [f'/** Complete {item.name} typed call and real-submit client. */\nexport class {name}Client extends BoundedServiceClient<{q},{s}> {{constructor(api:Api,alias:string,observerAlias:string="",capacity:i32=8){{super(api,{version},alias,new {q}Codec(),new {s}Codec(),(value:{q}):bool=>value.validValue(),{sm},observerAlias,capacity);}}}}',
                             f'/** Complete {item.name} incoming request/response owner. */\nexport class {name}Server extends BoundedServiceServer<{q},{s}> {{constructor(api:Api,advertise:string,respond:string,capacity:i32=8){{super(api,{version},advertise,respond,new {q}Codec(),new {s}Codec(),(value:{s}):bool=>value.validValue(),{qm},capacity);}}}}']
            bodies['go'] += [f'''// {name}Client owns a typed service source and optional exclusive completion observer.
type {name}Client struct{{core *rpcService}}
// New{name}Client opens explicit aliases; observer="" enables synchronous calls only.
func New{name}Client(alias,observer string,capacity int)(*{name}Client,error){{core,err:=rpcServiceOpen(alias,observer,{version},capacity);if err!=nil{{return nil,err}};return &{name}Client{{core}},nil}}
// Call performs exactly one canonical RPC and decodes a complete typed response.
func(c *{name}Client)Call(value {q})({s},error){{if c==nil||c.core==nil{{return {s}{{}},rpcError(ros.BadHandle)}};wire,err:=value.Encode();if err!=nil{{return {s}{{}},err}};out,err:=c.core.call(wire,{sm});if err!=nil{{return {s}{{}},err}};return Decode{s}(out)}}
// Submit uses real service_submit. Its full-width ID acknowledges admission only.
func(c *{name}Client)Submit(value {q})(uint64,error){{if c==nil||c.core==nil{{return 0,rpcError(ros.BadHandle)}};wire,err:=value.Encode();if err!=nil{{return 0,err}};return c.core.submit(wire)}}
// {name}Completion separates a numeric terminal failure from a validated response.
type {name}Completion struct{{RequestID uint64;Response {s};Error uint32}}
// PollCompletion exclusively polls the fresh observer, then checks stream/source/owned ID.
func(c *{name}Client)PollCompletion()({name}Completion,error){{var result {name}Completion;if c==nil||c.core==nil{{return result,rpcError(ros.BadHandle)}};id,wire,code,err:=c.core.completion({sm});if err!=nil{{return result,err}};result.RequestID,result.Error=id,code;if code==0{{result.Response,err=Decode{s}(wire);if err!=nil{{return {name}Completion{{}},err}}}};rpcRemove(&c.core.pending,id);return result,nil}}
// CloseChecked preserves the primary error and every one-attempt cleanup result.
func(c *{name}Client)CloseChecked(primary error)RPCCleanup{{if c==nil||c.core==nil{{return RPCCleanup{{Primary:primary,Results:[]error{{rpcError(ros.BadHandle)}}}}}};return c.core.cleanup(primary,false)}}
// RevokeChecked revokes only existing aliases; it creates no authority.
func(c *{name}Client)RevokeChecked(primary error)RPCCleanup{{if c==nil||c.core==nil{{return RPCCleanup{{Primary:primary,Results:[]error{{rpcError(ros.BadHandle)}}}}}};return c.core.cleanup(primary,true)}}
// {name}Server owns separate advertise/respond roles; copies share its state.
type {name}Server struct{{core *rpcServer}}
// New{name}Server opens both aliases before advertisement; rollback is inspectable.
func New{name}Server(advertise,respond string,capacity int)(*{name}Server,error){{core,err:=rpcServerOpen(advertise,respond,false,capacity);if err!=nil{{return nil,err}};return &{name}Server{{core}},nil}}
func(s *{name}Server)pollInner()(uint64,{q},error){{id,wire,err:=s.core.poll({qm});if err!=nil{{return 0,{q}{{}},err}};value,err:=Decode{q}(wire);if err!=nil{{return 0,{q}{{}},err}};s.core.live=append(s.core.live,id);return id,value,nil}}
// PollOnce returns one locally owned ticket and complete value, or unchanged idle11.
func(s *{name}Server)PollOnce()(uint64,{q},error){{if s==nil||s.core==nil{{return 0,{q}{{}},rpcError(ros.BadHandle)}};if err:=s.core.owners.idle();err!=nil{{return 0,{q}{{}},err}};return s.pollInner()}}
// Respond accepts only a ticket polled here; host principal/deadline checks still apply.
func(s *{name}Server)Respond(id uint64,value {s})error{{if s==nil||s.core==nil{{return rpcError(ros.BadHandle)}};wire,err:=value.Encode();if err!=nil{{return err}};return s.core.respond(id,-1,wire)}}
// SpinOnce executes one handler under a lease. Return a value, do not also respond manually.
// Normal errors/panic unwinding release the lease; Wasm abort cannot promise cleanup.
func(s *{name}Server)SpinOnce(handler func(uint64,{q})({s},error))error{{if s==nil||s.core==nil{{return rpcError(ros.BadHandle)}};if handler==nil{{return rpcError(ros.Invalid)}};if err:=s.core.owners.idle();err!=nil{{return err}};s.core.owners.busy=true;defer func(){{s.core.owners.busy=false}}();id,value,err:=s.pollInner();if err!=nil{{return err}};response,err:=handler(id,value);if err!=nil{{return err}};return s.Respond(id,response)}}
// CloseChecked preflights dispatch before any retirement import.
func(s *{name}Server)CloseChecked(primary error)RPCCleanup{{if s==nil||s.core==nil{{return RPCCleanup{{Primary:primary,Results:[]error{{rpcError(ros.BadHandle)}}}}}};return s.core.cleanup(primary,false)}}
// RevokeChecked retains all status results separately from the primary error.
func(s *{name}Server)RevokeChecked(primary error)RPCCleanup{{if s==nil||s.core==nil{{return RPCCleanup{{Primary:primary,Results:[]error{{rpcError(ros.BadHandle)}}}}}};return s.core.cleanup(primary,true)}}''']
        else:
            g, r, f = [item.components[key].symbol for key in ('goal', 'result', 'feedback')]
            gm, rm, fm = [item.components[key].maximum for key in ('goal', 'result', 'feedback')]
            bodies['rs'] += [f'/// Complete custom {item.name} client; four roles plus optional cancel-any.\npub type {name}Client=ActionClient<{g},{r},{f}>;',
                             f'/// Complete custom {item.name} server; feedback1/terminal2..4 remain distinct.\npub type {name}Server=ActionServer<{g},{r},{f}>;']
            bodies['mjs'] += [f'/** Custom {item.name} client; aliases send/cancelOwn/feedback/result are mandatory. */\nexport class {name}Client extends TypedActionClient {{constructor(session,aliases,capacity=8){{super(session,{version},aliases,records.{g}Codec,records.{r}Codec,records.{f}Codec,capacity);}}}}',
                              f'/** Custom {item.name} server; callbacks explicitly choose their responses. */\nexport class {name}Server extends TypedServer {{constructor(session,advertise,respond,capacity=8){{super(session,{version},advertise,respond,records.{g}Codec,records.{r}Codec,true,capacity,records.{f}Codec);}}respondFeedback(id,value){{return this.respond(id,value,1);}}respondResult(id,kind,value){{if(!Number.isInteger(kind)||kind<2||kind>4)invalid();return this.respond(id,value,kind);}}}}']
            bodies['ts'] += [f'/** Complete custom {item.name} client. Role aliases never imply grants. */\nexport class {name}Client extends BoundedActionClient<{g},{r},{f}> {{constructor(api:Api,send:string,cancelOwn:string,feedback:string,result:string,cancelAny:string="",capacity:i32=8){{super(api,{version},send,cancelOwn,feedback,result,new {g}Codec(),new {r}Codec(),new {f}Codec(),(value:{g}):bool=>value.validValue(),{rm},{fm},cancelAny,capacity);}}}}',
                             f'/** Complete custom {item.name} server with matching-ID response codecs. */\nexport class {name}Server extends BoundedActionServer<{g},{r},{f}> {{constructor(api:Api,advertise:string,respond:string,capacity:i32=8){{super(api,{version},advertise,respond,new {g}Codec(),new {r}Codec(),new {f}Codec(),(value:{r}):bool=>value.validValue(),(value:{f}):bool=>value.validValue(),{gm},capacity);}}}}']
            bodies['go'] += [f'''// {name}Client owns four distinct action roles and an optional fifth cancel-any grant.
type {name}Client struct{{core *rpcAction}}
// New{name}Client opens only explicit aliases; cancelAny="" never falls back to cancel-own.
func New{name}Client(send,cancelOwn,feedback,result,cancelAny string,capacity int)(*{name}Client,error){{core,err:=rpcActionOpen(send,cancelOwn,feedback,result,cancelAny,capacity);if err!=nil{{return nil,err}};return &{name}Client{{core}},nil}}
// SendGoal admits one complete typed goal and records its nonzero unique ID.
func(c *{name}Client)SendGoal(value {g})(uint64,error){{if c==nil||c.core==nil{{return 0,rpcError(ros.BadHandle)}};wire,err:=value.Encode();if err!=nil{{return 0,err}};return c.core.send(wire)}}
// CancelOwn acknowledges cancellation without removing the goal.
func(c *{name}Client)CancelOwn(id uint64)(bool,error){{if c==nil||c.core==nil{{return false,rpcError(ros.BadHandle)}};return c.core.cancel(id,false)}}
// CancelAny requires the fifth role. Zero explicitly selects cancel-all.
func(c *{name}Client)CancelAny(id uint64)(bool,error){{if c==nil||c.core==nil{{return false,rpcError(ros.BadHandle)}};return c.core.cancel(id,true)}}
// CancelAll explicitly requests stronger cancellation under the fifth-role grant.
func(c *{name}Client)CancelAll()(bool,error){{return c.CancelAny(0)}}
// Feedback checks the matching envelope ID before decoding the complete component.
func(c *{name}Client)Feedback(id uint64)({f},error){{if c==nil||c.core==nil{{return {f}{{}},rpcError(ros.BadHandle)}};wire,_,err:=c.core.receive(id,false,{fm});if err!=nil{{return {f}{{}},err}};return Decode{f}(wire)}}
// Result removes exactly one goal only after complete terminal-kind/component validation.
func(c *{name}Client)Result(id uint64)(uint8,{r},error){{if c==nil||c.core==nil{{return 0,{r}{{}},rpcError(ros.BadHandle)}};wire,kind,err:=c.core.receive(id,true,{rm});if err!=nil{{return 0,{r}{{}},err}};value,err:=Decode{r}(wire);if err!=nil{{return 0,{r}{{}},err}};rpcRemove(&c.core.goals,id);return kind,value,nil}}
// CloseChecked attempts every role once and clears only local ownership.
func(c *{name}Client)CloseChecked(primary error)RPCCleanup{{if c==nil||c.core==nil{{return RPCCleanup{{Primary:primary,Results:[]error{{rpcError(ros.BadHandle)}}}}}};return c.core.cleanup(primary,false)}}
// RevokeChecked deliberately revokes exactly the already opened roles.
func(c *{name}Client)RevokeChecked(primary error)RPCCleanup{{if c==nil||c.core==nil{{return RPCCleanup{{Primary:primary,Results:[]error{{rpcError(ros.BadHandle)}}}}}};return c.core.cleanup(primary,true)}}
// {name}Server owns incoming goal tickets; capabilities never enter callbacks.
type {name}Server struct{{core *rpcServer}}
// New{name}Server activates only after both advertise/respond roles open.
func New{name}Server(advertise,respond string,capacity int)(*{name}Server,error){{core,err:=rpcServerOpen(advertise,respond,true,capacity);if err!=nil{{return nil,err}};return &{name}Server{{core}},nil}}
func(s *{name}Server)pollInner()(uint64,{g},error){{id,wire,err:=s.core.poll({gm});if err!=nil{{return 0,{g}{{}},err}};value,err:=Decode{g}(wire);if err!=nil{{return 0,{g}{{}},err}};s.core.live=append(s.core.live,id);return id,value,nil}}
// PollOnce returns one nonzero goal and a fully decoded value, never partial data.
func(s *{name}Server)PollOnce()(uint64,{g},error){{if s==nil||s.core==nil{{return 0,{g}{{}},rpcError(ros.BadHandle)}};if err:=s.core.owners.idle();err!=nil{{return 0,{g}{{}},err}};return s.pollInner()}}
// RespondFeedback preserves the live goal after host success.
func(s *{name}Server)RespondFeedback(id uint64,value {f})error{{if s==nil||s.core==nil{{return rpcError(ros.BadHandle)}};wire,err:=value.Encode();if err!=nil{{return err}};return s.core.respond(id,1,wire)}}
// RespondResult accepts only success2/abort3/cancel4 and retires after actual success.
func(s *{name}Server)RespondResult(id uint64,kind uint8,value {r})error{{if s==nil||s.core==nil{{return rpcError(ros.BadHandle)}};if kind<2||kind>4{{return rpcError(ros.Invalid)}};wire,err:=value.Encode();if err!=nil{{return err}};return s.core.respond(id,int(kind),wire)}}
// SpinOnce runs a local handler under a lease; it chooses explicit later responses.
func(s *{name}Server)SpinOnce(handler func(uint64,{g})error)error{{if s==nil||s.core==nil{{return rpcError(ros.BadHandle)}};if handler==nil{{return rpcError(ros.Invalid)}};if err:=s.core.owners.idle();err!=nil{{return err}};s.core.owners.busy=true;defer func(){{s.core.owners.busy=false}}();id,value,err:=s.pollInner();if err!=nil{{return err}};return handler(id,value)}}
// CloseChecked preflights callback ownership before any close import.
func(s *{name}Server)CloseChecked(primary error)RPCCleanup{{if s==nil||s.core==nil{{return RPCCleanup{{Primary:primary,Results:[]error{{rpcError(ros.BadHandle)}}}}}};return s.core.cleanup(primary,false)}}
// RevokeChecked preserves the original error and every cleanup result.
func(s *{name}Server)RevokeChecked(primary error)RPCCleanup{{if s==nil||s.core==nil{{return RPCCleanup{{Primary:primary,Results:[]error{{rpcError(ros.BadHandle)}}}}}};return s.core.cleanup(primary,true)}}''']
    products, pins = {}, []
    for extension in ('rs', 'mjs', 'go', 'ts'):
        relative = 'interface_templates/bounded_interfaces.' + extension + '.in'
        path = Path(__file__).parent / relative
        raw = parent.bounded_read(path, parent.MAX_SOURCE_BYTES)
        text = raw.decode('utf-8')
        if text.count('@@INTERFACES@@') != 1:
            raise parent.GenerationError('one closed RPC template interface placeholder required')
        body = text.replace('@@INTERFACES@@', '\n\n'.join(bodies[extension]))
        if extension == 'ts':
            if body.count('@@IMPORTS@@') != 1:
                raise parent.GenerationError('one closed RPC template import placeholder required')
            body = body.replace('@@IMPORTS@@', 'import {' + imports + '} from "./bounded_records";')
        if '@@' in body:
            raise parent.GenerationError('unknown RPC template placeholder')
        products['bounded_interfaces.' + extension] = body
        pins.append({'generator_relative': relative, 'bytes': len(raw), 'sha256': hashlib.sha256(raw).hexdigest()})
    return products, pins


def generate(parent,path,output,guest_only=False):
    """Write a fresh source revision, never overwrite a prior receipt/product."""
    records,ordered,interfaces,pins=load_catalog(parent,path,not guest_only)
    if any(r.profile==CONTROL_PROFILE for r in ordered):
        return generate_control(parent,path,output,records,ordered,interfaces,pins,guest_only)
    products={'bounded_records.h':emit_c(parent,records,ordered),'bounded_records.hpp':emit_cpp(ordered),'bounded_interfaces.h':emit_c_interfaces(interfaces),'bounded_interfaces.hpp':emit_cpp_interfaces(interfaces),
              'bounded_records.rs':emit_rust(parent,records,ordered),'bounded_records.mjs':emit_js(parent,records,ordered),
              'bounded_records.go':emit_go(parent,records,ordered),'bounded_records.ts':emit_as(parent,records,ordered)}
    facade_products, facade_templates = emit_managed_interfaces(parent, interfaces)
    products.update(facade_products)
    if facade_templates:
        pins['facade_templates'] = facade_templates
    if not guest_only:
        products.update({'manifest.hpp':emit_manifest(parent,records,ordered,interfaces),
                         'native_ros1.hpp':emit_native(parent,records,ordered,interfaces,1),
                         'native_ros2.hpp':emit_native(parent,records,ordered,interfaces,2)})
    proof=summary(interfaces,pins)
    if any(interface.profile == STANDARD_PROFILE for interface in interfaces):
        proof.update(profile=STANDARD_PROFILE,registered_interfaces=[interface.name for interface in interfaces if interface.registered],
            dependency_interfaces=[interface.name for interface in interfaces if not interface.registered],
            native_primitive_identities='ROS1 time/duration have no invented message identity; actual compiled field traits required')
    proof.update({'status':'GENERATED_GUEST_SOURCE_ONLY' if guest_only else 'GENERATED_SOURCE_ONLY',
                  'native_registry_emitted':not guest_only,'source_inputs':pins,
                  'generator_sha256':hashlib.sha256(parent.bounded_read(Path(parent.__file__),parent.MAX_SOURCE_BYTES)).hexdigest(),
                  'bounded_generator_sha256':hashlib.sha256(parent.bounded_read(Path(__file__),parent.MAX_SOURCE_BYTES)).hexdigest(),
                  'native_identity_provenance':'caller-exported official root and component identities; native metadata/traits qualification required before entities',
                  'records':[{'ros_version':record.version,'type':record.name,'native_identity':record.identity,
                              'minimum_wire_bytes':record.minimum,'maximum_wire_bytes':record.maximum,
                              'expanded_nodes':record.nodes,'adapter_codec_identity':record.codec_identity}
                             for record in ordered],
                  'packages':{'ros1':sorted({item.name.split('/')[0] for item in interfaces if item.version==1}),
                              'ros2':sorted({item.name.split('/')[0] for item in interfaces if item.version==2})},
                  'outputs':{name:{'bytes':len(body.encode()),'sha256':hashlib.sha256(body.encode()).hexdigest()}
                             for name,body in products.items()},
                  'supported_source_profile':'explicit finite .msg/.srv/.action; no unknown dynamic type, constants, defaults, aliases or recursive schema'})
    declared_defaults = {record.name:{member.name:member.native_default for member in record.members if member.native_default is not None}
                         for record in ordered if any(member.native_default is not None for member in record.members)}
    if declared_defaults:
        proof['native_default_contract']={'declared_defaults':declared_defaults,
            'initialization_only':True,'wire_decode_fills_missing_fields':False,
            'native_qualification':'complete actual modern TypeDescription including normalized defaults is mandatory',
            'legacy_introspection_defaults':'REFUSED_NULL_DEFAULT_POINTER_IS_UNKNOWN',
            'implemented_record_outputs':['c','cpp','rust','javascript','assemblyscript','tinygo'],
            'language_inventory_is_extensible':True}
        proof['supported_source_profile']='explicit finite .msg/.srv/.action; complete ROS2 bool/integer/finite-float scalar defaults with full modern metadata; array/string/nested defaults, constants, aliases and recursive schemas require separate implementations'
    # Templates are complete trusted generator inputs, not catalog-relative definitions.
    # Reopen every used body before committing any source revision.
    for pin in facade_templates:
        raw = parent.bounded_read(Path(__file__).parent / pin['generator_relative'], parent.MAX_SOURCE_BYTES)
        if len(raw) != pin['bytes'] or hashlib.sha256(raw).hexdigest() != pin['sha256']:
            raise parent.GenerationError('RPC facade template changed after source emission')
    current_catalog=parent.bounded_read(path,parent.MAX_CATALOG_BYTES)
    if len(current_catalog)!=pins['catalog']['bytes'] or hashlib.sha256(current_catalog).hexdigest()!=pins['catalog']['sha256']:
        raise parent.GenerationError('catalog changed after source resolution')
    root=path.resolve().parent
    for pin in pins['definitions']:
        source=(root/pin['file']).resolve()
        if not source.is_relative_to(root):raise parent.GenerationError('definition escaped after source resolution')
        current=parent.bounded_read(source,parent.MAX_SOURCE_BYTES)
        if len(current)!=pin['bytes'] or hashlib.sha256(current).hexdigest()!=pin['sha256']:
            raise parent.GenerationError('definition changed after source resolution')
    if output.exists():
        raise parent.GenerationError('bounded output already exists; select a fresh revision directory')
    output.mkdir(parents=True)
    for name,body in products.items():
        (output/name).write_text(body,encoding='utf-8')
    (output/'generation.json').write_text(json.dumps(proof,indent=2)+'\n',encoding='utf-8')
    return proof



# CONTROL-only managed source emission. Original custom/standard emitters above
# are not changed or called with relabeled/erased fields.
def control_rust_member(name):
    if name in ('self','super','crate'):
        return 'ros_'+name
    keywords=set('as async await break const continue dyn else enum extern false fn for if impl in let loop match mod move mut pub ref return static struct trait true type unsafe use where while abstract become box do final macro override priv typeof unsized virtual yield try'.split())
    return 'r#'+name if name in keywords else name


def control_as_member(name):
    # Keep native Field.name in schema/identity. A finite source-language spelling
    # avoids the AS declaration grammar; the proof publishes the exact mapping.
    return 'ros_'+name if name in ('type','default','function','namespace','constructor','get','set') else name


def control_member_maps(ordered):
    result={}
    for record in ordered:
        value={'rust':{m.name:control_rust_member(m.name) for m in record.members},
               'javascript':{m.name:m.name for m in record.members},
               'assemblyscript':{m.name:control_as_member(m.name) for m in record.members},
               'tinygo':{m.name:m.name[:1].upper()+m.name[1:] for m in record.members}}
        if any(len(set(v.values()))!=len(v) for v in value.values()):
            raise ValueError('control source-language member mapping collision')
        result[record.name]=value
    return result


def control_rust_element(parent,record,member,records):
    if member.type=='string' and member.string_encoding=='raw_bytes':return 'Vec<u8>'
    return rust_element(parent,record,member,records)


def control_js_default(member,record,records):
    if member.type=='string' and member.string_encoding=='raw_bytes':
        return ('[]' if member.array=='sequence' else 'Array.from({length:'+str(member.count)+'},()=>new Uint8Array())' if member.array=='array' else 'new Uint8Array()')
    return js_default(member,record,records)


def control_js_capture(record,records):
    sym=record.symbol
    lines=[f'function {sym}Capture(value){{if(value===null||typeof value!=="object")bad();const out=Object.create(null);']
    for i,m in enumerate(record.members):
        lines += [f'{{const field=value[{json.dumps(m.name)}];']
        a='field';dest='out['+json.dumps(m.name)+']'
        if m.array:
            compare='!==' if m.array=='array' else '>'
            lines += [f'if(!Array.isArray(field))bad();const count=field.length;if(!Number.isInteger(count)||count<0||count{compare}{m.count})bad();const values=new Array(count);for(let i=0;i<count;++i){{const element=field[i];']
            a='element';dest='values[i]'
        if m.type=='string' and m.string_encoding=='raw_bytes':
            lines += [f'{dest}=copyRaw({a},{m.string_bound});']
        elif m.type not in ('string','bool','uint8','int8','uint16','int16','uint32','int32','uint64','int64','float32','float64'):
            lines += [f'{dest}={records[(record.version,m.type)].symbol}Capture({a});']
        else:lines += [f'{dest}={a};']
        if m.array:lines += ['}out['+json.dumps(m.name)+']=values;']
        lines += ['}']
    return lines+['return out;}']

def emit_control_rust(parent,records,ordered):
    """Emit owned Rust records with a bounded, shared-cursor Canonical codec."""
    # Generated container loops bind a local `count`. Keep the cursor helper
    # distinct so fixed arrays of strings can still read each element's length.
    lines=['//! Generated bounded-v2 canonical values; no I/O, threads, clocks or authority.',
           '//! Enable the existing SDK alloc feature. Containers/UTF-8 are validated before',
           '//! output allocation; decoder lengths are checked before bounded allocations.',
           '//! Empty interface components are zero-sized semantic values, not native byte copies.',
           'extern crate alloc;','use alloc::{vec::Vec,string::String};','use roswasm::{Error,codes};','use roswasm::typed::Canonical;',
           'fn invalid()->Error {Error::new(codes::INVALID)}',
           'fn add_size(n:usize,m:usize)->Result<usize,Error>{n.checked_add(m).filter(|v|*v<=32768).ok_or(Error::new(codes::TOO_LARGE))}',
           'fn take<\'a>(bytes:&\'a [u8],at:&mut usize,n:usize)->Result<&\'a [u8],Error>{if *at>bytes.len()||n>bytes.len()-*at{return Err(invalid());}let start=*at;*at+=n;Ok(&bytes[start..*at])}',
           'fn read_count(bytes:&[u8],at:&mut usize)->Result<usize,Error>{Ok(u32::from_le_bytes(take(bytes,at,4)?.try_into().map_err(|_|invalid())?) as usize)}',
           'fn put(out:&mut [u8],at:&mut usize,bytes:&[u8]){out[*at..*at+bytes.len()].copy_from_slice(bytes);*at+=bytes.len();}']
    for record in ordered:
        sym=record.symbol
        lines += [f'/// Complete bounded value for ROS{record.version} {record.name}.', '#[derive(Clone,Debug,PartialEq)]',f'pub struct {sym} {{']
        for member in record.members:
            type_name=control_rust_element(parent,record,member,records)
            if member.array=='array':type_name=f'[{type_name};{member.count}]'
            elif member.array=='sequence':type_name=f'Vec<{type_name}>'
            lines += [f'  /// Declared {member.name}; canonical bounds are part of CODEC_IDENTITY.',f'  pub {control_rust_member(member.name)}:{type_name},']
        lines += ['}',f'impl Default for {sym} {{fn default()->Self {{Self {{']
        lines += [control_rust_member(m.name)+':'+(scalar_default_expression(m.type,m.native_default,'rust') if m.native_default is not None else 'core::array::from_fn(|_|Default::default())' if m.array=='array' else 'Default::default()')+',' for m in record.members]
        lines += ['}}}',f'impl {sym} {{']
        for constant in record.constants:
            lines += [f'  /// Original scalar constant; no instance wire bytes.\n  pub const {constant["name"]}:{parent.SCALARS[constant["type"]][3]}='+scalar_default_expression(constant['type'],constant['value'],'rust')+';']
        lines += [f'  pub const MAX_WIRE_BYTES:usize={record.maximum};',f'  pub const CARTESIAN_WIRE_BYTES:u64={record.cartesian_maximum};',f'  pub const AGGREGATE_WIRE_BYTES:usize={record.aggregate_wire_bytes};',f'  pub const CODEC_IDENTITY:&\'static str={json.dumps(record.codec_identity)};',
                  '  /// Validate all fields and measure only their initialized canonical prefix.',
                  '  pub fn wire_size(&self)->Result<usize,Error>{let mut n=0usize;']
        for member in record.members:
            a='self.'+control_rust_member(member.name)
            if member.array:
                if member.array=='sequence':lines += [f'if {a}.len()>{member.count}{{return Err(invalid());}}n=add_size(n,4)?;']
                lines += [f'for element in &{a} {{'];a='element'
            if member.type=='string':
                raw=member.string_encoding=='raw_bytes'
                lines += [f'if {a}.len()>{member.string_bound}'+('' if raw else f'||{a}.as_bytes().contains(&0)')+'{return Err(invalid());}'+f'n=add_size(n,4)?;n=add_size(n,{a}.len())?;']
            elif member.type not in parent.SCALARS:lines += [f'n=add_size(n,{a}.wire_size()?)?;']
            else:
                if member.type.startswith('float'):lines += [f'if !{a}.is_finite(){{return Err(invalid());}}']
                lines += [f'n=add_size(n,{parent.SCALARS[member.type][1]})?;']
            if member.array:lines += ['}']
        if semantic_condition(record, 'rust', 'self'):
            lines += ['if '+semantic_condition(record, 'rust', 'self')+'{return Err(invalid());}']
        lines += [f'if n>{record.maximum}{{return Err(Error::new(codes::TOO_LARGE));}}Ok(n)}}',
                  '  /// Encode after complete validation; output remains unchanged on refusal.',
                  '  pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error>{let n=self.wire_size()?;if out.len()<n{return Err(Error::new(codes::TOO_LARGE));}let mut at=0;self.write_fields(out,&mut at);Ok(at)}',
                  '  fn write_fields(&self,out:&mut [u8],at:&mut usize){']
        for member in record.members:
            a='self.'+control_rust_member(member.name)
            if member.array=='sequence':lines += [f'put(out,at,&({a}.len() as u32).to_le_bytes());']
            if member.array:lines += [f'for element in &{a} {{'];a='element'
            if member.type=='string':lines += [f'put(out,at,&({a}.len() as u32).to_le_bytes());put(out,at,'+(f'{a}.as_slice()' if member.string_encoding=='raw_bytes' else f'{a}.as_bytes()')+');']
            elif member.type not in parent.SCALARS:lines += [f'{a}.write_fields(out,at);']
            elif member.type=='bool':lines += [f'put(out,at,&[u8::from('+('*element' if member.array else a)+')]);']
            else:lines += [f'put(out,at,&{a}.to_le_bytes());']
            if member.array:lines += ['}']
        lines += ['}', '  fn read_fields(bytes:&[u8],at:&mut usize)->Result<Self,Error>{let mut value=Self::default();']
        for member in record.members:
            a='value.'+control_rust_member(member.name)
            if member.array:
                lines += ['{let count='+('read_count(bytes,at)?;' if member.array=='sequence' else str(member.count)+'usize;')+f'if count>{member.count}{{return Err(invalid());}}']
                if member.array=='sequence':lines += [f'{a}=Vec::with_capacity(count);']
                lines += ['for i in 0..count {']
            if member.type=='string':
                lines += [f'let size=read_count(bytes,at)?;if size>{member.string_bound}{{return Err(invalid());}}let raw=take(bytes,at,size)?;'+('let decoded=raw.to_vec();' if member.string_encoding=='raw_bytes' else 'if raw.contains(&0){return Err(invalid());}let decoded=core::str::from_utf8(raw).map_err(|_|invalid())?.into();')]
            elif member.type not in parent.SCALARS:lines += [f'let decoded={records[(record.version,member.type)].symbol}::read_fields(bytes,at)?;']
            elif member.type=='bool':lines += ['let byte=take(bytes,at,1)?[0];if byte>1{return Err(invalid());}let decoded=byte!=0;']
            else:
                width=parent.SCALARS[member.type][1]
                lines += [f'let decoded={parent.SCALARS[member.type][3]}::from_le_bytes(take(bytes,at,{width})?.try_into().map_err(|_|invalid())?);']
                if member.type.startswith('float'):lines += ['if !decoded.is_finite(){return Err(invalid());}']
            lines += [f'{a}.push(decoded);' if member.array=='sequence' else f'{a}[i]=decoded;' if member.array=='array' else f'{a}=decoded;']
            if member.array:lines += ['}}']
        if semantic_condition(record, 'rust', 'value'):
            lines += ['if '+semantic_condition(record, 'rust', 'value')+'{return Err(invalid());}']
        lines += ['Ok(value)}','}',f'impl Canonical for {sym} {{',
                  '  /// Allocate one exact validated canonical output; never native serialization.',
                  f'  fn encode(&self)->Result<Vec<u8>,Error>{{let n=self.wire_size()?;let mut out=alloc::vec![0u8;n];let mut at=0;self.write_fields(&mut out,&mut at);Ok(out)}}',
                  f'  fn decode(bytes:&[u8])->Result<Self,Error>{{if bytes.len()>{record.maximum}{{return Err(Error::new(codes::TOO_LARGE));}}if bytes.len()<{record.minimum}{{return Err(invalid());}}let mut at=0;let value=Self::read_fields(bytes,&mut at)?;if at!=bytes.len(){{return Err(invalid());}}Ok(value)}}','}']
    return '\n'.join(lines+[''])


def emit_control_js(parent,records,ordered):
    """Emit complete standalone JavaScript typed codecs with bounded readers."""
    lines=['/** Generated bounded-v2 canonical codecs. No ROS, filesystem or network imports.',
           ' * Records are ordinary owned values. Decode throws before exposing malformed bytes;',
           ' * encode validates the complete value before allocating its one wire output.',
           ' * 64-bit integers remain BigInt. Strings are strict UTF-8 with no embedded NUL.',
           ' * Owned Service transport is a separate explicit raw12 adapter, not legacy RPC. */',
           'const utf8Encoder=new TextEncoder(),utf8Decoder=new TextDecoder("utf-8",{fatal:true,ignoreBOM:true});',
           'export class ControlCodecError extends Error{constructor(code){super("invalid original-control canonical value");this.code=code;}}',
           'function bad(code=22){throw new ControlCodecError(code);}',
           'const nativeTypedLength=Object.getOwnPropertyDescriptor(Object.getPrototypeOf(Uint8Array.prototype),"length").get,nativeTypedSet=Uint8Array.prototype.set;',
           'function copyRaw(value,bound,code=22){if(!(value instanceof Uint8Array))bad();const n=nativeTypedLength.call(value);if(n>bound)bad(code);const out=new Uint8Array(n);nativeTypedSet.call(out,value);return out;}',
           'function addSize(n,m){if(!Number.isSafeInteger(n)||!Number.isSafeInteger(m)||n<0||m<0||n>32768||m>32768-n)bad(90);return n+m;}',
           'function textBytes(value,bound){if(typeof value!=="string"||value.length>bound||value.includes("\\0"))bad();for(let i=0;i<value.length;++i){const c=value.charCodeAt(i);if(c>=0xd800&&c<=0xdbff){if(++i===value.length)bad();const d=value.charCodeAt(i);if(d<0xdc00||d>0xdfff)bad();}else if(c>=0xdc00&&c<=0xdfff)bad();}const b=utf8Encoder.encode(value);if(b.length>bound)bad();return b;}',
           'function integer(value,width,signed){if(width===8){if(typeof value!=="bigint"||value<(signed?-(1n<<63n):0n)||value>(signed?(1n<<63n)-1n:(1n<<64n)-1n))bad();}else if(!Number.isInteger(value)||value<(signed?-(2**(width*8-1)):0)||value>(signed?2**(width*8-1)-1:2**(width*8)-1))bad();}',
           'class Reader{constructor(bytes){if(!(bytes instanceof Uint8Array)||bytes.length>32768)bad();this.b=bytes;this.v=new DataView(bytes.buffer,bytes.byteOffset,bytes.byteLength);this.at=0;}take(n){if(!Number.isInteger(n)||n<0||n>this.b.length-this.at)bad();const p=this.at;this.at+=n;return p;}u32(){return this.v.getUint32(this.take(4),true);}text(bound){const n=this.u32();if(n>bound)bad();const p=this.take(n);let s;try{s=utf8Decoder.decode(this.b.subarray(p,p+n));}catch{bad();}if(s.includes("\\0"))bad();return s;}raw(bound){const n=this.u32();if(n>bound)bad();const p=this.take(n);return this.b.slice(p,p+n);}}',
           'class Writer{constructor(size){if(size>32768)bad();this.b=new Uint8Array(size);this.v=new DataView(this.b.buffer);this.at=0;}text(value,bound){const b=textBytes(value,bound);this.v.setUint32(this.at,b.length,true);this.at+=4;this.b.set(b,this.at);this.at+=b.length;}raw(value){this.v.setUint32(this.at,value.length,true);this.at+=4;this.b.set(value,this.at);this.at+=value.length;}}']
    for record in ordered:
        sym=record.symbol
        lines += [f'/** Complete ROS{record.version} {record.name} codec; maxima are not actual lengths. */',
                  f'export const {sym}Codec=Object.freeze({{codecIdentity:{json.dumps(record.codec_identity)},maxWireBytes:{record.maximum},minWireBytes:{record.minimum},cartesianMaxWireBytes:{record.cartesian_maximum}n,aggregateWireBytes:{record.aggregate_wire_bytes},constants:Object.freeze({{'+','.join(json.dumps(c['name'])+':'+scalar_default_expression(c['type'],c['value'],'javascript') for c in record.constants)+'}),',
                  'defaultValue(){return {'+','.join(json.dumps(m.name)+':'+control_js_default(m,record,records) for m in record.members)+'};},',
                  f'validateValue(value){{{sym}Size({sym}Capture(value));return true;}},',
                  f'encode(value){{const owned={sym}Capture(value),n={sym}Size(owned),w=new Writer(n);{sym}Write(owned,w);if(w.at!==n)bad();return w.b;}},',
                  f'decode(bytes){{const owned=copyRaw(bytes,{record.maximum},90);if(owned.length<{record.minimum})bad();const r=new Reader(owned),value={sym}Read(r);if(r.at!==owned.length)bad();return value;}},',
                  'valid(bytes){try{this.decode(bytes);return true;}catch{return false;}}});',
                  f'function {sym}Size(value){{if(value===null||typeof value!=="object")bad();let n=0;']
        for member in record.members:
            a='value['+json.dumps(member.name)+']'
            if member.array:
                compare='!==' if member.array=='array' else '>'
                lines += [f'if(!Array.isArray({a})||{a}.length{compare}{member.count})bad();']
                if member.array=='sequence':lines += ['n=addSize(n,4);']
                lines += [f'for(const element of {a}){{'];a='element'
            if member.type=='string':lines += [f'if(!({a} instanceof Uint8Array)||{a}.length>{member.string_bound})bad();n=addSize(n,4+{a}.length);'] if member.string_encoding=='raw_bytes' else [f'n=addSize(n,4+textBytes({a},{member.string_bound}).length);']
            elif member.type not in parent.SCALARS:lines += [f'n=addSize(n,{records[(record.version,member.type)].symbol}Size({a}));']
            elif member.type=='bool':lines += [f'if(typeof {a}!=="boolean")bad();n=addSize(n,1);']
            elif member.type.startswith('float'):
                lines += [f'if(typeof {a}!=="number"||!Number.isFinite({a})'+(f'||!Number.isFinite(Math.fround({a}))' if member.type=='float32' else '')+f')bad();n=addSize(n,{parent.SCALARS[member.type][1]});']
            else:
                lines += [f'integer({a},{parent.SCALARS[member.type][1]},{str(member.type.startswith("int")).lower()});n=addSize(n,{parent.SCALARS[member.type][1]});']
            if member.array:lines += ['}']
        if semantic_condition(record, 'js', 'value'):
            lines += ['if('+semantic_condition(record, 'js', 'value')+')bad();']
        lines += [f'if(n>{record.maximum})bad(90);return n;}}',f'function {sym}Write(value,w){{']
        for member in record.members:
            a='value['+json.dumps(member.name)+']'
            if member.array=='sequence':lines += [f'w.v.setUint32(w.at,{a}.length,true);w.at+=4;']
            if member.array:lines += [f'for(const element of {a}){{'];a='element'
            if member.type=='string':lines += [f'w.raw({a});'] if member.string_encoding=='raw_bytes' else [f'w.text({a},{member.string_bound});']
            elif member.type not in parent.SCALARS:lines += [f'{records[(record.version,member.type)].symbol}Write({a},w);']
            else:
                width=parent.SCALARS[member.type][1]
                method='Uint8' if member.type=='bool' else {'u8':'Uint8','i8':'Int8','u16':'Uint16','i16':'Int16','u32':'Uint32','i32':'Int32','u64':'BigUint64','i64':'BigInt64','f32':'Float32','f64':'Float64'}[parent.SCALARS[member.type][0]]
                lines += [f'w.v.set{method}(w.at,{a}'+('?1:0' if member.type=='bool' else '')+f',true);w.at+={width};']
            if member.array:lines += ['}']
        lines += ['}',f'function {sym}Read(r){{const value={{}};']
        for member in record.members:
            lines += ['{']
            a='value['+json.dumps(member.name)+']'
            if member.array:
                lines += [f'{{const count='+('r.u32();' if member.array=='sequence' else str(member.count)+';')+f'if(count>{member.count})bad();{a}=[];for(let i=0;i<count;++i){{']
            if member.type=='string':expression=f'r.raw({member.string_bound})' if member.string_encoding=='raw_bytes' else f'r.text({member.string_bound})'
            elif member.type not in parent.SCALARS:expression=f'{records[(record.version,member.type)].symbol}Read(r)'
            else:
                width=parent.SCALARS[member.type][1]
                method='Uint8' if member.type=='bool' else {'u8':'Uint8','i8':'Int8','u16':'Uint16','i16':'Int16','u32':'Uint32','i32':'Int32','u64':'BigUint64','i64':'BigInt64','f32':'Float32','f64':'Float64'}[parent.SCALARS[member.type][0]]
                expression=f'r.v.get{method}(r.take({width}),true)'
            lines += [f'const decoded={expression};']
            if member.type=='bool':lines += ['if(decoded>1)bad();']
            if member.type.startswith('float'):lines += ['if(!Number.isFinite(decoded))bad();']
            decoded='decoded!==0' if member.type=='bool' else 'decoded'
            lines += [f'{a}.push({decoded});' if member.array else f'{a}={decoded};']
            if member.array:lines += ['}}']
            lines += ['}']
        if semantic_condition(record, 'js', 'value'):
            lines += ['if('+semantic_condition(record, 'js', 'value')+')bad();']
        lines += ['return value;}']
        lines += control_js_capture(record,records)
    return '\n'.join(lines+[''])


def emit_control_go(parent,records,ordered):
    """Emit Go/TinyGo Message values without reflection, native I/O or retries."""
    lines=['// Code generated by bounded_profile.py. Bounded canonical records perform no I/O.',
           '// Put this file in a separate customtypes package and use the selected SDK backend.',
           '// Encode validates complete values before allocation. Decode never returns a partial value.',
           '//go:build tinygo && (wasm || tinygo.wasm) && ros2 && !ros1','',
           'package customtypes','import("encoding/binary";"math";"unicode/utf8";"strings";ros "uwvm2.local/roswasm")',
           'var boundedInvalid=ros.Error{Code:ros.Invalid};var boundedTooLarge=ros.Error{Code:ros.TooLarge}',
           'func boundedAdd(n,m int)(int,error){if n<0||m<0||n>32768||m>32768-n{return 0,boundedTooLarge};return n+m,nil}',
           'var _=math.Float64bits;var _=utf8.Valid;var _=strings.IndexByte',
           'func boundedTake(b []byte,at *int,n int)([]byte,error){if n<0||*at>len(b)||n>len(b)-*at{return nil,boundedInvalid};start:=*at;*at+=n;return b[start:*at],nil}',
           'func boundedCount(b []byte,at *int)(uint32,error){raw,err:=boundedTake(b,at,4);if err!=nil{return 0,err};return binary.LittleEndian.Uint32(raw),nil}',
           'func boundedPut(out []byte,at *int,b []byte){copy(out[*at:],b);*at+=len(b)}']
    def go_type(record,member):
        base=('[]byte' if member.string_encoding=='raw_bytes' else 'string') if member.type=='string' else parent.SCALARS[member.type][4] if member.type in parent.SCALARS else records[(record.version,member.type)].symbol
        return '['+str(member.count)+']'+base if member.array=='array' else '[]'+base if member.array=='sequence' else base
    for record in ordered:
        sym=record.symbol
        lines += [f'// {sym} preserves every declared ROS{record.version} {record.name} field.',f'type {sym} struct {{']
        lines += [f'// {m.name}: explicit canonical bounds are committed in CodecIdentity.\n{m.name[:1].upper()+m.name[1:]} {go_type(record,m)}' for m in record.members]
        lines += ['}',f'const {sym}CodecIdentity={json.dumps(record.codec_identity)}',f'const {sym}MaxWireBytes={record.maximum}',f'const {sym}CartesianMaxWireBytes=uint64({record.cartesian_maximum})',f'const {sym}AggregateWireBytes={record.aggregate_wire_bytes}']
        for constant in record.constants:
            lines += [f'// Original constant; not a serialized field.\nconst {sym}_{constant["name"]} {parent.SCALARS[constant["type"]][4]}='+constant['value'].lower()]
        if record.has_native_defaults:
            lines += ['// Explicit declared initialization; the Go zero value is deliberately not relabeled.',
                      f'func Default{sym}(){sym}{{value:={sym}{{}}']
            for member in record.members:
                field_name=member.name[:1].upper()+member.name[1:]
                if member.native_default is not None:
                    expression=scalar_default_expression(member.type,member.native_default,'tinygo')
                    lines += [f'value.{field_name}={expression}']
                elif member.type not in parent.SCALARS and member.type != 'string':
                    child=records[(record.version,member.type)]
                    if child.has_native_defaults and member.array != 'sequence':
                        if member.array == 'array':
                            lines += [f'for i:=range value.{field_name}{{value.{field_name}[i]=Default{child.symbol}()}}']
                        else:lines += [f'value.{field_name}=Default{child.symbol}()']
            lines += ['return value}']
        lines += [f'// WireSize validates all active elements and measures exact canonical bytes.\nfunc(value {sym}) WireSize()(int,error){{n:=0']
        for member in record.members:
            lines += ['{']
            a='value.'+member.name[:1].upper()+member.name[1:]
            if member.array:
                if member.array=='sequence':lines += [f'if len({a})>{member.count}{{return 0,boundedInvalid}};next,err:=boundedAdd(n,4);if err!=nil{{return 0,err}};n=next']
                lines += [f'for _,element:=range {a} {{_ = element'];a='element'
            if member.type=='string':lines += [f'if len({a})>{member.string_bound}{{return 0,boundedInvalid}};next,err:=boundedAdd(n,4);if err!=nil{{return 0,err}};n=next;next,err=boundedAdd(n,len({a}));if err!=nil{{return 0,err}};n=next'] if member.string_encoding=='raw_bytes' else [f'if len({a})>{member.string_bound}||!utf8.ValidString({a})||strings.IndexByte({a},0)>=0{{return 0,boundedInvalid}};next,err:=boundedAdd(n,4);if err!=nil{{return 0,err}};n=next;next,err=boundedAdd(n,len({a}));if err!=nil{{return 0,err}};n=next']
            elif member.type not in parent.SCALARS:lines += [f'size,err:={a}.WireSize();if err!=nil{{return 0,err}};next,err:=boundedAdd(n,size);if err!=nil{{return 0,err}};n=next']
            else:
                if member.type.startswith('float'):lines += [f'if math.IsNaN(float64({a}))||math.IsInf(float64({a}),0){{return 0,boundedInvalid}}']
                lines += [f'next,err:=boundedAdd(n,{parent.SCALARS[member.type][1]});if err!=nil{{return 0,err}};n=next']
            if member.array:lines += ['}']
            lines += ['}']
        if semantic_condition(record, 'go', 'value'):
            lines += ['if '+semantic_condition(record, 'go', 'value')+'{return 0,boundedInvalid}']
        lines += [f'if n>{record.maximum}{{return 0,boundedTooLarge}};return n,nil}}',
                  f'// Encode satisfies the SDK Message interface with one bounded output allocation.\nfunc(value {sym}) Encode()([]byte,error){{n,err:=value.WireSize();if err!=nil{{return nil,err}};out:=make([]byte,n);at:=0;value.writeBounded(out,&at);return out,nil}}',
                  f'func(value {sym}) writeBounded(out []byte,at *int){{']
        for member in record.members:
            a='value.'+member.name[:1].upper()+member.name[1:]
            if member.array=='sequence':lines += [f'binary.LittleEndian.PutUint32(out[*at:],uint32(len({a})));*at+=4']
            if member.array:lines += [f'for _,element:=range {a} {{'];a='element'
            if member.type=='string':lines += [f'binary.LittleEndian.PutUint32(out[*at:],uint32(len({a})));*at+=4;copy(out[*at:],{a});*at+=len({a})']
            elif member.type not in parent.SCALARS:lines += [f'{a}.writeBounded(out,at)']
            elif member.type=='bool':lines += [f'if {a}{{out[*at]=1}}else{{out[*at]=0}};*at+=1']
            else:
                width=parent.SCALARS[member.type][1]
                if width==1:lines += [f'out[*at]=byte({a});*at+=1']
                else:
                    bits=f'math.Float{width*8}bits({a})' if member.type.startswith('float') else f'uint{width*8}({a})'
                    lines += [f'binary.LittleEndian.PutUint{width*8}(out[*at:],{bits});*at+={width}']
            if member.array:lines += ['}']
        lines += ['}',f'func read{sym}(b []byte,at *int)({sym},error){{var value {sym}']
        for index,member in enumerate(record.members):
            lines += ['{'];a='value.'+member.name[:1].upper()+member.name[1:]
            if member.array:
                lines += [('count,err:=boundedCount(b,at);if err!=nil{return '+sym+'{},err};' if member.array=='sequence' else 'count:=uint32('+str(member.count)+');')+f'if count>{member.count}{{return {sym}{{}},boundedInvalid}}']
                if member.array=='sequence':lines += [f'{a}=make({go_type(record,member)},int(count))']
                lines += ['for i:=0;i<int(count);i++{'];a+='[i]'
            if member.type=='string':
                prefix=f'size,err:=boundedCount(b,at);if err!=nil||size>{member.string_bound}{{return {sym}{{}},boundedInvalid}};raw,err:=boundedTake(b,at,int(size));'
                lines += [prefix+(f'if err!=nil{{return {sym}{{}},err}};{a}=append([]byte(nil),raw...)' if member.string_encoding=='raw_bytes' else f'if err!=nil||!utf8.Valid(raw){{return {sym}{{}},boundedInvalid}};{a}=string(raw);if strings.IndexByte({a},0)>=0{{return {sym}{{}},boundedInvalid}}')]
            elif member.type not in parent.SCALARS:lines += [f'decoded,err:=read{records[(record.version,member.type)].symbol}(b,at);if err!=nil{{return {sym}{{}},err}};{a}=decoded']
            else:
                width=parent.SCALARS[member.type][1]
                lines += [f'raw,err:=boundedTake(b,at,{width});if err!=nil{{return {sym}{{}},err}}']
                if member.type=='bool':lines += [f'if raw[0]>1{{return {sym}{{}},boundedInvalid}};{a}=raw[0]!=0']
                else:
                    bits='raw[0]' if width==1 else f'binary.LittleEndian.Uint{width*8}(raw)'
                    expression=f'math.Float{width*8}frombits({bits})' if member.type.startswith('float') else f'{parent.SCALARS[member.type][4]}({bits})'
                    lines += [f'{a}={expression}']
                    if member.type.startswith('float'):lines += [f'if math.IsNaN(float64({a}))||math.IsInf(float64({a}),0){{return {sym}{{}},boundedInvalid}}']
            if member.array:lines += ['}']
            lines += ['}']
        if semantic_condition(record, 'go', 'value'):
            lines += ['if '+semantic_condition(record, 'go', 'value')+'{return '+sym+'{},boundedInvalid}']
        lines += ['return value,nil}',f'// Decode{sym} rejects every trailing/truncated/invalid prefix before exposing a value.',
                  f'func Decode{sym}(b []byte)({sym},error){{if len(b)>{record.maximum}{{return {sym}{{}},boundedTooLarge}};if len(b)<{record.minimum}{{return {sym}{{}},boundedInvalid}};at:=0;value,err:=read{sym}(b,&at);if err!=nil||at!=len(b){{return {sym}{{}},boundedInvalid}};return value,nil}}']
    tag=next(i for i,line in enumerate(lines) if line.startswith('//go:build '))
    lines=lines[tag:tag+2]+lines[:tag]+lines[tag+2:]
    return '\n'.join(lines+[''])



def emit_control_as(parent,records,ordered):
    """CONTROL-only owned typed-buffer codec; no managed object address is imported.

    status/checked methods distinguish aggregate/capacity90 from malformed22.
    Codec compatibility methods retain the existing SDK's explicit valid/decode
    convention. The checked path publishes a value only after complete parsing.
    """
    lines=['/** Complete original-control canonical records. No I/O or authority. */',
      'import {Codec,Reader,validUnicode,validUtf8} from "../../assemblyscript/codecs";',
      'export class ControlEncoded {constructor(public status:u32=0,public bytes:Uint8Array=new Uint8Array(0)){} }',
      'export class ControlDecoded<T> {value:T|null=null;constructor(public status:u32=0){} }',
      'export interface ControlCodec<T> {encode(value:T):Uint8Array;valid(bytes:Uint8Array):bool;decode(bytes:Uint8Array):T;encodeChecked(value:T):ControlEncoded;decodeChecked(bytes:Uint8Array):ControlDecoded<T>;}',
      '/** One exact-size private linear buffer; finish refuses any unexpected extent. */',
      'class ControlWriter {private b:Uint8Array;private at:i32=0;private good:bool=true;constructor(n:i32){if(n<0||n>32768){this.good=false;n=0;}this.b=new Uint8Array(n);}u8(v:u8):void{if(!this.good||this.at>=this.b.length){this.good=false;return;}this.b[this.at++]=v;}integer(v:u64,n:i32):void{if(!this.good||n<0||n>8||n>this.b.length-this.at){this.good=false;return;}for(let i=0;i<n;++i)this.b[this.at++]=<u8>(v>>(i*8));}u32(v:u32):void{this.integer(v,4);}blob(v:Uint8Array):void{this.u32(<u32>v.length);if(!this.good||v.length>this.b.length-this.at){this.good=false;return;}this.b.set(v,this.at);this.at+=v.length;}text(v:string):void{this.blob(Uint8Array.wrap(String.UTF8.encode(v)));}finish():ControlEncoded{return this.good&&this.at==this.b.length?new ControlEncoded(0,this.b):new ControlEncoded(22);}}']
    def base(m,r):
        if m.type=='string':return 'Uint8Array' if m.string_encoding=='raw_bytes' else 'string'
        return parent.SCALARS[m.type][6] if m.type in parent.SCALARS else records[(r.version,m.type)].symbol
    def default(m,r):
        if m.native_default is not None:return scalar_default_expression(m.type,m.native_default,'assemblyscript')
        if m.type=='string':return 'new Uint8Array(0)' if m.string_encoding=='raw_bytes' else '""'
        return 'false' if m.type=='bool' else '0' if m.type in parent.SCALARS else 'new '+records[(r.version,m.type)].symbol+'()'
    for r in ordered:
        sym=r.symbol
        lines += [f'/** Complete original {r.name}; spellings are published in generation proof. */',f'export class {sym} {{',
          f'static readonly maxWireBytes:i32={r.maximum};',f'static readonly aggregateWireBytes:i32={r.aggregate_wire_bytes};',
          f'static readonly cartesianWireBytes:u64={r.cartesian_maximum};']
        for c in r.constants:
            lines += [f'/** Original scalar constant: not an instance field. */\nstatic readonly {c["name"]}:{parent.SCALARS[c["type"]][6]}='+scalar_default_expression(c['type'],c['value'],'assemblyscript')+';']
        for m in r.members:
            typ='Array<'+base(m,r)+'>' if m.array else base(m,r)
            value='new '+typ+'()' if m.array else default(m,r)
            lines += [f'/** Exact native field {m.name}, native scalar {m.native_scalar or m.type}. */',f'{control_as_member(m.name)}:{typ}={value};']
        if any(m.array=='array' for m in r.members):
            lines += ['constructor(){']
            for m in r.members:
                if m.array=='array':lines += [f'for(let i=0;i<{m.count};++i)this.{control_as_member(m.name)}.push({default(m,r)});']
            lines += ['}']
        lines += ['/** Exact active logical bytes or negative errno; no cartesian allocation. */','wireSize():i32 {let n:i32=0;']
        for m in r.members:
            lines += ['{'];a='this.'+control_as_member(m.name)
            if m.array:
                compare='!=' if m.array=='array' else '>'
                lines += [f'if({a}.length{compare}{m.count})return -22;']
                if m.array=='sequence':lines += [f'if(4>{r.maximum}-n)return -90;n+=4;']
                lines += [f'for(let i=0;i<{a}.length;++i){{'];a+='[i]'
            if m.type=='string':
                if m.string_encoding=='raw_bytes':lines += [f'if({a}.length>{m.string_bound})return -22;if(4+{a}.length>{r.maximum}-n)return -90;n+=4+{a}.length;']
                else:lines += [f'if(!validUnicode({a})||{a}.indexOf("\\0")>=0)return -22;const size=String.UTF8.byteLength({a});if(size>{m.string_bound})return -22;if(4+size>{r.maximum}-n)return -90;n+=4+size;']
            elif m.type not in parent.SCALARS:lines += [f'const child={a}.wireSize();if(child<0)return child;if(child>{r.maximum}-n)return -90;n+=child;']
            else:
                if m.type.startswith('float'):lines += [f'if(!isFinite({a}))return -22;']
                lines += [f'if({parent.SCALARS[m.type][1]}>{r.maximum}-n)return -90;n+={parent.SCALARS[m.type][1]};']
            # Admitted expanded-node4096 and scalar/string bounds keep sums far
            # below signed i32; check every addition before further traversal.
            lines += [f'if(n>{r.maximum})return -90;']
            if m.array:lines += ['}']
            lines += ['}']
        # Whole-value standard rules are independent of capacity accounting.
        # Keep this owned-control emitter aligned with every other language.
        if semantic_condition(r, 'as', 'this'):
            lines += ['if('+semantic_condition(r, 'as', 'this')+')return -22;']
        lines += ['return n;}','statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}','validValue():bool{return this.statusValue()==0;}',
                  '/** Internal trusted writer; public encode first measures the complete value. */','writeControl(w:ControlWriter):void{']
        for m in r.members:
            a='this.'+control_as_member(m.name)
            if m.array=='sequence':lines += [f'w.u32(<u32>{a}.length);']
            if m.array:lines += [f'for(let i=0;i<{a}.length;++i){{'];a+='[i]'
            if m.type=='string':lines += [f'w.blob({a});'] if m.string_encoding=='raw_bytes' else [f'w.text({a});']
            elif m.type not in parent.SCALARS:lines += [f'{a}.writeControl(w);']
            elif m.type=='bool':lines += [f'w.u8({a}?1:0);']
            elif m.type.startswith('float'):
                width=parent.SCALARS[m.type][1];lines += [f'w.integer(<u64>reinterpret<u{width*8}>({a}),{width});']
            else:lines += [f'w.integer(<u64>{a},{parent.SCALARS[m.type][1]});']
            if m.array:lines += ['}']
        lines += ['}',f'/** Internal shared reader. Only decodeChecked publishes accepted values. */\nstatic readControl(r:Reader):{sym}{{const value=new {sym}();']
        for m in r.members:
            lines += ['{'];a='value.'+control_as_member(m.name)
            if m.array:
                lines += [('const count=r.u32();' if m.array=='sequence' else f'const count:u32={m.count};')+f'if(!r.valid||count>{m.count}){{r.valid=false;return value;}}',f'{a}=new Array<{base(m,r)}>();for(let i:u32=0;i<count;++i){{']
            if m.type=='string':
                lines += [f'const size=r.u32();if(!r.valid||size>{m.string_bound}){{r.valid=false;return value;}}const raw=r.take(size);']
                lines += ['const decoded=raw;'] if m.string_encoding=='raw_bytes' else ['if(!r.valid||!validUtf8(raw,false)){r.valid=false;return value;}const decoded=String.UTF8.decode(raw.buffer);']
            elif m.type not in parent.SCALARS:lines += [f'const decoded={records[(r.version,m.type)].symbol}.readControl(r);']
            elif m.type=='bool':lines += ['const bit=r.u8();if(bit>1)r.valid=false;const decoded=bit!=0;']
            elif m.type.startswith('float'):
                width=parent.SCALARS[m.type][1];lines += [f'const decoded=reinterpret<f{width*8}>(<u{width*8}>r.integer({width}));if(!isFinite(decoded))r.valid=false;']
            else:lines += [f'const decoded=<{base(m,r)}>r.integer({parent.SCALARS[m.type][1]});']
            lines += [f'{a}.push(decoded);' if m.array else f'{a}=decoded;']
            if m.array:lines += ['}']
            lines += ['}']
        if semantic_condition(r, 'as', 'value'):
            lines += ['if('+semantic_condition(r, 'as', 'value')+')r.valid=false;']
        lines += ['return value;}','}',f'export class {sym}Codec implements ControlCodec<{sym}>{{',f'static readonly codecIdentity:string={json.dumps(r.codec_identity)};',f'static readonly maxWireBytes:i32={r.maximum};',
          f'encodeChecked(value:{sym}):ControlEncoded{{const n=value.wireSize();if(n<0)return new ControlEncoded(<u32>(-n));const w=new ControlWriter(n);value.writeControl(w);return w.finish();}}',
          f'encode(value:{sym}):Uint8Array{{return this.encodeChecked(value).bytes;}}',
          f'decodeChecked(bytes:Uint8Array):ControlDecoded<{sym}>{{const out=new ControlDecoded<{sym}>();if(bytes.length>{r.maximum}){{out.status=90;return out;}}if(bytes.length<{r.minimum}){{out.status=22;return out;}}const reader=new Reader(bytes),value={sym}.readControl(reader);if(!reader.done()){{out.status=22;return out;}}const status=value.statusValue();if(status){{out.status=status;return out;}}out.value=value;return out;}}',
          f'valid(bytes:Uint8Array):bool{{return this.decodeChecked(bytes).status==0;}}',
          f'decode(bytes:Uint8Array):{sym}{{const out=this.decodeChecked(bytes);return out.value===null?new {sym}():changetype<{sym}>(out.value);}}','}']
    return '\n'.join(lines+[''])




def control_managed_products(parent,records,ordered,interfaces,pins):
    """Produce complete four-language RECORD source proof, never Native authority.

    The existing generation.json C/C++ inventory is consumed strictly by real
    native generators and remains its authoritative product contract. This
    independent managed proof binds additional products rather than changing
    that old list or inventing native adapters/identity hashes.
    """
    if len(ordered)!=32 or any(r.profile!=CONTROL_PROFILE for r in ordered) or any(i.profile!=CONTROL_PROFILE for i in interfaces):
        raise parent.GenerationError('CONTROL-only complete32 record IR required')
    maps=control_member_maps(ordered)
    products={'bounded_records.rs':emit_control_rust(parent,records,ordered),
              'bounded_records.mjs':emit_control_js(parent,records,ordered),
              'bounded_records.go':emit_control_go(parent,records,ordered),
              'bounded_records.ts':emit_control_as(parent,records,ordered)}
    # These facts bind the genuine root separately from its components.
    services=control_service_facts(interfaces)
    facts=emit_control_service_facts(services)
    for extension,body in facts.items():products['bounded_records.'+extension]+=body
    # Full control Services require actual owned raw12 transport, not old
    # business call/submit/event_poll. That independent layer is in progress.
    templates=[]
    for source in templates:
        raw=parent.bounded_read(Path(__file__).parent/source['generator_relative'],parent.MAX_SOURCE_BYTES)
        if len(raw)!=source['bytes']or hashlib.sha256(raw).hexdigest()!=source['sha256']:
            raise parent.GenerationError('CONTROL managed RPC template changed during generation')
    proof={'format':'uwvm-ros-original-node-control-managed-source-v1',
      'canonical_profile':CONTROL_PROFILE,'status':'GENERATED_SOURCE_ONLY_NOT_EXECUTED',
      'implemented_record_outputs':['rust','javascript','assemblyscript','tinygo'],
      'implemented_service_outputs':[],
      'service_status':'IN_PROGRESS_SEPARATE_OWNED_RMW_RAW12_TRANSPORT_NOT_EMITTED',
      'source_inputs':pins,'fixed_facade_templates':templates,
      'generator_sha256':hashlib.sha256(parent.bounded_read(Path(parent.__file__),parent.MAX_SOURCE_BYTES)).hexdigest(),
      'bounded_generator_sha256':hashlib.sha256(parent.bounded_read(Path(__file__),parent.MAX_SOURCE_BYTES)).hexdigest(),
      'native_registry_emitted':False,'implicit_permissions':[],
      'native_identity_policy':'unchanged independently exported original Native identities; generated Guest codecs are not Native proof',
      'record_names':[r.name for r in ordered],
      'services':services,
      'registered_interfaces':[i.name for i in interfaces if i.registered],
      'complete_member_spelling_maps':maps,
      'constants':{r.name:r.constants for r in ordered if r.constants},
      'record_contracts':{r.name:{'minimum_wire_bytes':r.minimum,'maximum_wire_bytes':r.maximum,
         'cartesian_maximum_wire_bytes':r.cartesian_maximum,'aggregate_wire_bytes':r.aggregate_wire_bytes,
         'adapter_codec_identity':r.codec_identity,'native_identity':r.identity,
         'string_encodings':{m.name:m.string_encoding for m in r.members if m.type=='string'}} for r in ordered},
      'outputs':{name:{'bytes':len(body.encode()),'sha256':hashlib.sha256(body.encode()).hexdigest()} for name,body in products.items()},
      'ownership':'Rust/Go ordinary exclusive values; JS privately captures bounded fields before encode; AS one exact private typed buffer. Complete decode only. Terminal process abort/OOM is not recoverable cleanup.',
      'aggregate_error':90,'malformed_error':22,
      'physical_memory':'wire quota32768 is not full record/cartesian storage, stack/RSS or foreign DDS allocation bound'}
    products['generation.control-managed.json']=json.dumps(proof,indent=2)+'\n'
    return products,proof



def control_service_facts(interfaces):
    """Copy original root/component identities, never derive a Native identity."""
    return [{'symbol':i.symbol,'native_type':i.name,'native_identity':i.identity,
             'codec_identity':i.codec_identity,
             'request':{'type':i.components['request'].name,'codec_identity':i.components['request'].codec_identity,'maximum_wire_bytes':i.components['request'].maximum},
             'response':{'type':i.components['response'].name,'codec_identity':i.components['response'].codec_identity,'maximum_wire_bytes':i.components['response'].maximum}}
            for i in interfaces if i.kind=='service']


def emit_control_service_facts(services):
    bodies={ext:[] for ext in ('rs','mjs','go','ts')}
    bodies['rs']=['/// Explicit original Service facts; none of these strings grants authority.','pub struct ControlServiceIdentity {pub native_type:&\'static str,pub codec_identity:&\'static str,pub native_identity:Option<&\'static str>}']
    for row in services:
        name=row['symbol'];native=json.dumps(row['native_type']);codec=json.dumps(row['codec_identity']);identity=json.dumps(row['native_identity'] or '')
        bodies['rs'] += [f'pub const {name}Identity:ControlServiceIdentity=ControlServiceIdentity{{native_type:{native},codec_identity:{codec},native_identity:'+('None' if row['native_identity'] is None else 'Some('+identity+')')+'};']
        bodies['mjs'] += [f'export const {name}Identity=Object.freeze({{nativeType:{native},codecIdentity:{codec},nativeIdentity:'+json.dumps(row['native_identity'])+'});']
        bodies['go'] += [f'const {name}NativeType={native}',f'const {name}CodecIdentity={codec}',f'const {name}NativeIdentity={identity}',f'const {name}HasNativeIdentity='+str(row['native_identity'] is not None).lower()]
        bodies['ts'] += [f'export class {name}Identity{{static readonly nativeType:string={native};static readonly codecIdentity:string={codec};static readonly nativeIdentity:string={identity};static readonly hasNativeIdentity:bool='+str(row['native_identity'] is not None).lower()+';}']
    return {ext:'\n'.join(lines)+'\n' for ext,lines in bodies.items()}
