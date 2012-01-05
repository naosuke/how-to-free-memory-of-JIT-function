set(LLVM_LINK_COMPONENTS jit interpreter nativecodegen asmparser)

add_llvm_example(how-to-free-memory-of-JIT-function
  main.cpp
  )
