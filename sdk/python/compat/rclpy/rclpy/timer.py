"""Local timer with explicit ROS clock reads and one callback per executor pass."""
import math
from roswasm import Result


class Timer:
    def __init__(self, period_sec, callback, clock, *, autostart=True):
        if type(period_sec) not in (int, float) or not math.isfinite(period_sec) or period_sec <= 0 or not callable(callback):
            raise ValueError('positive finite period and callback required')
        period = int(period_sec * 1000000000)
        if not 0 < period < 1 << 63:
            raise ValueError('timer nanosecond range')
        self.timer_period_ns, self.callback, self.clock = period, callback, clock
        self._canceled, self._destroyed, self._busy = not autostart, False, False
        self._previous = clock.now().nanoseconds
        self._deadline = self._previous + period

    def cancel(self):
        self._canceled = True

    def is_canceled(self):
        return self._canceled

    def reset(self):
        if self._destroyed:
            raise RuntimeError('destroyed timer')
        self._previous = self.clock.now().nanoseconds
        self._deadline = self._previous + self.timer_period_ns
        self._canceled = False

    def destroy(self):
        self._destroyed = self._canceled = True

    def poll_once(self):
        if self._destroyed or self._canceled:
            return Result(11)
        if self._busy:
            return Result(17)
        self._busy = True
        try:
            now = self.clock.now().nanoseconds
            if now < self._previous:
                self._deadline = now + self.timer_period_ns
            self._previous = now
            if now < self._deadline:
                return Result(11)
            self._deadline += ((now - self._deadline) // self.timer_period_ns + 1) * self.timer_period_ns
            self.callback()
            return Result(0)
        finally:
            self._busy = False
