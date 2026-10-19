"""Native/Guest-neutral exact selected Gen3 joint-name source facts."""
GEN3_6DOF = tuple("joint_" + str(index) for index in range(1, 7))
GEN3_7DOF = tuple("joint_" + str(index) for index in range(1, 8))
