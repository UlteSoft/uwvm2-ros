//go:build tinygo && (wasm || tinygo.wasm) && ros2 && !ros1 && unitree_lowstate && !unitree_topicrpc

// Real same-main GO/HG LowState command. Stage the complete actual example,
// lifecycle and selected generated family. Remove ONLY its exact empty
// "func main() {}" from the owned copy, recording the original/copy derivation;
// this file supplies main. Every actual typed function body remains unchanged.
package main

func main() {
	mode:=typedSetup(selectedFamily);code:=initializeCase(mode,selectedFamily,0);typedOpened=true;typedCheck(code)
	wanted:=uint32(0);if mode==0 {wanted=1}
	typedRequire(caseStatus()==0&&receivedCount()==0&&deniedVerified()==wanted)
	typedAppend("TYPED_INITIALIZED ");typedNumber(mode);typedLine("\n")
	if mode==0 {
		typedRequire(pollOnce()==9&&closeCase()==9);typedLine("TYPED_DENIED_VERIFIED\n");typedRequire(typedControl()=='F')
	} else {
		polls:=uint32(0)
		for {
			typedRequire(typedControl()=='R');if polls>=4096 {typedFail(110)};polls++;result:=pollOnce()
			if result==11 {typedLine("TYPED_AGAIN\n");continue};typedCheck(result)
			typedRequire(caseStatus()==0&&receivedCount()==1&&deniedVerified()==0);break
		}
		typedAppend("TYPED_STATE ");typedNumber(observedTick());typedAppend(" ");typedNumber(observedCRC());typedAppend(" ");typedNumber(uint32(selectedMotors))
		for slot:=uint32(0);slot<uint32(selectedMotors);slot++ {for field:=uint32(0);field<3;field++ {typedAppend(" ");typedNumber(motorWord(slot,field))}}
		typedRequire(caseStatus()==0);typedLine("\n")
		typedRequire(typedControl()=='C');typedCheck(closeCase());typedRequire(closeCase()==9&&pollOnce()==9)
		typedLine("TYPED_CLOSED\n");typedRequire(typedControl()=='F')
	}
	typedComplete()
}
