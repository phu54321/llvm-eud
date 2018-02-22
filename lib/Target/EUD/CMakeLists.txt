set(LLVM_TARGET_DEFINITIONS EUD.td)

tablegen(LLVM EUDGenRegisterInfo.inc -gen-register-info)
tablegen(LLVM EUDGenInstrInfo.inc -gen-instr-info)
tablegen(LLVM EUDGenDisassemblerTables.inc -gen-disassembler)
tablegen(LLVM EUDGenAsmWriter.inc -gen-asm-writer)
tablegen(LLVM EUDGenAsmMatcher.inc -gen-asm-matcher)
tablegen(LLVM EUDGenDAGISel.inc -gen-dag-isel)
tablegen(LLVM EUDGenMCCodeEmitter.inc -gen-emitter)
tablegen(LLVM EUDGenCallingConv.inc -gen-callingconv)
tablegen(LLVM EUDGenSubtargetInfo.inc -gen-subtarget)
add_public_tablegen_target(EUDCommonTableGen)

add_llvm_target(EUDCodeGen
  EUDAsmPrinter.cpp
  EUDFrameLowering.cpp
  EUDInstrInfo.cpp
  EUDISelDAGToDAG.cpp
  EUDISelLowering.cpp
  EUDMCInstLower.cpp
  EUDRegisterInfo.cpp
  EUDSubtarget.cpp
  EUDTargetMachine.cpp
  )

add_subdirectory(AsmParser)
add_subdirectory(InstPrinter)
add_subdirectory(TargetInfo)
add_subdirectory(MCTargetDesc)
