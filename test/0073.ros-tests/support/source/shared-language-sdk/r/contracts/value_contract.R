# Pure value candidate. Host R is distinct from a real Wasm/ROS result.
args <- commandArgs(trailingOnly = TRUE)
stopifnot(length(args) == 1L)
source(file.path(args[1L], "roswasm", "wire.R"), local = environment())
source(file.path(args[1L], "bounded_records.R"), local = environment())
wire <- roswasm_wire()
types <- roswasm_generated(wire)
checks <- 0L
check <- function(ok) { checks <<- checks + 1L; if (!isTRUE(ok)) stop(paste("R value assertion", checks), call. = FALSE) }
refuses <- function(action) { failed <- FALSE; tryCatch(action(), error = function(e) failed <<- TRUE); check(failed) }
scalar <- types$Ros2Managed_fixtureMsgScalars
value <- scalar$new(list(flag = TRUE, u8 = 255, i8 = -128, u16 = 65535, i16 = -32768,
  u32 = 4294967295, i32 = -2147483648,
  u64 = wire$from_decimal("18446744073709551615", FALSE),
  i64 = wire$from_decimal("-9223372036854775808", TRUE), f32 = -0.0, f64 = 1.25, text = "π"))
hex <- paste0("01ff80ffff0080ffffffff00000080ffffffffffffffff0000000000000080",
              "00000080000000000000f43f02000000cf80")
golden <- as.raw(strtoi(substring(hex, seq(1L, nchar(hex), 2L), seq(2L, nchar(hex), 2L)), base = 16L))
check(identical(scalar$encode(value), golden))
decoded <- scalar$decode(golden)
check(wire$to_decimal(decoded$u64) == "18446744073709551615")
check(wire$to_decimal(decoded$i64) == "-9223372036854775808")
check(decoded$i32 == -2147483648 && !is.na(decoded$i32))
check(decoded$text == "π" && identical(writeBin(decoded$f32, raw(), size = 4L, endian = "little"), as.raw(c(0,0,0,128))))
refuses(function() wire$from_decimal("18446744073709551616", FALSE))
refuses(function() wire$from_decimal("-9223372036854775809", TRUE))
refuses(function() scalar$decode(c(as.raw(2L), golden[-1L])))
refuses(function() scalar$decode(c(golden, as.raw(0L))))
refuses(function() scalar$decode(golden[-length(golden)]))
value$f32 <- 1e100; refuses(function() scalar$encode(value))
outer <- types$Ros2Managed_fixtureMsgOuter
composite <- outer$new()
check(composite$rotation$w == 1 && composite$rotations[[2L]]$w == 1)
composite$fixed[[2L]]$scalar$u64 <- wire$from_decimal("18446744073709551615", FALSE)
copy <- outer$decode(outer$encode(composite))
check(wire$to_decimal(copy$fixed[[2L]]$scalar$u64) == "18446744073709551615")
copy$fixed[[2L]]$scalar$u64 <- wire$from_decimal("0", FALSE)
check(wire$to_decimal(composite$fixed[[2L]]$scalar$u64) == "18446744073709551615")
composite$items <- lapply(seq_len(3L), function(i) types$Ros2Managed_fixtureMsgInner$new())
refuses(function() outer$encode(composite))
check(is.null(scalar$native_identity))
cat(paste0("PASS_R_VALUE_ONLY_CHECKS=", checks, ";ROS=NOT_RUN\n"))
