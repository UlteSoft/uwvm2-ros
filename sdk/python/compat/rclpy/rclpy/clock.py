"""Explicit ROS clock access; no ungranted WASI or host clock fallback."""
from enum import IntEnum
from .time import Time


class ClockType(IntEnum):
    SYSTEM_TIME = 1
    STEADY_TIME = 2
    ROS_TIME = 3


class Clock:
    """One separately granted clock kind and its retained local owner."""
    def __init__(self, *, clock_type=ClockType.SYSTEM_TIME, context=None, node_name=None):
        if context is None or node_name is None or not context.ok():
            raise RuntimeError('explicit initialized context and clock binding required')
        self.clock_type = ClockType(clock_type)
        self.operation = {ClockType.SYSTEM_TIME: 'time_read_system',
                          ClockType.STEADY_TIME: 'time_read_steady',
                          ClockType.ROS_TIME: 'time_read_ros'}[self.clock_type]
        binding = context.binding(self.operation, node_name)
        self.owner = context.session.open(binding.alias)

    def now(self):
        data = self.owner.invoke(self.operation, capacity=8).checked().data
        if len(data) != 8:
            raise ValueError('clock response must be eight bytes')
        return Time(nanoseconds=int.from_bytes(data, 'little'), clock_type=self.clock_type)
