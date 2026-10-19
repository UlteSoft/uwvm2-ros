//go:build tinygo && (wasm || tinygo.wasm) && ros1 && !ros2 && elephant_kinetic_mycobot && !elephant_kinetic_mypalletizer && !elephant_melodic_mecharm && !elephant_melodic_myarm && !elephant_melodic_mybuddy && !elephant_melodic_mycobot320 && !elephant_melodic_mycobot && !elephant_melodic_mypalletizer && !elephant_melodic_ultraarm && !elephant_noetic_mecharm && !elephant_noetic_myarm && !elephant_noetic_mybuddy && !elephant_noetic_mycobot320 && !elephant_noetic_mycobot && !elephant_noetic_pro450 && !elephant_noetic_mypalletizer && !elephant_noetic_ultraarm && !elephant_foxy_mecharm && !elephant_foxy_mycobot && !elephant_foxy_pro450 && !elephant_foxy_mypalletizer && !elephant_galactic_mecharm && !elephant_galactic_mycobot && !elephant_galactic_pro450 && !elephant_galactic_mypalletizer && !elephant_humble_mecharm && !elephant_humble_mycobot && !elephant_humble_pro450 && !elephant_humble_mypalletizer && !elephant_humble_ultraarm

// Exactly one original model/domain and actual controlled backend.
package main
import r "uwvm2.local/elephant-example/records_kinetic"
type State = r.Ros1Mycobot_communicationMycobotAngles
const stateCapacity = r.Ros1Mycobot_communicationMycobotAnglesMaxWireBytes
func decodeState(b []byte)(State,error){return r.DecodeRos1Mycobot_communicationMycobotAngles(b)}
