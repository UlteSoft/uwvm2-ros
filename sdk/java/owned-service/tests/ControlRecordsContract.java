package org.uwvm.roswasm;

import java.util.Arrays;
import org.uwvm.roswasm.generated.BoundedRecords.*;
import org.uwvm.roswasm.generated.ControlServices;

/** Future complete emitter-output contracts, not generated/runnable evidence. */
public final class ControlRecordsContract {
    static void check(boolean b){if(!b)throw new AssertionError();}
    static void refuses(Runnable r){boolean refused=false;try{r.run();}catch(IllegalArgumentException e){refused=true;}check(refused);}
    public static void main(String[] args){
        Ros2Rcl_interfacesMsgIntegerRange range=new Ros2Rcl_interfacesMsgIntegerRange();range.from_value=Long.MIN_VALUE;range.to_value=Long.MAX_VALUE;range.step=-1L;
        Ros2Rcl_interfacesMsgIntegerRange rangeCopy=Ros2Rcl_interfacesMsgIntegerRange.CODEC.decode(Ros2Rcl_interfacesMsgIntegerRange.CODEC.encode(range));check(rangeCopy.step==-1L&&rangeCopy.from_value==Long.MIN_VALUE);
        // CONTROL actual IR semantics is empty. Keep canonical u32 separate
        // from original Native Time's independently enforced normalization.
        Ros2Builtin_interfacesMsgTime time=new Ros2Builtin_interfacesMsgTime();time.nanosec=4294967295L;check(Ros2Builtin_interfacesMsgTime.CODEC.decode(Ros2Builtin_interfacesMsgTime.CODEC.encode(time)).nanosec==4294967295L);
        // Original raw source bytes are not constrained to Java/UTF8 text.
        Ros2Type_description_interfacesMsgTypeSource source=new Ros2Type_description_interfacesMsgTypeSource();source.type_name="demo/msg/All";source.encoding="msg";source.raw_file_contents=new byte[]{0,(byte)0xff,(byte)0xc0,10};
        byte[] wire=Ros2Type_description_interfacesMsgTypeSource.CODEC.encode(source);Ros2Type_description_interfacesMsgTypeSource decoded=Ros2Type_description_interfacesMsgTypeSource.CODEC.decode(wire);
        check(Arrays.equals(source.raw_file_contents,decoded.raw_file_contents));wire[wire.length-1]=99;check(decoded.raw_file_contents[3]==10);
        // Ordinary UTF8 fields retain their strict byte domain and NUL refusal.
        source.type_name="bad\0name";refuses(()->Ros2Type_description_interfacesMsgTypeSource.CODEC.encode(source));source.type_name="demo/msg/All";
        byte[] trailing=Arrays.copyOf(Ros2Type_description_interfacesMsgTypeSource.CODEC.encode(source),wire.length+1);refuses(()->Ros2Type_description_interfacesMsgTypeSource.CODEC.decode(trailing));
        Ros2Rcl_interfacesMsgParameterValue p=new Ros2Rcl_interfacesMsgParameterValue();p.type=9;p.integer_value=Long.MIN_VALUE;p.integer_array_value=new long[]{-1L,Long.MIN_VALUE,Long.MAX_VALUE};p.byte_array_value=new int[]{0,255};p.string_array_value=new String[]{"a","b"};
        Ros2Rcl_interfacesMsgParameterValue q=Ros2Rcl_interfacesMsgParameterValue.CODEC.decode(Ros2Rcl_interfacesMsgParameterValue.CODEC.encode(p));check(Arrays.equals(p.integer_array_value,q.integer_array_value)&&q.byte_array_value[1]==255);
        p.byte_array_value=new int[]{256};refuses(()->Ros2Rcl_interfacesMsgParameterValue.CODEC.encode(p));p.byte_array_value=new int[0];p.double_value=Double.NaN;refuses(()->Ros2Rcl_interfacesMsgParameterValue.CODEC.encode(p));
        // A nested aggregate cannot hide behind its parent's larger remainder.
        Ros2Type_description_interfacesMsgTypeDescription description=new Ros2Type_description_interfacesMsgTypeDescription();description.referenced_type_descriptions=new Ros2Type_description_interfacesMsgIndividualTypeDescription[4];
        for(int i=0;i<4;i++){Ros2Type_description_interfacesMsgIndividualTypeDescription t=new Ros2Type_description_interfacesMsgIndividualTypeDescription();t.type_name="n".repeat(128);t.fields=new Ros2Type_description_interfacesMsgField[4];for(int j=0;j<4;j++){t.fields[j]=new Ros2Type_description_interfacesMsgField();t.fields[j].name="f".repeat(64);t.fields[j].default_value="d".repeat(64);}description.referenced_type_descriptions[i]=t;}
        // Explicit small complete roundtrip first; large/truncated wire mutation
        // matrices are owned by the Linux suite, not an all-defaults-only oracle.
        Ros2Type_description_interfacesMsgTypeDescription small=Ros2Type_description_interfacesMsgTypeDescription.CODEC.decode(Ros2Type_description_interfacesMsgTypeDescription.CODEC.encode(description));check(small.referenced_type_descriptions.length==4);
        description.referenced_type_descriptions=new Ros2Type_description_interfacesMsgIndividualTypeDescription[11];
        for(int i=0;i<11;i++){Ros2Type_description_interfacesMsgIndividualTypeDescription t=new Ros2Type_description_interfacesMsgIndividualTypeDescription();t.type_name="n".repeat(255);t.fields=new Ros2Type_description_interfacesMsgField[10];for(int j=0;j<10;j++){t.fields[j]=new Ros2Type_description_interfacesMsgField();t.fields[j].name="f".repeat(64);t.fields[j].default_value="d".repeat(64);t.fields[j].type.nested_type_name="z".repeat(255);}description.referenced_type_descriptions[i]=t;}
        refuses(()->Ros2Type_description_interfacesMsgTypeDescription.CODEC.encode(description));
        check(ControlServices.Ros2Rcl_interfacesSrvDescribeParameters().nativeType.equals("rcl_interfaces/srv/DescribeParameters"));
        check(ControlServices.Ros2Rcl_interfacesSrvGetParameterTypes().requestMaximum>0);
        check(ControlServices.Ros2Rcl_interfacesSrvGetParameters().responseMaximum>0);
        check(ControlServices.Ros2Rcl_interfacesSrvListParameters().requestMaximum>0);
        check(ControlServices.Ros2Rcl_interfacesSrvSetParameters().requestMaximum>0);
        check(ControlServices.Ros2Rcl_interfacesSrvSetParametersAtomically().responseMaximum>0);
        check(ControlServices.Ros2Type_description_interfacesSrvGetTypeDescription().responseMaximum==32768);
        check(Ros2Type_description_interfacesSrvGetTypeDescription_Request.CODEC.decode(new byte[9]).include_type_sources==false);
        check(new Ros2Type_description_interfacesSrvGetTypeDescription_Request().include_type_sources==true);
        // Constants retain original declarations, independent of wire fields.
        check(Ros2Rcl_interfacesMsgParameterType.PARAMETER_NOT_SET==0);
    }
}
