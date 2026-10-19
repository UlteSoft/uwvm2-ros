/** Stage as header.ts beside selected ROS2 binding.ts. */
import {CIOState} from './binding';
export function headerMatches(v:CIOState):bool{return v.header.stamp.sec == -2&&v.header.stamp.nanosec==300000000;}
