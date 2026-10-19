// All 38/43 imports remain reachable only through this explicitly exported
// inspection probe. Do not execute valid selector indexes.
pub fn abi_coverage(index : Int) -> Int { @ros.abi_coverage(index) }

fn main {
  if @ros.raw_abi_version() != 1 { abort("ROS ABI version") }
  @stdio.line_ascii("PASS_MOONBIT_RAW_ABI_VERSION_1")
}
