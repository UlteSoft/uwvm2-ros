/* Defined Wasm function-table control, with no imported function address. */
static int identity(int value) { return value; }
static int (*volatile selected)(int) = identity;
int main(void) { return selected(0); }
