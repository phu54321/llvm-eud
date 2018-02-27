add_llvm_target(EUDCodeGen
  EUDSubtarget.cpp
  EUDTargetMachine.cpp
  )

add_subdirectory(MCTargetDesc)
add_subdirectory(TargetInfo)
