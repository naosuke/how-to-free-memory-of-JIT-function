##===- examples/how-to-free-memory-of-JIT-function/Makefile ---------------===##
# 
# a test for freeing memory of JIT'd function
# 
##===----------------------------------------------------------------------===##

LEVEL = ../..
TOOLNAME = how-to-free-memory-of-JIT-function
EXAMPLE_TOOL = 1

# Link in JIT support
LINK_COMPONENTS := jit interpreter nativecodegen asmparser

include $(LEVEL)/Makefile.common
