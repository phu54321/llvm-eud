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

#include "EUDSubtarget.h"
#include "llvm/Target/TargetMachine.h"

namespace llvm {
class EUDTargetMachine : public LLVMTargetMachine {
  std::unique_ptr<TargetLoweringObjectFile> TLOF;
  EUDSubtarget Subtarget;

public:
  EUDTargetMachine(const Target &T, const Triple &TT, StringRef CPU,
                   StringRef FS, const TargetOptions &Options,
                   Optional<Reloc::Model> RM, Optional<CodeModel::Model> CM,
                   CodeGenOpt::Level OL, bool JIT);

  const EUDSubtarget *getSubtargetImpl() const { return &Subtarget; }
  const EUDSubtarget *getSubtargetImpl(const Function &) const override {
    return &Subtarget;
  }

  TargetPassConfig *createPassConfig(PassManagerBase &PM) override;

  TargetLoweringObjectFile *getObjFileLowering() const override {
    return TLOF.get();
  }
};
}

#endif
