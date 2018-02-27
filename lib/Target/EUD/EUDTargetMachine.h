//===-- EUDTargetMachine.h - Define TargetMachine for EUD --- C++ ---===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file declares the EUD specific subclass of TargetMachine.
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_LIB_TARGET_EUD_EUDTARGETMACHINE_H
#define LLVM_LIB_TARGET_EUD_EUDTARGETMACHINE_H

#include "llvm/Target/TargetMachine.h"
#include "llvm/CodeGen/TargetLowering.h"
#include "llvm/CodeGen/TargetSubtargetInfo.h"

namespace llvm {

// TargetLowering impl
class EUDTargetLowering : public TargetLowering {
public:
  explicit EUDTargetLowering(const TargetMachine &TM)
    : TargetLowering(TM) {}
};


// Subtarget impl
class EUDSubtarget : public TargetSubtargetInfo {
  EUDTargetLowering TLInfo;

public:
  EUDSubtarget(const TargetMachine &TM, const Triple &TT);
  const EUDTargetLowering *getTargetLowering() const override {
    return &TLInfo;
  }
};


// TargetMachine impl
class EUDTargetMachine : public LLVMTargetMachine {
  EUDSubtarget Subtarget;

public:
  EUDTargetMachine(const Target &T, const Triple &TT,StringRef CPU,
                   StringRef FS, const TargetOptions &Options,
                   Optional<Reloc::Model> RM, CodeModel::Model CM,
                   CodeGenOpt::Level OL);

  const EUDSubtarget *getSubtargetImpl() const { return &Subtarget; }
  const EUDSubtarget *getSubtargetImpl(const Function &) const override {
    return &Subtarget;
  }
};
}

#endif
