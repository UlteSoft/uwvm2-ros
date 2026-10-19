"""Gen3 model checks around the complete standard FollowJointTrajectory SDK."""
from ..trajectory_client import TrajectoryClient

from .models import GEN3_6DOF, GEN3_7DOF


class Gen3Client(TrajectoryClient):
    def __init__(self, *args, model, **kwargs):
        if model not in ('gen3-6dof', 'gen3-7dof'):
            raise ValueError('explicit Gen3 6DOF/7DOF model required')
        self.model = model
        self.joint_names = GEN3_6DOF if model == 'gen3-6dof' else GEN3_7DOF
        super().__init__(*args, **kwargs)

    def submit(self, complete_goal):
        # Selected driver config has allow_partial_joints_goal=false. This is
        # extra model validation; the complete standard codec still validates
        # every other field. It is not a joint-limit or collision guarantee.
        if tuple(complete_goal.trajectory.joint_names) != self.joint_names:
            raise ValueError('complete selected joint order required')
        return super().submit(complete_goal)
