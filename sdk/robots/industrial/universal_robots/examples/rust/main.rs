//! Real WASI command; argv supplies one nonempty policy alias.
use uwvm_ur_example::read_once;
fn main(){let args:Vec<_>=std::env::args().collect();if args.len()!=2||args[1].is_empty(){std::process::exit(22)}
let r=read_once(&args[1]);let code=if r.primary!=0{r.primary}else{r.cleanup};std::process::exit(if code<126{code as i32}else{125})}
