//! Actual WASI command entry; the alias must be an explicit argument.
//! A default-zero Runtime returns13 before subscription or dequeue.
fn main() {
    let args:Vec<String>=std::env::args().collect();
    if args.len()!=2 {std::process::exit(22);}
    let result=uwvm_agilex_scout_example::read_once(&args[1]);
    // Applications may inspect result.value's complete fields after success.
    // Exit0 alone is not an independent Native delivery or robot witness.
    std::process::exit(if result.primary<126 {result.primary as i32}else{125});
}
