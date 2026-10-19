//go:build tinygo && (wasm || tinygo.wasm) && ros2 && !ros1 && rmw_service_v1
// Authored and UNRUN local JSON/typed-record contracts. No Native/GPU claim.
package main
import "testing"
func TestFixedShapeDoesNotDiscardExtra(t *testing.T){_,e:=decodeInput([]byte(`{"application_nonce":1,"video":[{"key":"cam","shape":[1,1,1,1,3,99],"pixels":[1,2,3]}],"state":[],"language":[]}`));if e==nil{t.Fatal("overlong shape must not be projected")}}
func TestDuplicateKeyIsRefused(t *testing.T){_,e:=decodeInput([]byte(`{"application_nonce":1,"application_nonce":2,"video":[],"state":[],"language":[]}`));if e==nil{t.Fatal("duplicate key")}}
func TestCaseAliasCannotOverwrite(t *testing.T){_,e:=decodeInput([]byte(`{"application_nonce":1,"video":[{"key":"cam","Key":"other","shape":[1,1,1,1,3],"pixels":[1,2,3]}],"state":[],"language":[]}`));if e==nil{t.Fatal("case alias overwrite")}}
func TestFullUnsignedNonceIsExact(t *testing.T){q,e:=decodeInput([]byte(`{"application_nonce":9007199254740993,"video":[],"state":[],"language":[]}`));if e!=nil||q.Application_nonce!=9007199254740993{t.Fatal("u64 projected through f64")}}
func TestByteOverflowIsNotNarrowed(t *testing.T){_,e:=decodeInput([]byte(`{"application_nonce":1,"video":[{"key":"cam","shape":[1,1,1,1,3],"pixels":[1,2,256]}],"state":[],"language":[]}`));if e==nil{t.Fatal("pixel uint8 wrap")}}
func TestInvalidUnicodeIsNotReplacement(t *testing.T){if strictText([]byte{'"',255,'"'})||strictText([]byte(`"\ud800"`)){t.Fatal("invalid Unicode replaced")};if !strictText([]byte(`"\ud83d\ude00"`)){t.Fatal("actual surrogate pair")}}
func TestEveryCompleteValueRetained(t *testing.T){q,e:=decodeInput([]byte(`{"application_nonce":1,"video":[{"key":"cam","shape":[1,1,1,1,3],"pixels":[1,2,255]}],"state":[{"key":"arm","shape":[1,1,2],"values":[1.25,-2.5]}],"language":[{"key":"task","shape":[1,1],"texts":["move µ"]}]}`));if e!=nil||q.Video[0].Pixels[2]!=255||q.State[0].Values[1]!=-2.5||q.Language[0].Texts[0]!="move µ"{t.Fatal("whole typed value differs")}}
