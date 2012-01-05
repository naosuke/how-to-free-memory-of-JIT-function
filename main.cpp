//===--- examples/how-to-free-memory-of-JIT-function/main.cpp -------------===//
//
//        a test for freeing memory of JIT'd function
//
//===----------------------------------------------------------------------===//

//===--- examples/how-to-free-memory/main.cpp - An example use of the JIT -----===//
//
// Test for freeing memory of JIT'd function
//
//===----------------------------------------------------------------------===//
 
#include "llvm/LLVMContext.h"
#include "llvm/Module.h"
#include "llvm/ExecutionEngine/JIT.h"
#include "llvm/Support/TargetSelect.h"
#include "llvm/Support/ManagedStatic.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Support/TargetSelect.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Support/SourceMgr.h"
#include "llvm/Assembly/Parser.h"
 
using namespace llvm;
 
 
// function_assembly is the result from the demo page (http://llvm.org/demo/index.cgi)
// for this:
//     int factorial(int X) {
//      if (X == 0) return 1;
//      return X*factorial(X-1);
//     }
static const char *const function_assembly =
  "define i32 @factorial(i32 %X) nounwind uwtable {\n"
  "  %1 = alloca i32, align 4\n"
  "  %2 = alloca i32, align 4\n"
  "  store i32 %X, i32* %2, align 4\n"
  "  %3 = load i32* %2, align 4\n"
  "  %4 = icmp eq i32 %3, 0\n"
  "  br i1 %4, label %5, label %6\n"
  "\n"
  "; <label>:5                                       ; preds = %0\n"
  "  store i32 1, i32* %1\n"
  "  br label %12\n"
  "\n"
  "; <label>:6                                       ; preds = %0\n"
  "  %7 = load i32* %2, align 4\n"
  "  %8 = load i32* %2, align 4\n"
  "  %9 = sub nsw i32 %8, 1\n"
  "  %10 = call i32 @factorial(i32 %9)\n"
  "  %11 = mul nsw i32 %7, %10\n"
  "  store i32 %11, i32* %1\n"
  "  br label %12\n"
  "\n"
  "; <label>:12                                      ; preds = %6, %5\n"
  "  %13 = load i32* %1\n"
  "  ret i32 %13\n"
  "}\n";
 
int main(int argc, char **argv) {
  int n = argc > 1 ? atol(argv[1]) : 10;

  InitializeNativeTarget();
 
  LLVMContext Context;
 
  Module *M = new Module("test", Context);
  ExecutionEngine* EE = llvm::EngineBuilder(M).setEngineKind(EngineKind::JIT).create();
 
  for (int i = 0; i < n; ++i) {
    SMDiagnostic error;
    ParseAssemblyString(function_assembly,
                        M,
                        error,
                        Context);
 
    Function *func = M->getFunction( "factorial" );
 
    typedef int(*func_t)(int);
    func_t f = (func_t)(uintptr_t)(EE->getPointerToFunction(func));
 
    EE->freeMachineCodeForFunction(func);
    func->eraseFromParent();
  }
 
  delete EE;
 
  llvm_shutdown();
 
  return 0;
}
