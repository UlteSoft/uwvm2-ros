"""ROS2 QoS values for the explicit-binding source migration profile.

QoS is part of the predeclared host resource contract. A requested profile must
match the installed binding; this module never silently substitutes host QoS.
Deadline/lifespan/liveliness events require further native protocol support and
are rejected instead of being ignored. Numeric values match ROS2 rmw policies.
"""
from enum import IntEnum


class HistoryPolicy(IntEnum):
    SYSTEM_DEFAULT = 0
    KEEP_LAST = 1
    KEEP_ALL = 2
    UNKNOWN = 3


class ReliabilityPolicy(IntEnum):
    SYSTEM_DEFAULT = 0
    RELIABLE = 1
    BEST_EFFORT = 2
    UNKNOWN = 3


class DurabilityPolicy(IntEnum):
    SYSTEM_DEFAULT = 0
    TRANSIENT_LOCAL = 1
    VOLATILE = 2
    UNKNOWN = 3


class QoSProfile:
    """Depth/reliability/durability/history, validated before any endpoint opens."""
    def __init__(self, *, depth=None, history=HistoryPolicy.KEEP_LAST,
                 reliability=ReliabilityPolicy.RELIABLE,
                 durability=DurabilityPolicy.VOLATILE, **unsupported):
        if unsupported:
            raise NotImplementedError('QoS fields require native protocol support: ' + ','.join(unsupported))
        history = HistoryPolicy(history)
        if depth is None:
            if history == HistoryPolicy.KEEP_LAST:
                raise ValueError('KEEP_LAST requires depth')
            depth = 0
        if type(depth) is not int or not 0 <= depth <= 0xffffffff:
            raise ValueError('QoS depth')
        if history == HistoryPolicy.KEEP_LAST and depth == 0:
            raise ValueError('KEEP_LAST depth must be positive')
        self.depth, self.history = depth, history
        self.reliability, self.durability = ReliabilityPolicy(reliability), DurabilityPolicy(durability)

    def key(self):
        """An immutable comparison key; no native QoS is changed."""
        return self.history, self.depth, self.reliability, self.durability


QoSHistoryPolicy = HistoryPolicy
QoSReliabilityPolicy = ReliabilityPolicy
QoSDurabilityPolicy = DurabilityPolicy
qos_profile_default = QoSProfile(depth=10)
qos_profile_sensor_data = QoSProfile(depth=5, reliability=ReliabilityPolicy.BEST_EFFORT)
qos_profile_services_default = QoSProfile(depth=10)
qos_profile_system_default = QoSProfile(history=HistoryPolicy.SYSTEM_DEFAULT,
                                       reliability=ReliabilityPolicy.SYSTEM_DEFAULT,
                                       durability=DurabilityPolicy.SYSTEM_DEFAULT)


def checked(value):
    """The integer overload denotes KEEP_LAST depth with standard defaults."""
    if type(value) is int:
        return QoSProfile(depth=value)
    if not isinstance(value, QoSProfile):
        raise TypeError('QoSProfile or depth required')
    return value
