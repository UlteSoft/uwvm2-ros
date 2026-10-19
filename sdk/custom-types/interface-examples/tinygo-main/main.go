//go:build tinygo && (wasm || tinygo.wasm) && ros2 && !ros1

// Command rpcexample type-checks the real public RPC library and exercises one
// complete generated canonical value. It opens no alias and performs no RPC.
// Native Service/Action/default-denied behavior requires separate owned cases.
package main

import (
 rpc "uwvm2.local/customrpc"
 "uwvm2.local/customrpc/customtypes"
)

func main() {
 // Refer to the real reusable API; never replace its implementation with a
 // source stub. The application must supply an actual opened owner to call it.
 _ = rpc.SubmitStep
 value := customtypes.Ros2Demo_rpcSrvIncrement_Request{Value: 1}
 wire, err := value.Encode()
 if err != nil || len(wire) != 4 || wire[0] != 1 || wire[1] != 0 || wire[2] != 0 || wire[3] != 0 {
  panic("complete generated Increment request codec refused canonical value")
 }
}
