package org.uwvm.roswasm;

import java.nio.ByteBuffer;
import java.nio.ByteOrder;
import java.nio.charset.StandardCharsets;
import java.util.Arrays;

/** Canonical ABI v1 builtins. These bytes are neither CDR nor Java serialization.
 * Every decoder validates the full value. Encoding does not imply a ROS grant.
 */
public final class Codecs {
    private Codecs() {}
    /** Validate UTF-16 before the JDK encoder can replace an unpaired surrogate. */
    public static byte[] utf8(String value) {
        if (value == null) throw new IllegalArgumentException("text");
        for (int i = 0; i < value.length(); ++i) {
            char c = value.charAt(i);
            if (Character.isHighSurrogate(c)) {
                if (++i >= value.length() || !Character.isLowSurrogate(value.charAt(i)))
                    throw new IllegalArgumentException("unpaired surrogate");
            } else if (Character.isLowSurrogate(c)) throw new IllegalArgumentException("unpaired surrogate");
        }
        return value.getBytes(StandardCharsets.UTF_8);
    }
    /** Validate strict UTF-8, including overlong, surrogate and maximum scalar rules. */
    public static String text(byte[] bytes) {
        for (int i = 0; i < bytes.length;) {
            int lead = bytes[i++] & 255, count, code, minimum;
            if (lead < 128) continue;
            if (lead >= 0xc2 && lead <= 0xdf) {count=1;code=lead&31;minimum=128;}
            else if (lead >= 0xe0 && lead <= 0xef) {count=2;code=lead&15;minimum=2048;}
            else if (lead >= 0xf0 && lead <= 0xf4) {count=3;code=lead&7;minimum=65536;}
            else throw new IllegalArgumentException("UTF-8 lead");
            if (count > bytes.length-i) throw new IllegalArgumentException("truncated UTF-8");
            for (int j=0;j<count;++j) {
                int next=bytes[i++]&255;
                if ((next&0xc0)!=0x80) throw new IllegalArgumentException("UTF-8 continuation");
                code=(code<<6)|(next&63);
            }
            if (code<minimum || code>0x10ffff || (code>=0xd800 && code<=0xdfff))
                throw new IllegalArgumentException("UTF-8 scalar");
        }
        return new String(bytes, StandardCharsets.UTF_8);
    }
    /** Bounded LE writer for complete generated vendor records.
     * Capacity grows geometrically within the caller's exact wire maximum;
     * small messages do not eagerly allocate the4MiB protocol maximum. All
     * encoding is guest memory work and creates no native ROS effect.
     */
    public static final class Writer {
        private final int maximum;
        private byte[] data;
        private int size;
        public Writer(int maximum){
            if(maximum<0||maximum>4193786)throw new IllegalArgumentException("wire allocation bound");
            this.maximum=maximum;data=new byte[Math.min(maximum,128)];
        }
        private void ensure(int count){
            if(count<0||count>maximum-size)throw new IllegalArgumentException("wire capacity");
            int required=size+count;
            if(required>data.length)data=Arrays.copyOf(data,Math.min(maximum,Math.max(required,Math.max(16,data.length*2))));
        }
        public Writer raw(byte[] value){
            if(value==null)throw new IllegalArgumentException("bytes");
            ensure(value.length);System.arraycopy(value,0,data,size,value.length);size+=value.length;return this;
        }
        /** Full-width unsigned64 uses long's bit pattern; narrower values check range. */
        public Writer integer(long value,int width,boolean signed){
            if(width!=1&&width!=2&&width!=4&&width!=8)throw new IllegalArgumentException("integer width");
            if(width<8){
                int bits=width*8;
                if(signed?(value<-(1L<<(bits-1))||value>((1L<<(bits-1))-1)):(value<0||value>=(1L<<bits)))
                    throw new IllegalArgumentException("integer range");
            }
            ensure(width);for(int i=0;i<width;++i)data[size++]=(byte)(value>>>(8*i));return this;
        }
        public Writer f32(float value){
            if(!Float.isFinite(value))throw new IllegalArgumentException("finite float32");
            // TeaVM's linear WASI class library lacks Integer.toUnsignedLong.
            // Widen then mask, preserving every bit including negative zero.
            return integer(((long)Float.floatToRawIntBits(value)) & 0xffffffffL,4,false);
        }
        public Writer f64(double value){
            if(!Double.isFinite(value))throw new IllegalArgumentException("finite float64");
            return integer(Double.doubleToRawLongBits(value),8,false);
        }
        public Writer text(String value){
            if(value==null||value.length()>maximum-size-4)throw new IllegalArgumentException("wire text bound");
            byte[] encoded=utf8(value);ensure(4+encoded.length);integer(encoded.length,4,false);return raw(encoded);
        }
        public Writer blob(byte[] value){
            if(value==null)throw new IllegalArgumentException("blob");ensure(4+value.length);integer(value.length,4,false);return raw(value);
        }
        /** Guest-local cursor for nested aggregate bounds; no allocation or Host effect. */
        public int position(){return size;}
        public byte[] finish(){return Arrays.copyOf(data,size);}
    }
    /** Bounded exact wire reader used by codecs and Action/service envelopes. */
    public static final class Reader {
        private final byte[] data;
        private int position;
        public Reader(byte[] data) {
            if(data==null || data.length>4194304)throw new IllegalArgumentException("bounded wire");
            this.data=data;
        }
        public int remaining(){return data.length-position;}
        public byte[] take(int size) {
            if(size<0 || size>remaining())throw new IllegalArgumentException("truncated wire");
            byte[] result=Arrays.copyOfRange(data,position,position+size);position+=size;return result;
        }
        public long unsigned(int size) {
            if(size<1 || size>8 || size>remaining())throw new IllegalArgumentException("integer width/range");
            long value=0;for(int i=0;i<size;++i)value|=((long)data[position++]&255L)<<(8*i);return value;
        }
        public long integer(int size,boolean signed){
            if(size!=1&&size!=2&&size!=4&&size!=8)throw new IllegalArgumentException("integer width");
            long value=unsigned(size);int shift=64-size*8;return signed&&shift!=0?(value<<shift)>>shift:value;
        }
        public float f32(){float value=Float.intBitsToFloat((int)unsigned(4));if(!Float.isFinite(value))throw new IllegalArgumentException("nonfinite float32");return value;}
        public double f64(){double value=Double.longBitsToDouble(unsigned(8));if(!Double.isFinite(value))throw new IllegalArgumentException("nonfinite float64");return value;}
        public String text() {
            long size=unsigned(4);
            if(size>remaining())throw new IllegalArgumentException("text length");
            return Codecs.text(take((int)size));
        }
        public void end(){if(remaining()!=0)throw new IllegalArgumentException("trailing bytes");}
    }
    /** std_msgs/String; preserves embedded NUL and leading BOM. */
    public static final Codec<String> STRING=new Codec<String>() {
        public byte[] encode(String value) {
            if(value==null || value.length()>4193782)throw new IllegalArgumentException("String bound");
            byte[] data=utf8(value);
            if(data.length>4193782)throw new IllegalArgumentException("String UTF-8 bound");
            return ByteBuffer.allocate(4+data.length).order(ByteOrder.LITTLE_ENDIAN).putInt(data.length).put(data).array();
        }
        public String decode(byte[] data){Reader r=new Reader(data);String value=r.text();r.end();return value;}
    };
    /** Canonical Bool, also the SetBool request. */
    public static final Codec<Boolean> BOOL=new Codec<Boolean>() {
        public byte[] encode(Boolean value){if(value==null)throw new IllegalArgumentException("Bool");return new byte[]{(byte)(value?1:0)};}
        public Boolean decode(byte[] data){if(data==null||data.length!=1||(data[0]!=0&&data[0]!=1))throw new IllegalArgumentException("Bool wire");return data[0]==1;}
    };
    /** Complete Twist linear XYZ then angular XYZ; finite IEEE754 binary64. */
    public static final Codec<double[]> TWIST=new Codec<double[]>() {
        public byte[] encode(double[] value){
            if(value==null||value.length!=6)throw new IllegalArgumentException("six Twist fields");
            ByteBuffer b=ByteBuffer.allocate(48).order(ByteOrder.LITTLE_ENDIAN);
            for(double item:value){if(!Double.isFinite(item))throw new IllegalArgumentException("nonfinite Twist");b.putDouble(item);}return b.array();
        }
        public double[] decode(byte[] data){
            if(data==null||data.length!=48)throw new IllegalArgumentException("Twist length");
            double[] value=new double[6];ByteBuffer b=ByteBuffer.wrap(data).order(ByteOrder.LITTLE_ENDIAN);
            for(int i=0;i<6;++i){value[i]=b.getDouble();if(!Double.isFinite(value[i]))throw new IllegalArgumentException("nonfinite Twist");}return value;
        }
    };
    /** Exact i64 payload. All long bit patterns are retained. */
    public static final Codec<Long> INT64=new Codec<Long>() {
        public byte[] encode(Long value){if(value==null)throw new IllegalArgumentException("i64");return ByteBuffer.allocate(8).order(ByteOrder.LITTLE_ENDIAN).putLong(value).array();}
        public Long decode(byte[] data){Reader r=new Reader(data);long value=r.unsigned(8);r.end();return value;}
    };
    /** Empty Trigger request. Void is represented by null, with zero wire bytes. */
    public static final Codec<Void> TRIGGER=new Codec<Void>() {
        public byte[] encode(Void value){return new byte[0];}
        public Void decode(byte[] data){if(data==null||data.length!=0)throw new IllegalArgumentException("empty Trigger");return null;}
    };
    /** Owning Trigger/SetBool response value. */
    public static final class Response {
        public final boolean success;public final String message;
        public Response(boolean success,String message){this.success=success;this.message=message;}
    }
    public static final Codec<Response> SERVICE_RESPONSE=new Codec<Response>() {
        public byte[] encode(Response value){
            if(value==null)throw new IllegalArgumentException("service response");
            byte[] text=STRING.encode(value.message);
            if(text.length>4193785)throw new IllegalArgumentException("service response bound");
            byte[] data=new byte[1+text.length];data[0]=(byte)(value.success?1:0);System.arraycopy(text,0,data,1,text.length);return data;
        }
        public Response decode(byte[] data){Reader r=new Reader(data);boolean success=BOOL.decode(r.take(1));String text=r.text();r.end();return new Response(success,text);}
    };
    /** Prefix one exact nonzero owned service/Action ID to a complete body. */
    public static byte[] prefixId(long id,byte[] body){
        if(id==0 || body==null || body.length>4193778)throw new IllegalArgumentException("owned ID/body bound");
        return ByteBuffer.allocate(8+body.length).order(ByteOrder.LITTLE_ENDIAN).putLong(id).put(body).array();
    }
}
