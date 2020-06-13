cmake_minimum_required(VERSION 3.16)
project(dummy-kaleidoscope)

set(CMAKE_CXX_STANDARD 17)

include_directories(/usr/local/opt/llvm/include)
include_directories(/usr/local/opt/llvm/lib)

set(CMAKE_CXX_FLAGS "-I/usr/local/Cellar/llvm/10.0.0_3/include -std=c++14 -stdlib=libc++ -D__STDC_CONSTANT_MACROS -D__STDC_FORMAT_MACROS -D__STDC_LIMIT_MACROS -L/usr/local/Cellar/llvm/10.0.0_3/lib -Wl,-search_paths_first -Wl,-headerpad_max_install_names -lLLVMXRay -lLLVMWindowsManifest -lLLVMTableGen -lLLVMSymbolize -lLLVMDebugInfoPDB -lLLVMOrcJIT -lLLVMOrcError -lLLVMJITLink -lLLVMObjectYAML -lLLVMMCA -lLLVMLTO -lLLVMPasses -lLLVMObjCARCOpts -lLLVMLineEditor -lLLVMLibDriver -lLLVMInterpreter -lLLVMFuzzMutate -lLLVMFrontendOpenMP -lLLVMMCJIT -lLLVMExecutionEngine -lLLVMRuntimeDyld -lLLVMDWARFLinker -lLLVMDlltoolDriver -lLLVMOption -lLLVMDebugInfoGSYM -lLLVMCoverage -lLLVMCoroutines -lLLVMXCoreDisassembler -lLLVMXCoreCodeGen -lLLVMXCoreDesc -lLLVMXCoreInfo -lLLVMX86Disassembler -lLLVMX86AsmParser -lLLVMX86CodeGen -lLLVMX86Desc -lLLVMX86Utils -lLLVMX86Info -lLLVMWebAssemblyDisassembler -lLLVMWebAssemblyCodeGen -lLLVMWebAssemblyDesc -lLLVMWebAssemblyAsmParser -lLLVMWebAssemblyInfo -lLLVMSystemZDisassembler -lLLVMSystemZCodeGen -lLLVMSystemZAsmParser -lLLVMSystemZDesc -lLLVMSystemZInfo -lLLVMSparcDisassembler -lLLVMSparcCodeGen -lLLVMSparcAsmParser -lLLVMSparcDesc -lLLVMSparcInfo -lLLVMRISCVDisassembler -lLLVMRISCVCodeGen -lLLVMRISCVAsmParser -lLLVMRISCVDesc -lLLVMRISCVUtils -lLLVMRISCVInfo -lLLVMPowerPCDisassembler -lLLVMPowerPCCodeGen -lLLVMPowerPCAsmParser -lLLVMPowerPCDesc -lLLVMPowerPCInfo -lLLVMNVPTXCodeGen -lLLVMNVPTXDesc -lLLVMNVPTXInfo -lLLVMMSP430Disassembler -lLLVMMSP430CodeGen -lLLVMMSP430AsmParser -lLLVMMSP430Desc -lLLVMMSP430Info -lLLVMMipsDisassembler -lLLVMMipsCodeGen -lLLVMMipsAsmParser -lLLVMMipsDesc -lLLVMMipsInfo -lLLVMLanaiDisassembler -lLLVMLanaiCodeGen -lLLVMLanaiAsmParser -lLLVMLanaiDesc -lLLVMLanaiInfo -lLLVMHexagonDisassembler -lLLVMHexagonCodeGen -lLLVMHexagonAsmParser -lLLVMHexagonDesc -lLLVMHexagonInfo -lLLVMBPFDisassembler -lLLVMBPFCodeGen -lLLVMBPFAsmParser -lLLVMBPFDesc -lLLVMBPFInfo -lLLVMARMDisassembler -lLLVMARMCodeGen -lLLVMARMAsmParser -lLLVMARMDesc -lLLVMARMUtils -lLLVMARMInfo -lLLVMAMDGPUDisassembler -lLLVMAMDGPUCodeGen -lLLVMMIRParser -lLLVMipo -lLLVMInstrumentation -lLLVMVectorize -lLLVMLinker -lLLVMIRReader -lLLVMAsmParser -lLLVMAMDGPUAsmParser -lLLVMAMDGPUDesc -lLLVMAMDGPUUtils -lLLVMAMDGPUInfo -lLLVMAArch64Disassembler -lLLVMMCDisassembler -lLLVMAArch64CodeGen -lLLVMCFGuard -lLLVMGlobalISel -lLLVMSelectionDAG -lLLVMAsmPrinter -lLLVMDebugInfoDWARF -lLLVMCodeGen -lLLVMTarget -lLLVMScalarOpts -lLLVMInstCombine -lLLVMAggressiveInstCombine -lLLVMTransformUtils -lLLVMBitWriter -lLLVMAnalysis -lLLVMProfileData -lLLVMObject -lLLVMTextAPI -lLLVMBitReader -lLLVMCore -lLLVMRemarks -lLLVMBitstreamReader -lLLVMAArch64AsmParser -lLLVMMCParser -lLLVMAArch64Desc -lLLVMMC -lLLVMDebugInfoCodeView -lLLVMDebugInfoMSF -lLLVMBinaryFormat -lLLVMAArch64Utils -lLLVMAArch64Info -lLLVMSupport -lLLVMDemangle -lz -lcurses -lm -lxml2")

add_executable(toycompile
    main.cpp
    lexer.cpp
    parser.cpp
    ast.cpp
    codegen.cpp
)