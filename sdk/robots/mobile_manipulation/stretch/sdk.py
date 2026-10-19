"""Stretch complete FJT wrapper, retaining mode/tool-specific native validation."""
from ..trajectory_client import TrajectoryClient


class StretchClient(TrajectoryClient):
    def __init__(self, *args, model, mode, **kwargs):
        if model not in ('RE1V0', 'RE2V0', 'SE3'):
            raise ValueError('explicit actual Stretch model required')
        if mode not in ('position', 'trajectory', 'navigation'):
            raise ValueError('explicit driver mode required')
        # This selection describes the expected deployment; it does not call
        # switch-mode, home/stow, runstop or command any actuator by itself.
        self.model, self.mode = model, mode
        super().__init__(*args, **kwargs)
