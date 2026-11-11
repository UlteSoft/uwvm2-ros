# Memory value inspection

A Memory window reads a bounded, authenticated stopped snapshot. Click a byte
with full permission to edit it inline: Enter writes, Escape or leaving the
input cancels. Drag across bytes, or use Shift-click / Shift-arrow, to inspect a
selection. Read-only can select and interpret but cannot write. Limited and none
do not receive these bytes. Formatting performs no additional runtime requests.

**Row bytes** offers 1, 2, 4, 8, 16, 32 and 64 bytes. This controls row layout;
the existing page-size selector controls the number of copied bytes. Unaligned
rows retain the exact address and allocation-tag granule of every byte.

## Types

Only types matching the complete selection appear. Fixed-width values never
silently consume a prefix or pad a short selection.

| Selected bytes | Fixed-width interpretations |
| --- | --- |
| 1 | i8, u8, Char (default), FP8 E4M3FN, FP8 E5M2; Bool for 0 or 1 |
| 2 | i16, u16, Float16, BF16, Char16 when valid |
| 4 | i32, u32, Float32, Decimal32 BID/DPD, Wchar (default) and Char32 when valid |
| 8 | i64, u64, Float64, Decimal64 BID/DPD |
| 10 | Float80: the packed x87 80-bit format |
| 16 | i128, u128, Float128, Decimal128 BID/DPD |

Hex and binary numeric views accompany supported integer widths. Variable-width
interpretations include ASCII, UTF-8, UTF-16, UTF-32, unsigned/signed LEB128 up to
128-bit values, and unsigned packed BCD. Invalid, unfinished or trailing-byte
encodings are omitted. UTF strings require complete code units and valid Unicode
scalar values. LEB must consume the entire selection; its encoded bit order is
independent of the numeric byte-order control.

Char (default) denotes one 8-bit C character code unit; non-ASCII bytes use an
explicit hexadecimal escape rather than an invented Unicode character. Wchar
(default) denotes the Wasm C ABI's 32-bit Unicode scalar. These are labelled
interpretations, not an inference about the selected object's source-language
type. Char16 is available for a valid two-byte character. ABI padding is not
silently removed: a 16-byte selection is not guessed to be a padded x87 value.

## Byte order and precision

The default is little-endian, with no suffix. Big-endian fields have `.be`;
PDP fields have `.pdp`. The PDP convention is high-order 16-bit words first,
with little-endian bytes inside each word. For example, `34 12 78 56` is
`0x12345678` in PDP order. The same word convention extends to wider numbers.
For UTF-32 it applies within each code point; UTF-16 code units in PDP order
match little-endian units. UTF-8, ASCII, BCD and LEB retain their encoding order.

Integer and floating-point decoding uses integer bit patterns, including for
Float80/128. Every binary floating-point row is accompanied by an unconditional
**hex** row containing its exact Hexfloat value. A rounded decimal presentation has an **≈** prefix; hovering the
value provides its exact hexadecimal significand and binary exponent. Decimal
BID/DPD values retain the exact coefficient and decimal exponent in the value's
hover. Signed zero, infinity and quiet/signaling NaNs are distinguished. NaN
payloads remain inspectable. Unsupported x87 bit patterns are labelled rather
than assigned a fabricated number. Non-canonical BID coefficients decode to
zero, with that fact identified in the exact-value description.

## Display rounding

The **Rounding** selector defaults to `nearest_to_even` and follows the ten
explicit policies in the bundled fast_io `floating_rounding` enum. It excludes
`current_environment`; displaying a value never depends on the IDE/libc floating
point environment. **Digits** sets significant decimal digits from 1 to 36;
Auto chooses the format's round-trip precision (decimal formats keep their full
coefficient). This is output formatting: it does not change the copied bytes or
the VM's arithmetic rounding environment. Hexfloat remains exact in every mode.

| Policy | Inexact decimal output |
| --- | --- |
| nearest_to_even | Nearest; midpoint chooses an even final digit |
| nearest_to_odd | Nearest; midpoint chooses an odd final digit |
| nearest_toward_plus_infinity | Nearest; midpoint toward +infinity |
| nearest_toward_minus_infinity | Nearest; midpoint toward -infinity |
| nearest_toward_zero | Nearest; midpoint toward zero |
| nearest_away_from_zero | Nearest; midpoint away from zero |
| toward_plus_infinity | Every inexact value toward +infinity |
| toward_minus_infinity | Every inexact value toward -infinity |
| toward_zero | Every inexact value toward zero |
| away_from_zero | Every inexact value away from zero |

## Format references

- [FP8 specification paper](https://arxiv.org/abs/2209.05433) and
  [ONNX FP8 formats](https://onnx.ai/onnx/technical/float8.html) define the
  E4M3FN and E5M2 interpretations.
- [Oracle numerical computation guide](https://docs.oracle.com/cd/E19957-01/806-3568/ncg_math.html)
  documents binary floating-point fields and x87 unsupported/pseudo-denormal
  patterns.
- [Decimal encoding specification](https://speleotrove.com/decimal/dbspec.html)
  and [DPD encoding](https://speleotrove.com/decimal/DPDecimal.html) describe
  decimal field layout and densely packed digits.
- [GCC libbid unpacking reference](https://github.com/gcc-mirror/gcc/blob/master/libgcc/config/libbid/bid_internal.h)
  provides an independent reference for BID special and non-canonical values.
