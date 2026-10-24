# Pure value candidate. Host execution is not Wasm/ROS evidence.
raise ArgumentError, 'explicit generated package root required' unless ARGV.length == 1
require File.join(File.expand_path(ARGV[0]), 'bounded_records.rb')
types = Uwvm::RosWasm::Generated
scalar = types::Ros2Managed_fixtureMsgScalars
checks = 0
check = ->(ok) { checks += 1; raise "Ruby value assertion #{checks}" unless ok }
refuses = lambda do |&action|
  failed = false
  begin action.call; rescue ArgumentError, RangeError; failed = true; end
  check.call(failed)
end
value = scalar.new('flag'=>true,'u8'=>255,'i8'=>-128,'u16'=>65_535,'i16'=>-32_768,
  'u32'=>(1<<32)-1,'i32'=>-(1<<31),'u64'=>(1<<64)-1,'i64'=>-(1<<63),
  'f32'=>-0.0,'f64'=>1.25,'text'=>'π')
golden = ['01ff80ffff0080ffffffff00000080ffffffffffffffff0000000000000080' \
          '00000080000000000000f43f02000000cf80'].pack('H*')
check.call(scalar.encode(value) == golden)
decoded = scalar.decode(golden)
check.call(decoded.u64 == (1<<64)-1 && decoded.i64 == -(1<<63))
check.call(decoded.text == 'π' && 1.0/decoded.f32 == -Float::INFINITY)
refuses.call { scalar.decode("\2".b+golden.byteslice(1..)) }
refuses.call { scalar.decode(golden+"\0") }
refuses.call { scalar.decode(golden.byteslice(0...-1)) }
value.text = "\xed\xa0\x80".b
refuses.call { scalar.encode(value) }
value.text = "a\0b"
refuses.call { scalar.encode(value) }
value.text = 'π'; value.u64 = (1<<64)-1.0
refuses.call { scalar.encode(value) }
value.u64 = (1<<64)-1; value.f32 = 1e100
refuses.call { scalar.encode(value) }
outer = types::Ros2Managed_fixtureMsgOuter
composite = outer.new
check.call(composite.fixed[0].object_id != composite.fixed[1].object_id)
check.call(composite.rotation.w == 1.0 && composite.rotations[1].w == 1.0)
composite.fixed[1].scalar.u64 = (1<<64)-1
copy = outer.decode(outer.encode(composite))
check.call(copy.fixed[1].scalar.u64 == (1<<64)-1)
copy.fixed[1].scalar.u64 = 0
check.call(composite.fixed[1].scalar.u64 == (1<<64)-1)
composite.items = Array.new(3) { types::Ros2Managed_fixtureMsgInner.new }
refuses.call { outer.encode(composite) }
check.call(scalar::NATIVE_IDENTITY.nil?)
puts "PASS_RUBY_VALUE_ONLY_CHECKS=#{checks};ROS=NOT_RUN"
