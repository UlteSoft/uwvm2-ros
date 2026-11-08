module;
#include <span>
#include <llvm/IR/IRBuilder.h>
export module uwvm2.runtime.compiler.llvm_jit.compile_all_from_uwvm.translate.memtag_disabled_ir;
import uwvm2.extension.memtag.instruction;
#define UWVM_MODULE
#define UWVM_MODULE_EXPORT export
#include "memtag_disabled_ir.h"
#undef UWVM_MODULE_EXPORT
#undef UWVM_MODULE
