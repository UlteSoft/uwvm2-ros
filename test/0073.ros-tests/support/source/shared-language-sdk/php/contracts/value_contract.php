<?php
declare(strict_types=1);
// Pure value candidate. Host PHP cannot establish a Wasm/ROS result.
if ($argc !== 2) throw new InvalidArgumentException('explicit generated package root required');
require rtrim($argv[1],'/') . '/bounded_records.php';
use Uwvm\RosWasm\Word;
use Uwvm\RosWasm\Generated as T;
$checks = 0;
$check = function(bool $ok) use (&$checks): void { ++$checks; if (!$ok) throw new RuntimeException('PHP value assertion '.$checks); };
$refuses = function(callable $action) use ($check): void {
    $failed = false;
    try { $action(); } catch (InvalidArgumentException|OutOfRangeException|TypeError $error) { $failed = true; }
    $check($failed);
};
$codec = T\codecs(); $symbol = 'Ros2Managed_fixtureMsgScalars';
$v = new T\Ros2Managed_fixtureMsgScalars();
$v->flag=true; $v->u8=255; $v->i8=-128; $v->u16=65535; $v->i16=-32768;
$v->u32=Word::fromDecimal('4294967295',4,false); $v->i32=(-2147483647-1);
$v->u64=Word::fromDecimal('18446744073709551615',8,false);
$v->i64=Word::fromDecimal('-9223372036854775808',8,true);
$v->f32=-0.0; $v->f64=1.25; $v->text='π';
$golden=hex2bin('01ff80ffff0080ffffffff00000080ffffffffffffffff0000000000000080'.'00000080000000000000f43f02000000cf80');
$check($codec->encode($symbol,$v)===$golden);
$d=$codec->decode($symbol,$golden);
$check($d->u64->decimal()==='18446744073709551615' && $d->i64->decimal()==='-9223372036854775808');
$check($d->u32->decimal()==='4294967295' && $d->i32===(-2147483647-1));
$check($d->text==='π' && bin2hex(pack('g',$d->f32))==='00000080');
$refuses(fn()=>Word::fromDecimal('18446744073709551616',8,false));
$refuses(fn()=>Word::fromDecimal('-9223372036854775809',8,true));
$refuses(fn()=>Word::fromDecimal('9223372036854775808',8,true));
$refuses(fn()=>$codec->decode($symbol,"\2".substr($golden,1)));
$refuses(fn()=>$codec->decode($symbol,$golden."\0"));
$refuses(fn()=>$codec->decode($symbol,substr($golden,0,-1)));
$v->text="\xed\xa0\x80"; $refuses(fn()=>$codec->encode($symbol,$v));
$v->text="a\0b"; $refuses(fn()=>$codec->encode($symbol,$v));
$v->text='π'; $v->f32=1e100; $refuses(fn()=>$codec->encode($symbol,$v));
$o=new T\Ros2Managed_fixtureMsgOuter();
$check($o->fixed[0]!==$o->fixed[1]);
$check($o->rotation->w===1.0 && $o->rotations[1]->w===1.0);
$o->fixed[1]->scalar->u64=Word::fromDecimal('18446744073709551615',8,false);
$c=$codec->decode('Ros2Managed_fixtureMsgOuter',$codec->encode('Ros2Managed_fixtureMsgOuter',$o));
$check($c->fixed[1]->scalar->u64->decimal()==='18446744073709551615');
$o->items=[new T\Ros2Managed_fixtureMsgInner(),new T\Ros2Managed_fixtureMsgInner(),new T\Ros2Managed_fixtureMsgInner()];
$refuses(fn()=>$codec->encode('Ros2Managed_fixtureMsgOuter',$o));
$check(T\schemas()[$symbol]['native_identity']===null);
echo 'PASS_PHP_VALUE_ONLY_CHECKS='.$checks.';ROS=NOT_RUN'.PHP_EOL;
