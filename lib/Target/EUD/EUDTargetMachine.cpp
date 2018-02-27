//===-- EUDTargetMachine.cpp - Define TargetMachine for EUD ---------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// Implements the info about EUD target spec.
//
//===----------------------------------------------------------------------===//

#include "llvm/Support/TargetRegistry.h"
#include "EUDTargetMachine.h"
#include "EUD.h"

using namespace llvm;

extern "C" void LLVMInitializeEUDTarget() {
  // Register the target.
  RegisterTargetMachine<EUDTargetMachine> X(getEUDTarget());
}

// DataLayout: little or big endian
static std::string computeDataLayout(const Triple &TT) {
  return "e-m:e-p:64:64-i64:64-n32:64-S128";
}

EUDTargetMachine::EUDTargetMachine(const Target &T, const Triple &TT,
                                   StringRef CPU, StringRef FS,
                                   const TargetOptions &Options,
                                   Optional<Reloc::Model> RM,
                                   CodeModel::Model CM,
                                   CodeGenOpt::Level OL)
    : LLVMTargetMachine(T, "e-m:e-p:64:64-i64:64-n32:64-S128", TT, CPU, FS, Options,
                        Reloc::Static, CM, OL),
      Subtarget(*this, TT) {
  initAsmInfo();
}
