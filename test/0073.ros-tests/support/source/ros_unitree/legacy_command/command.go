//go:build tinygo && (wasm || tinygo.wasm) && ros1 && !ros2

// Actual TinyGo command main. Compile with real legacy SDK application files,
// full native-bound generated family and the actual TinyGo command startup.
package main
func main(){
 mode:=legacySetup(4);legacyOpened=true;legacyCheck(initializeCase(mode,4,0))
 wanted:=uint32(0);if mode==0{wanted=2};legacyRequire(lastStatus==0&&received==0&&denials==wanted)
 legacyAppend("LEGACY_INITIALIZED ");legacyNumber(mode);legacyLine("\n")
 if mode==0{
  legacyRequire(pollOnce()==9&&submitOnce()==9&&closeCase(0)==9);legacyLine("LEGACY_DENIED 2\n");legacyRequire(legacyControl()=='F')
 }else{
  attempts:=uint32(0);for{
   legacyRequire(legacyControl()=='R');if attempts>=4096{legacyFail(110)};attempts++;code:=pollOnce()
   if code==11{legacyLine("LEGACY_AGAIN\n");continue};legacyCheck(code);break
  }
  legacyRequire(lastStatus==0&&received==1&&denials==0&&len(observed)>0&&len(observed)<=1536)
  legacyAppend("LEGACY_GUEST_STATE ");legacyNumber(uint32(len(observed)));legacyAppend(" ")
  const digits="0123456789abcdef"
  for _,b:=range observed{legacyRequire(legacyUsed+2<=4096);legacyOutput[legacyUsed]=digits[b>>4];legacyUsed++;legacyOutput[legacyUsed]=digits[b&15];legacyUsed++};legacyLine("\n")
  legacyRequire(legacyControl()=='S');legacyCheck(submitOnce());legacyRequire(submitOnce()==9);legacyLine("LEGACY_SUBMITTED\n")
  legacyRequire(legacyControl()=='C');legacyCheck(closeCase(0));legacyRequire(closeCase(0)==9&&pollOnce()==9&&submitOnce()==9);legacyLine("LEGACY_CLOSED\n");legacyRequire(legacyControl()=='F')
 }
 legacyComplete()
}
