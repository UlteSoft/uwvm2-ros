//go:build tinygo && (wasm || tinygo.wasm) && ros2 && !ros1 && unitree_topicrpc && !unitree_lowstate

// Real same-main Request/Response topic fixture. This never invokes a ROS
// service or firmware API. Exactly one selected main replaces only the empty
// main in the owned actual example copy; all real typed bodies are unchanged.
package main

func main() {
	mode:=typedSetup(3);code:=initializeCase(mode,1,1);typedOpened=true;typedCheck(code)
	wanted:=uint32(0);if mode==0 {wanted=1}
	typedRequire(caseStatus()==0&&receivedCount()==0&&deniedVerified()==wanted)
	typedAppend("TYPED_INITIALIZED ");typedNumber(mode);typedLine("\n")
	if mode==0 {
		typedRequire(pollOnce()==9&&closeCase()==9&&submitOnce()==9);typedLine("TYPED_DENIED_VERIFIED\n");typedRequire(typedControl()=='F')
	} else {
		typedRequire(typedControl()=='S');typedCheck(submitOnce());typedRequire(submitOnce()==9);typedLine("TYPED_SUBMITTED\n")
		polls:=uint32(0)
		for {
			typedRequire(typedControl()=='R');if polls>=4096 {typedFail(110)};polls++;result:=pollOnce()
			if result==11 {typedLine("TYPED_AGAIN\n");continue};typedCheck(result)
			typedRequire(caseStatus()==0&&receivedCount()==1&&deniedVerified()==0);break
		}
		typedAppend("TYPED_RPC ");typedNumber(responseCodeValue());typedRequire(caseStatus()==0);typedLine(" 1 0\n")
		typedRequire(typedControl()=='C');typedCheck(closeCase());typedRequire(closeCase()==9&&pollOnce()==9&&submitOnce()==9)
		typedLine("TYPED_CLOSED\n");typedRequire(typedControl()=='F')
	}
	typedComplete()
}
