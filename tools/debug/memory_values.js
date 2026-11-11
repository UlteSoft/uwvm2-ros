"use strict";
// Interpret copied, authorized bytes only. No runtime access or host evaluation.
// Formats and reference sources are documented in README.memory-values.md.
(function(root,factory) {
  if(typeof module==="object" && module.exports)module.exports=factory();
  else root.uwvmMemoryValues=factory();
})(typeof globalThis!=="undefined"?globalThis:this,()=>{
  const ROUNDING=["nearest_to_even","nearest_to_odd","nearest_toward_plus_infinity","nearest_toward_minus_infinity","nearest_toward_zero","nearest_away_from_zero","toward_plus_infinity","toward_minus_infinity","toward_zero","away_from_zero"];
  function increment(negative,kept,remainder,scale,rounding) {
    if(remainder===0n)return false;
    if(rounding.startsWith("nearest_")) {
      if(remainder*2n!==scale)return remainder*2n>scale;
      if(rounding==="nearest_to_even")return kept%2n===1n;
      if(rounding==="nearest_to_odd")return kept%2n===0n;
    }
    if(rounding.endsWith("plus_infinity"))return !negative;
    if(rounding.endsWith("minus_infinity"))return negative;
    return rounding.endsWith("away_from_zero");
  }
  const mask=bits=>(1n<<BigInt(bits))-1n;
  function integer(bytes,order="little") {
    let sequence=[...bytes];
    if(order==="little")sequence.reverse();
    else if(order==="pdp") {
      if(sequence.length%2) {if(sequence.length!==1)throw new Error("PDP requires complete 16-bit words");}
      else for(let i=0;i<sequence.length;i+=2)[sequence[i],sequence[i+1]]=[sequence[i+1],sequence[i]];
    } else if(order!=="big")throw new Error("Unknown byte order");
    let value=0n;for(const n of sequence)value=(value<<8n)|BigInt(n);return value;
  }
  function decimalText(negative,coefficient,exponent,precision,rounding="nearest_to_even") {
    const sign=negative?"-":"";
    if(coefficient===0n)return {value:sign+"0",rounded:false};
    let digits=coefficient.toString(),rounded=false;
    if(precision && digits.length>precision) {
      const removed=digits.length-precision,scale=10n**BigInt(removed),remainder=coefficient%scale;
      let kept=coefficient/scale;rounded=remainder!==0n;
      if(increment(negative,kept,remainder,scale,rounding))kept++;
      digits=kept.toString();exponent+=removed;
    }
    while(digits.endsWith("0")){digits=digits.slice(0,-1);exponent++;}
    const point=digits.length+exponent,scientific=point-1;
    let text;
    if(scientific>=-6 && scientific<21) {
      text=point<=0?"0."+"0".repeat(-point)+digits:point>=digits.length?digits+"0".repeat(point-digits.length):digits.slice(0,point)+"."+digits.slice(point);
    } else text=digits[0]+(digits.length>1?"."+digits.slice(1):"")+"e"+(scientific>=0?"+":"")+scientific;
    return {value:(rounded?"≈ ":"")+sign+text,rounded};
  }
  function binaryValue(negative,significand,power,precision,rounding) {
    if(significand===0n)return {value:negative?"-0":"0",exact:(negative?"-":"")+"0x0p+0",hexfloat:(negative?"-":"")+"0x0p+0"};
    while(!(significand&1n)){significand>>=1n;power++;}
    const length=significand.toString(2).length,padding=(4-(length-1)%4)%4,hex=(significand<<BigInt(padding)).toString(16),exponent=power+length-1;
    const fraction=hex.slice(1).replace(/0+$/,""),exact=(negative?"-":"")+"0x"+hex[0]+(fraction?"."+fraction:"")+"p"+(exponent>=0?"+":"")+exponent;
    const coefficient=power>=0?significand<<BigInt(power):significand*5n**BigInt(-power);
    return {...decimalText(negative,coefficient,Math.min(0,power),precision,rounding),exact,hexfloat:exact};
  }
  function binary(bits,exponentBits,fractionBits,precision,finiteOnly=false,explicitInteger=false,rounding="nearest_to_even") {
    const storage=fractionBits+(explicitInteger?1:0),negative=!!(bits>>BigInt(storage+exponentBits)&1n);
    const fraction=bits&mask(fractionBits),exponent=Number(bits>>BigInt(storage)&mask(exponentBits)),max=2**exponentBits-1,bias=2**(exponentBits-1)-1;
    const leading=explicitInteger?Number(bits>>BigInt(fractionBits)&1n):exponent!==0?1:0;
    if(explicitInteger && exponent!==0 && !leading)return {value:"Unsupported x87 encoding",exact:"0x"+bits.toString(16)};
    if(exponent===max && (finiteOnly?fraction===mask(fractionBits):true)) {
      if(!finiteOnly && fraction===0n)return {value:negative?"-Infinity":"Infinity"};
      return {value:(negative?"-":"")+(finiteOnly||fraction>>(BigInt(fractionBits)-1n)?"qNaN":"sNaN"),exact:"NaN payload 0x"+fraction.toString(16)};
    }
    return binaryValue(negative,(BigInt(leading)<<BigInt(fractionBits))|fraction,(exponent||1)-bias-fractionBits,precision,rounding);
  }
  function dpd(code) {
    const p=(code>>9)&1,q=(code>>8)&1,r=(code>>7)&1,s=(code>>6)&1,t=(code>>5)&1,u=(code>>4)&1,v=(code>>3)&1,w=(code>>2)&1,x=(code>>1)&1,y=code&1;
    let a,b,c;
    if(!v){a=4*p+2*q+r;b=4*s+2*t+u;c=4*w+2*x+y;}
    else if(!w&&!x){a=4*p+2*q+r;b=4*s+2*t+u;c=8+y;}
    else if(!w&&x){a=4*p+2*q+r;b=8+u;c=4*s+2*t+y;}
    else if(w&&!x){a=8+r;b=4*s+2*t+u;c=4*p+2*q+y;}
    else if(!s&&!t){a=8+r;b=8+u;c=4*p+2*q+y;}
    else if(!s&&t){a=8+r;b=4*p+2*q+u;c=8+y;}
    else if(s&&!t){a=4*p+2*q+r;b=8+u;c=8+y;}
    else {a=8+r;b=8+u;c=8+y;}
    return a*100+b*10+c;
  }
  function decimal(bits,width,encoding,precision,rounding) {
    const [digits,continuation,bias]=({32:[7,6,101],64:[16,8,398],128:[34,12,6176]})[width];
    const negative=!!(bits>>BigInt(width-1)),combination=Number(bits>>BigInt(width-6)&31n);
    if(combination===30)return {value:negative?"-Infinity":"Infinity"};
    if(combination===31)return {value:(negative?"-":"")+(bits>>BigInt(width-7)&1n?"sNaN":"qNaN"),exact:"Raw NaN 0x"+bits.toString(16)};
    let coefficient,exponent,noncanonical=false;
    if(encoding==="DPD") {
      const tail=(digits-1)/3*10,leading=combination<24?combination&7:8+(combination&1),high=combination<24?combination>>3:(combination>>1)&3;
      exponent=Number(bits>>BigInt(tail)&mask(continuation))+(high<<continuation)-bias;
      coefficient=BigInt(leading);
      for(let shift=tail-10;shift>=0;shift-=10)coefficient=coefficient*1000n+BigInt(dpd(Number(bits>>BigInt(shift)&1023n)));
    } else {
      const expBits=continuation+2,ordinary=width-1-expBits,steering=Number(bits>>BigInt(width-3)&3n)===3,shift=ordinary-(steering?2:0);
      exponent=Number(bits>>BigInt(shift)&mask(expBits))-bias;
      coefficient=(bits&mask(shift))|(steering?1n<<BigInt(ordinary):0n);
      if(coefficient>=10n**BigInt(digits)){coefficient=0n;noncanonical=true;}
    }
    const exact=(negative?"-":"")+coefficient+"e"+(exponent>=0?"+":"")+exponent+(noncanonical?" (non-canonical coefficient: zero)":"");
    return {...decimalText(negative,coefficient,exponent,precision,rounding),exact,coefficient:coefficient.toString(),exponent};
  }
  function interpret(input,order="little",options={}) {
    const rounding=options.rounding||"nearest_to_even",precision=options.precision;
    if(!ROUNDING.includes(rounding))throw new Error("Unknown rounding mode");
    if(precision!==undefined && (!Number.isInteger(precision)||precision<1||precision>36))throw new Error("Use 1..36 significant digits or Auto");
    if(!["little","big","pdp"].includes(order))throw new Error("Unknown byte order");
    if(!Array.isArray(input)||!input.length||input.some(v=>!Number.isInteger(v)||v<0||v>255))return [];
    const bytes=Uint8Array.from(input),size=bytes.length,rows=[],suffix=order==="little"?"":"."+(order==="big"?"be":"pdp");
    const add=(name,value,endian=false)=>rows.push({name:name+(endian&&size>1?suffix:""),...(typeof value==="object"?value:{value:String(value)})});
    const controls={0:"nul",8:"backspace",9:"tab",10:"line feed",11:"vertical tab",12:"form feed",13:"carriage return",27:"escape",32:"space",127:"delete"};
    const quoted=value=>Array.from(value,char=>{
      const code=char.codePointAt(0);
      if(Object.hasOwn(controls,code))return "("+controls[code]+")";
      if(code<32 || code>=128 && code<160)return "(control U+"+code.toString(16).padStart(4,"0")+")";
      return "('"+char+"')";
    }).join(" ");
    if([1,2,4,8,16].includes(size)) {
      const value=integer(bytes,order),bits=BigInt(size*8),sign=1n<<(bits-1n);
      add("u"+size*8,value.toString(),true);add("i"+size*8,(value&sign?value-(1n<<bits):value).toString(),true);
      add("Hex","0x"+value.toString(16).padStart(size*2,"0"),true);
      add("Binary",value.toString(2).padStart(size*8,"0"),true);
    }
    if(size===1) {
      add("Char (default)",bytes[0]<128?quoted(String.fromCharCode(bytes[0])):"(byte 0x"+bytes[0].toString(16).padStart(2,"0")+"; not ASCII)");
      if(bytes[0]<2)add("Bool",bytes[0]?"true":"false");
    }
    if(bytes.every(n=>n<128))add("ASCII",quoted(Array.from(bytes,byte=>String.fromCharCode(byte)).join("")));
    function decode(label,encoding) {try{const text=new TextDecoder(encoding,{fatal:true,ignoreBOM:true}).decode(bytes);add(label,quoted(text));return text;}catch(_){add(label,"(invalid or incomplete "+label+"; inspect Hex bytes)");return undefined;}}
    decode("UTF-8","utf-8");
    if(size%2===0) {
      const text=decode("UTF-16"+suffix,order==="big"?"utf-16be":"utf-16le");
      if(size===2 && text!==undefined)add("Char16",quoted(text),true);
    }
    if(size%4===0) {
      const points=[];let valid=true;
      for(let i=0;i<size;i+=4){const point=Number(integer(bytes.slice(i,i+4),order));if(point>0x10ffff||point>=0xd800&&point<=0xdfff){valid=false;break;}points.push(point);}
      if(valid){const text=quoted(points.map(point=>String.fromCodePoint(point)).join(""));add("UTF-32"+suffix,text);if(size===4){add("Wchar (default)",text,true);add("Char32",text,true);}}
    }
    const types=size===1?[["FP8 E4M3FN",4,3,4,true],["FP8 E5M2",5,2,4]]:size===2?[["Float16",5,10,5],["BF16",8,7,4]]:
      size===4?[["Float32",8,23,9]]:size===8?[["Float64",11,52,17]]:size===10?[["Float80",15,63,21,false,true]]:size===16?[["Float128",15,112,36]]:[];
    if(types.length){const bits=integer(bytes,order);for(const [name,e,f,p,fn,explicit] of types){const result=binary(bits,e,f,precision||p,fn,explicit,rounding);add(name,result,true);add(name+" hex",{value:result.hexfloat||result.value,exact:result.exact},true);}}
    if([4,8,16].includes(size)){const bits=integer(bytes,order);for(const encoding of ["BID","DPD"])add("Decimal"+size*8+" "+encoding,decimal(bits,size*8,encoding,precision,rounding),true);}
    // LEB is ordered by its own encoding, not the numeric byte-order selector.
    if(size<=19 && bytes.every((n,i)=>i===size-1?!(n&128):!!(n&128))) {
      let value=0n;for(let i=0;i<size;i++)value|=BigInt(bytes[i]&127)<<BigInt(i*7);
      if(value<1n<<128n)add("ULEB128",value.toString());
      const signed=bytes[size-1]&64?value-(1n<<BigInt(size*7)):value;
      if(signed>=-(1n<<127n)&&signed<1n<<127n)add("SLEB128",signed.toString());
    }
    // Packed BCD is a byte-sequence encoding, independent of numeric endianness.
    if(bytes.every(n=>(n>>4)<10&&(n&15)<10))add("BCD",Array.from(bytes,n=>String(n>>4)+String(n&15)).join(""));
    return rows;
  }
  return {interpret,integer,binary,decimal,dpd,ROUNDING};
});
